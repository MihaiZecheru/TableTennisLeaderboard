#ifndef PRE_GAME_PHASE_H
#define PRE_GAME_PHASE_H

#include "Game.h"

uint8_t SelectSetLength();
uint8_t SelectPlayer();

/**
 * P1 first selects the set length (first to 6, 11, or 21) using the dial
 * He then sets who's playing using the KY-040 dial (first himself then his opponent)
 * A game object will be created and returned with the inputted data
 * @returns The created Game object that will represent the set and keep track of set data
*/
Game* PreGamePhase()
{
  uint8_t set_length = SelectSetLength(); // 6, 11, or 21
  uint8_t p1_id = SelectPlayer();
  uint8_t p2_id = SelectPlayer();

  Game* game = new Game(set_length, p1_id, p2_id);
  return game;
}

/**
 * P1 will select the set length using the dial
 * @returns Either 6, 11, or 21.
 */
uint8_t SelectSetLength()
{
  return 0; // TODO: 
}

/**
 * P1 will select a player using the dial
 * @returns The ID of the selected player
 */
uint8_t SelectPlayer()
{
  return 0; // TODO: 
}

#endif