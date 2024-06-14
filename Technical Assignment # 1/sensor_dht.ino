#include <WiFi.h>
#include <HTTPClient.h>
#include <DHT.h>

const char *WIFI_SSID = "xyz"; 
const char *WIFI_PASS = "muntilan"; 

// Pin sensor
#define DHTPIN 27     
#define DHTTYPE DHT11   

DHT dht(DHTPIN, DHTTYPE);

// Mengambil data sensor
float get_temperature() {
  float temperature = dht.readTemperature();
  if (isnan(temperature)) {
    Serial.println("Gagal membaca temperatur!");
    return 0.0; 
  } else {
    Serial.print("Temperature : ");
    Serial.print(temperature);
    Serial.println("°C");
    return temperature;
  }
}

float get_humidity() {
  float humidity = dht.readHumidity();
  if (isnan(humidity)) {
    Serial.println("Gagal membaca kelembapan!");
    return 0.0; 
  } else {
    Serial.print("Kelembapan : ");
    Serial.print(humidity);
    Serial.println("%");
    return humidity;
  }
}

void setup() {
  Serial.begin(115200);
  dht.begin();

  Serial.println("Connecting to WiFi ");
  Serial.println(WIFI_SSID);

  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print("Menghubungkan WiFi.....");
  }
  Serial.println("");
  Serial.println("WiFi Terhubung");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  float temperature = get_temperature();
  float humidity = get_humidity();

  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin("http://192.168.116.49:5000/sensor/data"); 
    http.addHeader("Content-Type", "application/json");

    String payload = "{\"temperature\":" + String(temperature) + ",\"humidity\":" + String(humidity, 2) + "}";
    int httpResponseCode = http.POST(payload);

    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println(response);
    } else {
      Serial.print("Gagal mengirim data ke server : ");
      Serial.println(httpResponseCode);
    }
    http.end();
  } else {
    Serial.println("Koneksi ke WiFi error!");
  }

  delay(8000); 
}
