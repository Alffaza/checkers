# Tugas 1 magang tim ICHIRO 
Implementasi OOP, eksplorasi sebebas mungkin, bahasa bisa python/c++
Ngerjain lewat repository github dan diberi dokumentasi nya (penjelasan rinci class, dsbnya) di README.md

Nama: Ridzki Raihan Alfaza
NRP: 5025201178
Departemen Teknik Informatika

# Penjelasan rinci Class

## playingBoard
  Menyimpan char yang merepresentasikan kotak putih dan kotak hitam dan simbol lainnya di dalam papan main (wBox, bBox). Mengandung nilai jumlah kotak ukuran papan main (Height, Width) serta jumlah tinggi x lebar satu kotak (cHeight, cWidth)Semua karakter disimpan menggunakan malloc ke dalam pointer cords.
  ### init()
    Membuat papan main menggunakan malloc sebesar luas papan menggunakan malloc char, pola checkers dibuat menggunakan modulo.
  ### show(int clrScr) 
    Menampilkan papan main serta grid untuk menunjukkan kordinat per kotak (angka secara horizontal, huruf secara vertikal)
  
## Checkers Board
  -Berisi function untuk melihat board dan sebagai juri game
  -anak dari class playing board
  -memiliki semua pointer checker piece
  
## Checker
  -menyimpan posisi dalam board, lambang, warna
  -melakukan promosi menjadi ratu jika di ujung
  
