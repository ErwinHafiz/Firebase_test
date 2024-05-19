#include "Send_database.h"
#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include "kredensial.h"
#include "Pseudo_DHT22.h"

#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

// Firebase configuration variables
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

// Variables for sending data
unsigned long sendDataPrevMillis = 0;
int count = 0;
bool signupOK = false;

void connectToWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
}

void setupFirebase() {
  // Assign the API key (required)
  config.api_key = API_KEY;

  // Assign the RTDB URL (required)
  config.database_url = DATABASE_URL;

  // Sign up
  if (Firebase.signUp(&config, &auth,"","")) {
    Serial.println("ok");
    signupOK = true;
  } else {
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  // Assign the callback function for the long running token generation task
  config.token_status_callback = tokenStatusCallback; // see addons/TokenHelper.h
  
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void setupSend() {
  connectToWiFi();
  setupFirebase();
}

void loopSend() {
  if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 15000 || sendDataPrevMillis == 0)) {
    sendDataPrevMillis = millis();
    // Read temperature and humidity
    float temperature = readTemperature();
    float humidity = readHumidity();
    
    // Write temperature and humidity to Firebase
    if (Firebase.RTDB.setFloat(&fbdo, "test/temperature", temperature) && Firebase.RTDB.setFloat(&fbdo, "test/humidity", humidity)) {
      Serial.println("Data sent successfully");
      Serial.println("Temperature: " + String(temperature));
      Serial.println("Humidity: " + String(humidity));
    } else {
      Serial.println("Failed to send data");
    }
  }
}
