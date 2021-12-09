// How to use this library with a FM6126 panel, thanks goes to:
// https://github.com/hzeller/rpi-rgb-led-matrix/issues/746

#include <Arduino.h>
#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>
#include <xtensa/core-macros.h>

#include "animations.h"

/* ----------------*/
// -- HUB75E pinout
// R1 | G1
// B1 | GND
// R2 | G2
// B2 | E
//  A | B
//  C | D
// CLK| LAT
// OE | GND

/* ----------------*/
// -- left side
const uint8_t R1 = 32;
const uint8_t BL1 = 33;
const uint8_t R2 = 25;
const uint8_t BL2 = 26;
const uint8_t CH_A = 27;
const uint8_t CH_C = 14;
const uint8_t CLK = 12;
const uint8_t OE = 13;
// -- right side
const uint8_t G1 = 23;
// GND
const uint8_t G2 = 22;
const uint8_t CH_E = 21; // assign to any available pin if using panels with 1/32 scan
const uint8_t CH_B = 19;
const uint8_t CH_D = 18;
const uint8_t LAT = 5;
// GND
/* ----------------*/

const uint8_t PANEL_RES_X = 64; // Number of pixels wide of each INDIVIDUAL panel module.
const uint8_t PANEL_RES_Y = 32; // Number of pixels tall of each INDIVIDUAL panel module.
const uint8_t PANEL_CHAIN = 1;  // Total number of panels chained one to another

const uint32_t sceneDelay = 2000;

MatrixPanel_I2S_DMA* dma_display = nullptr;

const uint32_t BAUD_RATE = 115200; // serial debug port baud rate

void setupPanel() {
    HUB75_I2S_CFG::i2s_pins _pins = {R1,   G1,   BL1,  R2,   G2,  BL2, CH_A,
                                     CH_B, CH_C, CH_D, CH_E, LAT, OE,  CLK};
    // Module configuration
    HUB75_I2S_CFG mxconfig(PANEL_RES_X, // module width
                           PANEL_RES_Y, // module height
                           PANEL_CHAIN, // Chain length
                           _pins);

    mxconfig.gpio.e = CH_E;
    mxconfig.clkphase = false;
    mxconfig.driver = HUB75_I2S_CFG::FM6126A;

    dma_display = new MatrixPanel_I2S_DMA(mxconfig);
    dma_display->begin();
    dma_display->setBrightness8(90); // 0-255
}

void setup() {
    Serial.begin(BAUD_RATE);
    Serial.println("Starting tests...");

    setupPanel();

    flashScreen();
}

void loop() {
    Serial.println(".");
    // drawAnimatedText(); return;

    drawText(0);
    delay(sceneDelay);
    drawShapes(sceneDelay);
    delay(sceneDelay);
    drawColorFills(sceneDelay / 2);
    delay(sceneDelay);
}
