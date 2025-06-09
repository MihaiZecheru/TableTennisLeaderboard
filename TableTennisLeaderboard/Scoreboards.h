#ifndef SCOREBOARDS_H
#define SCOREBOARDS_H

#include <TM1637Display.h>

// Set brightness (0 = dim, 7 = bright)
#define SCORE_DISPLAY_BRIGHTNESS 1
// Used in the showNumberDecEx func for turning on the colon
#define COLON_BYTE 0b01000000

#define CLK_P1 19
#define DIO_P1 18
#define CLK_P2 17
#define DIO_P2 5
#define CLK_SPEC 16
#define DIO_SPEC 4

TM1637Display p1_scoreboard(CLK_P1, DIO_P1);
TM1637Display p2_scoreboard(CLK_P2, DIO_P2);
TM1637Display spectator_scoreboard(CLK_SPEC, DIO_SPEC);

void ClearScoreboards() {
  p1_scoreboard.clear();
  p2_scoreboard.clear();
  spectator_scoreboard.clear();
}

/**
 * Set brightness and clear displays
 */
void initialize_scoreboards() {
  p1_scoreboard.setBrightness(SCORE_DISPLAY_BRIGHTNESS);
  p2_scoreboard.setBrightness(SCORE_DISPLAY_BRIGHTNESS);
  spectator_scoreboard.setBrightness(SCORE_DISPLAY_BRIGHTNESS);
  ClearScoreboards();
}

/**
 * Update the score across all three scoreboards.
 * All three scoreboards will always look the same.
 * P1 score is first, then P2. Format is "P1:P2"
 */
void _update_scoreboards(uint8_t p1_score, uint8_t p2_score) {
  // Possible with long deuce (though unlikely)
  if (p1_score > 99) p1_score = 99;
  if (p2_score > 99) p2_score = 99;

  // Will turn a score of 17 and 15 into 1715
  // Format: [P1][P2], e.g. "1715"
  // Used because individual digits can't be set; the showNumberDecEx func is used instead to write one number
  uint16_t combined_number = (p1_score * 100) + p2_score;

  // Write to all three with colon on and leading zeros = true
  p1_scoreboard.showNumberDecEx(combined_number, COLON_BYTE, true);
  p2_scoreboard.showNumberDecEx(combined_number, COLON_BYTE, true);
  spectator_scoreboard.showNumberDecEx(combined_number, COLON_BYTE, true);
}

#endif