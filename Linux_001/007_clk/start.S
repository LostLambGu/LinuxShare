.text
.global _start

_start:

	/* Turn off watchdog */
	ldr r0, =0x53000000
	ldr r1, =0
	str r1, [r0]

	/* set MPLL, FCLK : HCLK : PCLK = 400m : 100m : 50m */
	/* LOCKTIME(0x4C000000) = 0xFFFFFFFF */
	ldr r0, =0x4C000000
	ldr r1, =0xFFFFFFFF
	str r1, [r0]

	/* CLKDIVN(0x4C000014) = 0x5, tFCLK : tHCLK : tPCLK = 1 : 4 : 8 */
	ldr r0, =0x4C000014
	ldr r1, =0x5
	str r1, [r0]

	/* set CPU to the asynchronous mode  */
	mrc p15,0,r0,c1,c0,0
	orr r0,r0,#0xc0000000   //R1_nF:OR:R1_iA
	mcr p15,0,r0,c1,c0,0

	/* Set MPLLCON(0x4C000004) = (92 << 12) | (1 << 4) | (1 << 0) 
	*	m = MDIV + 8 = 92 + 8 = 100
	*	p = PDIV + 2 = 1 + 2 = 3
	*	s = SDIV = 1
	*	FCKK = 2 * m * Fin / (p * 2 ^ 1) = 400M
	*/
	ldr r0, =0x4C000004
	ldr r1, =(92 << 12) | (1 << 4) | (1 << 0)
	str r1, [r0]

	/* 一旦设置了PLL, 就会锁定lock time, 直到PLL输出稳定
	*	然后CPU工作于新的频率FCLK 
	*/

	/* 设置内存
	 * 分辨是nor / nand 启动
	 * 写0到0地址, 再读出来
	 * 如果得到0, 表示0地址的内容被修改了, 它对应ram, 这就是nand启动
	 * 否则就是nor启动
	 */
	mov r1, #0
	ldr r0, [r1]
	str r1, [r1]
	ldr r2, [r1]
	cmp r1, r2
	ldr sp, =0x40000000 + 4096
	moveq sp, #4096
	streq r0, [r1]

	bl main

halt :
	b halt


