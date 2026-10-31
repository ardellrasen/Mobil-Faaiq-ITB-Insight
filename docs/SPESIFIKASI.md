# Spesifikasi Firmware — Player 5 (Mobil Faaiq)

Dokumen ini menjelaskan firmware robot **Player 5**, yaitu mobil robot berbasis ESP32 yang
dikendalikan lewat Wi-Fi. Robot punya penggerak diferensial, servo kemudi, kicker, dan
dribbler dua servo. Isi dokumen diturunkan langsung dari kode sumber pada commit `0152003`.

---

## 1. Ringkasan

| Item | Nilai |
|---|---|
| Mikrokontroler | ESP32 (FQBN `esp32:esp32:esp32`) |
| Core Arduino ESP32 | 3.3.12 |
| Mode jaringan | Access Point (ESP32 membuat hotspot sendiri) |
| SSID / Password | `ESP32-Faaiq` / `12345678` |
| IP robot | `192.168.4.1` (default soft-AP) |
| Protokol kontrol | HTTP GET ke `/` dengan query parameter |
| Port HTTP | 80 |
| Baud serial | 115200 |
| Ukuran firmware | ±942 KB (71% flash), RAM global ±46 KB (14%) |

---

## 2. Struktur Kode

Arduino IDE / arduino-cli menggabungkan semua file `.ino` dalam folder sesuai urutan nama
file (`player5.ino` lebih dulu, lalu urut alfabet). Karena itu, awalan angka pada nama file
menentukan urutan deklarasi.

| File | Isi |
|---|---|
| `player5.ino` | `#include` library, fallback `LED_BUILTIN`, konstanta `BAUD` dan `PROG_NAME` |
| `00_data.ino` | Semua variabel global, definisi pin, dan konstanta tuning |
| `10_connection.ino` | **Aktif.** Mode Access Point: `setupWifi()` dan handler HTTP `getMessageHttp()` |
| `11_connection.ino` | **Nonaktif (seluruhnya dikomentari).** Alternatif mode Station (terhubung ke hotspot luar) |
| `20_driver.ino` | Kendali 2 motor DC lewat driver H-bridge (L298N atau sejenisnya) |
| `21_steer.ino` | Servo kemudi |
| `22_kicker.ino` | Kicker (aktuator lewat modul MOSFET XY-MOS) |
| `23_dribbler.ino` | Dribbler dari 2 servo (penjepit bola) |
| `99_main.ino` | `setup()`, `loop()`, dan log serial |

### 2.1 Dependensi Library

| Library | Versi teruji | Keterangan |
|---|---|---|
| ESP Async WebServer (ESP32Async) | 3.12.1 | Header `ESPAsyncWebServer.h` |
| Async TCP (ESP32Async) | 3.5.0 | Dependensi web server |
| ESP32Servo | 3.2.1 | Servo kemudi dan dribbler |
| WiFi | bawaan core | Soft-AP |

> **Catatan:** Library lama `ESPAsyncWebServer` 3.1.0 (lacamera/me-no-dev) dan `AsyncTCP`
> 1.1.4 (dvarrel) **tidak kompatibel** dengan core ESP32 3.x karena memakai API mbedTLS
> `*_ret` yang sudah dihapus. Library lama itu harus disingkirkan dari folder `libraries`
> supaya tidak terpilih saat compile.

### 2.2 Build & Upload

```
arduino-cli compile --fqbn esp32:esp32:esp32 .
arduino-cli upload  -p COM5 --fqbn esp32:esp32:esp32 .
arduino-cli monitor -p COM5 -c baudrate=115200
```

---

## 3. Perangkat Keras

### 3.1 Pemetaan Pin ESP32

