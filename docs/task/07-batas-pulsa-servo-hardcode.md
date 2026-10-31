# Task 07 — Batas Pulsa Servo Kemudi Hardcode

| | |
|---|---|
| **Prioritas** | Rendah |
| **Area** | Kemudi |
| **Lokasi** | `21_steer.ino:2`, `00_data.ino:40-41` |
| **Terkait** | [01](01-filter-kemudi-tidak-mencapai-target.md) |

## Masalah

```cpp
steer.attach(STEER, min_us, 2000);   // max hardcode 2000, bukan max_us (2500)
```

`min_us` dan `max_us` dideklarasikan sebagai konfigurasi bersama, tetapi servo kemudi
memakai `2000` langsung. Servo dribbler tetap memakai `max_us = 2500`. Tidak jelas apakah
nilai 2000 disengaja, misalnya untuk membatasi sudut mekanis kemudi, atau sisa
eksperimen.

## Dampak

- Konfigurasi menyesatkan: mengubah `max_us` tidak berpengaruh pada kemudi.
- Dengan rentang 1000–2000 µs, sudut `write(0..180)` dipetakan ke ±90° fisik untuk servo
  standar. Kalibrasi `left_angle`, `default_angle`, dan `right_angle` bergantung pada
  nilai ini.

## Usulan Solusi

Buat konstanta terpisah dengan nama yang jelas, dan beri komentar alasannya:

```cpp
// 00_data.ino — Steer
const int steer_min_us = 1000;
const int steer_max_us = 2000;   // dibatasi agar tidak membentur batas mekanis kemudi

// 21_steer.ino
steer.attach(STEER, steer_min_us, steer_max_us);
```

Jika ternyata 2500 yang benar, kalibrasi ulang `left_angle`, `default_angle`, dan
`right_angle` di robot.

## Keputusan yang Dibutuhkan

- Apakah 2000 µs disengaja? Konfirmasi ke pembuat kode atau dengan uji mekanis.

## Kriteria Selesai

- [ ] Tidak ada angka pulsa hardcode di `21_steer.ino`.
- [ ] Alasan nilai batas terdokumentasi di komentar.
- [ ] Sudut kemudi kiri/tengah/kanan diverifikasi di robot.
- [ ] Compile tanpa error.
