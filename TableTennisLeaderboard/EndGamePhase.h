#ifndef END_GAME_PHASE_H
#define END_GAME_PHASE_H

/**
 * Flashes the scoreboards & sends the game data
 * over wifi to the scoreboard server
 */
void EndGamePhase(Game* game)
{
  String json_payload = game->Serialize();
  SendGameData(json_payload);
  // FlashScoreboards(); // TODO: 
  // ClearScoreboards(); // TODO: 
}

#endif