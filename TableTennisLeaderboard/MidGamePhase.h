#ifndef MID_GAME_PHASE_H
#define MID_GAME_PHASE_H

#include "OLED.h"
#include "ButtonBoard.h"

/**
 * Listens for P1's button presses and updates the TM1637 scoreboard displays in response
 * until one of the players has won
 * @returns True if the player pressed the Reset Button, which immediately quits the function and is used to start back at the PreGamePhase
 */
bool MidGamePhase(Game* game)
{
  game->ShowServeInfoMessage();
  game->UpdateScoreboards();

  while (!game->CheckForWin())
  {
    if (P1ButtonPressed())
    {
      delay(100);
      while (P1ButtonPressed());
      game->AddPointToP1();
    }

    if (P2ButtonPressed())
    {
      delay(100);
      while (P2ButtonPressed());
      game->AddPointToP2();
    }

    if (UndoPointButtonPressed())
    {
      delay(100);
      while (UndoPointButtonPressed());
      game->UndoLastPoint();
    }

    if (ResetGameButtonPressed())
    {
      delay(100);
      while (ResetGameButtonPressed());
      return true; // loop() will receive this 'true' value and return, skipping the EndGamePhase and going back to the start of the loop
    }
  }

  return false;
}

#endif