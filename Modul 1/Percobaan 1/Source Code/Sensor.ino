#include <DHT.h>

#define DHTPIN 4
#define DHTTYPE DHT11

// Inisialisasi sensor DHT11
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  dht.begin();
  Serial.println("Memulai akuisisi data sensor DHT11 dengan sistem rata-rata...");
}

void loop() {
  float totalSuhu = 0;
  float totalKelembaban = 0;
  int pembacaanBerhasil = 0;

  // Lakukan 5 kali proses pembacaan data
  for (int i = 0; i < 5; i++) {
    float kelembaban = dht.readHumidity();
    float suhu = dht.readTemperature();
    
    // Periksa apakah hasil pembacaan gagal/NaN
    if (isnan(kelembaban) || isnan(suhu)) {
      Serial.println("Gagal membaca data dari sensor DHT11!");
    } else {
      // Jika berhasil, tambahkan nilai ke dalam total 
      totalKelembaban += kelembaban;
      totalSuhu += suhu;
      pembacaanBerhasil++;
    }
    
    // Tunggu 2 detik untuk setiap siklus baca, sesuai spesifikasi sensor
    delay(2000); 
  }

  // Jika minimal ada 1 pembacaan berhasil, hitung dan tampilkan rata-ratanya
  if (pembacaanBerhasil > 0) {
    float rataSuhu = totalSuhu / pembacaanBerhasil;
    float rataKelembaban = totalKelembaban / pembacaanBerhasil;
    
    Serial.println("------------------------------------");
    Serial.print("Rata-rata Suhu (dari "); Serial.print(pembacaanBerhasil); Serial.print(" data): "); 
    Serial.print(rataSuhu); Serial.println(" °C");
    
    Serial.print("Rata-rata Kelembaban: "); 
    Serial.print(rataKelembaban); Serial.println(" %");
    Serial.println("------------------------------------");
  } else {
    Serial.println("Gagal total mengambil 5 sampel data.");
  }
}