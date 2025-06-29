#ifndef END_GAME_PHASE_H
#define END_GAME_PHASE_H

#include "SendGameData.h"
#include "Scoreboards.h"

// The amount of ms between flashes and between elipsis update
// Stands for multi-purpose delay
#define MP_DELAY 300

/**
 * Flashes the scoreboards & sends the game data
 * over wifi to the scoreboard server
 */
void EndGamePhase(Game* game) {
  CloseWebsocket();

  // Flash scoreboards & show Uploading scores message simultaneously
  for (uint8_t i = 0; i < 4; i++)
  {
    ClearScoreboards();
    ShowUploadingScoresMessage(0);
    delay(MP_DELAY);
    ShowUploadingScoresMessage(1);
    game->UpdateScoreboards(); // write the numbers back on
    delay(MP_DELAY);
    ShowUploadingScoresMessage(2);
    ClearScoreboards();
    delay(MP_DELAY);
    ShowUploadingScoresMessage(3);
    game->UpdateScoreboards();
    delay(MP_DELAY);
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

  ClearScoreboards();
}

#endif