#ifndef _LCD_H
#define _LCD_H

enum {
	NORMAL = 0,
	INVERT = 1,
};

/* NORMAL : normal polarity
 * INVERT : invert polarity
 */
typedef struct pins_polarity {
	int de; 		/* normal : high level data can be transmitted */
	int pwren; 		/* normal : high level valid */
	int vclk;		/* normal : get data when falling */
	int rgb;		/* normal : high level for 1 */
	int hsync;		/* normal : high level pulse */
	int vsync;		/* normal : high level pulse */
}pins_polarity, *p_pins_polarity;

typedef struct time_sequence {
	/* vertical */
	int tvp; /* vsync pulse width */
	int tvb; /* vertical back porch, black upside */
	int tvf; /* vertical front porch, black downside */

	/* horizontal */
	int thp; /* hsync pulse width */
	int thb; /* horizontal back porch, black left */
	int thf; /* horizontal front porch, black right */

	int vclk;
}time_sequence, *p_time_sequence;

typedef struct lcd_parms {
	char *name;

	/* pin polarity */
	pins_polarity pins_pol;

	/* timing */
	time_sequence time_seq;

	/* resolution, bpp */
	int xres;
	int yres;
	int bpp;

	/* framebuffer address */
	unsigned int fb_base;
}lcd_params, *p_lcd_params;

#endif /* _LCD_H */
