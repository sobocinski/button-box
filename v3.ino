#include <Key.h>
#include <Keypad.h>
#include <Bounce2.h>
#include <Encoder.h>
#include <Wire.h>
#include "FlowSerialRead.h"
#include "LiquidCrystal_I2C.h"


#define NUMSWITCHES 5
#define NUMENCODERS 2
#define NUMBUTTONS 16
#define NUMROWS 6
#define NUMCOLS 3


#define MESSAGE_HEADER 0x03
#define INCLUDE_I2CLCD
int I2CLCD_enabled = 1;


int joySticKey = 0;
int isGetKeys = 0;


/* Switches */
byte switchesPins[NUMSWITCHES] = {13, 14, 15, 16, 17};
Bounce switches[NUMSWITCHES] = {Bounce(), Bounce(), Bounce(), Bounce(), Bounce()};

void setupSwiches() {
  for (int i = 0; i < NUMSWITCHES; i++) {
    switches[i].attach(switchesPins[i]);
    switches[i].interval(20);
    pinMode(switchesPins[i], INPUT_PULLUP);
  }
}


void updateSwitches() {
  for (int i = 0; i < NUMSWITCHES; i++) {
    switches[i].update();
  }

  for (int i = 0; i < NUMSWITCHES; i++) {
    joySticKey = i + 1;
    if (switches[i].fallingEdge()) {
      Joystick.button(joySticKey, 0);
    }
    if (switches[i].risingEdge()) {
      Joystick.button(joySticKey, 1);
    }
  }
}


/* Buttons */
byte keys[NUMROWS][NUMCOLS] = {
  {6, 7, 8},
  {9, 10, 11},
  {12, 13, 14},
  {15, 16, 17},
  {18, 19, 20},
  {21, 22, 23},
};

byte rowPins[NUMROWS] = {3, 4, 5, 6, 7, 8}; //connect to the row pinouts of the keypad
byte colPins[NUMCOLS] = {0, 1, 2}; //connect to the column pinouts of the keypad

Keypad buttbx = Keypad( makeKeymap(keys), rowPins, colPins, NUMROWS, NUMCOLS);

void updateButtons() {
  isGetKeys = buttbx.getKeys();
  if (isGetKeys) {
    for (int i = 0; i < LIST_MAX; i++) {
      if ( buttbx.key[i].stateChanged )
      {
        switch (buttbx.key[i].kstate) {
          case PRESSED:
          case HOLD:
            Joystick.button(buttbx.key[i].kchar, 1);
            break;
          case RELEASED:
          case IDLE:
            Joystick.button(buttbx.key[i].kchar, 0);
            break;
        }
      }
    }
  }
}


/* Encoders */
Encoder encoder[NUMENCODERS] = {Encoder(20, 21), Encoder(22, 23)};
int encoderButton[NUMENCODERS][2] = {{24, 25}, {26, 27}};
int encoderVal[NUMENCODERS] = {9999, -9999};
long encoderValue = -9999;

void updateEncoders() {
  for (int i = 0; i < NUMENCODERS; i++) {
    encoderValue = encoder[i].read();
    if (encoderValue == encoderVal[i]) {
      Joystick.button(encoderButton[i][0], 0);
      Joystick.button(encoderButton[i][1], 0);
      delay(10);
    }
    else if (encoderValue > encoderVal[i]) {
      Joystick.button(encoderButton[i][0], 1);
      delay(20);
    }
    else {
      Joystick.button(encoderButton[i][1], 1);
      delay(20);
    }
    encoderVal[i] = encoderValue;
  }
}

/*----------------------------------------------------------------------------------*/




String DEVICE_NAME = String("SimHub Dash"); //{"Group":"General","Name":"DEVICE_NAME","Title":"Device name","DefaultValue":"SimHub Dash","Type":"string","Template":"String DEVICE_NAME = String(\"{0}\");"}

