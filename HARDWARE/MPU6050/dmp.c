/*****************************************************
功能:	采集MPU6050 DMP 数据
CPU:	STC89C54RD+
晶震:	11.0592
环境:	Keli2.0
语言: 	c
作者：	XW1005
来源：	移植 Jeff Jrowberg 公开的程序。
目的：
sda 或则scl 为高时是:释放总线
*****************************************************/

#include "MPU6050.h"
#include "delay.h"
float Q[4];	//四元数

uint8_t dmpdatas[42];	//DMP数据

//以下的 firmware 及 config update 数据来自于 Jeff Jrowberg 公开的程序
/* ================================================================================================ *
| Default MotionApps v2.0 42-byte FIFO packet structure:                                           |
|                                                                                                  |
| [QUAT W][      ][QUAT X][      ][QUAT Y][      ][QUAT Z][      ][GYRO X][      ][GYRO Y][      ] |
|   0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15  16  17  18  19  20  21  22  23  |
|                                                                                                  |
| [GYRO Z][      ][ACC X ][      ][ACC Y ][      ][ACC Z ][      ][      ]                         |
|  24  25  26  27  28  29  30  31  32  33  34  35  36  37  38  39  40  41                          |
* ================================================================================================ */
 uint8_t dmpmemorydata[1929] = 
{
	// bank 0, 256 bytes
	0xFB, 0x00, 0x00, 0x3E, 0x00, 0x0B, 0x00, 0x36, 0x00, 0x01, 0x00, 0x02, 0x00, 0x03, 0x00, 0x00,
	0x00, 0x65, 0x00, 0x54, 0xFF, 0xEF, 0x00, 0x00, 0xFA, 0x80, 0x00, 0x0B, 0x12, 0x82, 0x00, 0x01,
	0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x28, 0x00, 0x00, 0xFF, 0xFF, 0x45, 0x81, 0xFF, 0xFF, 0xFA, 0x72, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x03, 0xE8, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x7F, 0xFF, 0xFF, 0xFE, 0x80, 0x01,
	0x00, 0x1B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x3E, 0x03, 0x30, 0x40, 0x00, 0x00, 0x00, 0x02, 0xCA, 0xE3, 0x09, 0x3E, 0x80, 0x00, 0x00,
	0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00,
	0x41, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x0B, 0x2A, 0x00, 0x00, 0x16, 0x55, 0x00, 0x00, 0x21, 0x82,
	0xFD, 0x87, 0x26, 0x50, 0xFD, 0x80, 0x00, 0x00, 0x00, 0x1F, 0x00, 0x00, 0x00, 0x05, 0x80, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00,
	0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x6F, 0x00, 0x02, 0x65, 0x32, 0x00, 0x00, 0x5E, 0xC0,
	0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0xFB, 0x8C, 0x6F, 0x5D, 0xFD, 0x5D, 0x08, 0xD9, 0x00, 0x7C, 0x73, 0x3B, 0x00, 0x6C, 0x12, 0xCC,
	0x32, 0x00, 0x13, 0x9D, 0x32, 0x00, 0xD0, 0xD6, 0x32, 0x00, 0x08, 0x00, 0x40, 0x00, 0x01, 0xF4,
	0xFF, 0xE6, 0x80, 0x79, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0xD0, 0xD6, 0x00, 0x00, 0x27, 0x10,
	// bank 1, 256 bytes
	0xFB, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00,
	0x00, 0x00, 0xFA, 0x36, 0xFF, 0xBC, 0x30, 0x8E, 0x00, 0x05, 0xFB, 0xF0, 0xFF, 0xD9, 0x5B, 0xC8,
	0xFF, 0xD0, 0x9A, 0xBE, 0x00, 0x00, 0x10, 0xA9, 0xFF, 0xF4, 0x1E, 0xB2, 0x00, 0xCE, 0xBB, 0xF7,
	0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x04, 0x00, 0x02, 0x00, 0x02, 0x02, 0x00, 0x00, 0x0C,
	0xFF, 0xC2, 0x80, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0xCF, 0x80, 0x00, 0x40, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x14,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x03, 0x3F, 0x68, 0xB6, 0x79, 0x35, 0x28, 0xBC, 0xC6, 0x7E, 0xD1, 0x6C,
	0x80, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0xB2, 0x6A, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0xF0, 0x00, 0x00, 0x00, 0x30,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x25, 0x4D, 0x00, 0x2F, 0x70, 0x6D, 0x00, 0x00, 0x05, 0xAE, 0x00, 0x0C, 0x02, 0xD0,
	// bank 2, 256 bytes
	0x00, 0x00, 0x00, 0x00, 0x00, 0x65, 0x00, 0x54, 0xFF, 0xEF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x01, 0x00, 0x00, 0x44, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x01, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x65, 0x00, 0x00, 0x00, 0x54, 0x00, 0x00, 0xFF, 0xEF, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x1B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00,
	0x00, 0x1B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	// bank 3, 256 bytes
	0xD8, 0xDC, 0xBA, 0xA2, 0xF1, 0xDE, 0xB2, 0xB8, 0xB4, 0xA8, 0x81, 0x91, 0xF7, 0x4A, 0x90, 0x7F,
	0x91, 0x6A, 0xF3, 0xF9, 0xDB, 0xA8, 0xF9, 0xB0, 0xBA, 0xA0, 0x80, 0xF2, 0xCE, 0x81, 0xF3, 0xC2,
	0xF1, 0xC1, 0xF2, 0xC3, 0xF3, 0xCC, 0xA2, 0xB2, 0x80, 0xF1, 0xC6, 0xD8, 0x80, 0xBA, 0xA7, 0xDF,
	0xDF, 0xDF, 0xF2, 0xA7, 0xC3, 0xCB, 0xC5, 0xB6, 0xF0, 0x87, 0xA2, 0x94, 0x24, 0x48, 0x70, 0x3C,
	0x95, 0x40, 0x68, 0x34, 0x58, 0x9B, 0x78, 0xA2, 0xF1, 0x83, 0x92, 0x2D, 0x55, 0x7D, 0xD8, 0xB1,
	0xB4, 0xB8, 0xA1, 0xD0, 0x91, 0x80, 0xF2, 0x70, 0xF3, 0x70, 0xF2, 0x7C, 0x80, 0xA8, 0xF1, 0x01,
	0xB0, 0x98, 0x87, 0xD9, 0x43, 0xD8, 0x86, 0xC9, 0x88, 0xBA, 0xA1, 0xF2, 0x0E, 0xB8, 0x97, 0x80,
	0xF1, 0xA9, 0xDF, 0xDF, 0xDF, 0xAA, 0xDF, 0xDF, 0xDF, 0xF2, 0xAA, 0xC5, 0xCD, 0xC7, 0xA9, 0x0C,
	0xC9, 0x2C, 0x97, 0x97, 0x97, 0x97, 0xF1, 0xA9, 0x89, 0x26, 0x46, 0x66, 0xB0, 0xB4, 0xBA, 0x80,
	0xAC, 0xDE, 0xF2, 0xCA, 0xF1, 0xB2, 0x8C, 0x02, 0xA9, 0xB6, 0x98, 0x00, 0x89, 0x0E, 0x16, 0x1E,
	0xB8, 0xA9, 0xB4, 0x99, 0x2C, 0x54, 0x7C, 0xB0, 0x8A, 0xA8, 0x96, 0x36, 0x56, 0x76, 0xF1, 0xB9,
	0xAF, 0xB4, 0xB0, 0x83, 0xC0, 0xB8, 0xA8, 0x97, 0x11, 0xB1, 0x8F, 0x98, 0xB9, 0xAF, 0xF0, 0x24,
	0x08, 0x44, 0x10, 0x64, 0x18, 0xF1, 0xA3, 0x29, 0x55, 0x7D, 0xAF, 0x83, 0xB5, 0x93, 0xAF, 0xF0,
	0x00, 0x28, 0x50, 0xF1, 0xA3, 0x86, 0x9F, 0x61, 0xA6, 0xDA, 0xDE, 0xDF, 0xD9, 0xFA, 0xA3, 0x86,
	0x96, 0xDB, 0x31, 0xA6, 0xD9, 0xF8, 0xDF, 0xBA, 0xA6, 0x8F, 0xC2, 0xC5, 0xC7, 0xB2, 0x8C, 0xC1,
	0xB8, 0xA2, 0xDF, 0xDF, 0xDF, 0xA3, 0xDF, 0xDF, 0xDF, 0xD8, 0xD8, 0xF1, 0xB8, 0xA8, 0xB2, 0x86,
	// bank 4, 256 bytes
	0xB4, 0x98, 0x0D, 0x35, 0x5D, 0xB8, 0xAA, 0x98, 0xB0, 0x87, 0x2D, 0x35, 0x3D, 0xB2, 0xB6, 0xBA,
	0xAF, 0x8C, 0x96, 0x19, 0x8F, 0x9F, 0xA7, 0x0E, 0x16, 0x1E, 0xB4, 0x9A, 0xB8, 0xAA, 0x87, 0x2C,
	0x54, 0x7C, 0xB9, 0xA3, 0xDE, 0xDF, 0xDF, 0xA3, 0xB1, 0x80, 0xF2, 0xC4, 0xCD, 0xC9, 0xF1, 0xB8,
	0xA9, 0xB4, 0x99, 0x83, 0x0D, 0x35, 0x5D, 0x89, 0xB9, 0xA3, 0x2D, 0x55, 0x7D, 0xB5, 0x93, 0xA3,
	0x0E, 0x16, 0x1E, 0xA9, 0x2C, 0x54, 0x7C, 0xB8, 0xB4, 0xB0, 0xF1, 0x97, 0x83, 0xA8, 0x11, 0x84,
	0xA5, 0x09, 0x98, 0xA3, 0x83, 0xF0, 0xDA, 0x24, 0x08, 0x44, 0x10, 0x64, 0x18, 0xD8, 0xF1, 0xA5,
	0x29, 0x55, 0x7D, 0xA5, 0x85, 0x95, 0x02, 0x1A, 0x2E, 0x3A, 0x56, 0x5A, 0x40, 0x48, 0xF9, 0xF3,
	0xA3, 0xD9, 0xF8, 0xF0, 0x98, 0x83, 0x24, 0x08, 0x44, 0x10, 0x64, 0x18, 0x97, 0x82, 0xA8, 0xF1,
	0x11, 0xF0, 0x98, 0xA2, 0x24, 0x08, 0x44, 0x10, 0x64, 0x18, 0xDA, 0xF3, 0xDE, 0xD8, 0x83, 0xA5,
	0x94, 0x01, 0xD9, 0xA3, 0x02, 0xF1, 0xA2, 0xC3, 0xC5, 0xC7, 0xD8, 0xF1, 0x84, 0x92, 0xA2, 0x4D,
	0xDA, 0x2A, 0xD8, 0x48, 0x69, 0xD9, 0x2A, 0xD8, 0x68, 0x55, 0xDA, 0x32, 0xD8, 0x50, 0x71, 0xD9,
	0x32, 0xD8, 0x70, 0x5D, 0xDA, 0x3A, 0xD8, 0x58, 0x79, 0xD9, 0x3A, 0xD8, 0x78, 0x93, 0xA3, 0x4D,
	0xDA, 0x2A, 0xD8, 0x48, 0x69, 0xD9, 0x2A, 0xD8, 0x68, 0x55, 0xDA, 0x32, 0xD8, 0x50, 0x71, 0xD9,
	0x32, 0xD8, 0x70, 0x5D, 0xDA, 0x3A, 0xD8, 0x58, 0x79, 0xD9, 0x3A, 0xD8, 0x78, 0xA8, 0x8A, 0x9A,
	0xF0, 0x28, 0x50, 0x78, 0x9E, 0xF3, 0x88, 0x18, 0xF1, 0x9F, 0x1D, 0x98, 0xA8, 0xD9, 0x08, 0xD8,
	0xC8, 0x9F, 0x12, 0x9E, 0xF3, 0x15, 0xA8, 0xDA, 0x12, 0x10, 0xD8, 0xF1, 0xAF, 0xC8, 0x97, 0x87,
	// bank 5, 256 bytes
	0x34, 0xB5, 0xB9, 0x94, 0xA4, 0x21, 0xF3, 0xD9, 0x22, 0xD8, 0xF2, 0x2D, 0xF3, 0xD9, 0x2A, 0xD8,
	0xF2, 0x35, 0xF3, 0xD9, 0x32, 0xD8, 0x81, 0xA4, 0x60, 0x60, 0x61, 0xD9, 0x61, 0xD8, 0x6C, 0x68,
	0x69, 0xD9, 0x69, 0xD8, 0x74, 0x70, 0x71, 0xD9, 0x71, 0xD8, 0xB1, 0xA3, 0x84, 0x19, 0x3D, 0x5D,
	0xA3, 0x83, 0x1A, 0x3E, 0x5E, 0x93, 0x10, 0x30, 0x81, 0x10, 0x11, 0xB8, 0xB0, 0xAF, 0x8F, 0x94,
	0xF2, 0xDA, 0x3E, 0xD8, 0xB4, 0x9A, 0xA8, 0x87, 0x29, 0xDA, 0xF8, 0xD8, 0x87, 0x9A, 0x35, 0xDA,
	0xF8, 0xD8, 0x87, 0x9A, 0x3D, 0xDA, 0xF8, 0xD8, 0xB1, 0xB9, 0xA4, 0x98, 0x85, 0x02, 0x2E, 0x56,
	0xA5, 0x81, 0x00, 0x0C, 0x14, 0xA3, 0x97, 0xB0, 0x8A, 0xF1, 0x2D, 0xD9, 0x28, 0xD8, 0x4D, 0xD9,
	0x48, 0xD8, 0x6D, 0xD9, 0x68, 0xD8, 0xB1, 0x84, 0x0D, 0xDA, 0x0E, 0xD8, 0xA3, 0x29, 0x83, 0xDA,
	0x2C, 0x0E, 0xD8, 0xA3, 0x84, 0x49, 0x83, 0xDA, 0x2C, 0x4C, 0x0E, 0xD8, 0xB8, 0xB0, 0xA8, 0x8A,
	0x9A, 0xF5, 0x20, 0xAA, 0xDA, 0xDF, 0xD8, 0xA8, 0x40, 0xAA, 0xD0, 0xDA, 0xDE, 0xD8, 0xA8, 0x60,
	0xAA, 0xDA, 0xD0, 0xDF, 0xD8, 0xF1, 0x97, 0x86, 0xA8, 0x31, 0x9B, 0x06, 0x99, 0x07, 0xAB, 0x97,
	0x28, 0x88, 0x9B, 0xF0, 0x0C, 0x20, 0x14, 0x40, 0xB8, 0xB0, 0xB4, 0xA8, 0x8C, 0x9C, 0xF0, 0x04,
	0x28, 0x51, 0x79, 0x1D, 0x30, 0x14, 0x38, 0xB2, 0x82, 0xAB, 0xD0, 0x98, 0x2C, 0x50, 0x50, 0x78,
	0x78, 0x9B, 0xF1, 0x1A, 0xB0, 0xF0, 0x8A, 0x9C, 0xA8, 0x29, 0x51, 0x79, 0x8B, 0x29, 0x51, 0x79,
	0x8A, 0x24, 0x70, 0x59, 0x8B, 0x20, 0x58, 0x71, 0x8A, 0x44, 0x69, 0x38, 0x8B, 0x39, 0x40, 0x68,
	0x8A, 0x64, 0x48, 0x31, 0x8B, 0x30, 0x49, 0x60, 0xA5, 0x88, 0x20, 0x09, 0x71, 0x58, 0x44, 0x68,
	// bank 6, 256 bytes
	0x11, 0x39, 0x64, 0x49, 0x30, 0x19, 0xF1, 0xAC, 0x00, 0x2C, 0x54, 0x7C, 0xF0, 0x8C, 0xA8, 0x04,
	0x28, 0x50, 0x78, 0xF1, 0x88, 0x97, 0x26, 0xA8, 0x59, 0x98, 0xAC, 0x8C, 0x02, 0x26, 0x46, 0x66,
	0xF0, 0x89, 0x9C, 0xA8, 0x29, 0x51, 0x79, 0x24, 0x70, 0x59, 0x44, 0x69, 0x38, 0x64, 0x48, 0x31,
	0xA9, 0x88, 0x09, 0x20, 0x59, 0x70, 0xAB, 0x11, 0x38, 0x40, 0x69, 0xA8, 0x19, 0x31, 0x48, 0x60,
	0x8C, 0xA8, 0x3C, 0x41, 0x5C, 0x20, 0x7C, 0x00, 0xF1, 0x87, 0x98, 0x19, 0x86, 0xA8, 0x6E, 0x76,
	0x7E, 0xA9, 0x99, 0x88, 0x2D, 0x55, 0x7D, 0x9E, 0xB9, 0xA3, 0x8A, 0x22, 0x8A, 0x6E, 0x8A, 0x56,
	0x8A, 0x5E, 0x9F, 0xB1, 0x83, 0x06, 0x26, 0x46, 0x66, 0x0E, 0x2E, 0x4E, 0x6E, 0x9D, 0xB8, 0xAD,
	0x00, 0x2C, 0x54, 0x7C, 0xF2, 0xB1, 0x8C, 0xB4, 0x99, 0xB9, 0xA3, 0x2D, 0x55, 0x7D, 0x81, 0x91,
	0xAC, 0x38, 0xAD, 0x3A, 0xB5, 0x83, 0x91, 0xAC, 0x2D, 0xD9, 0x28, 0xD8, 0x4D, 0xD9, 0x48, 0xD8,
	0x6D, 0xD9, 0x68, 0xD8, 0x8C, 0x9D, 0xAE, 0x29, 0xD9, 0x04, 0xAE, 0xD8, 0x51, 0xD9, 0x04, 0xAE,
	0xD8, 0x79, 0xD9, 0x04, 0xD8, 0x81, 0xF3, 0x9D, 0xAD, 0x00, 0x8D, 0xAE, 0x19, 0x81, 0xAD, 0xD9,
	0x01, 0xD8, 0xF2, 0xAE, 0xDA, 0x26, 0xD8, 0x8E, 0x91, 0x29, 0x83, 0xA7, 0xD9, 0xAD, 0xAD, 0xAD,
	0xAD, 0xF3, 0x2A, 0xD8, 0xD8, 0xF1, 0xB0, 0xAC, 0x89, 0x91, 0x3E, 0x5E, 0x76, 0xF3, 0xAC, 0x2E,
	0x2E, 0xF1, 0xB1, 0x8C, 0x5A, 0x9C, 0xAC, 0x2C, 0x28, 0x28, 0x28, 0x9C, 0xAC, 0x30, 0x18, 0xA8,
	0x98, 0x81, 0x28, 0x34, 0x3C, 0x97, 0x24, 0xA7, 0x28, 0x34, 0x3C, 0x9C, 0x24, 0xF2, 0xB0, 0x89,
	0xAC, 0x91, 0x2C, 0x4C, 0x6C, 0x8A, 0x9B, 0x2D, 0xD9, 0xD8, 0xD8, 0x51, 0xD9, 0xD8, 0xD8, 0x79,
	// bank 7, 138 bytes (remainder)
	0xD9, 0xD8, 0xD8, 0xF1, 0x9E, 0x88, 0xA3, 0x31, 0xDA, 0xD8, 0xD8, 0x91, 0x2D, 0xD9, 0x28, 0xD8,
	0x4D, 0xD9, 0x48, 0xD8, 0x6D, 0xD9, 0x68, 0xD8, 0xB1, 0x83, 0x93, 0x35, 0x3D, 0x80, 0x25, 0xDA,
	0xD8, 0xD8, 0x85, 0x69, 0xDA, 0xD8, 0xD8, 0xB4, 0x93, 0x81, 0xA3, 0x28, 0x34, 0x3C, 0xF3, 0xAB,
	0x8B, 0xF8, 0xA3, 0x91, 0xB6, 0x09, 0xB4, 0xD9, 0xAB, 0xDE, 0xFA, 0xB0, 0x87, 0x9C, 0xB9, 0xA3,
	0xDD, 0xF1, 0xA3, 0xA3, 0xA3, 0xA3, 0x95, 0xF1, 0xA3, 0xA3, 0xA3, 0x9D, 0xF1, 0xA3, 0xA3, 0xA3,
	0xA3, 0xF2, 0xA3, 0xB4, 0x90, 0x80, 0xF2, 0xA3, 0xA3, 0xA3, 0xA3, 0xA3, 0xA3, 0xA3, 0xA3, 0xA3,
	0xA3, 0xB2, 0xA3, 0xA3, 0xA3, 0xA3, 0xA3, 0xA3, 0xB0, 0x87, 0xB5, 0x99, 0xF1, 0xA3, 0xA3, 0xA3,
	0x98, 0xF1, 0xA3, 0xA3, 0xA3, 0xA3, 0x97, 0xA3, 0xA3, 0xA3, 0xA3, 0xF3, 0x9B, 0xA3, 0xA3, 0xDC,
	0xB9, 0xA7, 0xF1, 0x26, 0x26, 0x26, 0xD8, 0xD8, 0xFF
};

 uint8_t dmpcfgupddata[192] = 
{
//  dmp config 
//  BANK    OFFSET  LENGTH  [DATA]
	0x03,   0x7B,   0x03,   0x4C, 0xCD, 0x6C,
	0x03,   0xAB,   0x03,   0x36, 0x56, 0x76,
	0x00,   0x68,   0x04,   0x02, 0xCB, 0x47, 0xA2,
	0x02,   0x18,   0x04,   0x00, 0x05, 0x8B, 0xC1,
	0x01,   0x0C,   0x04,   0x00, 0x00, 0x00, 0x00,
	0x03,   0x7F,   0x06,   0x0C, 0xC9, 0x2C, 0x97, 0x97, 0x97,
	0x03,   0x89,   0x03,   0x26, 0x46, 0x66,
	0x00,   0x6C,   0x02,   0x20, 0x00,
	0x02,   0x40,   0x04,   0x00, 0x00, 0x00, 0x00,
	0x02,   0x44,   0x04,   0x00, 0x00, 0x00, 0x00,
	0x02,   0x48,   0x04,   0x00, 0x00, 0x00, 0x00,
	0x02,   0x4C,   0x04,   0x00, 0x00, 0x00, 0x00,
	0x02,   0x50,   0x04,   0x00, 0x00, 0x00, 0x00,
	0x02,   0x54,   0x04,   0x00, 0x00, 0x00, 0x00,
	0x02,   0x58,   0x04,   0x00, 0x00, 0x00, 0x00,
	0x02,   0x5C,   0x04,   0x00, 0x00, 0x00, 0x00,
	0x02,   0xBC,   0x04,   0x00, 0x00, 0x00, 0x00,
	0x01,   0xEC,   0x04,   0x00, 0x00, 0x40, 0x00,
	0x03,   0x7F,   0x06,   0x0C, 0xC9, 0x2C, 0x97, 0x97, 0x97,
	0x04,   0x02,   0x03,   0x0D, 0x35, 0x5D,
	0x04,   0x09,   0x04,   0x87, 0x2D, 0x35, 0x3D,
	0x00,   0xA3,   0x01,   0x00,
	0x00,   0x00,   0x00,   0x01, 	//这里是开启DMP的特殊中断的
	//原程序中此行代码为(这里不一定错)
	//0x00,   0x00,   0x00,   0x01,  即LENGTH=0x00，有错
	0x07,   0x86,   0x01,   0xFE,
	0x07,   0x41,   0x05,   0xF1, 0x20, 0x28, 0x30, 0x38,
	0x07,   0x7E,   0x01,   0x30,
	0x07,   0x46,   0x01,   0x9A,
	0x07,   0x47,   0x04,   0xF1, 0x28, 0x30, 0x38,
	0x07,   0x6C,   0x04,   0xF1, 0x28, 0x30, 0x38,
	0x02,   0x16,   0x02,   0x00, 0x03,
	/* 上行最后一个数据调整FIFO rate :0x01=100HZ,0x02=66HZ,0x03=50HZ ,0x04=40HZ,0x05=33.33HZ,
	// 可从 datasheet 公式推算
	//dmp updates
	0x01,   0xB2,   0x02,   0xFF, 0xFF,
	0x01,   0x90,   0x04,   0x09, 0x23, 0xA1, 0x35,
	0x01,   0x6A,   0x02,   0x06, 0x00,
	0x01,   0x60,   0x08,   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00,   0x60,   0x04,   0x40, 0x00, 0x00, 0x00,
	0x01,   0x62,   0x02,   0x00, 0x00,
	0x00,   0x60,   0x04,   0x00, 0x40, 0x00, 0x00*/
};

 uint8_t dmpUpdates[47] = 
{
	0x01,   0xB2,   0x02,   0xFF, 0xFF,
	0x01,   0x90,   0x04,   0x09, 0x23, 0xA1, 0x35,
	0x01,   0x6A,   0x02,   0x06, 0x00,
	0x01,   0x60,   0x08,   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00,   0x60,   0x04,   0x40, 0x00, 0x00, 0x00,
	0x01,   0x62,   0x02,   0x00, 0x00,
	0x00,   0x60,   0x04,   0x00, 0x40, 0x00, 0x00

};
void MPU_Delay_Ms(u32 ms)
{
	delay_ms(ms);
}
/*
加载 DMP代码到
返回值  (1=成功,0=失败)
*/
uint8_t loadfirmware(void)
{
	uint16_t datanum = 0;	//DMP固件写入标志位
	uint8_t ye, i/*, j*/;
	uint8_t bank = 0;	//段（256个数据一段）
	uint8_t addr = 0;

	for (; bank<8; bank++)
	{
		if (bank == 7)	//这里的作用就是区分最后一段数据
			i = 8;
		else
			i = 16;
		for (ye = 0; ye<i; ye++)
		{
			MPU_Write_Byte(0x6d, bank);
			MPU_Write_Byte(0x6e, addr);
			MPU_Write_Len(MPU_ADDR, 0x6f, 16, dmpmemorydata + datanum);
			datanum += 16;
			addr += 16;
		}
	}
	MPU_Write_Byte(0x6d, 7);
	MPU_Write_Byte(0x6e, addr);
	MPU_Write_Len(MPU_ADDR, 0x6f, 9, dmpmemorydata + datanum);
	datanum += 9;
	return 1;
}

