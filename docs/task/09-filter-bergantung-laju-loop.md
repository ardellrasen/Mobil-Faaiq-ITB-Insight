# Task 09 — Filter Bergantung pada Laju Loop

| | |
|---|---|
| **Prioritas** | Sedang |
| **Area** | Kemudi / Penggerak |
| **Lokasi** | `21_steer.ino:12`, `20_driver.ino:16`, `99_main.ino:21` |
| **Terkait** | [01](01-filter-kemudi-tidak-mencapai-target.md), [05](05-log-serial-membanjiri-loop.md), [08](08-filter-pwm-tidak-mencapai-target.md) |
| **Prasyarat** | Sebaiknya dikerjakan setelah 01 dan 08 |

## Masalah

Filter kemudi (α = 0,045) dan filter PWM (α = 0,1) diperbarui **sekali per iterasi
`loop()`**. `loop()` tidak punya delay, sehingga lajunya bergantung pada:

- ada atau tidaknya klien yang terhubung, yang memicu log serial
  ([05](05-log-serial-membanjiri-loop.md));
- beban Wi-Fi dan request HTTP;
- perubahan kode di masa depan.

Konstanta waktu filter dalam satuan detik jadi tidak tetap. Di loop yang sangat cepat,
filter hampir tidak berefek. Di loop yang lambat, respons menjadi lamban.

## Dampak

- Karakter akselerasi dan kemudi berubah-ubah dan sulit di-tuning.
- Hasil tuning di meja (tanpa klien) berbeda dengan di lapangan (dengan klien).

## Usulan Solusi

Pilih salah satu.

**A. Update dengan periode tetap (paling sederhana):**

```cpp
const unsigned long CONTROL_PERIOD_MS = 10;   // 100 Hz
unsigned long last_control_ms = 0;

void loop() {
  // ... baca perintah ...
  if (millis() - last_control_ms >= CONTROL_PERIOD_MS) {
    last_control_ms += CONTROL_PERIOD_MS;
    move(speed);
    steering(right, left);
  }
  kick(is_kick);        // timing kicker sudah berbasis millis
  dribble(is_dribble);
}
```

Setelah itu, nilai α ditentukan ulang untuk periode 10 ms.

**B. α dihitung dari dt:** `alpha = 1 - expf(-dt / tau)` dengan `tau` dalam detik. Opsi
ini lebih fleksibel tetapi lebih rumit.

## Keputusan yang Dibutuhkan

- Waktu respons yang diinginkan untuk kemudi dan akselerasi (misalnya 63% dalam 200 ms).

## Kriteria Selesai

- [ ] Filter kemudi dan PWM diperbarui dengan periode tetap atau berbasis dt.
- [ ] Waktu respons (dari perintah sampai ±63% target) sama, baik dengan maupun tanpa
      klien/log serial.
- [ ] Konstanta tuning terdokumentasi dalam satuan waktu.
- [ ] Compile tanpa error.
