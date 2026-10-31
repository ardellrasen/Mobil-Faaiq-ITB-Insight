# Task 05 — Log Serial Membanjiri Loop

| | |
|---|---|
| **Prioritas** | Rendah |
| **Area** | Debug / Performa |
| **Lokasi** | `99_main.ino:33-35`, `99_main.ino:61-77`, `00_data.ino:61` |
| **Terkait** | [09](09-filter-bergantung-laju-loop.md), [10](10-bersihkan-kode-mati.md) |

## Masalah

Selama ada klien yang terhubung, `printReceivedMessage()` dipanggil **di setiap
iterasi** `loop()` tanpa jeda. Setiap panggilan mencetak ±80 karakter. Pada 115200 baud
(±11,5 KB/s), buffer TX serial cepat penuh dan `Serial.print` mulai **memblokir**.

## Dampak

- Laju `loop()` turun drastis dan tidak stabil saat klien terhubung dibanding saat tidak
  ada klien. Karena filter kemudi dan PWM bergantung pada laju loop
  ([09](09-filter-bergantung-laju-loop.md)), respons robot ikut berubah.
- Serial monitor sulit dibaca.

## Usulan Solusi

Batasi frekuensi log dengan `millis()`. Variabel `prevTime` yang sudah ada bisa dipakai,
tetapi tipenya diganti menjadi `unsigned long`:

```cpp
// 00_data.ino
unsigned long prevTime = 0;
const unsigned long LOG_INTERVAL_MS = 200;

// 99_main.ino
if (WiFi.softAPgetStationNum()) {
  digitalWrite(LED_BUILTIN, HIGH);
  if (millis() - prevTime >= LOG_INTERVAL_MS) {
    prevTime = millis();
    printReceivedMessage();
  }
}
```

Opsional: tambahkan flag `#define DEBUG_LOG 1` supaya log bisa dimatikan total saat
pertandingan.

Opsional: tambahkan `rotate_right` dan `rotate_left` ke isi log.

## Kriteria Selesai

- [ ] Log tercetak paling sering sekali per `LOG_INTERVAL_MS`.
- [ ] Laju loop (diukur dengan counter per detik) tidak berbeda jauh antara ada dan tidak
      ada klien.
- [ ] Compile tanpa error.
