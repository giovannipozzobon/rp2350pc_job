#define CHIPS_IMPL
#include "mos6502_cpu_module.h"
#include "mos6502cpu.h"


// RAM emulata da 64 KB
uint8_t ram[1 << 16];

// Indirizzo di video RAM stile C64 ($0400)
#define FB_ADDR 0x0400
#define FB_LEN  5  // Lunghezza di "HELLO"

// Carica un piccolo programma 6502 che scrive "HELLO" in RAM video
void load_test_program(void) {
    const uint8_t program[] = {
        0xA9, 'H',         // LDA #'H'
        0x8D, 0x00, 0x04,  // STA $0400
        0xA9, 'E',
        0x8D, 0x01, 0x04,
        0xA9, 'L',
        0x8D, 0x02, 0x04,
        0xA9, 'L',
        0x8D, 0x03, 0x04,
        0xA9, 'O',
        0x8D, 0x04, 0x04,
        0x00               // BRK
    };
    memcpy(&ram[0x0801], program, sizeof(program));

    // Vettore di RESET → $0801
    ram[0xFFFC] = 0x01;
    ram[0xFFFD] = 0x08;
}

// Stampa il contenuto della video RAM
void dump_framebuffer(void) {
    COMPrint("\nContenuto della RAM video ($0400): ");
    for (int i = 0; i < FB_LEN; i++) {
        char c = ram[FB_ADDR + i];
        if (c >= 32 && c < 127)
            putchar(c);
        else
            putchar('.');
    }
    printf("\n");
}


int MAINPROGRAM(int argc,char *argv[]) {

    // inizializza la connessione per il debug
    COMInitialise();

    mos6502cpu_t cpu;

    memset(ram, 0, sizeof(ram));
    load_test_program();

    mos6502cpu_desc_t desc = {
        .bcd_disabled = false,
        .mos6510cpu_in_cb = NULL,
        .mos6510cpu_out_cb = NULL,
        .mos6510cpu_io_pullup = 0xFF,
        .mos6510cpu_io_floating = 0x00,
        .mos6510cpu_user_data = NULL
    };

    mos6502cpu_init(&cpu, &desc);

    bool running = true;
    while (running) {
        // Tick della CPU
        mos6502cpu_tick(&cpu);

        uint16_t addr = cpu.addr;

        if (cpu.rw) {
            // Lettura da RAM
            cpu.data = ram[addr];
        } else {
            // Scrittura in RAM
            ram[addr] = cpu.data;
        }

        // Termina se inizia BRK (istruzione 0x00 con SYNC attivo)
        if ((cpu.sync) && (ram[addr] == 0x00)) {
            running = false;
        }
    }

    dump_framebuffer();
    return 0;

}