| Fungsi | Konstanta | GPIO | Mode |
|---|---|---|---|
| Motor kanan — enable/PWM | `ENA` | 23 | `analogWrite` (PWM) |
| Motor kanan — arah 1 | `IN1` | 22 | Digital out |
| Motor kanan — arah 2 | `IN2` | 21 | Digital out |
| Motor kiri — enable/PWM | `ENB` | 17 | `analogWrite` (PWM) |
| Motor kiri — arah 1 | `IN3` | 19 | Digital out |
| Motor kiri — arah 2 | `IN4` | 18 | Digital out |
| Servo kemudi | `STEER` | 13 | Servo, 100 Hz, 1000–2000 µs |
| Kicker (XY-MOS) | `KICKER` | 27 | Digital out |
| Servo dribbler kiri | `LEFT_DRIBBLER` | 25 | Servo, 1000–2500 µs |
| Servo dribbler kanan | `RIGHT_DRIBBLER` | 26 | Servo, 1000–2500 µs |
| LED indikator | `LED_BUILTIN` | 2 (fallback) | Digital out |

### 3.2 Board Catu Daya (dari `Schematic Mobil Faaiq.jpg`)

- **Input:** 12 V lewat konektor JST-XH 2 pin (`X1`). 12 V juga diteruskan ke terminal `X2`
  (kemungkinan untuk driver motor dan kicker).
- **Regulator:** IC 78xx (`IC1`, ditandai "786TV"). Kemungkinan 7805, dengan output 5 V.
- **Kapasitor:** 10 µF + 0,1 µF di sisi input (C1, C3) dan di sisi output (C2, C4).
- **Header:** `SV1` adalah 6 pin rel **5 V**, `SV2` adalah 6 pin rel **GND**. Keduanya
  untuk servo dan ESP32.
- **Indikator:** `LED-IN` (12 V hadir) dan `LED-OUT` (5 V hadir), masing-masing dengan
  resistor 1 kΩ.

> Tiga servo berbagi rel 5 V dari satu regulator linear. Arus puncak servo bisa melebihi
> kemampuan 78xx (±1 A) dan menyebabkan ESP32 brown-out/reset.

---

## 4. Komunikasi

### 4.1 Wi-Fi (Access Point)

`setupWifi()` menunggu 2 detik, lalu menyalakan soft-AP dengan SSID dan password di atas,
kemudian mencetak IP ke serial. Pengendali (laptop atau HP) terhubung ke hotspot ini.

### 4.2 Protokol Perintah HTTP

**Request:**

```
GET http://192.168.4.1/?speed=<1-5>&forward=<0|1>&backward=<0|1>&right=<0|1>&left=<0|1>&rotate_right=<0|1>&rotate_left=<0|1>&dribble=<0|1>&kick=<0|1>
```

| Parameter | Tipe | Rentang | Arti |
|---|---|---|---|
| `speed` | int | 1–5 | Level kecepatan |
| `forward` | flag | 0/1 | Maju |
| `backward` | flag | 0/1 | Mundur |
| `right` | flag | 0/1 | Belok kanan (servo kemudi saja) |
| `left` | flag | 0/1 | Belok kiri (servo kemudi saja) |
| `rotate_right` | flag | 0/1 | Putar di tempat ke kanan |
| `rotate_left` | flag | 0/1 | Putar di tempat ke kiri |
| `dribble` | flag | 0/1 | 1 = jepit bola, 0 = lepas |
| `kick` | flag | 0/1 | Picu tendangan |

**Response:**

- `200 text/plain`, `Data received: speed=…, forward=…, …` jika **kesembilan** parameter
  ada. Response ini tidak menyertakan `rotate_right` dan `rotate_left`.
- `400 text/plain`, `Bad Request: Missing parameters` jika ada parameter yang hilang. Pada
  kasus ini state lama dipertahankan.

Nilai disimpan sebagai `String` di callback async. `loop()` lalu mengonversinya dengan
`toInt()`, sehingga nilai non-angka menjadi 0.

Contoh dengan curl:

```
curl "http://192.168.4.1/?speed=3&forward=1&backward=0&right=0&left=0&rotate_right=0&rotate_left=0&dribble=1&kick=0"
```