uint8_t loadcfgupd(void)	//DMP设置
{
	uint8_t line;	//一共需要写入30条设置数据
	uint8_t bank;	//页
	uint8_t datacounts = 0;	//DMP设置数据标志位
	uint8_t bytes2write;	//数据长度。
	uint8_t offset;	//偏移地址
	uint8_t writingcounts;	//数据写入标志与bytes2write一同使用
	uint8_t special;

	for (line = 0; line<30; line++)
	{
		bank = dmpcfgupddata[datacounts++];
		offset = dmpcfgupddata[datacounts++];
		bytes2write = dmpcfgupddata[datacounts++];
		MPU_Write_Byte(0x6d, bank);
		MPU_Write_Byte(0x6e, offset);
		MPU_Write_Len(MPU_ADDR, 0x6f, bytes2write, dmpcfgupddata + datacounts);
		writingcounts = bytes2write;
		datacounts += bytes2write;
		if (0 == bytes2write)
		{
			special = dmpcfgupddata[datacounts++];
			if (0x01 == special)
			{
				//设置零运动中断启用（真）;
				//设置FIFO缓冲区溢出启用（真）;
				//设置DMP启用（真）;
				MPU_Write_Byte(MPU_INT_EN_REG, 0x32);
			}
			else
				return 0;
		}
	}
	return 1;
}

