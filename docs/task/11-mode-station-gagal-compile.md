# Task 11 — Mode Station Gagal Compile Jika Diaktifkan

| | |
|---|---|
| **Prioritas** | Rendah |
| **Area** | Komunikasi |
| **Lokasi** | `11_connection.ino` (seluruh file) |
| **Terkait** | [10](10-bersihkan-kode-mati.md), [04](04-race-condition-data-perintah.md) |

## Masalah

`11_connection.ino` berisi mode alternatif (ESP32 terhubung ke hotspot luar dengan IP
statis `192.168.4.10`). Seluruh isinya dikomentari. Jika komentarnya dibuka:

1. Muncul error compile karena `angleString` tidak dideklarasikan di `00_data.ino`.
2. Muncul error *redefinition* `setupWifi()` dan `getMessageHttp()` jika
   `10_connection.ino` tidak dikomentari bersamaan.
3. Protokolnya versi lama (`turning_angle`, tanpa `right`, `left`, `rotate_*`), sehingga
   tidak cocok dengan `loop()` saat ini. Robot tidak akan bisa belok atau berputar.
4. `while (WiFi.status() != WL_CONNECTED)` memblokir `setup()` tanpa batas jika hotspot
   tidak ada.

## Dampak

Mode Station praktis tidak bisa dipakai. Beralih mode membutuhkan edit manual di dua
file.

## Usulan Solusi

Pilih salah satu.

**A. Hapus `11_connection.ino`**, jika mode Station memang tidak dipakai lagi. Riwayatnya
tetap ada di git.

**B. Satukan dengan saklar compile-time:**

```cpp
// player5.ino
#define WIFI_MODE_AP 1   // 1 = Access Point, 0 = Station

// 10_connection.ino
void setupWifi() {
#if WIFI_MODE_AP
  WiFi.softAP(ssid, password);
#else
  WiFi.config(local_IP, gateway, subnet);
  WiFi.begin(ssid, password);
  // tunggu dengan timeout, jangan blokir selamanya
#endif
}
// getMessageHttp() dipakai bersama oleh kedua mode, dengan protokol yang sama
```

Pada opsi B, logika LED dan failsafe ([03](03-failsafe-koneksi-putus.md)) juga perlu
disesuaikan, karena `softAPgetStationNum()` tidak berlaku di mode Station.

## Keputusan yang Dibutuhkan

- Apakah mode Station masih dibutuhkan, misalnya untuk dikendalikan dari satu hotspot
  bersama beberapa robot?

## Kriteria Selesai

- [ ] (A) File dihapus dan referensinya di dokumen diperbarui, **atau**
- [ ] (B) Kedua mode bisa di-compile dengan mengganti satu `#define`, memakai protokol
      yang sama, dan mode Station tidak memblokir selamanya.
- [ ] Compile tanpa error untuk mode yang aktif.
