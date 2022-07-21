#define BLYNK_TEMPLATE_ID "TMPLWU38IxTR"
#define BLYNK_DEVICE_NAME "SMART IRRIGATION"
#define BLYNK_AUTH_TOKEN "NAC33OiJBqctvbfNcmlNgXM6arTINN_9"

#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).

char auth[] = "NAC33OiJBqctvbfNcmlNgXM6arTINN_9";

// Your WiFi credentials.
// Set password to "" for open networks.
 
char ssid[] = "dlink";
char pass[] = "";
#define DHTPIN 21          // What digital pin we're connected to

// Uncomment whatever type you're using!

#define DHTTYPE DHT11     // DHT 11
#define smoke 32
int MoistData = 0;
int MoistPercentage = 0;
int LdrData = 0;
int PirData = 0;
int ADCsmoke_data = 0;
float Vout = 0;
int smokeData = 0;

//#define DHTTYPE DHT22   // DHT 22, AM2302, AM2321
//#define DHTTYPE DHT21   // DHT 21, AM2301

DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;

// This function sends Arduino's up time every second to Virtual Pin (5).
// In the app, Widget's reading frequency should be set to PUSH. This means
// that you define how often to send data to Blynk App.


void sendSensor()
{

  //FOR SMOKE SENSOR
   smokeData = digitalRead(smoke);
   ADCsmoke_data= analogRead(smoke);
   Serial.print("Analog Smoke Data = ");
   Serial.println(ADCsmoke_data);
//   Vout= ((3.3*ADCsmoke_data)/4096);
//   Serial.print("VOLTAGE OUT = ");
//   Serial.println(Vout);
   delay(200);
   if(ADCsmoke_data > 1000)
   {
    digitalWrite(4,0);
   }
   else if(ADCsmoke_data < 1000 )
   {
    digitalWrite(4,1);
   }

  //FOR PIR SENSOR
  if(digitalRead(22)==HIGH){
    digitalWrite(23,1);
  }
  else if(digitalRead(22)==LOW){
    digitalWrite(23,0);
  }
 

  //FOR LDR SENSOR
  LdrData = analogRead(35);
    if (LdrData >= 2500) {
      digitalWrite(0,LOW); 

    } else if (LdrData <= 2000) {
      digitalWrite(0,HIGH); 
    }
  

  //FOR SOIL MOISTURE SENSOR
  MoistData = analogRead(34);
  if ( isnan(MoistData) ){
    Serial.println("Failed to read from Hygrometer Soil Moisture sensor!");
    return;
  }
  Serial.print("Analog Data Soil Moisture = ");
  Serial.println(MoistData);
  MoistPercentage = ( 100 - ( (MoistData/4095.00) * 100 ) );
  Blynk.virtualWrite(V0, MoistPercentage);

  if(MoistData > 3000){
    Blynk.logEvent("less_water","Moisture content is low, turn on the pump");
  }
  else if(MoistData < 2800){
    Blynk.logEvent("more_water","Moisture content is high, turn off the pump");
  }

  
  //FOR DHT11 SENSOR
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
    }

  // You can send any value at any time.

  // Please don't send more that 10 values per second.
  Serial.print("HUMIDITY= ");
  Serial.print(h);
  Serial.print("% ");
  Serial.print("TEMP= ");
  Serial.print(t);
  Serial.println("° ");
  
  Blynk.virtualWrite(V2, h);
  Blynk.virtualWrite(V1, t);
}
  
void setup()
{
// Debug console
Serial.begin(9600);
Blynk.begin(auth, ssid, pass);

// You can also specify server:
//Blynk.begin(auth, ssid, pass, "blynk-cloud.com", 80);
//Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);

dht.begin();
pinMode(22,INPUT);
pinMode(23,OUTPUT);

// Setup a function to be called every second

timer.setInterval(1000L, sendSensor);
}

 
 
void loop()
{
 
  Blynk.run();

  timer.run();

}
