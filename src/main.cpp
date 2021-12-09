// How to use this library with a FM6126 panel, thanks goes to:
// https://github.com/hzeller/rpi-rgb-led-matrix/issues/746

#include "xtensa/core-macros.h"
#include <Arduino.h>
#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>
//#include <FastLED.h>

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

uint16_t myBLACK = dma_display->color565(0, 0, 0);
uint16_t myWHITE = dma_display->color565(255, 255, 255);
uint16_t myRED = dma_display->color565(255, 0, 0);
uint16_t myGREEN = dma_display->color565(0, 255, 0);
uint16_t myBLUE = dma_display->color565(0, 0, 255);

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
// From: https://gist.github.com/davidegironi/3144efdc6d67e5df55438cc3cba613c8
uint16_t colorWheel(uint8_t pos) {
    if (pos < 85) {
        return dma_display->color565(pos * 3, 255 - pos * 3, 0);
    } else if (pos < 170) {
        pos -= 85;
        return dma_display->color565(255 - pos * 3, 0, pos * 3);
    } else {
        pos -= 170;
        return dma_display->color565(0, pos * 3, 255 - pos * 3);
    }
}

void drawText(int colorWheelOffset) {

    // draw text with a rotating colour
    dma_display->setTextSize(1);     // size 1 == 8 pixels high
    dma_display->setTextWrap(false); // Don't wrap at end of line - will do ourselves

    dma_display->setCursor(5, 0); // start at top left, with 8 pixel of spacing
    uint8_t w = 0;
    const char* str = "ESP32 DMA";
    for (w = 0; w < strlen(str); w++) {
        dma_display->setTextColor(colorWheel((w * 32) + colorWheelOffset));
        dma_display->print(str[w]);
    }

    dma_display->println();
    dma_display->print(" ");
    for (w = 9; w < 18; w++) {
        dma_display->setTextColor(colorWheel((w * 32) + colorWheelOffset));
        dma_display->print("*");
    }

    dma_display->println();

    dma_display->setTextColor(dma_display->color444(15, 15, 15));
    dma_display->println("LED MATRIX!");

    // print each letter with a fixed rainbow color
    dma_display->setTextColor(dma_display->color444(0, 8, 15));
    dma_display->print('3');
    dma_display->setTextColor(dma_display->color444(15, 4, 0));
    dma_display->print('2');
    dma_display->setTextColor(dma_display->color444(15, 15, 0));
    dma_display->print('x');
    dma_display->setTextColor(dma_display->color444(8, 15, 0));
    dma_display->print('6');
    dma_display->setTextColor(dma_display->color444(8, 0, 15));
    dma_display->print('4');

    // Jump a half character
    dma_display->setCursor(34, 24);
    dma_display->setTextColor(dma_display->color444(0, 15, 15));
    dma_display->print("*");
    dma_display->setTextColor(dma_display->color444(15, 0, 0));
    dma_display->print('R');
    dma_display->setTextColor(dma_display->color444(0, 15, 0));
    dma_display->print('G');
    dma_display->setTextColor(dma_display->color444(0, 0, 15));
    dma_display->print("B");
    dma_display->setTextColor(dma_display->color444(15, 0, 8));
    dma_display->println("*");
}

void setup() {
    Serial.begin(BAUD_RATE);
    Serial.println("Starting tests...");

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

    // Display Setup
    dma_display = new MatrixPanel_I2S_DMA(mxconfig);
    dma_display->begin();
    dma_display->setBrightness8(90); // 0-255
    dma_display->clearScreen();
    dma_display->fillScreen(myWHITE);
}

void drawShapes(uint32_t sceneDelay) {
    // fix the screen with green
    dma_display->fillRect(0, 0, dma_display->width(), dma_display->height(),
                          dma_display->color444(0, 15, 0));
    delay(sceneDelay);

    // draw a box in yellow
    dma_display->drawRect(0, 0, dma_display->width(), dma_display->height(),
                          dma_display->color444(15, 15, 0));
    delay(sceneDelay);

    // draw an 'X' in red
    dma_display->drawLine(0, 0, dma_display->width() - 1, dma_display->height() - 1,
                          dma_display->color444(15, 0, 0));
    dma_display->drawLine(dma_display->width() - 1, 0, 0, dma_display->height() - 1,
                          dma_display->color444(15, 0, 0));
    delay(sceneDelay);

    // draw a blue circle
    dma_display->drawCircle(10, 10, 10, dma_display->color444(0, 0, 15));
    delay(sceneDelay);

    // fill a violet circle
    dma_display->fillCircle(40, 21, 10, dma_display->color444(15, 0, 15));
    delay(sceneDelay);

    // fill the screen with 'black'
    dma_display->fillScreen(dma_display->color444(0, 0, 0));

    // drawText(0);
}

void drawAnimatedText() {
    static uint8_t wheelval = 0;

    // animate by going through the colour wheel for the first two lines
    drawText(wheelval);
    wheelval += 1;

    delay(20);
}

void drawColorFills(uint32_t sceneDelay) {
    dma_display->clearScreen();
    dma_display->fillScreen(myBLACK);
    delay(sceneDelay);
    dma_display->fillScreen(myBLUE);
    delay(sceneDelay);
    dma_display->fillScreen(myRED);
    delay(sceneDelay);
    dma_display->fillScreen(myGREEN);
    delay(sceneDelay);
    dma_display->fillScreen(myWHITE);
    dma_display->clearScreen();
}

void loop() {
    // drawAnimatedText(); return;

    drawText(0);
    delay(sceneDelay);
    drawShapes(sceneDelay);
    delay(sceneDelay);
    drawColorFills(sceneDelay / 2);
    delay(sceneDelay);
}