/*最后更新DMP*/
uint8_t xdmpUpdates(uint8_t datacounts)
{
	uint8_t writingcounts, bank, offset, bytes2write;
	bank = dmpUpdates[datacounts++];
	offset = dmpUpdates[datacounts++];
	bytes2write = dmpUpdates[datacounts++];
	MPU_Write_Byte(0x6d, bank);
	MPU_Write_Byte(0x6e, offset);
	MPU_Write_Len(MPU_ADDR, 0x6f, bytes2write, dmpUpdates + datacounts);
	writingcounts = bytes2write;
	datacounts += bytes2write;
	return 1;
}

/*读取 FIFO 计数*/
uint16_t getFIFOCount()
{
	uint8_t i[2];
	MPU_Read_Len(MPU_ADDR, MPU_FIFO_CNTH_REG, 2, i);
	return ((i[0] << 8) + i[1]);
}

/*FIFO数据读取
参数 *Data	存储数据的地址
返回值 (1=读取成功,0读取失败)
*/
uint8_t readdmp(uint8_t *Data)
{
	return !MPU_Read_Len(MPU_ADDR, MPU_FIFO_RW_REG, 42, Data);
}

//加载并配置 DMP 数字运动处理引擎
uint8_t dmpInitialize(void)
{
	uint8_t hwRevision, otpValid, mpuIntStatus/*fifoBuffer[128]*/;
	uint8_t xgOffsetTC, ygOffsetTC, zgOffsetTC;
	uint16_t fifoCount;
	MPU_Write_Bit(MPU_PWR_MGMT1_REG, 7, 1);	//复位 MPU6050
	MPU_Delay_Ms(30);
	MPU_Write_Bit(MPU_PWR_MGMT1_REG, 6, 0);	//禁止睡眠模式
	MPU_Write_Byte(0x6D, 0x70);	//写入一个字节数据到0x6d寄存器(选择用户 bank)
	MPU_Write_Byte(0x6E, 0x06);	//写入一个字节数据到0x6e寄存器(选择存储字节)
	MPU_Read_Len(MPU_ADDR, 0x6F, 1, &hwRevision);	//读取 
	MPU_Write_Byte(0x6D, 0);	//重置内存 bank 选择
	MPU_Read_Bit(0x00, 0, &otpValid);	//读取 OTP bank 有效标志
	MPU_Read_Bits(0x00, 6, 6, &xgOffsetTC);	//读陀螺偏置TC值 X
	MPU_Read_Bits(0x01, 6, 6, &ygOffsetTC);	//读陀螺偏置TC值 Y)
	MPU_Read_Bits(0x02, 6, 6, &zgOffsetTC);	//读陀螺偏置TC值 Z
	//MPU_Write_Byte(MPU_I2CSLV0_ADDR_REG,0x7f);	//设置从0地址 0x7
	//MPU_Write_Bit(MPU_USER_CTRL_REG,5,0);	//禁用I2C主模式
	//MPU_Write_Byte(MPU_I2CSLV0_ADDR_REG,0x68);	//这里可能要改。还没有弄明白这里
	MPU_Write_Bit(MPU_USER_CTRL_REG, 1, 1);	//I2C总线主控复位
	MPU_Delay_Ms(20);
	if ((loadfirmware()) == 0) 
		return 0;	//加载 
	if ((loadcfgupd()) == 0) 
		return 0;	//配置DMP
	MPU_Write_Bits(MPU_PWR_MGMT1_REG, 2, 3, 0x03);	//设置时钟脉冲源Z陀螺
	MPU_Write_Byte(MPU_INT_EN_REG, 0x12);	//设置DMP和FIFO_OFLOW启用中断
	MPU_Write_Byte(MPU_SAMPLE_RATE_REG, 19);	//设置采样率为200 hz  (1khz / (1 + 4) = 200 Hz)
	MPU_Write_Bits(MPU_CFG_REG, 5, 3, 0x1);	//设置外部帧同步TEMP_OUT_L[0]
	MPU_Write_Bits(MPU_CFG_REG, 2, 3, 0x03);	//设置DLPF带宽42赫兹
	MPU_Write_Bits(MPU_GYRO_CFG_REG, 4, 2, 0x03);	//陀螺灵敏度设置为+ / - 2000 deg/sec
	MPU_Write_Byte(0x70, 0x03);	//设置DMP配置字节（功能未知）
	MPU_Write_Byte(0x71, 0x00);	//设置DMP配置字节（功能未知）
	MPU_Write_Bit(0x00, 0, 0);	//清除OTP Bank 标志
	MPU_Write_Bits(0x00, 6, 6, 0);	//设置X 陀螺抵消TCs之前的值
	MPU_Write_Bits(0x01, 6, 6, 0);	//设置Y 陀螺抵消TCs之前的值
	MPU_Write_Bits(0x02, 6, 6, 0);	//设置Z 陀螺抵消TCs之前的值
	if (xdmpUpdates(0) == 0) 
		return 0;	//最后更新1/7(函数未知)dmpUpdates数组第一行
	if (xdmpUpdates(5) == 0) 
		return 0;	//最后更新2/7(函数未知)dmpUpdates数组第二行
	MPU_Write_Bit(MPU_USER_CTRL_REG, 2, 1);	//复位 FIFO
	fifoCount = getFIFOCount();	//读取 FIFO 计数
								//readdmp(fifoCount,fifoBuffer);	//读取FIFO里的数据
	MPU_Write_Bit(MPU_USER_CTRL_REG, 2, 1);	//复位 FIFO

	MPU_Write_Byte(MPU_MOTION_DET_REG, 2);	//运动检测阈值设置为2
	MPU_Write_Byte(0x21, 156);	//零运动检测阈值为156
	MPU_Write_Byte(0x20, 80);	//设置运动检测持续时间至80
	MPU_Write_Byte(0x22, 0);	//设置零运动检测时间0
	MPU_Write_Bit(MPU_USER_CTRL_REG, 2, 1);	//复 位 FIFO
	MPU_Write_Bit(MPU_USER_CTRL_REG, 6, 1);	//使能 FIFO
	MPU_Write_Bit(MPU_USER_CTRL_REG, 7, 1);	//使能 DMP
	MPU_Write_Bit(MPU_USER_CTRL_REG, 3, 1);	//复位 DMP
	if (xdmpUpdates(12) == 0) 
		return 0; 	//最后更新3/7(函数未知)dmpUpdates数组第三行
	if (xdmpUpdates(17) == 0) 
		return 0;	//最后更新4/7(函数未知)dmpUpdates数组第四行
	if (xdmpUpdates(28) == 0) 
		return 0;	//最后更新5/7(函数未知)dmpUpdates数组第五行
	while ((fifoCount = getFIFOCount()) < 3);	//等待 FIFO 计数 > 2
	MPU_Write_Bit(MPU_USER_CTRL_REG, 2, 1);			//复位 FIFO
	//readdmp(fifoCount,fifoBuffer);	//读取FIFO里的数据
	MPU_Read_Len(MPU_ADDR, MPU_INT_STA_REG, 1, &mpuIntStatus);		//读取中断状态
	if (xdmpUpdates(35) == 0) 
		return 0;	//最后更新6/7(函数未知)dmpUpdates数组第六行
	while ((fifoCount = getFIFOCount()) < 3);	//等待 FIFO 计数 > 2
	MPU_Write_Bit(MPU_USER_CTRL_REG, 2, 1);	//复位 FIFO
	//readdmp(fifoCount,fifoBuffer);	//读取FIFO里的数据
	MPU_Read_Len(MPU_ADDR, MPU_INT_STA_REG, 1, &mpuIntStatus);	//读取中断状态
	if (xdmpUpdates(40) == 0) 
		return 0;	//最后更新7/7(函数未知)dmpUpdates数组第七行
	MPU_Write_Bit(MPU_USER_CTRL_REG, 7, 0);	//禁用DMP(稍后您打开它)
	MPU_Write_Bit(MPU_USER_CTRL_REG, 2, 1);	//复位 FIFO
	MPU_Read_Len(MPU_ADDR, MPU_INT_STA_REG, 1, &mpuIntStatus);
	//星期六 (2014/06/28)
	return 1;
}

