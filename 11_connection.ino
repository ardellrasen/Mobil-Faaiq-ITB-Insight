// // Using Hotspot
// // (Uncomment all to use)

// void setupWifi() {
//   delay(2000);
//   WiFi.begin(ssid, password);
//   IPAddress local_IP(192, 168, 4, 10); 
//   IPAddress gateway(192, 168, 4, 1); 
//   IPAddress subnet(255, 255, 255, 0);

//   WiFi.config(local_IP, gateway, subnet);

//   while (WiFi.status() != WL_CONNECTED) {
//     delay(500);
//     Serial.print(".");
//   }

//   Serial.println("");
//   Serial.println("WiFi connected!");
//   Serial.println("IP address: ");
//   Serial.println(WiFi.localIP());

//   if (WiFi.SSID() == ssid) {
//     digitalWrite(LED_BUILTIN, HIGH);
//     Serial.println("Connected to the desired WiFi network");
//   } else {
//     digitalWrite(LED_BUILTIN, LOW);
//     Serial.println("Connected to a different WiFi network");
//   }
// }

// void getMessageHttp() {
//   server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
//     if (request->hasParam("speed") && request->hasParam("forward") && request->hasParam("backward") && request->hasParam("turning_angle") &&request->hasParam("dribble") && request->hasParam("kick")) {
//       speedString = request->getParam("speed")->value();
//       forwardString = request->getParam("forward")->value();
//       backwardString = request->getParam("backward")->value();
//       angleString = request->getParam("turning_angle")->value();
//       dribbleString = request->getParam("dribble")->value();
//       kickString = request->getParam("kick")->value();

//       request->send(200, "text/plain", "Data received: speed=" + speedString + ", forward=" + forwardString + ", backward=" + backwardString + ", angle=" + angleString + ", dribble=" + dribbleString + ", kick=" + kickString);
//       // kalo http request berhasil
//       digitalWrite(LED_BUILTIN, HIGH);
//     } else {
//       request->send(400, "text/plain", "Bad Request: Missing parameters");
//       // kalo http request gagal
//       digitalWrite(LED_BUILTIN, LOW);
//     }
//   });
// }