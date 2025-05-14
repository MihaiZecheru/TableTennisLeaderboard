#ifndef MID_GAME_PHASE_H
#define MID_GAME_PHASE_H

#include "OLED.h"

bool P1ButtonPressed();
bool P2ButtonPressed();

/**
 * Listens for P1's button presses and updates the TM1637 scoreboard displays in response
 * until one of the players has won
 */
void MidGamePhase(Game* game)
{
  game->ShowServeInfoMessage();

  while (!game->CheckForWin())
  {
    if (P1ButtonPressed())
    {
      while (P1ButtonPressed());
      game->AddPointToP2();
    }

    if (P2ButtonPressed())
    {
      while (P2ButtonPressed());
      game->AddPointToP2();
    }
  }
}

/**
 * @returns True if the P1 button is currently being pressed
 */
bool P1ButtonPressed()
{
  // TODO:
}

/**
 * @returns True if the P2 button is currently being pressed
 */
bool P2ButtonPressed()
{
  // TODO:
}

#endif