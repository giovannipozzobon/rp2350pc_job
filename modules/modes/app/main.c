// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      main.c
//      Purpose :   Video mode module test program
//      Date :      7th July 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************

#include "modes_module.h"
#include "modes_module_local.h"

uint8_t vRAM[640*480];

static void plotPixel(uint16_t x,uint16_t y,uint8_t colour);
static void drawLine(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint8_t colour);

int MAINPROGRAM() {

    VMDInitialise();
    VMDSetVideoMemory(vRAM,sizeof(vRAM));

    memset(vRAM,0x18,640*480);                                              // Fast screen clear
    for (int i = 0;i < 640*480;i++) {                                       // Random dots.  
        vi.drawSurface[i] = (random() & 63) ? 0:random();  
    } 

    uint8_t c = 0x7;                                                        // Test triangle
    drawLine(10,10,vi.xScreen-20,vi.yScreen/2,c);
    drawLine(vi.xScreen-20,vi.yScreen/2,vi.xScreen/2,vi.yScreen-20,c);
    drawLine(10,10,vi.xScreen/2,vi.yScreen-20,c);

    while (COMAppRunning()) {                                                                     
        YIELD();                                                                
    }
    return 0;
}

#define max(a,b) (((a) > (b)) ? (a):(b))

/**
 * @brief      Draws a line
 *
 * @param[in]  x1      x1
 * @param[in]  y1      y1
 * @param[in]  x2      x2
 * @param[in]  y2      y2
 * @param[in]  colour  colour
 */
static void drawLine(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint8_t colour) {
    int16_t dx = x2-x1;
    int16_t dy = y2-y1;
    int16_t l = max(abs(dx),abs(dy));
    for (int i = 0;i < l;i++) {
        plotPixel(x1+dx*i/l,y1+dy*i/l,colour);
    }
}

/**
 * @brief      Plot a pixel
 *
 * @param[in]  x       x
 * @param[in]  y       y
 * @param[in]  colour  colour
 */
static void plotPixel(uint16_t x,uint16_t y,uint8_t colour) {
    uint8_t *address,mask,shift;
    if (x >= vi.xScreen || y >= vi.yScreen) return;

    switch(vi._dviMode & 0x0F) {
        case 1:
            vi.drawSurface[x+y*vi.bytesPerLine] = colour;
            break;
        case 2:
            address = vi.drawSurface + (x >> 1) + y * vi.bytesPerLine;
            shift = ((x & 1) == 0) ? 0 : 4;
            mask = 0xF0 >> shift;colour = (colour & 0x0F) << shift;
            *address = ((*address) & mask) | colour;
            break;
        case 4:
            address = vi.drawSurface + (x >> 2) + y * vi.bytesPerLine;
            shift = (x & 3) * 2;
            mask = (0x03 << shift) ^ 0xFF;
            colour = (colour & 3) << shift;
            *address = ((*address) & mask) | colour;
            break;
        case 8:
            address = vi.drawSurface + (x >> 3) + y * vi.bytesPerLine;
            shift = (x & 7);
            *address &= ((0x01 << shift) ^ 0xFF);
            if ((colour & 1) != 0) {
                *address |= (0x01 << shift);
            }
            break;
    }
}
