#ifndef ROTARY_ENCODER_H
#define ROTARY_ENCODER_H

#include <RotaryEncoder.h>

// Define encoder pins
#define ROTARY_ENCODER_SW_PIN  34  // Can stay here (button only)
#define ROTARY_ENCODER_DT_PIN  35
#define ROTARY_ENCODER_CLK_PIN 32

// Create encoder instance (CLK, DT)
RotaryEncoder encoder(ROTARY_ENCODER_CLK_PIN, ROTARY_ENCODER_DT_PIN);

// Store position
long last_encoder_position = 0;

/**
 * Set pin modes and initialize encoder. Call in setup()
 */
void initialize_rotary_encoder()
{
  pinMode(ROTARY_ENCODER_SW_PIN, INPUT_PULLUP); // SW is active low
  last_encoder_position = encoder.getPosition();
}

/**
 * Call this once per loop to update encoder state
 */
void rotary_encoder_tick()
{
  encoder.tick();
}

/**
 * Get the encoder movement since last check
 * @returns -1, 0, or +1
 */
int8_t rotary_encoder_movement()
{
  long new_position = encoder.getPosition();
  int8_t movement = new_position - last_encoder_position;
  last_encoder_position = new_position;
  return movement * -1;
}

/**
 * Check if the encoder button is pressed
 * @returns true if pressed
 */
bool rotary_encoder_btn_pressed()
{
  return digitalRead(ROTARY_ENCODER_SW_PIN) == LOW;
}

#endif