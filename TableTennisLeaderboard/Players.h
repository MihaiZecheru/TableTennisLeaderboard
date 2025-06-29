#ifndef PLAYERS_H
#define PLAYERS_H

struct Player
{
  const uint8_t id; // The ID from the database
  const char name[9]; // The name from the database (VARCHAR(8)). 8 chars + null terminator
};

const uint8_t SAVED_PLAYER_COUNT = 6;

// TODO: add more players.
/**
 * All the players from the database.
 * Note: this is not synced up. New players have to be added manually.
 */
const Player ALL_SAVED_PLAYERS[SAVED_PLAYER_COUNT] = {
  {10, "Chris"},
  {11, "Daria"},
  {12, "Erin"},
  {13, "Taticu"},
  {14, "TJ"},
  {15, "Wyatt"}
};

#endif