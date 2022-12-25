#include "max1415.h"

int ChipSelect;
int DRDY;

max1415::max1415(int CS, int Select)
{
  ChipSelect = CS;
  DRDY = Select;

  ADCSerialInt();
  ChipConfig();
}

void max1415::ADCSerialInt()
{
  digitalWrite(ChipSelect, LOW); // Enable ADC SPI
  
  SPI.transfer(0xFF);
  SPI.transfer(0xFF);  
  SPI.transfer(0xFF); 
  SPI.transfer(0xFF);

  digitalWrite(ChipSelect,HIGH);
}

void max1415::ChipConfig()
{
    //series of commandbit    
  digitalWrite(ChipSelect,LOW); // Enable ADC SPI
  
  //Write OP
  SPI.transfer(0x20);//command for comm reg to select ch1 and write to clock register 
  delay(100);
  SPI.transfer(0xA7);//command for clock reg to set 2,4576Mhz                                  
  //End Write OP
  delay(100);
  //Write OP
  SPI.transfer(0x10);//command for comm reg to write setup register 
  delay(100);
  SPI.transfer(0x44);//command for setup reg to self calibration,unipolar,unbuffered,     
  //End Write OP

  digitalWrite(ChipSelect,HIGH); // Disable ADC SPI
}

unsigned int max1415::GetReading()
{
  unsigned int uiData;
  byte highByte;
  byte lowByte;
  
  digitalWrite(ChipSelect,LOW); // Enable ADC SPI
  
  // READ Data OPERATION
  SPI.transfer(0x38);//command for the comm to read data register for channel 1 (dec  56)
  //read 16bit of data ADC
  highByte = SPI.transfer(0x00);
  lowByte = SPI.transfer(0x00);
  // End Read Data
  
  digitalWrite(ChipSelect,HIGH); // Disable ADC SPI

  
  uiData = highByte;
  uiData <<= 8;
  uiData |= lowByte;
  
  return uiData;
}

void max1415::WaitForData()
{
  char DataNotReady = 1;
  char value;

  while(DataNotReady) // wait for end of conversion 
  {
      // Read OP
      value = digitalRead(DRDY); // Read comm register
       if (value == LOW)
         DataNotReady = 0;
       else
         DataNotReady = 1;
      // End Read OP
      //Serial.println("NOT READY");
  }
}
