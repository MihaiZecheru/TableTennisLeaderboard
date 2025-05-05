#ifndef ROTARY_ENCODER_H
#define ROTARY_ENCODER_H

#define ROTARY_ENCODER_SW_PIN  34
#define ROTARY_ENCODER_DT_PIN  35
#define ROTARY_ENCODER_CLK_PIN 32

/**
 * Set pin modes. Call in setup()
 */
void initialize_rotary_encoder()
{
  pinMode(ROTARY_ENCODER_CLK_PIN, INPUT);
  pinMode(ROTARY_ENCODER_DT_PIN, INPUT);
  pinMode(ROTARY_ENCODER_SW_PIN, INPUT_PULLUP); // SW is active low
}

/**
 * Read the signal from the clock pin (CLK)
 */
bool rotary_encoder_read_clk()
{
  return digitalRead(ROTARY_ENCODER_CLK_PIN);
}

/**
 * Read the signal from the data pin (DT)
 */
bool rotary_encoder_read_dt()
{
  return digitalRead(ROTARY_ENCODER_DT_PIN);
}

/**
 * Check to see if the built-in switch on the
 * rotary encoder is pressed
 * @returns true if the button is pressed
 */
bool rotary_encoder_btn_pressed()
{
  return digitalRead(ROTARY_ENCODER_SW_PIN) == LOW;
}

#endif