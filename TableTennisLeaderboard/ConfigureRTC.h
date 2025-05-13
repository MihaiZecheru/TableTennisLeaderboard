#ifndef CONFIGURE_RTC_H
#define CONFIGURE_RTC_H

#include <WiFi.h>
#include <time.h>

#include "Secrets.h"

void ConfigureRTC()
{
  Serial.println("Connecting to wifi for RTC sync...");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(250);
  }
  Serial.println("Wifi connected for RTC sync.");

  const char* ntpServer = "pool.ntp.org";
  configTime(0, 0, ntpServer);
  Serial.println("Syncing time with NTP...");

  time_t now = time(nullptr);
  unsigned long start = millis();
  const unsigned long timeout = 10000; // 10 sec timeout

  while (now < 100000 && millis() - start < timeout)
  {
    delay(100);
    now = time(nullptr);
  }

  if (now < 100000)
  {
    Serial.println("❌ Time sync failed!");
  }
  else
  {
    Serial.println("✅ Time synced (UTC): " + String(ctime(&now)));
  }

  WiFi.disconnect(true);
}

#endif