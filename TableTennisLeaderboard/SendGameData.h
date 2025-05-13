#ifndef SEND_GAME_DATA_H
#define SEND_GAME_DATA_H

#include <WiFi.h>
#include <HTTPClient.h>
#include "Secrets.h"
#include "OLED.h"

void SendGameData(String data)
{
  Serial.println(F("\nConnecting to wifi for EndGamePhase..."));
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  // Try to connect for 10 seconds max
  unsigned long start_attempt_time = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - start_attempt_time < 10000) delay(500);

  if (WiFi.status() == WL_CONNECTED)
  {
    Serial.println(F("Connected to wifi."));

    HTTPClient http;
    http.begin(UPLOAD_GAME_DATA_API_ENDPOINT, PUBLIC_SSL_CERT_FOR_SERVER);
    http.addHeader("Content-Type", "application/json");
    int response_code = http.POST(data);

    Serial.print(F("HTTP Response code: "));
    Serial.println(response_code);

    if (response_code != 200)
    {
      // Ask user if the upload should be tried again
      bool try_again = PromptRetryOnOLED(response_code);
      if (try_again)
      {
        SendGameData(data);
      }
    }

    http.end();
  }
  else
  {
    Serial.println("Failed to connect to WiFi. Cannot send game data.");
  }

  WiFi.disconnect(true); // WiFi won't be needed again for 5-30m depending on the length of the next game/set
}

#endif