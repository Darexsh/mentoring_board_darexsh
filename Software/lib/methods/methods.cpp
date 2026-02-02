#include "methods.h"

extern CRGB leds[NUM_LEDS];           // Declare the LED array from main file
extern Adafruit_NeoPixel strip;       // Declare the LED strip object from main file
extern TwoWire I2C_MENTORING;         // Declare the I2C object from main file
extern Adafruit_SHT4x sht4x;          // Declare the SHT4x sensor object

Adafruit_VCNL4040 vcnl4040 = Adafruit_VCNL4040();  // VCNL4040 object for proximity/light sensing
Adafruit_ADS1015 ads1015;                          // ADS1015 object for analog-to-digital conversion
#define BNO08X_RESET -1                            // BNO085 reset pin (not used in this example)
Adafruit_BNO08x bno08x(BNO08X_RESET);              // BNO085 object for orientation/motion sensing
sh2_SensorValue_t sensorValue;                     // Structure to hold BNO085 sensor data
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &I2C_MENTORING, -1);  // OLED display object

/**
 * @brief Set up GPIO pins as inputs or outputs
 */
void initPins() {
  pinMode(BTN1, INPUT);          // Button 1 as input
  pinMode(BTN2, INPUT);          // Button 2 as input
  pinMode(SW1, INPUT);           // Switch 1 as input
  pinMode(SW2, INPUT);           // Switch 2 as input
  pinMode(POTI2, INPUT);         // Potentiometer 2 as input
  pinMode(JOY_X, INPUT);         // Joystick X-axis as input
  pinMode(JOY_Y, INPUT);         // Joystick Y-axis as input
  pinMode(JOY_BTN, INPUT);       // Joystick button as input
  pinMode(ENCODER_A, INPUT);     // Encoder A signal as input
  pinMode(ENCODER_B, INPUT);     // Encoder B signal as input
  pinMode(SW_ENCODER, INPUT);    // Encoder switch as input
  pinMode(LED_PIN, OUTPUT);      // LED strip pin as output
  pinMode(SD_CS, OUTPUT);        // SD card chip select as output
  pinMode(SD_CLK, OUTPUT);       // SD card clock as output
  pinMode(SD_MOSI, OUTPUT);      // SD card MOSI as output
  pinMode(SD_MISO, INPUT);       // SD card MISO as input
}

/**
 * @brief Initialize I2C sensors and check if they are connected
 */
void initI2CSensors() {
  bool status_vcnl = vcnl4040.begin(VCNL4040_ADDRESS, &I2C_MENTORING);
  if (!status_vcnl) {
    Serial.println("Could not find VCNL4040, check wiring!");
    while (1); // Stop if sensor not found
  } else {
    Serial.println("Found VCNL4040!");
  }

  bool status_ADS = ads1015.begin(ADS1015_ADDRESS, &I2C_MENTORING);
  if (!status_ADS) {
    Serial.println("ADS1015 not found. Check wiring!");
    while (1); // Stop if sensor not found
  } else {
    Serial.println("Found ADS1015!");
  }

  bool status_OLED = display.begin(SSD1306_SWITCHCAPVCC, OLED_DISPLAY_ADDRESS);
  if (!status_OLED) {
    Serial.println("OLED display not found. Check wiring!");
    while (1); // Stop if display not found
  } else {
    Serial.println("Found OLED display!");
  }

  bool status_BNO = bno08x.begin_I2C(BNO085_ADDRESS, &I2C_MENTORING);
  if (!status_BNO) {
    Serial.println("BNO085 not found. Check wiring!");
    while (1); // Stop if sensor not found
  } else {
    Serial.println("Found BNO085!");
  }

  bool status_SHT4x = sht4x.begin(SHT4X_ADDRESS, &I2C_MENTORING);
  if (!status_SHT4x) {
    Serial.println("SHT4x not found. Check wiring!");
    while (1); // Stop if sensor not found
  } else {
    Serial.println("Found SHT4x!");
  }
}

/**
 * @brief Read and print button states over USB serial
 */
