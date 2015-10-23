char hex_s[2];
byte data = 0;
byte io_addr = 0;

// pins 2 - 9 are used for the ISA data bus TODO: don't hardcode these

byte A = 10; // A0 Address select input
byte B = 11; // A1 Address select input

// TODO: IOWRITE writes are hardcoded at the moment
byte IOWRITE = 12; // ISA I/O write pin

void setup() {
  Serial.begin(230400);

  // Set port D & B to outputx
  DDRD = DDRD | B11111111;
  DDRB = DDRB | B11111111;

  pinMode(A, OUTPUT); // A0 Address select input
  pinMode(B, OUTPUT); // A1 Address select input

  digitalWrite(A, LOW);
  digitalWrite(B, LOW);
}

void loop() {
  while (true) {
    // io_addr is sent as (port - 0x388), the opl3 responds to addresses:
    // 0x388, 0x389, 0x39a, 0x39b,
    // so we only need to change the latter two bits of the I/O address
    io_addr = Serial.read();
    if (io_addr >= 0 && io_addr <= 3) {
      break;
    }
  }

  /*
   * PORTB pins (X = unused):
   * X, X, X, IOWRITE, A1, A0, D7, D6
   *
   * PORTD pins (X = unused):
   * D5, D4, D3, D2, D1, D0, X, X
   */

  // clear PORTB, don't touch unused pins
  PORTB &= B11100000;
  // set IOWRITE pin HIGH
  PORTB |= B00010000;

  // clear PORTD, don't touch unused pins
  PORTD &= B00000011;

  // read two ASCII hex characters from serial in
  Serial.readBytes(hex_s, 2);

  // convert hex_s to byte
  data = strtol(hex_s, NULL, 16);

  // write address to address select pins
  PORTB |= io_addr << 2;

  // write data
  PORTD |= data << 2;
  PORTB |= data >> 6;

  // 100 ns delay

  // set IOWRITE pin LOW
  PORTB &= B11101111;
}
