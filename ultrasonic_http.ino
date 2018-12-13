/*
  
 * Description:  Distance measurement using ultrasonic HC-SR04 and ESP32 connected to AskSensors
 *  Author: https://asksensors.com, 2018
 *  github: https://github.com/asksensors
 */
 
#include <WiFi.h>
#include <WiFiMulti.h>
#include <HTTPClient.h>

// utlrasonic pinout
#define ULTRASONIC_TRIG_PIN     5   // pin TRIG 
#define ULTRASONIC_ECHO_PIN     4 // pin ECHO 


WiFiMulti WiFiMulti;
HTTPClient ask;
// TODO: user config
const char* ssid     = "............."; //Wifi SSID
const char* password = "............."; //Wifi Password
String apiKeyIn = "............."; // API Key

const char* host = "asksensors.com";  // host
const int httpPort = 80;      // port
  
void setup(){
  
  // open serial
  Serial.begin(9600);
  Serial.println("*****************************************************");
  Serial.println("********** Program Start : Connect ESP32 to AskSensors.");
  Serial.println("Wait for WiFi... ");

  // connecting to the WiFi network
  WiFiMulti.addAP(ssid, password);
  while (WiFiMulti.run() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  // connected
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // ultraonic setup 
  pinMode(ULTRASONIC_TRIG_PIN, OUTPUT);
  pinMode(ULTRASONIC_ECHO_PIN, INPUT);
  
}


void loop(){

  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  // measure distance 
  long duration, distance;
  digitalWrite(ULTRASONIC_TRIG_PIN, LOW);  
  delayMicroseconds(2); 
  
  digitalWrite(ULTRASONIC_TRIG_PIN, HIGH);
  delayMicroseconds(10); 
  
  digitalWrite(ULTRASONIC_TRIG_PIN, LOW);
  duration = pulseIn(ULTRASONIC_ECHO_PIN, HIGH);
  distance = (duration/2) / 29.1;
  Serial.print("********** Ultrasonic Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }else {

    // Create a URL for the request
    String url = "https://asksensors.com/api.asksensors/write/";
    url += apiKeyIn;
    url += "?module1=";
    url += distance;
    
    Serial.print("********** requesting URL: ");
    Serial.println(url);
   
    ask.begin(url); //Specify the URL
  
    //Check for the returning code
    int httpCode = ask.GET();          
 
    if (httpCode > 0) { 
 
        String payload = ask.getString();
        Serial.println(httpCode);
        Serial.println(payload);
      } else {
      Serial.println("Error on HTTP request");
    }
 
    ask.end(); //End 
    Serial.println("********** End ");
    Serial.println("*****************************************************");

  }

  client.stop();  // stop client
  
  delay(20000);    // delay
}
