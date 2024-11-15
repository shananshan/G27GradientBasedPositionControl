// pressA, pressB, pressC

#ifndef _BUTTON_H
#define _BUTTON_H

#include <Arduino.h>

#define A_button 14
#define B_button 30
#define C_button 17


class Button_c {
  public:
    // Constructor to initialize a button with pin
    Button_c() {
      // leave this empty
    }

    // Initialize the button pin
    void initialiseForButton() {
        pinMode(A_button, INPUT_PULLUP);  // Use INPUT_PULLUP to make the pin HIGH when the button is not pressed
        pinMode(B_button, INPUT_PULLUP);
        pinMode(C_button, INPUT_PULLUP);
    }

    // Check if the button is pressed
    bool AisPressed() {
        return digitalRead(A_button) == LOW;  // Button is pressed if the pin reads LOW
    }
    bool BisPressed() {
        return digitalRead(B_button) == LOW;  // Button is pressed if the pin reads LOW
    }
    bool CisPressed() {
        return digitalRead(C_button) == LOW;  // Button is pressed if the pin reads LOW
    }

private:
    int pin;
};

#endif
