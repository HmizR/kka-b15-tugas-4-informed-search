# Perbandingan A* dan Greedy Best-First Search

| Nama | NRP |
| --- | --- |
| Ahmad Fateh Eydil Haq | 5025251234 |
| Irsyad Ansyari Hakim | 5025251242 |
| Hamizan Rifqi Afandi | 5025251246 |
| Darryl Ardan Wicaksono | 5025251253 |

Proyek ini membandingkan A* dan Greedy Best-First Search (GBFS) untuk mencari jalur dari start ke goal pada grid dengan obstacle. Eksperimen menilai kualitas jalur, jumlah node yang diekspansi, ukuran frontier maksimum, dan waktu eksekusi.

Pada hasil yang tersimpan, GBFS lebih cepat pada seluruh testcase. A* menghasilkan jalur optimal pada seluruh testcase, sedangkan GBFS menghasilkan jalur lebih panjang pada Misleading dan Large. Hasil ini menunjukkan pertukaran antara kecepatan pencarian dan kualitas solusi.

## 1. Aturan pencarian dan cara kerja algoritma

Grid menggunakan `S` untuk start, `G` untuk goal, `#` untuk obstacle, dan `.` untuk sel yang dapat dilalui. Gerakan diperbolehkan ke atas, kanan, bawah, dan kiri, tanpa diagonal. Setiap langkah memiliki biaya 1.

Kedua algoritma menggunakan Manhattan Distance:

```text
h(n) = |row(n) - row(goal)| + |col(n) - col(goal)|
```

Heuristic ini mengestimasi jarak tanpa memperhitungkan obstacle. Untuk aturan gerakan proyek ini, Manhattan Distance tidak melebihi biaya sebenarnya dan konsisten, sehingga A* dapat menemukan jalur optimal.

| Aspek | A* | GBFS |
| --- | --- | --- |
| Prioritas | `f(n) = g(n) + h(n)` | `h(n)` |
| Pertimbangan | Biaya dari start dan estimasi ke goal | Estimasi ke goal |
| Optimalitas | Menjamin jalur optimal dengan heuristic dan aturan proyek ini | Tidak menjamin jalur optimal |
| Pengelolaan pencarian | Memperbarui biaya dan parent ketika menemukan jalur lebih murah | Menandai sel saat pertama dimasukkan ke frontier |

Keduanya memakai priority queue. Jika nilai prioritas sama, node yang dimasukkan lebih dahulu dipilih lebih dahulu. Urutan tetangga adalah atas, kanan, bawah, kiri. Aturan ini memengaruhi urutan pencarian dan jalur yang dipilih ketika ada beberapa pilihan.

## 2. Testcase

Semua testcase pembanding memiliki tepat satu start dan satu goal, baris dengan panjang seragam, dan minimal satu solusi. Biaya terpendek berikut diperiksa secara terpisah menggunakan BFS dengan aturan gerakan yang sama.

| Testcase | Ukuran (baris × kolom) | Obstacle | Karakteristik | Biaya terpendek |
| --- | --- | --- | --- | --- |
| Simple | 5 × 8 | 3 | Grid kecil dengan sedikit obstacle | 11 |
| Moderate | 9 × 15 | 49 | Lebih banyak obstacle dan pilihan jalur | 22 |
| Misleading | 15 × 21 | 93 | Arah yang terlihat dekat ke goal dapat membawa detour | 22 |
| Dense | 15 × 21 | 140 | Koridor sempit yang berkelok | 174 |
| Large | 30 × 30 | 223 | Grid lebih besar untuk membandingkan eksplorasi dan waktu | 58 |

`testcases/nosolution.txt` berukuran 4 × 5 dan tidak memiliki solusi. File ini dipakai dalam tes GBFS, bukan dalam lima testcase benchmark. Ukuran dan hasil di README ini mengikuti file terbaru; informasi Large 41 × 61 dalam `README-test.md` merupakan informasi versi lama.

## 3. Metode pengukuran

