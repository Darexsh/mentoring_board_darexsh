#ifndef PINCONFIG_H
#define PINCONFIG_H

/**
 * @brief Button pin definitions
 */
#define BTN1 15  // Pin for Button 1
#define BTN2 16  // Pin for Button 2

/**
 * @brief Tactile switch pin definitions
 */
#define SW1 17  // Pin for Switch 1
#define SW2 18  // Pin for Switch 2

/**
 * @brief Potentiometer pin/channel definitions
 */
#define POTI1_ADS_Channel 3  // ADS1015 channel for Potentiometer 1
#define POTI2 8              // Pin for Potentiometer 2

/**
 * @brief Joystick pin definitions
 */
#define JOY_X 2     // Pin for Joystick X-axis
#define JOY_Y 1     // Pin for Joystick Y-axis
#define JOY_BTN 38  // Pin for Joystick button

/**
 * @brief Encoder pin definitions
 */
#define ENCODER_A 37    // Pin for Encoder A signal
#define ENCODER_B 36    // Pin for Encoder B signal
#define SW_ENCODER 35   // Pin for Encoder switch

/**
 * @brief BNO085 sensor pin definitions (Gyroscope, Accelerometer, Magnetometer)
 * Don't need to be defined if using I2C interface
 */
// #define BNO085_INT 9    // Interrupt pin for BNO085
// #define BNO085_NRST 10  // Reset pin for BNO085
// #define BNO085_CSN 11   // Chip select pin for BNO085
// #define BNO085_BOOTN 12 // Boot mode pin for BNO085

/**
 * @brief SD card SPI pin definitions
 */
#define SD_CS 7    // Chip select pin for SD card
#define SD_CLK 5   // Clock pin for SD card
#define SD_MOSI 6  // MOSI pin for SD card
#define SD_MISO 4  // MISO pin for SD card

/**
 * @brief WS2812B LED strip configuration
 */
#define LED_BRIGHTNESS 100  // Brightness level (0-255)
#define LED_TYPE WS2812B    // LED strip type
#define COLOR_ORDER GRB     // Color order for LEDs
#define LED_PIN 21          // Pin for LED strip
#define NUM_LEDS 12         // Number of LEDs in strip

/**
 * @brief OLED display configuration
 */
#define SCREEN_WIDTH 128   // OLED screen width in pixels
#define SCREEN_HEIGHT 64   // OLED screen height in pixels

/**
 * @brief I2C pin and address definitions
 */
#define SDA 14                     // I2C data pin
#define SCL 13                     // I2C clock pin
#define VCNL4040_ADDRESS 0x60      // I2C address for VCNL4040 sensor
#define ADS1015_ADDRESS 0x48       // I2C address for ADS1015 ADC
#define BNO085_ADDRESS 0x4A        // I2C address for BNO085 sensor
#define OLED_DISPLAY_ADDRESS 0x3C  // I2C address for OLED display
#define SHT4X_ADDRESS 0x44         // I2C address for SHT4x sensor

#endif // PINCONFIG_H