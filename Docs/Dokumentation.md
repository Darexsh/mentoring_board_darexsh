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

___________

### Dokumentation des Hardware-Teils des ESP32-S3-Mentoring-Board-Projekts

#### Übersicht
Die Hardware des ESP32-S3-Mentoring-Board-Projekts ist ein vielseitiges Mentoring-Board, das eine breite Palette von Eingabegeräten, Sensoren, Aktoren und Schnittstellen integriert. Das System basiert auf dem ESP32-S3-WROOM-1U-Modul und ist darauf ausgelegt, Lernenden und Entwicklern eine Plattform zu bieten, um Mikrocontroller-Programmierung, Sensorintegration und Hardware-Software-Interaktion zu erlernen und zu erweitern. Die Schaltpläne decken verschiedene Aspekte ab, darunter Stromversorgung, Mikrocontroller, Eingaben, Sensoren, Ausgaben und Schnittstellen.

#### Detaillierte Beschreibung der Schaltpläne

##### Schaltplan 1: ADS1015 Analog-to-Digital Converter
- **Komponenten**:
  - **U2**: ADS1015 (4-Kanal-ADC, I²C-Schnittstelle).
  - **R20, R21**: 10 kΩ Pull-up-Widerstände für SDA und SCL.
  - **C25**: 100 nF Entkopplungskondensator.
- **Beschreibung**:
  - Der ADS1015 ist an die I²C-Schnittstelle (SDA, SCL) angeschlossen.
  - Adresse: 0x72 oder 0x73, abhängig von Pin 7 (ADDR).
  - Versorgung: +3V3, entkoppelt durch C25.
  - Eingänge: AIN0 bis AIN3 für analoge Messungen (z. B. Potentiometer POTI1).

##### Schaltplan 2: Tactile Switches und Push Buttons (LEDs On/Off)
- **Komponenten**:
  - **SW1, SW2**: Taktschalter (SW-EG1218).
  - **BTN1, BTN2**: Taster (BTN-TL3305).
  - **R23, R24**: 1 kΩ Pull-up-Widerstände für SW1, SW2.
  - **R25, R26**: 1 kΩ Vorwiderstände für LEDs.
  - **GREEN150060S0575000**: Grüne LEDs.
- **Beschreibung**:
  - SW1 und SW2 sind mit Pull-up-Widerständen an +3V3 angeschlossen und geben Signale SW1 und SW2 aus.
  - BTN1 und BTN2 steuern LEDs: Wenn gedrückt, leuchtet die LED (Strom fließt durch R25/R26).

##### Schaltplan 3: JTAG und USB
- **Komponenten**:
  - **J5**: JTAG-Header (10-polig).
  - **J6**: USB-C-Anschluss.
  - **R8, R9**: 5.1 kΩ Pull-down-Widerstände für USB-Linien (CC1, CC2).
- **Beschreibung**:
  - JTAG-Header für Debugging (Signale: TCK, TDI, TDO, TMS, etc.).
  - USB-C-Anschluss für Stromversorgung (VBUS) und Daten (D+, D-).
  - R8 und R9 konfigurieren den USB-Port als Gerät.

##### Schaltplan 4: Input Voltage Selection
- **Komponenten**:
  - **R7, R8**: 120R Widerstände für LEDs.
  - **SW3**: Schalter (SW-EG1218) zur Auswahl der Eingangsspannung.
  - **F2**: Sicherung (FUSE-3.15A).
  - **D5**: Diode (ESD5Z3.3).
- **Beschreibung**:
  - SW3 wählt zwischen USB (VBUS) und externer Spannung (VIN).
  - LEDs zeigen die gewählte Spannungsquelle an.
  - F2 schützt vor Überstrom; D5 schützt vor ESD.

##### Schaltplan 5: Encoder mit 2H-Puls
- **Komponenten**:
  - **ENCODER**: Drehgeber (PEC12R-4220F-S0024).
  - **R30, R31**: 10 kΩ Pull-up-Widerstände.
  - **C28, C29**: 100 nF Entkopplungskondensatoren.
  - **R32**: 1 kΩ Vorwiderstand für SW_ENCODER.
- **Beschreibung**:
  - Der Encoder hat zwei Ausgänge (ENCODER_A, ENCODER_B) und einen Taster (SW_ENCODER).
  - R30 und R31 ziehen die Signale hoch; C28 und C29 filtern Störungen.

##### Schaltplan 6: ESP32-S3-WROOM-1U
- **Komponenten**:
  - **ESP32-S3-WROOM-1U**: Mikrocontroller-Modul.
  - **C1, C2**: 100 nF Entkopplungskondensatoren.
  - **R1, R2**: 10 kΩ Pull-up-Widerstände für EN und BOOT.
- **Beschreibung**:
  - Das Modul ist mit +3V3 versorgt.
  - GPIO-Pins sind mit Signalen verbunden (z. B. SDA, SCL, ENCODER_A, ENCODER_B, SW1, SW2, BTN1, BTN2).
  - R1 und R2 sorgen für einen definierten Boot-Modus.

