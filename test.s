    .arch armv7-a
    .file "test/04_arr_defn3.sy"
    .text
    .global main
    .arm
main:
    .main_pre:
    push {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    mov r11, #1024
    sub sp, sp, r11
    .main_0:
    @ alloc %1 to sp+0,  size: 16
    add r0, sp, #0
    @ gep %2,  from %1
    mov r1, #0
    add r1, r1, r0
    @ gep %3,  from %2
    mov r2, #0
    add r2, r2, r1
    mov r3, #1
    str r3, [r2, #0]
    @ gep %4,  from %3
    mov r3, #0
    @ gep += 1 * 4, 
    add r3, r3, #4
    add r3, r3, r2
    mov r4, #2
    str r4, [r3, #0]
    @ gep %5,  from %3
    mov r4, #0
    @ gep += 2 * 4, 
    add r4, r4, #8
    add r4, r4, r2
    mov r5, #3
    str r5, [r4, #0]
    @ gep %6,  from %3
    mov r5, #0
    @ gep += 3 * 4, 
    add r5, r5, #12
    add r5, r5, r2
    mov r6, #4
    str r6, [r5, #0]
    @ gep %7,  from %1
    mov r6, #0
    @ gep += 1 * 8, 
    add r6, r6, #8
    @ gep += 1 * 4, 
    add r6, r6, #4
    add r6, r6, r0
    ldr r7, [r6, #0]
    @ gep %9,  from %1
    mov r8, #0
    add r8, r8, r0
    ldr r9, [r8, #0]
    add r10, r7, r9
    @store %2 from reg 1, to sp+16
    str r1, [sp, #16]
    @store %4 from reg 3, to sp+28
    str r3, [sp, #28]
    @store %5 from reg 4, to sp+32
    str r4, [sp, #32]
    @ gep %12,  from %1
    mov r1, #0
    @ gep += 1 * 4, 
    add r1, r1, #4
    add r1, r1, r0
    @store %4 from reg 0, to sp+28
    str r0, [sp, #28]
    ldr r0, [r1, #0]
    @store %5 from reg 0, to sp+32
    str r0, [sp, #32]
    add r0, r10, r0
    @store %3 from reg 2, to sp+24
    str r2, [sp, #24]
    @store %6 from reg 5, to sp+36
    str r5, [sp, #36]
    @store %7 from reg 6, to sp+40
    str r6, [sp, #40]
    @ gep %15,  from %1
    mov r2, #0
    @ gep += 1 * 8, 
    add r2, r2, #8
    add r2, r2, r0
    @store %6 from reg 0, to sp+36
    str r0, [sp, #36]
    ldr r0, [r2, #0]
    @store %7 from reg 0, to sp+40
    str r0, [sp, #40]
    add r0, r0, r0
    mov r11, #1024
    add sp, sp, r11
    pop {r4, r5, r6, r7, r8, r9, r10, r11, pc}
    bx  lr 
    .data
