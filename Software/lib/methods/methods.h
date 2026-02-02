#ifndef METHODS_H
#define METHODS_H

#include <Arduino.h>
#include <USB.h>
#include <USBCDC.h>
#include <FastLED.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_VCNL4040.h>
#include <Adafruit_ADS1X15.h>
#include <Adafruit_BNO08x.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SHT4x.h>
#include <SD.h>
#include <FS.h>
#include <SPI.h>
#include <pinConfig.h>
#include <select_sensory.h>

/**
 * @brief Function declarations for sensor and hardware interactions
 */

/**
 * @brief Initialize GPIO pins from pinConfig.h
 */
void initPins();

/**
 * @brief Set up I2C sensors (e.g., VCNL4040, ADS1015, BNO085)
 */
void initI2CSensors();

/**
 * @brief Read and display states of push buttons
 */
void pushButtons();

/**
 * @brief Read and display states of tactile switches
 */
void tactileSwitches();

/**
 * @brief Read analog value from potentiometer
 */
void analogReadPoti();

/**
 * @brief Measure and display potentiometer voltage
 */
void voltageMeasuring();

/**
 * @brief Read joystick X, Y, and button values
 */
void readJoystick();

/**
 * @brief Display rainbow effect on WS2812 LEDs
 */
void rainbowLEDs();

/**
 * @brief Read rotary encoder position
 */
void readEncoder();

/**
 * @brief Update LED states based on encoder position
 */
void updateLEDs(int position);

/**
 * @brief Set individual LED on/off state
 */
void setLEDs(int ledIndex, int ledState);

/**
 * @brief Read data from VCNL4040 proximity/light sensor
 */
void vcnl4040Read();

/**
 * @brief Read data from ADS1015 analog-to-digital converter
 */
void ads1015Read();

/**
 * @brief Initialize and test BNO085 sensor
 */
void chipInformation(void);

/**
 * @brief Configure reporting for BNO085 sensor
 */
void setReports();

/**
 * @brief Read orientation data from BNO085 sensor
 */
void bno085Read();

/**
 * @brief Display text on OLED screen
 */
void oledText();

/**
 * @brief Display logo on OLED screen
 */
void oledLogo();

/**
 * @brief Display VCNL4040 sensor data on OLED screen
 */
void oledVCNL4040();

/**
 * @brief Write a message to a file on SD card
 */
void writeFile(fs::FS &fs, const char * path, const char * message);

/**
 * @brief Initialize and test SD card functionality
 */
void sdCard();

/**
 * @brief Read temperature and humidity data from SHT4x sensor
 */
void sht4xRead();

#endif // METHODS_H