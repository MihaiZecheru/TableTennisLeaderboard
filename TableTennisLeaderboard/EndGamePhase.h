#ifndef END_GAME_PHASE_H
#define END_GAME_PHASE_H

#include "SendGameData.h"
#include "Scoreboards.h"

// The amount of ms that each flash takes in FlashScoreboard()
#define FLASH_DELAY 500

// The amount of ms that each dot in the UploadingScoresMessage animation takes
#define DOT_DELAY 400

/**
 * Flashes the scoreboards & sends the game data
 * over wifi to the scoreboard server
 */
void EndGamePhase(Game* game)
{
  for (uint8_t i = 0; i < 3; i++)
  {
    ShowUploadingScoresMessage(0);
    delay(DOT_DELAY);
    ShowUploadingScoresMessage(1);
    delay(DOT_DELAY);
    ShowUploadingScoresMessage(2);
    delay(DOT_DELAY);
    ShowUploadingScoresMessage(3);
    delay(DOT_DELAY);
  }

  // Send game data to server--keep trying until it's successful, because the game data shouldn't be lost
  String json_payload = game->Serialize();
  while (true)
  {
    bool success = SendGameData(json_payload);
    if (success) break;
    else ShowHttpFailMessage();
  }

  ShowSubmittedMessage();
  delay(1000);

  // Show winner on OLED
  if (game->GetWinner() != nullptr)
  {
    ShowGameOverMessage(game->GetWinner()->name, game->GetP1Score(), game->GetP2Score());
  }

  // Flash scoreboards
  for (uint8_t i = 0; i < 5; i++) {
    ClearScoreboards();
    delay(FLASH_DELAY);
    game->UpdateScoreboards(); // write the numbers back on
    delay(FLASH_DELAY);
  }

  ClearScoreboards();
}

#endif