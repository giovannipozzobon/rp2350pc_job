// *******************************************************************************************
// *******************************************************************************************
//
//      Name :      renderer.h
//      Purpose :   Renderer includes
//      Date :      24th July 2025
//      Author :    Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************
// *******************************************************************************************

#pragma once

// *******************************************************************************************
// 
//                                          Common code
// 
// *******************************************************************************************

#include "pico/asm_helper.S"

pico_default_asm_setup
.align 2

//
//      This macro does expand1 4 times, with an offset into the source byte data. Expand1 therefore
//      needs to unpack one byte.
//
.macro  expand4 offset
    expand1 (\offset)+0
    expand1 (\offset)+1
    expand1 (\offset)+2
    expand1 (\offset)+3
.endm

// *******************************************************************************************
//
//                              ARM version of wrapper macros
//
// *******************************************************************************************

#ifdef __arm__

//
//      Stock requirements for ARM6 Thumb
//
.syntax unified
.cpu cortex-m33
.thumb
//
//      The initialisation phase. The parameter is the total number of bytes of data to convert.
//
.macro preamble count
    push    {r0-r5}                                                                 // Push registers on stack, no LR
    mov     r4,#\count/16                                                           // How many chunks of 16 bytes do we do.
.endm
//
//      The main body. First expand 16 bytes (unrolled for speed). Adjust the source and target registers accordingly
//      and loop round the required number of times, restore and return
//
.macro body incSource,incTarget,nextRender
    expand4     0                                                                   // Unrolled expand
    expand4     4
    expand4     8
    expand4     12

    add     r0,r0,#\incTarget*16                                                    // Adjust target ptr
    add     r1,r1,#\incSource*16                                                    // Adjust source ptr
    add     r4,r4,#-1                                                               // And loop round.
    cmp     r4,#0
    bne     \nextRender
    pop     {r0-r5}                                                                 // Restore registets and exit
    bx      lr
.endm

#endif

// *******************************************************************************************
//
//                                          RISCV version
//
// *******************************************************************************************

#ifdef __riscv

//
//      The initialisation phase. The parameter is the total number of bytes of data to convert.
//
.macro preamble count
    li      a5,\count/16                                                            // How many chunks of 16 bytes do we do.
.endm

//
//      The main body. First expand 16 bytes (unrolled for speed). Adjust the source and target registers accordingly
//      and loop round the required number of times, restore and return
//
.macro body incSource,incTarget,nextRender
    expand4    0                                                                    // Unrolled expand
    expand4    4
    expand4    8
    expand4    12

    addi    a0,a0,\incTarget*16                                                     // Adjust target pointer
    addi    a1,a1,\incSource*16                                                     // Adjust source pointe
    addi    a5,a5,-1                                                                // Loop round
    bnez    a5,\nextRender
    ret                                                                             // Return to caller.
.endm

#endif
