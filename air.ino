#define triggerPin 16
#define echoPin 5
#define BLYNK_PRINT Serial


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "";
char pass[] = "";

int tinggiPenampung = 10; // cm
int batasAtas = 9; // cm, batas penuhnya air. Mematikan motor.
int batasBawah = 2; // cm, batas sedikitnya air. Menyalakan motor.
int pompa = 0; // off

void setup() {
  Serial.begin (9600);
  Blynk.begin(auth, ssid, pass);
  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);
}
 
void loop() {
  long duration, distance, ketinggian;
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(8);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(8);
  digitalWrite(triggerPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;
  ketinggian = tinggiPenampung - distance;
  Serial.print("Ketinggian air: ");
  Serial.print(ketinggian);
  Serial.println(" cm");

  if (ketinggian >= batasAtas && pompa == 1){
    delay(500);
    Serial.println("Tank Penuh, pompa akan dimatikan.");
    Blynk.notify("Tank Penuh, pompa akan dimatikan.");
    pompa = 0;
    Blynk.virtualWrite(V1, LOW);
  }
  else if (ketinggian <= batasBawah && distance < tinggiPenampung && pompa == 0){
    delay(500);
    
    Serial.println("Tank Kosong, pompa akan dinyalakan.");
    Blynk.notify("Tank Kosong, pompa akan dinyalakan.");
    pompa = 1;
    Blynk.virtualWrite(V1, HIGH);
  }
  
  Blynk.virtualWrite(V5, ketinggian);
  Blynk.virtualWrite(V4, distance);
  delay(200);

  Blynk.run();
  delay(200);
}
