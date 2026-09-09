# Pertemuan 1 - Sensor dan Aktuator 

## Tujuan dan Penjelasan Singkat
Praktikum pada Pertemuan 1 ini berfokus pada dua konsep fundamental dalam ekosistem Internet of Things (IoT), yaitu akuisisi data sensor dan kendali aktuator berbasis mikrokontroler ESP8266:
1. Akuisisi Data Sensor: Mengambil data fisik lingkungan berupa suhu dan kelembaban udara menggunakan sensor digital DHT11, mengolah sinyal digital tersebut di ESP8266, dan menampilkannya secara berkala pada Serial Monitor.
2. Kendali Aktuator: Menggunakan hasil pemrosesan data sensor untuk mengendalikan perangkat keluaran secara otomatis (modul relay atau LED indikator) berdasarkan ambang batas (*threshold*) suhu tertentu.
3. Analisis Sistem: Memahami alur kerja pemrosesan dari sinyal input (sensor), logika pengambilan keputusan (*decision making*), hingga aksi fisik (*actuation*) pada perangkat IoT.

## Peralatan yang Diperlukan
<div align="center">
<table border="1" cellpadding="10" cellspacing="0" width="100%">
  <tr align="center">
    <th>ESP8266</th>
    <th>Breadboard</th>
    <th>Kabel Jumper</th>
    <th>Relay Modul</th>
    <th>Sensor DHT11</th>
    <th>Kabel USB Type B</th>
    <th>Arduino IDE</th>
  </tr>

  <tr align="center">
    <td>
      <img width="100" height="100" alt="img1" src="../Pertemuan 1 - Sensor dan Aktuator/Images/esp8266.png"><br>
    </td>
    <td>
      <img width="100" height="100" alt="img2" src="../Pertemuan 1 - Sensor dan Aktuator/Images/breadboard.jpg"><br>
    </td>
    <td>
      <img width="100" height="100" alt="img3" src="../Pertemuan 1 - Sensor dan Aktuator/Images/kabel jumper.jpg"><br>
    </td>
    <td>
      <img width="100" height="100" alt="img4" src="../Pertemuan 1 - Sensor dan Aktuator/Images/modul relay 2chn.jpg"><br>
    </td>
    <td>
      <img width="100" height="100" alt="img5" src="../Pertemuan 1 - Sensor dan Aktuator/Images/sensor dht11.jpg"><br>
    </td>
    <td>
      <img width="100" height="100" alt="img6" src="../Pertemuan 1 - Sensor dan Aktuator/Images/kabel micro usb arduino.jpg"><br>
    </td>
    <td>
      <img width="100" height="100" alt="img7" src="../Pertemuan 1 - Sensor dan Aktuator/Images/arduino ide.png"><br>
    </td>
  </tr>
</table>
</div>

## Percobaan 1A
### Gambaran Umum 
Percobaan 1A bertujuan untuk mengimplementasikan proses pembacaan data lingkungan fisik (suhu dan kelembaban) menggunakan sensor DHT11 yang terhubung ke pin GPIO digital ESP8266. Sensor DHT11 mengirimkan pulsa sinyal digital melalui jalur tunggal (*single-bus/1-Wire protocol*) yang didekode oleh pustaka `DHT.h`. Program melakukan validasi menggunakan fungsi `isnan()` untuk mendeteksi kegagalan komunikasi sensor dan menampilkan nilai pembacaan ke Serial Monitor setiap 2 detik.

### Skematik Percobaan
```
[ ESP8266 3.3V ] -------------------- [ VCC DHT11 ]
[ ESP8266 GPIO 4 ] ------------------- [ DATA DHT11 ]
[ ESP8266 GND ] ---------------------- [ GND DHT11 ]
```

