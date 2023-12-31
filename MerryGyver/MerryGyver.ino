#include "TinyMAX7219.h"
TinyMAX7219<3, 0, 2> disp;  // CS, MOSI, SCK

// с новым годом
const uint8_t bitmap_59x5[] PROGMEM = {
  0xE4, 0xBD, 0xC8, 0xA2, 0x77, 0x8D, 0xE8, 0xA0,
  0x84, 0xA5, 0x48, 0xB6, 0x44, 0x95, 0x2D, 0xA0,
  0x87, 0xA5, 0xEE, 0xAA, 0x44, 0x95, 0x2A, 0xA0,
  0x84, 0xA5, 0x2A, 0xA2, 0x44, 0xBD, 0x28, 0x80,
  0xE4, 0xBD, 0xEE, 0xA2, 0x47, 0xA5, 0xE8, 0xA0,
};

// 2021 — уходи
const uint8_t bitmap_46x5[] PROGMEM = {
  0xEF, 0x72, 0x25, 0x17, 0x8D, 0x14, 
  0x29, 0x12, 0x24, 0xA4, 0x95, 0x34, 
  0xE9, 0x72, 0x3C, 0x44, 0x95, 0x54, 
  0x89, 0x42, 0x04, 0xA4, 0xBD, 0x90, 
  0xEF, 0x72, 0x3D, 0x17, 0xA5, 0x14, 
};

// елка, олень, снеговик, ...
const uint8_t bitmap_42x8[] PROGMEM = {
  0x18, 0x00, 0x00, 0x00, 0x05, 0x00, 
  0x18, 0x0E, 0x0E, 0x00, 0x02, 0x00, 
  0x3C, 0x0A, 0x0E, 0x18, 0x03, 0x00, 
  0x3C, 0x0E, 0x04, 0x18, 0xFF, 0x80, 
  0x7E, 0x7F, 0xDF, 0x3C, 0x7E, 0x00, 
  0xFF, 0x11, 0x04, 0x3C, 0x7C, 0x00, 
  0x18, 0x11, 0x0A, 0x7E, 0x44, 0x00, 
  0x18, 0x1F, 0x0A, 0x18, 0x44, 0x00, 
};

const uint8_t star[] PROGMEM = {0x18, 0x24, 0xe7, 0x81, 0x42, 0x81, 0x99, 0xe7};
const uint8_t pacm1[] PROGMEM = {0x3c, 0x7e, 0xdf, 0xff, 0xf0, 0xff, 0x7e, 0x3c};
const uint8_t pacm2[] PROGMEM = {0x3c, 0x7e, 0xdc, 0xf8, 0xf8, 0xfc, 0x7e, 0x3c};
const uint8_t heart1[] PROGMEM = {0x00, 0x66, 0x99, 0x81, 0x81, 0x42, 0x24, 0x18};
const uint8_t heart2[] PROGMEM = {0x00, 0x66, 0xff, 0xff, 0xff, 0x7e, 0x3c, 0x18};

byte offs = 0;
byte mode = 0;
byte mcount = 0;

void setup() {
  disp.begin();
  disp.setBright(6);
}

void loop() {
  switch (mode) {
    case 0: snow(); break;
    case 1: scrollBitmap(bitmap_59x5, 8, 5, 2); break;
    case 2: scrollBitmap(bitmap_46x5, 6, 5, 2); break;
    case 3: scrollBitmap(bitmap_42x8, 6, 8, 0); break;
    case 4: blinkBitmap(star); break;
    case 5: blinkBitmap(heart2); break;
    case 6: switchBitmap(pacm1, pacm2); break;
    case 7: switchBitmap(heart1, heart2); break;
  }
  offs++;
  if (++mcount > 80) {
    offs = mcount = 0;
    mode = (mode + 1) & 0x7;
    // mode = rnd();
    disp.clear();
  }
  delay(180);
}

void blinkBitmap(const uint8_t* bmap) {
  static bool blink;
  blink = !blink;
  if (blink) drawBitmap(bmap);
  else disp.clear();
}

void switchBitmap(const uint8_t* bmap1, const uint8_t* bmap2) {
  static bool blink;
  blink = !blink;
  if (blink) drawBitmap(bmap1);
  else drawBitmap(bmap2);
}

void drawBitmap(const uint8_t* bmap) {
  for (byte i = 0; i < 8; i++) {
    disp.buf[i] = pgm_read_byte(&bmap[i]);
  }
  disp.update();
}

void scrollBitmap(const uint8_t* bmap, byte w, byte h, byte p) {
  if (offs > (w << 3)) offs = 0;
  for (byte i = 0; i < h; i++) {
    byte num = (offs >> 3) + w * i;
    byte left = offs & 7;
    byte val1 = pgm_read_byte(&bmap[num - 1]);
    byte val2 = pgm_read_byte(&bmap[num]);
    if (offs < 8) val1 = val2, val1 = 0;
    if (left == 0) disp.buf[i + p] = val1;
    else disp.buf[i + p] = ((val1 << left) | (val2 >> (8 - left)));
  }
  disp.update();
}

void snow() {
  for (byte i = 0; i < 7; i++) {
    disp.buf[7 - i] = disp.buf[7 - i - 1];
    if (rnd() < 2) disp.buf[7 - i] >>= 1;
    if (rnd() > 5) disp.buf[7 - i] <<= 1;
  }
  disp.buf[0] = bit(rnd());
  disp.update();
}

byte rnd() {
  static uint16_t seed;
  seed = (seed * 2053ul) + 13849;
  return (seed >> 8) & 7;
}