##### Schaltplan 7: BNO085 Gyroscope
- **Komponenten**:
  - **BNO085**: 9-Achsen-Sensor (I²C).
  - **R16, R17**: 10 kΩ Pull-up-Widerstände für SDA und SCL.
  - **C20**: 100 nF Entkopplungskondensator.
  - **R19**: 10 kΩ Pull-down für BOOTN.
- **Beschreibung**:
  - BNO085 ist an I²C angeschlossen (Adresse 0x4A oder 0x4B, abhängig von Pin 7).
  - Steuerleitungen: NRST, BOOTN, INT, CSN.
  - C20 entkoppelt die Versorgung.

##### Schaltplan 8: LDO (Low Dropout Regulators)
- **Komponenten**:
  - **U5, U6**: TVL1117-3.3CDCYR (LDO-Regler, 3.3V Ausgang).
  - **C30, C31, C6**: 1 µF Entkopplungskondensatoren.
  - **F1, F3**: Sicherungen (FUSE-3.15A).
  - **D1**: Diode (ESD5Z3.3).
- **Beschreibung**:
  - U5 und U6 wandeln +5V in +3V3 um.
  - F1 und F3 schützen die Eingänge; D1 schützt vor ESD.
  - C30, C31 und C6 entkoppeln die Spannungen.

##### Schaltplan 9: WS2812B-2020 LEDs
- **Komponenten**:
  - **LED1 bis LED12**: WS2812B-2020 (RGB-LEDs).
  - **C7 bis C18**: 100 nF Entkopplungskondensatoren.
  - **R11**: 330R Vorwiderstand.
- **Beschreibung**:
  - 12 WS2812B-LEDs sind in Reihe geschaltet.
  - Signal: LED_WS2812B_GPIO (DIN).
  - Jede LED hat einen Entkopplungskondensator (C7 bis C18).
  - R11 begrenzt den Strom am ersten DIN-Eingang.

##### Schaltplan 10: Joystick
- **Komponenten**:
  - **U3**: Joystick (COM-09032).
  - **R22**: 10 kΩ Pull-up-Widerstand für JOY_BTN.
- **Beschreibung**:
  - Joystick mit X/Y-Achsen (JOY_X, JOY_Y) und Taster (JOY_BTN).
  - JOY_X und JOY_Y geben analoge Signale; JOY_BTN ist digital mit Pull-up.

##### Schaltplan 11: Time-of-Flight Sensor (VCNL4040)
- **Komponenten**:
  - **IC3**: VCNL4040 (Näherungs- und Umgebungslichtsensor, I²C).
  - **R30**: 2.2 kΩ Pull-up-Widerstand für INT.
  - **C26, C27**: 100 nF Entkopplungskondensatoren.
- **Beschreibung**:
  - VCNL4040 ist an I²C angeschlossen (SDA, SCL).
  - INT-Signal für Unterbrechungen.
  - C26 und C27 entkoppeln die Versorgung.

##### Schaltplan 12: Potentiometer (Poti 10k)
- **Komponenten**:
  - **RV1, RV2**: 10 kΩ Potentiometer.
- **Beschreibung**:
  - Zwei Potentiometer (POTI1, POTI2) zwischen +3V3 und GND.
  - Ausgangssignale: POTI1 und POTI2 (analoge Spannung).

##### Schaltplan 13: Micro SD-Card Reader
- **Komponenten**:
  - **J6**: Micro-SD-Kartensteckplatz.
  - **C1**: 100 nF Entkopplungskondensator.
- **Beschreibung**:
  - SD-Karte über SPI angeschlossen (SD_MOSI, SD_MISO, SD_CLK, SD_CS).
  - Versorgung: +3V3, entkoppelt durch C1.

#### Zusammenfassung der Hardware
Das Mentoring-Board ist ein umfassendes System mit:
- **Zentraler Mikrocontroller**: ESP32-S3-WROOM-1U.
- **Eingaben**:
  - Taster (BTN1, BTN2), Taktschalter (SW1, SW2).
  - Joystick (JOY_X, JOY_Y, JOY_BTN).
  - Encoder (ENCODER_A, ENCODER_B, SW_ENCODER).
  - Potentiometer (POTI1, POTI2).
- **Sensoren**:
  - ADS1015 (4-Kanal-ADC).
  - BNO085 (9-Achsen-Sensor).
  - VCNL4040 (Näherungs- und Umgebungslichtsensor).
- **Ausgaben**:
  - WS2812B-LEDs (12 Stück).
  - Grüne LEDs (über BTN1, BTN2).
- **Schnittstellen**:
  - I²C (SDA, SCL) für Sensoren.
  - SPI für SD-Karte (SD_MOSI, SD_MISO, SD_CLK, SD_CS).
  - USB-C und JTAG für Programmierung/Debugging.
- **Stromversorgung**:
  - Auswahl zwischen USB (VBUS) und externer Spannung (VIN).
  - LDOs (TVL1117-3.3) für +3V3.
