void setupDriver() {
  // Setup motors pin
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // Turn off motors
  stop();
}

void move(int speed) {
  int pwm = map(speed, 1, 5, 120, 255);
  pwm = (pwm*0.1) + (prev_pwm*0.9);
  prev_pwm = pwm;

  if (forward) {
    moveForward(pwm);
  }
  else if (backward) {
    moveBackward(pwm);
  }
  else if (rotate_right) {
    rotateRight(pwm);
  }
  else if(rotate_left) {
    rotateLeft(pwm);
  }
  else {
    pwm = 0;
    prev_pwm = pwm;
    stop();
  }
}

void stop() {
  // Right Motor
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  // Left Motor
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void moveForward(int pwm) {
  // Right Motor
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  // Left Motor
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  // Set PWM
  analogWrite(ENA, pwm);
  analogWrite(ENB, pwm);
}

void moveBackward(int pwm) {
  // Right Motor
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  // Left Motor
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  // Set PWM
  analogWrite(ENA, pwm);
  analogWrite(ENB, pwm);
}

void rotateRight(int pwm) {
  // Right Motor
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  // Left Motor
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  // Set PWM
  analogWrite(ENA, pwm);
  analogWrite(ENB, pwm);
}

void rotateLeft(int pwm) {
  // Right Motor
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  // Left Motor
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  // Set PWM
  analogWrite(ENA, pwm);
  analogWrite(ENB, pwm);

}