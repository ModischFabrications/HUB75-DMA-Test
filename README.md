# Simple ESP32-HUB75-MatrixPanel-I2S-DMA Test with PlatformIO

This is a quick test sketch for [ESP32-HUB75-MatrixPanel-I2S-DMA](https://github.com/mrfaptastic/ESP32-HUB75-MatrixPanel-I2S-DMA). 

Open using PlatformIO. Look it up and use it, it's a huge improvement over the basic Arduino IDE. 

I bought [this 64x32 panel](https://www.aliexpress.com/item/32810362851.html) and [this ESP32 (ESP32-CH9102X)](https://www.aliexpress.com/item/32959541446.html) closely researching specifications. As of now everything seems to work fine. Manual wiring is a nightmare though, get a wiring PCB asap. 

It currently draws a maximum of 1.4A at 5V, dropping lower to 0.5A with fewer LEDs and 0.06A with no LEDs shown. 

## Inspirations
- Examples from the library, notably [PIO Patterns](https://github.com/mrfaptastic/ESP32-HUB75-MatrixPanel-I2S-DMA/tree/master/examples/PIO_TestPatterns) and [Simple Pattern check](https://github.com/mrfaptastic/ESP32-HUB75-MatrixPanel-I2S-DMA/tree/master/examples/1_SimpleTestShapes)
- https://www.instructables.com/Morphing-Digital-Clock/
- https://github.com/bogd/esp32-morphing-clock


## Contributing
### CPP Formatting
Press Shift+Alt+F to apply formatting. 

This project using the Clang Formatter with a style configuration ´.clang-format´ file. 
For that install the [Clang compiler](https://github.com/nextcloud/desktop/wiki) as well as the Clang-Format extension for vscode. 
They should be recommended once checking this project out. 
