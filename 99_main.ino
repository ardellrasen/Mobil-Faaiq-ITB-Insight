void printReceivedMessage();

// Setup
void setup() {
  Serial.begin(BAUD);
  pinMode(LED_BUILTIN, OUTPUT);
  // Setup Actuator
  setupDriver();
  setupSteer();
  setupKicker();
  setupDribbler();
  // Setup Connection
  setupWifi();
  getMessageHttp();
  server.begin();

  pinMode(KICKER, OUTPUT);
}

// Main
void loop() {
  // if (WiFi.status() == WL_CONNECTED) {
  //   digitalWrite(LED_BUILTIN, HIGH);
  //   if (millis() - prevTime > 3) {
  //     printReceivedMessage();
  //     prevTime = 0;
  //   }
  // } else {
  //   digitalWrite(LED_BUILTIN, HIGH);
  //   Serial.println("Disconnected from WiFi");
  // }

  if (WiFi.softAPgetStationNum()) {
    digitalWrite(LED_BUILTIN, HIGH);
    printReceivedMessage();
  } else {
    digitalWrite(LED_BUILTIN, LOW);
  }

  speed = speedString.toInt();
  forward = forwardString.toInt();
  backward = backwardString.toInt();
  right = rightString.toInt();
  left = leftString.toInt();
  rotate_right = rotateRightString.toInt();
  rotate_left = rotateLeftString.toInt();
  is_kick = kickString.toInt();
  is_dribble = dribbleString.toInt();

  if (rotate_right) right = 1;
  if (rotate_left) left = 1;

  move(speed);
  steering(right, left);
  kick(is_kick);
  dribble(is_dribble);
}

void printReceivedMessage() {
  Serial.print("speed: ");
  Serial.print(speed);
  Serial.print(", forward: ");
  Serial.print(forward);
  Serial.print(", backward: ");
  Serial.print(backward);
  Serial.print(", right: ");
  Serial.print(right);
  Serial.print(", left: ");
  Serial.print(left);
  Serial.print(", dribble: ");
  Serial.print(is_dribble);
  Serial.print(", kick: ");
  Serial.println(is_kick);
}