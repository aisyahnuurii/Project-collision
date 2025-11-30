# Project-collision 

#Deskripsi Proyek

Proyek ini adalah simulasi collision yang dibuat menggunakan C++ dan library SFML (Simple and Fast Multimedia Library). Program ini dibuat untuk membandingkan dua algoritma deteksi collision : Brute Force dan QuadTree untuk menunjukkan perbedaan performa jumlah objek yang banyak.

#Fitur Utama

1. Simulasi collition 
2. Dua algoritma collision detection
3. Spawn otomatis lingkaran baru saat terjadi collision
4. Real-time performance 
5. Warna yang berubah-ubah untuk setiap lingkaran yang terbuat.

#Tahap Pengerjaan

1. Persiapan Struktur Folder dan File
Langkah Pembuatan Folder Proyek :
-Buat folder utama PROJECT COLLISION
-Buat subfolder: build, include, src
-Buat file-file header di folder include/
-Buat main.cpp di folder src/
-Buat compile.bat untuk memudahkan kompilasi

2. Tahap Implementasi Logika Program
A. Class Vec2 (Vec2.h)
Fungsi dari class Vec 2 adalah untuk merepresentasikan Vektor 2D dan operasi matematika dasar, untuk fitur yang diimplementasikan sebagai berikut : 
-Konstruktor untuk inisialisasi koordinat x dan y
-Operator overloading (+, -, *, /, +=, -=)
-Method length() untuk menghitung panjang vektor
-Method normalize() untuk normalisasi vektor
-Method dot() untuk dot product
-Static method distance() untuk menghitung jarak antar titik

B. Class Circle (Circle.h)
Fungsi class Circle berguna untuk merepresentasikan objek lingkaran dengan fisika

Properti:
-position          : Posisi lingkaran (Vec2)
-velocity          : Kecepatan lingkaran (Vec2)
-radius            : Jari-jari lingkaran
-color             : Warna lingkaran
-mass              : Massa (berdasarkan r²)
-collisionCooldown : Cooldown untuk mencegah spawn berlebihan

Method Penting:
1.update(float dt)
-Update posisi berdasarkan velocity dan delta time
-Untuk mengurangi collision cooldown
2.isColliding(const Circle& other)
-Berfungsi untuk deteksi collision dengan lingkaran lain
-Menggunakan distance antara center < sum of radii
3.resolveCollision(Circle& other)
-Mengimplementasi fisika tumbukan elastis
-Menggunakan conservation of momentum
-Formula impulse: J = -(1 + e) * vrel · n / (1/m1 + 1/m2)
-Separation untuk mencegah overlap
-Return true jika collision berhasil diresolve
4.checkWallCollision(float width, float height)
-Deteksi dan resolve collision dengan dinding
-Membalik velocity saat menabrak dinding
5.draw(sf::RenderWindow& window)
-Render lingkaran ke window SFML

C. Class Rectangle (Rectangle.h)
Berfungsi untuk merepresentasi area rectangular untuk QuadTree

Method Penting:
-contains(Vec2 point)            : Cek apakah point di dalam rectangle
-intersects(Rectangle range)     : Cek intersection dengan rectangle lain
-intersectsCircle(Circle circle) : Cek intersection dengan lingkaran
