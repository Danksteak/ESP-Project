#include <ESP8266WiFi.h>
#include <ThingESP.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

ThingESP8266 thing("Username", "ProjectName", "ProjectUserName");


int BUZZER = D0;
int ora=0;
int minuti=0;
bool interrotta=false;
bool richiesta = false;


char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};


unsigned long previousMillis = 0;
const long INTERVAL = 6000;  
WiFiUDP ntpUDP;
const long utcOffsetInSeconds = 3600;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);


void setup()
{
  Serial.begin(9600);
  thing.SetWiFi("WiFiUsername", "WiFiPassword");
  pinMode(BUZZER, OUTPUT); // Set buzzer - pin 9 as an output
  thing.initDevice();
  
}

//Funzione per slittare stringa

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

//Funzione per comunicare con Twilio

String HandleResponse(String query)
{
  
  if (richiesta==1){
    String orario=query;
    String S_ora = splitString(orario, ':', 0);
    String S_minuti = splitString(orario, ':', 1);
    richiesta=false;
    String feedback ="Sveglia Attivata alle " +query;
    ora = S_ora.toInt();
    minuti = S_minuti.toInt();    
    return feedback;
  }   
 
  else if (query == "imposta sveglia"){
    richiesta=true;
    interrotta=false;
    return "Certo, dimmi l'ora e i minuti";
  }

  else if (query == "interrompi"){
    interrotta=true;
    return "Sveglia interrotta";
  }

  else {
    return "*MESSAGGIO NON VALIDO*";
  }
  
}



void loop()
{
  //Per vedere nel Serial Monitor l'ora attuale

  timeClient.update(); //Aggiornamento ora

  Serial.print(daysOfTheWeek[timeClient.getDay()]);
  Serial.print(", ");
  Serial.print(timeClient.getHours());
  Serial.print(":");
  Serial.print(timeClient.getMinutes());
  Serial.print(":");
  Serial.println(timeClient.getSeconds());

  //Sveglia

  if(interrotta==0){               
    if((timeClient.getHours() == ora) && (timeClient.getMinutes() == minuti)){
      tone(BUZZER, 1000); 
      delay(800); 
      noTone(BUZZER);     
      delay(800);
    }
  }
 

thing.Handle();

}