// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      render320x240x256.S
//      Purpose :   Manual Renderer : 320x240, 256 colours.
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
    ldrb  r5,[r3,r2]
    strb  r5,[r0,#(\offset)*2]
    strb  r5,[r0,#(\offset)*2+1]
.endm

#endif

#ifdef __riscv
//
//      RISCV Expander
//
.macro  expand1 offset
    lb  a4,(\offset)(a1)
    sb  a4,(\offset)*2(a0)
    sb  a4,(\offset)*2+1(a0)
.endm

#endif

//
//      Common routine.
//
.global ASMRender320To640
.section .time_critical.ASMRender320To640

ASMRender320To640:
    preamble    320
_RenderLoop:    
    body        1,2,_RenderLoop

