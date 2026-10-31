# Task 02 — Servo Kemudi Menyentak Saat Start

| | |
|---|---|
| **Prioritas** | Sedang |
| **Area** | Kemudi |
| **Lokasi** | `00_data.ino:45-46`, `21_steer.ino:4` |
| **Terkait** | [01](01-filter-kemudi-tidak-mencapai-target.md) |

## Masalah

`setupSteer()` menulis servo ke `default_angle` (110°), tetapi state filter dimulai dari 0:

```cpp
int angle = 0;
int prev_angle = angle;   // = 0
```

Pada pemanggilan `steering()` pertama di `loop()`:
`0.045 × 110 + 0.955 × 0 = 4.95 → 4`.

## Dampak

- Servo langsung melompat dari 110° ke ±4°, lalu merangkak naik. Ini membebani servo dan
  mekanik kemudi.
- Sudut 4° berada di luar rentang kerja kemudi (40°–180°), sehingga berisiko membentur
  batas mekanis.
- Lonjakan arus servo saat start memperparah risiko brown-out
  ([12](12-risiko-brown-out-catu-daya.md)).

## Usulan Solusi

Inisialisasi state filter ke posisi awal servo:

```cpp
int angle = default_angle;
int prev_angle = default_angle;
```

Jika Task 01 dikerjakan dengan `float steer_filtered = default_angle;`, task ini ikut
selesai.

## Kriteria Selesai

- [ ] Saat power-on, servo kemudi diam di 110° tanpa gerakan ke sudut kecil.
- [ ] Compile tanpa error.
- [ ] Diamati di robot saat menyala (dan setelah reset).
