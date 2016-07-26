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

#include <math.h>
#include "dotstar/dotstar.h"

#define NUM_PIXELS 11
#define FADE_PRESET 1
#define PULSE_PRESET 2

struct rgb {
  double r; // percent
  double g; // percent
  double b; // percent
};

struct hsv {
  double h; // angle in degrees
  double s; // percent
  double v; // percent
};

hsv rgb2hsv(rgb in);
rgb hsv2rgb(hsv in);

// *** Fade settings ***

// Settable values
#define FADE_RATE 0
#define FADE_DEFAULT_RATE 128

// Other details
#define FADE_COLOR_SPACING 40

double fade_color_step = 0;

void fade_set_value(unsigned char type, unsigned char value);
void fade_init_colors();
void fade_update_colors();

// Pulse settings

// Settable values
#define PULSE_RATE 0
#define PULSE_DEFAULT_RATE 64
#define PULSE_HUE 1
#define PULSE_DEFAULT_HUE 200
#define PULSE_SATURATION 2
#define PULSE_DEFAULT_SATURATION 255

// Other details
double pulse_rate = 0;
double pulse_current_brightness = 0;
unsigned char pulse_current_direction = 0;

void pulse_set_value(unsigned char type, unsigned char value);
void pulse_init_colors();
void pulse_update_colors();

// Main App

Adafruit_DotStar strip = Adafruit_DotStar(NUM_PIXELS, DOTSTAR_BRG);

hsv colors[NUM_PIXELS];
unsigned char preset = 0;

void update_colors();
void display_colors();
int parse_preset(String data);

void setup() {
  strip.begin();
  strip.show();
  strip.setBrightness(32);
  Particle.function("preset", parse_preset);
}

void loop() {
  update_colors();
  display_colors();
}

int parse_preset(String data) {
  set_preset(data.substring(0, 1).toInt());
  return 0;
}

void set_preset(unsigned char new_preset) {
  preset = new_preset;
  switch (preset) {
    case FADE_PRESET:
      fade_init_colors();
      fade_set_value(FADE_RATE, FADE_DEFAULT_RATE);
      break;
    case PULSE_PRESET:
      pulse_init_colors();
      pulse_set_value(PULSE_RATE, PULSE_DEFAULT_RATE);
      pulse_set_value(PULSE_HUE, PULSE_DEFAULT_HUE);
      pulse_set_value(PULSE_SATURATION, PULSE_DEFAULT_SATURATION);
      break;
  }
  display_colors();
}

void set_value(unsigned char type, unsigned char value) {
  switch (preset) {
    case FADE_PRESET:
      fade_set_value(type, value);
      break;
    case PULSE_PRESET:
      pulse_set_value(type, value);
      break;
  }
}

void update_colors() {
  switch (preset) {
    case FADE_PRESET:
      fade_update_colors();
      break;
    case PULSE_PRESET:
      pulse_update_colors();
      break;
  }
}

void display_colors() {
  for (unsigned int i = 0; i < NUM_PIXELS; i++) {
    rgb converted_color = hsv2rgb(colors[i]);
    strip.setPixelColor(i, (int)(converted_color.r * 255), (int)(converted_color.g * 255), (int)(converted_color.b * 255));
  }
  strip.show();
}

// Helper Methods

hsv rgb2hsv(rgb in) {
  hsv out;
  double min, max, delta;

  min = in.r < in.g ? in.r : in.g;
  min = min  < in.b ? min : in.b;

  max = in.r > in.g ? in.r : in.g;
  max = max  > in.b ? max : in.b;

  out.v = max;
  delta = max - min;
  if (delta < 0.00001) {
    out.s = 0;
    out.h = 0; // undefined, maybe nan?
    return out;
  }
  if (max > 0.0) { // NOTE: if Max is == 0, this divide would cause a crash
    out.s = (delta / max);
  } else {
    // if max is 0, then r = g = b = 0
    // s = 0, v is undefined
    out.s = 0.0;
    out.h = NAN;
    return out;
  }
  if(in.r >= max) {
    out.h = (in.g - in.b) / delta; // between yellow & magenta
  } else if(in.g >= max) {
    out.h = 2.0 + (in.b - in.r) / delta; // between cyan & yellow
  } else {
    out.h = 4.0 + (in.r - in.g) / delta; // between magenta & cyan
  }

  out.h *= 60.0;

  if(out.h < 0.0) {
    out.h += 360.0;
  }

  return out;
}

