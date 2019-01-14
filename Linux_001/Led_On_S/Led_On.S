
/*
 * Turn on LED1: gpf4
 */

 .text
 .global _start

 _start:

 /*
  *Output, 0x100 to 0x56000050
  */
  ldr r1, =0x56000050
  ldr r0, =0x100 /* mov r0, #0x100 */
  str r0, [r1]

 /*
  *Turn on, 0 to 0x56000054
  */
  ldr r1, =0x56000054
  ldr r0, =0
  str r0, [r1]

 halt:
 	b halt

