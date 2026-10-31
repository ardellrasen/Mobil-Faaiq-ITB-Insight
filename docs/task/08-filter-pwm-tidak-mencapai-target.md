# Task 08 — Filter PWM Tidak Mencapai Target

| | |
|---|---|
| **Prioritas** | Rendah |
| **Area** | Penggerak |
| **Lokasi** | `20_driver.ino:14-17`, `00_data.ino:27` |
| **Terkait** | [01](01-filter-kemudi-tidak-mencapai-target.md), [09](09-filter-bergantung-laju-loop.md) |

## Masalah

Masalahnya sama dengan Task 01, tetapi pada PWM motor:

```cpp
int pwm = map(speed, 1, 5, 120, 255);
pwm = (pwm*0.1) + (prev_pwm*0.9);   // dipotong ke int
```

Kenaikan per langkah hilang terpotong ketika selisih ke target < 10.

## Bukti (simulasi filter)

| speed | Target PWM | Berhenti di |
|---|---|---|
| 1 | 120 | 111 |
| 5 | 255 | 246 |

## Dampak

- Kecepatan maksimum ±3,5% di bawah 255.
- Pada speed 1, PWM efektif 111 mungkin berada di bawah ambang mulai motor. Ini perlu
  dicek, karena nilai minimum 120 kemungkinan dipilih berdasarkan ambang tersebut.

Masalah tambahan: `speed` di luar 1–5 (misalnya 0 atau 9) tidak dibatasi, sehingga `map()`
bisa menghasilkan PWM < 120 atau > 255.

## Usulan Solusi

```cpp
// 00_data.ino
float pwm_filtered = 0;

// 20_driver.ino
void move(int speed) {
  speed = constrain(speed, 1, 5);
  int target = map(speed, 1, 5, 120, 255);
  pwm_filtered = 0.1f * target + 0.9f * pwm_filtered;
  int pwm = (int)lroundf(pwm_filtered);
  // ... cabang arah memakai pwm ...
  // di cabang stop: pwm_filtered = 0;
}
```

## Kriteria Selesai

- [ ] PWM konvergen ke target (±1) untuk speed 1–5.
- [ ] `speed` di luar rentang dibatasi ke 1–5.
- [ ] Motor tetap berputar di speed 1.
- [ ] Compile tanpa error.
