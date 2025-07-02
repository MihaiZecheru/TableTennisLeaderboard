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
  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("Not connected to WiFi. Cannot send game data.");
    ShowErrorMessage(2);
  }

  HTTPClient http;
  http.begin(UPLOAD_GAME_DATA_API_ENDPOINT, PUBLIC_SSL_CERT_FOR_SERVER);
  http.addHeader("Content-Type", "application/json");
  int response_code = http.POST(data);

  if (response_code != 200)
  {
    Serial.println("HTTP request FAILED!");
    http.end();
    return false; // fail
  }

  http.end();
  return true; // success
}

#endif