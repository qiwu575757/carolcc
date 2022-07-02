    .arch armv7-a
    .file "test/01_var_defn2.sy"
    .text
    .global main
    .arm
main:
    .main_pre:
    push {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    sub sp, sp, #128
    .main_0:
    add r0, sp, #0
    mov r1, #5
    str r1, [r0, #0]
    ldr r2, [r0, #0]
    ldr r3, [r0, #0]
    add r4, r2, r3
    mov r0, r4
    add sp, sp, #128
    pop {r4, r5, r6, r7, r8, r9, r10, r11, pc}
    bx  lr 
    .data
a:
    .long 3
b:
    .long 5
