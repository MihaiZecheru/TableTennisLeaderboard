#ifndef OLED_H
#define OLED_H

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "Players.h"

#define OLED_ADDRESS 0x3C

Adafruit_SSD1306 display(-1); // // -1 because no reset pin used

void initialize_OLED()
{
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS);
  display.clearDisplay();
  display.setTextSize(2);
  display.setFont();
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.print(" Ping Pong");
  display.display();
}

/**
 * Clear the OLED and immediately update the display
 */
void ClearOLED()
{
  display.clearDisplay();
  display.display();
}

/**
 * Alert the user that the POST request to the server failed.
 * Tell him that the upload will be attempted again (it will be attempted infinetely until the arduino is turned off)
 */
bool ShowHttpFailMessage()
{
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("ERROR");
  display.setCursor(0, 16);
  display.print("SYNCING");
  display.display();

  delay(3000);

  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("RETRYING...");
  display.display();
}

/**
 * Write "ERROR <code>" to the OLED and then block the ESP32
 */
void ShowErrorMessage(uint8_t code)
{
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("ERROR:");
  display.setCursor(0, 16);
  display.print("C");
  display.print(code);
  display.display();
  while (true);
}

/**
 * Write "Set length\n<current_length> points" to the OLED.
 * @param current_length Must be 6, 11, or 21
 */
void ShowSelectSetLengthMessage(uint8_t current_length)
{
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("Set length");
  display.setCursor(0, 16);
  display.print(current_length);
  display.print(" points");
  display.display();
}

/**
 * Write "Set P<player_num>: <p.id>\n<p.name>" to the OLED.
 * @param player_num Either 1 or 2, representing P1 or P2
 * @param currently_selected_player The player currently selected by the dial
 */
void ShowSelectPlayerMessage(uint8_t player_num, const Player& currently_selected_player)
{
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("Set P");
  display.print(player_num);
  display.print(": ");
  display.print(currently_selected_player.id);
  display.setCursor(0, 16);
  display.print(currently_selected_player.name);
  display.display();
}

/**
 * Write "<current_server>\nto serve <serves_left>" to the OLED.
 */
void ShowServeInfoMessageOLED(const Player& current_server, uint8_t serves_left)
{
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print(current_server.name);
  display.setCursor(0, 16);
  display.print("to serve ");
  display.print(serves_left);
  display.display();
}

/**
 * Write "1st serve: <player_name>" to the OLED.
 */
void ShowSelectServerMessage(uint8_t selected_player, const char* p1_name, const char* p2_name)
{
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("1st server");
  display.setCursor(0, 16);
  display.print("- ");
  if (selected_player == 1)
    display.print(p1_name);
  else if (selected_player == 2)
    display.print(p2_name);
  display.display();
}

/**
 * Write "Submitting\nResults..." to the OLED.
 * @param dots The number of trailing elipsis dots (either 0, 1, 2, or 3)
 */
void ShowUploadingScoresMessage(uint8_t dots)
{
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("Submitting");
  display.setCursor(0, 16);
  display.print("Results");
  // For animation
  if (dots == 1) display.print(".");
  else if (dots == 2) display.print("..");
  else if (dots == 3) display.print("...");
  display.display();
}

/**
 * Write "<winning_player_name>\nwon 21-18" to the OLED,
 * where 21-18 was the score
 */
void ShowGameOverMessage(const char* winning_player_name, uint8_t p1_score, uint8_t p2_score)
{
  uint8_t _max = max(p1_score, p2_score);
  uint8_t _min = min(p2_score, p2_score);

  display.display();
  display.setCursor(0, 0);
  display.print(winning_player_name);
  display.setCursor(0, 16);
  display.print("won ");
  display.print(_max);
  display.print("-");
  display.print(_min);
  display.display();
}

#endif