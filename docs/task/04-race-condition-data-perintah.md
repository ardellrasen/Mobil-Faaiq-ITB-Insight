# Task 04 — Race Condition pada Data Perintah

| | |
|---|---|
| **Prioritas** | Sedang |
| **Area** | Komunikasi |
| **Lokasi** | `10_connection.ino:17-25`, `99_main.ino:40-48`, `00_data.ino:8-16` |
| **Terkait** | [03](03-failsafe-koneksi-putus.md) |

## Masalah

Handler `ESPAsyncWebServer` berjalan di task `async_tcp`, terpisah dari task `loop()`.
Handler menulis 9 objek `String` global, sementara `loop()` membacanya dengan `toInt()`.
Tidak ada mutex atau critical section di antara keduanya.

`String` bukan tipe atomik. Penulisan `String` bisa melibatkan realokasi heap, dan
pembacaan yang terjadi di tengah realokasi bisa membaca memori yang sudah dibebaskan.
Selain itu, 9 nilai bisa terbaca sebagian lama dan sebagian baru, misalnya `forward`
sudah baru tetapi `backward` masih lama.

## Dampak

- Kemungkinan kecil tetapi nyata: crash/reset acak (use-after-free pada buffer `String`).
- Kombinasi perintah tidak konsisten selama satu iterasi loop.
- Fragmentasi heap karena alokasi `String` berulang.

## Usulan Solusi

Parse nilai ke `int` di handler, simpan dalam satu struct, dan salin struct secara
atomik dengan critical section:

```cpp
// 00_data.ino
struct Command {
  int speed, forward, backward, right, left;
  int rotate_right, rotate_left, dribble, kick;
};
Command cmd_shared = {1, 0, 0, 0, 0, 0, 0, 0, 0};
portMUX_TYPE cmd_mux = portMUX_INITIALIZER_UNLOCKED;

// 10_connection.ino (handler)
Command c;
c.speed = request->getParam("speed")->value().toInt();
// ... parameter lain ...
portENTER_CRITICAL(&cmd_mux);
cmd_shared = c;
portEXIT_CRITICAL(&cmd_mux);

// 99_main.ino (loop)
Command c;
portENTER_CRITICAL(&cmd_mux);
c = cmd_shared;
portEXIT_CRITICAL(&cmd_mux);
speed = c.speed; forward = c.forward; // ...
```

Setelah ini, variabel `*String` global bisa dihapus. Response teks di handler cukup
dibangun dari variabel lokal. Jika Task 03 dikerjakan, `last_cmd_ms` bisa ditaruh di
dalam struct yang sama.

## Kriteria Selesai

- [ ] Tidak ada `String` global yang dibagi antara handler dan `loop()`.
- [ ] Akses data perintah bersama dilindungi critical section.
- [ ] Uji spam request (misalnya loop `curl` cepat selama beberapa menit) tanpa reset.
- [ ] Compile tanpa error.