Robot **tidak** punya mekanisme timeout atau failsafe. Perintah terakhir terus dijalankan
sampai ada perintah baru, jadi pengendali harus mengirim perintah stop (semua flag 0)
secara eksplisit.

---

## 5. Alur Program

### 5.1 `setup()`

1. Mulai serial 115200, LED sebagai output.
2. `setupDriver()`: semua pin motor output, motor berhenti.
3. `setupSteer()`: servo kemudi ke `default_angle` (110°).
4. `setupKicker()`: kicker LOW.
5. `setupDribbler()`: dribbler ke posisi awal (kiri 180°, kanan 0°).
6. `setupWifi()`, lalu daftarkan handler `/`, lalu `server.begin()`.

### 5.2 `loop()` (tanpa delay, berjalan secepat mungkin)

1. Jika ada klien yang terhubung ke AP, LED menyala dan status dicetak ke serial. Jika
   tidak ada, LED mati.
2. Konversi semua `String` perintah menjadi `int`.
3. `rotate_right` memaksa `right = 1`, dan `rotate_left` memaksa `left = 1`. Akibatnya
   kemudi ikut membelok saat berputar.
4. Panggil `move(speed)`, `steering(right, left)`, `kick(is_kick)`, dan
   `dribble(is_dribble)`.

---

## 6. Spesifikasi Subsistem

### 6.1 Penggerak (`20_driver.ino`)

**Kecepatan.** `speed` 1–5 dipetakan dengan `map()` ke PWM 120–255:

| speed | 1 | 2 | 3 | 4 | 5 |
|---|---|---|---|---|---|
| PWM target | 120 | 153 | 187 | 221 | 255 |

**Akselerasi halus.** PWM dilewatkan filter low-pass orde-1, dievaluasi sekali per loop:
`pwm = 0.1·target + 0.9·prev`. Karena hasilnya dipotong ke `int`, PWM berhenti naik
sekitar 9–10 di bawah target. Contohnya, target 255 berhenti di 246, dan target 120
berhenti di 111.

**Prioritas arah:** `forward` > `backward` > `rotate_right` > `rotate_left`. Jika tidak
ada yang aktif, motor berhenti dan filter di-reset ke 0. Mode berhenti adalah *coast*
(IN = LOW/LOW), bukan rem aktif.

| Mode | IN1 | IN2 | IN3 | IN4 |
|---|---|---|---|---|
| Maju | H | L | H | L |
| Mundur | L | H | L | H |
| Putar kanan | H | L | L | H |
| Putar kiri | L | H | H | L |
| Stop | L | L | L | L |

`right` dan `left` biasa **tidak** memengaruhi motor. Keduanya hanya menggerakkan servo
kemudi.

### 6.2 Kemudi (`21_steer.ino`)

| Kondisi | Sudut target |
|---|---|
| `right` saja | 180° (`right_angle`) |
| `left` saja | 40° (`left_angle`) |
| Lainnya | 110° (`default_angle`) |

Servo dipasang dengan pulsa 1000–2000 µs pada 100 Hz. Batas atasnya hardcode `2000`,
bukan `max_us` (2500). Sudut juga difilter: `angle = 0.045·target + 0.955·prev`
(`int`). Hasil simulasi filter ini:

- Pada loop pertama `prev_angle = 0`, sehingga servo **melompat dari 110° ke ±4°**, lalu
  naik perlahan.
- Karena pemotongan `int`, sudut yang **naik** berhenti sekitar 22° di bawah target. Dari
  bawah, posisi tengah tertahan di **±88°** (bukan 110°), dan belok kanan tertahan di
  **±158°** (bukan 180°). Sudut yang turun (ke 40°) tercapai.
- Laju filter bergantung pada kecepatan `loop()`, bukan pada waktu.

### 6.3 Kicker (`22_kicker.ino`)

