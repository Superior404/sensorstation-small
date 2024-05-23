Sure, I'll update the README file based on your instructions. Here's the revised version:

---

# ESP8266 SCD41 and OLED Display Project

This project demonstrates the integration of an ESP8266 microcontroller with a CIWEI SCD41 CO2 sensor and a 1.3-inch OLED I2C display. The system reads CO2 concentration, temperature, and humidity from the SCD41 sensor and displays the data on the OLED screen.

## Hardware Components

- **ESP8266**: The microcontroller used for reading sensor data and driving the display.
- **CIWEI SCD41**: A CO2 sensor that also measures temperature and humidity.
- **1.3-inch OLED Display (I2C)**: An I2C OLED display used to show the sensor readings.

## Connections

| ESP8266 Pins | SCD41/OLED Pins |
|--------------|------------------|
| 3V3          | VCC              |
| GND          | GND              |
| D1           | SCL              |
| D2           | SDA              |

Note: The SCD41 sensor and the OLED display share the same I2C pins.

## Installation and Usage

1. **Clone this repository**:
    ```bash
    git clone https://github.com/yourusername/esp8266-scd41-oled.git
    cd esp8266-scd41-oled
    ```

2. **Install necessary libraries**:
    Ensure you have the following libraries installed in your Arduino IDE:
    - `Adafruit_GFX`
    - `Adafruit_SSD1306`
    - `SCD41`

3. **Upload the code**:
    - Connect your ESP8266 to your computer.
    - Open the code in Arduino IDE.
    - Select the correct board and port.
    - Upload the code.

4. **Connect the hardware**:
    - Wire the SCD41 sensor and the OLED display to the ESP8266 as per the connections table.

5. **Power up**:
    - Once powered, the system will start displaying CO2 concentration, temperature, and humidity readings on the OLED screen.

## Images

1. [Complete Code Image 1](images/image1.jpg)
2. [Complete Code Image 2](images/image2.jpg)

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Acknowledgments

- Adafruit for their excellent libraries.
- Sensirion for the SCD41 sensor.
