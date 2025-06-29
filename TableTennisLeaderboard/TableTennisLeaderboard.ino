#include "PreGamePhase.h"
#include "MidGamePhase.h"
#include "EndGamePhase.h"
#include "Game.h"
#include "OLED.h"
#include "Scoreboards.h"
#include "ConfigureRTC.h"
#include "ButtonBoard.h"
#include "Buzzer.h"

void setup()
{
  Serial.begin(9600);
  Serial.println("ESP32 powered on. Initializing...");
  ConfigureRTC();
  initialize_OLED();
  initialize_buzzer();
  initialize_rotary_encoder();
  initialize_scoreboards();
  initialize_button_board();
  Serial.println("\n✅ ESP32 READY\n");
}

void loop()
{
  // P1 can set the score mode (first to 6, 11, or 21)
  // He must also set who's playing using the KY-040 dial
  // This phase ends when P1 selects who both himself and P2 are
  Game* game = PreGamePhase();

  // Listens for P1's button presses and updates the TM1637 scoreboard displays
  // If the user pressed the Reset Button, then reset_called will be true
  bool reset_called = MidGamePhase(game);
  if (reset_called)
  {
    ClearScoreboards();
    delete game;
    return;
  }
  // else: proceed as normal to EndGamePhase

  // Flashes the scoreboards, resets the game, and sends
  // the game data over wifi to the scoreboard server
  EndGamePhase(game);

  // Clean-up
  delete game;
}
