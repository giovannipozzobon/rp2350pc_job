#define CHIPS_IMPL
#include "mos6502_cpu_module.h"
#include "mos6502cpu.h"

// RAM emulata da 64 KB
uint8_t ram[1 << 16];

//Video RAM
uint8_t vRAM[640*480];

// Indirizzo di video RAM stile C64 ($0400)
#define FB_ADDR 0x0400
#define FB_LEN  5  // Lunghezza di "HELLO"

void load_test_program(void) {
const uint8_t program[] = {
    0xA9, 'H', 0x8D, 0x00, 0x04, // LDA #'H' : STA $0400
    0xA9, 'I', 0x8D, 0x01, 0x04, // LDA #'I' : STA $0401
    0x4C, 0x0F, 0x08              // JMP $080F (loop infinito)
};
    memcpy(&ram[0x0801], program, sizeof(program));

    // Vettore di RESET → $0801
    ram[0xFFFC] = 0x01;
    ram[0xFFFD] = 0x08;

    // Vettore IRQ → $0900
    ram[0xFFFE] = 0x00;
    ram[0xFFFF] = 0x09;

    // Routine IRQ → scrive '!' in $0405 e RTI
    ram[0x0900] = 0xA9;  // LDA #'!'
    ram[0x0901] = '!';
    ram[0x0902] = 0x8D;  // STA $0405
    ram[0x0903] = 0x05;
    ram[0x0904] = 0x04;
    ram[0x0905] = 0x40;  // RTI
}

void dump_framebuffer(void) {
    printf("Contenuto RAM video ($0400): ");
    for (int i = 0; i < FB_LEN + 1; i++) {
        char c = ram[FB_ADDR + i];
        putchar((c >= 32 && c <= 126) ? c : '.');
        CONWrite((c >= 32 && c <= 126) ? c : '.');
    }
    printf("\n");
}

int MAINPROGRAM(int argc,char *argv[]) {

    // inizializza la connessione per il debug, la console e gli input
    COMInitialise();
    CONInitialise();

    // Set video RAM e mode
    VMDSetVideoMemory(vRAM,sizeof(vRAM));                                           // Set video ram and size
    GFXDraw(Mode,MODE_640_480_256,0);                                               // Set mode.
    GFXDraw(Desktop,0,0);  
    GFXDraw(Colour,0xFFF,0);                                                        // Draw frame
    GFXDraw(Move,4*8-1,5*8-1);GFXDraw(Rect,40*8,32*8);

    CONSetWindow(4,5,40,32);                                                        // Set window in units of 8 pixels (why all the frame draws are *8)


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

   int ticks = 0;

    while (1) {
        mos6502cpu_tick(&cpu);
        uint16_t addr = cpu.addr;

        if (cpu.rw) {
            cpu.data = ram[addr];
        } else {
            ram[addr] = cpu.data;
        }

        // Simula IRQ dopo 1000 tick
        if (ticks == 1000) {
            cpu.irq = true;
        }

        // Rilascia IRQ all'inizio di una nuova istruzione
        if (cpu.sync && cpu.irq) {
            printf("IRQ triggered\n");
            cpu.irq = false;
        }

        // Simula DMA dopo 2000 tick
        if (ticks == 2000) {
            cpu.rdy = false; // blocca la CPU per DMA
            for (int i = 0; i < 20; ++i) mos6502cpu_tick(&cpu);
            ram[0x0402] = 'D';
            ram[0x0403] = 'M';
            ram[0x0404] = 'A';
            cpu.rdy = true;
            printf("DMA completed\n");
        }

        // Stop dopo 3000 tick
        if (ticks++ > 3000) break;
    }

    dump_framebuffer();
    return 0;

}