int ENABLE_MICRO_GAMEPAD = 0; //{"Group":"GAMEPAD","Name":"ENABLE_MICRO_GAMEPAD","Title":"Enable arduino micro gamepad output for all the activated buttons\r\nSet to 0 to disable","DefaultValue":"0","Type":"integer","Template":"int ENABLE_MICRO_GAMEPAD = {0};"}
int TM1638_ENABLEDMODULES = 0; //{"Group":"TM1638","Name":"TM1638_ENABLEDMODULES","Title":"TM1638 modules connected\r\nSet to 0 if none","DefaultValue":"0","Type":"integer","Template":"int TM1638_ENABLEDMODULES = {0};"}
int TM1637_ENABLEDMODULES = 0; //{"Group":"TM1637","Name":"TM1637_ENABLEDMODULES","Title":"TM1637 modules connected\r\nSet to 0 if none","DefaultValue":"0","Type":"integer","Template":"int TM1637_ENABLEDMODULES = {0};"}
int MAX7221_ENABLEDMODULES = 0; //{"Group":"MAX7221","Name":"MAX7221_ENABLEDMODULES","Title":"MAX7219 / MAX7221 7 Segment modules connected \r\nSet to 0 if none\r\nMultiple modules can be cascaded connected module output to next module input","DefaultValue":"0","Type":"integer","Template":"int MAX7221_ENABLEDMODULES = {0};"}
int MAX7221_MATRIX_ENABLED = 0; //{"Group":"MAX7221_MATRIX","Name":"MAX7221_MATRIX_ENABLED","Title":"MAX7221 led matrix enabled \r\n0 = disabled, 1 = enabled","DefaultValue":"0","Type":"integer","Template":"int MAX7221_MATRIX_ENABLED = {0};"}
int WS2812B_RGBLEDCOUNT = 0; //{"Group":"WS2812B_RGBLED","Name":"WS2812B_RGBLEDCOUNT","Title":"WS2812B RGB leds count\r\nSet to 0 if none","DefaultValue":"0","Type":"integer","Template":"int WS2812B_RGBLEDCOUNT = {0};"}
int WS2812B_MATRIX_ENABLED = 0; //{"Group":"WS2812B_MATRIX","Name":"WS2812B_MATRIX_ENABLED","Title":"Support Coming Soon !! -- WS2812B 8x8 Matrix enabled\r\nSet to 1 to enable","DefaultValue":"0","Type":"integer","Template":"int WS2812B_MATRIX_ENABLED = {0};"}
int WS2801_RGBLEDCOUNT = 0; //{"Group":"WS2801_RGBLED","Name":"WS2801_RGBLEDCOUNT","Title":"WS2801 RGB leds count\r\nSet to 0 if none","DefaultValue":"0","Type":"integer","Template":"int WS2801_RGBLEDCOUNT = {0};"}

#ifdef INCLUDE_I2CLCD
int I2CLCD_width = 16;
int I2CLCD_height = 2;
byte I2CLCD_address = 0x27;
LiquidCrystal_I2C I2CLCD(I2CLCD_address, I2CLCD_width, I2CLCD_height);

#endif
int ENABLE_74HC595_GEAR_DISPLAY = 0; //{"Group":"74HC595 GEAR DISPLAY","Name":"ENABLE_74HC595_GEAR_DISPLAY","Title":"74HC595 gear display enabled\r\n0 = disabled, 1 = enabled","DefaultValue":"0","Type":"hex","Template":"int ENABLE_74HC595_GEAR_DISPLAY = {0};"}
int ENABLE_ADA_HT16K33_7SEGMENTS = 0; //{"Group":"HT16K33_7SEGMENTS","Name":"ENABLE_ADA_HT16K33_7SEGMENTS","Title":"Adafruit HT16K33 7 segments display enabled\r\n0 = disabled, 1 = enabled","DefaultValue":"0","Type":"hex","Template":"int ENABLE_ADA_HT16K33_7SEGMENTS = {0};"}
int ENABLE_ADA_HT16K33_Matrix = 0; //{"Group":"HT16K33_Matrix","Name":"ENABLE_ADA_HT16K33_Matrix","Title":"Adafruit HT16K33 matrix display enabled\r\n0 = disabled, 1 = enabled","DefaultValue":"0","Type":"hex","Template":"int ENABLE_ADA_HT16K33_Matrix = {0};"}
int ENABLED_BUTTONS_COUNT = 0; //{"Group":"ADDITIONAL BUTTONS","Name":"ENABLED_BUTTONS_COUNT","Title":"Additional buttons (directly connected to the arduino) buttons count\r\n0 = disabled, >0  = enabled","DefaultValue":"0","Type":"integer","Template":"int ENABLED_BUTTONS_COUNT = {0};"}


void setup() {
  FlowSerialBegin(19200);
  Joystick.hat(-1);
  setupSwiches();
  I2CLCD.begin();
  I2CLCD.backlight();
  I2CLCD.setCursor(2, 0);
  I2CLCD.print("Full Throttle!");
  I2CLCD.setCursor(1, 1);
  I2CLCD.print("fullthrottle.pl");
  
}



void sendButtonState() {
  FlowSerialFlush();
}

///usb14400
//secline %3
//lcd speed 4
int secLine = 0;
void Read20x4LCD() {

  if (FlowSerialTimedRead() > 0) {
    //I2CLCD.backlight();
  }
  else {
    //I2CLCD.noBacklight();
  }
  int row = FlowSerialTimedRead();
  String data = FlowSerialReadStringUntil('\n');
  if (row == 0) {
    I2CLCD.setCursor(0, row);
    I2CLCD.print(data);
    return;
  }
  secLine++;
  if(secLine%3 == 0) {
    I2CLCD.setCursor(0, row);
    I2CLCD.print(data);

    secLine =0;
  }


   
  
}

