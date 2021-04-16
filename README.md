# Tugas 1 magang tim ICHIRO 
Implementasi OOP, eksplorasi sebebas mungkin, bahasa bisa python/c++
Ngerjain lewat repository github dan diberi dokumentasi nya (penjelasan rinci class, dsbnya) di README.md

Nama: Ridzki Raihan Alfaza
NRP: 5025201178
Departemen Teknik Informatika

# Checkers
  Permainan checkers yang dimulai langsung dengan warna putih yang dilambangkan dengan huruf w, dan diikuti oleh hitam yang dilambangkan b.
# Penjelasan rinci Class

## playingBoard
Menyimpan char yang merepresentasikan kotak putih dan kotak hitam dan simbol lainnya di dalam papan main (`wBox`, `bBox`). Mengandung nilai jumlah kotak ukuran papan main (`height`, `width`) serta jumlah tinggi x lebar satu kotak (`cHeight`, `cWidth`) Semua karakter disimpan menggunakan malloc ke dalam pointer `*cords`.
  ### init
  Membuat papan main menggunakan malloc sebesar luas papan menggunakan malloc char, pola checkers dibuat menggunakan modulo.
  ### show 
  Menghapus terminal dan menampilkan papan main serta grid untuk menunjukkan kordinat per kotak (angka secara horizontal, huruf secara vertikal). `int clrScr` bernilai 0 apabila tidak ingin menghapus terminal sebelum menampilkan papan.
  ### lookAt
  Mengeluarkan pointer char pada tengah kotak pada x dan y tertentu.
  ### place dan rem
  place mengubah char pada tengah kotak pada (x,y) menjadi char lain.
  rem menghapus char pada tengah kotak (x,y) menjadi putih atau hitam.
  ### isOccupied
  True bila ada char pada kotak (x,y) yang bukan box putih atau hitam.
  
## checkersBoard
Anak dari playingBoard, berfungsi sebagai juri di permainan checkers.
  ### checkersBoard()
  Constructor checkersBoard, mengatur ukuran papan main dan menjalankan `checkersInit()` dan `init()`.
  ### checkersInit
  Membuat array yang isinya semua alamat checker.
  ### makeApiece
  Membuat checker tim `t` pada (x,y).
  ### setPiece
  Memindahkan checker ke (x,y).
  ### remPiece
  Menghilangkan checker pada (x,y).
  ### pieceAt
  Mengembalikan checker pada (x,y).
  ### cleanAllMarkers
  Menghapus semua marker jalan pada papan main.
  ### canEatAt
  Mengembalikan jumlah makan maksimum yang dilakukan pada checker pada (x,y) menggunakan rekursi.
  ### chainEat
  Memberi marker untuk rantai makan terpanjang yang bisa diambil.
  ### eatChain
  Melakukan forced eat untuk checker yang dapat memakan paling banyak. Jika yang dapat memakan lebih dari 1, meminta input untuk memilih checker yang gerak.
  ### playerTurn
  - Apabila rantai makan paling panjang bukan 0, memaksa pemain untuk mengambil gerakan yang memberi makan paling banyak.
  - Apabila rantai makan paling panjang 0, meminta input pemain untuk memilih checker mana yang ingin digerakkan.
  - Saat bergerak, kotak yang dapat dipilih untuk tujuan gerak dilambangkan dengan marker
  
## checker
  Melambangkan satu checker di papan, menyimpan posisi checker di papan (`posX`,`posY`) dan alamat checkerBoard yang dia diletakkan.
  ### make
  Mengisi posisi dan tim checker dan menggambarkan dia di papan main.
  ### move
  Apabila dapat bergerak, meminta input untuk checker bergerak ke mana.
  ### moving
  Memindah checker ke (x,y)
  ### forcedEat
  Memimdahkan checker ke (x,y) serta menghapus checker yang dilompat.
  ### checkPromote
  `Promote` checker jika dia di ujung papan bagian musuh.
  ### Promote
  Mengubah checker jadi queen. Mengubah simbil checker menjadi huruf kapital.
