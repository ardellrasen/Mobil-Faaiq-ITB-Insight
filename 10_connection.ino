// ESP32 as Access Point
// (Uncomment all to use)

void setupWifi() {
  delay(2000);
  Serial.println("Setting AP (Access Point)...");
  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);
}

void getMessageHttp() {
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    if (request->hasParam("speed") && request->hasParam("forward") && request->hasParam("backward") && request->hasParam("right") && request->hasParam("left") && request->hasParam("rotate_right") && request->hasParam("rotate_left") && request->hasParam("dribble") && request->hasParam("kick")) {
      speedString = request->getParam("speed")->value();
      forwardString = request->getParam("forward")->value();
      backwardString = request->getParam("backward")->value();
      rightString = request->getParam("right")->value();
      leftString = request->getParam("left")->value();
      rotateRightString = request->getParam("rotate_right")->value();
      rotateLeftString = request->getParam("rotate_left")->value();
      dribbleString = request->getParam("dribble")->value();
      kickString = request->getParam("kick")->value();

      request->send(200, "text/plain", "Data received: speed=" + speedString + ", forward=" + forwardString + ", backward=" + backwardString + ", right=" + rightString + ", left=" + leftString + ", dribble=" + dribbleString + ", kick=" + kickString);
      // kalo http request berhasil
      digitalWrite(LED_BUILTIN, HIGH);
    } else {
      request->send(400, "text/plain", "Bad Request: Missing parameters");
      // kalo http request gagal
      digitalWrite(LED_BUILTIN, LOW);
    }
  });
}