rgb hsv2rgb(hsv in) {
    double hh, p, q, t, ff;
    long i;
    rgb out;

    if(in.s <= 0.0) {
      out.r = in.v;
      out.g = in.v;
      out.b = in.v;
      return out;
    }
    hh = in.h;
    if(hh >= 360.0) {
      hh = 0.0;
    }
    hh /= 60.0;
    i = (long)hh;
    ff = hh - i;
    p = in.v * (1.0 - in.s);
    q = in.v * (1.0 - (in.s * ff));
    t = in.v * (1.0 - (in.s * (1.0 - ff)));

    switch(i) {
      case 0:
        out.r = in.v;
        out.g = t;
        out.b = p;
        break;
      case 1:
        out.r = q;
        out.g = in.v;
        out.b = p;
        break;
      case 2:
        out.r = p;
        out.g = in.v;
        out.b = t;
        break;
      case 3:
        out.r = p;
        out.g = q;
        out.b = in.v;
        break;
      case 4:
        out.r = t;
        out.g = p;
        out.b = in.v;
        break;
      case 5:
      default:
        out.r = in.v;
        out.g = p;
        out.b = q;
        break;
    }
    return out;
}

// Fade Methods

void fade_set_value(unsigned char type, unsigned char value) {
  switch(type) {
    case FADE_RATE:
      fade_color_step = (double)value / 1280.0;
      break;
  }
}

void fade_init_colors() {
  for (unsigned int i = 0; i < NUM_PIXELS; i++) {
    colors[i].h = FADE_COLOR_SPACING * i; // angle
    colors[i].s = 1;
    colors[i].v = 1;
  }
}

void fade_update_colors() {
  for (unsigned int i = 0; i < NUM_PIXELS; i++) {
    colors[i].h += fade_color_step;
    if (colors[i].h >= 360.0) {
      colors[i].h = 0;
    }
  }
}

// Pulse Methods

void pulse_set_value(unsigned char type, unsigned char value) {
  switch(type) {
    case PULSE_RATE:
      pulse_rate = (double)value / 64000.0;
      break;
    case PULSE_HUE:
      for (unsigned int i = 0; i < NUM_PIXELS; i++) {
        colors[i].h = (double)value * 360.0 / 255;
      }
      break;
    case PULSE_SATURATION:
      for (unsigned int i = 0; i < NUM_PIXELS; i++) {
        colors[i].s = (double)value / 255;
      }
      break;
  }
}

void pulse_init_colors() {
  for (unsigned int i = 0; i < NUM_PIXELS; i++) {
    colors[i].h = 0;
    colors[i].s = 1;
    colors[i].v = 0;
  }
  pulse_current_brightness = 0;
  pulse_current_direction = 0;
}

void pulse_update_colors() {
  if (pulse_current_direction) {
    pulse_current_brightness += pulse_rate;
    if (pulse_current_brightness >= 1.0) {
      pulse_current_direction = 0;
      pulse_current_brightness = 1;
    }
  } else {
    pulse_current_brightness -= pulse_rate;
    if (pulse_current_brightness <= 0) {
      pulse_current_direction = 1;
      pulse_current_brightness = 0;
    }
  }
  for (unsigned int i = 0; i < NUM_PIXELS; i++) {
    colors[i].v = pulse_current_brightness;
  }
}