### Kode Program
```cpp
#include <DHT.h>

#define DHTPIN 4
#define DHTTYPE DHT11

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

  for (int i = 0; i < 5; i++) {
    float kelembaban = dht.readHumidity();
    float suhu = dht.readTemperature();

    if (isnan(kelembaban) || isnan(suhu)) {
      Serial.println("Gagal membaca data dari sensor DHT11!");
    } else {
      totalKelembaban += kelembaban;
      totalSuhu += suhu;
      pembacaanBerhasil++;
    }
    delay(2000); 
  }

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
```

### Penjelasan Kode per Baris Fungsi
1. #include <DHT.h> : Mengimpor pustaka (library) sensor DHT agar mikrokontroler dapat mengakses fungsi dan protokol komunikasi pembacaan sensor DHT.
2. #define DHTPIN 4 : Mendefinisikan konstanta macro bahwa pin keluaran data sensor terhubung ke pin GPIO 4 pada ESP8266.
3. #define DHTTYPE DHT11 : Menentukan varian sensor yang digunakan, yaitu DHT11.
4. DHT dht(DHTPIN, DHTTYPE); : Melakukan instansiasi objek bernama dht dari kelas DHT dengan memasukkan konfigurasi pin dan tipe sensor.
5. void setup() { ... } : Fungsi konfigurasi awal yang hanya dieksekusi satu kali saat ESP8266 pertama kali dinyalakan atau di-reset.
    - Serial.begin(115200); : Memulai komunikasi serial USART dengan kecepatan transfer data (baud rate) 115200 bps untuk komunikasi dengan Serial Monitor PC.
    - dht.begin(); : Menginisialisasi jalur komunikasi data dan sensor DHT agar siap membaca data.
    - Serial.println("Memulai akuisisi data sensor DHT11..."); : Mengirim pesan teks informasi awal ke Serial Monitor.
6. void loop() { ... } : Fungsi utama yang dieksekusi secara berulang terus-menerus selama board menyala.
    - float kelembaban = dht.readHumidity(); : Memanggil fungsi internal pustaka untuk membaca nilai kelembaban relatif dalam satuan persen (%) dan menyimpannya ke variabel bertipe float.
    - float suhu = dht.readTemperature(); : Memanggil fungsi pembacaan suhu dalam derajat Celsius (°C) dan menyimpannya ke variabel bertipe float.
    - if (isnan(kelembaban) || isnan(suhu)) : Mengevaluasi kondisi apakah nilai variabel kelembaban atau suhu menghasilkan nilai Not a Number (NaN), yang mengindikasikan kegagalan koneksi atau pembacaan sensor.
    - Serial.println("Gagal membaca data dari sensor DHT11!"); : Menampilkan notifikasi kesalahan jika kondisi isnan() bernilai benar (true).
    - else { ... } : Blok eksekusi yang dijalankan apabila nilai suhu dan kelembaban berhasil dibaca secara valid.
    - Serial.print(...) & Serial.println(...) : Mencetak teks label, nilai suhu aktual, dan nilai kelembaban aktual ke Serial Monitor secara terformat.
    - delay(2000); : Memberikan jeda waktu eksekusi selama 2000 milidetik (2 detik) sebelum siklus pembacaan berikutnya, menyesuaikan spesifikasi sampling rate sensor DHT.

### Library/Dependencies yang Dibutuhkan
1. DHT sensor library.
2. Adafruit Unified Sensor.
3. ESP8266 Board Support Package pada Arduino IDE Boards Manager.

### Pertanyaan Praktikum
1. Gambarkan diagram alur (flowchart) proses akuisisi data sensor pada program di atas!
2. Apa fungsi dari perintah `isnan()` pada program tersebut?
3. Jelaskan mengapa diperlukan jeda (*delay*) minimal sekitar 2 detik antar pembacaan sensor DHT11/DHT22!
4. Modifikasi program agar data suhu dan kelembaban dirata-ratakan dari 5 kali pembacaan sebelum ditampilkan, dan berikan penjelasan di setiap baris kode yang ditambahkan!

