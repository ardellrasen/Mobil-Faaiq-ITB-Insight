# Task 10 — Bersihkan Kode Mati

| | |
|---|---|
| **Prioritas** | Rendah |
| **Area** | Umum |
| **Lokasi** | `00_data.ino`, `99_main.ino`, `player5.ino`, `22_kicker.ino` |
| **Terkait** | [05](05-log-serial-membanjiri-loop.md), [11](11-mode-station-gagal-compile.md) |

## Masalah

Ada beberapa sisa kode yang tidak dipakai atau duplikat:

| Item | Lokasi | Keterangan |
|---|---|---|
| `serverName` | `00_data.ino:6` | Tidak pernah dipakai |
| `prevTime` | `00_data.ino:61` | Hanya dipakai di kode yang dikomentari. Bisa dipakai ulang di Task 05 |
| `PROG_NAME` | `player5.ino` | Tidak pernah dipakai |
| `enum {STOP, MOVE}` | `00_data.ino:19` | Tidak pernah dipakai |
| `pinMode(KICKER, OUTPUT)` | `99_main.ino:17` | Duplikat dengan `setupKicker()` |
| Blok `if (WiFi.status() ...)` dikomentari | `99_main.ino:22-31` | Sisa mode Station |
| `// #include <AsyncEspFsWebServer.h>` | `player5.ino` | Include yang dikomentari |
| `// #include <TFScope22.h>` | `player5.ino` | Library tidak tersedia dan tidak dipakai |
| Komentar "(Uncomment all to use)" | `10_connection.ino:2` | Menyesatkan, karena file ini sudah aktif |

## Dampak

Pembaca kode jadi bingung dan tidak tahu variabel mana yang benar-benar berpengaruh.
Tidak ada dampak fungsional.

## Usulan Solusi

- Hapus item yang tidak dipakai. Untuk `prevTime`, koordinasikan dengan Task 05.
- Hapus `pinMode(KICKER, OUTPUT)` di `setup()`.
- Hapus blok kode yang dikomentari. Riwayatnya tetap tersimpan di git.
- Perbaiki komentar di `10_connection.ino`.
- Jika `PROG_NAME` ingin dipertahankan, cetak di `setup()` sebagai banner.

## Kriteria Selesai

- [ ] Tidak ada variabel/konstanta global yang tidak terpakai.
- [ ] Tidak ada inisialisasi pin ganda.
- [ ] Compile tanpa error dan tanpa warning baru.
- [ ] Perilaku robot tidak berubah (uji singkat semua fungsi).
