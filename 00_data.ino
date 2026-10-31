// Wifi
const char* ssid = "ESP32-Faaiq";
const char* password = "12345678";

AsyncWebServer server(80);
String serverName = "http://192.168.4.1/";

String speedString;
String forwardString;
String backwardString;
String rightString;
String leftString;
String rotateRightString;
String rotateLeftString;
String dribbleString;
String kickString;

// Driver
enum {STOP, MOVE};
int speed = 1;
int forward = 0;
int backward = 0;
int right = 0;
int left = 0;
int rotate_right = 0;
int rotate_left = 0;
int prev_pwm = 0;
// Right Motor
#define ENA 23
#define IN1 22
#define IN2 21
// Left Motor
#define ENB 17
#define IN3 19
#define IN4 18

// Steer
#define STEER 13
Servo steer;
const int min_us = 1000;
const int max_us = 2500;
const int default_angle = 110;
const int left_angle = 40;
const int right_angle = 180;
int angle = 0;
int prev_angle = angle;

// Kicker
#define KICKER 27
int is_kick = 0;
const int time_kick = 1000;
int start_time_kick = 0;

// Dribbler
#define LEFT_DRIBBLER 25
#define RIGHT_DRIBBLER 26
Servo left_dribbler;
Servo right_dribbler;
int is_dribble = 0;

int prevTime = 0;