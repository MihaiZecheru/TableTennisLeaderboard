#ifndef GAME_H
#define GAME_H

#include <ArduinoJson.h>
#include <time.h>
#include "PointRecord.h"
#include "GetUTC.h"
#include "Players.h"
#include "OLED.h"
#include "Scoreboards.h"

#define MAX_POINTS 100

class Game
{
private:
  /**
   * The length of the set that will be played
   */
  const uint8_t set_length;
  /**
   * Player 1
   */
  const Player& p1;
  /**
   * Pplayer 2
   */
  const Player& p2;
  /**
   * The time/date the object was created (which represents the start of the game)
   */
  const String started_at;
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
  uint8_t player_serving;

  /**
   * @returns p1 or p2 based on the value of player_serving
   */
  const Player& get_current_server()
  {
    return this->player_serving == 1 ? p1 : p2;
  }

  /**
   * @returns True if the game is currently in deuce, false otherwise
   */
  bool in_deuce()
  {
    return (set_length == 21 && p1_score >= 20 && p2_score >= 20) || (set_length == 11 && p1_score >= 10 && p2_score >= 10);
  }

  /**
   * Calculate how many serves the current server has left
   */
  uint8_t calculate_remaining_serves()
  {
    if (in_deuce() || set_length == 6) // One serve per player during deuce or when playing first to 6
      return 1;
    else if (set_length == 21)
      return 5 - (point_count % 5);
    else if (set_length == 11)
      return 2 - (point_count % 2);
  }

  /**
   * Change the server if it's time for a swap;
   * this is every 5 points for a set of 21,
   * every 2 points for a set of 11,
   * and every point for a set of 6.
   * 
   * Note: this function is called every time a point is added
   */
  void change_server_if_necessary()
  {
    if (in_deuce() || set_length == 6) // Server changes every point during deuce and when playing first to 6
      __change_server();
    else if (set_length == 21 && point_count % 5 == 0)
      __change_server();
    else if (set_length == 11 && point_count % 2 == 0)
      __change_server();
  }

  /**
   * Actually change the server
   */
  void __change_server()
  {
    if (player_serving == 1)
    {
      player_serving = 2;
    }
    else if (player_serving == 2)
    {
      player_serving = 1;
    }
  }

public:
  /**
   * Used to keep track of the game and its history
   * @param set_length The length of the set that will be played (first to 6, 11, or 21). This is the length of the game (set/game are interchangable)
   * @param p1 Player struct representing player #1 in the game
   * @param p2 Player struct representing player #2 in the game
   * @param first_server The number of the player serving first. Either 1 or 2 (representing p1 or p2)
   */
  Game(uint8_t set_length, const Player& p1, const Player& p2, uint8_t first_server)
  : set_length(set_length),
    p1(p1),
    p2(p2),
    player_serving(first_server),
    started_at(GetUTC())
  {}

  /**
   * Increment player 1's score and log the point to point_history.
   * Change the server if necessary and update the OLED, which is displaying the Server Info Message
   */
  void AddPointToP1()
  {
    if (this->p1_score == MAX_POINTS) return;
    this->p1_score++;
    this->point_history[this->point_count++] = {this->player_serving, 1}; // 1 is p1
    
    // Will change_server only if it's time to
    change_server_if_necessary();
    // Update OLED & scoreboards
    this->ShowServeInfoMessage();
    this->UpdateScoreboards();
  }

  /**
   * Increment player 2's score and log the point to point_history.
   * Change the server if necessary and update the OLED, which is displaying the Server Info Message
   */
  void AddPointToP2()
  {
    if (this->p2_score == MAX_POINTS) return;
    this->p2_score++;
    this->point_history[this->point_count++] = {this->player_serving, 2}; // 2 is p2

    // Will change_server only if it's time to
    change_server_if_necessary();
    // Update OLED & scoreboards
    this->ShowServeInfoMessage();
    this->UpdateScoreboards();
  }

  void UpdateScoreboards()
  {
    _update_scoreboards(this->p1_score, this->p2_score);
  }

  /**
   * A player has won if he has a number of points equal to the set length and has won by 2 points or more.
   * @returns True if a player has won.
   */
  bool CheckForWin()
  {
    return (p1_score >= set_length || p2_score >= set_length) && abs(p1_score - p2_score) >= 2;
  }

  void UndoLastPoint()
  {
    if (this->point_count == 0) return;
    PointRecord point = this->point_history[this->point_count - 1];
    this->point_history[point_count] = {}; // Zero out the last record
    this->point_count--;

    // Undo score awarded from last point
    if (point.player_who_won == 1)
    {
      // Player 1 won the last point (that is being undone), so decrement his score
      this->p1_score--;
    }
    else if (point.player_who_won == 2)
    {
      // Player 2 won the last point (that is being undone), so decrement his score
      this->p2_score--;
    }

    // Set server to the last server
    if (point.player_who_served == 1)
    {
      this->player_serving = 1;
    }
    else if (point.player_who_served == 2)
    {
      this->player_serving = 2;
    }

    UpdateScoreboards();
    this->ShowServeInfoMessage();
  }

  const Player* GetWinner()
  {
    if (!CheckForWin()) return nullptr;

    if (max(p1_score, p2_score) == p1_score) return &this->p1;
    else return &this->p2;
  }

  uint8_t GetP1Score()
  {
    return this->p1_score;
  }

  uint8_t GetP2Score()
  {
    return this->p2_score;
  }

  /**
   * Write "<current_server>\nto serve <serves_left>" to the OLED.
   */
  void ShowServeInfoMessage()
  {
    const Player& current_server = get_current_server();
    uint8_t serves_left = calculate_remaining_serves();
    ShowServeInfoMessageOLED(current_server, serves_left);
  }

  /**
   * Serialize the game for sending over wifi to the server
   */
  String Serialize()
  {
    // Estimate needed size (use ArduinoJson Assistant online if needed)
    const size_t capacity = JSON_OBJECT_SIZE(4) + JSON_ARRAY_SIZE(MAX_POINTS) + MAX_POINTS * JSON_OBJECT_SIZE(2) + 200;
    StaticJsonDocument<capacity> doc;

    doc["p1_id"] = p1.id;
    doc["p2_id"] = p2.id;
    doc["set_length"] = set_length;
    doc["started_at"] = started_at;
    doc["ended_at"] = GetUTC();

    JsonArray points = doc.createNestedArray("point_history");

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