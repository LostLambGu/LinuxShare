#include "my_printf.h"
#include "string_utils.h"


#define NOR_FLASH_BASE  0  /* jz2440, nor-->cs0, base addr = 0 */


/* 比如:   55H 98 
 * 本意是: 往(0 + (0x55)<<1)写入0x98
 */
void nor_write_word(unsigned int base, unsigned int offset, unsigned int val)
{
	volatile unsigned short *p = (volatile unsigned short *)(base + (offset << 1));
	*p = val;
}

void nor_cmd(unsigned int offset, unsigned int cmd)
{
	nor_write_word(NOR_FLASH_BASE, offset, cmd);
}

unsigned int nor_read_word(unsigned int base, unsigned int offset)
{
	volatile unsigned short *p = (volatile unsigned short *)(base + (offset << 1));
	return *p;
}

unsigned int nor_dat(unsigned int offset)
{
	return nor_read_word(NOR_FLASH_BASE, offset);
}

void wait_ready(unsigned int addr)
{
	unsigned int val;
	unsigned int pre;

	pre = nor_dat(addr>>1);
	val = nor_dat(addr>>1);
	while ((val & (1<<6)) != (pre & (1<<6)))
	{
		pre = val;
		val = nor_dat(addr>>1);
	}
}

/* 进入NOR FLASH的CFI模式
 * 读取各类信息
 */
void do_scan_nor_flash(void)
{
	char str[4];
	unsigned int size;
	int regions, i;
	int region_info_base;
	int block_addr, blocks, block_size, j;
	int cnt;

	int vendor, device;

	/* 打印厂家ID, 设备ID */
	nor_cmd(0x555, 0xaa); /* 解锁 */
	nor_cmd(0x2aa, 0x55);
	nor_cmd(0x555, 0x90); /* read id */
	vendor = nor_dat(0);
	device = nor_dat(1);
	nor_cmd(0, 0xf0); /* reset */

	nor_cmd(0x55, 0x98); /* 进入cfi模式 */

	str[0] = nor_dat(0x10);
	str[1] = nor_dat(0x11);
	str[2] = nor_dat(0x12);
	str[3] = '\0';
	printf("str = %s\r\n", str);

	/* 打印容量 */
	size = 1 << (nor_dat(0x27));
	printf("vendor id = 0x%x, device id = 0x%x, nor size = 0x%x, %dM\r\n", vendor, device, size, size/(1024 * 1024));

	/* 打印各个扇区的起始地址 */
	/* 名词解释:
	 * erase block region : 里面含有1个或多个block, 它们的大小一样
	 * 一个nor flash含有一个或多个region
	 * 一个region含有一个或多个block(扇区)
	 * erase block region information:
	 * 前2字节+1 : 表示该region有多少个block
	 * 后2字节*256 : 表示block的大小
	 */
	regions = nor_dat(0x2c);
	region_info_base = 0x2d;
	block_addr = 0;
	printf("BLock/Sector start Address:\r\n");
	cnt = 0;
	for (i = 0; i < regions; i++)
	{
		blocks = 1 + nor_dat(region_info_base) + (nor_dat(region_info_base + 1) << 8);
		block_size = 256 * (nor_dat(region_info_base + 2) + (nor_dat(region_info_base + 3) << 8));
		region_info_base += 4;

		for (j = 0; j < blocks; j++)
		{
			/* 打印每个block的起始地址 */
			// printf("0x%08x ", block_addr);
			printHex(block_addr);
			putchar(' ');
			cnt++;
			block_addr += block_size;
			if (cnt % 5 == 0)
			{
				printf("\n\r");
			}
		}
	}
	printf("\r\n");

	/* 退出CFI模式 */
	nor_cmd(0, 0xf0);
}

void do_erase_nor_flash(void)
{
	unsigned int addr;

	/* 获得地址 */
	printf("Enter the address of sector to erase: ");
	addr = get_uint();

	printf("erasing ...\r\n");
	nor_cmd(0x555, 0xaa); /* 解锁 */
	nor_cmd(0x2aa, 0x55);
	nor_cmd(0x555, 0x80); /* erase sector */

	nor_cmd(0x555, 0xaa); /* 解锁 */
	nor_cmd(0x2aa, 0x55);
	nor_cmd(addr>>1, 0x30); /* 发出扇区地址 */
	wait_ready(addr);
}

void do_write_nor_flash(void)
{
	unsigned int addr;
	unsigned char str[100];
	int i, j;
	unsigned int val;

	/* 获得地址 */
	printf("Enter the address of sector to write: ");
	addr = get_uint();

	printf("Enter the string to write: ");
	gets(str);

	printf("writing ...\r\n");

	/* str[0], str[1]==>16bit
	 * str[2], str[3]==>16bit
	 */
	i = 0;
	j = 1;
	while (str[i] && str[j])
	{
		val = str[i] + (str[j]<<8);
		
		/* 烧写 */
		nor_cmd(0x555, 0xaa);	 /* 解锁 */
		nor_cmd(0x2aa, 0x55); 
		nor_cmd(0x555, 0xa0);	 /* program */
		nor_cmd(addr>>1, val);
		/* 等待烧写完成 : 读数据, Q6无变化时表示结束 */
		wait_ready(addr);

		i += 2;
		j += 2;
		addr += 2;
	}

	val = str[i];
	/* 烧写 */
	nor_cmd(0x555, 0xaa);	 /* 解锁 */
	nor_cmd(0x2aa, 0x55); 
	nor_cmd(0x555, 0xa0);	 /* program */
	nor_cmd(addr>>1, val);
	/* 等待烧写完成 : 读数据, Q6无变化时表示结束 */
	wait_ready(addr);
}
void do_read_nor_flash(void)
{
	unsigned int addr;
	volatile unsigned char *p;
	int i, j;
	unsigned char c;
	unsigned char str[16];

	/* 获得地址 */
	printf("Enter the address to read: ");
	addr = get_uint();

	p = (volatile unsigned char *)addr;

	printf("Data : \r\n");
	/* 长度固定为64 */
	for (i = 0; i < 4; i++)
	{
		/* 每行打印16个数据 */
		for (j = 0; j < 16; j++)
		{
			/* 先打印数值 */
			c = *p++;
			str[j] = c;
			printf("%02x ", c);
		}

		printf("  ;");

		for (j = 0; j < 16; j++)
		{
			/* 打印字符 */
			if (str[j] < 0x20 || str[j] > 0x7e) /* 不可视字符 */
				putchar('.');
			else
				putchar(str[j]);
		}
		printf("\r\n");
	}
}

void nor_flash_test(void)
{
	char c;

	while (1)
	{
		/* 打印菜单, 供我们选择测试内容 */
		printf("[s] Scan nor flash\n\r");
		printf("[e] Erase nor flash\n\r");
		printf("[w] Write nor flash\n\r");
		printf("[r] Read nor flash\n\r");
		printf("[q] quit\n\r");
		printf("Enter selection: ");

		c = getchar();
		printf("%c\n\r", c);

		/* 测试内容:
		 * 1. 识别nor flash
		 * 2. 擦除nor flash某个扇区
		 * 3. 编写某个地址
		 * 4. 读某个地址
		 */
		switch (c)		 
		{
			case 'q':
			case 'Q':
				return;
				break;
				
			case 's':
			case 'S':
				do_scan_nor_flash();
				break;

			case 'e':
			case 'E':
				do_erase_nor_flash();
				break;

			case 'w':
			case 'W':
				do_write_nor_flash();
				break;

			case 'r':
			case 'R':
				do_read_nor_flash();
				break;
			default:
				break;
		}
	}
}