- Saat `kick = 1`, `KICKER` HIGH selama `time_kick` = **1000 ms**, lalu LOW.
- Jika `kick` tetap 1 setelah pulsa selesai, pulsa baru langsung dimulai. Kicker
  praktis menyala terus, jadi pengendali sebaiknya mengirim `kick=1` sekali lalu kembali
  ke `kick=0`.
- Dalam fungsi, `is_kick` adalah parameter lokal yang menutupi variabel global, sehingga
  `is_kick = 0` tidak berdampak apa pun.

### 6.4 Dribbler (`23_dribbler.ino`)

| Kondisi | Servo kiri | Servo kanan |
|---|---|---|
| Posisi awal (setup) | 180° | 0° |
| `dribble = 1` (jepit / `close`) | 0° | 150° |
| `dribble = 0` (lepas / `open`) | 120° | 60° |

Posisi awal hanya bertahan sampai loop pertama, setelah itu langsung ke `open`.

### 6.5 LED Indikator

LED menyala jika ada klien yang terhubung ke AP. Handler HTTP juga menulis ke LED (HIGH
jika request valid, LOW jika tidak), tetapi status itu segera ditimpa oleh `loop()`.

---

## 7. Mode Alternatif: Station (`11_connection.ino`)

File ini berisi versi di mana ESP32 terhubung ke hotspot luar dengan IP statis
`192.168.4.10` (gateway `192.168.4.1`). Protokolnya versi lama: `speed`, `forward`,
`backward`, `turning_angle`, `dribble`, `kick`.

Untuk memakainya, komentar `10_connection.ino` lalu buka komentar `11_connection.ino`.
Kodenya **tidak akan langsung bisa di-compile** karena memakai `angleString`, yang tidak
dideklarasikan di `00_data.ino`, dan formatnya tidak sesuai dengan logika `loop()` saat
ini.

---

## 8. Temuan & Catatan Teknis

Berikut daftar perilaku dari kode saat ini yang perlu diketahui. Belum ada yang
diperbaiki. Setiap temuan punya dokumen task tersendiri di [`task/`](task/README.md),
dengan nomor task sama dengan nomor temuan.

| # | Lokasi | Temuan | Dampak |
|---|---|---|---|
| 1 | `21_steer.ino` | Filter `int` membuat sudut naik tertahan ±22° di bawah target (tengah ±88°, kanan ±158°) | Kemudi tidak lurus/tidak penuh |
| 2 | `00_data.ino` / `21_steer.ino` | `prev_angle` awal 0, padahal servo diset 110° di setup | Servo menyentak di awal |
| 3 | `99_main.ino` | Tidak ada failsafe/timeout perintah | Robot terus jalan jika koneksi putus |
| 4 | `10_connection.ino` | `String` ditulis di task async dan dibaca di `loop()` tanpa proteksi | Potensi race condition |
| 5 | `99_main.ino` | `printReceivedMessage()` dipanggil setiap loop saat ada klien | Serial banjir, loop melambat |
| 6 | `22_kicker.ino` | `kick=1` yang ditahan membuat kicker menyala terus-menerus | Risiko panas pada solenoid/MOSFET |
| 7 | `21_steer.ino` | Batas pulsa servo 2000 µs hardcode, `max_us` tidak dipakai | Inkonsistensi konfigurasi |
| 8 | `20_driver.ino` | Filter PWM berhenti ±9 di bawah target | Kecepatan maksimum sedikit lebih rendah |
| 9 | Umum | Filter kemudi dan PWM bergantung pada laju loop, bukan waktu | Respons berubah jika beban loop berubah |
| 10 | `00_data.ino` | `serverName`, `prevTime`, dan `PROG_NAME` tidak dipakai; `pinMode(KICKER)` dipanggil dua kali | Kode mati |
| 11 | `11_connection.ino` | Mereferensikan `angleString` yang tidak ada | Gagal compile jika diaktifkan |
| 12 | Hardware | 3 servo pada regulator linear 78xx | Potensi brown-out |
