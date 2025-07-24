// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      render640_256.S
//      Purpose :   Manual Renderer : 640 pixels, 256 colours, no palette.
//      Date :      24th July 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************

#include "renderer.h"

// *******************************************************************************************
//
//      Renderer for 320x240x256. Simply copies each byte out twice, doubling the 
//      effective width.
//
// *******************************************************************************************

#ifdef __arm__

//
//      ARM Expander
//
.macro  expand1 offset
    ldrb  r3,[r1,#\offset]
    strb  r5,[r0,#(\offset)]
.endm

#endif

#ifdef __riscv
//
//      RISCV Expander
//
.macro  expand1 offset
    lbu     a4,(\offset)(a1)
    sb      a4,(\offset)(a0)
.endm

#endif

//
//      Common routine.
//
.global ASMRender640_256
.section .time_critical.ASMRender640_256

ASMRender640_256:
    preamble    640
_RenderLoop2:    
    body        1,1,_RenderLoop2

