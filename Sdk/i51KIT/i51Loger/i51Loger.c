
#include "i51KitAde.h"
#include "i51Loger.h"

i51KIT_STATIC_KIT_DEF(Adapter);

#define EX_FONT_CHAR(value) (value)
#define EX_FONT_UNICODE_VAL(value) (value),
#define EX_FONT_ANSI_VAL(value) ,

#define I51_EXPLORER_APP_COUNT	40
#define I51_EXPLORER_APPNAME_LEN	32

iU16 i51SimplePortalAppnameList[I51_EXPLORER_APP_COUNT][I51_EXPLORER_APPNAME_LEN] = {0} ;
iU16 i51SimplePortalAppCount = 0;
struct {

	//	author : Jelo 
	//	12x12 字模数据
	 
	unsigned short value; 
	unsigned char data[14];

} AnsiFont7x14[94] = 
{
	{
		EX_FONT_CHAR('!')
			EX_FONT_ANSI_VAL(0x0021)
		{0x00, 0x00, 0x00, 0x00, 0x10, 0x10, 0x10, 0x10, 0x10, 0x00, 0x10, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('\"')
			EX_FONT_ANSI_VAL(0x0022)
		{0x00, 0x00, 0x00, 0x00, 0x28, 0x28, 0x28, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('#')
			EX_FONT_ANSI_VAL(0x0023)
		{0x00, 0x00, 0x00, 0x00, 0x14, 0x28, 0x7c, 0x28, 0x28, 0x7c, 0x28, 0x50, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('$')
			EX_FONT_ANSI_VAL(0x0024)
		{0x00, 0x00, 0x00, 0x00, 0x10, 0x38, 0x40, 0x30, 0x08, 0x70, 0x20, 0x20, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('%')
			EX_FONT_ANSI_VAL(0x0025)
		{0x00, 0x00, 0x00, 0x00, 0x20, 0x50, 0x20, 0x78, 0x10, 0x28, 0x10, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('&')
			EX_FONT_ANSI_VAL(0x0026)
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x20, 0x20, 0x58, 0x48, 0x3c, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('\'')
			EX_FONT_ANSI_VAL(0x0027)
		{0x00, 0x00, 0x00, 0x00, 0x10, 0x10, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('(')
			EX_FONT_ANSI_VAL(0x0028)
		{0x00, 0x00, 0x00, 0x00, 0x08, 0x08, 0x10, 0x10, 0x10, 0x10, 0x08, 0x08, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR(')')
			EX_FONT_ANSI_VAL(0x0029)
		{0x00, 0x00, 0x00, 0x00, 0x20, 0x20, 0x10, 0x10, 0x10, 0x10, 0x20, 0x20, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('*')
			EX_FONT_ANSI_VAL(0x002a)
		{0x00, 0x00, 0x00, 0x00, 0x10, 0x7c, 0x10, 0x28, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('+')
			EX_FONT_ANSI_VAL(0x002b)
		{0x00, 0x00, 0x00, 0x00, 0x10, 0x10, 0x10, 0x7c, 0x10, 0x10, 0x10, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR(',')
			EX_FONT_ANSI_VAL(0x002c)
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x30, 0x20, 0x00}
	},
	{
		EX_FONT_CHAR('-')
			EX_FONT_ANSI_VAL(0x002d)
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('.')
			EX_FONT_ANSI_VAL(0x002e)
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('/')
			EX_FONT_ANSI_VAL(0x002f)
		{0x00, 0x00, 0x00, 0x00, 0x04, 0x08, 0x08, 0x10, 0x10, 0x20, 0x20, 0x40, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('0')
			EX_FONT_ANSI_VAL(0x0030)
		{0x00, 0x00, 0x00, 0x00, 0x38, 0x44, 0x44, 0x44, 0x44, 0x44, 0x38, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('1')
			EX_FONT_ANSI_VAL(0x0031)
		{0x00, 0x00, 0x00, 0x00, 0x30, 0x10, 0x10, 0x10, 0x10, 0x10, 0x7c, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('2')
			EX_FONT_ANSI_VAL(0x0032)
		{0x00, 0x00, 0x00, 0x00, 0x38, 0x44, 0x08, 0x10, 0x20, 0x40, 0x7c, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('3')
			EX_FONT_ANSI_VAL(0x0033)
		{0x00, 0x00, 0x00, 0x00, 0x38, 0x44, 0x04, 0x18, 0x04, 0x44, 0x38, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('4')
			EX_FONT_ANSI_VAL(0x0034)
		{0x00, 0x00, 0x00, 0x00, 0x08, 0x18, 0x28, 0x48, 0x7c, 0x08, 0x1c, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('5')
			EX_FONT_ANSI_VAL(0x0035)
		{0x00, 0x00, 0x00, 0x00, 0x3c, 0x20, 0x20, 0x38, 0x04, 0x44, 0x38, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('6')
			EX_FONT_ANSI_VAL(0x0036)
		{0x00, 0x00, 0x00, 0x00, 0x1c, 0x20, 0x40, 0x78, 0x44, 0x44, 0x38, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('7')
			EX_FONT_ANSI_VAL(0x0037)
		{0x00, 0x00, 0x00, 0x00, 0x7c, 0x44, 0x04, 0x08, 0x08, 0x10, 0x10, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('8')
			EX_FONT_ANSI_VAL(0x0038)
		{0x00, 0x00, 0x00, 0x00, 0x38, 0x44, 0x44, 0x38, 0x44, 0x44, 0x38, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('9')
			EX_FONT_ANSI_VAL(0x0039)
		{0x00, 0x00, 0x00, 0x00, 0x38, 0x44, 0x44, 0x3c, 0x04, 0x08, 0x70, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR(':')
			EX_FONT_ANSI_VAL(0x003a)
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR(';')
			EX_FONT_ANSI_VAL(0x003b)
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x30, 0x20, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('<')
			EX_FONT_ANSI_VAL(0x003c)
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x18, 0x60, 0x18, 0x04, 0x00, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('=')
			EX_FONT_ANSI_VAL(0x003d)
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7c, 0x00, 0x7c, 0x00, 0x00, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('>')
			EX_FONT_ANSI_VAL(0x003e)
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x30, 0x0c, 0x30, 0x40, 0x00, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('?')
			EX_FONT_ANSI_VAL(0x003f)
		{0x00, 0x00, 0x00, 0x00, 0x30, 0x48, 0x08, 0x10, 0x10, 0x00, 0x30, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('@')
			EX_FONT_ANSI_VAL(0x0040)
		{0x00, 0x00, 0x00, 0x00, 0x38, 0x44, 0x4c, 0x54, 0x54, 0x4c, 0x40, 0x3c, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('A')
			EX_FONT_ANSI_VAL(0x0041)
		{0x00, 0x00, 0x00, 0x00, 0x30, 0x10, 0x28, 0x28, 0x38, 0x44, 0xee, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('B')
			EX_FONT_ANSI_VAL(0x0042)
		{0x00, 0x00, 0x00, 0x00, 0xf8, 0x44, 0x44, 0x78, 0x44, 0x44, 0xf8, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('C')
			EX_FONT_ANSI_VAL(0x0043)
		{0x00, 0x00, 0x00, 0x00, 0x3c, 0x44, 0x40, 0x40, 0x40, 0x44, 0x38, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('D')
			EX_FONT_ANSI_VAL(0x0044)
		{0x00, 0x00, 0x00, 0x00, 0xf0, 0x48, 0x44, 0x44, 0x44, 0x48, 0xf0, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('E')
			EX_FONT_ANSI_VAL(0x0045)
		{0x00, 0x00, 0x00, 0x00, 0x7c, 0x24, 0x28, 0x38, 0x28, 0x24, 0x7c, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('F')
			EX_FONT_ANSI_VAL(0x0046)
		{0x00, 0x00, 0x00, 0x00, 0x7c, 0x24, 0x28, 0x38, 0x28, 0x20, 0x70, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('G')
			EX_FONT_ANSI_VAL(0x0047)
		{0x00, 0x00, 0x00, 0x00, 0x3c, 0x44, 0x40, 0x40, 0x4e, 0x44, 0x38, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('H')
			EX_FONT_ANSI_VAL(0x0048)
		{0x00, 0x00, 0x00, 0x00, 0xee, 0x44, 0x44, 0x7c, 0x44, 0x44, 0xee, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('I')
			EX_FONT_ANSI_VAL(0x0049)
		{0x00, 0x00, 0x00, 0x00, 0x7c, 0x10, 0x10, 0x10, 0x10, 0x10, 0x7c, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('J')
			EX_FONT_ANSI_VAL(0x004a)
		{0x00, 0x00, 0x00, 0x00, 0x3c, 0x08, 0x08, 0x08, 0x48, 0x48, 0x30, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('K')
			EX_FONT_ANSI_VAL(0x004b)
		{0x00, 0x00, 0x00, 0x00, 0xee, 0x44, 0x48, 0x70, 0x48, 0x44, 0xe6, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('L')
			EX_FONT_ANSI_VAL(0x004c)
		{0x00, 0x00, 0x00, 0x00, 0x70, 0x20, 0x20, 0x20, 0x24, 0x24, 0x7c, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('M')
			EX_FONT_ANSI_VAL(0x004d)
		{0x00, 0x00, 0x00, 0x00, 0xee, 0x6c, 0x6c, 0x54, 0x44, 0x44, 0xee, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('N')
			EX_FONT_ANSI_VAL(0x004e)
		{0x00, 0x00, 0x00, 0x00, 0xee, 0x64, 0x64, 0x54, 0x54, 0x4c, 0xec, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('O')
			EX_FONT_ANSI_VAL(0x004f)
		{0x00, 0x00, 0x00, 0x00, 0x38, 0x44, 0x44, 0x44, 0x44, 0x44, 0x38, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('P')
			EX_FONT_ANSI_VAL(0x0050)
		{0x00, 0x00, 0x00, 0x00, 0x78, 0x24, 0x24, 0x24, 0x38, 0x20, 0x70, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('Q')
			EX_FONT_ANSI_VAL(0x0051)
		{0x00, 0x00, 0x00, 0x00, 0x38, 0x44, 0x44, 0x44, 0x44, 0x44, 0x38, 0x1c, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('R')
			EX_FONT_ANSI_VAL(0x0052)
		{0x00, 0x00, 0x00, 0x00, 0x78, 0x24, 0x24, 0x24, 0x38, 0x24, 0x72, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('S')
			EX_FONT_ANSI_VAL(0x0053)
		{0x00, 0x00, 0x00, 0x00, 0x34, 0x4c, 0x40, 0x38, 0x04, 0x44, 0x78, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('T')
			EX_FONT_ANSI_VAL(0x0054)
		{0x00, 0x00, 0x00, 0x00, 0x7c, 0x54, 0x10, 0x10, 0x10, 0x10, 0x38, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('U')
			EX_FONT_ANSI_VAL(0x0055)
		{0x00, 0x00, 0x00, 0x00, 0xee, 0x44, 0x44, 0x44, 0x44, 0x44, 0x38, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('V')
			EX_FONT_ANSI_VAL(0x0056)
		{0x00, 0x00, 0x00, 0x00, 0xee, 0x44, 0x44, 0x48, 0x28, 0x28, 0x30, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('W')
			EX_FONT_ANSI_VAL(0x0057)
		{0x00, 0x00, 0x00, 0x00, 0xee, 0x44, 0x54, 0x54, 0x54, 0x54, 0x28, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('X')
			EX_FONT_ANSI_VAL(0x0058)
		{0x00, 0x00, 0x00, 0x00, 0xc6, 0x44, 0x28, 0x10, 0x28, 0x44, 0xc6, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('Y')
			EX_FONT_ANSI_VAL(0x0059)
		{0x00, 0x00, 0x00, 0x00, 0xee, 0x44, 0x28, 0x10, 0x10, 0x10, 0x38, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('Z')
			EX_FONT_ANSI_VAL(0x005a)
		{0x00, 0x00, 0x00, 0x00, 0x7c, 0x44, 0x08, 0x10, 0x20, 0x44, 0x7c, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('[')
			EX_FONT_ANSI_VAL(0x005b)
		{0x00, 0x00, 0x00, 0x00, 0x18, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x18, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('\\')
			EX_FONT_ANSI_VAL(0x005c)
		{0x00, 0x00, 0x00, 0x00, 0x40, 0x20, 0x20, 0x10, 0x10, 0x10, 0x08, 0x08, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR(']')
			EX_FONT_ANSI_VAL(0x005d)
		{0x00, 0x00, 0x00, 0x00, 0x30, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x30, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('^')
			EX_FONT_ANSI_VAL(0x005e)
		{0x00, 0x00, 0x00, 0x00, 0x10, 0x28, 0x44, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('_')
			EX_FONT_ANSI_VAL(0x005f)
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe}
	},
	{
		EX_FONT_CHAR('`')
			EX_FONT_ANSI_VAL(0x0060)
		{0x00, 0x00, 0x00, 0x00, 0x20, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('a')
			EX_FONT_ANSI_VAL(0x0061)
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x44, 0x3c, 0x44, 0x7e, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('b')
			EX_FONT_ANSI_VAL(0x0062)
		{0x00, 0x00, 0x00, 0x00, 0xc0, 0x40, 0x58, 0x64, 0x44, 0x44, 0xf8, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('c')
			EX_FONT_ANSI_VAL(0x0063)
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x44, 0x40, 0x40, 0x3c, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('d')
			EX_FONT_ANSI_VAL(0x0064)
		{0x00, 0x00, 0x00, 0x00, 0x0c, 0x04, 0x34, 0x4c, 0x44, 0x44, 0x3e, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('e')
			EX_FONT_ANSI_VAL(0x0065)
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x44, 0x7c, 0x40, 0x3c, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('f')
			EX_FONT_ANSI_VAL(0x0066)
		{0x00, 0x00, 0x00, 0x00, 0x0c, 0x10, 0x3c, 0x10, 0x10, 0x10, 0x3c, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('g')
			EX_FONT_ANSI_VAL(0x0067)
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x36, 0x4c, 0x44, 0x44, 0x3c, 0x04, 0x38, 0x00}
	},
	{
		EX_FONT_CHAR('h')
			EX_FONT_ANSI_VAL(0x0068)
		{0x00, 0x00, 0x00, 0x00, 0xc0, 0x40, 0x58, 0x64, 0x44, 0x44, 0xee, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('i')
			EX_FONT_ANSI_VAL(0x0069)
		{0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x70, 0x10, 0x10, 0x10, 0x7c, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('j')
			EX_FONT_ANSI_VAL(0x006a)
		{0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x78, 0x08, 0x08, 0x08, 0x08, 0x08, 0x70, 0x00}
	},
	{
		EX_FONT_CHAR('k')
			EX_FONT_ANSI_VAL(0x006b)
		{0x00, 0x00, 0x00, 0x00, 0xc0, 0x40, 0x5c, 0x48, 0x70, 0x48, 0xdc, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('l')
			EX_FONT_ANSI_VAL(0x006c)
		{0x00, 0x00, 0x00, 0x00, 0x30, 0x10, 0x10, 0x10, 0x10, 0x10, 0x7c, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('m')
			EX_FONT_ANSI_VAL(0x006d)
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe8, 0x54, 0x54, 0x54, 0xfe, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('n')
			EX_FONT_ANSI_VAL(0x006e)
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xd8, 0x64, 0x44, 0x44, 0xee, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('o')
			EX_FONT_ANSI_VAL(0x006f)
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x44, 0x44, 0x44, 0x38, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('p')
			EX_FONT_ANSI_VAL(0x0070)
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xd8, 0x64, 0x44, 0x44, 0x78, 0x40, 0xe0, 0x00}
	},
	{
		EX_FONT_CHAR('q')
			EX_FONT_ANSI_VAL(0x0071)
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x36, 0x4c, 0x44, 0x44, 0x3c, 0x04, 0x0e, 0x00}
	},
	{
		EX_FONT_CHAR('r')
			EX_FONT_ANSI_VAL(0x0072)
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x6c, 0x30, 0x20, 0x20, 0x78, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('s')
			EX_FONT_ANSI_VAL(0x0073)
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x40, 0x38, 0x04, 0x78, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('t')
			EX_FONT_ANSI_VAL(0x0074)
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0xf8, 0x40, 0x40, 0x44, 0x38, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('u')
			EX_FONT_ANSI_VAL(0x0075)
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xcc, 0x44, 0x44, 0x4c, 0x36, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('v')
			EX_FONT_ANSI_VAL(0x0076)
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xee, 0x44, 0x48, 0x28, 0x30, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('w')
			EX_FONT_ANSI_VAL(0x0077)
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xee, 0x44, 0x54, 0x54, 0x28, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('x')
			EX_FONT_ANSI_VAL(0x0078)
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x6c, 0x28, 0x10, 0x28, 0x6c, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('y')
			EX_FONT_ANSI_VAL(0x0079)
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xee, 0x44, 0x28, 0x28, 0x10, 0x10, 0x70, 0x00}
	},
	{
		EX_FONT_CHAR('z')
			EX_FONT_ANSI_VAL(0x007a)
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7c, 0x48, 0x10, 0x24, 0x7c, 0x00, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('{')
			EX_FONT_ANSI_VAL(0x007b)
		{0x00, 0x00, 0x00, 0x00, 0x08, 0x10, 0x10, 0x10, 0x30, 0x10, 0x10, 0x10, 0x08, 0x00}
	},
	{
		EX_FONT_CHAR('|')
			EX_FONT_ANSI_VAL(0x007c)
		{0x00, 0x00, 0x00, 0x00, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x00, 0x00}
	},
	{
		EX_FONT_CHAR('}')
			EX_FONT_ANSI_VAL(0x007d)
		{0x00, 0x00, 0x00, 0x00, 0x20, 0x10, 0x10, 0x10, 0x18, 0x10, 0x10, 0x10, 0x20, 0x00}
	},
	{
		EX_FONT_CHAR('~')
			EX_FONT_ANSI_VAL(0x007e)
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x34, 0x58, 0x00, 0x00, 0x00, 0x00, 0x00}
	}
};

static void CORENRDrawPoint ( 
	iU16* panel , 	
	iU16 panel_width ,
	iU16 panel_height ,
	iU32 desx ,
	iU32 desy ,
	iU16 color 
) { 

	if ( desx < 0 || desy < 0 ) return ;
	else if ( desx >= panel_width || desy >= panel_height ) return ;
	
	panel[desy*panel_width+desx] = color ;

}

static void CORENRDrawRect ( 
	iU16* panel , 	
	iU16 panel_width ,
	iU16 panel_height ,
	iU32 desx ,
	iU32 desy ,
	iU32 rect_w ,
	iU32 rect_h ,	
	iU16 color 
) { 

	iU32 x = desx ;
	iU32 y = desy ;
	
	iU32 xborder = x + rect_w ;
	iU32 yborder = y + rect_h ;

	for ( y = desy ; y < panel_height && y < yborder ; y ++ )
	{

		for ( x = desx ; x < panel_width && x < xborder ; x ++ )
		{
			CORENRDrawPoint ( panel , panel_width , panel_height , x , y , color ) ;
		}
	
	}

}

static void CORENRDrawTextModel  ( iS32 x , iS32 y , const iS8* words , iU16 color ) {

	//	author : Jelo Wang
	//	since : 20100802
	//	(C)TOK
	
	iS32 font_walker = 0;
	iS32 rowwalker = 0;
	iS32 colwalker = 0;
	iS32 wdlen = 0;
	iU32* panel = 0 ;

	//	14x14 点阵
	# define MATRIXSCALE 14

	if ( !words ) return ;

	wdlen = (MATRIXSCALE) / 8 ;

	panel = i51AdeMmiGetPanel () ;
		
	for( font_walker = 0; font_walker < MATRIXSCALE; font_walker ++ ) {
		for( rowwalker = 0 ;rowwalker < wdlen; rowwalker ++ ) {
	
			for(colwalker = 0; colwalker < 8; colwalker ++ ) {
				if(words[font_walker * wdlen + rowwalker] & (0x80 >> colwalker)) {
					CORENRDrawPoint ( (iU16*)panel[1] , (iU16)panel[2], (iU16)panel[3] , x+rowwalker*8+colwalker , y+font_walker , color ) ;
				}
			} 
		}
	}
		
}

static void* CORENRSearchTextModel ( iS8 element ) {

	//	author : Jelo Wang
	//	since : 20100802
	//	(C)TOK
	
	iS32 walker = 0 ;	

	for ( walker = 0 ; walker < 94 ; walker ++ ) {
		if ( EX_FONT_CHAR ( element ) == AnsiFont7x14 [ walker ].value ) 
			return AnsiFont7x14 [ walker ].data ;
	}
	return 0 ;

}

void CORENRDrawText ( iS32 x , iS32 y , const iS8* text , iU16 color ) {

	//	author : Jelo Wang
	//	since : 20100802
	//	(C)TOK

	iS32 walker = 0 ;
	
	static iU16 lcdwidth = 0 ;
	static iU16 lcdheight = 0 ;
	
	if ( 0 == lcdwidth || 0 == lcdheight ) {
		i51AdeMmiGetScreenScale ( &lcdwidth , &lcdheight ) ;
	}
	if ( !text ) return ;
	
	for ( ; text[walker] !='\0' && x < lcdwidth ; walker ++ ) {
		CORENRDrawTextModel ( x  , y , (iS8*)CORENRSearchTextModel( text[walker]) , color );
		x = x + 8 ;
	}

}
static iU8 ChangeMode()
{
	i51FILEINFO FileInfo = {0};
	iU8 mode = 0;
	iFILE file = 0;
#if 0
    file = i51AdeOsFileFind( L"i51TEST" , &FileInfo);
	if(i51AdeStdWStrlen(FileInfo.name))mode=1;
	i51AdeOsFileFindClose(file);
#else
	file = i51AdeOsFileOpen( L"i51TEST" ,i51_OS_FL_EXISTING_OPEN | i51_OS_FL_WRITE , __FILE__ , __LINE__ ) ;
	if(file!=0)
	{
		i51AdeOsFileClose ( file ) ;
		mode = 1;
	}
#endif
	if(mode)
	{
		i51AdeOsFileDelete(L"i51TEST");
		return 0;
	}
	else
	{
		file = i51AdeOsFileOpen( L"i51TEST" ,i51_OS_FL_ALWAYS_CREATE | i51_OS_FL_WRITE , __FILE__ , __LINE__ ) ;
		if(file!=0)i51AdeOsFileClose ( file ) ;
		return 1;
	}
}

static iBOOL i51SimplePortalTouchDo ( iU16 event , iU32 value ,iU32 height)
{

	iU16 i = 0 ;
	iU32 x= 0 , y = 0 ;
	iU32 PointX = 5;
	iU32 PointY = height;
	static iBOOL IsDown = iFALSE;

	x =  ( value >>16 ) & 0x0000ffff ;
	y = value & 0x0000ffff ;

	if( i51_MSG_PADDOWN == event)
	{
		if(x>PointX&&x<240&&y>PointY&&y<PointY+14)
		{
					
			IsDown = iTRUE ;
			return iTRUE;
		}	
	}
    if(IsDown && i51_MSG_PADUP == event ){

				IsDown = iFALSE ;
				return iTRUE;
	} 

	IsDown = iFALSE;
	return iFALSE;
}

iKIT iBOOL i51KitMain ( iS32 aid , iU32 message , void* parameter )
{

	//	TODO :	消息循环
	
	static iU32 drawtimes = 0 ;
	static iU32 PointY = 0;
     iS8 buffer [ 128 ] = { 0 } ;

	switch ( message )
	{
		case i51_MSG_PAINT :
		{
			iU8* sn = 0;
			iU8 pp[8] = {0};
			iU8 PrintDataBuff[4] = {0};
			iU32 *PrintDataP = (iU32 *)PrintDataBuff;
			iU8 *PrintData = NULL;
			iU8 snlen = 0 ;
			iS32* panel = 0 ;
			iS32 looper = 0 ;
			iS32* sims = 0 ;
			iS32 dx = 0 ;
			iU8 a,b,c;
			iS8 *p = 0;
			iU8 i = 0;
			i51FILEINFO FileInfo = {0};
			iU8 mode = 0;
			iFILE file = 0;
			
			if ( 0 == drawtimes ) {

				panel = (iS32* )i51AdeMmiGetPanel () ;
					
				i51AdeStdMemset16 ( (void*)panel[1] , 0 , panel[2]*panel[3]<<1 ) ;

				sprintf ( (iCHAR *)buffer , ":%s" , i51KernelGeti51Version () ) ;
				CORENRDrawText ( 5 , 32 , buffer , 0xffff ) ;

				sprintf ( (iCHAR *)buffer , "Kernel Version:%d" , i51KernelGetVersion () ) ;
				CORENRDrawText ( 5 , 32+14 , buffer , 0xffff ) ;

				sprintf ( (iCHAR *)buffer , "Adapter Version:%d" , i51AdeCnfGetVersion () ) ;
				CORENRDrawText ( 5 , 32+14+14 , buffer , 0xffff ) ;

				sprintf ( (iCHAR *)buffer , "Project ID:%d" , i51AdeCnfGetProjectId () ) ;
				CORENRDrawText ( 5 , 32+14+14+14 , buffer , 0xffff ) ;

				sprintf ( (iCHAR *)buffer , "Customer ID:%d" , i51AdeCnfGetCustomerId () ) ;
				CORENRDrawText ( 5 , 32+14+14+14+14 , buffer , 0xffff ) ;

				sprintf ( (iCHAR *)buffer , "Platform:%s" , i51AdeCnfGetPlatformId () ) ;
				CORENRDrawText ( 5 , 32+14+14+14+14+14 , buffer , 0xffff ) ;

				sprintf ( (iCHAR *)buffer , "Endian:%d" , i51AdeCnfGetEndian () ) ;
				CORENRDrawText ( 5 , 32+14+14+14+14+14+14 , buffer , 0xffff ) ;

				i51KernelGetSerialNumber ( (void*)&sn , (iU8*)&snlen ) ;
				p = buffer;
				sprintf ( (iCHAR *)buffer , "SN:") ;
				p = p + 3;
				for(i = 0;i<snlen;i++)
				{
					pp[i] = sn[7-i];
				}

				PrintData = (iU8 *)pp;
				for(i = 0;i<4;i++)
				{
					PrintDataBuff[i] = PrintData[3-i];
				}
				//i51AdeOsLog(0,"xr:%d",*PrintDataP);

				sprintf ( (iCHAR *)p , "%d" ,*PrintDataP) ;
				CORENRDrawText ( 5 , 32+14+14+14+14+14+14+14 , buffer , 0xffff ) ;
				
				sims = i51AdeSmsGetSims () ;
					
				dx = 32+14+14+14+14+14+14+14+14 ;

				for ( looper = 0 ; looper < sims[0]; looper ++ ) {
	
					sprintf ( (iCHAR *)buffer , "Imsi%d:%s" , looper+1 , i51AdeSmsGetImsi(sims[looper+1]) ) ;
					CORENRDrawText ( 5 , dx , buffer , 0xffff ) ;
					dx = dx + 14 ;
					
				}

				for ( looper = 0 ; looper < sims[0]; looper ++ ) {
	
					sprintf ( (iCHAR *)buffer , "Smsc%d:%s" , looper+1 , i51AdeSmsGetCenter (sims[looper+1]) ) ;
					CORENRDrawText ( 5 , dx , buffer , 0xffff ) ;
					dx = dx + 14 ;
					
				}

				sprintf ( (iCHAR *)buffer , "Imei:%s" , i51AdeSmsGetImei () ) ;
				CORENRDrawText ( 5 , dx , buffer , 0xffff ) ;
#if 0
				file = i51AdeOsFileFind( L"i51TEST" , &FileInfo);
				if(i51AdeStdWStrlen(FileInfo.name))mode=1;
				i51AdeOsFileFindClose(file);

#else
				file = i51AdeOsFileOpen( L"i51TEST" ,i51_OS_FL_EXISTING_OPEN | i51_OS_FL_WRITE , __FILE__ , __LINE__ ) ;
				if(file!=0)
				{
					i51AdeOsFileClose ( file ) ;
					mode = 1;
				}
#endif
				sprintf ( (iCHAR *)buffer , "9:Test Mode:%d",mode) ;
				CORENRDrawText ( 5 , dx+14 , buffer , 0xffff ) ;
                PointY = dx+14;
				i51AdeMmiUpdateScreen () ;
			}

			drawtimes = 0 ;

		}
		break ;
	
		case i51_MSG_KEYUP :
		{
			switch ( (iU32 ) parameter ) {
				case i51_KEY_NUM9 :
			    sprintf ( (iCHAR *)buffer , "9:Test Mode:%d",ChangeMode()) ;
				CORENRDrawText ( 5 , PointY , buffer , 0xffff ) ;
				i51AdeMmiUpdateScreen () ;
				break ;

				default:
					break;
			}
		}
		break ;

		case i51_MSG_KEYDOWN :
		break ;

		case i51_MSG_PADUP :
			if(i51SimplePortalTouchDo (i51_MSG_PADUP , (iU32)parameter,PointY))
			{
				sprintf ( (iCHAR *)buffer , "9:Test Mode:%d",ChangeMode()) ;
				CORENRDrawText ( 5 , PointY , buffer , 0xffff ) ;
				i51AdeMmiUpdateScreen () ;
			}
		break ;

		case i51_MSG_PADDOWN :
			i51SimplePortalTouchDo (i51_MSG_PADDOWN , (iU32)parameter,PointY) ;
		break ;

		case i51_MSG_RUN :
		{
		}
		break ;
		
		case i51_MSG_INIT :
		{
	
			i51KIT_STATIC_KIT_SET(Adapter,parameter);
			i51KIT_STATIC_KIT_ASSERT(Adapter);

		}
		break ;
		
		case i51_MSG_PAUSE :
		break ;

		case i51_MSG_EXIT :
		break ;
		

	}

	

	return iTRUE ;

}
#if 0
iKIT iU32* i51KitPort ()
{

	//	TODO : Initialize API Port for external layer
	
	//	i51KIT_DYNAMIC_KIT_MAPING_BEGIN(DlDemo)
	//	i51KIT_DYNAMIC_KIT_MAPING_END 

	//	return i51KIT_DYNAMIC_KIT_GET_MAPING(DlDemo) ;

}
#endif