#include <ESP8266WiFi.h>
#include <ThingESP.h>
#include "pitches.h"
#include <NTPClient.h>
#include <WiFiUdp.h>

ThingESP8266 thing("Username", "ProjectName", "ProjectUserName");


 
int buzzerPin =D0;
 
int melodia[] = {
  NOTE_C4, NOTE_F4, NOTE_F4, NOTE_G4, NOTE_A4, NOTE_F4, NOTE_C5,
  NOTE_AS4, NOTE_A4, NOTE_A4, NOTE_AS4, NOTE_C5, NOTE_AS4, NOTE_A4,
  NOTE_AS4, NOTE_C5, NOTE_G4, NOTE_F4, NOTE_G4, NOTE_A4, NOTE_G4
};
 
int durataNote[] = {
  4, 4, 8, 8, 4, 4, 3, 8, 3, 8, 4, 8, 8, 8, 8, 4, 8, 8, 8, 8, 3
};
 
int numeroNote = 21;

int richiesta = 0;
int ora=0;
int minuti=0;
int interrotta=0;

const long utcOffsetInSeconds = 3600;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

unsigned long previousMillis = 0;
const long INTERVAL = 6000;  

WiFiUDP ntpUDP;

NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);


void setup()
{
  Serial.begin(9600);
  thing.SetWiFi("WiFiUsername", "WiFiPassword");
  pinMode(buzzerPin, OUTPUT);
  thing.initDevice();
  
}



String splitString(String str, char sep, int index)
{
 int found = 0;
 int strIdx[] = { 0, -1 };
 int maxIdx = str.length() - 1;

 for (int i = 0; i <= maxIdx && found <= index; i++)
 {
    if (str.charAt(i) == sep || i == maxIdx)
    {
      found++;
      strIdx[0] = strIdx[1] + 1;
      strIdx[1] = (i == maxIdx) ? i+1 : i;
    }
 }
 return found > index ? str.substring(strIdx[0], strIdx[1]) : "";
}

String HandleResponse(String query)
{
  
  if (richiesta==1){
    String orario=query;
    String S_ora = splitString(orario, ':', 0);
    String S_minuti = splitString(orario, ':', 1);
    richiesta=0;
    String accertato ="Sveglia Attivata alle " +query;
    ora = S_ora.toInt();
    minuti = S_minuti.toInt();    
    return accertato;
  }   
 
  else if (query == "imposta sveglia"){
    richiesta=1;
    interrotta=0;
    return "Certo, dimmi l'ora e i minuti";
  }

  else if (query == "interrompi"){
    interrotta=1;
    return "Sveglia interrotta";
  }

  else {
    return "*MESSAGGIO NON VALIDO*";
  }
  
}



void loop()
{
  //SERIAL PRINT

  timeClient.update();

  Serial.print(daysOfTheWeek[timeClient.getDay()]);
  Serial.print(", ");
  Serial.print(timeClient.getHours());
  Serial.print(":");
  Serial.print(timeClient.getMinutes());
  Serial.print(":");
  Serial.println(timeClient.getSeconds());


  if(interrotta==0){
    if((timeClient.getHours() == ora) && (timeClient.getMinutes() == minuti)){
      for(int i = 0; i < numeroNote; i++){
        int durata = 1500 / durataNote[i];
        tone(buzzerPin, melodia[i], durata);
        thing.Handle();
        if(interrotta==1){
          break;
        }
        delay(durata * 1.3);
      }
    }
  }
 

thing.Handle();

}


 
