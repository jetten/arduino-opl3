char inputhex[2];
byte data = 0;

boolean addresswrite = true;

byte A = 10; // A0 Address select input
byte CS = 11; // CS Chip select
byte WR = 12; // WR Write enable
byte B = 13; // A1 Address select input


void setup() {
  
  Serial.begin(115200);
  // Set port D & B to outputx
  DDRD = DDRD | B11111111;
  DDRB = DDRB | B11111111;
  
  pinMode(A,OUTPUT); // A0 Address select input
  pinMode(B,OUTPUT); // A1 Address select input
  pinMode(CS,OUTPUT); // CS Chip select
  pinMode(WR,OUTPUT); // WR Write enable

  digitalWrite(A,LOW);
  digitalWrite(CS,HIGH);
  digitalWrite(WR,LOW);
  
}

void loop() {
  
  if(Serial.available() > 0) {
    // Read 2 characters from the serial buffer into inputhex
    Serial.readBytes(inputhex,2);

    // convert string inputhex to byte
    data = strtol(inputhex, NULL, 16);

    if(addresswrite) {
      //A LOW, CS HIGH
      PORTB = B00001000;
      
    }
    else {
      //A HIGH, CS HIGH
      PORTB = B00001100;
    }

    // 10ns delay
    // Write CS LOW
    PORTB = PORTB & B11110111;

    // 10ns delay
    // WR LOW (it is already!)
    // PORTB = PORTB & B11101111;

    // Uuh DATAA :d
    PORTD = data << 2;
    PORTB = data >> 6;
    
    // 100 ns delay
    

    // CS HIGH, data reading will happen now :DD:D
    PORTB = PORTB | B00001000;


    // Invert addresswrite
    if(addresswrite) {addresswrite = false;}
    else {addresswrite = true;}
    
  }

  

}