`src/main.cpp` memuat seluruh testcase sebelum pengukuran. Untuk setiap testcase, kedua algoritma menerima objek Grid yang sama dan jumlah pengulangan yang sama.

1. Satu pemanggilan tanpa pengukuran digunakan untuk pemanasan dan mengambil path serta metrik.
2. Algoritma dipanggil berulang kali, dengan nilai default 1.000 kali atau sesuai argumen terminal.
3. Setiap pemanggilan diukur menggunakan `std::chrono::steady_clock`.
4. Durasi dijumlahkan dan dibagi jumlah pengulangan.
5. Hasil ditampilkan di terminal dan disimpan ke `results/comparison.csv`.

```text
AverageTimeUs = total durasi pemanggilan dalam mikrodetik / jumlah pengulangan
```

Interval pengukuran mencakup pemanggilan algoritma, termasuk inisialisasi struktur pencarian dan rekonstruksi jalur. Pembacaan file, pencetakan grid, penulisan CSV, pemeriksaan konsistensi hasil, serta penghancuran hasil run setelah stop timer berada di luar interval tersebut.

| Metrik | Makna |
| --- | --- |
| Found | Apakah goal ditemukan |
| PathCost | Jumlah langkah pada jalur, yaitu `path.size() - 1` |
| ExpandedNodes | Jumlah ekspansi node; goal tidak dihitung karena pemeriksaan goal dilakukan sebelum ekspansi |
| MaxFrontierSize | Jumlah entri terbanyak dalam priority queue selama pencarian |
| AverageTimeUs | Rata-rata waktu pemanggilan dalam mikrodetik (`us`) |

## 4. Hasil eksperimen

Tabel berikut disalin dari [results/comparison.csv](results/comparison.csv) yang tersedia saat README disusun. Semua run menemukan solusi. CSV tidak menyimpan jumlah pengulangan, spesifikasi komputer, atau opsi kompilasi, sehingga konfigurasi yang menghasilkan angka ini tidak dapat dipastikan dari CSV saja.

| Testcase | Algoritma | Found | Path Cost | Expanded Nodes | Max Frontier Size | Rata-rata waktu (us) |
| --- | --- | --- | --- | --- | --- | --- |
| Simple | A* | true | 11 | 36 | 6 | 9.017 |
| Simple | GBFS | true | 11 | 11 | 9 | 2.864 |
| Moderate | A* | true | 22 | 85 | 7 | 19.237 |
| Moderate | GBFS | true | 22 | 22 | 7 | 4.197 |
| Misleading | A* | true | 22 | 54 | 23 | 14.753 |
| Misleading | GBFS | true | 34 | 48 | 25 | 9.959 |
| Dense | A* | true | 174 | 174 | 1 | 27.655 |
| Dense | GBFS | true | 174 | 174 | 1 | 22.701 |
| Large | A* | true | 58 | 481 | 48 | 112.626 |
| Large | GBFS | true | 60 | 62 | 46 | 14.158 |

Untuk membandingkan waktu, rasio dihitung sebagai `waktu A* / waktu GBFS`. Kelebihan biaya GBFS dihitung sebagai `(biaya GBFS - biaya A*) / biaya A* × 100%`.

| Testcase | Rasio waktu A*/GBFS | Tambahan langkah GBFS | Kelebihan biaya GBFS |
| --- | --- | --- | --- |
| Simple | 3,15 | 0 | 0% |
| Moderate | 4,58 | 0 | 0% |
| Misleading | 1,48 | 12 | 54,55% |
| Dense | 1,22 | 0 | 0% |
| Large | 7,95 | 2 | 3,45% |

## 5. Analisis per testcase

### Simple

Keduanya menemukan jalur optimal dengan biaya 11. GBFS hanya mengekspansi 11 node, sedangkan A* mengekspansi 36 node. Pada grid ini, prioritas berdasarkan jarak ke goal berhasil mengarahkan GBFS ke jalur optimal dengan eksplorasi lebih sedikit. Waktu A* sekitar 3,15 kali waktu GBFS.

