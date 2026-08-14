cmake_minimum_required(VERSION 3.16)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 1. Kunci Compiler & Set Baremetal (Wajib Paling Atas, Cok!)
if(WIN32)
    set(CMAKE_SYSTEM_NAME Generic)
    set(CMAKE_CXX_COMPILER "clang++")
    set(CMAKE_C_COMPILER "clang")
endif()

project(Inpianda-OS CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 2. Pengaturan Flags Lintas Planet (Windows Clang vs Linux GCC)
if(WIN32)
    message(STATUS "[INPIANDA-OS] MENGGUNAKAN LLVM CLANG MURNI DI WINDOWS, COK!")
    
    set(TARGET_TRIPLE "-target i386-unknown-none-elf")
    
    set(FREESTANDING_FLAGS "${TARGET_TRIPLE} -ffreestanding -O3 -fno-exceptions -fno-rtti -fno-pie -nostdlib")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${FREESTANDING_FLAGS}")
    
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION OFF)
    
    # KUNCI UTAMA WINDOWS CLANG: Paksa -fuse-ld=lld langsung menyatu dengan target triple!
    set(LINKER_FLAGS
        "${TARGET_TRIPLE}"
        "-fuse-ld=lld"
        "-T" "${CMAKE_CURRENT_SOURCE_DIR}/linker.ld"
        "-nostdlib"
        "-Wl,-m,elf_i386"
    )
else()
    message(STATUS "[INPIANDA-OS] MENGGUNAKAN GCC LINUX BAWAAN, BRAY!")

    set(FREESTANDING_FLAGS "-m32 -ffreestanding -O3 -fno-exceptions -fno-rtti -fno-pie -nostdlib")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${FREESTANDING_FLAGS}")

    set(LINKER_FLAGS
        "-Wl,-m,elf_i386"
        "-Wl,-T,${CMAKE_CURRENT_SOURCE_DIR}/linker.ld"
        "-nostdlib"
        "-no-pie"
    )
endif()

# 3. Kompilasi Bootloader Assembly (NASM)
add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/boot.o
    COMMAND nasm -f elf32 ${CMAKE_CURRENT_SOURCE_DIR}/boot/boot.asm -o ${CMAKE_CURRENT_BINARY_DIR}/boot.o
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/boot/boot.asm
    COMMENT "Mengompilasi Assembly ke 32-bit ELF menggunakan NASM"
)

# 5. Daftar File C++ Source Kernel Lo (Wajib ditaruh DI ATAS add_executable agar dibaca CMake!)
set(SOURCES
    main/main.cc
    core/driver/driver.cc
    core/window/window.cc
    core/io/io.cc
    core/cursor/cursor.cc
    core/kernel/kernel.cc
    core/keyboard/keyboard.cc
    core/screen/screen.cc
    core/system/system.cc
)

# Gabungin semuanya jadi Target Executable
add_executable(inpianda-os.bin ${SOURCES} ${CMAKE_CURRENT_BINARY_DIR}/boot.o)

# 4. Jalur Header/Include Modular Lo (Jalur Absolut Biar clangd Nggak Ngamuk!)
target_include_directories(inpianda-os.bin PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/include/
    ${CMAKE_CURRENT_SOURCE_DIR}/include/cursor
    ${CMAKE_CURRENT_SOURCE_DIR}/include/driver
    ${CMAKE_CURRENT_SOURCE_DIR}/include/io
    ${CMAKE_CURRENT_SOURCE_DIR}/include/kernel
    ${CMAKE_CURRENT_SOURCE_DIR}/include/keyboard
    ${CMAKE_CURRENT_SOURCE_DIR}/include/screen
    ${CMAKE_CURRENT_SOURCE_DIR}/include/system
    ${CMAKE_CURRENT_SOURCE_DIR}/include/window
    ${CMAKE_CURRENT_SOURCE_DIR}/include/define
)

# Colok flags linker hasil saringan
target_link_options(inpianda-os.bin PRIVATE ${LINKER_FLAGS})

set_target_properties(inpianda-os.bin PROPERTIES SUFFIX "")

# 6. Auto Run QEMU Emulator

add_custom_target(run
    COMMAND qemu-system-i386 -kernel iso_root/inpianda-os.bin -vga std -display sdl
    DEPENDS iso_root/inpianda-os.bin
    COMMENT "[GOOD] OS Lo Siap Meluncur, Cok!"
)
# Gunakan POST_BUILD hanya untuk membuat file ISO
add_custom_command(TARGET inpianda-os.bin POST_BUILD
    # 1. Bersihkan folder lama agar tidak ada cache sampah
    # COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_CURRENT_BINARY_DIR}/iso_root
    
    # # 2. Buat semua variasi struktur folder GRUB yang dicari oleh BIOS/VMware
    # COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/iso_root/boot/grub
    # COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/iso_root/boot/grub2
    # COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/iso_root/GRUB
    
    # # 3. Salin kernel Anda ke folder /boot
    # COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/inpianda-os.bin ${CMAKE_CURRENT_BINARY_DIR}/iso_root/boot/inpianda-os.bin
    
    # # 4. Buat isi konfigurasi GRUB (ditambahkan perintah 'boot' eksplisit)
    # COMMAND echo -e "set timeout=0\\nset default=0\\nmenuentry 'Inpianda OS' {\\n  multiboot /boot/inpianda-os.bin\\n  boot\\n}" > ${CMAKE_CURRENT_BINARY_DIR}/iso_root/boot/grub/grub.cfg
    
    # # 5. Duplikat konfigurasi ke folder cadangan grub2 (Standar Fedora)
    # COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/iso_root/boot/grub/grub.cfg ${CMAKE_CURRENT_BINARY_DIR}/iso_root/boot/grub2/grub.cfg
    
    # # 6. Bangun ulang ISO dengan xorriso melalui grub2-mkrescue
    # COMMAND grub2-mkrescue -o ${CMAKE_CURRENT_BINARY_DIR}/inpianda-os.iso ${CMAKE_CURRENT_BINARY_DIR}/iso_root
    
    # COMMENT "[BERHASIL] ISO GRUB2 diperbarui. Siap dicoba ulang di VMware!"

# info cpus, info lafic -monitor stdio
    COMMAND qemu-system-i386 -smp 4 -kernel inpianda-os.bin  -vga std -display sdl -monitor stdio

)

#${CMAKE_CURRENT_BINARY_DIR}