void SetBaudrate() {
  int br = FlowSerialTimedRead();
  delay(200);
  if (br == 1) FlowSerialBegin(300);
  if (br == 2) FlowSerialBegin(1200);
  if (br == 3) FlowSerialBegin(2400);
  if (br == 4) FlowSerialBegin(4800);
  if (br == 5) FlowSerialBegin(9600);
  if (br == 6) FlowSerialBegin(14400);
  if (br == 7) FlowSerialBegin(19200);
  if (br == 8) FlowSerialBegin(28800);
  if (br == 9) FlowSerialBegin(38400);
  if (br == 10) FlowSerialBegin(57600);
  if (br == 11) FlowSerialBegin(115200);
  if (br == 12) FlowSerialBegin(230400);
  if (br == 13) FlowSerialBegin(250000);
  if (br == 14) FlowSerialBegin(1000000);
  if (br == 15) FlowSerialBegin(2000000);
  if (br == 16) FlowSerialBegin(200000);
}



uint8_t header = 0;
char opt;


void loop() {
  updateButtons();
  updateSwitches();
  updateEncoders();

  // Wait for data
  if (FlowSerialAvailable() > 0) {
    // Reader header
    header = FlowSerialRead();

    if (header == MESSAGE_HEADER)
    {
      // Read command
      opt = FlowSerialTimedRead();

      // Hello command
      if (opt == '1') {
        FlowSerialTrigger = FlowSerialTimedRead();
        delay(10);
        FlowSerialPrint('h');
        FlowSerialFlush();
      }

      // Set baudrate
      if (opt == '8') {
        SetBaudrate();
      }

      // Simple buttons count
      if (opt == 'J') {
        FlowSerialWrite((byte)(ENABLED_BUTTONS_COUNT));
        FlowSerialFlush();
      }

      //  Module count command
      if (opt == '2') {
        FlowSerialWrite((byte)(TM1638_ENABLEDMODULES));
        FlowSerialFlush();
      }

      //  SIMPLE Module count command
      if (opt == 'B') {
        FlowSerialWrite((byte)(MAX7221_ENABLEDMODULES + TM1637_ENABLEDMODULES + ENABLE_ADA_HT16K33_7SEGMENTS));
        FlowSerialFlush();
      }

      // ACQ Packet
      if (opt == 'A') {
        FlowSerialWrite(0x03);
        FlowSerialFlush();
      }

      // Buttons state
      if (opt == 'C') {
        sendButtonState();
        FlowSerialFlush();
      }

      // Device Name
      if (opt == 'N') {
        FlowSerialPrint(DEVICE_NAME);
        FlowSerialPrint("\n");
        FlowSerialFlush();
      }


      // Features command
      if (opt == '0') {
        delay(10);
        // LCD
        if (I2CLCD_enabled == 1) {
          FlowSerialPrint("L");
        }
        // Gear
        FlowSerialPrint("G");
        // Name
        FlowSerialPrint("N");
        // Additional buttons
        FlowSerialPrint("J");
        // Custom Protocol Support
        FlowSerialPrint("P");
        FlowSerialPrint("\n");
        FlowSerialFlush();
      }


      //  RGBLED count command
      if (opt == '4') {
        FlowSerialWrite((byte)(WS2812B_RGBLEDCOUNT + WS2801_RGBLEDCOUNT));
        FlowSerialFlush();
      }

      // Write RGB LEDS data
      if (opt == '6') {

        // Acq !
        FlowSerialWrite(0x15);
      }

      // Write RGB MATRIX data
      if (opt == 'R') {
        // Acq !
        FlowSerialWrite(0x15);
      }



      // GEAR
      if (opt == 'G') {
        //char gear = FlowSerialTimedRead();

        //
        // Do what you want with current gear here ;)
        //
      }

      // I2C LCD
      if (opt == 'L') {
        Read20x4LCD();
      }


      // Custom protocol for end users (See Wiki for more infos : https://github.com/zegreatclan/AssettoCorsaTools/wiki/Custom-Arduino-hardware-support)
      if (opt == 'P') {
        // -------------------------------------------------------
        // EXAMPLE 1 - read the whole message and sent it back to simhub
        // -------------------------------------------------------
        String message = FlowSerialReadStringUntil('\n');
        FlowSerialDebugPrintLn("Message received : " + message);

        /*
          // -------------------------------------------------------
          // EXAMPLE 2 - reads speed and gear from the message
          // Protocol formula must be set in simhub to
          // format([DataCorePlugin.GameData.NewData.SpeedKmh],'0') + ';' + isnull([DataCorePlugin.GameData.NewData.Gear],'N')
          // -------------------------------------------------------

          int speed = FlowSerialReadStringUntil(';').toInt();
          String gear = FlowSerialReadStringUntil('\n');

          FlowSerialDebugPrintLn("Speed : " + String(speed));
          FlowSerialDebugPrintLn("Gear : " + gear);
        */

        /*
          // -------------------------------------------------------
          // EXAMPLE 3 - Driving a E36 speed gauge connected to pin D3
          // Protocol formula must be set in simhub to round([DataCorePlugin.GameData.NewData.SpeedKmh],0)
          // format([DataCorePlugin.GameData.NewData.SpeedKmh],'0')
          // -------------------------------------------------------
          CustomProtocolExample3_E36Speedo();
        */
      }
    }
  }





}//end loop
