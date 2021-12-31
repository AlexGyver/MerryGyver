template <uint8_t CS, uint8_t DAT, uint8_t CLK>
struct TinyMAX7219 {
  uint8_t buf[8];

  TinyMAX7219() {
    pinMode(CS, OUTPUT);
    pinMode(DAT, OUTPUT);
    pinMode(CLK, OUTPUT);
  }
  void begin() {
    sendCMD(0x0f, 0x00);  // отключить режим теста
    sendCMD(0x09, 0x00);  // выключить декодирование
    sendCMD(0x0b, 0x0f);  // отображаем всё
    sendCMD(0x0C, 0x01);  // включить
    setBright(0);
    clear();
  }

  void update() {
    for (int i = 0; i < 8; i++) send(i, buf[i]);
  }

  void clear() {
    for (int i = 0; i < 8; i++) {
      send(i, 0);
      buf[i] = 0;
    }
  }

  void setBright(byte val) {
    sendCMD(0x0a, val);  // яркость 0-15
  }

  void send(uint8_t addr, uint8_t val) {
    sendCMD(addr + 1, val);
  }

  void sendCMD(uint8_t addr, uint8_t val) {
    digitalWrite(CS, LOW);
    sendByte(addr);
    sendByte(val);
    digitalWrite(CS, HIGH);
  }

  void sendByte(uint8_t data) {
    for (uint8_t i = 0; i < 8; i++)  {
      digitalWrite(DAT, data & (1 << 7));
      data <<= 1;
      digitalWrite(CLK, 1);
      digitalWrite(CLK, 0);
    }
  }
};
