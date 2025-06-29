#ifndef BUZZER_H
#define BUZZER_H

#define BUZZER_PIN 23

#define P1_FREQ 500
#define P2_FREQ 1000

#define POINT_BEEP_DURATION_MS 250
#define SERVER_CHANGE_BEEP_DURATION_MS 1000
#define UNDO_POINT_BEEP_DURATION_MS 100

void initialize_buzzer()
{
  pinMode(BUZZER_PIN, OUTPUT);
}

void _play_tone(int frequency, int duration_ms)
{
  int period_us = 1000000 / frequency;
  int half_period = period_us / 2;
  long cycles = (long)frequency * duration_ms / 1000;

  for (long i = 0; i < cycles; i++)
  {
    digitalWrite(BUZZER_PIN, HIGH);
    delayMicroseconds(half_period);
    digitalWrite(BUZZER_PIN, LOW);
    delayMicroseconds(half_period);
  }
}

/**
 * @param mode (1, 2 or 3). 1 for point scored beep. 2 for server changed beep, 3 for point-removed beep
 */
void PlayP1Tone(int mode)
{
  int duration;
  if (mode == 1)
  {
    duration = POINT_BEEP_DURATION_MS;
  }
  else if (mode == 2)
  {
    duration = SERVER_CHANGE_BEEP_DURATION_MS;
  }
  else if (mode == 3)
  {
    duration = UNDO_POINT_BEEP_DURATION_MS;
  }

  _play_tone(P1_FREQ, duration);
}

/**
 * @param mode (1, 2 or 3). 1 for point scored beep. 2 for server changed beep, 3 for point-removed beep
 */
void PlayP2Tone(int mode)
{
  int duration;
  if (mode == 1)
  {
    duration = POINT_BEEP_DURATION_MS;
  }
  else if (mode == 2)
  {
    duration = SERVER_CHANGE_BEEP_DURATION_MS;
  }
  else if (mode == 3)
  {
    duration = UNDO_POINT_BEEP_DURATION_MS;
  }
  _play_tone(P2_FREQ, duration);
}

/**
 * @param player_whose_point_removed Either 1 or 2; 1 if player 1's point was undone. 2 if player 2's point was undone.
 */
void PlayUndoPointBeep(int player_whose_point_removed)
{
  if (player_whose_point_removed == 1)
  {
    for (int i = 0; i < 3; i++)
    {
      PlayP1Tone(3);
      delay(UNDO_POINT_BEEP_DURATION_MS);
    }
  }
  else
  {
    for (int i = 0; i < 3; i++)
    {
      PlayP2Tone(3);
      delay(UNDO_POINT_BEEP_DURATION_MS);
    }
  }
}

#endif