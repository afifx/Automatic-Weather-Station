void serialmonitor() {
  Serial.println("=========SERIAL MONITOR=========");
  Serial.println("Hari, Tanggal : " + String() + hari + ", " + tanggal + "-" + bulan + "-" + tahun);
  Serial.println("Jam : " + String() + jam + ":" + menit + ":" + detik);
  Serial.print("Suhu : "); Serial.print(su); Serial.println(" °C");
  Serial.print("Tekanan Udara : "); Serial.print(tu); Serial.println(" hPa");
  Serial.print("Altitude : "); Serial.print(tt); Serial.println(" m");
  Serial.print("Kelembapan : "); Serial.print(ku); Serial.println(" %");
  Serial.print("Lux : "); Serial.println(lux);
  Serial.print("Arah Angin  : "); Serial.println(aa);
  Serial.print("Kecepatan Angin : "); Serial.println(ka);
  Serial.print("Curah Hujan : "); Serial.println(ch);
  Serial.print("Suhu 485 : "); Serial.println(su485);
  Serial.print("Kelembapan 485 : "); Serial.println(ku485);
  Serial.print("Suhu : "); Serial.println(suhu);
  Serial.print("Kelembapan : "); Serial.println(temperature);
  Serial.println("----------------------------");
}
