#ifndef INP_WINDOW_HPP
#define INP_WINDOW_HPP

inline void draw_rect(int x, int y, int lebar, int tinggi, char warna) {
  char *vga_memory = (char *)0xA0000;

  for (int i = 0; i < tinggi; i++) {
    int baris = y + i;
    // Kalau koordinat Y ngelewatin batas layar (0-199), skip baris ini
    if (baris < 0 || baris >= 200)
      continue;

    for (int j = 0; j < lebar; j++) {
      int kolom = x + j;
      // Kalau koordinat X ngelewatin batas layar (0-319), skip piksel ini
      if (kolom < 0 || kolom >= 320)
        continue;

      vga_memory[baris * 320 + kolom] = warna;
    }
  }
}

inline void draw_taskbar() {
  // Jalur memori teks mode
  char *video_memory = (char *)0xB8000;

  // Baris paling bawah adalah baris ke-24 (ingat indeks dari 0-24)
  int baris_taskbar = 24;

  // 1. Gambar badan taskbar penuh dari kiri ke kanan (kolom 0-79)
  for (int x = 0; x < 80; x++) {
    int indeks = (baris_taskbar * 80) + x;
    video_memory[indeks * 2] = ' ';      // Kosongin karakternya
    video_memory[indeks * 2 + 1] = 0x18; // Warna abu-abu gelap (Taskbar)
  }
}

inline void draw_text_rect(int x, int y, int lebar, int tinggi,
                           char warna_atribut) {
  char *video_memory = (char *)0xB8000;

  for (int i = 0; i < tinggi; i++) {
    for (int j = 0; j < lebar; j++) {
      // Hitung indeks karakter di layar (80 karakter per baris)
      int indeks = ((y + i) * 80 + (x + j));

      // Byte Genap: Isi karakter (kita pake spasi ' ' biar jadi kotak polos)
      video_memory[indeks * 2] = ' ';

      // Byte Ganjil: Isi warna background dan text
      video_memory[indeks * 2 + 1] = warna_atribut;
    }
  }
}

inline void print_text_string(const char *teks, int x, int y,
                              char warna_atribut) {
  char *video_memory = (char *)0xB8000;

  for (int i = 0; teks[i] != '\0'; i++) {
    // Cari posisi kotak karakter (ingat, 1 baris teks mode itu 80 kotak)
    int indeks = ((y * 80) + (x + i));

    // Byte genap diisi hurufnya
    video_memory[indeks * 2] = teks[i];
    // Byte ganjil diisi warnanya (misal tulisan hitam di atas abu-abu)
    video_memory[indeks * 2 + 1] = warna_atribut;
  }
}





// Old Window Management

struct Window {
  const char *judul;
  int x, y;
  int lebar, tinggi;
  char warna_body;
  char warna_bar;
  bool is_focused;
  bool is_minimized;
};

#pragma region CLASS

// ALL road to this Hierarki

class Graphics_ {
protected:
  int x, y;
  int lebar, tinggi;
  unsigned char warna;

public:
  inline Graphics_(int ax, int ay, int al, int at, unsigned char awarna) {
    x = ax;
    y = ay;
    lebar = al;
    tinggi = at;
    warna = awarna;
  }
};

class Taskbar_ : public Graphics_ {
public:
  inline Taskbar_(unsigned char warna) : Graphics_(0, 24, 40, 1, warna) {}

  inline void startmenu() {
    draw_rect(x, y, lebar, tinggi, warna);
    print_text_string(" START ", 0, 24, warna);
  }
  inline void Taskbar() { draw_taskbar(); }
};

// Window management
class Window_ {

protected:
  const char *judul;
  int x, y;                 // Posisi pojok kiri atas jendela di layar
  int lebar, tinggi;        // Ukuran lebar dan tinggi jendela
  unsigned char warna_body; // Warna isi dalam jendela
  unsigned char warna_bar;  // Warna bar atas (Title Bar)
  bool is_focused; // True jika jendela lagi aktif (warna bar jadi terang)
  bool is_minimized;

public:
  inline Window_(const char *wjudul, int wx, int wy, int wl, int wt,
                 bool ifocus, bool iminim, char wbody, char wbar) {
    x = wx;
    y = wy;
    lebar = wl;
    tinggi = wt;
    warna_body = wbody;
    warna_bar = wbar;
    is_focused = ifocus;
    is_minimized = iminim;
    judul = wjudul;
  }
  int getX() { return x; }
  int getY() { return y; }
  int getLebar() { return lebar; }
  int getTinggi() { return tinggi; }
  int setX(int x_baru) { return x = x_baru; }
  int setY(int y_baru) { return y = y_baru; }
  inline void window() {

    draw_text_rect(x, y, lebar, 1,
                   warna_bar); // Title bar
    draw_text_rect(x, y + 1, lebar, tinggi - 1,
                   warna_body); // Body
    print_text_string(judul, x + 1, y, warna_bar);
  }
};

#pragma endregion CLASS

#endif