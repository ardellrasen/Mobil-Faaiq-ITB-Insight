void setupKicker() {
  // Using XY-MOS
  pinMode(KICKER, OUTPUT);
  digitalWrite(KICKER, LOW);
}

void kick(int is_kick) {
  if (start_time_kick) is_kick = 1;

  if (is_kick) {
    if (!start_time_kick) start_time_kick = millis();

    if (millis() - start_time_kick <= time_kick) {
      digitalWrite(KICKER, HIGH); // turn on kicker
    }
    else {
      digitalWrite(KICKER, LOW); // turn off kicker
      // reset
      start_time_kick = 0;
      is_kick = 0;
    }
  }
}