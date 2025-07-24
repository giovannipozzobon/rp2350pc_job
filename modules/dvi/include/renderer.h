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

#include "pico/asm_helper.S"

pico_default_asm_setup
.align 2

#ifdef __arm__
.syntax unified
.cpu cortex-m33
.thumb
#endif

.macro  expand4 offset
    expand1 (\offset)+0
    expand1 (\offset)+1
    expand1 (\offset)+2
    expand1 (\offset)+3
.endm

#ifdef __arm__

.macro preamble count
    push    {r0-r5}
    mov     r4,#\count/16
.endm

.macro body incSource,incTarget,nextRender
    expand4     0
    expand4     4
    expand4     8
    expand4     12

    add     r0,r0,#\incTarget*16
    add     r1,r1,#\incSource*16
    add     r4,r4,#-1
    cmp     r4,#0
    bne     \nextRender
    pop     {r0-r5}
    bx      lr
.endm

#endif

#ifdef __riscv
#endif
