
#include <ESP8266WiFi.h>
#include <ThingESP.h>

ThingESP8266 thing("Username", "ProjectName", "ProjectUserName");

int LED = D1;

unsigned long previousMillis = 0;
const long INTERVAL = 6000;  

void setup()
{
  Serial.begin(9600);
  pinMode(LED, OUTPUT);
  thing.SetWiFi("WiFiUsername", "WiFiPassword");
  thing.initDevice();
}


String HandleResponse(String query)
{
  if (query == "accendi led") {
    digitalWrite(LED, 1);
    return "Il LED è ora acceso";
  }

  else if (query == "spegni led") {
    digitalWrite(LED, 0);
    return "Il LED è ora spento";
  }

  else if (query == "stato led")
    return digitalRead(LED) ? "Il LED è acceso" : "Il LED è spento";
  else return "Richiesta invalida";

}


void loop()
{
  thing.Handle();

}