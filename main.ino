#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <Ticker.h>
#include "server.h"

// CONSTANTS
#define INTERVAL_TO_ON 10
#define INTERVAL_TO_OFF 10


const char* SSID = "";
const char* PASSWORD = "";

// OBJECTS
Ticker timer;
WiFiUDP udp;
NTPClient ntp(udp, "a.st1.ntp.br", -3 * 3600, 60000);
const IPAddress ip(192, 168, 15, 100);
const IPAddress gateway(192, 168, 15, 1);
const IPAddress subnet(255, 255, 255, 0);

// FUNCTION HEADERS
void ICACHE_RAM_ATTR onTime();
void initWiFi();

// VARIABLES
bool status = false;
int minuteOn = 0;
int minute;
int day;
int hour;
unsigned long auxiliar = 0;

void setup() {
  initWiFi();
  setupNTP();
  setupTimer();
  initializeServer();
  Schedule::initializeSchedule();
  digitalWrite(2, HIGH);
  pinMode(0, OUTPUT);
  pinMode(2, OUTPUT);
}

void loop() {
  server.handleClient();
}

void initWiFi() {
  delay(10);
  WiFi.begin(SSID, PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(2, LOW);
    delay(100);
    digitalWrite(2, LOW);
  }

  WiFi.config(ip, gateway, subnet);
}

void setupNTP() {
  ntp.begin();
  ntp.forceUpdate();
}

void checkTime() {
  hour = ntp.getHours();
  day = ntp.getDay();
  minute = ntp.getMinutes();
}

void setupTimer() { 
  timer1_attachInterrupt(onTime); 
  timer1_enable(TIM_DIV256, TIM_EDGE, TIM_SINGLE);
  timer1_write(10000);
}


//ISR to check if it's already time to enable or disable the device
void ICACHE_RAM_ATTR onTime() {
  checkTime();

  if (status == true) {
    if (minute - minuteOn >= INTERVAL_TO_OFF || minute - minuteOn < 0) {
      digitalWrite(0, LOW);
      digitalWrite(2, HIGH);
      status = false;
    }
  } else if (Schedule::schedule[day].hour == hour) {
    if (minute - Schedule::schedule[day].minute < INTERVAL_TO_ON && minute - Schedule::schedule[day].minute > 0) {
      status = true;
      minuteOn = minute;
      digitalWrite(0, HIGH);
      digitalWrite(2, LOW);
    }
  }
  timer1_write(8000000);
}
