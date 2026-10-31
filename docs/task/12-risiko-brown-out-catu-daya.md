# Task 12 — Risiko Brown-out pada Catu Daya Servo

| | |
|---|---|
| **Prioritas** | Sedang |
| **Area** | Hardware |
| **Lokasi** | `Schematic Mobil Faaiq.jpg` (IC1, SV1, SV2) |
| **Terkait** | [02](02-servo-kemudi-menyentak-saat-start.md) |

## Masalah

Menurut skema, rel 5 V berasal dari satu regulator linear 78xx (IC1, ditandai "786TV",
kemungkinan 7805) dengan input 12 V. Rel ini dipakai oleh:

- 3 servo (kemudi, dribbler kiri, dribbler kanan);
- kemungkinan ESP32 juga (via header SV1). Ini perlu dicek di wiring sebenarnya.

Perkiraan beban:

- Satu servo standar dapat menarik 0,5–1 A saat bergerak dan lebih besar saat stall.
- Regulator 7805 umumnya dibatasi ±1 A (1,5 A puncak).
- Disipasi daya: (12 − 5) V × 1 A = **7 W**, terlalu besar tanpa heatsink yang memadai.
  Regulator akan masuk thermal shutdown.

## Dampak

- Tegangan 5 V turun saat servo bergerak bersamaan (misalnya saat start, atau saat
  dribbler menjepit sambil membelok), sehingga ESP32 bisa **reset (brown-out)** atau
  Wi-Fi terputus.
- Regulator panas dan bisa shutdown di tengah pertandingan.

## Langkah Verifikasi

1. Pastikan dari wiring sebenarnya apakah ESP32 diberi daya dari rel 5 V yang sama.
2. Ukur tegangan rel 5 V dengan multimeter/osiloskop saat ketiga servo bergerak bersamaan.
3. Pantau log serial: cari pesan `Brownout detector was triggered` atau reset yang tidak
   terduga.
4. Rasakan suhu IC1 setelah ±5 menit operasi.

## Usulan Solusi

- Ganti 7805 dengan **buck converter 5 V ≥ 3 A** (misalnya modul MP1584 atau XL4015, atau
  modul UBEC servo).
- Pisahkan catu ESP32 dari catu servo, atau minimal tambahkan kapasitor bulk
  (470–1000 µF) di rel servo.
- Kerjakan Task 02 untuk mengurangi lonjakan arus saat start.

## Kriteria Selesai

- [ ] Tegangan rel 5 V tetap ≥ 4,75 V saat ketiga servo bergerak bersamaan.
- [ ] Tidak ada reset/brown-out selama uji operasi ±10 menit.
- [ ] Suhu regulator/converter dalam batas aman.
- [ ] Skema diperbarui sesuai perubahan hardware.
