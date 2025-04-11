#define TINY_GSM_MODEM_SIM800
#define SerialAT Serial1
#define TINY_GSM_DEBUG Serial
#define GSM_PIN ""

#include <PubSubClient.h>
#include <TinyGsmClient.h>
#include <TimeLib.h>
#include <Wire.h>
#include "FS.h"
#include "SD.h"
#include "SPI.h"
#include <Adafruit_INA219.h>
#include <RTClib.h>
#include <ModbusMaster.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <BH1750.h>
#include <Adafruit_SHT31.h>

#define SEALEVELPRESSURE_HPA (1013.25)
Adafruit_BME280 bme; // I2C

#define sensor_1_address 0x76
#define sensor_2_address 0x77

#define BUILTIN_LED 2
float suhuu, kelemb, kelem, suuhuu;

Adafruit_SHT31 sht31 = Adafruit_SHT31();
float suhu, temperature;

//Adafruit_BME280 bme; // I2C device 1
//Adafruit_BME280 bme2; // I2C device 2

float suu, kuu, ttt, tuu;

BH1750 lightMeter;

const float kalibrasilux = 3.5;

ModbusMaster node;
File Data;
RTC_DS3231 rtc;

Adafruit_INA219 panel (0x40);
Adafruit_INA219 baterai (0x45);
Adafruit_INA219 mppt (0x41);

float sensorsuhu485, hasil,  su485, ku485, su, ac, tsp, ku, tu, tt, cha, chr, ch, presentase, aa, ka, lux, ab, ap, tb, tp, tm, am;
char daysOfTheWeek[7][12] = {"Minggu", "Senin", "Selasa", "Rabu", "Kamis", "Jum'at", "Sabtu"};
String hari, ht;
int jam, menit, detik, tanggal, bulan, tahun;
int counter = 0;

const char apn[] = "internet";
const char gprsUser[] = "";
const char gprsPass[] = "";

const char* broker = "203.175.10.112";

String mqttUsername = "teminsiot";
String mqttPassword = "t3m1n510t";
String mqtt_id = "0015";
String mqtt_topic = "temins_iot/" + mqtt_id + "/data/";

TinyGsm modem(SerialAT);
TinyGsmClient client(modem);
PubSubClient mqtt(client);

#define EN_RS485 25
#define MODEM_TX 16
#define MODEM_RX 17
#define RX_PIN 26 //DI
#define TX_PIN 27 //RO
#define RST_PIN 32

float sp = 0;
float bp = 0;
float pp = 0;

float sm = 0;
float bm = 0;
float pm = 0;

float sb = 0;
float bb = 0;
float pb = 0;

uint16_t nilai;
uint32_t lastReconnectAttempt = 0, lastNetworkCheck = 0;
long lastMsg = 0;
long lastSuccessfulPublish = 0;
unsigned long patokan;
boolean CHreset = 0;
int cardStatus = 0;

/*
  arah angin  =  0x03
  kec angin   =  0x02
  curah hujan =  0x01
  suhu        =  0x04
*/

void setup() {
  Serial.begin(115200);
  delay(1000);
  pinMode(EN_RS485, OUTPUT);
  pinMode(BUILTIN_LED, OUTPUT);
  pinMode(RST_PIN, OUTPUT);
  Wire.begin();
  lightMeter.begin();
  rtc.begin();
  //  SD.begin(5);
  //
  if (! sht31.begin(0x44)) {   // Set to 0x45 for alternate I2C address
    Serial.println("Couldn't find SHT31");
    while (1) delay(1);
  }
  unsigned status;
  status = bme.begin(0x76);

  //  bool device_status = bme.begin(sensor_1_address);
  //  if (!device_status) Serial.println("Could not find a BME280 sensor, check wiring or address of device-1");
  //  device_status = bme2.begin(sensor_2_address);
  //  if (!device_status) Serial.println("Could not find a BME280 sensor, check wiring or address of device-2");

  Serial.println("Initializing...");
  SerialAT.begin(115200, SERIAL_8N1, MODEM_TX, MODEM_RX);
  delay(3000);

  panel.begin();
  baterai.begin();
  mppt.begin();


  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
  }
  //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  //rtc.adjust(DateTime(2024, 04, 29, 15, 08, 10));

  reconnectModem();

  mqtt.setServer(broker, 1883);
  mqtt.setCallback(mqttCallback);
}

void loop() {
  uint32_t now = millis();
  if (now - lastSuccessfulPublish > 300000) {
    Serial.println("No data sent for 5 minutes. Reconnecting modem...");
    reconnectModem();
    lastSuccessfulPublish = now;
  }
  if (!mqtt.connected()) {
    if (now - lastReconnectAttempt > 5000) {
      lastReconnectAttempt = now;
      if (mqttConnect()) {
        lastReconnectAttempt = 0;
      }
    }
    delay(100);
    return;
  }


  if (millis() - patokan > 1000) {
    daya();
    getBME();
    getSHT();
    //waktu();
    //datalog();
    sensorsuhu485 = suhu485(0x04);
    delay(10);
    ka = sensor485(0x02);
    delay(10);
    aa = sensor485(0x03);
    aa += 40;
    if (aa > 360) {
      aa = aa - 360;
    }
    delay(10);
    ch = sensor485(0x01);
    delay(10);
    lux = lightMeter.readLightLevel();
    lux = kalibrasilux * lux;
    //serialmonitor();
    patokan = millis();
  }

  if (now - lastMsg > 1400) {
    lastMsg = now;
    kirimData();
  }
  mqtt.loop();
}

void reconnectModem() {
  Serial.println("Restarting modem...");
  digitalWrite(RST_PIN, LOW);
  delay(500);
  digitalWrite(RST_PIN, HIGH);
  delay(500);
  modem.restart();
  Serial.println("Connecting to network...");
  if (!modem.waitForNetwork(60000)) {  // Timeout 60 detik
    Serial.println("Failed to connect to network. Restarting...");
    ESP.restart();
  }
  Serial.println("Connecting to APN...");
  if (!modem.gprsConnect(apn, gprsUser, gprsPass)) {
    Serial.println("Failed to connect to GPRS. Restarting...");
    ESP.restart();
  }
  Serial.println("Reconnected!");
}
