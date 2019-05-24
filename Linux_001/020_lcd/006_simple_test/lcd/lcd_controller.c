#include "lcd_controller.h"

#define LCD_CONTROLLER_NUM 10

static p_lcd_controller p_array_lcd_controller[LCD_CONTROLLER_NUM];
static p_lcd_controller g_p_lcd_controller_selected;

int register_lcd_controller(p_lcd_controller plcdcon)
{
	int i;
	for (i = 0; i < LCD_CONTROLLER_NUM; i++)
	{
		if (!p_array_lcd_controller[i])
		{
			p_array_lcd_controller[i] = plcdcon;
			return i;
		}
	}

	return -1;
}

int select_lcd_controller(char *name)
{
	int i;
	for (i = 0; i < LCD_CONTROLLER_NUM; i++)
	{
		if (p_array_lcd_controller[i] && !strcmp(p_array_lcd_controller[i]->name, name))
		{
			g_p_lcd_controller_selected = p_array_lcd_controller[i];
			return i;
		}
	}
	return -1;
}

/* up: receive params of different LCDs
 * down: init LCD controller with the params
 */
int lcd_controller_init(p_lcd_params plcdparams)
{
	/* init the selected lcd controller */
	if (g_p_lcd_controller_selected)
	{
		g_p_lcd_controller_selected->init(plcdparams);
		return 0;
	}
	return -1;
}

void lcd_controller_enable(void)
{
	if (g_p_lcd_controller_selected)
	{
		g_p_lcd_controller_selected->enable();
	}
}

void lcd_controller_disable(void)
{
	if (g_p_lcd_controller_selected)
	{
		g_p_lcd_controller_selected->disable();
	}
}

void lcd_controller_add(void)
{
	s3c2440_lcd_controller_add();
}
