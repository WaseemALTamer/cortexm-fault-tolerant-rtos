.syntax unified
.cpu cortex-m4
.thumb

.global g_pfnVectors
.global Reset_Handler

.extern main

.extern _estack
.extern _sidata
.extern _sdata
.extern _edata
.extern _sbss
.extern _ebss


.section .isr_vector, "a", %progbits
.align 2

g_pfnVectors:
    .word _estack
    .word Reset_Handler
    .word NMI_Handler
    .word HardFault_Handler
    .word MemManage_Handler
    .word BusFault_Handler
    .word UsageFault_Handler
    .word 0
    .word 0
    .word 0
    .word 0
    .word SVC_Handler
    .word DebugMon_Handler
    .word 0
    .word PendSV_Handler
    .word SysTick_Handler

    /* External interrupt slots */
    .rept 85
    .word Default_Handler
    .endr

/* -------------------------------------------------
 * Reset handler
 * ------------------------------------------------- */
.section .text.Reset_Handler, "ax", %progbits
.align 2

.thumb_func
Reset_Handler:
    /* Copy initialized data from Flash to RAM */
    ldr r0, =_sidata
    ldr r1, =_sdata
    ldr r2, =_edata

copy_data:
    cmp r1, r2
    bcc copy_word
    b zero_bss

copy_word:
    ldr r3, [r0]
    str r3, [r1]
    adds r0, #4
    adds r1, #4
    b copy_data

zero_bss:
    /* Clear the .bss section */
    ldr r1, =_sbss
    ldr r2, =_ebss
    movs r3, #0

zero_loop:
    cmp r1, r2
    bcc zero_word
    b call_main

zero_word:
    str r3, [r1]
    adds r1, #4
    b zero_loop

call_main:
    bl main

hang:
    b hang

/* -------------------------------------------------
 * Default exception/interrupt handler
 * ------------------------------------------------- */
.section .text.Default_Handler, "ax", %progbits
.align 2

.thumb_func
Default_Handler:
    b Default_Handler

/* Weak aliases: handlers can later be replaced */
.weak NMI_Handler
.thumb_set NMI_Handler, Default_Handler

.weak HardFault_Handler
.thumb_set HardFault_Handler, Default_Handler

.weak MemManage_Handler
.thumb_set MemManage_Handler, Default_Handler

.weak BusFault_Handler
.thumb_set BusFault_Handler, Default_Handler

.weak UsageFault_Handler
.thumb_set UsageFault_Handler, Default_Handler

.weak SVC_Handler
.thumb_set SVC_Handler, Default_Handler

.weak DebugMon_Handler
.thumb_set DebugMon_Handler, Default_Handler

.weak PendSV_Handler
.thumb_set PendSV_Handler, Default_Handler

.weak SysTick_Handler
.thumb_set SysTick_Handler, Default_Handler

.end