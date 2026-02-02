#include <Arduino.h>
#include <USB.h>
#include <USBCDC.h>
#include <Wire.h>
#include <methods.h>
#include <select_sensory.h>

CRGB leds[NUM_LEDS]; 
Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);
TwoWire I2C_MENTORING = TwoWire(0);
Adafruit_SHT4x sht4x = Adafruit_SHT4x(); 

/**
 * @brief Initialize hardware and peripherals for sensor readings
 */
void setup() {
  Serial.begin(115200); // Start USB serial communication
  delay(2000); // Wait for hardware to stabilize

  initPins();                                 // Set up GPIO pins
  I2C_MENTORING.begin(SDA, SCL);              // Start I2C bus with SDA  pins SDA and SCL
  initI2CSensors();                           // Initialize I2C sensors
  SPI.begin(SD_CLK, SD_MISO, SD_MOSI, SD_CS); // Start SPI bus for SD card 

  #if defined WS2812_RAINBOW
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip); // Initialize LED strip
    FastLED.setBrightness(LED_BRIGHTNESS); // Set brightness
  #endif

  #if defined ENCODER
    strip.begin();                       // Initialize the LED strip
    strip.setBrightness(LED_BRIGHTNESS); // Set brightness
    strip.show();                        // Turn off all LEDs initially
  #endif

  #if defined BNO085
    chipInformation(); // Print BNO085 chip information
    setReports();      // Configure BNO085 sensor reporting
  #endif

  #if defined OLED_TEXT
    oledText(); // Show text on OLED display
  #endif

  #if defined OLED_LOGO
    oledLogo(); // Show logo on OLED display
  #endif

  #if defined SD_CARD
    sdCard(); // Set up SD card
  #endif

  #if defined SHT4X
    sht4x.setPrecision(SHT4X_HIGH_PRECISION); // Set SHT4x sensor to high precision mode
  #endif

  delay(100); // Short delay for USB stability
  Serial.println(" ");
  Serial.println("Setup complete. Ready to read sensors.");
}

/**
 * @brief Main loop to read sensors and control outputs
 */
void loop() {
  #if defined PUSH_BUTTONS_SIMPLE
    pushButtons(); // Check and display button states
  #endif

  #if defined TACTILE_SWITCHES_SIMPLE
    tactileSwitches(); // Read tactile switch states
  #endif

  #if defined POTI_ANALOG
    analogReadPoti(); // Read potentiometer analog value
  #endif

  #if defined POTI_VOLTAGE
    voltageMeasuring(); // Measure potentiometer voltage
  #endif

  #if defined JOYSTICK
    readJoystick(); // Read joystick position
  #endif

  #if defined WS2812_RAINBOW
    rainbowLEDs(); // Display rainbow effect on LEDs
  #endif

  #if defined ENCODER
    readEncoder(); // Read encoder position and update LEDs
  #endif

  #if defined VCNL4040
    vcnl4040Read(); // Read proximity/light sensor data
  #endif

  #if defined ADS1015
    ads1015Read(); // Read ADC sensor values
  #endif

  #if defined BNO085
    bno085Read(); // Read orientation sensor data
  #endif

  #if defined OLED_VCNL4040
    oledVCNL4040(); // Show VCNL4040 data on OLED
  #endif

  #if defined SHT4X
    sht4xRead(); // Read temperature/humidity sensor data
  #endif

  #ifndef WS2812_RAINBOW
    #ifndef ENCODER
      Serial.println(" ");
      delay(200); // Slow down loop outputs for readability
    #endif
  #endif
}