Namun, frontier maksimum GBFS adalah 9, lebih besar daripada A* yang bernilai 6. Lebih sedikit ekspansi tidak selalu berarti frontier lebih kecil: node alternatif dapat tetap tersimpan ketika goal ditemukan.

### Moderate

Biaya jalur keduanya tetap optimal, yaitu 22. A* mengekspansi 85 node, sedangkan GBFS hanya 22 node, dengan frontier maksimum yang sama-sama bernilai 7. GBFS menemukan solusi optimal dengan waktu sekitar 4,58 kali lebih singkat pada pengukuran ini. Penambahan obstacle dalam kasus ini belum menyebabkan pilihan greedy menghasilkan jalur lebih panjang.

### Misleading

A* memperoleh biaya 22, sedangkan GBFS memperoleh biaya 34. Jalur GBFS memiliki 12 langkah tambahan atau 54,55% lebih panjang. Ini menunjukkan bahwa posisi dengan Manhattan Distance kecil belum tentu berada pada jalur dengan total biaya paling rendah ketika ada obstacle.

GBFS tetap lebih cepat, tetapi perbedaannya lebih kecil: A* membutuhkan sekitar 1,48 kali waktu GBFS. Jumlah ekspansinya juga berdekatan, yaitu 54 dan 48. Frontier GBFS justru sedikit lebih besar, yaitu 25 dibandingkan 23. Keuntungan waktu GBFS di sini disertai penurunan kualitas jalur yang cukup besar.

### Dense

Keduanya menghasilkan biaya 174, mengekspansi 174 node, dan memiliki frontier maksimum 1. Koridor sempit membatasi pilihan pencarian, sehingga perbedaan fungsi prioritas tidak memberi perbedaan pada metrik jalur dan ekspansi.

A* tetap tercatat lebih lambat: 27.655 us dibandingkan 22.701 us, dengan rasio sekitar 1,22. Karena jumlah ekspansinya sama, selisih waktu tidak dapat dijelaskan hanya oleh banyaknya ekspansi. Pengelolaan biaya, struktur data, dan variasi pengukuran dapat berkontribusi; eksperimen ini tidak mengukur kontribusi masing-masing secara terpisah.

### Large

A* menemukan jalur optimal dengan biaya 58. GBFS menemukan jalur dengan biaya 60, hanya dua langkah lebih panjang atau sekitar 3,45%. Sebaliknya, A* mengekspansi 481 node dibandingkan 62 node oleh GBFS dan memerlukan sekitar 7,95 kali waktu GBFS. Ini merupakan perbedaan waktu terbesar di antara lima testcase.

Pada grid ini, eksplorasi tambahan A* menghasilkan penghematan dua langkah. Frontier maksimum keduanya relatif dekat, yaitu 48 dan 46. Hasil tersebut dapat dipakai untuk mempertimbangkan apakah jaminan jalur terpendek lebih penting daripada waktu pencarian dalam kebutuhan aplikasi.

## 6. Kesimpulan

A* menghasilkan jalur optimal pada kelima testcase, sesuai hasil validasi BFS. GBFS juga optimal pada Simple, Moderate, dan Dense, tetapi menghasilkan jalur lebih panjang pada Misleading dan Large. GBFS memiliki waktu lebih rendah pada semua pengukuran yang tersimpan.

A* cocok ketika jalur terpendek merupakan kebutuhan utama. GBFS dapat dipertimbangkan ketika respons cepat lebih penting dan jalur yang sedikit atau cukup jauh lebih panjang masih dapat diterima. Kasus Misleading memperlihatkan bahwa penurunan kualitas tersebut dapat besar, sehingga kesesuaian GBFS perlu dinilai dengan bentuk lingkungan yang akan digunakan.

