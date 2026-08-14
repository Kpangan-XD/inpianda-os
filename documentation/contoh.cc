
/*
 ENTRY(_start) /* Titik masuk utama ada di fungsi _start milik Assembly */
SECTIONS
{
    . = 1M; /* OS dimuat di RAM alamat 1 MB */

    .text : {
        *(.text.boot)  /* Taruh kode booting Assembly paling atas */
        *(.text)       /* Sisa kode C++ mengikuti di bawahnya */
    }
    .rodata : { *(.rodata) }
    .data : { *(.data) }
    .bss  : { *(.bss)  }
}
*/

Siap, bre! Kita bedah **keseluruhan register** yang ada di *dump* CPU #0 lo tadi satu per satu sampai tuntas.

Ini ibarat foto *x-ray* jeroan CPU x86 saat kernel lo lagi jalan. Semuanya punya tugas khusus!

```text
EAX=000b8341 EBX=000b8340 ECX=0000000a EDX=00000072
ESI=000b8000 EDI=00001000 EBP=00000000 ESP=0010b104
EIP=00100492 EFL=00000002 [-------] CPL=0 II=0 A20=1 SMM=0 HLT=0

```

---

## 1. General Purpose Registers (GPR) — "Meja Kerja" CPU

Ini register 32-bit utama yang biasa dipake buat olah data, matematika, dan penunjuk memori.

* **`EAX = 000B8341` (Accumulator):**
Register serbaguna buat operasi aritmatika, *return value* fungsi, dan I/O. Di sini `AL = 0x41` (huruf `'A'`), lagi dipake buat nampilin teks.
* **`EBX = 000B8340` (Base Register):**
Sering dipake sebagai penunjuk alamat dasar memori (*pointer*). Di sini dia megang alamat VGA Text Mode.
* **`ECX = 0000000A` (Counter Register):**
Register penghitung buat perulangan (*loop*) atau operasi *string*. Nilai `0x0A` (10 desimal) kemungkinan besar sisa *counter* perulangan *string/print* lo.
* **`EDX = 00000072` (Data Register):**
Pasangan `EAX` buat perkalian/pembagian besar, atau dipake buat menyimpan nomor *I/O Port* (misal pas pake instruksi `in`/`out` ke hardware).

---

## 2. Index Registers — "Penunjuk Alamat Memori"

* **`ESI = 000B8000` (Source Index):**
Menunjuk ke lokasi data asal pas pemindahan data memori. Nilai `0x000B8000` ini tepat di awal memori layar VGA.
* **`EDI = 00001000` (Destination Index):**
Menunjuk ke lokasi tujuan data. Biasa dipake bareng `ESI` buat instruksi *copy* memori (kayak `memcpy`).

---

## 3. Stack Pointer Registers — "Pengatur Tumpukan Memori"

* **`EBP = 00000000` (Base Pointer):**
Menunjuk ke bagian bawah *stack frame* fungsi saat ini (buat melacak variabel lokal C++). Nilainya `0` berarti fungsi utama/kernel lo belum bikin *frame pointer* baru atau di-compile pake optimasi `-fomit-frame-pointer`.
* **`ESP = 0010B104` (Stack Pointer):**
**INI KRUSIAL BANGET!** Menunjuk ke "puncak" tumpukan memori (*stack*) saat ini. Angka `0x0010B104` ngebuktiin kalau Stack CPU #0 ini **SANGAT SEHAT** dan teralokasi dengan aman di RAM (jauh di atas 1MB)!

---

## 4. Execution & Status Registers — "Kemudi & Indikator CPU"

* **`EIP = 00100492` (Instruction Pointer / Program Counter):**
Ini **otak eksekusi CPU**. `EIP` nampung alamat memori untuk **instruksi selanjutnya** yang bakal dijalankan CPU. Kode kernel lo saat detik itu lagi dieksekusi di alamat `0x00100492`.
* **`EFL = 00000002` (EFLAGS):**
Register bendera status CPU (misal: apakah hasil pengurangan tadi 0, minus, ada *carry*, dsb.). Nilai `0x00000002` artinya cuma *Reserved bit 1* yang menyala, artinya bendera *Interrupt* (`IF`) lagi mati/dihapus (`cli`).

---

## 5. Status Mode Hardware QEMU — "Kondisi Operasional"

Bagian dalam kurung siku `[-------]` dan teks setelahnya itu status khusus arsitektur x86:

| Status | Nilai | Artinya |
| --- | --- | --- |
| **`CPL`** | `0` | **Current Privilege Level = 0.** CPU lo lagi ada di **Ring 0 (Kernel Mode)**. Mode paling sakti tanpa batasan akses ke hardware! |
| **`II`** | `0` | **Interrupt Inhibit.** Lagi gak ada interupsi yang tertunda/ditahan. |
| **`A20`** | `1` | **A20 Line Active.** Jalur memori A20 udah kebuka! Tanpa A20 = 1, CPU gak bakal bisa akses RAM di atas 1 MegaByte. |
| **`SMM`** | `0` | **System Management Mode.** CPU lagi di mode normal, bukan lagi masuk ke mode *Power Management/Firmware BIOS*. |
| **`HLT`** | `0` | **Halt State.** CPU lagi **aktif kerja** mengeksekusi instruksi, gak lagi di-istirahatkan (di-`hlt`). |

---

> 💡 **Rangkuman Singkat:**
> Semua register ini nunjukin kondisi CPU #0 lo lagi **super sehat, berada di mode 32-bit Protected Mode (Ring 0), jalur memori A20 aktif, stack-nya aman, dan lagi sibuk nyetak teks ke layar via VGA Buffer!**



CompileFlags:
  CompilationDatabase: "build",
  Add: [
    "-target", "i386-unknown-none-elf",
    "-ffreestanding",
    "-m32",
    "-Iinclude/",
    "-Iinclude/cursor",
    "-Iinclude/driver",
    "-Iinclude/io",
    "-Iinclude/kernel",
    "-Iinclude/keyboard",
    "-Iinclude/screen",
    "-Iinclude/system",
    "-Iinclude/window",
    "-Iinclude/define",
    "-I."
    "-I../include"
  ]
