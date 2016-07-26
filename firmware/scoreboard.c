/*
The MIT License (MIT)

Copyright (c) 2016 Bryan Hughes <bryan@nebri.us>, Alex Glow <alex@hackster.io>,
Nadine Hachouche <nadine@polybit.com>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "application.h"
#include "dotstar/dotstar.h"

#define NUMPIXELS 11 // Number of LEDs in strip

// Hardware SPI is a little faster, but must be wired to specific pins
// (Core/Photon/P1/Electron = pin A5 for data, A3 for clock)
Adafruit_DotStar strip = Adafruit_DotStar(NUMPIXELS, DOTSTAR_BGR);

// led/r/g/b in decimal: LRRRGGGBBB
int set_color(String data) {
    int led = data.substring(0, 2).toInt();
    int r = data.substring(2, 5).toInt();
    int g = data.substring(5, 8).toInt();
    int b = data.substring(8, 11).toInt();
    strip.setPixelColor(led, r * 0x10000 + g * 0x100 + b);
    strip.show();
    return 0;
}

void setup() {
  strip.begin();
  strip.show();
  strip.setBrightness(32);
  Particle.function("set_color", set_color);
}

void loop() {
}
