# Daftar Task

Setiap task berasal dari satu temuan di [`../SPESIFIKASI.md`](../SPESIFIKASI.md) §8.

| # | Task | Prioritas | Area | Status |
|---|---|---|---|---|
| 01 | [Filter kemudi tidak mencapai target](01-filter-kemudi-tidak-mencapai-target.md) | Tinggi | Kemudi | Open |
| 02 | [Servo kemudi menyentak saat start](02-servo-kemudi-menyentak-saat-start.md) | Sedang | Kemudi | Open |
| 03 | [Failsafe saat koneksi putus](03-failsafe-koneksi-putus.md) | Tinggi | Komunikasi / Keselamatan | Open |
| 04 | [Race condition data perintah](04-race-condition-data-perintah.md) | Sedang | Komunikasi | Open |
| 05 | [Log serial membanjiri loop](05-log-serial-membanjiri-loop.md) | Rendah | Debug | Open |
| 06 | [Kicker menyala terus jika `kick` ditahan](06-kicker-menyala-terus.md) | Tinggi | Kicker / Keselamatan | Open |
| 07 | [Batas pulsa servo kemudi hardcode](07-batas-pulsa-servo-hardcode.md) | Rendah | Kemudi | Open |
| 08 | [Filter PWM tidak mencapai target](08-filter-pwm-tidak-mencapai-target.md) | Rendah | Penggerak | Open |
| 09 | [Filter bergantung laju loop](09-filter-bergantung-laju-loop.md) | Sedang | Kemudi / Penggerak | Open |
| 10 | [Bersihkan kode mati](10-bersihkan-kode-mati.md) | Rendah | Umum | Open |
| 11 | [Mode Station gagal compile](11-mode-station-gagal-compile.md) | Rendah | Komunikasi | Open |
| 12 | [Risiko brown-out catu daya servo](12-risiko-brown-out-catu-daya.md) | Sedang | Hardware | Open |

## Urutan Pengerjaan yang Disarankan

1. **Keselamatan:** kerjakan 03 dan 06 lebih dulu.
2. **Kemudi:** kerjakan 01 dan 02 sekaligus, karena perbaikannya sama-sama di `21_steer.ino`
   dan 02 ikut selesai jika 01 memakai nilai awal yang benar. Setelah itu kerjakan 07.
3. **Filter:** kerjakan 08 lalu 09. Task 09 sebaiknya dikerjakan setelah 01 dan 08.
4. **Robustness:** kerjakan 04, lalu 05.
5. **Kebersihan:** kerjakan 10 dan 11.
6. **Hardware:** kerjakan 12. Task ini paralel dengan task lain dan butuh pengukuran fisik.

## Konvensi

- Setelah setiap perubahan, jalankan `arduino-cli compile --fqbn esp32:esp32:esp32 .` dan
  pastikan tidak ada error.
- Ubah kolom Status menjadi `Done` beserta hash commit-nya.
