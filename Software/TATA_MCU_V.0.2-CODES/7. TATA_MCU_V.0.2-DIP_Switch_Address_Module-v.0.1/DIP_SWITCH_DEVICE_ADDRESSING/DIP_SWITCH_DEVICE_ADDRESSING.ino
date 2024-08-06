#define uint16_t unsigned int
#define default_address 0x00;

uint16_t address = default_address;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  DDRD &= ~(0xE0);
  DDRB &= ~(0x01);

  address = ((PIND >> PD7) & 0x01) << 2 |
            ((PIND >> PD6) & 0x01) << 1 |
            ((PIND >> PD5) & 0x01) << 0 |
            (PINB & 0x01) << 3;

}

void loop() {

  Serial.print("Device Address : "); Serial.println(address, HEX);
  delay(2000);

}
