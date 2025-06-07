#ifndef SEND_GAME_DATA_H
#define SEND_GAME_DATA_H

#include <WiFi.h>
#include <HTTPClient.h>
#include "Secrets.h"
#include "OLED.h"

/**
 * --------------------------------------------------------------------------------------------------------------------------------------------------------- 
 * NOTE: the WiFi is left constantly on for the lifespan of the ESP32 because it was failing to reconnect by the time that it was meant to upload the scores
 * Having WiFi on consumes an additional 70-120mA apprx.
 * Also, the WiFi would have to be on if using the separate big scoreboard (since the actions would have to sent live over WiFi)
 * If you want to change back to turning the WiFi on/off, then you can uncomment the lines below
 * ---------------------------------------------------------------------------------------------------------------------------------------------------------
 */

/**
 * Send the game data to the server over wifi via HTTP.
 * @returns True if scores were successfully uploaded to the server, false if the HTTP request returned an error
 */
bool SendGameData(String data)
{
  // Serial.println(F("\nConnecting to wifi for EndGamePhase..."));
  // WiFi.mode(WIFI_STA);
  // WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  // // Try to connect for 10 seconds max
  // unsigned long start_attempt_time = millis();
  // while (WiFi.status() != WL_CONNECTED && millis() - start_attempt_time < 10000) delay(500);

  if (WiFi.status() == WL_CONNECTED)
  {
    // Serial.println(F("Connected to wifi."));

    HTTPClient http;
    http.begin(UPLOAD_GAME_DATA_API_ENDPOINT, PUBLIC_SSL_CERT_FOR_SERVER);
    http.addHeader("Content-Type", "application/json");
    int response_code = http.POST(data);

    Serial.print(F("HTTP Response code: "));
    Serial.println(response_code);

    if (response_code != 200)
    {
      Serial.println("HTTP request FAILED!");
      http.end();
      // WiFi.disconnect(true);
      return false; // fail
    }

    http.end();
  }
  else
  {
    // Serial.println("Failed to connect to WiFi. Cannot send game data.");
    Serial.println("Not connected to WiFi. Cannot send game data.");
    ShowErrorMessage(2);
  }

  // WiFi.disconnect(true); // WiFi won't be needed again for 5-30m depending on the length of the next game/set
  return true; // success
}

#endif