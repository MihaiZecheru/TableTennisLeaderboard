#ifndef END_GAME_PHASE_H
#define END_GAME_PHASE_H

#include "SendGameData.h"
#include "Scoreboards.h"

// The amount of ms that each flash takes in FlashScoreboard()
#define FLASH_DELAY 750

/**
 * Flashes the scoreboards & sends the game data
 * over wifi to the scoreboard server
 */
void EndGamePhase(Game* game)
{
  // Send game data to server
  String json_payload = game->Serialize();
  SendGameData(json_payload);
  
  // Flash scoreboards
  for (uint8_t i = 0; i < 3; i++) {
    ClearScoreboards();
    delay(FLASH_DELAY);
    game->UpdateScoreboards(); // write the numbers back on
    delay(FLASH_DELAY);
  }

  ClearScoreboards();
}

#endif