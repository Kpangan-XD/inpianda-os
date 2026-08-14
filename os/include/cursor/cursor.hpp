#ifndef INP_CURSOR_HPP
#define INP_CURSOR_HPP

#include "io.hpp"
extern "C" {
void outb(unsigned short port, unsigned char data);
unsigned char inb(unsigned short port);
char prt_keyboard();
}

#pragma region // CURSOR
// Array buat nyontek status ubin lama (1 byte karakter, 1 byte warna)
inline char backup_karakter = ' ';
inline char backup_warna = 0x07;

// FUNGSI 1: Nyontek teks di layar sebelum ditabrak kursor
inline void simpan_background_text(int mx, int my) {
  int indeks = (my * 80) + mx;
  volatile char *video_mem = (char *)0xB8000;

  backup_karakter = video_mem[indeks * 2];
  backup_warna = video_mem[indeks * 2 + 1];
}

// FUNGSI 2: Ngembaliin teks asli pas kursor geser
inline void hapus_kursor_text_lama(int mx, int my) {
  int indeks = (my * 80) + mx;
  char *video_mem = (char *)0xB8000;

  video_mem[indeks * 2] = backup_karakter;
  video_mem[indeks * 2 + 1] = backup_warna;
}

// FUNGSI 3: GAMBAR KURSORNYA (Pake karakter '+' warna Merah Terang biar
// mencolok!)
inline void draw_cursor_text(int mx, int my) {
  int indeks = (my * 80) + mx;
  char *video_mem = (char *)0xB8000;

  video_mem[indeks * 2] =
      '+'; // Bentuk kursor lo, cok! Bisa lo ganti 'X' atau '>'
  video_mem[indeks * 2 + 1] =
      0x0C; // Warna Merah Terang (Background Hitam 0, Teks Merah C)
}

inline int mouse_x = 40; // Mulai di tengah kolom (0-79)
inline int mouse_y = 12; // Mulai di tengah baris (0-24)

inline void update_posisi_mouse_text(int delta_x, int delta_y) {
  // 1. HAPUS KURSOR DI TEMPAT LAMA DULU
  hapus_kursor_text_lama(mouse_x, mouse_y);

  // 2. JINAKKAN SENSIVITAS (Ubah angka pembagi ke 15 atau 20 kalau masih
  // terlalu kenceng!) Trik ini bikin pergerakan mouse lo jadi lebih presisi
  // mikron, cok!
  int geser_x = delta_x / 15;
  int geser_y = delta_y / 15;

  // 3. UPDATE KOORDINATNYA
  mouse_x += geser_x;
  mouse_y += geser_y;

  // 4. SASIS KUNCI MATI (Absolute Clamping 80x25)
  // Jangan kasih toleransi lolos 1 angka pun keluar batas layar, cok!
  if (mouse_x < 0)
    mouse_x = 0;
  if (mouse_x > 79)
    mouse_x = 79;
  if (mouse_y < 0)
    mouse_y = 0;
  if (mouse_y > 24)
    mouse_y = 24;

  // 5. CONTEK TEMPAT BARU & GAMBAR KURSOR BARU
  simpan_background_text(mouse_x, mouse_y);
  draw_cursor_text(mouse_x, mouse_y);
}
#pragma endregion // CURSOR

inline void init_mouse_bengkel() {
  // Ngomong ke keyboard controller buat aktifin port mouse (Auxiliary device)
  outb(0x64, 0xA8);

  // Minta controller kirim perintah ke mouse
  outb(0x64, 0x20);
  unsigned char status = inb(0x60) | 2;
  outb(0x64, 0x60);
  outb(0x60, status);

  // Kasih perintah ke mouse murni: "Woy mouse, aktifin pengiriman data!"
  outb(0x64, 0xD4);
  outb(0x60, 0xF4);
  inb(0x60); // Buang data ACK balasannya
}

inline void tunggu_mouse_siap(unsigned char tipe) {
  int timeout = 100000;
  if (tipe == 0) {
    // Tunggu sampai data masuk (Bit 0 harus bernilai 1)
    while (((inb(0x64) & 1) == 0) && timeout--) {
    }
  } else {
    // Tunggu sampai port kosong siap dikirim perintah (Bit 1 harus bernilai 0)
    while (((inb(0x64) & 2) != 0) && timeout--) {
    }
  }
}

inline unsigned char baca_mouse_pure() {
  // Tunggu sampai Bit 0 di port 0x64 bernilai 1 (artinya data hardware udah
  // mateng!)
  while ((inb(0x64) & 1) == 0) {
    // CPU nahan di sini, gak boleh lolos!
  }
  // Begitu data ready, langsung sikat port 0x60!
  return inb(0x60);
}

