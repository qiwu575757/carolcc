define  dso_local i32 @getint() {
}
define  dso_local i32 @getch() {
}
define  dso_local @getfloat() {
}
define  dso_local @putint(0x5555555b1ed0 0%0) {
}
define  dso_local @putch(0x5555555b1ed0 0%0) {
}
define  dso_local @putfloat(0x5555555b1ef0 0%0) {
}
define  dso_local i32 @getarray(i32 * %0) {
}
define  dso_local i32 @getfarray(* %0) {
}
define  dso_local @putarray(0x5555555b1ed0 0%0,i32 * %1) {
}
define  dso_local @putfarray(0x5555555b1ed0 0%0,i32 * %1) {
}
define  dso_local @starttime() {
}
define  dso_local @stoptime() {
}
define  dso_local i32 @__mtstart() {
}
define  dso_local @__mtend(0x5555555b1ed0 0%0) {
}
define  dso_local i32 @main() {
entry:
0:
    %1 = alloca i32 * , align 4
