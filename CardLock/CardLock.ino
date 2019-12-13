#include <SPI.h>
#include <MFRC522.h>
#include <Adafruit_NeoPixel.h>

const byte UID[] = {0x95, 0x1A, 0x25, 0xD9};

#define NUMPIXELS 8
#define CS_RFID 10

String strUID = "";
MFRC522 rfid(10, 9);
MFRC522::MIFARE_Key key;
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, 8, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(9600);
  SPI.begin();
  rfid.PCD_Init();
  pinMode(2, OUTPUT);
  pinMode(CS_RFID, OUTPUT);
  pixels.begin();
  pixels.setBrightness(30);
  pixels.show();

  // open serial connection
    Serial.begin(9600);

    //Serial.println("CLEARDATA"); // clears sheet starting at row 2
    Serial.println("CLEARSHEET"); // clears sheet starting at row 1
    
  // define 5 columns named "Date", "Time", "Timer", "Counter" and "millis"
    Serial.println("LABEL,Date,Time,RFID UID, ACCESS,");
}

void loop() {
  digitalWrite(CS_RFID, HIGH);
  if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
  readUID();
    if (rfid.uid.uidByte[0] == UID[0] && 
        rfid.uid.uidByte[1] == UID[1] &&
        rfid.uid.uidByte[2] == UID[2] &&
        rfid.uid.uidByte[3] == UID[3])
    {
      Serial.println("Poprawny");
      diode(pixels.Color(0, 255, 0));
      Serial.println( (String) "DATA,DATE,TIME," + strUID + ",Granted");
    } else
    {
      Serial.println("Niepoprawny");
      diode(pixels.Color(255, 0, 0));
      Serial.println( (String) "DATA,DATE,TIME," + strUID + ",Denied");
    }
    rfid.PICC_HaltA();
    rfid.PCD_StopCrypto1();
    strUID = "";
  }
}

void diode(uint32_t c) {
  for(int i = 0; i < NUMPIXELS; i++) {
      pixels.setPixelColor(i, c);
      pixels.show();
  }
  delay(1000);
  pixels.clear();
  pixels.show();
}
void readUID() {
  for(byte i = 0; i < rfid.uid.size; i++) {
  strUID.concat(String(rfid.uid.uidByte[i]));
  }
}
