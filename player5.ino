#include "WiFi.h"
#include "ESPAsyncWebServer.h"
// #include <AsyncEspFsWebServer.h>
#include <ESP32Servo.h>
// #include <TFScope22.h>  // library tidak ditemukan & tidak dipakai di kode

#ifndef LED_BUILTIN
#define LED_BUILTIN 2  // LED onboard ESP32 DevKit
#endif

#define BAUD 115200
#define PROG_NAME "Player"