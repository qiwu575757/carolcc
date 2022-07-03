    .arch armv7-a
    .file "test/07_const_var_defn3.sy"
    .text
    .global main
    .arm
main:
    .main_pre:
    push {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    mov r11, #1024
    sub sp, sp, r11
    .main_0:
    mov r0, #5
    mov r11, #1024
    add sp, sp, r11
    pop {r4, r5, r6, r7, r8, r9, r10, r11, pc}
    bx  lr 
    .data