inline void polling_mouse_data() {
  // SIKAT 3 BYTE SECARA KETAT DAN BERURUTAN!
  unsigned char byte1 = baca_mouse_pure();
  unsigned char byte2 = baca_mouse_pure();
  unsigned char byte3 = baca_mouse_pure();

  // FILTER BIT TANDA (Sign Bit Extension manual)
  // Ini taktik paling aman kalau cast (signed char) lo didepak sama compiler
  int d_x = (int)byte2;
  int d_y = (int)byte3;

  if ((byte1 & 0x10) == 0x10)
    d_x |= 0xFFFFFF00; // Kalau minus, bungkus bitnya!
  if ((byte1 & 0x20) == 0x20)
    d_y |= 0xFFFFFF00;

  // Balikin sumbu Y biar gak kebalik
  d_y = -d_y;

  // =================================================================
  // EKSEKUSI UPDATE KOORDINAT (Skala diperhalus, bagi 20 biar makin jinak!)
  // =================================================================
  hapus_kursor_text_lama(mouse_x, mouse_y);

  mouse_x += (d_x / 20);
  mouse_y += (d_y / 20);

  // Kunci mati absolut di ring ubin text mode 80x25
  if (mouse_x < 0)
    mouse_x = 0;
  if (mouse_x > 79)
    mouse_x = 79;
  if (mouse_y < 0)
    mouse_y = 0;
  if (mouse_y > 24)
    mouse_y = 24;

  simpan_background_text(mouse_x, mouse_y);
  draw_cursor_text(mouse_x, mouse_y);
}

inline void satpam_gabungan_input() {
  // 1. Cek apakah di port 0x64 emang beneran BISA dibaca (Bit 0 harus bernilai
  // 1)
  if ((inb(0x64) & 1) == 1) {

    // Baca status portnya buat nentuin kasta datanya
    unsigned char status = inb(0x64);

    // 2. JALUR MOUSE (Kalau Bit 5 bernilai 1, ini fiks paket tikus!)
    if ((status & 0x20) == 0x20) {

      // TUNGGU DAN SIKAT BYTE 1 (Status & Tombol Klik)
      unsigned char byte1 = inb(0x60);

      // TUNGGU DAN SIKAT BYTE 2 (Delta X)
      tunggu_mouse_siap(0);
      unsigned char byte2 = inb(0x60);

      // TUNGGU DAN SIKAT BYTE 3 (Delta Y)
      tunggu_mouse_siap(0);
      unsigned char byte3 = inb(0x60);

      // Konversi murni ke koordinat minus/plus
      int d_x = (signed char)byte2;
      int d_y = (signed char)byte3;
      d_y = -d_y;

      // JEBRET! Update kursor tanda plus lo!
      update_posisi_mouse_text(d_x, d_y);
    }
    // 3. JALUR KEYBOARD (Kalau Bit 5 bernilai 0, ini murni ketukan jari lo!)
    else {
      // Sikat scancode tombol keyboardnya langsung dari port 0x60
      unsigned char scancode = inb(0x60);

      // Panggil fungsi cetak teks lo di sini, cok!
      // Misal: proses_keyboard_lo(scancode);
    }
  }
}

inline void update_cursor(int x, int y) {
  // Hitung posisi linear 1D dari koordinat 2D
  unsigned short posisi = (y * 80) + x;

  // Kirim byte rendah (low byte) ke hardware
  outb(0x3D4, 0x0F);
  outb(0x3D5, (unsigned char)(posisi & 0xFF));

  // Kirim byte tinggi (high byte) ke hardware
  outb(0x3D4, 0x0E);
  outb(0x3D5, (unsigned char)((posisi >> 8) & 0xFF));
}

inline void enable_cursor() {
  // Tembak register 0x0A (Cursor Start Register)
  outb(0x3D4, 0x0A);
  outb(0x3D5,
       0x00); // 0x00 artinya: Aktifkan kursor dan set baris awal scanline ke 0

  // Tembak register 0x0B (Cursor End Register)
  outb(0x3D4, 0x0B);
  outb(0x3D5, 0x0F); // 0x0F artinya: Set baris akhir scanline ke 15 (kursor
                     // kotak penuh)
}

extern int cursor_x;
extern int  cursor_y;
#include <stdint.h>
inline void praw_pursor_otomatis() {
  // 1. Hitung lokasi index byte atribut warna tempat pursor berada saat ini
  int index_warna = (cursor_x * 80 + cursor_y) * 2 + 1;

  // 2. INTIP warna asli yang lagi tayang di koordinat itu sekarang, cok!
  uint8_t warna_asli_layar = video_memory[index_warna];

  // 3. ISOLASI 4 bit kiri (Cuma ambil warna Background-nya doang, bray!)
  // Misal warna terminal lo Hitam_Hijau (0x02), mask & 0xF0 bakal ngambil 0x00
  // (Hitam murni)
  uint8_t bg_terminal = (warna_asli_layar & 0xF0);

  // 4. BIKIN WARNA PURSOR BARU
  // Kita gabungin Background Terminal tadi sama warna karakter kursornya (misal
  // Putih Kelap-kelip = 0x0F)
  uint8_t warna_pursor_baru = bg_terminal | 0x0F;

  // 5. RENDER/PRAW LANGSUNG KE HARDWARE VGA, COK!
  int index_karakter = (cursor_y * 80 + cursor_x) * 2;

  // Ubah warna background kursornya tanpa ngerubah huruf/karakter yang lagi
  // diketik!
  video_memory[index_karakter + 1] = warna_pursor_baru;
}
#endif
