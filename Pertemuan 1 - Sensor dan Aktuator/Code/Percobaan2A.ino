#include <DHT.h>

#define DHTPIN 4
#define DHTTYPE DHT11

// Pin kendali relay/LED indikator
#define RELAYPIN 26

DHT dht(DHTPIN, DHTTYPE);

// Menentukan dua ambang batas (Histerisis)
const float suhuAtas = 30.0;
const float suhuBawah = 28.0;

void setup() {
  Serial.begin(115200);
  dht.begin();
  
  pinMode(RELAYPIN, OUTPUT);
  // Pastikan aktuator dalam kondisi mati di awal program
  digitalWrite(RELAYPIN, LOW); 
  
  Serial.println("Memulai sistem kontrol aktuator dengan Histerisis...");
}

void loop() {
  float suhu = dht.readTemperature();
  float kelembaban = dht.readHumidity();

  if (isnan(suhu) || isnan(kelembaban)) {
    Serial.println("Gagal membaca data sensor!");
  } else {
    Serial.print("Suhu Saat Ini: ");
    Serial.print(suhu);
    Serial.print(" °C -> ");

    // Kendali aktuator menggunakan sistem dua ambang batas (Histerisis)
    if (suhu > suhuAtas) {
      digitalWrite(RELAYPIN, HIGH); // Aktifkan relay/LED
      Serial.println("Aktuator: ON (Suhu melampaui batas atas)");
    } 
    else if (suhu < suhuBawah) {
      digitalWrite(RELAYPIN, LOW); // Matikan relay/LED
      Serial.println("Aktuator: OFF (Suhu berada di bawah batas bawah)");
    } 
    else {
      // Jika suhu berada di antara batas bawah (28) dan batas atas (30),
      // Relay mempertahankan status terakhirnya tanpa perubahan.
      Serial.println("Aktuator: STATUS TETAP (Suhu di rentang toleransi)");
    }
  }
  
  delay(2000); // jeda pembacaan setiap 2 detik
}