void pushButtons() {
  int btn1State = digitalRead(BTN1);
  int btn2State = digitalRead(BTN2);
  Serial.print(String("Button 1: ") + (btn1State == HIGH ? "Pressed (1)" : "Released (0)"));
  Serial.println(String(", Button 2: ") + (btn2State == HIGH ? "Pressed (1)" : "Released (0)"));
}

/**
 * @brief Read and print tactile switch states over USB serial
 */
void tactileSwitches() {
  int sw1State = digitalRead(SW1);
  int sw2State = digitalRead(SW2);
  Serial.println(String("Switch 1: ") + (sw1State == HIGH ? "High (1)" : "Low (0)") + String(", Switch 2: ") + (sw2State == HIGH ? "High (1)" : "Low (0)"));
}

/**
 * @brief Read and print potentiometer analog value
 */
void analogReadPoti() {
  int potiValue = analogRead(POTI2);
  Serial.println(String("Poti: ") + potiValue);
}

/**
 * @brief Measure and print potentiometer voltage
 */
void voltageMeasuring() {
  int potiValue = analogRead(POTI2);
  float voltage = (potiValue * 3.3) / 4095.0; // Convert 12-bit reading to voltage (0-3.3V)
  Serial.println(String("Poti Voltage: ") + voltage + " V");
}

/**
 * @brief Read and print joystick position and button state
 */
void readJoystick() {
  int joyXValue = analogRead(JOY_X);
  int joyYValue = analogRead(JOY_Y);
  int joyBtnState = digitalRead(JOY_BTN);
  Serial.println(String("Joystick X: ") + joyXValue + String(", Y: ") + joyYValue + String(", Button: ") + (joyBtnState == HIGH ? "Pressed (1)" : "Released (0)"));
}

/**
 * @brief Create a moving rainbow effect on WS2812 LEDs
 */
void rainbowLEDs() {
  static uint8_t hueOffset = 0; // Track color shift for rainbow
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[NUM_LEDS - 1 - i] = CHSV((hueOffset + i * (255 / NUM_LEDS)) % 255, 255, 255); // Set HSV colors
  }
  FastLED.show(); // Update LEDs
  hueOffset++;    // Move rainbow
  delay(5);       // Control speed
}

/**
 * @brief Read encoder position and update LEDs
 */
void readEncoder() {
  static int encoderPos = 0;          // Current position (0-23 for 24 steps)
  static int lastStateA = LOW;        // Last state of pin A
  static bool lastButtonState = HIGH; // Last state of encoder button
  static int lastEncoded = 0;         // Last combined state of A and B

  int currentStateA = digitalRead(ENCODER_A);
  int currentStateB = digitalRead(ENCODER_B);
  int encoded = (currentStateA << 1) | currentStateB; // Combine A and B states

  if (lastEncoded == 0 && encoded != lastEncoded) {
    if (encoded == 1) encoderPos--;      // Clockwise
    else if (encoded == 2) encoderPos++; // Counterclockwise
    encoderPos = (encoderPos + 24) % 24; // Keep within 0-23
    updateLEDs(encoderPos);              // Update LEDs
  }
  lastEncoded = encoded;

  int buttonState = digitalRead(SW_ENCODER);
  if (buttonState == LOW && lastButtonState == HIGH) {
    encoderPos = 0;        // Reset position on button press
    updateLEDs(encoderPos);
  }
  lastButtonState = buttonState;
  delay(1); // Reduce CPU load
}

/**
 * @brief Update LEDs based on encoder position
 */
