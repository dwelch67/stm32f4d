
/* vectors.s */
.cpu cortex-m3
.thumb

.word   0x20002000  /* stack top address */
.word   _start      /* 1 Reset */
.word   hang        /* 2 NMI */
.word   hang        /* 3 HardFault */
.word   hang        /* 4 MemManage */
.word   hang        /* 5 BusFault */
.word   hang        /* 6 UsageFault */
.word   hang        /* 7 RESERVED */
.word   hang        /* 8 RESERVED */
.word   hang        /* 9 RESERVED*/
.word   hang        /* 10 RESERVED */
.word   hang        /* 11 SVCall */
.word   hang        /* 12 Debug Monitor */
.word   hang        /* 13 RESERVED */
.word   hang        /* 14 PendSV */
.word   hang        /* 15 SysTick */
.word   hang        /* 16 External Interrupt(0) */
.word   hang        /* 17 External Interrupt(1) */
.word   hang        /* 18 External Interrupt(2) */
.word   hang        /* 19 ...   */

.thumb_func
.global _start
_start:
    /*ldr r0,stacktop */
    /*mov sp,r0*/
    bl notmain
    b hang

.thumb_func
hang:   b .

/*.align
stacktop: .word 0x20001000*/

;@-----------------------
.thumb_func
.globl PUT16
PUT16:
    strh r1,[r0]
    bx lr
;@-----------------------
.thumb_func
.globl PUT32
PUT32:
    str r1,[r0]
    bx lr
;@-----------------------
.thumb_func
.globl GET32
GET32:
    ldr r0,[r0]
    bx lr

.thumb_func
.globl fun
fun:
    vmov s0,r0
    vcvt.f32.u32 s0,s0
    vmov s1,r1
    vcvt.f32.u32 s1,s1
    vmul.f32 s0,s0,s1
    vcvt.u32.f32 s0,s0
    vmov r0,s0
    bx lr

.end
