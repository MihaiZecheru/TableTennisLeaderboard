#ifndef POPULATE_PLAYERS_ARRAY_H
#define POPULATE_PLAYERS_ARRAY_H

#include "Players.h"
#include "OLED.h"

void PopulateSavedPlayersArray()
{
  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("Not connected to WiFi. Cannot get player data.");
    ShowErrorMessage(5);
  }

  HTTPClient http;
  http.begin(GET_PLAYER_DATA_API_ENDPOINT, PUBLIC_SSL_CERT_FOR_SERVER);
  http.addHeader("Content-Type", "application/json");
  int response_code = http.GET();

  if (response_code != 200)
  {
    Serial.println("HTTP request FAILED - could not get player data!");
    http.end();
    ShowErrorMessage(6);
  }

  String payload = http.getString(); // = "Chris;Daria;Taticu;" etc. Note that it ends in the separator
  http.end();

  if (payload.indexOf(';') == -1)
  {
    Serial.println("Invalid player data received from server");
    ShowErrorMessage(7);
  }

  uint8_t count = 0;
  int index = 0;
  int last_index = 0;
  while ((index = payload.indexOf(PLAYERS_DATA_SEP, last_index)) != -1)
  {
    if (count >= MAX_UINT8) break;
    String player_name = payload.substring(last_index, index);
    player_name.trim();
    uint8_t name_len = player_name.length();
    if (name_len > 8) name_len = 8;
    _ALL_SAVED_PLAYERS[count].id = count + 10; // ID starts with 10
    player_name.toCharArray(_ALL_SAVED_PLAYERS[count].name, name_len + 1);
    last_index = index + 1;
    count++;
  }
  SAVED_PLAYER_COUNT = count;
}

#endif