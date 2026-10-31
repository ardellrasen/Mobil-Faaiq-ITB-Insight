void setupSteer() {
  steer.attach(STEER, min_us, 2000);
  steer.setPeriodHertz(100);
  steer.write(default_angle);
}

void steering(int right, int left) {
  if (right && !left) angle = right_angle;
  else if (left && !right) angle = left_angle;
  else angle = default_angle;

  angle = 0.045*angle + 0.955*prev_angle;
  steer.write(angle);
  prev_angle = angle;
}