### Jawaban Praktikum
1. Diagram Alur (Flowchart) Proses Akuisisi Data Sensor
<div align="center">
  <img src="Images/Flowchart 1_1.png" alt="flowchart1A" width="500">
</div>

2. Fungsi isnan() (is Not a Number) digunakan untuk memeriksa apakah nilai variabel yang dihasilkan dari fungsi dht.readHumidity() atau dht.readTemperature() bernilai valid secara numerik atau tidak:  
    - Jika jalur komunikasi 1-Wire sensor terputus, kabel jumper longgar, timing sinyal tidak sesuai, atau sensor belum siap, pustaka DHT akan mengembalikan nilai float khusus berupa NaN.  
    - Fungsi isnan() mengembalikan logika boolean true jika nilai variabel tersebut adalah NaN, dan false jika bernilai angka normal.  
    - Perintah ini berfungsi sebagai mekanisme error handling agar program tidak mengolah atau menampilkan data sampah/rusak ke Serial Monitor.
3. Jeda minimal 2 detik (2000 ms) diperlukan karena batasan karakteristik fisik dan perangkat keras sensor keluarga DHT:  
    - Sampling Rate Terbatas: Sensor DHT11 memiliki frekuensi sampling maksimal 1 Hz (1 kali per detik) dan DHT22 sebesar 0.5 Hz (1 kali per 2 detik).  
    - Stabilisasi Elemen Pengukur: Komponen pengukur kelembaban internal berupa polimer kapasitif serta termistor suhu membutuhkan waktu pemulihan (recovery time) dan stabilisasi termal agar pembacaan berikutnya akurat.
    - Mencegah Data Korup / Cached: Jika mikrokontroler meminta data lebih cepat dari rentang refresh rate sensor, sensor akan mengembalikan pembacaan lama (cached data), data korup, atau gagal merespons sinyal start sehingga memicu galat NaN.
4. Modifikasi Program
```cpp
#include <DHT.h>

#define DHTPIN 4
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  dht.begin();
  Serial.println("Memulai akuisisi data sensor DHT11 dengan sistem rata-rata...");
}

void loop() {
  // Variabel akumulator untuk menampung total nilai
  float totalSuhu = 0.0;
  float totalKelembaban = 0.0;
  int pembacaanBerhasil = 0; // Counter sampel valid

  // Melakukan 5 kali siklus pengambilan sampel data
  for (int i = 0; i < 5; i++) {
    float kelembaban = dht.readHumidity();
    float suhu = dht.readTemperature();

    // Validasi pembacaan data
    if (isnan(kelembaban) || isnan(suhu)) {
      Serial.println("Gagal membaca data dari sensor DHT11!");
    } else {
      totalKelembaban += kelembaban; // Menjumlahkan nilai kelembaban
      totalSuhu += suhu;             // Menjumlahkan nilai suhu
      pembacaanBerhasil++;           // Menambah counter sampel berhasil
    }

    // Jeda 2 detik antar siklus sampling sesuai batas frekuensi sensor
    delay(2000);
  }

  // Menghitung dan mencetak rata-rata jika terdapat minimal 1 sampel valid
  if (pembacaanBerhasil > 0) {
    float rataSuhu = totalSuhu / pembacaanBerhasil;
    float rataKelembaban = totalKelembaban / pembacaanBerhasil;

    Serial.println("------------------------------------");
    Serial.print("Rata-rata Suhu (dari ");
    Serial.print(pembacaanBerhasil);
    Serial.print(" data): ");
    Serial.print(rataSuhu);
    Serial.println(" °C");

    Serial.print("Rata-rata Kelembaban: ");
    Serial.print(rataKelembaban);
    Serial.println(" %");
    Serial.println("------------------------------------");
  } else {
    // Penanganan jika seluruh 5 iterasi sampling gagal
    Serial.println("Gagal total: Tidak ada sampel data valid terbaca!");
  }
}
```

