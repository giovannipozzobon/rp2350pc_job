// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      manual.c
//      Purpose :   Default manual renderer (expands 320 to 640)
//      Date :      2nd July 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************

#include "dvi_module.h"
#include "dvi_module_local.h"

DVIRenderBuffer dviRender[2];
static uint8_t mostRecentlyUsed = 0;

void ASMRender320To640(uint8_t *target,uint8_t *data);
static void render320To640(uint8_t *target,uint8_t *data);



/**
 * @brief      A manual renderer, which takes a 320 byte buffer and byte doubles
 *             it into a 640 byte buffer. So if you feed it $13 $2A $4C it spits
 *             out $13 $13 $2A $2A $4C $4C
 *
 * @param[in]  func  What is required of the manual renderer
 * @param      data  The data in the framebuffer (in this case a 320 byte line)
 *
 * @return     { description_of_the_return_value }
 */
uint8_t *__not_in_flash_func(DVI320To640Renderer)(uint8_t func,uint8_t *data) {

    uint8_t *retVal = NULL;

    switch(func) {
        //
        //      Initialise just marks the 2 buffers as not representing anything.
        //
        case DVIM_INITIALISE:
            dviRender[0].source = dviRender[1].source = NULL; 
            memset(dviRender[0].render,0xE0,640);                                   // For testing, so we can see what isn't rendered.
            memset(dviRender[1].render,0x18,640);                                     
            break;
        //
        //      Get Render gets the render for the current address, returns NULL if not
        //      rendered yet. 
        //      
        //      Also tracks the most recently used. When we create a new render, we do it to
        //      the *least* recently used - the MRU is probably going through the DMA at this point.
        //
        case DVIM_GETRENDER:
            if (dviRender[0].source == data) {
                retVal = dviRender[0].render;
                mostRecentlyUsed = 0;
            }
            if (dviRender[1].source == data) {
                retVal = dviRender[1].render;
                mostRecentlyUsed = 1;
            }
            break;
        //
        //      Render Next. Check if it isn't already done - in that case we can reuse it
        //      if not use the least recently used to render data.
        //
        case DVIM_RENDERNEXT:
            if (dviRender[0].source != data && dviRender[1].source != data) {       // If not already rendered
                uint8_t n = 1 - mostRecentlyUsed;                                   // Use *this* buffer - not the most recently used.
                dviRender[n].source = data;                                         // Remember what it is rendering for getRender
                render320To640(dviRender[n].render,data);                           // Do the expansion.
                //ASMRender320To640(dviRender[n].render,data);
            }
            break;
    }
    return retVal;
}
//
//      For other expanders, you can borrow most of this wholesale. The memset() in initialise isn't needed, that was just
//      so I could see what wasn't being converted, but it might be useful. 
//      
//      The only bit that needs changing - mostly - is the render320To640 call which does the actual copying of the 320 bytes
//      to 640 bytes.
//      
//      However, I've left the other functionality in here just in cases.
//      

//
//      Expanders for unrolling the renderer.
//
#define EXPAND1()       { *target++ = *target++ = *data++; }                          
#define EXPAND4()       EXPAND1();EXPAND1();EXPAND1();EXPAND1();
#define EXPAND16()      EXPAND4();EXPAND4();EXPAND4();EXPAND4();

/**
 * @brief      Copy 320 bytes to 640 bytes repeating each byte
 *
 * @param      target  Render buffer
 * @param      data    The 320 byte source data.
 */
static void __not_in_flash_func(render320To640)(uint8_t *target,uint8_t *data) {
    for (uint16_t i = 0;i < 320/16;i++) {
        EXPAND16();
    }    
}

// *******************************************************************************************
//
//      We do not know what order the renders are coming in, or whether they are duplicates, 
//      so with each buffer we store the address of the data that is rendered. PicoDVI
//      assumes lines will be sequential, but this stops hardware vertical scrolling and
//      other tricks.
//      
//      The simplest behaviour of the renderer (which is pointless) is simply to return the
//      provided data on DVIM_GETRENDER and ignore everything else.
//      
//      Renderers are not called with data = NULL (e.g. blank lines) *except* for the 
//      initialisation, which doesn't expect you to render anything.
//      
// *******************************************************************************************

