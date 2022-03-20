/*
1. 무게를 측정하여 20g이하면 살충제X
2. 무게를 측정하여 20g이상이면 살충제 O (주기별로)
3. CDS를 설치하여 개폐여부를 확인하여 열려있을때 살충제X ---------------------------------해결
4. 초음파센서를 활용하여 얼마나 차는지 확인 ---------------------------------------------해결
5. DS1302를 활용하여 계절을 파악하고(설정또한 디스플레이로 가능하도록)
6. 살충제가 동작할때는 LED 혹은 부저또한 같이 사용한다
7. 수위센서를 활용하여 살충제양을 파악하고 부족하면 LED혹은 부저로 나타냄
*/
#include <LiquidCrystal_I2C.h>       //LCD 라이브러리
#include <ThreeWire.h>  
#include <RtcDS1302.h>

ThreeWire myWire(4,5,2); // IO, SCLK, CE
RtcDS1302<ThreeWire> Rtc(myWire);

LiquidCrystal_I2C lcd(0x27, 16,2);                               //lcd 주소값 설정

int Bottom_trig = 4;              //1번 초음파센서 핀 4,3 번
int Bottom_echo = 5;

int dis = 5;                      //쓰레기와 센서 최소 거리(cm)

int CDSPin = A0;
int CDS = 0;

float Bottom_duration;
float Bottom_distance;

boolean state = false;

void setup()
{
  Serial.begin(9600);
  lcd.init();                                                   //lcd초기화
  lcd.backlight();                                              //lcd 바탕을 밝게하기
  lcd.print("Start~");

  pinMode(Bottom_trig, OUTPUT);
  pinMode(Bottom_echo, INPUT);
}

void loop(){
  CDS = analogRead(CDSPin);

 /*
  조도센서값 모니터창에 표시 센서를 가리고 열고 하여 센서값에 최대 변화를 확인
  0에 가까울수록 주변이 밝음
  */
  

  if(CDS < 600)            //뚜껑 열림
  {
    Serial.print("CDS = ");
    Serial.println(CDS);
    //Serial.print("살충제X");
  }
  else if(CDS > 600)                      //뚜껑 닫음
  {
    BOTTOM_SONIC();
    //  Serial.print("살충제 치익");
  }
  delay(200);
}

void BOTTOM_SONIC()
{
  digitalWrite(Bottom_trig, HIGH);
  delay(10);
  digitalWrite(Bottom_trig, LOW);
  Bottom_duration = pulseIn(Bottom_echo, HIGH);        //pulseIn함수 (핀, 상태) 핀상태가 바뀌면 그때까지 경과된 시간을 ms단위로 바꿔줌.
  Bottom_distance = ((float)(340 * Bottom_duration) / 10000) / 2;    //계산한 거리를 다시 cm로 바꾸는 코드
  
  
  if(Bottom_distance < dis)
  {
    lcd.setCursor(12,1);                                    //lcd 커서를 두번째 줄 12번째 칸으로 옮김
    lcd.print("FULL");                                      //lcd에 good 표시
    Serial.print("Tdis = ");
    Serial.println(Bottom_distance);
    state = true;
  }
  else if(Bottom_distance < dis*2 && Bottom_distance > dis)
  {
    Serial.print("Mdis = ");
    Serial.println(Bottom_distance);
    state = false;
  }
  else if(Bottom_distance > dis*2)
  {
    Serial.print("Bdis = ");
    Serial.println(Bottom_distance);
    state = false;
  }
  delay(100);
}

