#include <esp_now.h>
#include <WiFi.h>

// ------------------------------
// Data structure to send
// ------------------------------
typedef struct struct_message {
  int buttonState;  // 0 = pressed, 1 = released
} struct_message;

struct_message msg;

// ------------------------------
// Replace this with ESP32 #2 MAC
// ------------------------------
uint8_t receiverMAC[] = { 0x24, 0x6F, 0x28, 0xAA, 0xBB, 0xCC };  // placeholder

// ------------------------------
// ESP-NOW send callback
// ------------------------------
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  // Optional: you can print status for debugging
  // Serial.print("Send Status: "); Serial.println(status == ESP_NOW_SEND_SUCCESS ? "OK" : "FAIL");
}

void setup() {
  Serial.begin(115200);

  // ------------------------------
  // Setup WiFi in STA mode (required for ESP-NOW)
  // ------------------------------
  WiFi.mode(WIFI_STA);

  // ------------------------------
  // Initialize ESP-NOW
  // ------------------------------
  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW init failed");
    return;
  }

  esp_now_register_send_cb(OnDataSent);

  // ------------------------------
  // Add peer (ESP32 #2)
  // ------------------------------
  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, receiverMAC, 6);
  peerInfo.channel = 0;   // same channel (0 = automatic)
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add ESP-NOW peer");
    return;
  }

  Serial.println("ESP32 Sender ready");
}

void loop() {

  // ------------------------------
  // Read Arduino serial input
  // ------------------------------
  if (Serial.available()) {
    String raw = Serial.readStringUntil('\n');
    raw.trim();

    if (raw.length() > 0) {
      msg.buttonState = raw.toInt();  // convert to integer (0 / 1)

      // ------------------------------
      // Send via ESP-NOW
      // ------------------------------
      esp_now_send(receiverMAC, (uint8_t*)&msg, sizeof(msg));
    }
  }
}
