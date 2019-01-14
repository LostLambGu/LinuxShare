
.text
.global _start

_start:

	ldr sp, =4096 /* nand start */
	// ldr sp, =0x40000000+4096 /* nor start */

	bl main

halt:
	b halt

