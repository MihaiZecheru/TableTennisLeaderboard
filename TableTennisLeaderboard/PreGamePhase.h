#ifndef PRE_GAME_PHASE_H
#define PRE_GAME_PHASE_H

#include "Game.h"
#include "RotaryEncoderFuncs.h"
#include "OLED.h"
#include "Players.h"
#include "ButtonBoard.h"
#include "WebSocket.h"

uint8_t SelectSetLength();
const Player* SelectPlayer(uint8_t player_num, int8_t banned_id = -1);
uint8_t SelectServer(const char* p1_name, const char* p2_name);

/**
 * Main pre-game logic to collect match setup info from user
 */
Game* PreGamePhase()
{
  select_set_length_label:;
  uint8_t set_length = SelectSetLength();

  select_p1_label:;
  const Player* p1 = SelectPlayer(1);
  if (p1 == nullptr) goto select_set_length_label;

  select_p2_label:;
  const Player* p2 = SelectPlayer(2, p1->id);
  if (p2 == nullptr) goto select_p1_label;

  uint8_t first_server = SelectServer(p1->name, p2->name);
  if (first_server == 0) goto select_p2_label;

  StartWebSocket(p1->name, p2->name, first_server);
  ShowGameOnMessage();
  delay(1000);
  ClearOLED();

  Game* game = new Game(set_length, *p1, *p2, first_server);
  return game;
}

/**
 * Choose set length: 6, 11, or 21 using rotary dial
 */
uint8_t SelectSetLength()
{
  uint8_t set_lengths[] = {6, 11, 21};
  int8_t index = 1;
  int8_t prev_index = 1;

  rotary_encoder_tick();
  ShowSelectSetLengthMessage(set_lengths[index]);

  while (true)
  {
    rotary_encoder_tick();
    int8_t movement = rotary_encoder_movement();

    if (movement != 0)
    {
      index += movement;
      if (index < 0) index = 0;
      if (index > 2) index = 2;

      if (index != prev_index)
      {
        ShowSelectSetLengthMessage(set_lengths[index]);
        prev_index = index;
      }
    }

    if (rotary_encoder_btn_pressed() || P1ButtonPressed())
    {
      delay(200);
      while (rotary_encoder_btn_pressed() || P1ButtonPressed());
      return set_lengths[index];
    }

    delay(5);
  }
}

/**
 * Choose a player from saved list using rotary dial
 */
const Player* SelectPlayer(uint8_t player_num, int8_t banned_id)
{
  int8_t index = 0;
  int8_t prev_index = 0;

  if (ALL_SAVED_PLAYERS[0].id == banned_id)
    index = prev_index = 1;

  rotary_encoder_tick();
  ShowSelectPlayerMessage(player_num, ALL_SAVED_PLAYERS[index]);

  while (true)
  {
    if (UndoPointButtonPressed())
    {
      delay(100);
      while (UndoPointButtonPressed());
      return nullptr;
    }

    rotary_encoder_tick();
    int8_t movement = rotary_encoder_movement();

    if (movement != 0)
    {
      index += movement;

      if (index < 0) index = 0;
      if (index >= SAVED_PLAYER_COUNT) index = SAVED_PLAYER_COUNT - 1;

      if (ALL_SAVED_PLAYERS[index].id == banned_id)
      {
        index += (movement > 0) ? 1 : -1;
        if (index < 0) index = 0;
        if (index >= SAVED_PLAYER_COUNT) index = SAVED_PLAYER_COUNT - 1;
      }

      if (index != prev_index)
      {
        ShowSelectPlayerMessage(player_num, ALL_SAVED_PLAYERS[index]);
        prev_index = index;
      }
    }

    if (rotary_encoder_btn_pressed() || P1ButtonPressed())
    {
      delay(200);
      while (rotary_encoder_btn_pressed() || P1ButtonPressed());
      return &ALL_SAVED_PLAYERS[index];
    }

    delay(5);
  }
}

/**
 * Choose server (P1 or P2) using rotary encoder
 */
uint8_t SelectServer(const char* p1_name, const char* p2_name)
{
  int8_t player_num = 1;
  int8_t prev_player_num = 1;

  rotary_encoder_tick();
  ShowSelectServerMessage(player_num, p1_name, p2_name);

  while (true)
  {
    if (UndoPointButtonPressed())
    {
      delay(100);
      while (UndoPointButtonPressed());
      return 0;
    }

    rotary_encoder_tick();
    int8_t movement = rotary_encoder_movement();

    if (movement != 0)
    {
      player_num += movement;
      if (player_num < 1) player_num = 1;
      if (player_num > 2) player_num = 2;

      if (player_num != prev_player_num)
      {
        ShowSelectServerMessage(player_num, p1_name, p2_name);
        prev_player_num = player_num;
      }
    }

    if (rotary_encoder_btn_pressed() || P1ButtonPressed())
    {
      delay(200);
      while (rotary_encoder_btn_pressed() || P1ButtonPressed());
      return player_num;
    }

    delay(5);
  }
}

#endif