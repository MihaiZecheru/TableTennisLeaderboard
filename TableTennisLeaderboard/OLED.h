#ifndef OLED_H
#define OLED_H

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

#endif