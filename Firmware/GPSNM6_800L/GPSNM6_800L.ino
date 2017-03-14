
#define SLEEP_GPRS  2
#define SLEEP_GPS   3
#define ACTUATOR    4
#define SENSOR      5
#define BATTERY     A0

String Host = "http://postingsite.com/data.php?id=101010";

#include <SoftwareSerial.h>
SoftwareSerial GPS(8, 9);

int SleepInterval_MCU = 600;

void setup() {
  // Start GPRS Serial
  Serial.begin(9600);
  // Start GPS Serial
  GPS.begin(9600);
  // Setup important pins
  pinMode(SENSOR, INPUT);
  pinMode(SLEEP_GPRS, OUTPUT);
  pinMode(SLEEP_GPS, OUTPUT);
  pinMode(ACTUATOR, OUTPUT);
  // Put all to zero
  digitalWrite(2, LOW);
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);
  // Set Reference
  analogReference(EXTERNAL);
}

void loop() {
  // put your main code here, to run repeatedly:
  
}

/*
  Boot GPRS subroutine
*/

// For sending new requests
unsigned long timeout = 0;
String GPRSmsg = "";
void bootGPRS()
{
  // AT -> OK
  _newAT:
  GPRSmsg = "";
  Serial.println("AT");
  timeout = millis() + 3000;
  while(GPRSmsg.indexOf("OK") != -1)
  {
    // Catch new data
    if(Serial.available() > 0)
      GPRSmsg += Serial.readString(); 
    // Waiting for OK 
    if(timeout < millis())
      goto newAT;
  }
  // AT+CPIN? -> CPIN: READY
  _newAT_CPIN:
  GPRSmsg = "";
  Serial.println("AT+CPIN?");
  timeout = millis() + 5000;
  while(GPRSmsg.indexOf("READY") != -1)
  {
    // Catch new data
    if(Serial.available() > 0)
      GPRSmsg += Serial.readString(); 
    // Waiting for OK 
    if(timeout < millis())
      goto _newAT_CPIN;
  }
  // AT+CREG? -> CREG: 0,1
  _newAT_CREG:
  GPRSmsg = "";
  Serial.println("AT+CREG?");
  timeout = millis() + 5000;
  while(GPRSmsg.indexOf("0,1") != -1)
  {
    // Catch new data
    if(Serial.available() > 0)
      GPRSmsg += Serial.readString(); 
    // Waiting for OK 
    if(timeout < millis())
      goto _newAT_CREG;
  }
  // AT+CGATT? -> CGATT: 1
  _newAT_CGATT:
  GPRSmsg = "";
  Serial.println("AT+CGATT?");
  timeout = millis() + 10000;
  while(GPRSmsg.indexOf("1") != -1)
  {
    // Catch new data
    if(Serial.available() > 0)
      GPRSmsg += Serial.readString(); 
    // Waiting for OK 
    if(timeout < millis())
      goto _newAT_CGATT;
  }
  // AT+SAPBR=3,1,"Contype","GPRS" -> OK
  _newAT_SAPBR3_1_Con:
  GPRSmsg = "";
  Serial.print("AT+SAPBR=3,1,");
  Serial.print(char(34)); // Print "
  Serial.print("Contype");
  Serial.print(char(34)); // Print "
  Serial.print(",");
  Serial.print(char(34)); // Print "
  Serial.print("GPRS");
  Serial.println(char(34)); // Print "
  timeout = millis() + 5000;
  while(GPRSmsg.indexOf("OK") != -1)
  {
    // Catch new data
    if(Serial.available() > 0)
      GPRSmsg += Serial.readString(); 
    // Waiting for OK 
    if(timeout < millis())
      goto _newAT_SAPBR3_1_Con;
  }
  // AT+SAPBR=3,1,"APN","kolbi3g" -> OK
  _newAT_SAPBR3_1_APN:
  GPRSmsg = "";
  Serial.print("AT+SAPBR=3,1,");
  Serial.print(char(34)); // Print "
  Serial.print("APN");
  Serial.print(char(34)); // Print "
  Serial.print(",");
  Serial.print(char(34)); // Print "
  Serial.print("kolbi3g");
  Serial.println(char(34)); // Print "
  timeout = millis() + 5000;
  while(GPRSmsg.indexOf("OK") != -1)
  {
    // Catch new data
    if(Serial.available() > 0)
      GPRSmsg += Serial.readString(); 
    // Waiting for OK 
    if(timeout < millis())
      goto _newAT_SAPBR3_1_APN;
  }
  // AT+SAPBR=1,1 -> OK
  _newAT_SAPBR1_1:
  GPRSmsg = "";
  Serial.println("AT+SAPBR=1,1");
  timeout = millis() + 10000;
  while(GPRSmsg.indexOf("OK") != -1)
  {
    // Catch new data
    if(Serial.available() > 0)
      GPRSmsg += Serial.readString(); 
    // Waiting for OK 
    if(timeout < millis())
      goto _newAT_SAPBR1_1;
    // If error
    if(GPRSmsg.indexOf("ERROR") > 0)
      errorHandling(1);
  }
  // AT+SAPBR=2,1-> OK
  _newAT_SAPBR2_1:
  GPRSmsg = "";
  Serial.println("AT+SAPBR=2,1");
  timeout = millis() + 5000;
  while(GPRSmsg.indexOf("OK") != -1)
  {
    // Catch new data
    if(Serial.available() > 0)
      GPRSmsg += Serial.readString(); 
    // Waiting for OK 
    if(timeout < millis())
      goto _newAT_SAPBR2_1;
  }
  // AT+HTTPINIT-> OK
  _newAT_HTTPINIT:
  GPRSmsg = "";
  Serial.println("AT+HTTPINIT");
  timeout = millis() + 5000;
  while(GPRSmsg.indexOf("OK") != -1)
  {
    // Catch new data
    if(Serial.available() > 0)
      GPRSmsg += Serial.readString(); 
    // Waiting for OK 
    if(timeout < millis())
      goto _newAT_HTTPINIT;
    // If error
    if(GPRSmsg.indexOf("ERROR") > 0)
      errorHandling(1);
  }
  // AT+HTTPPARA="CID",1-> OK
  _newAT_HTTPPARA_CID:
  GPRSmsg = "";
  Serial.print("AT+HTTPPARA=");
  Serial.print(char(34));
  Serial.print("CID");
  Serial.print(char(34));
  Serial.println(",1");
  timeout = millis() + 5000;
  while(GPRSmsg.indexOf("OK") != -1)
  {
    // Catch new data
    if(Serial.available() > 0)
      GPRSmsg += Serial.readString(); 
    // Waiting for OK 
    if(timeout < millis())
      goto _newAT_HTTPPARA_CID;
  }
  // AT+HTTPPARA="URL","SITE"-> OK
  _newAT_HTTPPARA_URL:
  GPRSmsg = "";
  Serial.print("AT+HTTPPARA=");
  Serial.print(char(34));
  Serial.print("URL");
  Serial.print(char(34));
  Serial.print(HOST);
  Serial.print("&");
  Serial.print("message=BOOTOK");
  Serial.println(char(34));
  timeout = millis() + 5000;
  while(GPRSmsg.indexOf("OK") != -1)
  {
    // Catch new data
    if(Serial.available() > 0)
      GPRSmsg += Serial.readString(); 
    // Waiting for OK 
    if(timeout < millis())
      goto _newAT_HTTPPARA_URL;
  }
  // AT+HTTPACTION-> OK
  _newAT_HTTPACTION:
  GPRSmsg = "";
  Serial.println("AT+HTTPACTION=0");
  timeout = millis() + 10000;
  while(GPRSmsg.indexOf("0,200,") > 0)
  {
    // Catch new data
    if(Serial.available() > 0)
      GPRSmsg += Serial.readString(); 
    // Waiting for OK 
    if(timeout < millis())
      goto _newAT_HTTPACTION;
    // If error
    if(GPRSmsg.indexOf("0,603,") > 0 || GPRSmsg.indexOf("0,601,") > 0)
      errorHandling(2);
  }
  // AT+HTTPREAD-> OK
  _newAT_HTTPREAD:
  GPRSmsg = "";
  Serial.println("AT+HTTPREAD");
  timeout = millis() + 5000;
  while(GPRSmsg.indexOf("OK") > 0)
  {
    // Catch new data
    if(Serial.available() > 0)
      GPRSmsg += Serial.readString(); 
    // Waiting for OK 
    if(timeout < millis())
      goto _newAT_HTTPACTION;
    // Get actuator
    if(GPRSmsg.indexOf("ACTUATOR=ON;")) > 0)
      digitalWrite(ACTUATOR, 1);
    else if(GPRSmsg.indexOf("ACTUATOR=OFF;")) > 0)
      digitalWrite(ACTUATOR, 0);
    // Get Sleep interval
    if(GPRSmsg.indexOf("SLEEP=")) > 0)
    {
      String SleepInterval = GPRSmsg.substring(GPRSmsg.indexOf("SLEEP="));
      SleepInterval = SleepInterval.substring(0, SleepInterval.indexOf(";"));
      SleepInterval_MCU = SleepInterval.toInt();
    }
  }
  
}

void errorHandling(int args)
{
  // Error during boot 1
  // Error during posting boot 2 
}
