/*
 * Copyright (c) 2021, Sensirion AG
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * * Neither the name of Sensirion AG nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <Arduino.h>
#include <SensirionI2CScd4x.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <SPI.h>
#include <Adafruit_GFX.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

SensirionI2CScd4x scd4x;
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void printUint16Hex(uint16_t value) {
    if (value < 4096) Serial.print("0");
    if (value < 256) Serial.print("0");
    if (value < 16) Serial.print("0");
    Serial.print(value, HEX);
}

void printSerialNumber(uint16_t serial0, uint16_t serial1, uint16_t serial2) {
    Serial.print("Serial: 0x");
    printUint16Hex(serial0);
    printUint16Hex(serial1);
    printUint16Hex(serial2);
    Serial.println();
}

void checkError(uint16_t error, const char* message) {
    if (error) {
        char errorMessage[256];
        Serial.print("Error trying to execute ");
        Serial.print(message);
        Serial.print(": ");
        errorToString(error, errorMessage, sizeof(errorMessage));
        Serial.println(errorMessage);
    }
}

void displayData(uint16_t co2, float temperature, float humidity) {
    display.clearDisplay();
    display.setTextColor(WHITE);
    display.setTextSize(2);
    display.setCursor(0, 0);
    display.print("Room-Moni:");

    display.setCursor(0, 32);
    display.setTextSize(1);
    display.print("CO2:   ");
    display.println(co2);
    display.print("Temp:  ");
    display.println(temperature);
    display.print("Humidity: ");
    display.println(humidity);
    display.display();
}

void displayVentilationWarning(uint16_t co2) {
    display.clearDisplay();
    display.setCursor(0,0);
    display.setTextSize(2);
    display.print("Ventilate!");

    display.setCursor(0,32);
    display.print("CO2: ");
    display.println(co2);
    display.display();
}

void setup() {
    Serial.begin(115200);

    if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
        Serial.println(F("SSD1306 allocation failed"));
        for (;;);
    }

    display.display();
    delay(2000);
    display.clearDisplay();

    while (!Serial) {
        delay(100);
    }

    Wire.begin();
    scd4x.begin(Wire);

    checkError(scd4x.stopPeriodicMeasurement(), "stopPeriodicMeasurement");

    uint16_t serial0, serial1, serial2;
    uint16_t error = scd4x.getSerialNumber(serial0, serial1, serial2);
    checkError(error, "getSerialNumber");
    if (!error) {
        printSerialNumber(serial0, serial1, serial2);
    }

    checkError(scd4x.startPeriodicMeasurement(), "startPeriodicMeasurement");

    Serial.println("Waiting for first measurement... (5 sec)");
}

void loop() {
    uint16_t error;
    uint16_t co2;
    float temperature, humidity;

    delay(5000);
    error = scd4x.readMeasurement(co2, temperature, humidity);
    if (error) {
        checkError(error, "readMeasurement");
    } else if (co2 == 0) {
        Serial.println("Invalid sample detected, skipping.");
    } else {
        displayData(co2, temperature, humidity);
        if (co2 > 1000) {
            delay(5000); // Show the data for 5 seconds
            displayVentilationWarning(co2);
        }
    }
}