Jumlah ekspansi dan frontier maksimum harus dibaca bersama: GBFS tidak selalu memiliki frontier lebih kecil meskipun ekspansinya lebih sedikit. Ukuran grid juga bukan satu-satunya penentu kesulitan; susunan obstacle dan banyaknya pilihan jalur memengaruhi perilaku kedua algoritma.

## 7. Keterbatasan eksperimen

- Lima grid ini belum mewakili seluruh kemungkinan lingkungan. Hasil tidak membuktikan A* selalu lebih lambat atau GBFS selalu menghasilkan jalur lebih panjang.
- Waktu dalam mikrodetik dapat dipengaruhi aktivitas sistem, compiler, optimasi, dan kondisi cache. Pengulangan membantu meratakan fluktuasi, tetapi CSV hanya memuat rata-rata tanpa simpangan baku atau distribusi waktu.
- A* selalu diukur sebelum GBFS pada setiap testcase. Pengaruh urutan pengukuran belum dievaluasi.
- Implementasi memakai struktur data berbeda: A* menggunakan beberapa vector dua dimensi, sedangkan GBFS menggunakan vector satu dimensi. Selisih waktu mencerminkan implementasi tersebut sekaligus strategi pencariannya.
- Frontier maksimum merupakan indikator jumlah entri antrean, bukan penggunaan memori total dalam byte. Struktur parent, biaya, dan penanda kunjungan juga memakai memori. Pada A*, entri lama dapat tetap berada di antrean hingga dikeluarkan dan dilewati.
- Hasil bergantung pada posisi start/goal, obstacle, heuristic, urutan tetangga, dan tie-breaking.
- README merupakan snapshot CSV saat disusun. Jika testcase atau CSV berubah, tabel dan analisis perlu diperbarui.

## 8. Kompilasi dan penggunaan

Gunakan compiler C++11 atau lebih baru. Jalankan perintah berikut dari folder utama proyek di PowerShell. Folder `results/` harus tersedia.

```powershell
g++ -std=c++11 -O2 -Wall -Wextra -I include src/main.cpp src/grid.cpp src/heuristic.cpp src/astar.cpp src/gbfs.cpp -o comparison.exe
.\comparison.exe
```

Tanpa argumen, setiap algoritma diukur 1.000 kali per testcase. Untuk 5.000 pengulangan:

```powershell
.\comparison.exe 5000
```

Jumlah pengulangan berlaku sama untuk A* dan GBFS. Satu run tambahan per algoritma per testcase dipakai untuk pemanasan dan mengambil metrik. Setiap eksekusi mengganti `results/comparison.csv`; simpan salinannya jika ingin membandingkan beberapa eksperimen.

Terminal menampilkan metrik, grid awal, jalur A*, dan jalur GBFS. Tanda `*` menunjukkan jalur akhir. Visualisasi ini tidak menunjukkan urutan seluruh node yang dieksplorasi dan berada di luar pengukuran waktu.

## 9. Menjalankan tes

`tests/test_gbfs.cpp` memeriksa validitas jalur pada lima testcase serta perilaku ketika solusi tidak tersedia. Pemeriksaan meliputi posisi start/goal, langkah bertetangga, obstacle, biaya jalur, dan kondisi hasil tanpa solusi.

```powershell
g++ -std=c++11 -Wall -Wextra -I include tests/test_gbfs.cpp src/gbfs.cpp src/grid.cpp src/heuristic.cpp -o test_gbfs.exe
.\test_gbfs.exe
```

Jika semua pemeriksaan lolos, baris terakhir adalah `All GBFS tests passed.` Jangan memakai `-DNDEBUG` karena tes menggunakan `assert`. Jangan menyertakan `src/main.cpp` dalam kompilasi tes karena tes memiliki fungsi `main()` sendiri.

`tests/test_astar.cpp` masih kosong saat README disusun. Karena itu, belum tersedia suite tes otomatis khusus A*. Validasi BFS atas biaya terpendek pada bagian testcase merupakan pemeriksaan terpisah dan tidak menggantikan suite tes lengkap.
