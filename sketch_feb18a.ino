#define BLYNK_TEMPLATE_ID "TMPL3H5OGT3wh"
#define BLYNK_TEMPLATE_NAME "Medicine Reminder"
#define BLYNK_AUTH_TOKEN "Iy1wVFRX4FBEF7tvoqqD09QvKRsOQoCS"

#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <TimeLib.h>
#include <WidgetRTC.h>

char ssid[] = "Ragini";
char pass[] = "12345678";

WidgetRTC rtc;
BlynkTimer timer;

int reminderHour = -1;
int reminderMinute = -1;
bool notified = false;

BLYNK_WRITE(V1)
{
  TimeInputParam t(param);

  if (t.hasStartTime())
  {
    reminderHour = t.getStartHour();
    reminderMinute = t.getStartMinute();

    Serial.print("Reminder Time Set: ");
    Serial.print(reminderHour);
    Serial.print(":");
    Serial.println(reminderMinute);

    notified = false;  // reset notification flag
  }
}

void checkTime()
{
  if (reminderHour == -1) return;  // time not set

  Serial.print("Current Time: ");
  Serial.print(hour());
  Serial.print(":");
  Serial.println(minute());

  if (hour() == reminderHour && minute() == reminderMinute && !notified)
  {
    Serial.println("Medicine Time!");
    Blynk.logEvent("medicine_alert", "Take medicine now");
    notified = true;   // prevent repeat
  }

  if (minute() != reminderMinute)
  {
    notified = false;  // reset after minute passes
  }
}

void setup()
{
  Serial.begin(9600);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  rtc.begin();
  Blynk.syncVirtual(V1);   // VERY IMPORTANT
  timer.setInterval(30000L, checkTime);  // check every 30 sec
}

void loop()
{
  Blynk.run();
  timer.run();
} 