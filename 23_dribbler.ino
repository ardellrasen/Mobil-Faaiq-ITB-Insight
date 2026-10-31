void setupDribbler() {
  // Using 2 servo
  left_dribbler.attach(LEFT_DRIBBLER, min_us, max_us);
  right_dribbler.attach(RIGHT_DRIBBLER, min_us, max_us);
  left_dribbler.write(180);
  right_dribbler.write(0);
}

void close() {
  left_dribbler.write(0);
  right_dribbler.write(150);
}

void open() {
  left_dribbler.write(120);
  right_dribbler.write(60);
}

void dribble(int is_dribble) {
  if (is_dribble) {
    close();
  }
  else {
    open();
  }
}