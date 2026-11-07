#include <WiFi.h>
#include <WebServer.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include <string.h>

// =====================================
// KONFIGURASI WIFI
// =====================================

const char* ssid = "Kos Kantin Depan";
const char* password = "kantindepan";

// =====================================
// KONFIGURASI PIN
// =====================================

#define DHT_PIN 4
#define DHT_TYPE DHT11

#define RELAY_PIN 23
#define LED_PIN 2

#define SDA_PIN 21
#define SCL_PIN 22

// =====================================
// OBJEK KOMPONEN
// =====================================

DHT dht(DHT_PIN, DHT_TYPE);

LiquidCrystal_I2C lcd(0x27, 16, 2);

WebServer server(80);

// =====================================
// VARIABEL SISTEM
// =====================================

bool lampState = false;

float temperature = NAN;
float humidity = NAN;

unsigned long lastSensorRead = 0;
unsigned long lastLCDUpdate = 0;

// =====================================
// WEBSITE DASHBOARD
// =====================================

const char MAIN_page[] PROGMEM = R"rawliteral(

<!DOCTYPE html>
<html lang="en">

<head>

<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">

<title>NEXORA - Smart Room System</title>

<style>

* {
  margin: 0;
  padding: 0;
  box-sizing: border-box;
}

body {
  font-family: 'Segoe UI', sans-serif;
  background: #080f1e;
  color: white;
  min-height: 100vh;
  overflow-x: hidden;
  padding: 25px;
}

.background {
  position: fixed;
  inset: 0;
  overflow: hidden;
  z-index: -1;
}

.blob {
  position: absolute;
  width: 300px;
  height: 300px;
  border-radius: 50%;
  filter: blur(100px);
  opacity: 0.35;
  animation: float 8s infinite alternate ease-in-out;
}

.blob.one {
  background: #2563eb;
  top: -100px;
  left: -80px;
}

.blob.two {
  background: #7c3aed;
  bottom: -100px;
  right: -80px;
  animation-delay: 2s;
}

@keyframes float {
  from {
    transform: translate(0, 0);
  }
  to {
    transform: translate(80px, 60px);
  }
}

.container {
  max-width: 950px;
  margin: auto;
}

.header {
  display: flex;
  justify-content: space-between;
  align-items: center;
  margin-bottom: 35px;
  flex-wrap: wrap;
  gap: 15px;
}

.logo {
  font-size: 25px;
  font-weight: 800;
  letter-spacing: 1px;
}

.logo span {
  color: #60a5fa;
}

.connection {
  background: #142d29;
  color: #4ade80;
  padding: 9px 15px;
  border-radius: 30px;
  font-size: 12px;
}

.dot {
  display: inline-block;
  width: 8px;
  height: 8px;
  background: #4ade80;
  border-radius: 50%;
  margin-right: 6px;
  animation: pulse 1.5s infinite;
}

@keyframes pulse {
  50% {
    opacity: 0.3;
    transform: scale(0.7);
  }
}

.welcome {
  margin-bottom: 25px;
}

.welcome h1 {
  font-size: clamp(25px, 5vw, 36px);
  margin-bottom: 8px;
}

.welcome p {
  color: #94a3b8;
  font-size: 14px;
}

.grid {
  display: grid;
  grid-template-columns: repeat(2, 1fr);
  gap: 18px;
}

.card {
  background: rgba(22, 35, 58, 0.8);
  border: 1px solid rgba(148, 163, 184, 0.12);
  border-radius: 20px;
  padding: 25px;
  backdrop-filter: blur(15px);
  box-shadow: 0 10px 35px rgba(0,0,0,0.2);
  transition: 0.3s;
  animation: appear 0.8s ease;
}

.card:hover {
  transform: translateY(-5px);
  border-color: #3b82f6;
}

@keyframes appear {
  from {
    opacity: 0;
    transform: translateY(20px);
  }
  to {
    opacity: 1;
    transform: translateY(0);
  }
}

.card-title {
  color: #94a3b8;
  font-size: 14px;
  margin-bottom: 20px;
}

.icon {
  font-size: 30px;
  margin-bottom: 12px;
}

.value {
  font-size: 38px;
  font-weight: 700;
}

.unit {
  color: #94a3b8;
  font-size: 16px;
}

.temperature {
  color: #fb923c;
}

.humidity {
  color: #60a5fa;
}

.lamp-card {
  grid-column: span 2;
  text-align: center;
  padding: 30px;
}

.lamp {
  font-size: 75px;
  margin: 15px 0;
  transition: 0.5s;
  filter: grayscale(1);
}

.lamp.on {
  filter: grayscale(0);
  animation: glow 2s infinite alternate;
}