void updateLEDs(int position) {
  for (int i = 0; i < NUM_LEDS; i++) {
    strip.setPixelColor(i, strip.Color(0, 0, 0)); // Turn off all LEDs
  }

  // Set LEDs according to the encoder position (24 positions total)
  // LEDs are arranged in a clockwise order, with LED 4 (Index 0) at position 12
  switch (position) {
    case 0:  setLEDs(3, 1);  break; // Position 1: LED 4 → Index 1
    case 1:  setLEDs(3, 2);  break; // Position 2: LED 4 & LED 5 → Index 1 & 0
    case 2:  setLEDs(4, 1);  break; // Position 3: LED 5 → Index 0
    case 3:  setLEDs(4, 2);  break; // Position 4: LED 5 & LED 6 → Index 0 & 11
    case 4:  setLEDs(5, 1);  break; // Position 5: LED 6 → Index 11
    case 5:  setLEDs(5, 2);  break; // Position 6: LED 6 & LED 7 → Index 11 & 10
    case 6:  setLEDs(6, 1);  break; // Position 7: LED 7 → Index 10
    case 7:  setLEDs(6, 2);  break; // Position 8: LED 7 & LED 8 → Index 10 & 9
    case 8:  setLEDs(7, 1);  break; // Position 9: LED 8 → Index 9
    case 9:  setLEDs(7, 2);  break; // Position 10: LED 8 & LED 9 → Index 9 & 8
    case 10: setLEDs(8, 1);  break; // Position 11: LED 9 → Index 8
    case 11: setLEDs(8, 2);  break; // Position 12: LED 9 & LED 10 → Index 8 & 7
    case 12: setLEDs(9, 1);  break; // Position 13: LED 10 → Index 7
    case 13: setLEDs(9, 2);  break; // Position 14: LED 10 & LED 11 → Index 7 & 6
    case 14: setLEDs(10, 1); break; // Position 15: LED 11 → Index 6
    case 15: setLEDs(10, 2); break; // Position 16: LED 11 & LED 12 → Index 6 & 5
    case 16: setLEDs(11, 1); break; // Position 17: LED 12 → Index 5
    case 17: setLEDs(11, 2); break; // Position 18: LED 12 & LED 1 → Index 5 & 4
    case 18: setLEDs(0, 1);  break; // Position 19: LED 1 → Index 4
    case 19: setLEDs(0, 2);  break; // Position 20: LED 1 & LED 2 → Index 4 & 3
    case 20: setLEDs(1, 1);  break; // Position 21: LED 2 → Index 3
    case 21: setLEDs(1, 2);  break; // Position 22: LED 2 & LED 3 → Index 3 & 2
    case 22: setLEDs(2, 1);  break; // Position 23: LED 3 → Index 2
    case 23: setLEDs(2, 2);  break; // Position 24: LED 3 & LED 4 → Index 2 & 1
  }
  strip.show(); // Update LED strip
}

/**
 * @brief Set specific LEDs with rainbow colors
 */
void setLEDs(int ledIndex, int numLEDs) {
  for (int i = 0; i < numLEDs; i++) {
    int index = (ledIndex + i) % NUM_LEDS;       // Wrap around LED strip
    uint8_t hue = (index * 255) / NUM_LEDS;      // Calculate hue for rainbow
    uint8_t r, g, b;                             // RGB values
    if (hue < 85) { r = 255 - (hue * 3); g = hue * 3; b = 0; }           // Red to Green
    else if (hue < 170) { hue -= 85; r = 0; g = 255 - (hue * 3); b = hue * 3; } // Green to Blue
    else { hue -= 170; r = hue * 3; g = 0; b = 255 - (hue * 3); }        // Blue to Red
    strip.setPixelColor(index, strip.Color(r, g, b)); // Set LED color
  }
}

/*
 * Proximity Integration Time = 0 -> 1T / 1 - 1.5T / 2 - 2T / 3 - 2.5T / 4 - 3T / 5 - 3.5T / 6 - 4T / 7 - 8T
 * Ambient Integration Time = 0 -> 80ms / 1 - 160ms / 2 - 320ms / 3 - 640ms
 * LED Current = 0 -> 50mA / 1 - 75mA / 2 - 100mA / 3 - 120mA / 4 - 140mA / 5 - 160mA / 6 - 180mA / 7 - 200mA
 * LED Duty Cycle = 0 -> 40 / 1 - 80 / 2 - 160 / 3 - 320
 * Proximity High Resolution = 0 -> false / 1 - true
 * Proximity Interrupt Status = 0 -> DISABLE / 1 - CLOSE / 2 - AWAY / 3 - CLOSE AWAY
 * Enable features (Ambient Light, Proximity, White Light) = 0 -> false / 1 - true
 */
