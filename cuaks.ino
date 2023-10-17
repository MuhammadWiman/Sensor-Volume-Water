#define BLYNK_PRINT Serial

/* Uncomment this to see the verbose Blynk protocol log */
//#define BLYNK_DEBUG

/* Fill in information from Blynk Device Info here */
/* Read more: https://bit.ly/BlynkSimpleAuth */
<<<<<<< HEAD
#define BLYNK_TEMPLATE_ID "isi sesuai Tutor di web Blynk"
#define BLYNK_TEMPLATE_NAME "Pendeteksi Volume Air"
#define BLYNK_AUTH_TOKEN "isi sesuai Tutor di web Blynk"
=======
#define BLYNK_TEMPLATE_ID "Silahkan isi template yang tersedia pada Dashboard Blynk"
#define BLYNK_TEMPLATE_NAME "Pendeteksi Volume Air"
#define BLYNK_AUTH_TOKEN "Silahkan isi Token yang tersedia pada Dashboard Blynk"
>>>>>>> 686d6dc3108eeedeb0ea0054fde457c0d8ffedde

/* BlynkMultiClient allows attaching Blynk to the standard Arduino Client,
   and also allows multiple (up to 4) connections to be registered.
   NOTE: don't replace it with any of the BlynkSimple*.h variants */
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

/*
 * WiFi
 */

// Your WiFi credentials.
// Set password to "" for open networks.
const char* ssid = "Pasca Sarjana";
const char* pass = "ublkecee";

// Sensor Ultrasonik
const int trigPin = D1;
const int echoPin = D2;
long duration;
int distance;
int minDistance = 0;
int maxDistance = 20;

// LED
const int ledPinM = D0;
const int ledPinK = D7;
const int ledPinH = D4;


/*
 * Main
 */
BlynkTimer timer; 
void KirimSensor () {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration / 2) / 29.1;  // Menghitung jarak dalam sentimeter


  // Kirim data jarak ke Blynk.Cloud
  if (isnan(distance)) {
    Serial.println("Gagal Membaca Nilai Sensor Jarak");
    return;
  }
  else if (distance >= 20) {
    Serial.println("Nilai Sensor Jarak Terlalu Jauh");
    Blynk.virtualWrite(V1, minDistance);
    Blynk.virtualWrite(V3, 0);
    Blynk.virtualWrite(V2, 0);
    Blynk.virtualWrite(V4, 0);
    return;
  }
  int total_data = maxDistance - distance;
  
  
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V1, total_data); // kita gunakan pin virtual 5 untuk humidity (kelembaban) 
  Serial.println("Data Sudah terkirim....");
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // Kontrol LED berdasarkan jarak
  if (distance <= 10) {
    Blynk.virtualWrite(V2, 1);
    Blynk.virtualWrite(V3, 0);
    Blynk.virtualWrite(V4, 0);
    digitalWrite(ledPinM, HIGH);
    digitalWrite(ledPinK, LOW);
    digitalWrite(ledPinH, LOW);
  } else if (distance <= 15) {
    Blynk.virtualWrite(V4, 1);
    Blynk.virtualWrite(V3, 0);
    Blynk.virtualWrite(V2, 0);
    digitalWrite(ledPinK, HIGH);
    digitalWrite(ledPinM, LOW);
    digitalWrite(ledPinH, LOW);     
 } else {
    Blynk.virtualWrite(V3, 1);
    Blynk.virtualWrite(V2, 0);
    Blynk.virtualWrite(V4, 0);
    digitalWrite(ledPinH, HIGH); 
    digitalWrite(ledPinM, LOW);
    digitalWrite(ledPinK, LOW);
  }
  delay(2000);  // Delay sebelum membaca jarak lagi
  
}

void setup()
{
  // Debug console
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ledPinM, OUTPUT);
  pinMode(ledPinK, OUTPUT);
  pinMode(ledPinH, OUTPUT);
  Serial.begin(115200);
  delay(10);
  

  // Setup Blynk
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass); 
  timer.setInterval(1000L, KirimSensor);
}

void loop()
{
  
  Blynk.run();
  timer.run();
}
