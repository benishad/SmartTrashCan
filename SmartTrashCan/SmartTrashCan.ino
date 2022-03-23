/*
1. 무게를 측정하여 20g이하면 살충제X                                         ---해결
2. 무게를 측정하여 20g이상이면 살충제 O (주기별로)                            ---
3. CDS를 설치하여 개폐여부를 확인하여 열려있을때 살충제X                      ---해결
4. 초음파센서를 활용하여 얼마나 차는지 확인                                   ---해결
5. DS1302를 활용하여 계절을 파악하고(설정또한 디스플레이로 가능하도록)        ---해결
6. 살충제가 동작할때는 LED 혹은 부저또한 같이 사용한다                        ---해결
7. 수위센서를 활용하여 살충제양을 파악하고 부족하면 LED혹은 부저로 나타냄     ---해결
*/
#include <LiquidCrystal_I2C.h>       //LCD 라이브러리
#include <RTClib_Johnny.h>
#include "HX711.h"

#define calibration_factor -7050.0
int DoutPin = 9;
int ClkPin = 8;

HX711 scale;

DS1302 rtc(7, 6, 5);              //RST, CLK, DAT

LiquidCrystal_I2C lcd(0x27, 16,2);                               //lcd 주소값 설정

int Bottom_trig = 4;              //1번 초음파센서 핀 4,3 번
int Bottom_echo = 3;
int ledpin = 8;

int dis = 5;                      //쓰레기와 센서 최소 거리(cm)

int CDSPin = A0;
int cds = 0;
int waterSen = A1;
int water = 0;
int weightOne = 0;

float Bottom_duration;
float Bottom_distance;

boolean state = false;

char buf[20];

void setup()
{
  Serial.begin(9600);
  scale.begin(DoutPin, ClkPin);
  rtc.begin();
  lcd.init();                                                   //lcd초기화
  lcd.backlight();                                              //lcd 바탕을 밝게하기
  lcd.print("Start~");
  if (rtc.isrunning()) {
    Serial.println("RTC is running!");
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(__DATE__, __TIME__));
  }

  scale.set_scale(calibration_factor);
  scale.tare();
  Serial.print("Reading: ");
  Serial.println(scale.get_units(), 1);

  pinMode(Bottom_trig, OUTPUT);
  pinMode(Bottom_echo, INPUT);
  pinMode(ledpin, OUTPUT);
}

void loop()
{
  CDS();
  SEASON_READ();
  MIN();
}

//**************************************덮게가 열린지 판단
void CDS()
{
  cds = analogRead(CDSPin);
  
  if(cds < 600)                 //뚜껑 열림
  {
    Serial.print("CDS = ");
    Serial.println(cds);
  }
  else if(cds > 600)            //뚜껑 닫음
  {
    BOTTOM_SONIC();
    LOADCELL();
    //  Serial.print("살충제 치익");
  }
  delay(200);
}

//*******************************************높이판단
void BOTTOM_SONIC()
{
  digitalWrite(Bottom_trig, HIGH);
  delay(10);
  digitalWrite(Bottom_trig, LOW);
  Bottom_duration = pulseIn(Bottom_echo, HIGH);        //pulseIn함수 (핀, 상태) 핀상태가 바뀌면 그때까지 경과된 시간을 ms단위로 바꿔줌.
  Bottom_distance = ((float)(340 * Bottom_duration) / 10000) / 2;    //계산한 거리를 다시 cm로 바꿔줌
  if(Bottom_distance < dis)
  {
    lcd.setCursor(12,1);
    lcd.print("FULL");
    Serial.print("Tdis = ");
    Serial.println(Bottom_distance);
  }
  else if(Bottom_distance < dis*2 && Bottom_distance > dis)
  {
    lcd.setCursor(12,1);
    lcd.print("MID");
    Serial.print("Mdis = ");
    Serial.println(Bottom_distance);
  }
  else if(Bottom_distance > dis*2)
  {
    lcd.setCursor(12,1);
    lcd.print("LESS");
    Serial.print("Bdis = ");
    Serial.println(Bottom_distance);
  }
  delay(100);
}

//*******************************************계절판단
void SEASON_READ()
{
  DateTime now = rtc.now();
  Serial.println(now.tostr(buf));
  delay(1000);
  //Serial.println(now.month(),DEC);

  if(now.month()==3 || now.month()==4 || now.month()==5)
  {
    Serial.println("봄");
    lcd.setCursor(10,0);
    lcd.print("SPRING");
  }
  else if(now.month()==6 || now.month()==7 || now.month()==8)
  {
    Serial.println("여름");
    lcd.setCursor(10,0);
    lcd.print("SUMMER");
  }
  else if(now.month()==9 || now.month()==10 || now.month()==11)
  {
    Serial.println("가을");
    lcd.setCursor(10,0);
    lcd.print("AUTUMN");
  }
  else if(now.month()==12 || now.month()==1 || now.month()==2)
  {
    
    Serial.println("겨울");
    lcd.setCursor(10,0);
    lcd.print("WINTER");
  }
}
//**************************************알림부부
void BLINKLED()
{
  digitalWrite(ledpin, HIGH);
  delay(200);
  digitalWrite(ledpin, LOW);
  delay(200);
}

void LOADCELL()
{
  DateTime now = rtc.now();
  weightOne = scale.get_units();
  delay(50);
  if(weightOne > 20 && now.minute()==30)
  {
    BLINKLED();
  }
  else
  {
    Serial.println("WAIT...");
  }
}

void WATERLEVEL()
{
  water = analogRead(waterSen);
  Serial.print("water = ");
  Serial.println(water);

  if(water < 500)
  {
    BLINKLED();
  }

}
void MIN()
{
  DateTime now = rtc.now();
  if(now.minute() == 58 && state == true){
    Serial.println("HOHO30MIN~~");
    state = false;
  }
  else if(now.minute() == 57)
  {
    state = true;
  }
}