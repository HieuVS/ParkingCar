#include <Servo.h>
#include <LiquidCrystal.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>

#define SS_PIN    10
#define RST_PIN   9

const int rs = 6, e = 7, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, e, d4, d5, d6, d7);

Servo servo;
MFRC522 rfid(SS_PIN, RST_PIN);

const int LIGHT_ANALOG_SENSOR_PIN1 = A1; // Arduino pin connected to light sensor's  pin
const int LIGHT_ANALOG_SENSOR_PIN2 = A2; // Arduino pin connected to light sensor's  pin
const int ANALOG_THRESHOLD = 400;
const int SERVO_POS = 8;
byte authorizedUID[4] = {0xA0, 0x9C, 0x36, 0x32}; //UID of RFID's tag
int angle = 0; // the current angle of servo motor

// 3 chan cam bien bai do xe
const uint8_t pos[] = {A3, A4, A5};
int sizePos = sizeof(pos);
int arrSaveDigitalValue[] = {0, 0, 0};
int numberParkCurrent = 3;


void setup() {
  lcd.begin(16, 2);
  lcd.print("Welcome!");
  lcd.setCursor(0, 1);
  lcd.print("So cho trong: 3");
  
  Serial.begin(9600);
  servo.attach(SERVO_POS);
  SPI.begin(); // init SPI bus
  rfid.PCD_Init(); // init MFRC522
  servo.write(angle); // rotate servo motor to 0°
}

void loop() {
//  int digitalValue = digitalRead(LIGHT_DIGITAL_SENSOR_PIN);
  int analogValue1 = analogRead(LIGHT_ANALOG_SENSOR_PIN1); // read the input on analog pin
  int analogValue2 = analogRead(LIGHT_ANALOG_SENSOR_PIN2); // read the input on analog pin

  bool openAnalogValue1 = analogValue1 >= ANALOG_THRESHOLD; //false
  bool openAnalogValue2 = analogValue2 >= ANALOG_THRESHOLD; //false
  
  lcd.setCursor(9, 0);
  if(openAnalogValue1 != openAnalogValue2){
    if (rfid.PICC_IsNewCardPresent()) { // new tag is available
      if (rfid.PICC_ReadCardSerial()) { // NUID has been readed
        MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
  
        if (rfid.uid.uidByte[0] == authorizedUID[0] &&
            rfid.uid.uidByte[1] == authorizedUID[1] &&
            rfid.uid.uidByte[2] == authorizedUID[2] &&
            rfid.uid.uidByte[3] == authorizedUID[3] ) {
          Serial.println("Authorized Tag");
  
          // control servo motor arccoding to the angle
          servo.write(100);
          lcd.print("Open !!");
          delay(5000);
        } else {
          Serial.print("Unauthorized Tag with UID:");
          for (int i = 0; i < rfid.uid.size; i++) {
            Serial.print(rfid.uid.uidByte[i] < 0x10 ? " 0" : " ");
            Serial.print(rfid.uid.uidByte[i], HEX);
          }
          Serial.println();
        }
  
        rfid.PICC_HaltA(); // halt PICC
        rfid.PCD_StopCrypto1(); // stop encryption on PCD
      }
    }
  } else {
    servo.write(0);
    Serial.println("CLOSE");
    lcd.print("Close !!");
  }

    // digital light sensor
  String showNumberPark = "So cho trong: " + String(numberParkCurrent);
  lcd.setCursor(0, 1);
  lcd.print(showNumberPark);

  // set arrSaveDigitalValue = [0 , 0, 0]
  for (int i = 0; i < sizePos; i++) {
    arrSaveDigitalValue[i] = 0;
  }

  //Serial.println("Hello");
  for (int i = 0; i < sizePos; i++) {
    int digitalValue = digitalRead(pos[i]);
    if (digitalValue == 1) arrSaveDigitalValue[i] = 1;
  }

  // dem so slot con lai
  int numberSlot = 0;
  for (int i = 0; i < sizePos; i++) {
    if (arrSaveDigitalValue[i] == 0) numberSlot++;
  }
  numberParkCurrent = numberSlot;
  
  Serial.println(analogValue1); 
  //delay(1000);
  Serial.println(analogValue2);
}
