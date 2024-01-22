//15VEXCVG34M4TSBC
#ifdef __cplusplus
extern "C" {
#endif
uint8_t temprature_sens_read();
#ifdef __cplusplus
}
#endif

uint8_t temprature_sens_read();
// ------header files----
#include <WiFi.h>
#include "DHT.h"
#include "ThingSpeak.h"

//-----network credentials
char* ssid = "GalaxyM31a"; //enter SSID
char* passphrase = "12345678"; // enter the password
WiFiServer server(80);
WiFiClient client;


//-----ThingSpeak channel details
unsigned long myChannelNumber = 2;
const char * myWriteAPIKey = "15VEXCVG34M4TSBC";

//----- Timer variables
unsigned long lastTime = 0;
unsigned long timerDelay = 100;

//----DHT declarations
#define DHTPIN 33 // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11 // DHT 11
#define raD 34
#define raA 35


// Initializing the DHT11 sensor.
DHT dht(DHTPIN, DHTTYPE);

void setup(){
  Serial.begin(115200); //Initialize serial
  pinMode(raD, INPUT);
  pinMode(raA, INPUT);
  delay(1000);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, passphrase);
  while (WiFi.status() != WL_CONNECTED) {
  delay(500);
  Serial.print(".");
}

// Print local IP address and start web server
Serial.println("");
Serial.println("WiFi connected.");
Serial.println("IP address: ");
Serial.println(WiFi.localIP());
server.begin();

//----Initialize dht11
dht.begin();
ThingSpeak.begin(client); // Initialize ThingSpeak
}

void loop(){
  if ((millis() - lastTime) > timerDelay)
{
delay(500);
// Reading temperature or humidity takes about 250 milliseconds!
float h = dht.readHumidity();

// Read temperature as Celsius (the default)
float t = dht.readTemperature();
float f = dht.readTemperature(true);

//Read rain sensor data
int rd = digitalRead(raD);
int ra = analogRead(raA);

if (isnan(h) || isnan(t) || isnan(f)) {
Serial.println(F("Failed to read from DHT sensor!"));
return;
}

Serial.print("Temperature: ");
Serial.print(t);
Serial.println("ºC");

Serial.print("Humidity: ");
Serial.println(h);

    // RS
    Serial.print("Digital Reading: ");
    Serial.println(rd); 
    if(digitalRead(rd) == 1) 
      {
        Serial.println("Digital value: Wet"); 
      }
    else
      {
        Serial.println("Digital value: Dry"); 
      }
    Serial.print("Analog Reading: ");
    Serial.println(ra); 
    if (ra >= 4090) 
      {
        Serial.println("Analog value: Dry"); 
      }
    if (ra <= 2900)
      {
        Serial.println("Analog value: Wet"); 
      }
    
    Serial.println("");


ThingSpeak.setField(1, h);
ThingSpeak.setField(2, t);
ThingSpeak.setField(3, rd);
ThingSpeak.setField(4, ra);

// Write to ThingSpeak. There are up to 8 fields in a channel, allowing you to store up to 8 different
// pieces of information in a channel. Here, we write to field 1.
int x = ThingSpeak.writeFields(myChannelNumber,myWriteAPIKey);
if(x == 200){
Serial.println("Channel update successful.");
}
else{
//Serial.println("Problem updating channel. HTTP error code " + String(x));
Serial.println(". . .");
}
lastTime = millis();
  }
}