#define SETTING_PROXIMITY_INTEGRATION_TIME 6  // 4T
#define SETTING_AMBIENT_INTEGRATION_TIME   0  // 80ms
#define SETTING_LED_CURRENT                0  // 50mA
#define SETTING_LED_DUTY_CYCLE             0  // 40
#define SETTING_PROXIMITY_HIGH_RESOLUTION  1  // True
#define SETTING_PROXIMITY_INTERRUPTS       0  // DISABLE
#define SETTING_FEATURE_AMBIENT_LIGHT      1  // Disabled
#define SETTING_FEATURE_PROXIMITY          1  // Enabled
#define SETTING_FEATURE_WHITE_LIGHT        1  // Disabled
void vcnl4040Read() {
  vcnl4040.enableAmbientLight(SETTING_FEATURE_AMBIENT_LIGHT);
  vcnl4040.enableProximity(SETTING_FEATURE_PROXIMITY);
  vcnl4040.enableWhiteLight(SETTING_FEATURE_WHITE_LIGHT);
  vcnl4040.setProximityIntegrationTime(static_cast<VCNL4040_ProximityIntegration>(SETTING_PROXIMITY_INTEGRATION_TIME));
  vcnl4040.setAmbientIntegrationTime(static_cast<VCNL4040_AmbientIntegration>(SETTING_AMBIENT_INTEGRATION_TIME));
  vcnl4040.setProximityLEDCurrent(static_cast<VCNL4040_LEDCurrent>(SETTING_LED_CURRENT));
  vcnl4040.setProximityLEDDutyCycle(static_cast<VCNL4040_LEDDutyCycle>(SETTING_LED_DUTY_CYCLE));
  vcnl4040.setProximityHighResolution(SETTING_PROXIMITY_HIGH_RESOLUTION);
  vcnl4040.enableProximityInterrupts(static_cast<VCNL4040_ProximityType>(SETTING_PROXIMITY_INTERRUPTS));

  int proximity = vcnl4040.getProximity();     // Get proximity value
  int ambient = vcnl4040.getAmbientLight();    // Get ambient light value
  int whiteLight = vcnl4040.getWhiteLight();   // Get white light value

  #if !defined OLED_VCNL4040
    Serial.println(String("Proximity: ") + proximity);
    Serial.println(String("Ambient Light: ") + ambient);
    Serial.println(String("White Light: ") + whiteLight);
  #else
    display.println(String("Proximity: ") + proximity);
    display.println("---------------------");
    display.println(String("Ambient Light: ") + ambient);
    display.println("---------------------");
    display.println(String("White Light: ") + whiteLight);
  #endif
}

/**
 * @brief Read and print ADS1015 ADC data
 */
void ads1015Read() {
  // int16_t adc0 = ads1015.readADC_SingleEnded(0); // Read ADC channel 0
  // int16_t adc1 = ads1015.readADC_SingleEnded(1); // Read ADC channel 1
  // int16_t adc2 = ads1015.readADC_SingleEnded(2); // Read ADC channel 2
  int16_t poti_digital = ads1015.readADC_SingleEnded(3); // Read ADC channel 3

  // Serial.println(String("ADC Channel 0: ") + adc0);
  // Serial.println(String("ADC Channel 1: ") + adc1);
  // Serial.println(String("ADC Channel 2: ") + adc2);
  Serial.println(String("Poti Digital: ") + poti_digital);
}

void chipInformation() {
  for (int n = 0; n < bno08x.prodIds.numEntries; n++) {
    Serial.print("Part ");
    Serial.print(bno08x.prodIds.entry[n].swPartNumber);
    Serial.print(": Version :");
    Serial.print(bno08x.prodIds.entry[n].swVersionMajor);
    Serial.print(".");
    Serial.print(bno08x.prodIds.entry[n].swVersionMinor);
    Serial.print(".");
    Serial.print(bno08x.prodIds.entry[n].swVersionPatch);
    Serial.print(" Build ");
    Serial.println(bno08x.prodIds.entry[n].swBuildNumber);
  }
}

