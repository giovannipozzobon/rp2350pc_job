// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      render160_256.S
//      Purpose :   Manual Renderer : 160 pixels, 256 colours, palette supported.
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
    strb  r5,[r0,#(\offset)*4]
    strb  r5,[r0,#(\offset)*4+1]
    strb  r5,[r0,#(\offset)*4+2]
    strb  r5,[r0,#(\offset)*4+3]
.endm

#endif

#ifdef __riscv
//
//      RISCV Expander
//
.macro  expand1 offset
    lbu     a4,(\offset)(a1)
    add     a4,a4,a2
    lbu     a4,0(a4)
    sb      a4,(\offset)*4(a0)
    sb      a4,(\offset)*4+1(a0)
    sb      a4,(\offset)*4+2(a0)
    sb      a4,(\offset)*4+3(a0)
.endm

#endif

//
//      Common routine.
//
.global ASMRender160_256
.section .time_critical.ASMRender160_256

ASMRender160_256:
    preamble    160
_RenderLoop:    
    body        1,4,_RenderLoop

