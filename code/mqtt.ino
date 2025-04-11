void kirimData() {
  Serial.println("Mengirim data...");

  mqtt.publish((mqtt_topic + "suuhuu").c_str(), String(suhuu).c_str());
  mqtt.publish((mqtt_topic + "kelemb").c_str(), String(kelem).c_str());

  Serial.println("Data terkirim!");
  lastSuccessfulPublish = millis();
}

void mqttCallback(char* topic, byte* message, unsigned int len) {
    Serial.print("Pesan diterima di topik: ");
    Serial.println(topic);

    String incoming_message;
    for (int i = 0; i < len; i++) {
        incoming_message += (char)message[i];
    }
    incoming_message.trim();

    Serial.println("Pesan: " + incoming_message);
    
    String kontrol = mqtt_topic + "kontrol"; 
    if (String(topic) == kontrol) {
        if (incoming_message == "ON") {
            digitalWrite(LED_BUILTIN, HIGH);
        } else if (incoming_message == "OFF") {
            digitalWrite(LED_BUILTIN, LOW);
        }
    }
}

boolean mqttConnect() {
    //Serial.print("Menghubungkan ke MQTT broker...");
    if (!mqtt.connect(mqtt_id.c_str(), mqttUsername.c_str(), mqttPassword.c_str())) {
        Serial.println(" Gagal!");
        delay(5000);
        return false;
    }

    //Serial.println(" Terhubung!");

    String kontrol = mqtt_topic + "kontrol"; 
    mqtt.subscribe(kontrol.c_str());

    return mqtt.connected();
}
