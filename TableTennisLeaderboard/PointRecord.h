#ifndef POINT_RECORD_H
#define POINT_RECORD_H

/**
 * Keeps track of one point: who served and who won
 */
struct PointRecord
{
  /**
   * 1 for Player1. 2 for Player2.
   */
  uint8_t player_who_served;
  /**
   * 1 for Player1. 2 for Player2.
   */
  uint8_t player_who_won;
};

#endif