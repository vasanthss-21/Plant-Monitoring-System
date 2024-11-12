#define BLYNK_TEMPLATE_ID "TMPL3fmvyX1pt"
#define BLYNK_TEMPLATE_NAME "SMART IRRIGATION SYSTEM"
#define BLYNK_AUTH_TOKEN "Oa8PgH8zcP1qev-sNA2wctTdabCLY0fX"

#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h> 
#include <BlynkSimpleEsp8266.h>

#include <DHT.h>

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "VS"; 
char pass[] = "vasa.21/06";  

BlynkTimer timer;

#define DHTPIN 4
#define DHTTYPE DHT11  
DHT dht(DHTPIN, DHTTYPE);

const int pumpPin = D1; 
int moistureThreshold = 30; 

void setup() {   
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
  dht.begin();
  pinMode(pumpPin, OUTPUT);
  timer.setInterval(250, sendSensor);
}

void loop() {
  Blynk.run();
  timer.run();
}

void sendSensor() {
  int value = analogRead(A0);
  value = map(value, 400, 1023, 100, 0);
  float h = dht.readHumidity();
  float t = dht.readTemperature(); 

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  
  Blynk.virtualWrite(V0, value);
  Blynk.virtualWrite(V1, t);
  Blynk.virtualWrite(V2, h);
  Serial.print("Soil Moisture : ");
  Serial.print(value);
  
  Serial.print("    Temperature : ");
  Serial.print(t);
  Serial.print("    Humidity : ");
  Serial.println(h);
  
  for (int i = 0; i <= moistureThreshold; i++) {
    if (value < i) {
      digitalWrite(pumpPin, LOW); 
      Blynk.virtualWrite(V3, "Watering");
    }
  
    else {
    digitalWrite(pumpPin, HIGH);   
    Blynk.virtualWrite(V3, "Not Watering");
  }
  
  }
}

BLYNK_WRITE(V4) {
  int pumpControl = param.asInt();
  
  if (pumpControl == 0) {
    digitalWrite(pumpPin, LOW); 
    Blynk.virtualWrite(V3, "Manually Watering");
  } else {
    digitalWrite(pumpPin, HIGH);  
    Blynk.virtualWrite(V3, "Manual Stop");
  }
}