#### Penjelasan Baris Kode Modifikasi:
- float totalSuhu = 0.0; dan float totalKelembaban = 0.0; : Variabel penampung akumulasi (penjumlahan) dari seluruh sampel data suhu dan kelembaban yang berhasil dibaca.
- int pembacaanBerhasil = 0; : Variabel pencatat (counter) berapa banyak sampel yang sukses dibaca tanpa error NaN sebagai pembagi rata-rata yang dinamis.
- for (int i = 0; i < 5; i++) { ... } : Struktur perulangan untuk mengambil 5 sampel pembacaan secara berturut-turut.
- totalKelembaban += kelembaban; dan totalSuhu += suhu; : Menambahkan nilai pembacaan sensor saat ini ke variabel total akumulator.
- pembacaanBerhasil++; : Menambah nilai pencacah sebesar 1 setiap kali ada sampel yang berhasil dibaca.
- delay(2000); (di dalam loop) : Memberikan interval jeda 2 detik pada setiap iterasi agar sensor memiliki waktu stabilisasi antar pengambilan sampel.  
- if (pembacaanBerhasil > 0) { ... } : Memastikan pembagian rata-rata hanya dieksekusi jika ada minimal satu data yang valid guna mencegah kesalahan pembagian dengan nol (division by zero).
- float rataSuhu = totalSuhu / pembacaanBerhasil; : Menghitung nilai rata-rata suhu aktual berdasarkan jumlah data valid.
- float rataKelembaban = totalKelembaban / pembacaanBerhasil; : Menghitung nilai rata-rata kelembaban relatif berdasarkan jumlah data valid.
- else { Serial.println(...); } : Menampilkan pesan kegagalan total apabila semua iterasi pembacaan menghasilkan nilai NaN.

### Dokumentasi
<div align="center">
<table border="1" cellpadding="10" cellspacing="0" width="100%">
    <td>
      <img width="400" alt="dokum1" src="../Pertemuan 1 - Sensor dan Aktuator/Images/M1_1_1.jpg"><br>
    </td>
    <td>
      <img width="400" alt="dokum2" src="../Pertemuan 1 - Sensor dan Aktuator/Images/M1_1_2.jpg"><br>
    </td>
    <td>
      <img width="400" alt="dokum3" src="../Pertemuan 1 - Sensor dan Aktuator/Images/M1_1_3.jpg"><br>
    </td>
    <td>
      <img width="400" alt="dokum4" src="../Pertemuan 1 - Sensor dan Aktuator/Images/M1_1_4.jpg"><br>
    </td>
    <td>
      <img width="400" alt="dokum5" src="../Pertemuan 1 - Sensor dan Aktuator/Images/M1_1_5.jpeg"><br>
    </td>
    <td>
      <img width="400" alt="dokum6" src="../Pertemuan 1 - Sensor dan Aktuator/Images/M1_1_6.jpeg"><br>
    </td>
  </tr>
</table>
</div>

## Percobaan 2A
### Gambaran Umum 
Percobaan 2A memadukan fungsi akuisisi data sensor suhu (DHT11) dengan aktuator berupa modul relay 1-channel atau LED indikator. Mikrokontroler ESP8266 bertindak sebagai pengendali logis: sistem membaca suhu lingkungan, membandingkannya dengan nilai batas kritis (suhuThreshold = 30.0 °C), dan menentukan logika aktuasi. Ketika suhu terbaca melebihi 30.0 °C, ESP8266 mengirim logika digital HIGH ke pin kendali relay untuk mengaktifkan beban. Sebaliknya, saat suhu berada di bawah ambang batas, pin diatur ke logika LOW untuk menonaktifkan relay.

