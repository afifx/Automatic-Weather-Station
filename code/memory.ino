/*void initMicroSD() {
  if (!SD.begin()) {
    Serial.println("Card Mount Failed");
    //return;
  } else {
    cardStatus = 1;
  }
  uint8_t cardType = SD.cardType();

  if (cardType == CARD_NONE) {
    Serial.println("No SD card attached");
    //return;
  }

  Serial.print("SD Card Type: ");
  if (cardType == CARD_MMC) {
    Serial.println("MMC");
  } else if (cardType == CARD_SD) {
    Serial.println("SDSC");
  } else if (cardType == CARD_SDHC) {
    Serial.println("SDHC");
  } else {
    Serial.println("UNKNOWN");
  }
}

void saveMicroSD() {
  Serial.println("SD Interval Reached");
  Serial.println(ht);
  //if (ht.toInt() > 0) {
  String namafile = "/Temins_AWS_" + mqtt_id + "_" + ht + ".csv";
  File file = SD.open(namafile.c_str());
  //Serial.println("Sampai sini 1");
  if (!file) {
    Serial.println("File doesn't exist");
    Serial.println("Creating file...");
    writeFile(SD, namafile.c_str(), "Suhu;Kelembapan;Kecepatan Angin;Arah Angin;Lux;Tekanan;Tinggi Tempat;Curah Hujan;V Baterai;Waktu\r\n");
  }
  else {
    // Serial.println("Sampai sini 2");
    Serial.println("File already exists");
    Serial.println("Menyimpan ke file");
    String dataSD = String(su) + ";" + String(ku) + ";" + String(ka) + ";" + String(aa) + ";" + String(lux) + ";" + String(tu) + ";" + String(tt) + ";" + String(ch) + ";" + String(tsp) + ";" + String(waktu) + "\r\n";
    Serial.println(dataSD);
    Serial.println(dataSD.c_str());
    appendFile(SD, namafile.c_str(), dataSD.c_str());
  }
  //}
  patokanSD = millis();
}

void writeFile(fs::FS &fs, const char * path, const char * message) {
  Serial.printf("Writing file: %s\n", path);

  File file = fs.open(path, FILE_WRITE);
  if (!file) {
    Serial.println("Failed to open file for writing");
    cardStatus = 0;
    Serial.println("Reinitialization Card....");
    if (!SD.begin()) {
      Serial.println("Card Mount Failed");
      //return;
    } else {
      cardStatus = 1;
    }
    //return;
  }
  if (file.print(message)) {
    Serial.println("File written");
    cardStatus = 1;
  } else {
    Serial.println("Write failed");
    cardStatus = 0;
  }
  file.close();
}

void appendFile(fs::FS &fs, const char * path, const char * message) {
  Serial.printf("Appending to file: %s\n", path);

  File file = fs.open(path, FILE_APPEND);
  if (!file) {
    Serial.println("Failed to open file for appending");
    cardStatus = 0;
    Serial.println("Reinitialization Card....");
    if (!SD.begin()) {
      Serial.println("Card Mount Failed");
      //return;
    } else {
      cardStatus = 1;
    }
    //return;
  }
  if (file.print(message)) {
    Serial.println("Message appended");
    cardStatus = 1;
  } else {
    Serial.println("Append failed");
    cardStatus = 0;
  }
  file.close();
}*/
