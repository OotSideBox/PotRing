
//ASTUCE: pour forcer une relecture alors que le TAG est toujours présent, couper puis remettre l'alim par le 0V
// (ne marche pas avec le +, peut être a cause d'une alim indirecte via UART ?)

//cablage du module RFID GROVE (SeedStudio)
//Arduino=>RFID
//Rouge=>5V
//Noir=>13
//Blanc=>3
//Jaune=>2


#include <SoftwareSerial.h>
#define START 0x02
#define END 0x03
#define TIMEOUT 200

enum STATES {VOID, PRESENT, CHECK };

SoftwareSerial SoftSerial(2, 3);
String ReceivedCode = "";
String LastCode="";
int count=0;     
int State=VOID;
bool Flag_ID=false;

void setup()
{
  SoftSerial.begin(9600);              
  Serial.begin(9600);             
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  State=CHECK;
  Flag_ID=false;
}
 
void loop()
{
  static int Tempo=0;
  static int Counter=0;
  char RecievedChar;
  delay(10); 
  
  switch(State)
  {
    case CHECK:
              digitalWrite(LED_BUILTIN, LOW); //alimente le module RFID
              if(Tempo<TIMEOUT) Tempo++;
              if(Tempo==TIMEOUT-1) 
              {
                Serial.println("<VOID>");
                LastCode="";
              }
              if(Flag_ID) 
              {
                State=PRESENT;
                Flag_ID=false;
              }
    break;
    
    case PRESENT:
              Tempo++;
              if (Tempo> 10)
              {
                Tempo=0;
                digitalWrite(LED_BUILTIN, HIGH);
                delay(200);
                State=CHECK;
                //Serial.println("<CHECK>");
               }        
    break; 
  }
  
  if (SoftSerial.available())             
  {
      RecievedChar=SoftSerial.read();  
      if(isprint(RecievedChar)) ReceivedCode+=RecievedChar; 
      if(RecievedChar==START) Counter=0;
      else Counter++;
      if(RecievedChar==END) 
        {
          ReceivedCode.remove(10, 2); // Remove 2 characters starting at index=10
          if(ReceivedCode != LastCode) 
          {
            Serial.print("<TAG:");
            Serial.print(ReceivedCode);
            Serial.println(">");
          }
          LastCode=ReceivedCode;
          ReceivedCode=""; 
          Flag_ID=true; 
        }
  }
}

