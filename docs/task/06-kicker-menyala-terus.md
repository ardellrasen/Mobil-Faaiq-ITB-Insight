# Task 06 — Kicker Menyala Terus Jika `kick` Ditahan

| | |
|---|---|
| **Prioritas** | Tinggi (keselamatan hardware) |
| **Area** | Kicker |
| **Lokasi** | `22_kicker.ino:7-23`, `00_data.ino:48-52` |
| **Terkait** | [03](03-failsafe-koneksi-putus.md) |

## Masalah

`kick()` bersifat *level-triggered*: selama `kick = 1`, pulsa 1000 ms terus dimulai
ulang. Setelah pulsa selesai (`start_time_kick = 0`), iterasi berikutnya melihat
`is_kick = 1` lagi dan langsung memulai pulsa baru. Hasilnya, `KICKER` hanya LOW selama
satu iterasi loop di antara pulsa, sehingga praktis menyala terus.

Masalah lain di fungsi yang sama:

- `is_kick` di dalam fungsi adalah **parameter lokal** yang menutupi variabel global, jadi
  `is_kick = 0;` tidak berefek apa pun.
- `start_time_kick` bertipe `int`, padahal `millis()` bertipe `unsigned long`. Tipe ini
  aman sampai ±24 hari, tetapi sebaiknya diperbaiki.
- Nilai `0` dipakai sebagai penanda "tidak aktif". Jika `millis()` kebetulan bernilai 0,
  logika bisa salah (kasus tepi).
- Tidak ada jeda pendinginan (cooldown) antar tendangan.

## Dampak

Solenoid/aktuator dan MOSFET XY-MOS dialiri arus terus-menerus, sehingga berisiko panas,
merusak komponen, dan menguras baterai. Robot tidak bisa membedakan "sekali tendang"
dari "tahan tombol".

## Usulan Solusi

Ubah menjadi *edge-triggered* (hanya menendang saat `kick` berubah 0 → 1), dengan durasi
pulsa dan cooldown:

```cpp
// 00_data.ino
const unsigned long KICK_PULSE_MS    = 1000;  // tinjau ulang, mungkin cukup 50–200 ms
const unsigned long KICK_COOLDOWN_MS = 1000;
bool kicking = false;
int  prev_kick_cmd = 0;
unsigned long kick_start_ms = 0;
unsigned long kick_end_ms = 0;

// 22_kicker.ino
void kick(int kick_cmd) {
  unsigned long now = millis();
  bool rising = kick_cmd && !prev_kick_cmd;
  prev_kick_cmd = kick_cmd;

  if (!kicking && rising && now - kick_end_ms >= KICK_COOLDOWN_MS) {
    kicking = true;
    kick_start_ms = now;
    digitalWrite(KICKER, HIGH);
  }
  if (kicking && now - kick_start_ms >= KICK_PULSE_MS) {
    kicking = false;
    kick_end_ms = now;
    digitalWrite(KICKER, LOW);
  }
}
```

Catatan: edge dihitung dari nilai `kick` yang dibaca `loop()`. Pengendali perlu mengirim
`kick=0` sebelum tendangan berikutnya. Jika Task 03 (heartbeat) diterapkan, pengendali
cukup mengirim `kick=1` selama tombol ditekan, dan robot hanya menendang sekali.

## Keputusan yang Dibutuhkan

- Durasi pulsa yang aman untuk aktuator kicker (1000 ms terasa lama untuk solenoid).
- Durasi cooldown.

## Kriteria Selesai

- [ ] `kick=1` yang ditahan hanya menghasilkan **satu** pulsa.
- [ ] Tendangan berikutnya baru terjadi setelah `kick` kembali ke 0 dan cooldown lewat.
- [ ] Diukur di pin 27 (LED/multimeter/osiloskop): lebar pulsa sesuai `KICK_PULSE_MS`.
- [ ] Compile tanpa error.