/**
 * @brief Set up BNO085 sensor to report orientation
 */
void setReports(void) {
  Serial.println("Setting desired reports");
  if (!bno08x.enableReport(SH2_GAME_ROTATION_VECTOR)) {
    Serial.println("Could not enable game vector");
  }
}

/**
 * @brief Read and print BNO085 orientation data
 */
void bno085Read() {
  if (bno08x.wasReset()) {
    Serial.print("sensor was reset ");
    setReports(); // Reset reports if sensor resets
  }

  if (!bno08x.getSensorEvent(&sensorValue)) {
    return;
  }

  switch (sensorValue.sensorId) {
    
    case SH2_GAME_ROTATION_VECTOR:
      Serial.print("Game Rotation Vector - r: ");
      Serial.print(sensorValue.un.gameRotationVector.real);
      Serial.print(" i: ");
      Serial.print(sensorValue.un.gameRotationVector.i);
      Serial.print(" j: ");
      Serial.print(sensorValue.un.gameRotationVector.j);
      Serial.print(" k: ");
      Serial.println(sensorValue.un.gameRotationVector.k);
      break;
  }
}

/**
 * @brief Show "Hello" text on OLED display
 */
void oledText() {
  display.setRotation(0);                 // Rotate display if needed
  display.clearDisplay();                 // Clear screen
  display.setTextSize(2);                 // Larger text
  display.setTextColor(WHITE);            // White text
  display.setCursor(0, 10);               // Start near top-left
  display.println("Hallo");               // Print "Hallo"
  display.println("Mentoring Board! :)"); // Print message
  display.display();                      // Show on screen
}

/**
 * @brief Show a logo on OLED display
 */
