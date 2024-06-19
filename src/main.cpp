/*
 * Specify which protocol(s) should be used for decoding.
 * If no protocol is defined, all protocols (except Bang&Olufsen) are active.
 * This must be done before the #include <IRremote.hpp>
 */
// #define DECODE_DENON        // Includes Sharp
// #define DECODE_JVC
// #define DECODE_KASEIKYO
// #define DECODE_PANASONIC    // alias for DECODE_KASEIKYO
// #define DECODE_LG
// #define DECODE_NEC          // Includes Apple and Onkyo. To enable all protocols , just comment/disable this line.
// #define DECODE_SAMSUNG
// #define DECODE_SONY
// #define DECODE_RC5
// #define DECODE_RC6

// #define DECODE_BOSEWAVE
// #define DECODE_LEGO_PF
// #define DECODE_MAGIQUEST
// #define DECODE_WHYNTER
// #define DECODE_FAST

// #define DECODE_DISTANCE_WIDTH // Universal decoder for pulse distance width protocols
// #define DECODE_HASH         // special decoder for all protocols

// #define DECODE_BEO          // This protocol must always be enabled manually, i.e. it is NOT enabled if no protocol is defined. It prevents decoding of SONY!

// #define DEBUG               // Activate this for lots of lovely debug output from the decoders.

// #define RAW_BUFFER_LENGTH  180  // Default is 112 if DECODE_MAGIQUEST is enabled, otherwise 100.
#include <Wire.h>
#include <Arduino.h>
#include <Stepper.h>
//This include defines the actual pin number for pins like IR_RECEIVE_PIN, IR_SEND_PIN for many different boards and architectures
#include "PinDefinitionsAndMore.h"
#include <IRremote.hpp> // include the library
#include <LiquidCrystal_I2C.h>

// Define the variables for the remote
bool playing = false;
unsigned long lastPlayToggle = 0;
const unsigned long playToggleInterval = 500;

// Define the buttons on the remote by code
const uint32_t BUTTON_PLAY = 0x43;
const uint32_t FORWARD = 0x40;
const uint32_t BACKWARD = 0x44;
const uint32_t PLUS = 0x15;
const uint32_t MINUS = 0x07;
const uint32_t PLUS_200 = 0xD;
const uint32_t MINUS_200 = 0x19; // PLUS 100 on the remote

// Define the settings for the step motor
Stepper stepper(2048, 8, 10, 9, 11);
// Set the step motor speed
int StepperSpeed = 4;

LiquidCrystal_I2C lcd(0x27, 16, 2); // set the LCD address to 0x27 for a 16 chars and 2 line display

void setup()
{
  Serial.begin(115200);
  // Just to know which program is running on my Arduino
  Serial.println(F("START " __FILE__ " from " __DATE__ "\r\nUsing library version " VERSION_IRREMOTE));

  // Start the receiver and if not 3. parameter specified, take LED_BUILTIN pin from the internal boards definition as default feedback LED
  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);

  Serial.print(F("Ready to receive IR signals of protocols: "));
  printActiveIRProtocols(&Serial);
  Serial.println(F("at pin " STR(IR_RECEIVE_PIN)));

  // Set the step motor speed
  stepper.setSpeed(4);
  
  // Initialize the LCD and print a welcome message
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Hello Habibi!");
  lcd.setCursor(0, 1);
  lcd.print("I'm a robot!");
}

void loop()
{
  /*
   * Check if received data is available and if yes, try to decode it.
   * Decoded result is in the IrReceiver.decodedIRData structure.
   *
   * E.g. command is in IrReceiver.decodedIRData.command
   * address is in command is in IrReceiver.decodedIRData.address
   * and up to 32 bit raw data in IrReceiver.decodedIRData.decodedRawData
   */
  if (IrReceiver.decode())
  {

    /*
     * Print a summary of received data
     */
    if (IrReceiver.decodedIRData.protocol == UNKNOWN)
    {
      Serial.println(F("Received noise or an unknown (or not yet enabled) protocol"));
      // We have an unknown protocol here, print extended info
      IrReceiver.printIRResultRawFormatted(&Serial, true);
      IrReceiver.resume(); // Do it here, to preserve raw data for printing with printIRResultRawFormatted()
    }
    else
    {
      IrReceiver.resume(); // Early enable receiving of the next IR frame
      IrReceiver.printIRResultShort(&Serial);
      IrReceiver.printIRSendUsage(&Serial);
    }
    Serial.println();

    
    // Finally, check the received data and perform actions according to the received command
    if (IrReceiver.decodedIRData.command == FORWARD)
    {
      // Go forwards
      stepper.step(64);
    }
    else if (!playing && IrReceiver.decodedIRData.command == BACKWARD)
    {
      // Go backwards
      stepper.step(-64);
    }
    else if (IrReceiver.decodedIRData.command == BUTTON_PLAY)
    {
      // Toggle play
      unsigned long now = millis();
      if (now - lastPlayToggle > playToggleInterval)
      {
        playing = !playing;
        // Print to the serial console the new play state
        Serial.print("Play toggled to: ");
        Serial.println(playing ? "true" : "false");
        lastPlayToggle = now;
        if (!playing)
        {
          // Clear the LCD and print a response
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print(F("I'm tired!"));
          lcd.setCursor(0, 1);
          lcd.print(F("Goodbye!"));
        }
        else
        {
          // Clear the LCD and print a response
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print(F("Let's go!"));
        }
      }
    }
    // If the plus button is pressed and motor is running and speed is less than 16
    if (playing && (IrReceiver.decodedIRData.command == PLUS) && StepperSpeed < 16)
    {
      // Increase the speed by 1
      StepperSpeed = StepperSpeed + 1;
      Serial.print("Speed set to: ");
      Serial.println(StepperSpeed);
      // Clear the LCD and print a response
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("Let's go faster!"));
    }
    // If the minus button is pressed and motor is running and speed is more than 1
    else if (playing && (IrReceiver.decodedIRData.command == MINUS) && StepperSpeed > 1)
    {
      // Decrease the speed by 1
      StepperSpeed = StepperSpeed - 1;
      Serial.print("Speed set to: ");
      Serial.println(StepperSpeed);
      // Clear the LCD and print a response
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("Whoo!"));
      lcd.setCursor(0, 1);
      lcd.print(F("Let's slow down!"));
    }
    // If the plus 200 button is pressed and motor is running and speed is less than 16
    else if (playing && (IrReceiver.decodedIRData.command == PLUS_200) && StepperSpeed != 16)
    {
      // Set speed to 16 (max speed)
      StepperSpeed = 16;
      Serial.print("Speed set to: ");
      Serial.println(StepperSpeed);
      // Clear the LCD and print a response
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("Wheee!"));
      lcd.setCursor(0, 1);
      lcd.print(F("Let's fly!"));
    }
    // If the minus 200 button is pressed and motor is running and speed is more than 4
    else if (playing && (IrReceiver.decodedIRData.command == MINUS_200) && StepperSpeed != 4)
    {
      // Set speed to 4
      StepperSpeed = 4;
      Serial.print("Speed set to: ");
      Serial.println(StepperSpeed);
      // Clear the LCD and print a response
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("Alright that's  "));
      lcd.setCursor(0, 1);
      lcd.print(F("for now!"));
    }
  }

  // If playing is true, move the motor
  if (playing)
  {
    // Set the speed and move the motor
    stepper.setSpeed(StepperSpeed);
    stepper.step(16);
  }
}
