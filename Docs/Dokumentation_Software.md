### Dokumentation des Software-Teils des ESP32-S3-Mentoring-Board-Projekts

#### Übersicht
Die Software für das ESP32-S3-Mentoring-Board-Projekt ist eine umfassende Lösung, die entwickelt wurde, um die Funktionalität eines Mentoring-Boards voll auszuschöpfen. Sie integriert eine Vielzahl von Hardwarekomponenten wie Sensoren, Eingabegeräte und Ausgabekomponenten, die über GPIO, I²C, SPI und serielle Schnittstellen angesprochen werden. Die Software ist modular aufgebaut, wobei einzelne Funktionen durch Makros in `select_sensory.h` aktiviert oder deaktiviert werden können. Dies ermöglicht eine hohe Anpassungsfähigkeit und erleichtert die Nutzung für verschiedene Lern- und Entwicklungsstufen.

#### Softwarestruktur
Die Software ist in mehrere Dateien aufgeteilt:
1. **Hauptdatei**: Enthält die `setup()`- und `loop()`-Funktionen, die die Initialisierung und Hauptschleife steuern.
2. **`methods.h`**: Deklariert Funktionen und globale Variablen für die Hardwareinteraktion.
3. **`methods.cpp`**: Implementiert die Funktionen zur Steuerung der Hardwarekomponenten.
4. **`select_sensory.h`**: Definiert Makros zur Aktivierung/Deaktivierung von Funktionen.
5. **`pinConfig_mentoringBoard.h`**: Enthält Pin-Definitionen für das Mentoring-Board.

#### Setup-Funktion
Die `setup()`-Funktion initialisiert die Hardware und Kommunikationsschnittstellen:
- **Serielle Kommunikation**: `Serial.begin(115200)` setzt die Baudrate auf 115200, um serielle Ausgaben zu ermöglichen.
- **GPIO-Initialisierung**: `initPins()` konfiguriert alle Eingabe- und Ausgabepins gemäß `pinConfig_mentoringBoard.h`.
- **I²C-Schnittstelle**: `Wire.begin(SDA, SCL)` startet die I²C-Kommunikation mit den Pins SDA (14) und SCL (13).
- **I²C-Sensoren**: `initializeI2Csensory()` initialisiert Sensoren wie VCNL4040, ADS1015, BNO085 und das SSD1306-OLED-Display.
- **WS2812-LEDs**: Bei aktiviertem `RAINBOW_WS2812` wird die NeoPixelBus-Bibliothek mit `strip.Begin()` und `strip.Show()` initialisiert.
- **Encoder-Interrupts**:
  - Bei `ENCODER_SIMPLE`: `attachInterrupt()` bindet `encoderISR()` an die Pins ENCODER_A (37) und ENCODER_B (36) bei Änderungen (CHANGE).
  - Bei `ENCODER_WITH_WS2812`: Interrupts für `readEncoder()` und `resetWS2812Encoder()` werden an ENCODER_A (37) und SW_ENCODER (35) gebunden.
- **SD-Karte**: Bei `SD_CARD_SIMPLE` wird `initializeSDCard()` aufgerufen, und eine Testnachricht wird in "/test.txt" geschrieben.
- **OLED**: Bei `OLED_SIMPLE` wird `printOnOLED()` aufgerufen, um eine Begrüßungsnachricht anzuzeigen.