### Skematik Percobaan
```
[ ESP8266 3.3V ] -------------------- [ VCC DHT11 ]
[ ESP8266 GPIO 4 ] ------------------- [ DATA DHT11 ]
[ ESP8266 GND ] ---------------------- [ GND DHT11 ]
[ ESP8266 VIN / 5V ] ----------------- [ VCC Relay ]
[ ESP8266 GPIO 26 ] ------------------ [ IN Relay ]
[ ESP8266 GND ] ---------------------- [ GND Relay ] -------------------> [ GND DHT11 & GND Relay/Katoda LED ]
```

### Kode Program
```cpp
#include <DHT.h>

#define DHTPIN 4
#define DHTTYPE DHT11
#define RELAYPIN 26

DHT dht(DHTPIN, DHTTYPE);

const float suhuAtas = 30.0;
const float suhuBawah = 28.0;

void setup() {
  Serial.begin(115200);
  dht.begin();
  
  pinMode(RELAYPIN, OUTPUT);
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

    if (suhu > suhuAtas) {
      digitalWrite(RELAYPIN, HIGH);
      Serial.println("Aktuator: ON (Suhu melampaui batas atas)");
    } 
    else if (suhu < suhuBawah) {
      digitalWrite(RELAYPIN, LOW);
      Serial.println("Aktuator: OFF (Suhu berada di bawah batas bawah)");
    } 
    else {
      Serial.println("Aktuator: STATUS TETAP (Suhu di rentang toleransi)");
    }
  }
  
  delay(2000);
}
```

### Penjelasan Kode per Baris Fungsi
1. #include <DHT.h> : Mengimpor pustaka DHT untuk mengakses metode pembacaan sensor suhu.
2. #define DHTPIN 4 : Menetapkan pin GPIO 4 sebagai saluran komunikasi data sensor DHT11.
3. #define DHTTYPE DHT11 : Menentukan tipe perangkat keras sensor adalah DHT11.
4. #define RELAYPIN 26 : Menetapkan pin GPIO 26 sebagai pin kendali aktuator (relay atau LED indikator).
5. DHT dht(DHTPIN, DHTTYPE); : Menginisialisasi objek sensor bernama dht.
6. const float suhuThreshold = 30.0; : Mendeklarasikan konstanta ambang batas (threshold) suhu sebesar 30.0 °C sebagai acuan pengaktifan aktuator.
7. void setup() { ... } : Konfigurasi awal perangkat keras :
    - Serial.begin(115200); : Membuka jalur komunikasi serial ke komputer pada kecepatan 115200 bps.
    - dht.begin(); : Mengaktifkan dan memulai komunikasi modul sensor DHT.
    - pinMode(RELAYPIN, OUTPUT); : Mengonfigurasi pin GPIO 26 sebagai keluaran (OUTPUT) digital.
    - digitalWrite(RELAYPIN, LOW); : Menuliskan logika tegangan rendah (LOW / 0V) ke pin relay untuk memastikan aktuator dalam kondisi mati (OFF) di awal program.
8. void loop() { ... } : Siklus pemrosesan utama :
    - float suhu = dht.readTemperature(); : Membaca nilai suhu lingkungan dalam derajat Celsius dari sensor.
    - if (isnan(suhu)) : Mengecek apakah data pembacaan valid atau menghasilkan galat (Not a Number).
    - Serial.println("Gagal membaca data sensor!"); : Memberikan pesan peringatan ke Serial Monitor jika pembacaan gagal.
    - else { ... } : Logika kendali dieksekusi jika data suhu valid:
    - Serial.print("Suhu: "); ... : Menampilkan nilai suhu aktual ke Serial Monitor.
    - if (suhu > suhuThreshold) : Memeriksa apakah suhu lingkungan melebihi nilai batas (30.0 °C).
    - digitalWrite(RELAYPIN, HIGH); : Mengirimkan sinyal logika HIGH (3.3V) ke GPIO 26 untuk mengaktifkan saklar relay / menyalakan LED.
    - Serial.println("Aktuator: ON"); : Mengirim teks informasi status ON ke Serial Monitor.
    - else { ... } : Dijalankan jika suhu berada di bawah atau sama dengan 30.0 °C.
    - digitalWrite(RELAYPIN, LOW); : Mengirim sinyal LOW (0V) ke GPIO 26 untuk memutus rangkaian relay / mematikan LED.
    - Serial.println("Aktuator: OFF"); : Mengirim teks informasi status OFF ke Serial Monitor.
    - delay(2000); : Jeda waktu 2 detik sebelum siklus pembacaan berikutnya.

