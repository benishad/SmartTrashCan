/*
1. 무게를 측정 후 
2. 살충제 분사
3-1. ~20g 까지는 살충제 X
3-2. 20g~이면 일정한 주기로 분사 (무게도 고려)
4. CDS로 개폐여부 확인하여 닫혀있을때만 살충제 분사
5. RTC로 시간을 확인 혹은 설정할 수 있게 또한 디스플레이로 조작가능하도록
6. 살충제 분사될떄는 LED로 표현(혹은 부저)
7. 초음파로 쓰레기통 차는 여부 확인
(8. 살충제 양이 부족하면 알려주는 경고등/혹은 부저)
*/
/*
아두이노 나노, DS1302(RTC), CDS
*/
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(500);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(500);                       // wait for a second
}