void oledLogo() {
  static const uint8_t image_data[1024] = {
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x00, 0x3f, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x1f, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x1f, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x00, 0x3f, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xf8, 0x00, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 0x1f, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 0x3f, 0xff, 0xff, 0xff, 0x80, 0x00, 0x0f, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 0x3f, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x0f, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xf8, 0x00, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x0f, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x0f, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x06, 0x00, 0x00, 0x0f, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xc0, 0x07, 0xff, 0xf0, 0x01, 0xf8, 0x00, 0x00, 0x0f, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0x00, 0x01, 0xff, 0xf8, 0x7f, 0xc0, 0x00, 0x00, 0x0f, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0x00, 0x01, 0xff, 0xff, 0xfe, 0x00, 0xe0, 0x00, 0x0f, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xfe, 0x00, 0x01, 0xff, 0xff, 0xf0, 0x0f, 0xe0, 0x00, 0x0f, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xfe, 0x00, 0x00, 0xff, 0xff, 0xf8, 0x7f, 0xe0, 0x00, 0x03, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xe0, 0x08, 0x00, 0x7f, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xf8, 0x00, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xe0, 0x1f, 0x80, 0x0f, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xf8, 0x00, 0x04, 0x0f, 0xff, 0xff, 0xff, 0xe0, 0x1f, 0xf8, 0x01, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xf0, 0x00, 0x0e, 0x07, 0xff, 0xff, 0xff, 0xe0, 0x1f, 0xff, 0x80, 0x7f, 0xff, 
    0xff, 0xff, 0xff, 0xf0, 0x00, 0x0f, 0x03, 0xff, 0xff, 0xff, 0xe0, 0x1f, 0xff, 0xe0, 0x3f, 0xff, 
    0xff, 0xff, 0xff, 0xe0, 0x00, 0x07, 0xef, 0xff, 0xff, 0xff, 0xe0, 0x3f, 0xff, 0xe0, 0x3f, 0xff, 
    0xff, 0xff, 0xff, 0xe0, 0x1c, 0x01, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x3f, 0xff, 0xe0, 0x3f, 0xff, 
    0xff, 0xff, 0xff, 0xc0, 0x3f, 0x80, 0x3f, 0xff, 0xff, 0xff, 0xe0, 0x3f, 0xff, 0xe0, 0x3f, 0xff, 
    0xff, 0xff, 0xff, 0xc0, 0x7f, 0xf0, 0x0f, 0xff, 0xff, 0xff, 0xe0, 0x3f, 0xff, 0xe0, 0x3f, 0xff, 
    0xff, 0xff, 0xff, 0x80, 0x7f, 0xfc, 0x03, 0xff, 0xff, 0xff, 0xe0, 0x30, 0x00, 0x00, 0x3f, 0xff, 
    0xff, 0xff, 0xff, 0x80, 0xff, 0xfe, 0x07, 0xff, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x0f, 
    0xff, 0xff, 0xff, 0x01, 0xff, 0xfe, 0x07, 0xff, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 
    0xff, 0xff, 0xff, 0x01, 0xff, 0xfc, 0x0f, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 
    0xff, 0xff, 0xfe, 0x03, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 
    0xff, 0xff, 0xfe, 0x07, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 
    0xff, 0xff, 0xfc, 0x07, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 
    0xff, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 
    0xff, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 
    0xff, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 
    0xff, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 
    0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 
    0xfc, 0x07, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xf8, 0xf9, 0xc1, 0x9e, 0xc0, 0xf1, 0xf0, 0xf7, 0x3d, 0xf1, 0xfc, 0x3f, 0x8f, 0xe7, 0x87, 0xc3, 
    0xf8, 0xf0, 0xc1, 0x8e, 0x80, 0xe0, 0xe0, 0x62, 0x3c, 0xe0, 0x7c, 0x0f, 0x07, 0xc7, 0x81, 0x81, 
    0xf8, 0xf0, 0xc1, 0x8e, 0x80, 0xc0, 0x60, 0x66, 0x1c, 0xc0, 0x7c, 0x0e, 0x03, 0xc7, 0x81, 0x80, 
    0xf8, 0x70, 0xdf, 0x8e, 0xf3, 0x8e, 0x66, 0x66, 0x1c, 0xcf, 0x7c, 0xce, 0x73, 0xc7, 0x99, 0x9c, 
    0xf8, 0x70, 0xdf, 0x8e, 0xf3, 0x9e, 0x66, 0x66, 0x0c, 0xcf, 0xfc, 0xec, 0x73, 0xd3, 0x99, 0x9c, 
    0xf8, 0x74, 0xdf, 0xa6, 0xf3, 0x9e, 0x66, 0x66, 0x0c, 0x9f, 0xfc, 0xec, 0xf9, 0x93, 0x99, 0x9e, 
    0xf8, 0x64, 0xdf, 0xa6, 0xf3, 0x9f, 0x66, 0x66, 0x0d, 0x9f, 0xfc, 0xcc, 0xf9, 0x93, 0x99, 0x9e, 
    0xf8, 0x64, 0xc3, 0xa6, 0xf3, 0x9f, 0x62, 0x66, 0x6d, 0x9f, 0xfc, 0x0c, 0xf9, 0x93, 0x99, 0x9e, 
    0xf9, 0x24, 0xc1, 0xb2, 0xf3, 0x9f, 0x20, 0x66, 0x6d, 0x98, 0x7c, 0x1c, 0xf9, 0x93, 0x81, 0x9e, 
    0xf9, 0x24, 0xc3, 0xb2, 0xf3, 0x9f, 0x60, 0xe6, 0x65, 0x98, 0x7c, 0x0c, 0xf9, 0xbb, 0x83, 0x9e, 
    0xf9, 0x2c, 0xdf, 0xb2, 0xf3, 0x9f, 0x60, 0x66, 0x61, 0x98, 0x7c, 0xc4, 0xf9, 0xb9, 0x83, 0x9e, 
    0xf9, 0x2c, 0xdf, 0xb2, 0xf3, 0x9e, 0x66, 0x66, 0x31, 0x9e, 0x7c, 0xe4, 0xf9, 0x01, 0x91, 0x9e, 
    0xf9, 0x0c, 0xdf, 0xb8, 0xf3, 0x9e, 0x66, 0x66, 0x20, 0x8e, 0x7c, 0xe4, 0xf3, 0x01, 0x99, 0x9e, 
    0xf9, 0x8c, 0xdf, 0xb8, 0xf3, 0x8e, 0x66, 0x66, 0x30, 0xce, 0x7c, 0xe4, 0x73, 0x31, 0x99, 0x9c, 
    0xf9, 0x8c, 0xc1, 0xb8, 0xf3, 0xc0, 0x66, 0x26, 0x38, 0xc0, 0x7c, 0x06, 0x03, 0x39, 0x99, 0x80, 
    0xf9, 0x9c, 0xc1, 0xbc, 0xf3, 0xc0, 0xe7, 0x36, 0x38, 0xc0, 0x7c, 0x0e, 0x07, 0x7c, 0x9c, 0x81, 
    0xf9, 0x9c, 0xc1, 0xbc, 0xf3, 0xe0, 0xe7, 0x36, 0x38, 0xe0, 0x7c, 0x1f, 0x06, 0x7c, 0x9c, 0x83, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
  };
  display.setRotation(0);                           // Rotate display if needed
  display.clearDisplay();                           // Clear screen
  display.drawBitmap(0, 0, image_data, 128, 64, 1); // Draw logo
  display.display();                                // Show on screen
}

