#include "lcd.h"

#define LCD_FB_BASE 0x33c00000

lcd_params lcd_4_3_params = {
	.name = "lcd_4.3",
	.pins_pol = {
		.de 	= NORMAL,	/* normal: high level data transmit */
		.pwren	= NORMAL, 	/* normal: high level valid */
		.vclk	= NORMAL,	/* normal: falling edge data acquire */
		.rgb	= NORMAL,	/* normal: high level for 1 */
		.hsync	= INVERT,	/* normal: high level pulse */
		.vsync	= INVERT,	/* normal: high level pulse */
	},
	.time_seq = {
		/* vertical */
		.tvp	= 10,	/* vsync pulse width */
		.tvb	= 2,	/* black upside, vertical back porch */
		.tvf	= 2,	/* black downside, vertical front porch */

		/* Horizontal */
		.thp	= 41,	/* hsync pulse width */
		.thb	= 2,	/* black left, horizontal back porch */
		.thf	= 2,	/* black right, horizontal front porch */

		.vclk	= 9,	/* Mhz */
	},
	.xres	= 480,
	.yres	= 272,
	.bpp	= 32,	/* 16, no 24bpp */
	.fb_base = LCD_FB_BASE,
};

void lcd_4_3_add(void)
{
	register_lcd(&lcd_4_3_params);
}
