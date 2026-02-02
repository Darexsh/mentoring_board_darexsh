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