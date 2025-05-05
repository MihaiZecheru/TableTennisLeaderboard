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
  display.print("Ping Pong");
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
 * Prompt him to try again on the OLED and wait for a button press.
 * One button will return true (meaning Retry), the other will return false (meaning Quit).
 * @returns True if should retry the POST request, false if not.
 */
bool PromptRetryOnOLED(int error_code)
{
  // TODO: 
  return false;
}

/**
 * Write "ERROR" to the OLED
 */
void ShowErrorMessage()
{
  // TODO: 

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
  display.print(currently_selected_player.name);
  display.display();
}

/**
 * Write "Choose 1st\nserver" to the OLED.
 */
void ShowChoose1stServerMessage()
{
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("Choose 1st");
  display.print("server");
  display.display();
}

/**
 * Write "<current_server>\nto serve <serves_left>" to the OLED.
 */
void ShowServeInfoMessage(const Player& current_server, uint8_t serves_left)
{
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print(current_server.name);
  display.setCursor(0, 32);
  display.print("to serve ");
  display.print(serves_left);
  display.display();
}

#endif