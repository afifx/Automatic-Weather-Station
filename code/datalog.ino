void datalog() {
  Data = SD.open("Data.csv", FILE_WRITE);
  if (Data) {
    Data.print(jam);
    Data.print(':');
    Data.print(menit);
    Data.print(':');
    Data.print(detik);
    Data.println(',');
    Serial.println("DATA TERSIMPAN");
  }
  Data.close();
}
