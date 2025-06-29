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

const uint8_t SEG_SPACE = 0x00;
const uint8_t SEG_0 = 0x3F;
const uint8_t SEG_1 = 0x06;
const uint8_t SEG_2 = 0x5B;
const uint8_t SEG_3 = 0x4F;
const uint8_t SEG_4 = 0x66;
const uint8_t SEG_5 = 0x6D;
const uint8_t SEG_6 = 0x7D;
const uint8_t SEG_7 = 0x07;
const uint8_t SEG_8 = 0x7F;
const uint8_t SEG_9 = 0x6F;

uint8_t digitToSegment(uint8_t digit) {
  const uint8_t segs[] = {SEG_0, SEG_1, SEG_2, SEG_3, SEG_4, SEG_5, SEG_6, SEG_7, SEG_8, SEG_9};
  return (digit < 10) ? segs[digit] : SEG_SPACE;
}

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
  // Clamp values
  if (p1_score > 99) p1_score = 99;
  if (p2_score > 99) p2_score = 99;

  // Extract digits
  uint8_t p1_tens = (p1_score >= 10) ? (p1_score / 10) : 0;
  uint8_t p1_ones = p1_score % 10;
  uint8_t p2_tens = (p2_score >= 10) ? (p2_score / 10) : 0;
  uint8_t p2_ones = p2_score % 10;

  // Compose segments
  uint8_t segments[4];
  segments[0] = (p1_score >= 10) ? digitToSegment(p1_tens) : SEG_SPACE;
  segments[1] = (p2_score >= 10) ? digitToSegment(p1_ones) | 0b10000000 : digitToSegment(p1_ones);
  segments[2] = (p2_score >= 10) ? digitToSegment(p2_tens) : SEG_SPACE;  // Colon on
  segments[3] = digitToSegment(p2_ones);

  p1_scoreboard.setSegments(segments);
  p2_scoreboard.setSegments(segments);
  spectator_scoreboard.setSegments(segments);
}

#endif