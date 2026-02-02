* * *

<div align="center">

🧩 ESP32-S3 Mentoring Board
==========================

**A multi-function mentoring/training board with ESP32-S3, common sensors, inputs, and LEDs**  
🧠🛠️🎛️💡📦


![Projekt-Status](https://img.shields.io/badge/Status-Aktiv-brightgreen) ![License](https://img.shields.io/badge/License-NonCommercial-blue) ![Version](https://img.shields.io/badge/Version-1.0-orange) [![Buy Me a Coffee](https://img.shields.io/badge/Buy%20Me%20a%20Coffee-yellow?logo=buy-me-a-coffee)](https://buymeacoffee.com/darexsh)


</div>

* * *

✨ Authors
---------

| Name | GitHub | Role | Contact | Contributions |
| --- | --- | --- | --- | --- |
| **[Darexsh by Daniel Sichler](https://github.com/Darexsh)** | [Link](https://github.com/Darexsh?tab=repositories) | Hardware & Firmware Development 🖥️🛠️ | 📧 [E-Mail](mailto:sichler.daniel@gmail.com) | Concept, Hardware, Firmware, Board Bring-up |

* * *

🚀 About the Project
--------------------

This project is a **mentoring/training board** based on the ESP32-S3 microcontroller. It combines common sensors, inputs, LEDs, and storage so embedded features can be demonstrated and tested on a single board.

### 🛠️ Technologies

* **Microcontroller:** ESP32-S3-WROOM-1-N8R8  
* **Sensors:** VCNL4040 (ambient light & proximity), SHT4x (temperature & humidity), BNO085 (IMU), ADS1015 (ADC)  
* **Inputs:** push buttons, tactile switches, potentiometers, joystick, rotary encoder  
* **Outputs:** WS2812B-2020 addressable LEDs  
* **Storage:** microSD card (SPI)  
* **Display:** I2C display header  
* **Software:** C++, Arduino framework (PlatformIO)  
* **Tools:** KiCad, PlatformIO, Git  

* * *

📋 Features
===========

* **🧭 Sensor Demonstrations:** IMU, proximity/light, temperature/humidity, external ADC  
* **🎛️ Input Practice:** buttons, tactile switches, potentiometers, joystick, encoder  
* **💡 LED Control:** WS2812B addressable LEDs  
* **📦 Data Logging:** microSD over SPI  
* **🖥️ Optional I2C Display:** header for showing sensor values  
* **🧩 Modular Firmware:** enable features via compile-time flags  

* * *

🔧 Hardware Setup
-----------------

* **ESP32-S3 module**  
* **VCNL4040, SHT4x, BNO085, ADS1015** on I2C  
* **WS2812B-2020 LED chain**  
* **microSD socket** over SPI  
* **Joystick, potentiometers, buttons, encoder** for user input  
* **I2C display connector**  

* * *

🚀 Installation & Usage
-----------------------

1. Open the KiCad project in `Hardware/` if you want to inspect or modify the hardware.  
2. Build and flash the firmware with PlatformIO:  
   
   ```bash
   cd Software
   pio run -t upload
   pio device monitor
   ```
3. Enable/disable features in `Software/lib/select_sensory/select_sensory.h`.  

* * *

📜 License
----------

This project is licensed under the **Non-Commercial Software License (MIT-style) v1.0** and was developed as an educational project. You are free to use, modify, and distribute the code for **non-commercial purposes only**, and must credit the author:

**Copyright (c) 2025 Darexsh by Daniel Sichler**

Please include the following notice with any use or distribution:

> Developed by Daniel Sichler aka Darexsh. Licensed under the Non-Commercial Software License (MIT-style) v1.0. See `LICENSE` for details.

The full license is available in the [LICENSE](LICENSE) file.

* * *

<!-- Keep this line! Rendered picture of PCB is going to be displayed after Production files are pushed to branch! -->
![](./Docs/top.png)
![](./Docs/bottom.png)

<div align="center"> <sub>Created with ❤️ by Daniel Sichler</sub> </div>
