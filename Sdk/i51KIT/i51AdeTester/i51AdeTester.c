
#include "i51KitAde.h"
#include "i51AdeTester.h"

i51KIT_STATIC_KIT_DEF(Adapter);

static iU32 state = 0 ;
static iU32 TotallBugs = 0 ;
static iU16 screenwidth = 0 ;
static iU16 screenheight = 0 ;
static iU8 ErrorMessage [300][100] = { 0 } ;

#define EX_FONT_CHAR(value) (value)
#define EX_FONT_UNICODE_VAL(value) (value),
#define EX_FONT_ANSI_VAL(value) ,

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

static void CORENRDrawTextModel  ( iS32 x , iS32 y , const iCHAR* words , iU16 color ) {

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

static void* CORENRSearchTextModel ( iCHAR element ) {

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

void CORENRDrawText ( iS32 x , iS32 y , const iCHAR* text , iU16 color ) {

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
		CORENRDrawTextModel ( x  , y , (char*)CORENRSearchTextModel( text[walker]) , color );
		x = x + 8 ;
	}

}

static void i51AdeTesterRun () {

	//	author : Jelo 
	//	since : 2011.11.1
	// 	(C) 2011 , PKIG Tech. Co., Ltd.	

	iU32* Panels = i51AdeMmiGetPanel () ; 
	iU16* panel = (iU16*)Panels[1] ;
	iU32 width = Panels[2] ;
	iU32 height = Panels[3] ;
	iU32 results = 0 ;

	switch (state) {
		
		//	For i51AdeOs Testing
		case 0 :

			//	i51AdeOsMalloc
			//	i51AdeOsFree
			i51AdeOsLog ( i51_LOG_DC , "i51AdeOsMalloc : Testing -> Start" ) ;
			i51AdeOsLog ( i51_LOG_DC , "Case : i51AdeOsMalloc ( 1024*1024*1024 , __FILE__ , __LINE__ )" ) ;
			//	分配1G内存
			results = (iU32)i51AdeOsMalloc ( 1024*1024*1024 , __FILE__ , __LINE__ ) ;

			i51AdeOsLog ( i51_LOG_DC , "Results = %x " , results ) ;

			//	分配1bytes内存
			i51AdeOsLog ( i51_LOG_DC , "Case : i51AdeOsMalloc ( 1 , __FILE__ , __LINE__ )" ) ;
			results = (iU32)i51AdeOsMalloc ( 1 , __FILE__ , __LINE__ ) ;

			i51AdeOsLog ( i51_LOG_DC , "Results = %x " , results ) ;
			
			if ( 0 == results ) {
				i51AdeOsLog ( i51_LOG_DC , "i51AdeOsMalloc : Testing -> FAILED" ) ;
				TotallBugs ++ ;
			} else {				
				
				i51AdeOsLog ( i51_LOG_DC , "i51AdeOsMalloc : Testing -> OK" ) ;

				i51AdeOsLog ( i51_LOG_DC , "\r\n" ) ;
				i51AdeOsLog ( i51_LOG_DC , "i51AdeOsFree : Testing -> Start" ) ;
				i51AdeOsLog ( i51_LOG_DC , "Case : i51AdeOsFree ( ... )" ) ;
				i51AdeOsFree ( (void*) results ) ;
				i51AdeOsLog ( i51_LOG_DC , "i51AdeOsFree : Testing -> OK" ) ;

			}

			//	i51AdeOsGetTotallMemoryLeft
			i51AdeOsLog ( i51_LOG_DC , "\r\n" ) ;
			i51AdeOsLog ( i51_LOG_DC , "i51AdeOsGetTotallMemoryLeft : Testing -> Start" ) ;
			i51AdeOsLog ( i51_LOG_DC , "Case : i51AdeOsGetTotallMemoryLeft ( void )" ) ;
			results = i51AdeOsGetTotallMemoryLeft () ;
			i51AdeOsLog ( i51_LOG_DC , "Results = %x " , results ) ;
			i51AdeOsLog ( i51_LOG_DC , "i51AdeOsGetTotallMemoryLeft : Testing -> OK" ) ;
				
			//	i51AdeOsFileCreateDir
			i51AdeOsLog ( i51_LOG_DC , "\r\n" ) ;
			i51AdeOsLog ( i51_LOG_DC , "i51AdeOsFileCreateDir : Testing -> Start" ) ;
			i51AdeOsLog ( i51_LOG_DC , "Case : i51AdeOsFileCreateDir ( \"CreateDirTest\" )" ) ;
			results = i51AdeOsFileCreateDir ( L"CreateDirTest" ) ;
			i51AdeOsLog ( i51_LOG_DC , "Results = %x " , results ) ;

			if ( 0 == results ) {
				i51AdeOsLog ( i51_LOG_DC , "i51AdeOsGetTotallMemoryLeft : Testing -> FAILED" ) ;
				TotallBugs ++ ;
			} else {
				i51AdeOsLog ( i51_LOG_DC , "i51AdeOsGetTotallMemoryLeft : Testing -> OK" ) ;
			}
			
			//	i51AdeOsFileIsDirExist
			i51AdeOsLog ( i51_LOG_DC , "\r\n" ) ;
			i51AdeOsLog ( i51_LOG_DC , "i51AdeOsFileIsDirExist : Testing -> Start" ) ;
			i51AdeOsLog ( i51_LOG_DC , "Case : i51AdeOsFileIsDirExist ( \"CreateDirTest\" )" ) ;
			results = i51AdeOsFileIsDirExist ( L"CreateDirTest" ) ;
			i51AdeOsLog ( i51_LOG_DC , "Results = %x " , results ) ;

			if ( 0 == results ) {
				i51AdeOsLog ( i51_LOG_DC , "i51AdeOsFileIsDirExist : Testing -> FAILED" ) ;
				TotallBugs ++ ;
			} else {
				//	继续下一个
				i51AdeOsLog ( i51_LOG_DC , "Case : i51AdeOsFileIsDirExist ( \"CreateDirTestEx\" )" ) ;
				results = i51AdeOsFileIsDirExist ( L"CreateDirTestEx" ) ;
				i51AdeOsLog ( i51_LOG_DC , "Results = %x " , results ) ;				

				if ( 0 == results ) {
					i51AdeOsLog ( i51_LOG_DC , "i51AdeOsFileIsDirExist : Testing -> OK" ) ;					
				} else {
					i51AdeOsLog ( i51_LOG_DC , "i51AdeOsFileIsDirExist : Testing -> FAILED" ) ;
					TotallBugs ++ ;
				}

			}

			//	i51AdeOsFileDeleteDir
			i51AdeOsLog ( i51_LOG_DC , "\r\n" ) ;
			i51AdeOsLog ( i51_LOG_DC , "i51AdeOsFileDeleteDir : Testing -> Start" ) ;
			i51AdeOsLog ( i51_LOG_DC , "Case : i51AdeOsFileDeleteDir ( \"CreateDirTest\" )" ) ;
			results = i51AdeOsFileDeleteDir ( L"CreateDirTest" ) ;
			i51AdeOsLog ( i51_LOG_DC , "Results = %x " , results ) ;

			if ( 0 == results ) {
				i51AdeOsLog ( i51_LOG_DC , "i51AdeOsFileDeleteDir : Testing -> FAILED" ) ;
				TotallBugs ++ ;
			} else {

				i51AdeOsLog ( i51_LOG_DC , "Case : i51AdeOsFileDeleteDir ( \"CreateDirTestEx\" )" ) ;
				results = i51AdeOsFileDeleteDir ( L"CreateDirTestEx" ) ;
				i51AdeOsLog ( i51_LOG_DC , "Results = %x " , results ) ;

				if ( 0 == results ) {
					i51AdeOsLog ( i51_LOG_DC , "i51AdeOsFileDeleteDir : Testing -> OK" ) ;					
				} else {
					i51AdeOsLog ( i51_LOG_DC , "i51AdeOsFileDeleteDir : Testing -> FAILED" ) ;
					TotallBugs ++ ;
				}

			}
			
			//	i51AdeOsFileOpen
			i51AdeOsLog ( i51_LOG_DC , "\r\n" ) ;
			i51AdeOsLog ( i51_LOG_DC , "i51AdeOsFileOpen : Testing -> Start" ) ;
			i51AdeOsLog ( i51_LOG_DC , "Case : i51AdeOsFileOpen ( L\"CreateFileTest , i51_OS_FL_EXISTING_OPEN | i51_OS_FL_READ , __FILE__ , __LINE__\")" ) ;
			results = i51AdeOsFileOpen ( L"CreateFileTest" , i51_OS_FL_EXISTING_OPEN | i51_OS_FL_READ , __FILE__ , __LINE__ ) ;
			i51AdeOsLog ( i51_LOG_DC , "Results = %x " , results ) ;

			if ( 0 != results ) {
				i51AdeOsLog ( i51_LOG_DC , "i51AdeOsFileOpen : Testing -> FAILED" ) ;
				TotallBugs ++ ;
			} else {

				//	继续下一个
				i51AdeOsLog ( i51_LOG_DC , "Case : i51AdeOsFileOpen ( L\"CreateFileTest , i51_OS_FL_ALWAYS_OPEN | i51_OS_FL_WRITE , __FILE__ , __LINE__\")" ) ;
				results = i51AdeOsFileOpen ( L"CreateFileTest" , i51_OS_FL_ALWAYS_OPEN | i51_OS_FL_WRITE , __FILE__ , __LINE__ ) ;
				i51AdeOsLog ( i51_LOG_DC , "Results = %x " , results ) ;

				if ( 0 == results ) {
					i51AdeOsLog ( i51_LOG_DC , "i51AdeOsFileOpen : Testing -> FAILED" ) ;
					TotallBugs ++ ;
				} else {
					
					//	i51AdeOsFileClose
					i51AdeOsLog ( i51_LOG_DC , "i51AdeOsFileClose : Testing -> Start" ) ;
					results = i51AdeOsFileClose ( results ) ;
					i51AdeOsLog ( i51_LOG_DC , "Case : i51AdeOsFileClose" ) ;
					i51AdeOsLog ( i51_LOG_DC , "Results = %x " , results ) ;
			
					if ( 0 == results ) {
						i51AdeOsLog ( i51_LOG_DC , "i51AdeOsFileClose : Testing -> FAILED" ) ;
						TotallBugs ++ ;
					} else {

						i51AdeOsLog ( i51_LOG_DC , "i51AdeOsFileClose : Testing -> OK" ) ;

						i51AdeOsLog ( i51_LOG_DC , "Case : i51AdeOsFileOpen ( L\"CreateFileTest , i51_OS_FL_EXISTING_OPEN | i51_OS_FL_READ , __FILE__ , __LINE__\")" ) ;
						results = i51AdeOsFileOpen ( L"CreateFileTest" , i51_OS_FL_EXISTING_OPEN | i51_OS_FL_READ , __FILE__ , __LINE__ ) ;
						i51AdeOsLog ( i51_LOG_DC , "Results = %x " , results ) ;

						if ( 0 == results ) {
							i51AdeOsLog ( i51_LOG_DC , "i51AdeOsFileOpen : Testing -> FAILED" ) ;
							TotallBugs ++ ;
						} else {
							
							iBOOL ret = 0 ;
							iU32 length = 0 ;

							i51AdeOsFileClose ( results ) ;
							results = i51AdeOsFileOpen ( L"CreateFileTest" , i51_OS_FL_EXISTING_OPEN | i51_OS_FL_WRITE , __FILE__ , __LINE__ ) ;
									
							//	i51AdeOsFileWrite
							i51AdeOsLog ( i51_LOG_DC , "i51AdeOsFileWrite : Testing -> Start" ) ;
							i51AdeOsLog ( i51_LOG_DC , "Case : i51AdeOsFileWrite ( results , \"TEST\" , 4 , 0 )" ) ;
							ret = i51AdeOsFileWrite ( results , "TEST" , 4 , 0 ) ;
							i51AdeOsFileWrite ( results , "TEST" , 4 , &length ) ;
							i51AdeOsLog ( i51_LOG_DC , "Results = %x " , ret ) ;
							i51AdeOsFileClose ( results ) ;

							if ( length == 4 && ret == iTRUE ) {
								
								iCHAR buffer [ 10 ] = { 0 } ;
								i51AdeOsLog ( i51_LOG_DC , "i51AdeOsFileWrite : Testing -> OK" ) ;

								i51AdeOsLog ( i51_LOG_DC , "\r\n" ) ;

								//	i51AdeOsFileRead
								results = i51AdeOsFileOpen ( L"CreateFileTest" , i51_OS_FL_EXISTING_OPEN | i51_OS_FL_READ , __FILE__ , __LINE__ ) ;
								i51AdeOsLog ( i51_LOG_DC , "i51AdeOsFileRead : Testing -> Start" ) ;
								i51AdeOsLog ( i51_LOG_DC , "Case : i51AdeOsFileRead ( results , buffer , 8 , 0 )" ) ;
								ret = i51AdeOsFileRead ( results , buffer , 8 , 0 ) ;								
								i51AdeOsLog ( i51_LOG_DC , "Results = %x " , ret ) ;
								i51AdeOsFileClose ( results ) ;

								if ( 0 == ret || 0 != strcmp (buffer,"TESTTEST") ) {
									i51AdeOsLog ( i51_LOG_DC , "i51AdeOsFileRead : Testing -> FAILED" ) ;
									TotallBugs ++ ;
								} else {
									
									iU32 counter = 0 ;
									
									results = i51AdeOsFileOpen ( L"CreateFileTest" , i51_OS_FL_EXISTING_OPEN | i51_OS_FL_READ , __FILE__ , __LINE__ ) ;
									i51AdeOsLog ( i51_LOG_DC , "i51AdeOsFileRead : Testing -> Start" ) ;
									i51AdeOsLog ( i51_LOG_DC , "Case : i51AdeOsFileRead ( results , buffer , 8 , &counter )" ) ;
									ret = i51AdeOsFileRead ( results , buffer , 8 , &counter ) ;								
									i51AdeOsLog ( i51_LOG_DC , "Results = %x " , ret ) ;
									i51AdeOsFileClose ( results ) ;

									if ( 0 == ret || 8 != counter ) {
										i51AdeOsLog ( i51_LOG_DC , "i51AdeOsFileRead : Testing -> FAILED" ) ;
										TotallBugs ++ ;
									} 

								}
															
							} else {
								i51AdeOsLog ( i51_LOG_DC , "i51AdeOsFileWrite : Testing -> FAILED" ) ;
								TotallBugs ++ ;
							}
						}

					}

				}

			}
			

			//	For i51AdeOsFileFind、i51AdeOsFileFindNext、i51AdeOsFileFindClose Testing
			{
				
				iFILE file = 0 ;
				i51FILEINFO info = { 0 } ;
				
				i51AdeOsLog ( i51_LOG_DC , "i51AdeOsFileFind : Testing -> Start" ) ;

				i51AdeOsFileCreateDir ( L"Dir1" ) ;
				i51AdeOsFileCreateDir ( L"Dir1\\Dir2" ) ;
				results = i51AdeOsFileOpen ( L"Dir1\\Dir2\\CreateFileTest1.xxx" , i51_OS_FL_ALWAYS_OPEN | i51_OS_FL_WRITE , __FILE__ , __LINE__ ) ;
				i51AdeOsFileClose ( results ) ;

				i51AdeOsFileSetAttr ( L"Dir1\\Dir2\\CreateFileTest1.xxx" , i51_OS_FL_ATTR_HID ) ;

				results = i51AdeOsFileOpen ( L"Dir1\\Dir2\\CreateFileTest2.x" , i51_OS_FL_ALWAYS_OPEN | i51_OS_FL_WRITE , __FILE__ , __LINE__ ) ;
				i51AdeOsFileClose ( results ) ;
			
				file = i51AdeOsFileFind ( L"Dir1\\Dir2\\*" , &info ) ;
				
				if ( 0 != file ) {

					if ( 0 != i51AdeStdWStrcmp ( info.name , L"CreateFileTest1.xxx" ) ) {
												
						i51AdeOsLog ( i51_LOG_DC , "i51AdeOsFileFind : Testing -> FAILED" ) ;
						TotallBugs ++ ;

					} else {

						if ( (i51_OS_FL_ATTR_HID != (info.attribute & i51_OS_FL_ATTR_HID )) ) {

							i51AdeOsLog ( i51_LOG_DC , "i51AdeOsFileFind : Testing -> FAILED" ) ;
							TotallBugs ++ ;

						} else {
							
							iBOOL ret = iTRUE ;

							while ( iTRUE != ret ) {
								
								ret = i51AdeOsFileFindNext ( file , &info ) ;
								
								if ( iTRUE != ret ) {
									i51AdeOsLog ( i51_LOG_DC , "i51AdeOsFileFind : Testing -> FAILED" ) ;
									TotallBugs ++ ;
									break ;
								} else {

									if ( 0 != i51AdeStdWStrcmp ( info.name , L"CreateFileTest2.x" ) ) {
										i51AdeOsLog ( i51_LOG_DC , "i51AdeOsFileFind : Testing -> FAILED" ) ;
										TotallBugs ++ ;
									} else {

										i51AdeOsLog ( i51_LOG_DC , "i51AdeOsFileFind : Testing -> OK" ) ;
	
									}

								}

							}							

						}

					}					
				
					i51AdeOsFileFindClose ( file ) ;

				}


			}

			{
				iU32 ret = 0 ;
				iU32 ret2 = 0 ;
				
				//	i51AdeOsFileIsDirExist、i51AdeOsFileDeleteDir
				i51AdeOsLog ( i51_LOG_DC , "i51AdeOsFileIsDirExist : Testing -> Start" ) ;
				ret = i51AdeOsFileIsDirExist ( L"Dir1" ) ;
				ret2 = i51AdeOsFileIsDirExist ( L"Dir1\\Dir2" ) ;

				if ( 0 == ret || 0 == ret2 ) {

					i51AdeOsLog ( i51_LOG_DC , "i51AdeOsFileIsDirExist : Testing -> FAILED" ) ;

					if ( 0 == ret && 0 == ret2 ) {
						TotallBugs += 2 ;
					} else {
						TotallBugs ++ ;
					}

				} else {
					
					i51AdeOsLog ( i51_LOG_DC , "i51AdeOsFileIsDirExist : Testing -> OK" ) ;

					i51AdeOsLog ( i51_LOG_DC , "\r\n" ) ;
					i51AdeOsLog ( i51_LOG_DC , "i51AdeOsFileDeleteDir : Testing -> Start" ) ;
					ret = i51AdeOsFileDeleteDir ( L"Dir1" ) ;

					if ( 0 == ret ) {
						i51AdeOsLog ( i51_LOG_DC , "i51AdeOsFileDeleteDir : Testing -> FAILED" ) ;
						TotallBugs ++ ;
					} else {
										
						ret = i51AdeOsFileIsDirExist ( L"Dir1" ) ;
						ret2 = i51AdeOsFileIsDirExist ( L"Dir1\\Dir2" ) ;

						if ( 1 == ret || 1 == ret2 ) {
						
							i51AdeOsLog ( i51_LOG_DC , "i51AdeOsFileDeleteDir : Testing -> FAILED" ) ;

							if ( 1 == ret && 1 == ret2 ) {
								TotallBugs += 2 ;
							} else {
								TotallBugs ++ ;
							}

						} else {
							i51AdeOsLog ( i51_LOG_DC , "i51AdeOsFileDeleteDir : Testing -> OK" ) ;
						}


					}

				}

				{
					//	SEEK
					iS32 seek = 0 ;
					iS32 size = 0 ;

					i51AdeOsLog ( i51_LOG_DC , "\r\n" ) ;
					i51AdeOsLog ( i51_LOG_DC , "SEEK : Testing -> Start" ) ;
					results = i51AdeOsFileOpen ( L"CreateFileTest" , i51_OS_FL_EXISTING_OPEN | i51_OS_FL_WRITE , __FILE__ , __LINE__ ) ;
					i51AdeOsFileGetSeek(results,&seek,i51_OS_FL_SEEK_HEAD);
					
					if ( 0 != seek ) {
						i51AdeOsLog ( i51_LOG_DC , "SEEK : Testing -> FAILED" ) ;
						TotallBugs ++ ;
					} else {
						
						i51AdeOsFileSetSeek ( results,8,i51_OS_FL_SEEK_HEAD ) ;
						i51AdeOsFileGetSeek(results,&seek,i51_OS_FL_SEEK_CURRENT);
						i51AdeOsFileGetSize(results,&size);
						if ( 0 >= seek || seek != size ) {
							i51AdeOsLog ( i51_LOG_DC , "SEEK : Testing -> FAILED" ) ;
							TotallBugs ++ ;
						} else {
							i51AdeOsFileSetSeek ( results,4,i51_OS_FL_SEEK_HEAD ) ;
							i51AdeOsFileSetSeek ( results,-2,i51_OS_FL_SEEK_CURRENT ) ;
							i51AdeOsFileSetSeek ( results,1,i51_OS_FL_SEEK_CURRENT ) ;
							i51AdeOsFileGetSeek(results,&seek,i51_OS_FL_SEEK_CURRENT);
							if ( 3 != seek  ) {
								i51AdeOsLog ( i51_LOG_DC , "SEEK : Testing -> FAILED" ) ;
								TotallBugs ++ ;
							} else {
								i51AdeOsFileSetSeek ( results,0,i51_OS_FL_SEEK_TAIL ) ;
								i51AdeOsFileGetSeek(results,&seek,i51_OS_FL_SEEK_CURRENT);
								if ( 8 != seek ) {
									i51AdeOsLog ( i51_LOG_DC , "SEEK : Testing -> FAILED" ) ;
									TotallBugs ++ ;
								} else {
									i51AdeOsFileSetSeek ( results,-1,i51_OS_FL_SEEK_TAIL ) ;
									i51AdeOsFileGetSeek(results,&seek,i51_OS_FL_SEEK_CURRENT);
									if ( 7 != seek ) {
										i51AdeOsLog ( i51_LOG_DC , "SEEK : Testing -> FAILED" ) ;
										TotallBugs ++ ;
									} else {
										i51AdeOsLog ( i51_LOG_DC , "SEEK : Testing -> OK" ) ;
									}
								}
							}
						}

					}

					i51AdeOsFileClose ( results ) ;

				}

			}
			
			//	i51AdeOsFileSetAttr
			{
				iU32 attr = 0 ;
				i51AdeOsLog ( i51_LOG_DC , "\r\n" ) ;
				i51AdeOsLog ( i51_LOG_DC , "i51AdeOsFileSetAttr : Testing -> Start" ) ;
				i51AdeOsFileSetAttr ( L"CreateFileTest" , i51_OS_FL_ATTR_ROL | i51_OS_FL_ATTR_HID ) ;
				attr = i51AdeOsFileGetAttr ( L"CreateFileTest" ) ;
				
				if ( (i51_OS_FL_ATTR_ROL != (attr & i51_OS_FL_ATTR_ROL )) 
					&& (i51_OS_FL_ATTR_HID != (attr & i51_OS_FL_ATTR_HID )) ) {

					i51AdeOsLog ( i51_LOG_DC , "i51AdeOsFileSetAttr : Testing -> FAILED" ) ;
					TotallBugs ++ ;

				} else {
					i51AdeOsLog ( i51_LOG_DC , "i51AdeOsFileSetAttr : Testing -> OK" ) ;
				}

				i51AdeOsFileSetAttr ( L"CreateFileTest" , i51_OS_FL_ATTR_WRT ) ;

			}

			//	i51AdeOsFileDelete
			{
				iU32 ret = 0 ;
 				i51AdeOsLog ( i51_LOG_DC , "\r\n" ) ;
				i51AdeOsLog ( i51_LOG_DC , "i51AdeOsFileDelete : Testing -> Start" ) ;
				ret = i51AdeOsFileDelete ( L"CreateFileTest" ) ;
				if ( 0 == ret ) {
					i51AdeOsLog ( i51_LOG_DC , "i51AdeOsFileDelete : Testing -> FAILED" ) ;
					TotallBugs ++ ;
				} else {
					
					ret = i51AdeOsFileIsExist ( L"CreateFileTest" ) ;

					if ( 0 != ret ) {
						i51AdeOsLog ( i51_LOG_DC , "i51AdeOsFileDelete : Testing -> FAILED" ) ;
						TotallBugs ++ ;
					} else {
						i51AdeOsLog ( i51_LOG_DC , "i51AdeOsFileDelete : Testing -> OK" ) ;
					}

				}
			}
			
			//	i51AdeOsFileCopy
			{

				iU32 ret = 0 ;
				iCHAR writebuffer [ 10 ] = {"TESTTEST"} ;
				iCHAR readbuffer [ 10 ] = { 0 } ;

 				i51AdeOsLog ( i51_LOG_DC , "\r\n" ) ;
				i51AdeOsLog ( i51_LOG_DC , "i51AdeOsFileCopy : Testing -> Start" ) ;
				i51AdeOsFileCreateDir ( L"Dir1" ) ;
				i51AdeOsFileCreateDir ( L"Dir1\\Dir2" ) ;
				results = i51AdeOsFileOpen ( L"Dir1\\Dir2\\CreateFileTest" , i51_OS_FL_ALWAYS_OPEN | i51_OS_FL_WRITE , __FILE__ , __LINE__ ) ;
				i51AdeOsFileWrite ( results , writebuffer , strlen("TESTTEST") , 0 ) ;
				i51AdeOsFileClose ( results ) ;

				i51AdeOsFileCreateDir ( L"Dir111" ) ;
				i51AdeOsFileCreateDir ( L"Dir111\\Dir222" ) ;				

				i51AdeOsFileCopy ( L"Dir1\\Dir2\\CreateFileTest" , L"Dir111\\Dir222\\CreateFileTest2" ) ;
			
				ret = i51AdeOsFileIsExist ( L"Dir111\\Dir222\\CreateFileTest2" ) ;

				if ( 0 == ret ) {
					i51AdeOsLog ( i51_LOG_DC , "i51AdeOsFileCopy : Testing -> FAILED" ) ;
					TotallBugs ++ ;
				} else {

					results = i51AdeOsFileOpen ( L"Dir111\\Dir222\\CreateFileTest2" , i51_OS_FL_EXISTING_OPEN | i51_OS_FL_READ , __FILE__ , __LINE__ ) ;
					i51AdeOsFileRead ( results , readbuffer , strlen("TESTTEST") , 0 ) ;
					i51AdeOsFileClose ( results ) ;

					if ( 0 != strcmp ( readbuffer , "TESTTEST" ) ) {
					
						i51AdeOsLog ( i51_LOG_DC , "i51AdeOsFileCopy : Testing -> FAILED" ) ;
						TotallBugs ++ ;

					} else {

						ret = i51AdeOsFileIsExist ( L"Dir1\\Dir2\\CreateFileTest" ) ;
						
						if ( 0 == ret ) {
							i51AdeOsLog ( i51_LOG_DC , "i51AdeOsFileCopy : Testing -> FAILED" ) ;
							TotallBugs ++ ;
						} else {
							i51AdeOsLog ( i51_LOG_DC , "i51AdeOsFileCopy : Testing -> OK" ) ;
						}
				
					}

				}
				
				//	clear up
				i51AdeOsFileDeleteDir ( L"Dir1" ) ;
				i51AdeOsFileDeleteDir ( L"Dir111" ) ;

			}
			
			//	i51AdeOsFileMove
			{

				iU32 ret = 0 ;
				iCHAR writebuffer [ 10 ] = {"TESTTEST"} ;
				iCHAR readbuffer [ 10 ] = { 0 } ;

 				i51AdeOsLog ( i51_LOG_DC , "\r\n" ) ;
				i51AdeOsLog ( i51_LOG_DC , "i51AdeOsFileMove : Testing -> Start" ) ;
				i51AdeOsFileCreateDir ( L"Dir1" ) ;
				i51AdeOsFileCreateDir ( L"Dir1\\Dir2" ) ;
				results = i51AdeOsFileOpen ( L"Dir1\\Dir2\\CreateFileTest" , i51_OS_FL_ALWAYS_OPEN | i51_OS_FL_WRITE , __FILE__ , __LINE__ ) ;
				i51AdeOsFileWrite ( results , writebuffer , strlen("TESTTEST") , 0 ) ;
				i51AdeOsFileClose ( results ) ;

				i51AdeOsFileCreateDir ( L"Dir111" ) ;
				i51AdeOsFileCreateDir ( L"Dir111\\Dir222" ) ;				

				i51AdeOsFileMove ( L"Dir1\\Dir2\\CreateFileTest" , L"Dir111\\Dir222\\CreateFileTest2" ) ;
			
				ret = i51AdeOsFileIsExist ( L"Dir111\\Dir222\\CreateFileTest2" ) ;

				if ( 0 == ret ) {
					i51AdeOsLog ( i51_LOG_DC , "i51AdeOsFileMove : Testing -> FAILED" ) ;
					TotallBugs ++ ;
				} else {

					results = i51AdeOsFileOpen ( L"Dir111\\Dir222\\CreateFileTest2" , i51_OS_FL_EXISTING_OPEN | i51_OS_FL_READ , __FILE__ , __LINE__ ) ;
					i51AdeOsFileRead ( results , readbuffer , strlen("TESTTEST") , 0 ) ;
					i51AdeOsFileClose ( results ) ;

					if ( 0 != strcmp ( readbuffer , "TESTTEST" ) ) {
					
						i51AdeOsLog ( i51_LOG_DC , "i51AdeOsFileMove : Testing -> FAILED" ) ;
						TotallBugs ++ ;

					} else {

						ret = i51AdeOsFileIsExist ( L"Dir1\\Dir2\\CreateFileTest" ) ;
						
						if ( 0 != ret ) {
							i51AdeOsLog ( i51_LOG_DC , "i51AdeOsFileMove : Testing -> FAILED" ) ;
							TotallBugs ++ ;
						} else {
							i51AdeOsLog ( i51_LOG_DC , "i51AdeOsFileMove : Testing -> OK" ) ;
						}
				
					}

				}
				
				//	clear up
				i51AdeOsFileDeleteDir ( L"Dir1" ) ;
				i51AdeOsFileDeleteDir ( L"Dir111" ) ;

			}
			

			{
	//i51KIT_STATIC_KIT_DEF_PORT(i51AdeSmsGetSims)
	//i51KIT_STATIC_KIT_DEF_PORT(i51AdeSmsGetImsi)
	//i51KIT_STATIC_KIT_DEF_PORT(i51AdeSmsGetImei)
	//i51KIT_STATIC_KIT_DEF_PORT(i51AdeSmsGetCenter)
				iS32* sims = i51AdeSmsGetSims () ;

 				i51AdeOsLog ( i51_LOG_DC , "\r\n" ) ;
				i51AdeOsLog ( i51_LOG_DC , "SIM : Testing -> Start" ) ;

				if ( 0 == sims ) {
					i51AdeOsLog ( i51_LOG_DC , "SIM : Testing -> FAILED" ) ;
					TotallBugs ++ ;
				} else {
					
					iCHAR* imsi = 0 ;
					iCHAR* imei = 0 ;
					iCHAR* imc = 0 ;
					iBOOL ret = iTRUE ;

					imsi = i51AdeSmsGetImsi ( sims[1] ) ;
					imei = i51AdeSmsGetImei () ;
					imc = i51AdeSmsGetCenter ( sims[1] ) ;

					if ( 0 == imsi ) {
						TotallBugs ++ ;
						ret = iFALSE ;
					}

					if ( 0 == imei ) {
						TotallBugs ++ ;
						ret = iFALSE ;
					}

					if ( 0 == imc ) {
						TotallBugs ++ ;
						ret = iFALSE ;
					}

					if ( iFALSE == ret ) {
						i51AdeOsLog ( i51_LOG_DC , "SIM : Testing -> FAILED" ) ;
					} else {
						i51AdeOsLog ( i51_LOG_DC , "SIM : Testing -> OK" ) ;
					}

				}

			}
			
			//	i51AdeStdWStrlen、i51AdeStdWStrcpy、i51AdeStdWStrcmp、i51AdeStdWStrcat
			{

				iU16 desrc [32] = { 0 } ;
				iU16* retsrc = 0 ;
				iBOOL ret = iTRUE ;

 				i51AdeOsLog ( i51_LOG_DC , "\r\n" ) ;
				i51AdeOsLog ( i51_LOG_DC , "WStr : Testing -> Start" ) ;

				if ( 4 != i51AdeStdWStrlen ( L"TEST" ) ) {
					TotallBugs ++ ;
					ret = iFALSE ;
				} 

				retsrc = i51AdeStdWStrcpy ( desrc , L"TEST" ) ;

				if ( (iU32 )retsrc != (iU32 )desrc ) {
					TotallBugs ++ ;
					ret = iFALSE ;
				}

				if ( 0 != i51AdeStdWStrcmp (desrc,L"TEST") ) {
					TotallBugs ++ ;
					ret = iFALSE ;
				}

				retsrc = i51AdeStdWStrcat ( desrc , L"Adapter" ) ;

				if ( (iU32 )retsrc != (iU32 )desrc ) {
					TotallBugs ++ ;
					ret = iFALSE ;
				}

				if ( 0 != i51AdeStdWStrcmp (desrc,L"TESTAdapter") ) {
					TotallBugs ++ ;
					ret = iFALSE ;
				}

				if ( iFALSE == ret ) {
					i51AdeOsLog ( i51_LOG_DC , "WStr : Testing -> FAILED" ) ;
				} else {
					i51AdeOsLog ( i51_LOG_DC , "WStr : Testing -> OK" ) ;
				}

			}

			state = 1 ;

			i51AdeOsLog ( i51_LOG_DC , "i51AdeTester Results:" ) ;
			i51AdeOsLog ( i51_LOG_DC , "Totall Bugs : %d" , TotallBugs ) ;


		break ;

	}

}

static void i51AdeTesterDrawCopyrights () {
	
	//	author : Jelo 
	//	since : 2011.12.9
	// 	(C) 2011 , PKIG Tech. Co., Ltd.	
	
	//	绘制版权信息

	CORENRDrawText ( 0 , 0 , "i51 Tester" , 0xffff ) ;

}

iKIT iBOOL i51KitMain ( iS32 aid , iU32 message , void* parameter )
{

	//	TODO :	消息循环

	switch ( message )
	{
		case i51_MSG_PAINT :
		{

			iU32* panel = i51AdeMmiGetPanel () ;
			i51AdeMmiGetScreenScale ( &screenwidth , &screenheight ) ;			
			i51AdeStdMemset16 ( (void*)panel[1] , 0 , screenwidth*screenheight<<1 ) ;

			i51AdeTesterDrawCopyrights () ;
			i51AdeTesterRun () ;
			i51AdeMmiUpdateScreen () ;

		}
		break ;

		case i51_MSG_RUN :
		{
			i51AdeOsScreenAlwaysLight ( iTRUE ) ;
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
