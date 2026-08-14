
; /**
; * This file include -*- C++ -*- for everyone.
;  *  @author Contributed by Kpangan ZkxFsd
;  * (kpanganbaru@gmail.com)
;  *  @file boot.asm
;  *  ISF (Inpianda Software Foundation)
;  *  @date 17 Mei 2026 -> ~~~
;  *  @version 1.0
;  *  @brief Hearts of Operating Systems.
;  *  @copyright (C) 2026-INF (ISF) Inpianda Software Foundation,
;  * Inc. This file is part and real of ISF Product. You can use
;  * or modify it under ISF terms. All rights reserved.
;  */
; /**
;  * Terms of Inpianda Software Foundation (ISF)
;  * You can change it or modify it under ISF Terms.
;  * You should have received a copy of GNU General Public
;  * License, and a copy of (ISF) Inpianda Software
;  * Foundation along with this code. Real Website :
;  * Main                         : https://www.inpianda.org/index.html.
;  * License                      : https://www.inpianda.org/license/license.html. 
;  * Terms (ISF terms, GNU terms) : https://www.inpianda.org/terms/terms.html.  
;  * - You can modify the code if you can.
;  * - You can change the code under GNU License.
;  * - You can use the code to your project, \ 
;  *   but remember under ISF terms and GNU License.
;  *   " use You can !!! " 
;  */

; global _start ; main entry

; global switch_to_stack ; switch per core
; global core1_entry ; core 1

; global keyboard ; keyboard
; global outb ; output
; global inb ; input


; extern main_   ; main kernel


; ; 80x25 Resolution (VGA)
; align 4
; MULTIBOOT_MAGIC    equ 0x1BADB002
; MULTIBOOT_FLAGS    equ 0x00000003  
; MULTIBOOT_CHECKSUM equ -(MULTIBOOT_MAGIC + MULTIBOOT_FLAGS)

; section .text.boot
; dd MULTIBOOT_MAGIC
; dd MULTIBOOT_FLAGS
; dd MULTIBOOT_CHECKSUM

; ; OUTB
; outb:
;     mov dx, [esp + 4]    
;     mov al, [esp + 8]    
;     out dx, al
;     ret
; ; INB
; inb:
;     mov dx, [esp + 4]    
;     in al, dx            
;     ret                  

; ; Scancode, essential for KEYBOARD.
; keyboard:
;     mov dx, 0x60
;     in al, dx
;     ret

; switch_to_stack:
;     mov eax, [esp + 4]    ; eax = Alamat penampung ESP lama (&old_esp)
;     mov edx, [esp + 8]    ; edx = Alamat ESP thread baru (new_esp)

;     pusha                 ; Simpan 8 register umum ke stack saat ini
;     mov [eax], esp        ; Simpan nilai ESP sekarang ke variabel old_esp C++
;     mov esp, edx          ; PINDAH STACK POINTER KE THREAD BARU!
;     popa                  ; Kembalikan register milik thread baru
;     ret                   ; Jump/kembali ke thread baru

; extern core1_main

; core1_entry:
;     cli                   ; Matikan interrupt dulu
;     call core1_main       ; Lompat langsung ke C++ Core 1
;     hlt                   ; Safety halted kalau fungsi selesai

; _start:
;     cli
;     ; dont panic if error, if still run, forget.
;     call main_
;     hlt

global _start           ; main entry Core 0
global switch_to_stack  ; switch per core
global core1_entry      ; entry point Core 1
global keyboard         ; keyboard
global outb             ; output port
global inb              ; input port

extern main_            ; C++ Kernel Main (Core 0)
extern core1_main       ; C++ Kernel Main (Core 1)

; Multiboot Header (80x25 / Text Mode)
align 4
MULTIBOOT_MAGIC    equ 0x1BADB002
MULTIBOOT_FLAGS    equ 0x00000003  
MULTIBOOT_CHECKSUM equ -(MULTIBOOT_MAGIC + MULTIBOOT_FLAGS)

section .text.boot
dd MULTIBOOT_MAGIC
dd MULTIBOOT_FLAGS
dd MULTIBOOT_CHECKSUM

section .text

; ------------------------------------------------------------------
; I/O Port Helper
; ------------------------------------------------------------------
outb:
    mov dx, [esp + 4]    
    mov al, [esp + 8]    
    out dx, al
    ret

inb:
    mov dx, [esp + 4]    
    in al, dx            
    ret                  

keyboard:
    mov dx, 0x60
    in al, dx
    ret

; ------------------------------------------------------------------
; Context Switcher (Di-panggil oleh yield_core0 & yield_core1)
; ------------------------------------------------------------------
switch_to_stack:
    mov eax, [esp + 4]    ; eax = Alamat penampung ESP lama (&old_esp)
    mov edx, [esp + 8]    ; edx = Alamat ESP thread baru (new_esp)

    pusha                 ; Simpan 8 register umum (EAX, ECX, EDX, EBX, ESP, EBP, ESI, EDI)
    mov [eax], esp        ; Simpan nilai ESP sekarang ke variabel old_esp C++
    mov esp, edx          ; PINDAH STACK POINTER KE THREAD BARU!
    popa                  ; Kembalikan register milik thread baru
    ret                   ; Jump/kembali ke thread baru

; ------------------------------------------------------------------
; Core 1 Entry Point (Dipanggil saat LAPIC SIPI Bangunin Core 1)
; ------------------------------------------------------------------
align 16
core1_entry:
    cli                   ; Matikan interrupt

    ; FIX CRITICAL: Set stack khusus buat Core 1 biar gak crash pas panggil C++!
    mov esp, core1_temp_stack_top

    call core1_main       ; Masuk ke C++ Core 1!

.halt_core1:
    hlt                   ; Loop safety kalau C++ selesai
    jmp .halt_core1

; ------------------------------------------------------------------
; Core 0 Boot Entry Point (Multiboot Entry)
; ------------------------------------------------------------------
_start:
    cli
    
    ; Setup Stack Utama Kernel Core 0
    mov esp, kernel_stack_top

    ; Panggil Main C++ Kernel
    call main_

.halt_core0:
    hlt                   ; Loop safety
    jmp .halt_core0

; ------------------------------------------------------------------
; Alokasi Memory Stack (BSS Section)
; ------------------------------------------------------------------
section .bss
align 16

; Stack khusus Boot Core 0 (16 KB)
kernel_stack_bottom:
    resb 16384
kernel_stack_top:

; Stack sementara Boot Core 1 (16 KB)
core1_temp_stack_bottom:
    resb 16384
core1_temp_stack_top: