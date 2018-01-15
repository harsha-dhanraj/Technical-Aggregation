#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"//is generic for api of adafruit MQTT
#include "Adafruit_MQTT_Client.h"//Specific to board

//defining values for random data
float temp;
float cel;
float soilMoisture;
float humidity;

//Use the below for random values
int randomTemp;
int randomHumidity;
int randomSoilMoisture;

//For actual sensors, the below pins would be an example
//int humidity_sensor= A0;  //Pin 3
//int soilMoisture_sensor= A1; //Pin 2
//int temp_sensor = A2; //Pin 1

WiFiClient client1; //WiFi connection

Adafruit_MQTT_Client mqtt(&client1,"io.adafruit.com",1883,"vedantkamlaskar","87de48c2a37e42408af65cff18617c5f");

Adafruit_MQTT_Publish mqtt_pub_humidty = Adafruit_MQTT_Publish(&mqtt,"vedantkamlaskar/feeds/greencity.humidity");
Adafruit_MQTT_Publish mqtt_pub_soilMoisture = Adafruit_MQTT_Publish(&mqtt,"vedantkamlaskar/feeds/greencity.soilmoisture");
Adafruit_MQTT_Publish mqtt_pub_temp = Adafruit_MQTT_Publish(&mqtt,"vedantkamlaskar/feeds/greencity.temperature");

Adafruit_MQTT_Subscribe waterTrees = Adafruit_MQTT_Subscribe(&mqtt,"kgm12345/feeds/waterplants");
Adafruit_MQTT_Subscribe waterFlowers = Adafruit_MQTT_Subscribe(&mqtt,"kgm12345/feeds/waterplants");
Adafruit_MQTT_Subscribe waterLawn = Adafruit_MQTT_Subscribe(&mqtt,"kgm12345/feeds/waterplants");


//We have used multiple feeds as per the requirement.
//Seperate feeds to water the plants as per the users choice.
//The user can water the particular set of plants as per the area they are located in
//Consider that one particular type of plantation is restricted to one area.

//Data of current soil Moisture, Temperature and humidty is being sent to the user.
//For now, single sensors of each type have been stated above, the application can be expanded
//as per area requirement.



void setup()
{
  //pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  delay(10);
  Serial.print("SSID : ");
  WiFi.begin("pass","123456789");      //enter your wifi details
  while(WiFi.status() != WL_CONNECTED)  //on successful connection WL_CONNECTED is status return value
  {
    delay(500);     //if not connected we stay in this loop with delay
            // or else, eventually sw watchdog reset may occur
    Serial.print(" :)");
  }
  Serial.println("Wifi connected.....");

  mqtt.subscribe(&waterTrees);
  mqtt.subscribe(&waterFlowers);
  mqtt.subscribe(&waterLawn);        
  /* if subscribing to more feeds add lines accordingly

    eg. mqtt.subscribe(&sub1);
        mqtt.subscribe(&sub2); ...so on */  
}

void loop()
{
  int i;

  while((i=mqtt.connect()) != 0)
  {
    Serial.println("retrying..");
    mqtt.disconnect();
    delay(4000);
  }
  Serial.println("Connected to MQTT broker..");

  Adafruit_MQTT_Subscribe *s;
  
  //temp=analogRead(tempPin);                 //Current Temperature being read in analog
                                              //NodeMCU has an inbuilt 10-bit ADC with a scale of 0-1023
  //cel=temp * 0.48828125;                    //Conversion from analog value to Celcius
  //soilMoisture=analogRead(soilMoisturePin); //Data being read from a soil moisture sensor
  //humidity= analogRead(humidityPin);        //Data read from humidity sensor
  
  randomTemp= random(15,40);                  //Temp between 15 degrees to 40 degrees
  mqtt_pub_temp.publish(randomTemp);          //Data being published to the feed of Temperature
  
  randomHumidity= random(0,100);              //Humidty as % values
  mqtt_pub_humidty.publish(randomHumidity);   //Data being published to the feed of Humidity
  
  randomSoilMoisture=random(0,100);                   //Soil Moisture as % values
  mqtt_pub_soilMoisture.publish(randomSoilMoisture);  //Data being published to the feed of Soil Moisture
  

  while((s = mqtt.readSubscription(500)))
  {
    //To send data of current tempature, soil moisture and humidity
    //to the feed at an interval
    
    if(s == &waterTrees)
    {
      if(strcmp((char *)waterTrees.lastread,"ON")==0)
      {
        //pub.publish("Watering Trees");
        digitalWrite(LED_BUILTIN, HIGH);  // try it out!
        delay(10000);  //Water the trees for 10 mins
        Serial.println("Watering Trees");
      }
  
      else if(strcmp((char *)waterTrees.lastread,"OFF")==0)
      {
        //pub.publish("Not watering trees");
        digitalWrite(LED_BUILTIN,LOW);  // try it out!
        Serial.println("Not watering trees");
      }       
    }
    else if(s== &waterFlowers){
      if(strcmp((char *)waterFlowers.lastread,"ON")==0)
      {
        //pub.publish("Watering Flowers");
        
        digitalWrite(LED_BUILTIN, HIGH);  // try it out!
        delay(5000);  //Water the flowers for 5 mins
        Serial.println("Watering Flowers");
      }
  
      else if(strcmp((char *)waterFlowers.lastread,"OFF")==0)
      {
        //pub.publish("Not Watering Flowers");
        digitalWrite(LED_BUILTIN,LOW);  // try it out!
        Serial.println("Not Watering Flowers");
      }
      }
    else if(s== &waterLawn){
      if(strcmp((char *)waterLawn.lastread,"ON")==0)
      {
        //pub.publish("Watering Lawn");
        digitalWrite(LED_BUILTIN, HIGH);  // try it out!
        delay(15000);  //Water the Lawn for 15 mins
        Serial.println("Watering Lawn");
      }
  
      else if(strcmp((char *)waterLawn.lastread,"OFF")==0)
      {
       // pub.publish("Not watering lawn");
        digitalWrite(LED_BUILTIN,LOW);  // try it out!
        Serial.println("Not watering lawn");
      }
      }
  }
}