/*初始化MPU6050*/
void initMPU6050(void)
{
		extern void MPU_IIC_Init();			
	MPU_IIC_Init();			
	MPU_Write_Bits(MPU_PWR_MGMT1_REG, 2, 3, 0x01);	//电源管理
	MPU_Write_Bits(MPU_GYRO_CFG_REG, 4, 2, 0x00);	//设置陀螺仪量程 250/s
	MPU_Write_Bits(MPU_ACCEL_CFG_REG, 4, 2, 0x00);	//设置加速度量程 2G
	MPU_Write_Bit(MPU_PWR_MGMT1_REG, 6, 1);	//电源管理MUP进入睡眠模式
}

/*验证MPU6050连接*/
uint8_t getDeviceID(void)
{
	uint8_t b = 0;	//临时变量
	MPU_Read_Bits(MPU_DEVICE_ID_REG, 6, 6, &b);	//读取i2c固定地址，去掉最高位和最低位这两位数据
	return b == 0x34;	//判断B是否等于0x34，如果等于返回1，不等于返回0(库的是0x38)

}

void MPU6050_DMP_Init(void)
{
	initMPU6050();	//初始化
	if (getDeviceID())	//验证连接是否正常(读取MPU6050的I2C地址)
		while(!(dmpInitialize()));	//加载并配置运动库
	MPU_Write_Bit(MPU_USER_CTRL_REG, 2, 1);	//复位 FIFO
	MPU_Write_Bit(MPU_USER_CTRL_REG, 7, 1);	//使能DMP
}
float a[3];
void MPU6050_Refresh_DMP(void)
{
	static uint8_t zd;
	static uint16_t i;
	i = getFIFOCount();//读取FIFO计数
	MPU_Read_Len(MPU_ADDR, MPU_INT_STA_REG, 1, &zd);	//读取中断状态
	if ((zd & 0x10) || i >= 840)	//判断FIFO是否溢出
	{
		MPU_Write_Bit(MPU_USER_CTRL_REG, 2, 1);	//复位 FIFO
	}
	else if (zd & 0x02)
	{
		while (i<42) 
			i = getFIFOCount();
		if (readdmp(dmpdatas))	//读取FIFO数据
		{
			Q[0] = ((int16_t)dmpdatas[0] << 8 | dmpdatas[1])/16384.;
			Q[1] = ((int16_t)dmpdatas[4] << 8 | dmpdatas[5])/16384.;
			Q[2] = ((int16_t)dmpdatas[8] << 8 | dmpdatas[9])/16384.;
			Q[3] = ((int16_t)dmpdatas[12] << 8 | dmpdatas[13])/16384.;
			
			short ax=((int16_t)dmpdatas[28] << 8 | dmpdatas[29]); 
			short ay=((int16_t)dmpdatas[32] << 8 | dmpdatas[33]);
			short az=((int16_t)dmpdatas[36] << 8 | dmpdatas[37]);			
			
			a[0]=((float)ax) / 16384 * 9.8;
			a[1]=((float)ay) / 16384 * 9.8;
			a[2]=((float)az) / 16384 * 9.8;

			
		}
	}
}

