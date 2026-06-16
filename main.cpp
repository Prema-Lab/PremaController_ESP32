#include <Arduino.h>
#include <WiFi.h>
#include <Bluepad32.h>
#include <WebSocketsServer.h>
#include <ArduinoJson.h> // Required for dynamic configuration

// === LLM INJECTION ZONE 1: CUSTOM LIBRARIES & PINS ===
// (e.g., #include <ESP32Servo.h>, define motor pins, sensor pins)

// =====================================================

// --- Wi-Fi & Network Setup ---
const char* ssid = "YOUR_WIFI_SSID"; 
const char* password = "YOUR_WIFI_PASSWORD";
const int WS_PORT = 8080;
WebSocketsServer webSocket = WebSocketsServer(WS_PORT);
TaskHandle_t WSTask;

// === LLM INJECTION ZONE 2: STATE VARIABLES & PARAMETERS ===
// (e.g., volatile float fb_frequency = 0.2;)

// ==========================================================

ControllerPtr myControllers[BP32_MAX_GAMEPADS];

// --- Function Prototypes ---
void onConnectedController(ControllerPtr ctl);
void onDisconnectedController(ControllerPtr ctl);
void webSocketTaskCode(void * parameter); 
void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length);
void sendConfiguration(uint8_t num);

void setup() {
    Serial.begin(115200);

    // === LLM INJECTION ZONE 3: HARDWARE SETUP ===
    // (e.g., attach servos, set pinModes, analogReadResolution)
    
    // ============================================

    // Network Connect
    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED) { delay(500); Serial.print("."); }
    Serial.println("\nWiFi Connected! IP: " + WiFi.localIP().toString());

    // WebSocket Start
    webSocket.begin();
    webSocket.onEvent(webSocketEvent);
    xTaskCreatePinnedToCore(webSocketTaskCode, "WSTask", 10000, NULL, 1, &WSTask, 0);                  
    BP32.setup(&onConnectedController, &onDisconnectedController);
}

void loop() {
    BP32.update();
    ControllerPtr ctl = myControllers[0];

    // === LLM INJECTION ZONE 4: ROBOT CONTROL LOGIC ===
    // (e.g., read gamepad inputs, drive motors, read sensors)

    // =================================================

    delay(20); 
}

// --- WebSocket Event Handler ---
void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
    switch(type) {
        case WStype_DISCONNECTED:
            Serial.printf("[%u] Disconnected\n", num);
            break;
        case WStype_CONNECTED:
            Serial.printf("[%u] Connected\n", num);
            sendConfiguration(num); // Trigger Handshake
            break;
        case WStype_TEXT: {
            String request = String((char*)payload);
            request.trim();
            
            // Format expected: "SET:parameter_id:value"
            if (request.startsWith("SET:")) {
                int firstColon = request.indexOf(':');
                int secondColon = request.indexOf(':', firstColon + 1);
                
                if (firstColon != -1 && secondColon != -1) {
                    String key = request.substring(firstColon + 1, secondColon);
                    float value = request.substring(secondColon + 1).toFloat();
                    
                    // === LLM INJECTION ZONE 5: PARAMETER UPDATES ===
                    // (e.g., if (key == "freq") fb_frequency = value;)

                    // ===============================================
                }
            }
            break;
        }
    }
}

// --- Dynamic Configuration Handshake ---
void sendConfiguration(uint8_t num) {
    StaticJsonDocument<1024> doc;
    
    // === LLM INJECTION ZONE 6: SLIDER CONFIGURATION ===
    // Define the UI for this specific robot
    JsonArray sliders = doc.createNestedArray("sliders");
    
    // Example format the LLM should generate:
    // JsonObject s1 = sliders.createNestedObject();
    // s1["id"] = "freq"; s1["label"] = "Frequency"; s1["min"] = 1; s1["max"] = 100; s1["val"] = 20; s1["scale"] = 0.01;
    
    // ==================================================

    // === LLM INJECTION ZONE 7: TELEMETRY CONFIGURATION ===
    JsonArray telemetry = doc.createNestedArray("telemetry");
    
    // Example format:
    // telemetry.add("Voltage (V)");
    // telemetry.add("Current (A)");
    
    // =====================================================

    String configStr;
    serializeJson(doc, configStr);
    String payload = "CONFIG:" + configStr;
    webSocket.sendTXT(num, payload);
}

// --- Telemetry Broadcasting Task ---
void webSocketTaskCode(void * parameter) {
    unsigned long lastTelemetryTime = 0;
    for(;;) {
        webSocket.loop(); 
        unsigned long currentMillis = millis();
        
        if (currentMillis - lastTelemetryTime >= 50) {
            lastTelemetryTime = currentMillis;
            
            // === LLM INJECTION ZONE 8: TELEMETRY DATA ===
            // Replace with actual sensor variables. MUST match the order in Zone 7.
            // Example format: "DATA:<timestamp>,<val1>,<val2>"
            
            // char msg[128];
            // snprintf(msg, sizeof(msg), "DATA:%lu,%.2f,%.2f", currentMillis, voltage_var, current_var);
            // webSocket.broadcastTXT(msg);
            
            // ============================================
        }
        vTaskDelay(10 / portTICK_PERIOD_MS); 
    }
}

void onConnectedController(ControllerPtr ctl) { if (myControllers[0] == nullptr) myControllers[0] = ctl; }
void onDisconnectedController(ControllerPtr ctl) { if (myControllers[0] == ctl) myControllers[0] = nullptr; }