/*
Sketch: Bluetooth AT Command 

Function: This sketch is used to test SIM800's Bluetooth function,you can connect
your mobile phone with SIM800 module in SPP profile. then you can send AT command
to control SIM800' Bluetooth.
*********************************************************************************
note: the following pins has been used and should not be used for other purposes.
  pin 8   // tx pin
  pin 7   // rx pin
  pin 9   // power key pin
  pin 12  // power status pin
*********************************************************************************
created on 2013/12/5, version: 0.1
by lawliet.zou(lawliet.zou@gmail.com)
*/

#include <bluetooth.h>
#include <SoftwareSerial.h>

#define DEFAULT_TIMEOUT 5
#define BT_BUF_LEN      64 
//32
BlueTooth bluetooth;

char bluetoothBuffer[BT_BUF_LEN];
int start = 0;
int ready = 0;

void setup() {
  Serial.begin(9600);
  while(!Serial);
  Serial.println("Bluetooth AT Command Test...");
  bluetooth.preInit();
  delay(3*1000);
  while(0 != bluetooth.powerOn()){ //bluetooth PowerOn
    Serial.println("bluetooth power on failed, try again...");
    delay(2000);
  }
  Serial.println("Bluetooth is on."); 
  
  // Check if bluetooth has connectted    
  if(0 == bluetooth.sendCmdAndWaitForResp("AT+BTSTATUS?\r\n", "AVRCP", 2) ){
    Serial.println("Bluetooth has connectted...");    
    ready = 1;
  }else{
    ready = 0;
    Serial.println("No bluetooth connectted, please open you phone's bluetooth and try to connect \"SIM800\"...");                 
  }  
}

void loop() 
{   
  if(!ready)
  {
    while(!bluetooth.serialSIM800.available());
    bluetooth.readBuffer(bluetoothBuffer,BT_BUF_LEN,2); //DEFAULT_TIMEOUT
    Serial.println(bluetoothBuffer);
    
    //bluetooth.cleanBuffer(bluetoothBuffer,64);
    //bluetooth.readBuffer(bluetoothBuffer,BT_BUF_LEN,DEFAULT_TIMEOUT);
        
    if(NULL != strstr(bluetoothBuffer,"+BTPAIRING:")){  //Accept pairing
      bluetooth.cleanBuffer(bluetoothBuffer,64);
      Serial.println("Bluetooth accept pairing...");
      bluetooth.acceptPairing();      
      Serial.println("Bluetooth paired...");
    }    
    else if(NULL != strstr(bluetoothBuffer,"+BTCONNECTING:")){  //Accept connection
      bluetooth.cleanBuffer(bluetoothBuffer,64);      
      Serial.println("Bluetooth connecting...");
      bluetooth.acceptConnect();  
      ready = 1;
      Serial.println("Bluetooth successfully connected...");                    
    }            
  }
  
  while(ready)
  {
    if(0 != bluetooth.sendCmdAndWaitForResp("AT+BTSTATUS?\r\n", "AVRCP", 2) ){
      Serial.println("Bluetooth didn't connected...");                    
      ready = 0;
    }
    delay(1000);
  }   
}
