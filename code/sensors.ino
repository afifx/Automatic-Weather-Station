
void preTransmission()
{
  digitalWrite(EN_RS485, 1);
}
void postTransmission ()
{
  digitalWrite(EN_RS485, 0);
}

void waktu() {
  DateTime now = rtc.now();
  jam     = now.hour();
  menit   = now.minute();
  detik   = now.second();
  tanggal = now.day();
  bulan   = now.month();
  tahun   = now.year();
  hari    = daysOfTheWeek[now.dayOfTheWeek()];
}

void daya() {
  sp = panel.getShuntVoltage_mV();
  bp = panel.getBusVoltage_V();
  ac = panel.getCurrent_mA();
  tsp = bp + (sp / 1000);
  pp = (panel.getPower_mW()) / 1000;

  sm = mppt.getShuntVoltage_mV();
  bm = mppt.getBusVoltage_V();
  am = mppt.getCurrent_mA();
  tm = bm + (sm / 1000);
  pm = (mppt.getPower_mW()) / 1000;

  sb = baterai.getShuntVoltage_mV();
  bb = baterai.getBusVoltage_V();
  ab = baterai.getCurrent_mA();
  tb = bb + (sb / 1000);
  pb = (baterai.getPower_mW()) / 1000;
}

int suhu485(uint16_t alamat) {
  Serial2.begin(4800, SERIAL_8N1, RX_PIN, TX_PIN);
  node.preTransmission(preTransmission);
  node.postTransmission(postTransmission);
  node.begin(alamat, Serial2);
  delay(5);
  int result;
  //node.writeSingleRegister(0x7D0, 4);
  result = node.readHoldingRegisters(0, 2);
  if (result == node.ku8MBSuccess)
  {
    su485 = node.getResponseBuffer(1) / 10.0;
    ku485 = node.getResponseBuffer(0) / 10.0;
  } else {
    Serial.println("modbus fail " + String(alamat));
  }
  Serial2.end();
  return su485;
}

float sensor485(uint16_t alamat) {
  Serial2.begin(4800, SERIAL_8N1, RX_PIN, TX_PIN);
  node.preTransmission(preTransmission);
  node.postTransmission(postTransmission);
  node.begin(alamat, Serial2);
  delay(5);
  int result;
  //node.writeSingleRegister(0x7D0, 4);
  result = node.readHoldingRegisters(0, 1);
  if (result == node.ku8MBSuccess)
  {
    hasil = node.getResponseBuffer(0) / 10.0;
  } else {
    Serial.println("modbus fail " + String(alamat));
  }
  Serial2.end();
  return hasil;
}

float resetCH(uint16_t alamat) {
  Serial.println("Resettinggg...");
  Serial2.begin(4800, SERIAL_8N1, RX_PIN, TX_PIN);
  node.preTransmission(preTransmission);
  node.postTransmission(postTransmission);
  node.begin(alamat, Serial2);
  delay(1);
  int result;
  result = node.writeSingleRegister(0x00, 0x5A);
  //result = node.beginTransmission(0x01, 0x00,  0x5A, 1);
  //result = node.readHoldingRegisters(0, 1);
  if (result == node.ku8MBSuccess)
  {
    hasil = node.getResponseBuffer(0) / 10.0;
    //Serial.println("Balasan :" + String(hasil));
  } else {
    Serial.println("modbus fail " + String(alamat));
  }
  Serial2.end();
  Serial.println("Resettinggg...end");
  return hasil;
}

void getBME() {
  suhuu = bme.readTemperature();
  kelem = bme.readHumidity();
  tu = bme.readPressure();
  tt = bme.readAltitude(1013.25);
  Serial.print("Suhu : "); Serial.println(suhuu);
  Serial.print("Kelembapan : "); Serial.println(kelem);
}


void getLux() {
  lux = lightMeter.readLightLevel();
  lux = kalibrasilux * lux;
}

void getSHT() {
  suhu = sht31.readTemperature();
  temperature = sht31.readHumidity();
}
