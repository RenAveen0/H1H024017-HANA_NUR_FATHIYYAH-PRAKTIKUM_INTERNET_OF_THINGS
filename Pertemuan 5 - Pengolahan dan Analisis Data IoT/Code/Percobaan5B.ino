#include <ESP8266WiFi.h> // Library khusus untuk ESP8266
#include <PubSubClient.h>
#include <ArduinoJson.h>

// WAJIB DIUBAH: Masukkan nama dan password WiFi/Tethering kamu
const char* ssid = "NAMA_WIFI_KAMU"; 
const char* password = "PASSWORD_WIFI_KAMU";

// Pengaturan Broker MQTT publik
const char* mqtt_server = "broker.hivemq.com";
// PENTING: Sesuaikan topic ini agar SAMA PERSIS dengan program Python!
const char* topic_data = "unsoed/tk245004/kelompokAnda/data";

WiFiClient espClient;
PubSubClient client(espClient);

// Mock function pengganti sensor DHT22
float bacaSuhuMock() {
    static unsigned long jumlahBaca = 0;
    jumlahBaca++;

    float suhuDasar = 28.0 + 2.0 * sin(jumlahBaca * 0.08);
    float noise = random(-5, 6) * 0.1;
    float suhu = suhuDasar + noise;

    // Lonjakan suhu setiap 25 pembacaan untuk simulasi anomali
    if (jumlahBaca % 25 == 0) {
        suhu += 8.0;
    }
    return suhu;
}

void setup_wifi() {
    delay(10);
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);
    
    WiFi.begin(ssid, password);
    
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected");
}

void reconnect() {
    while (!client.connected()) {
        Serial.print("Attempting MQTT connection...");
        // Membuat Client ID acak
        String clientId = "ESP8266Client-";
        clientId += String(random(0xffff), HEX);
        
        if (client.connect(clientId.c_str())) {
            Serial.println("connected");
        } else {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 5 seconds");
            delay(5000);
        }
    }
}

void setup() {
    Serial.begin(115200);
    setup_wifi();
    client.setServer(mqtt_server, 1883);
}

void loop() {
    if (!client.connected()) {
        reconnect();
    }
    client.loop();

    // Mengambil data dari mock function
    float suhu = bacaSuhuMock();

    if (isnan(suhu)) {
        Serial.println("Pembacaan gagal");
        return;
    }

    // Membentuk data ke dalam format JSON
    StaticJsonDocument<200> doc;
    doc["suhu"] = suhu;

    char jsonBuffer[512];
    serializeJson(doc, jsonBuffer);

    // Publikasi data ke broker
    Serial.print("Publish message: ");
    Serial.println(jsonBuffer);
    client.publish(topic_data, jsonBuffer);

    // Interval publish 5 detik
    delay(5000);
}