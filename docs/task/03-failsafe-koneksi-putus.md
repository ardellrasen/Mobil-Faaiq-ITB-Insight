# Task 03 — Failsafe Saat Koneksi Putus

| | |
|---|---|
| **Prioritas** | Tinggi (keselamatan) |
| **Area** | Komunikasi / Keselamatan |
| **Lokasi** | `10_connection.ino:14-35`, `99_main.ino:40-48` |
| **Terkait** | [04](04-race-condition-data-perintah.md), [06](06-kicker-menyala-terus.md) |

## Masalah

Perintah disimpan di variabel global dan terus dijalankan `loop()` sampai ada request
baru. Tidak ada pencatatan waktu perintah terakhir dan tidak ada timeout. Bahkan saat
tidak ada klien yang terhubung ke AP, `loop()` tetap menjalankan perintah terakhir.

## Dampak

Jika pengendali keluar jangkauan, aplikasi crash, atau Wi-Fi terputus saat robot sedang
maju, robot **terus melaju** dengan motor dan kicker dalam kondisi terakhir.

## Usulan Solusi

1. Catat waktu request valid terakhir di handler HTTP:

   ```cpp
   // 00_data.ino
   volatile unsigned long last_cmd_ms = 0;
   const unsigned long CMD_TIMEOUT_MS = 500;

   // 10_connection.ino, di dalam blok request valid
   last_cmd_ms = millis();
   ```

2. Di `loop()`, setelah membaca perintah, nolkan semua aksi jika timeout terlewati atau
   tidak ada klien:

   ```cpp
   if (millis() - last_cmd_ms > CMD_TIMEOUT_MS || !WiFi.softAPgetStationNum()) {
     forward = backward = right = left = 0;
     rotate_right = rotate_left = 0;
     is_kick = 0;
     // is_dribble: tentukan apakah dribbler dilepas atau tetap menjepit
   }
   ```

3. **Pengendali wajib mengirim perintah secara periodik** (heartbeat), misalnya setiap
   100–200 ms, walaupun tidak ada perubahan input. Ini perubahan di sisi aplikasi
   pengendali.

## Keputusan yang Dibutuhkan

- Nilai timeout (usulan 500 ms).
- Perilaku dribbler saat failsafe: lepas atau tahan bola.
- Apakah aplikasi pengendali sudah mengirim perintah secara periodik?

## Kriteria Selesai

- [ ] Robot berhenti ≤ timeout setelah pengendali berhenti mengirim request.
- [ ] Robot berhenti saat klien memutus Wi-Fi.
- [ ] Kicker tidak aktif dalam kondisi failsafe.
- [ ] Operasi normal (dengan heartbeat) tidak tersendat.
- [ ] Compile tanpa error.