#### Loop-Funktion
Die `loop()`-Funktion führt die aktivierten Funktionen kontinuierlich aus:
- **`PUSH_BUTTONS_SIMPLE`**: Ruft `pushButtons()` auf.
- **`TACTILE_SWITCHES_SIMPLE`**: Ruft `tactileSwitches()` auf.
- **`READ_ANALOG_POTI`**: Ruft `analogReadPoti()` auf.
- **`READ_JOYSTICK`**: Ruft `readJoystick()` auf.
- **`RAINBOW_WS2812`**: Ruft `setRainbow()` auf.
- **`ENCODER_SIMPLE`**: Ruft `displayEncoderPos()` auf.
- **`ENCODER_WITH_WS2812`**: Prüft Änderungen in `encoderLedPos` und ruft `updateLEDs()` auf.
- **`VCNL4040_SIMPLE`**: Ruft `readVCNL4040()` auf.
- **`READ_ADS1015`**: Ruft `readADS1015()` auf.
- **`BNO085_SIMPLE`**: Ruft `readBNO085()` auf.

#### Detaillierte Funktionsbeschreibung (aus `methods.cpp`)
1. **`void initPins()`**
   - **Zweck**: Konfiguriert GPIO-Pins als Eingänge oder Ausgänge.
   - **Details**: Setzt Pins wie BTN1 (15), BTN2 (16), SW1 (17), SW2 (18), POTI2 (8), JOY_X (1), JOY_Y (2), JOY_BTN (38), ENCODER_A (37), ENCODER_B (36), SW_ENCODER (35), LED_WS2812 (21) usw. entsprechend ihrer Funktion.

2. **`void initializeI2Csensory()`**
   - **Zweck**: Initialisiert I²C-Sensoren und das OLED-Display.
   - **Details**: 
     - Prüft die Verbindung zu VCNL4040 (Adresse 0x60), ADS1015 (0x72), BNO085 (0x4A) und SSD1306 (0x3C).
     - Gibt Fehlermeldungen seriell aus, falls ein Gerät nicht gefunden wird.

3. **`void pushButtons()`**
   - **Zweck**: Liest den Status von zwei Tastern und gibt ihn aus.
   - **Details**: Liest digitale Werte von BTN1 und BTN2 mit `digitalRead()` und gibt sie als String über `Serial.println()` aus.

4. **`void tactileSwitches()`**
   - **Zweck**: Liest den Status von zwei Taktschaltern.
   - **Details**: Ähnlich wie `pushButtons()`, liest SW1 und SW2 und gibt die Werte seriell aus.

5. **`void analogReadPoti()`**
   - **Zweck**: Liest den analogen Wert eines Potentiometers.
   - **Details**: Liest POTI2 (Pin 8) mit `analogRead()` und gibt den Wert seriell aus.

6. **`void readJoystick()`**
   - **Zweck**: Liest X/Y-Werte und den Tasterstatus eines Joysticks.
   - **Details**: Liest JOY_X (1), JOY_Y (2) analog und JOY_BTN (38) digital; gibt Werte seriell aus.

7. **`void setRainbow()`**
   - **Zweck**: Erzeugt einen Regenbogeneffekt auf WS2812-LEDs.
   - **Details**: 
     - Iteriert über `NUM_LEDS_WS2812` (12) LEDs.
     - Nutzt `HslColor` und `RgbwColor` zur Farbberechnung basierend auf `hue`.
     - Aktualisiert LEDs mit `strip.Show()`; `hue` wird inkrementell erhöht.

8. **`void displayEncoderPos()`**
   - **Zweck**: Zeigt die Position eines einfachen Encoders an.
   - **Details**: 
     - Setzt `encoderPos` bei gedrücktem SW_ENCODER (35) auf 0 zurück.
     - Gibt `encoderPos` seriell aus; Verzögerung von 100 ms.

9. **`void encoderISR()`**
   - **Zweck**: Interrupt-Service-Routine für den einfachen Encoder.
   - **Details**: 
     - Liest ENCODER_A und ENCODER_B, kombiniert sie zu einem 4-Bit-Wert (`sum`).
     - Erhöht/Verringert `encoderPos` basierend auf Drehrichtung.

10. **`void IRAM_ATTR readEncoder()`**
    - **Zweck**: Interrupt für den WS2812-Encoder.
    - **Details**: 
      - Erkennt Flankenänderungen an ENCODER_A.
      - Passt `encoderLedPos` (1-23) basierend auf ENCODER_B an.

