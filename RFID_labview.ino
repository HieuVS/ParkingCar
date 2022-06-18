//Include All Peripheral Libraries Used By LINX
#include <SPI.h>
#include <Wire.h>
#include <EEPROM.h>
#include <Servo.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9

MFRC522 rfid(SS_PIN, RST_PIN);

//Include Device Specific Header From Sketch>>Import Library (In This Case LinxChipkitMax32.h)
//Also Include Desired LINX Listener From Sketch>>Import Library (In This Case LinxSerialListener.h)
#include <LinxArduinoUno.h>
#include <LinxSerialListener.h>
 
//Create A Pointer To The LINX Device Object We Instantiate In Setup()
LinxArduinoUno* LinxDevice;

int RFID();

//Initialize LINX Device And Listener
void setup()
{
  //Instantiate The LINX Device
  LinxDevice = new LinxArduinoUno();
  
  //The LINXT Listener Is Pre Instantiated, Call Start And Pass A Pointer To The LINX Device And The UART Channel To Listen On
  LinxSerialConnection.Start(LinxDevice, 0);
  LinxSerialConnection.AttachCustomCommand(0, RFID); 

  SPI.begin();
  rfid.PCD_Init();
}

void loop()
{
  //Listen For New Packets From LabVIEW
  LinxSerialConnection.CheckForCommands();
  
  //Your Code Here, But It will Slow Down The Connection With LabVIEW
}
int RFID(unsigned char numInputBytes, unsigned char* input, unsigned char* numResponseBytes, unsigned char* response)
{
   if (!rfid.PICC_IsNewCardPresent()) { // new tag is available
      return; 
   }
   if (!rfid.PICC_ReadCardSerial()) {
      return;    
   }
   for(byte i=0; i<rfid.uid.size; i++)
   {
      response[i] = (unsigned char)rfid.uid.uidByte[i];
   }
   
  *numResponseBytes = rfid.uid.size;
  rfid.PICC_HaltA(); // halt PICC

   
  return 0;
}
