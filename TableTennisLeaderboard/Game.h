#ifndef GAME_H
#define GAME_H

#include <ArduinoJson.h>
#include "PointRecord.h"

#define MAX_POINTS 100

class Game
{
private:
  /**
   * The length of the set that will be played
   */
  const uint8_t set_length;
  /**
   * The ID of player 1
   */
  const uint8_t p1_id;
  /**
   * The ID of player 2
   */
  const uint8_t p2_id;
  /**
   * Keeps track of every point played (who served and who won).
   * This is what will be sent to the server when the set is over.
   */
  PointRecord point_history[MAX_POINTS];
  /**
   * Player 1's live score
   */
  uint8_t p1_score = 0;
  /**
   * Player 2's live score
   */
  uint8_t p2_score = 0;
  /**
   * The total amount of points that have been played (equal to p1_score + p2_score)
   */
  uint8_t point_count = 0;
  /**
   * 1 for Player1. 2 for Player2.
   */
  uint8_t player_serving; // Whoever wins the first point will serve first (first point is for serve and won't count for final score)

public:
  Game(const uint8_t set_length, const uint8_t p1_id, const uint8_t p2_id)
  : set_length(set_length),
    p1_id(p1_id),
    p2_id(p2_id)
  {}

  void AddPointToP1()
  {
    // if (this->point_count == 0)
    // {
    //   this->player_serving = 1;
    //   return;
    // }

    if (this->p1_score == MAX_POINTS) return;
    this->p1_score++;
    this->point_history[this->point_count++] = {this->player_serving, 1}; // 1 is p1
  }

  void AddPointToP2()
  {
    // if (this->point_count == 0)
    // {
    //   this->player_serving = 2;
    //   return;
    // }

    if (this->p2_score == MAX_POINTS) return;
    this->p2_score++;
    this->point_history[this->point_count++] = {this->player_serving, 2}; // 2 is p2
  }

  /**
   * A player has won if he has a number of points equal to the set length and has won by 2 points or more.
   * @returns True if a player has won.
   */
  bool CheckForWin()
  {
    return (p1_score >= set_length || p2_score >= set_length) && abs(p1_score - p2_score) >= 2;
  }

  String Serialize()
  {
    // Estimate needed size (use ArduinoJson Assistant online if needed)
    const size_t capacity = JSON_OBJECT_SIZE(4) + JSON_ARRAY_SIZE(MAX_POINTS) + MAX_POINTS * JSON_OBJECT_SIZE(2) + 200;
    StaticJsonDocument<capacity> doc;

    doc["p1_id"] = p1_id;
    doc["p2_id"] = p2_id;
    doc["set_length"] = set_length;

    JsonArray points = doc.createNestedArray("points");

    for (uint8_t i = 0; i < point_count; i++)
    {
      JsonObject point = points.createNestedObject();
      point["s"] = point_history[i].player_who_served;
      point["w"] = point_history[i].player_who_won;
    }

    String output;
    serializeJson(doc, output);
    return output;
  }
};

#endif