@keyframes glow {
  from {
    filter: drop-shadow(0 0 5px #facc15);
  }
  to {
    filter: drop-shadow(0 0 25px #facc15);
  }
}

.status {
  font-size: 14px;
  color: #94a3b8;
  margin-bottom: 20px;
}

.buttons {
  display: flex;
  justify-content: center;
  gap: 15px;
  flex-wrap: wrap;
}

button {
  border: none;
  padding: 13px 35px;
  border-radius: 12px;
  font-size: 15px;
  font-weight: 700;
  cursor: pointer;
  transition: 0.25s;
}

button:hover {
  transform: scale(1.06);
}

button:active {
  transform: scale(0.95);
}

.on-btn {
  background: #16a34a;
  color: white;
}

.off-btn {
  background: #dc2626;
  color: white;
}

button:disabled {
  opacity: 0.4;
  cursor: not-allowed;
}

.footer {
  text-align: center;
  color: #64748b;
  margin-top: 30px;
  font-size: 12px;
}

@media(max-width: 550px) {
  body {
    padding: 18px;
  }

  .grid {
    grid-template-columns: 1fr;
  }

  .lamp-card {
    grid-column: span 1;
  }

  .value {
    font-size: 32px;
  }
}

</style>

</head>

<body>

<div class="background">
  <div class="blob one"></div>
  <div class="blob two"></div>
</div>

<div class="container">

  <div class="header">

    <div class="logo">
      NEXO<span>RA</span>
    </div>

    <div class="connection" id="connection">
      <span class="dot"></span>
      CONNECTING
    </div>

  </div>

  <div class="welcome">

    <h1>Room Monitoring</h1>

    <p>
      ESP32 Smart Room Control System
    </p>

  </div>

  <div class="grid">

    <div class="card">

      <div class="card-title">
        TEMPERATURE
      </div>

      <div class="icon">🌡️</div>

      <div class="value temperature">
        <span id="temp">--.-</span>
        <span class="unit">°C</span>
      </div>

      <p class="card-title">
        Room temperature
      </p>

    </div>

    <div class="card">

      <div class="card-title">
        HUMIDITY
      </div>

      <div class="icon">💧</div>

      <div class="value humidity">
        <span id="hum">--</span>
        <span class="unit">%</span>
      </div>

      <p class="card-title">
        Room humidity
      </p>

    </div>

    <div class="card lamp-card">

      <div class="card-title">
        LIGHT CONTROL
      </div>

      <div class="lamp" id="lamp">
        💡
      </div>

      <h2 id="lampStatus">
        LAMP OFF
      </h2>

      <p class="status" id="relayStatus">
        Relay is inactive
      </p>

      <div class="buttons">

        <button class="on-btn" id="onBtn"
          onclick="setLamp('on')">
          TURN ON
        </button>

        <button class="off-btn" id="offBtn"
          onclick="setLamp('off')">
          TURN OFF
        </button>

      </div>

    </div>

  </div>

  <div class="footer">
    NEXORA · SMART ROOM SYSTEM · POWERED BY ESP32
    <br><br>
    Monitor. Connect. Control.
  </div>

</div>

<script>

async function updateDashboard() {

  try {

    const response = await fetch('/api', {
      cache: 'no-store'
    });

    if (!response.ok) {
      throw new Error('Connection error');
    }

    const data = await response.json();

    document.getElementById('temp').textContent =
      data.temperature === null ? '--.-' : data.temperature;

    document.getElementById('hum').textContent =
      data.humidity === null ? '--' : data.humidity;

    const lamp = document.getElementById('lamp');

    const status = document.getElementById('lampStatus');

    const relay = document.getElementById('relayStatus');

    if (data.relay) {

      lamp.classList.add('on');

      status.textContent = 'LAMP ON';

      relay.textContent = 'Relay is active';

    } else {

      lamp.classList.remove('on');

      status.textContent = 'LAMP OFF';

      relay.textContent = 'Relay is inactive';

    }

    document.getElementById('connection').innerHTML =
      '<span class="dot"></span> ESP32 ONLINE';

  }

  catch(error) {

    document.getElementById('connection').textContent =
      '● DISCONNECTED';

  }

}

async function setLamp(state) {

  const buttons = document.querySelectorAll('button');

  buttons.forEach(button => button.disabled = true);

  try {

    const response = await fetch('/relay?state=' + state);

    if (!response.ok) {
      throw new Error('Command failed');
    }

    await updateDashboard();

  }

  catch(error) {

    alert('Gagal mengirim perintah ke ESP32');

  }

  finally {

    buttons.forEach(button => button.disabled = false);

  }

}

updateDashboard();

setInterval(updateDashboard, 2500);

</script>

</body>
</html>

)rawliteral";

// =====================================
// FUNGSI LCD
// =====================================

void printLCDLine(int row, const char* text) {

  lcd.setCursor(0, row);

  lcd.print(text);

  int length = strlen(text);

  for (int i = length; i < 16; i++) {
    lcd.print(" ");
  }

}

// =====================================
// FUNGSI KONTROL RELAY
// =====================================

void controlLamp(bool state) {

  lampState = state;

  if (lampState) {

    // Relay Active LOW
    digitalWrite(RELAY_PIN, LOW);

    // LED internal ON
    digitalWrite(LED_PIN, HIGH);

  }

  else {

    // Relay OFF
    digitalWrite(RELAY_PIN, HIGH);

    // LED internal OFF
    digitalWrite(LED_PIN, LOW);

  }

}

// =====================================
// MEMBACA SENSOR DHT11
// =====================================

void readSensor() {

  float t = dht.readTemperature();
  float h = dht.readHumidity();

  if (!isnan(t) && !isnan(h)) {

    temperature = t;
    humidity = h;

  }

}

// =====================================
// UPDATE LCD
// =====================================

void updateLCD() {

  char line1[17];
  char line2[17];

  if (isnan(temperature) || isnan(humidity)) {

    snprintf(line1, sizeof(line1), "Sensor Error");

  }

  else {

    snprintf(
      line1,
      sizeof(line1),
      "T:%.1fC H:%.0f%%",
      temperature,
      humidity
    );

  }

  snprintf(
    line2,
    sizeof(line2),
    "LAMP: %s",
    lampState ? "ON" : "OFF"
  );

  printLCDLine(0, line1);
  printLCDLine(1, line2);

}

// =====================================
// SETUP WEB SERVER
// =====================================

void setupWebServer() {

  // Halaman utama
  server.on("/", HTTP_GET, []() {

    server.send_P(
      200,
      "text/html",
      MAIN_page
    );

  });

  // Data sensor dan status relay
  server.on("/api", HTTP_GET, []() {

    String json = "{";

    json += "\"temperature\":";

    if (isnan(temperature)) {
      json += "null";
    } else {
      json += String(temperature, 1);
    }

    json += ",\"humidity\":";

    if (isnan(humidity)) {
      json += "null";
    } else {
      json += String(humidity, 0);
    }

    json += ",\"relay\":";
    json += (lampState ? "true" : "false");

    json += "}";

    server.send(
      200,
      "application/json",
      json
    );

  });

  // Kontrol lampu
  server.on("/relay", HTTP_GET, []() {

    if (!server.hasArg("state")) {

      server.send(400, "text/plain", "Missing state");

      return;

    }

    String state = server.arg("state");

    if (state == "on") {

      controlLamp(true);

      server.send(200, "text/plain", "Relay ON");

    }

    else if (state == "off") {

      controlLamp(false);

      server.send(200, "text/plain", "Relay OFF");

    }

    else {

      server.send(400, "text/plain", "Invalid command");

    }

  });

  server.begin();

  Serial.println("Web Server Started");

}

// =====================================
// SETUP
// =====================================

void setup() {

  Serial.begin(115200);

  // Inisialisasi pin
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);

  // Kondisi awal: semua OFF
  controlLamp(false);

  // Inisialisasi I2C
  Wire.begin(SDA_PIN, SCL_PIN);

  // Inisialisasi LCD
  lcd.init();
  lcd.backlight();

  printLCDLine(0, "NEXORA");
  printLCDLine(1, "Initializing...");

  // Inisialisasi DHT11
  dht.begin();

  delay(2000);

  // =====================================
  // KONEKSI WIFI
  // =====================================

  WiFi.mode(WIFI_STA);

  WiFi.begin(ssid, password);

  Serial.println();
  Serial.print("Connecting to WiFi");

  printLCDLine(0, "Connecting WiFi");
  printLCDLine(1, "Please wait...");

  while (WiFi.status() != WL_CONNECTED) {

    delay(500);

    Serial.print(".");

  }

  Serial.println();

  Serial.println("WiFi Connected!");

  Serial.print("ESP32 IP Address: ");

  Serial.println(WiFi.localIP());

  // =====================================
  // TAMPILKAN IP DI LCD
  // =====================================

  lcd.clear();

  printLCDLine(0, "WiFi Connected");

  String ipAddress = WiFi.localIP().toString();

  printLCDLine(1, ipAddress.c_str());

  // IP ditampilkan selama 5 detik
  delay(5000);

  // =====================================
  // MEMBACA SENSOR AWAL
  // =====================================

  readSensor();

  // =====================================
  // MENJALANKAN WEB SERVER
  // =====================================

  setupWebServer();

  // Update LCD dengan data sensor
  updateLCD();

  Serial.println("System Ready!");

}

// =====================================
// LOOP
// =====================================

void loop() {

  server.handleClient();

  // Baca sensor setiap 2 detik
  if (millis() - lastSensorRead >= 2000) {

    lastSensorRead = millis();

    readSensor();

  }

  // Update LCD setiap 1 detik
  if (millis() - lastLCDUpdate >= 1000) {

    lastLCDUpdate = millis();

    updateLCD();

  }

}