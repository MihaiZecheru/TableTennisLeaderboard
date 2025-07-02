#ifndef PLAYERS_H
#define PLAYERS_H

#include <WiFi.h>
#include <HTTPClient.h>
#include "Secrets.h"

#define MAX_UINT8 255
#define PLAYERS_DATA_SEP ';'

struct Player
{
  uint8_t id; // The ID from the database. Note: starts from 10.
  char name[9]; // The name from the database (VARCHAR(8)). 8 chars + null terminator
};

uint8_t SAVED_PLAYER_COUNT;

/**
 * All the players from the database. Max
 */
Player _ALL_SAVED_PLAYERS[MAX_UINT8];

Player& GetPlayer(uint8_t index)
{
  return _ALL_SAVED_PLAYERS[index];
}

#endif