### Library/Dependencies yang Dibutuhkan
1. DHT sensor library.
2. Adafruit Unified Sensor.
3. ESP8266 Board Support Package pada Arduino IDE Boards Manager.

### Pertanyaan Praktikum
1. Mengapa diperlukan nilai ambang batas (*threshold*) dalam sistem kendali aktuator berbasis sensor?
2. Jelaskan apa yang akan terjadi apabila nilai `suhuThreshold` diturunkan menjadi sangat rendah, misalnya 20.0!
3. Apa perbedaan antara kendali aktuator secara terus-menerus (kondisi tunggal) dengan kendali menggunakan histerisis (dua ambang batas)?
4. Modifikasi program agar menggunakan dua ambang batas (histerisis), misalnya aktuator menyala pada suhu di atas 30°C dan baru mati pada suhu di bawah 28°C, dan berikan penjelasan di setiap baris kodenya!

### Jawaban Praktikum
1. Nilai ambang batas (*threshold*) berfungsi sebagai titik referensi acuan bagi mikrokontroler untuk mengubah data kontinu hasil pembacaan sensor menjadi keputusan aksi biner (ON/OFF). Tanpa *threshold*, sistem IoT tidak memiliki parameter pembanding objektif untuk menentukan kapan kondisi fisik lingkungan dinilai memerlukan tindakan korektif (seperti menyalakan kipas/pompa melalui relay saat suhu melewati batas toleransi).
2. Suhu ruangan normal lingkungan laboratorium atau wilayah tropis umumnya berada pada rentang 25 °C – 32 °C. Apabila `suhuThreshold` diturunkan menjadi 20.0 °C:
    - Kondisi evaluasi logika `(suhu > suhuThreshold)` akan hampir selalu bernilai benar (*true*) setiap saat.
    - Akibatnya, relay akan terus-menerus berada dalam status aktif (`ON`) tanpa pernah mati.
    - Sistem kehilangan sensitivitas kendali dinamisnya dan berisiko memboroskan energi atau menyebabkan *overheating* pada beban listrik yang terhubung ke relay.
