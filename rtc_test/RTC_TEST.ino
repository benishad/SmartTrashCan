// Date and time functions using a DS1302 RTC

#include <RTClib_Johnny.h>

// Init rtc object
//   DS1302 rtc;
//   DS1302 rtc(ce_pin, sck_pin, io_pin);
//
// ce_pin  (RST): default 4
// sck_pin (CLK): default 5
// io_pin  (DAT): default 6
DS1302 rtc;
//DS1302 rtc(4, 5, 6);  //DAT, CLK, RST

// buffer for DateTime.tostr
char buf[20];

void setup() {
  Serial.begin(9600);
  rtc.begin();

  if (rtc.isrunning()) {
    Serial.println("RTC is running!");
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(__DATE__, __TIME__));
  }
}

void loop() {
  DateTime now = rtc.now();
  Serial.println(now.tostr(buf));
  delay(1000);
}