/**
 * @brief Show VCNL4040 data on OLED display
 */
void oledVCNL4040() {
  display.setRotation(0);           // Rotate display if needed
  display.clearDisplay();           // Clear screen
  display.setTextColor(WHITE);      // White text
  display.setTextSize(1);           // Normal text size
  display.setCursor(0, 0);          // Start at top-left
  display.println("VCNL4040");      // Print title
  display.println(" ");
  vcnl4040Read();                   // Get and display sensor data
  display.display();                // Show on screen
}

/**
 * @brief Write a message to a file on SD card
 */
void writeFile(fs::FS &fs, const char *path, const char *message) {
  Serial.printf("Writing file: %s\n", path);
  File file = fs.open(path, FILE_WRITE);
  if (!file) {
    Serial.println("Failed to open file for writing");
    return;
  }
  if (file.print(message)) {
    Serial.println("File written");
  } else {
    Serial.println("Write failed");
  }
  file.close();
}

/**
 * @brief Initialize and check SD card
 */
void sdCard() {
  if (!SD.begin(SD_CS)) {
    Serial.println("SD card initialization failed!");
    return;
  }
  uint8_t cardType = SD.cardType();
  if (cardType == CARD_NONE) {
    Serial.println("No SD card found!");
    return;
  }
  Serial.print("SD card type: ");
  switch (cardType) {
    case CARD_MMC: Serial.println("MMC"); break;
    case CARD_SD: Serial.println("SDSC"); break;
    case CARD_SDHC: Serial.println("SDHC"); break;
    default: Serial.println("Unknown type"); break;
  }
  Serial.printf("SD card size: %lluMB\n", SD.cardSize() / (1024 * 1024));
  #if defined SD_CARD_WRITE
    writeFile(SD, "/test.txt", "Hallo Mentoring Board. Ich bin ein kleines Beispiel wie man eine SD-Karte mit einem ESP32 benutzt! :)");
  #endif
  Serial.printf("Total space: %lluMB\n", SD.totalBytes() / (1024 * 1024));
  Serial.printf("Used space: %lluMB\n", SD.usedBytes() / (1024 * 1024));
}

/**
 * @brief Read SHT4x sensor data
 */
void sht4xRead() {
  sensors_event_t humidity, temp;

  sht4x.getEvent(&humidity, &temp); // Read humidity and temperature
  Serial.println(String("Temperature: ") + temp.temperature + "°C");
  Serial.println(String("Humidity: ") + humidity.relative_humidity + "%");
}