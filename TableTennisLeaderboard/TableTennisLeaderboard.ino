#include "PreGamePhase.h"
#include "MidGamePhase.h"
#include "EndGamePhase.h"
#include "Game.h"
#include "OLED.h"
#include "Scoreboards.h"

void setup()
{
  Serial.begin(115200);
  initialize_OLED();
  initialize_rotary_encoder();
  initialize_scoreboards();

  UpdateScoreboards(5, 6);

  delay(1000000);
}

void loop()
{
  // P1 can set the score mode (first to 6, 11, or 21)
  // He must also set who's playing using the KY-040 dial
  // This phase ends when P1 selects who both himself and P2 are
  Game* game = PreGamePhase();

  // Listens for P1's button presses and updates the TM1637 scoreboard displays
  MidGamePhase(game);

  // Flashes the scoreboards, resets the game, and sends
  // the game data over wifi to the scoreboard server
  EndGamePhase(game);
}
