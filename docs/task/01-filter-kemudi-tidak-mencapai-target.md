# Task 01 — Filter Kemudi Tidak Mencapai Sudut Target

| | |
|---|---|
| **Prioritas** | Tinggi |
| **Area** | Kemudi |
| **Lokasi** | `21_steer.ino:12`, `00_data.ino:45-46` |
| **Terkait** | [02](02-servo-kemudi-menyentak-saat-start.md), [09](09-filter-bergantung-laju-loop.md) |

## Masalah

Sudut kemudi difilter dengan low-pass orde-1 memakai variabel `int`:

```cpp
angle = 0.045*angle + 0.955*prev_angle;   // angle bertipe int
```

Hasil perkalian float dipotong (truncate) ke `int` di setiap iterasi. Jika selisih
target dan sudut sekarang di bawah ±22° (`0.045 × 22 ≈ 1`), kenaikan per langkah menjadi
< 1 dan hilang terpotong. Akibatnya filter berhenti sebelum target.

## Bukti (simulasi filter)

| Dari → Target | Berhenti di |
|---|---|
| 0 → 110 (tengah, saat start) | **88** |
| 110 → 180 (belok kanan) | **158** |
| 110 → 40 (belok kiri) | 39 (tercapai) |

Arah turun tercapai karena pemotongan ke bawah justru membantu.

## Dampak

- Posisi "lurus" robot sekitar 88°, bukan 110°, jadi robot berjalan menyerong.
- Belok kanan kurang ±22° dibanding belok kiri, sehingga kemudi asimetris.
- Posisi tengah berbeda tergantung arah datangnya (histeresis).

## Usulan Solusi

Simpan state filter dalam `float` dan bulatkan hanya saat menulis ke servo:

```cpp
// 00_data.ino
float steer_filtered = default_angle;

// 21_steer.ino
void steering(int right, int left) {
  int target;
  if (right && !left)      target = right_angle;
  else if (left && !right) target = left_angle;
  else                     target = default_angle;

  steer_filtered = 0.045f * target + 0.955f * steer_filtered;
  angle = (int)lroundf(steer_filtered);
  steer.write(angle);
}
```

Variabel `prev_angle` tidak dibutuhkan lagi.

## Kriteria Selesai

- [ ] Dari kondisi diam, sudut servo konvergen ke 110° (±1°).
- [ ] Saat `right=1`, sudut mencapai 180° (±1°); saat `left=1`, mencapai 40° (±1°).
- [ ] Kembali ke 110° (±1°) dari kanan maupun dari kiri.
- [ ] Compile tanpa error.
- [ ] Diuji di robot: roda lurus saat tidak ada perintah belok.