3. Perbedaan Kendali Kondisi Tunggal vs Kendali Histerisis
    - Kendali Kondisi Tunggal (*Single Threshold*): Hanya menggunakan 1 titik acuan batas suhu (misalnya tepat 30.0 °C). Kelemahannya, jika suhu lingkungan berfluktuasi tipis di sekitar titik kritis (misalnya naik-turun antara 29.9 °C dan 30.1 °C akibat derau/noise sensor), relay akan beralih menyala dan mati secara berulang-ulang dalam frekuensi cepat (*chattering*/*hunting*). Kondisi ini dapat merusak kontak mekanis saklar relay dalam jangka pendek.
    - Kendali Histerisis (*Dual Threshold*): Memakai dua ambang batas yang menciptakan rentang toleransi (*deadband*), yaitu batas atas (*high threshold*) dan batas bawah (*low threshold*). Aktuator hanya berubah status menjadi `ON` saat suhu melampaui batas atas (30 °C), dan hanya mati (`OFF`) saat suhu turun melintasi batas bawah (28 °C). Selama suhu berada di antara rentang 28 °C – 30 °C, relay mempertahankan status terakhirnya sehingga sistem jauh lebih stabil dan perangkat lebih awet.
4. Modifikasi Program
```cpp
#include <DHT.h>

#define DHTPIN 4
#define DHTTYPE DHT11

// Pin kendali relay
#define RELAYPIN 26

DHT dht(DHTPIN, DHTTYPE);

// Menentukan dua ambang batas (Histerisis)
const float suhuAtas = 30.0;
const float suhuBawah = 28.0;

void setup() {
  Serial.begin(115200);
  dht.begin();
  
  pinMode(RELAYPIN, OUTPUT);
  // Pastikan relay dalam kondisi mati di awal program
  digitalWrite(RELAYPIN, LOW); 
  
  Serial.println("Memulai sistem kontrol aktuator dengan Histerisis...");
}

void loop() {
  float suhu = dht.readTemperature();
  float kelembaban = dht.readHumidity();

  // Validasi keberhasilan pembacaan data sensor
  if (isnan(suhu) || isnan(kelembaban)) {
    Serial.println("Gagal membaca data sensor!");
  } else {
    Serial.print("Suhu Saat Ini: ");
    Serial.print(suhu);
    Serial.print(" °C -> ");

    // Kendali aktuator menggunakan sistem dua ambang batas (Histerisis)
    if (suhu > suhuAtas) {
      digitalWrite(RELAYPIN, HIGH); // Aktifkan relay jika suhu melebihi batas atas
      Serial.println("Aktuator: ON (Suhu melampaui batas atas)");
    } 
    else if (suhu < suhuBawah) {
      digitalWrite(RELAYPIN, LOW);  // Matikan relay jika suhu di bawah batas bawah
      Serial.println("Aktuator: OFF (Suhu berada di bawah batas bawah)");
    } 
    else {
      // Jika suhu berada di rentang toleransi (28°C - 30°C),
      // status relay dipertahankan seperti kondisi terakhir
      Serial.println("Aktuator: STATUS TETAP (Suhu di rentang toleransi)");
    }
  }
  
  delay(2000); // Jeda pembacaan data setiap 2 detik
}
```

#### Penjelasan Baris Kode Modifikasi:
- const float suhuAtas = 30.0; dan const float suhuBawah = 28.0; : Menggantikan variabel tunggal suhuThreshold menjadi dua batas nilai ambang untuk membentuk sistem histeresis (batas atas pengaktifan di 30.0 °C dan batas bawah penonaktifan di 28.0 °C).  
- float kelembaban = dht.readHumidity(); : Menambahkan pembacaan parameter kelembaban udara yang sebelumnya tidak disertakan pada kode dasar modul.  
- if (isnan(suhu) || isnan(kelembaban)) : Memperluas validasi galat (error handling) dengan operator logika || (OR) agar sistem memeriksa kegagalan pembacaan pada sensor suhu maupun sensor kelembaban sekaligus.  
- if (suhu > suhuAtas) : Mengubah kondisi pemicu aktif relay dari acuan ambang tunggal menjadi batas atas 30.0 °C.  
- else if (suhu < suhuBawah) : Menambahkan syarat baru penonaktifan relay, di mana pin kendali hanya diberi sinyal LOW jika suhu terbukti telah turun melintasi batas bawah 28.0 °C.  
- else { Serial.println("Aktuator : STATUS TETAP ..."); } : Menambahkan blok rentang toleransi (deadband) ketika suhu berada di antara 28.0 °C dan 30.0 °C; blok ini sengaja tidak memanggil perintah digitalWrite() agar relay tetap mempertahankan status terakhirnya sehingga terhindar dari siklus saklar cepat (chattering).

### Dokumentasi
<div align="center">
<table border="1" cellpadding="10" cellspacing="0" width="100%">
    <td>
      <img width="400" alt="dokum7" src="../Pertemuan 1 - Sensor dan Aktuator/Images/M1_2_1.jpg"><br>
    </td>
    <td>
      <img width="400" alt="dokum8" src="../Pertemuan 1 - Sensor dan Aktuator/Images/M1_2_2.jpeg"><br>
    </td>
  </tr>
</table>
</div>