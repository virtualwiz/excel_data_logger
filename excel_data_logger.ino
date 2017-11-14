#include "Keyboard.h"

void setup() {
  Keyboard.begin();
}

void loop() {
  delay(3000);
  Keyboard.press(KEY_LEFT_CTRL);
  Keyboard.press(KEY_LEFT_ALT);
  Keyboard.press(KEY_DELETE);
  delay(100);
  Keyboard.releaseAll();
  while(1);
}








