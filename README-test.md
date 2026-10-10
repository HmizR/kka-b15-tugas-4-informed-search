# Implementasi Experiment & Comparison

Bagian ini menyediakan `src/main.cpp` dan lima grid dalam `testcases/`.
Header dan implementasi Grid, heuristic, A*, serta GBFS tidak diubah.

## Alur program

1. Membaca kelima testcase melalui `Grid::loadFromFile()`.
2. Menjalankan algoritma sekali sebagai pemanasan sekaligus mengambil path dan metrik.
3. Menjalankan A* dan GBFS masing-masing 1.000 kali per testcase.
4. Mengukur hanya pemanggilan algoritma dengan `std::chrono::steady_clock`.
5. Menampilkan metrik dan menyimpan sepuluh baris hasil ke `results/comparison.csv`.

Waktu rata-rata = jumlah durasi pemanggilan / jumlah pengulangan. Loading,
output, pemeriksaan hasil, dan penghancuran hasil setelah stop timer berada
di luar interval pengukuran. Kedua algoritma menerima objek Grid yang sama.

## Menjalankan setelah integrasi

File `src/grid.cpp`, `src/heuristic.cpp`, `src/astar.cpp`, dan `src/gbfs.cpp`
masih kosong saat bagian ini dibuat. Implementasi anggota 1–3 diperlukan
sebelum program dapat ditautkan dan menghasilkan pengukuran aktual.

Dari direktori utama proyek, gunakan PowerShell:

```powershell
g++ -std=c++11 -O2 -Wall -Wextra -I include src/main.cpp src/grid.cpp src/heuristic.cpp src/astar.cpp src/gbfs.cpp -o comparison.exe
.\comparison.exe
```

Jumlah pengulangan dapat diatur, dengan nilai yang sama untuk kedua algoritma:

```powershell
.\comparison.exe 5000
```

Folder `results/` harus tersedia. Setiap eksekusi mengganti CSV sebelumnya.
Jangan membandingkan timing dari build dengan opsi optimasi berbeda.

## Testcase

| Kategori | Ukuran | Tujuan | Biaya terpendek (BFS validasi) |
| --- | --- | --- | --- |
| Simple | 5 × 8 | Sedikit obstacle, jalur langsung | 11 |
| Moderate | 9 × 15 | Beberapa pilihan jalur dengan obstacle | 22 |
| Misleading | 15 × 21 | Jalur mendekati goal dapat membawa detour | 22 |
| Dense | 15 × 21 | Koridor sempit yang berkelok | 174 |
| Large | 41 × 61 | Grid besar dengan dinding berselang | 484 |

Seluruh grid telah diperiksa memiliki satu S, satu G, panjang baris seragam,
karakter hanya `S G # .`, serta solusi dengan gerakan empat arah.
BFS digunakan hanya untuk validasi testcase, bukan sebagai implementasi
algoritma anggota lain atau sumber hasil CSV.

Misleading dipilih dengan bantuan simulasi greedy Manhattan. Hasil GBFS
kelompok tetap harus diukur karena tie-breaking dapat memengaruhi jalurnya.
GBFS tidak wajib optimal dan tidak wajib memiliki biaya lebih tinggi di setiap kasus.

## Membaca hasil

- `Found`: apakah goal ditemukan.
- `PathCost`: biaya jalur dari implementasi algoritma.
- `ExpandedNodes`: jumlah node yang diekspansi menurut metrik kelompok.
- `MaxFrontierSize`: ukuran frontier maksimum menurut metrik kelompok.
- `AverageTimeUs`: rata-rata waktu pemanggilan dalam mikrodetik.

Bandingkan kualitas jalur, node yang diekspansi, frontier maksimum, dan waktu.
Frontier maksimum merupakan indikator kebutuhan memori, bukan pengukuran byte.
Waktu kecil dapat berubah antareksekusi; jangan menyimpulkan dari selisih kecil
atau mengubah algoritma demi memperoleh hasil tertentu.

CSV belum dibuat karena algoritma belum tersedia. Tidak ada angka performa
yang direkayasa atau diambil dari simulasi validasi.