11. **`void IRAM_ATTR resetWS2812Encoder()`**
    - **Zweck**: Setzt den WS2812-Encoder zurück.
    - **Details**: Setzt `encoderLedPos` auf 1 und aktualisiert LEDs.

12. **`void sendWS2812Encoder(uint8_t r, uint8_t g, uint8_t b)`**
    - **Zweck**: Sendet Farbdaten an WS2812-LEDs.
    - **Details**: Implementiert das WS2812-Protokoll mit Bit-Banging (HIGH/LOW-Timing).

13. **`void updateLEDs()`**
    - **Zweck**: Aktualisiert WS2812-LEDs basierend auf `encoderLedPos`.
    - **Details**: 
      - Sendet Rot (255, 0, 0) an die Haupt-LED(s) und Schwarz (0, 0, 0) an andere.

14. **`bool initializeSDCard()`**
    - **Zweck**: Initialisiert die SD-Karte.
    - **Details**: 
      - Nutzt SPI-Pins (SD_CS=7, SD_CLK=5, SD_MOSI=6, SD_MISO=4).
      - Gibt Erfolg/Fehler seriell aus.

15. **`void writeToFile(const char* filename, const char* message)`**
    - **Zweck**: Schreibt eine Nachricht in eine Datei auf der SD-Karte.
    - **Details**: Öffnet die Datei im Schreibmodus und protokolliert den Erfolg.

16. **`void readVCNL4040()`**
    - **Zweck**: Liest Näherungs- und Umgebungslichtwerte.
    - **Details**: Gibt Werte von `vcnl4040.getProximity()` und `getAmbientLight()` aus.

17. **`void readADS1015()`**
    - **Zweck**: Liest Werte von vier ADC-Kanälen.
    - **Details**: Liest Kanäle 0-2 und POTI1 (Kanal 3) mit `readADC_SingleEnded()`.

18. **`void readBNO085()`**
    - **Zweck**: Liest Rotationsvektoren des BNO085.
    - **Details**: Gibt Quaternion-Daten (`real`, `i`, `j`, `k`) aus, falls verfügbar.

19. **`void printOnOLED()`**
    - **Zweck**: Zeigt Text auf dem OLED-Display an.
    - **Details**: 
      - Löscht das Display, setzt Textgröße und Position, zeigt Begrüßungsnachricht.

#### Pin-Zuordnung (basierend auf `pinConfig_mentoringBoard.h`)
- **BTN1**: GPIO 15
- **BTN2**: GPIO 16
- **SW1**: GPIO 17
- **SW2**: GPIO 18
- **POTI1**: ADS1015 Kanal 3
- **POTI2**: GPIO 8
- **JOY_X**: GPIO 1
- **JOY_Y**: GPIO 2
- **JOY_BTN**: GPIO 38
- **ENCODER_A**: GPIO 37
- **ENCODER_B**: GPIO 36
- **SW_ENCODER**: GPIO 35
- **BNO085_INT**: GPIO 9
- **BNO085_NRST**: GPIO 10
- **BNO085_CSN**: GPIO 11
- **BNO085_BOOTN**: GPIO 12
- **SD_CS**: GPIO 7
- **SD_CLK**: GPIO 5
- **SD_MOSI**: GPIO 6
- **SD_MISO**: GPIO 4
- **LED_WS2812**: GPIO 21
- **SDA**: GPIO 14
- **SCL**: GPIO 13

#### Bibliotheken
- **Grundlegend**: `Arduino.h`, `Wire.h`, `SPI.h`, `SD.h`
- **Spezifisch**: `NeoPixelBus.h`, `Adafruit_VCNL4040.h`, `Adafruit_ADS1X15.h`, `Adafruit_BNO08x.h`, `Adafruit_SSD1306.h`, `Adafruit_GFX.h`
