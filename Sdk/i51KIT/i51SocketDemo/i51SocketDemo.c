
#include "i51KitAde.h"
#include "i51SocketDemo.h"

i51KIT_STATIC_KIT_DEF(Adapter);

//#define i51SIMULATOR
#define I51_DL_DATA_LEN		(2*1024)

#define I51_TEST_SOC_TIME_OUT (30*1000)
#define i51HTTP_PROXY_HEADER_PATTERN "POST %s HTTP/1.1\r\nX-Online-Host: %s:%d\r\nConnection: Close\r\nContent-Length:  %9d\r\n\r\n"
#define i51HTTP_HEADER_PATTERN "POST %s HTTP/1.1\r\nHost: %s:%d\r\nConnection: Close\r\nContent-Length:  %9d\r\n\r\n"

typedef enum 
{

	//	author : Jelo
	//	since : 2011.6.23
	// 	(C) 2011 , PKIG Tech. Co., Ltd.	

	//	notes : 下载器状态
	
	i51_DL_STATE_IDLE ,
	i51_DL_STATE_INIT ,
	i51_DL_STATE_CONNECT ,
	i51_DL_STATE_SEND ,
	i51_DL_STATE_RECEIVE ,
	i51_DL_STATE_CLOSE ,
	i51_DL_STATE_RESTART ,
	
} i51TestdlState ;

typedef enum
{

	//	notes : 链路层类型
	
	i51_DL_LINKLAYER_CMWAP ,
	i51_DL_LINKLAYER_CMNET ,

} i51TestDLLINKLAYER ;

typedef enum {

	//	成功
	NET_CODE_OK ,
	//	使用定时器错误
	NET_CODE_TIMER = -1 ,
	//	 创建链路出错
	NET_CODE_LINK = -2 ,
	//	超时
	NET_CODE_CONN_RES_TIMEOUT = -3 ,
	NET_CODE_CONN_LINK_TIMEOUT = -4 ,
	//	连接错误
	NET_CODE_CONN_ERROR = -5 ,
	//	发送错误
	NET_CODE_SEND_ERROR = -6 ,
	//	介绍错误
	NET_CODE_RECV_ERROR = -7 ,

}I51NETCode ;

typedef  struct {

	iU32 socketId ;
	iU32 timer ;
	iU32 monitortimer ;
	iU32 layerid ;
	iU32 recordtime ;
	iU32 monitortimerType ;
	iU32 state ;
	iU32 linkmode ;
	iU32 simnum ;
	iU32 sendlen ;
	i51SOCADDR server ;
	iS8 cache[I51_DL_DATA_LEN] ;

} I51TESTDL;

typedef void (*i51TestSocTouch) ( iU16 event , iS32 x , iS32 y ) ;

#define I51_TESTSOC_TOUCH_COUNT 12
typedef struct {

	//since : 20110918
	//note : 触屏信息

	iU16 x ;
	iU16 y ;
	iU16 width ;
	iU16 height ;
	//热区放大倍数
	iU16 magnification ;
	iBOOL IsDown ;
	//	触屏回调
	i51TestSocTouch TouchCallBack ;


}I51_TESTSOC_TOUCH_INFO ;

static I51_TESTSOC_TOUCH_INFO i51TestSocTouchList[I51_TESTSOC_TOUCH_COUNT] ;
//注册触摸事件的个数
static iU16 i51TestSocRegisterTouchCount = 0  ;

I51TESTDL i51TestDl = {0} ;

#define SOC_COUNT	5
static iS8 i51TestSocList[SOC_COUNT][32] = { "baidu" , "i51" ,"i51 Test" , "pay" , "pay Test"} ;
static iU16 i51TestSocCount = SOC_COUNT;
static iU32 KeyDownNumber = 0 ;
static iU32 SelectDl = -1 ;
static iS8 netcode[10] = {0} ;

static iBOOL showNetstateCode = iFALSE ;
static iS32 NetstateCode = 0 ;
static iS32 gaid = 0 ;
static iBOOL dlused = iFALSE ;

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

static void i51TestSocStart (iU32 selectItem ) ;

static void lla_utils_set_u8 (unsigned char **p, unsigned char n)
{
    *(*p)++ = n;
}
static void lla_utils_set_u16 (unsigned char **p, unsigned short n)
{
    *(*p)++ = (unsigned char)(n >> 8);
    *(*p)++ = (unsigned char)(n & 0xff);
}
static void lla_utils_set_u32 (unsigned char **p, unsigned int n)
{
    *(*p)++ = (unsigned char)((n>>24)&0xff);            
    *(*p)++ = (unsigned char)((n>>16)&0xff);            
    *(*p)++ = (unsigned char)((n>>8)&0xff);             
    *(*p)++ = (unsigned char)(n & 0xff);                
}


#define SET_U8(p,n)   lla_utils_set_u8((unsigned char **)&p, (unsigned char)(n))
#define SET_U16(p,n)  lla_utils_set_u16((unsigned char **)&p, (unsigned short)(n))
#define SET_U32(p,n)  lla_utils_set_u32((unsigned char **)&p, (unsigned int)(n))

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

static void CORENRDrawText ( iS32 x , iS32 y , const iS8* text , iU16 color ) {

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


static void i51TestSocSetNetStateCode ( iBOOL showNetstateCode1 , iS32 NetstateCode1 )
{
	showNetstateCode = showNetstateCode1 ;
	NetstateCode = NetstateCode1 ;
}
static void i51TestSocRegisterTouch ( iU16 x , iU16 y , iU16 width , iU16 height , iU16 magnification , i51TestSocTouch TouchCallBack )
{
	//	 注册触屏区域
	i51TestSocTouchList[i51TestSocRegisterTouchCount].x = x ;
	i51TestSocTouchList[i51TestSocRegisterTouchCount].y = y ;
	i51TestSocTouchList[i51TestSocRegisterTouchCount].width = width ;
	i51TestSocTouchList[i51TestSocRegisterTouchCount].height = height ;
	i51TestSocTouchList[i51TestSocRegisterTouchCount].magnification = magnification  ;
	i51TestSocTouchList[i51TestSocRegisterTouchCount].TouchCallBack = TouchCallBack ;
	
	i51TestSocRegisterTouchCount ++ ;
	
}

static void i51TestSocClearTouch ()
{
	
	//	 触屏信息清空
	
	memset ( i51TestSocTouchList , 0 ,  sizeof ( i51TestSocTouchList ) ) ;
	i51TestSocRegisterTouchCount = 0 ;
	
}

static void i51TestSocTouchDo ( iU16 event , iU32 value )
{

	iU16 i = 0 ;
	iU32 x= 0 , y = 0 ;

	x =  ( value >>16 ) & 0x0000ffff ;
	y = value & 0x0000ffff ;

	for ( i = 0 ; i < i51TestSocRegisterTouchCount ; i++ ) {
		
		if ( 	x > i51TestSocTouchList[i].x - i51TestSocTouchList[i].width * (i51TestSocTouchList[i].magnification - 1 ) && 
			x < i51TestSocTouchList[i].x + i51TestSocTouchList[i].width + i51TestSocTouchList[i].width * (i51TestSocTouchList[i].magnification - 1 ) &&
			y > i51TestSocTouchList[i].y - i51TestSocTouchList[i].height* (i51TestSocTouchList[i].magnification - 1 ) && 
			y < i51TestSocTouchList[i].y + i51TestSocTouchList[i].height + i51TestSocTouchList[i].height* (i51TestSocTouchList[i].magnification - 1 )) {
			
			if ( i51_MSG_PADDOWN == event ) {
				
				i51TestSocTouchList[i].IsDown = iTRUE ;
				
			} 
			
			if ( i51TestSocTouchList[i].IsDown && i51_MSG_PADUP == event ) {

				i51TestSocTouchList[i].IsDown = iFALSE ;
				if ( i51TestSocTouchList[i].TouchCallBack )
					i51TestSocTouchList[i].TouchCallBack ( event , x , y ) ;
			} 
			
			
			break ;	
			
		}
	}
	
	
}

void i51TestSocPadCB1 ( iU16 event , iS32 x , iS32 y )
{
	
	if ( i51TestSocCount >= 1 && !dlused) {

		SelectDl = 0 ;
		memset ( netcode , 0 , sizeof(0) ) ;
		i51TestSocSetNetStateCode ( iFALSE, 0 ) ;
		i51TestSocStart (SelectDl) ;
		dlused = iTRUE ;
	}

}

void i51TestSocPadCB2 ( iU16 event , iS32 x , iS32 y )
{
	if ( i51TestSocCount >= 2 && !dlused ) {

		SelectDl = 1 ;
		memset ( netcode , 0 , sizeof(0) ) ;
		i51TestSocSetNetStateCode ( iFALSE, 0 ) ;
		i51TestSocStart (SelectDl) ;
		dlused = iTRUE ;
	}

}

void i51TestSocPadCB3 ( iU16 event , iS32 x , iS32 y )
{
	if ( i51TestSocCount >= 3 && !dlused ) {

		SelectDl = 2 ;
		memset ( netcode , 0 , sizeof(0) ) ;
		i51TestSocSetNetStateCode ( iFALSE, 0 ) ;
		i51TestSocStart (SelectDl) ;
		dlused = iTRUE ;
	}

}

void i51TestSocPadCB4 ( iU16 event , iS32 x , iS32 y )
{
	if ( i51TestSocCount >= 4 && !dlused ) {

		SelectDl = 3 ;
		memset ( netcode , 0 , sizeof(0) ) ;
		i51TestSocSetNetStateCode ( iFALSE, 0 ) ;
		i51TestSocStart (SelectDl) ;
		dlused = iTRUE ;
	}

}

void i51TestSocPadCB5 ( iU16 event , iS32 x , iS32 y )
{
	if ( i51TestSocCount >= 5 && !dlused ) {

		SelectDl = 4 ;
		memset ( netcode , 0 , sizeof(0) ) ;
		i51TestSocSetNetStateCode ( iFALSE, 0 ) ;
		i51TestSocStart (SelectDl) ;
		dlused = iTRUE ;
	}

}


static void i51TestSocFillRect ( iU8* panel , iS32 width , iS32 height , iS32 desx , iS32 desy , iCOLOR color  )
{

	//	author : william
	//	since : 20111010
	//	PKIG Tech. Co., Ltd.
	//	填充矩形

	iS32 CleanX, CleanY;
	iS32 CleanW, CleanH;
	iS32 LcdBufPos = 0;
	iS32 TempX, TempY;
	iU16 lcdwight , lcdheight ;
	iU16 *buffer = iNULL , *OneRowColorBuf = iNULL ;
	i51AdeMmiGetScreenScale ( &lcdwight , &lcdheight ) ;

	OneRowColorBuf = (iU16*)i51AdeOsMalloc(lcdwight*2, __FILE__, __LINE__) ;

	if ( !OneRowColorBuf ) {

		i51AdeOsLog ( 0 , "KIT - INS : i51InstallerFillRect malloc F%d") ;
		return ;
	}
	
	CleanX = desx;
	CleanY = desy;
	CleanW = width;
	CleanH = height;

	if(CleanX<0)
	{
	    CleanW += CleanX;
	    CleanX = 0;
	}

	if(CleanY<0)
	{
	    CleanH += CleanH;
	    CleanY = 0;
	}

	if( (CleanX+CleanW)>lcdwight) CleanW = lcdwight-CleanX;
	if( (CleanY+CleanH)>lcdheight) CleanH = lcdheight-CleanY;

	for( TempX=0; TempX<CleanW ; TempX++)
	{
	    OneRowColorBuf[TempX] = color;
	}

	buffer = (iU16*)panel ;
	CleanW = (CleanW<<1);	
	LcdBufPos = CleanY*lcdwight + CleanX;
	for( TempY=0; TempY<CleanH; TempY++)
	{
	    i51AdeStdMemcpy16(&(buffer[LcdBufPos]), OneRowColorBuf, CleanW);
	    LcdBufPos += lcdwight;
	}
	i51AdeOsFree(OneRowColorBuf) ;
	
}


static void i51TestSocStopProcess ()
{

	if ( i51TestDl.timer > 0) {

		i51AdeOsTimerStop ( i51TestDl.timer ) ;
		i51TestDl.timer = 0 ;
	}

	if ( i51TestDl.monitortimer ) {

		i51AdeOsTimerStop (i51TestDl.monitortimer ) ;
		i51TestDl.monitortimer = 0 ;
	}

	i51AdeSocClose ( i51TestDl.socketId) ;
	i51AdeSocCloseLinkLayer(gaid, i51TestDl.layerid) ;
	i51AdeSocReleaseLinkLayer (i51TestDl.layerid) ;
	dlused = iFALSE ;
	
	
}


static void i51TestSocTimerCB ( iU32 id )
{

	//	author : william
	//	since : 20111125
	//	PKIG Tech. Co., Ltd.


	if (  i51AdeOsGetTick () - i51TestDl.recordtime > I51_TEST_SOC_TIME_OUT) {

		//	设定60秒为超时
		if ( i51TestDl.monitortimerType == 1 ) {

			if (i51TestDl.monitortimer > 0) {

				//	创建链路超时
				i51TestSocSetNetStateCode ( iTRUE , NET_CODE_CONN_LINK_TIMEOUT ) ;
				i51AdeOsLog ( 0 , "SL - Dl : i51TestSocTimerCB time out 1") ;
				i51AdeOsTimerStop ( i51TestDl.monitortimer) ;  
				i51TestDl.monitortimer = 0 ;
				i51AdeSocReleaseLinkLayer (i51TestDl.layerid) ;
			}

		} else {

				//	soc超时
			i51TestSocSetNetStateCode ( iTRUE , NET_CODE_CONN_RES_TIMEOUT ) ;
			i51AdeOsLog ( 0 , "SL - Dl : i51TestSocTimerCB time out 2") ;
			i51TestSocStopProcess () ;
		}
		
	}

	

	

}

static void i51TestSocTimerStart ( iU32 monitortimerType )
{
	//	author : william
	//	since : 20111125
	//	PKIG Tech. Co., Ltd.

	//	参数为1，创建链路开启
	//	参数为2，soc开启
	
	i51TestDl.monitortimer = i51AdeOsTimerCreate(1,1) ;
	i51TestDl.monitortimerType = monitortimerType ;
	if ( i51TestDl.monitortimer == 0 ) {

		i51TestSocSetNetStateCode ( iTRUE , NET_CODE_TIMER ) ;
		i51TestSocStopProcess () ;
		i51AdeOsLog ( 0 , "SL - Dl : i51TestSocTimerStart CT F" ) ;
		return ;

	}

	if ( !i51AdeOsTimerStart (i51TestDl.monitortimer , i51TestSocTimerCB ,3*1000)) {

		i51TestSocSetNetStateCode ( iTRUE , NET_CODE_TIMER ) ;
		if ( monitortimerType == 1 ) {

			
		} else {

		i51TestSocStopProcess () ;
		i51AdeOsLog ( 0 , "SL - Dl : i51TestSocTimerStart ST F") ;
		}
		return ; 
	}
	i51AdeOsLog ( 0 , "SL - Dl : i51TestSocTimerStart timerid = %d" , i51TestDl.monitortimer) ;
	i51TestDl.recordtime = i51AdeOsGetTick() ;
	
	

}

static void i51SlDlBuildRequest (  iS8* url , iS8 *host , iU32 port )
{

#define i51HTTP_PAY_PROXY_HEADER_PATTERN "POST %s HTTP/1.1\r\nX-Online-Host: %s:%d\r\nContent-Type: application/octet-stream\r\n"
#define i51HTTP_PAY_HEADER_PATTERN "POST %s HTTP/1.1\r\nHost: %s:%d\r\nContent-Type: application/octet-stream\r\n"
#define i51HTTP_CONTENT_LENGTH 30

	iS8* content = 0 ;
	iS8* mark = 0 ;
	iS8 header[256] = {0} ;

	#ifndef  i51SIMULATOR		
	sprintf ( (iS8*)i51TestDl.cache , i51HTTP_PAY_PROXY_HEADER_PATTERN ,url ,host , port ) ;	
	#else
	sprintf ( (iS8*)i51TestDl.cache , i51HTTP_PAY_HEADER_PATTERN  ,url , host , port ) ;
	#endif

	strcpy ( header , i51TestDl.cache ) ;

	
	//	skip http header
	content = i51TestDl.cache + strlen(header) + i51HTTP_CONTENT_LENGTH ;


	//	跳过校验码，需要回填
	content = content + 4 ;  
	
	//	底层协议版本
	SET_U32 ( content ,  1) ;
	
	//	kernel版本
	SET_U32 ( content ,  1) ;
	
	//	adapter版本
	SET_U32 ( content ,  1) ;
	
	//	客户id
	SET_U32 ( content ,  i51AdeCnfGetCustomerId()) ;
	
	//	项目id
	SET_U32 ( content ,  i51AdeCnfGetProjectId()) ;
	
	//	移植批号
	#if 0
	SET_U32 ( content , ((iU32)i51AdeOsLog));
	SET_U32 ( content , ((iU32)i51AdeOsFileOpen));
	SET_U32 ( content , ((iU32)i51AdeSmsSend));
	SET_U32 ( content , ((iU32)i51AdeStdWSprintf));
	#else
	{
	iU32 length = 0 ;
	i51KernelGetBatchCode ( (iU8*)content , 16 , &length ) ;
	content += length ;
	}
	#endif

	//	大小端
	{
		iU8 port = 0 ;
		SET_U8 ( content , port ) ;
	}
	
	//	客户平台标示
	{
	SET_U8 ( content , strlen (i51AdeCnfGetPlatformId()) ) ;		
	strncpy ( content, i51AdeCnfGetPlatformId() , strlen ( i51AdeCnfGetPlatformId() ) ) ;
	content += strlen (i51AdeCnfGetPlatformId()) ;
	}
	
	//	分辨率宽
	//	分辨率高
	{
		//	得到宽和高
		iU16 LCDWidtrh , LCDHeight ;
		i51AdeMmiGetScreenScale ( &LCDWidtrh , &LCDHeight ) ;
		SET_U16 ( content, LCDWidtrh ) ;   
    		SET_U16 ( content, LCDHeight ) ;  
	}
	
	//	smsc
	//	imsi
	//	imei
	{
		iS32 *SimCardInfo = NULL ;
		iS8 *SMSC = NULL ;
		iS8 *SMSI = NULL ;
		iS8 *IMEI = NULL ;
		
		SimCardInfo = i51AdeSmsGetSims () ;
		SMSC = i51AdeSmsGetCenter ( SimCardInfo[1] ) ;
		SMSI = i51AdeSmsGetImsi ( SimCardInfo[1] ) ;
		IMEI = i51AdeSmsGetImei () ;

		SET_U8 ( content , strlen (SMSC) ) ;
		strncpy ( content, SMSC , strlen ( SMSC ) ) ;
		content += strlen ( SMSC ) ;

		SET_U8 ( content , strlen (SMSI) ) ;
		strncpy ( content, SMSI , strlen ( SMSI ) ) ;
		content += strlen ( SMSI ) ;

		SET_U8 ( content , strlen (IMEI) ) ;
		strncpy ( content, IMEI , strlen ( IMEI ) ) ;
		content += strlen ( IMEI ) ;

	}

	//	通话时长(单位分钟)
	SET_U32 ( content ,  i51AdeOsGetCallingTime ()/60) ;

	//	T卡总大小
	SET_U32 ( content ,100 ) ;
	
	//	T卡剩余大小
	SET_U32 ( content , 100 ) ;
	
	//	系统盘总大小
	SET_U32 ( content , 100 ) ;
	
	//	手机盘剩余空间
	SET_U32 ( content , 100 ) ;
	//	手机号码
	SET_U8 ( content , 0 ) ;

	
	{
		iU8 SerialNumber[16] = { 0 } ;
		iU32 SerialNumber32bit1 = 0 ;
		iU32 SerialNumber32bit2 = 0 ;
		#if 0
		if (i51AdeOsPermanitBufferRead ( SerialNumber , 12 ) ) {

			memcpy ( &SerialNumber32bit1 , SerialNumber+4 , 4 ) ;
			memcpy ( &SerialNumber32bit2 , SerialNumber+8 , 4 ) ;
		} else {

			i51AdeOsLog ( 0 , "SL - DL : i51SlDlBuildRequest R SN F" ) ;
		}
		#endif
		//	sn 
		
		SET_U32 ( content , SerialNumber32bit1);
		SET_U32 ( content , SerialNumber32bit2); 
	}

	//	APP 协议版本
	SET_U32 (content ,0) ;
	//	app版本
	SET_U32 (content ,0) ;
	
	mark = content ;
	//	跳过包体长度，需要回填
	content += 4 ;

	//	命令id
	SET_U16 ( content ,101 ) ; 
	{
	iS8 id[32] = {0} ;
	strcpy ( id , "pkig_init_backer" ) ;
	SET_U8 ( content , strlen(id) ) ;
	strncpy ( content , id ,  strlen(id) ) ;
	content += strlen(id) ;
	}
	

	{

		//	包体长度回填
		iU32 len = content - mark - 4 ;
		SET_U32 ( mark , len ) ; 

	}

	{

		
		//	CRC回填
		iS8* CrcBuffer = 0 ;
		iU32 CRC = 0 ;

		//	+4 跳过crc
		CrcBuffer = i51TestDl.cache + strlen(header) + i51HTTP_CONTENT_LENGTH + 4 ; 

		CRC = i51AdeStdCrc32 ( (iU8 *)CrcBuffer , content-CrcBuffer , NULL) ;
		CrcBuffer = CrcBuffer - 4 ;

		//	整个请求数据src
		SET_U32 ( CrcBuffer , CRC ) ;
		
	}

	//	Set http header
	{

		iS8 line [ 128 ] = { 0 } ;
		iS8* buffer = i51TestDl.cache ;
		iU32 headlen = strlen ( header ) ;
			
		memcpy ( buffer , header , headlen ) ;
		buffer = buffer + headlen ;

		sprintf ( line , "Content-Length:  %9d\r\n\r\n" , (iS8*)content - (i51TestDl.cache + strlen(header) + i51HTTP_CONTENT_LENGTH) ) ;

		memcpy ( buffer , line , i51HTTP_CONTENT_LENGTH ) ;
		
	}
	i51TestDl.sendlen = (iS8*)content - i51TestDl.cache ;
	
//	i51TestDl.Socket.SendTotall = content - i51TestDl.cache ;
	
}


static void i51TestSocBuildHttpHeader ( iS8* url , iS8 *host , iU32 port )
{
	#ifndef  i51SIMULATOR		
	sprintf ( (iS8*)i51TestDl.cache , i51HTTP_PROXY_HEADER_PATTERN ,url ,host , port , 0 ) ;	
	#else
	sprintf ( (iS8*)i51TestDl.cache , i51HTTP_HEADER_PATTERN  ,url , host , port , 0 ) ;
	#endif
	i51TestDl.sendlen = strlen (i51TestDl.cache) ;

}

static void i51TestSocConCallback ( iBOOL state , iS32 socket )
{


	//	author : Jelo
	//	since : 2011.6.23
	// 	(C) 2011 , PKIG Tech. Co., Ltd.	

	i51TestDl.recordtime = i51AdeOsGetTick() ;
	if ( iTRUE == state )
	{
		i51AdeOsLog ( 0 , "SL - DL : i51TestSocConCallback SOC CONN OK" ) ;					
		i51TestDl.state = i51_DL_STATE_SEND ;  
	}
	else if ( iFALSE == state )
	{
		//	网络出错
		i51TestSocSetNetStateCode ( iTRUE , NET_CODE_CONN_ERROR ) ;
		i51AdeOsLog ( i51_LOG_SC , "SL - DL : i51TestSocConCallback CONN FAIL" ) ;
		//i51TestDl.state = i51_SAP_DL_STATE_CLOSE ;	
		i51TestSocStopProcess () ;
		//	通知应用层
//		i51TestDl.User.callback ( i51_SL_DL_ERROR_NET , 0 , 0 , 0 , 0 ) ;
		
	}
	
	
}

static void i51TestSocSendCallback ( iBOOL state , iS32 socket , iU32 textlen )
{


	//	author : Jelo
	//	since : 2011.6.23
	// 	(C) 2011 , PKIG Tech. Co., Ltd.	

	//
	i51TestDl.recordtime = i51AdeOsGetTick() ;
	if ( iTRUE == state )
	{
		i51AdeOsLog ( 0 , "SL - DL : i51TestSocSendCallback SOC SEND OK\r\n" ) ;					
		i51TestDl.state = i51_DL_STATE_RECEIVE ;
	}
	else if ( iFALSE == state )
	{
		//	网络出错
		i51TestSocSetNetStateCode ( iTRUE , NET_CODE_SEND_ERROR ) ;
		i51AdeOsLog ( i51_LOG_SC , "SL - DL : i51TestSocSendCallback FAIL" ) ;				
		//i51TestDl.state = i51_SAP_DL_STATE_CLOSE ;	
		//	通知应用层
		i51TestSocStopProcess () ;
//		i51TestDl.User.callback ( i51_SL_DL_ERROR_NET , 0 , 0 , 0 , 0 ) ;
		
	}
	
}

static void i51TestSocRecvCallback ( iBOOL state , iS32 socket , iU8* buffer , iU32 textlen )
{
	iS8 NetState[10] = {0} ;
	i51TestDl.recordtime = i51AdeOsGetTick() ;
	if ( iFALSE == state )
	{
		//	网络出错
		i51TestSocSetNetStateCode ( iTRUE , NET_CODE_RECV_ERROR ) ;
		i51AdeOsLog ( i51_LOG_SC , "SL - DL : i51TestSocRecvCallback  FAIL" ) ;				
		//	通知应用层
	}else {

		strncpy ( netcode , (iS8*)buffer + 9 , 3 ) ;
		i51TestSocSetNetStateCode ( iTRUE , NET_CODE_OK ) ;
		strncpy ( NetState , (iS8*)buffer + 9 , 3) ;
		i51AdeOsLog ( i51_LOG_SC , "SL - DL : i51TestSocRecvCallback NET state = %s" , NetState ) ;
		i51AdeOsLog ( i51_LOG_SC , "SL - DL : i51TestSocRecvCallback  OK" ) ;	
	}
	i51TestSocStopProcess () ;
	
}


static void i51TestDlDownloading ( iU32 id )
{

	switch ( i51TestDl.state )
	{

		//	空转
		case i51_DL_STATE_IDLE :
		break ;
		
		case i51_DL_STATE_INIT :

			i51TestDl.socketId = i51AdeSocGet() ;

			if ( 0 > i51TestDl.socketId )
			{
				//	通知上层下载失败
//				i51TestDl.User.callback ( i51_SL_DL_ERROR_CREATESOC , 0 , 0 , 0 , 0 ) ;
				i51AdeOsLog ( i51_LOG_SC , "SL - DL : i51TestDlDownloading create S F" ) ;
				return ;
			}  

			i51TestDl.state = i51_DL_STATE_CONNECT ;

			
		//	注意此处没有break ; 

		//	连接，发送，接受最终结果会通知回调，因此只需状态的处理
		case i51_DL_STATE_CONNECT :
		{

	
			i51TestSocTimerStart (2) ;
			i51AdeSocConnect (
				i51TestDl.socketId , 
				&i51TestDl.server , 
				i51TestSocConCallback 
			) ;
			if ( i51TestDl.state <= i51_DL_STATE_CONNECT )
					i51TestDl.state = i51_DL_STATE_IDLE ;

		}
		
		break ;

		case i51_DL_STATE_SEND :

			 i51AdeSocSend ( 
					i51TestDl.socketId , 
					i51TestDl.cache, 
					i51TestDl.sendlen  , 
					i51TestSocSendCallback ) ;
			if ( i51TestDl.state <= i51_DL_STATE_SEND ) {
	
				i51TestDl.state = i51_DL_STATE_IDLE ;
			}			
			
		break ;

		case i51_DL_STATE_RECEIVE :
		{


				 i51AdeSocReceive ( 
					i51TestDl.socketId , 
					(iU8*)i51TestDl.cache , 
					I51_DL_DATA_LEN , 
					i51TestSocRecvCallback
				) ;
				if (i51TestDl.state ==  i51_DL_STATE_RECEIVE)
					i51TestDl.state = i51_DL_STATE_IDLE;
				
		}
		break ;

		case i51_DL_STATE_CLOSE :
  
			i51AdeOsLog ( i51_LOG_SC , "SL - DL : i51TestDlDownloading Stop\r\n" ) ;
			i51TestSocStopProcess () ;		  
			
		break ;
		
	}

	
}

static void i51TestSocInit (iU32 selectItem )
{
	#define BAIDU_ADDR	"121.37.60.20"
	#define BAIDU_HOST	"www.baidu.com"
	#define BAIDU_URL	"http://www.baidu.com"

	#define I51_ADDR		"211.99.189.20"
	#define I51_HOST		"ps.pkingame.net"
	#define I51_URL		"/12.jpg"

	#define I51_TEST_ADDR		"121.37.60.20"
	#define I51_TEST_HOST		"ps.test.pkingame.net"
	#define I51_TEST_URL		"/12.jpg"

	#define I51_PAY_ADDR		"211.99.189.20"
	#define I51_PAY_HOST		"fc.pkingame.net"
	#define I51_PAY_URL		"/MainServlet"

	#define I51_PAY_TEST_ADDR		"121.37.60.20"
	#define I51_PAY_TEST_HOST		"fc.test.pkingame.net"
	#define I51_PAY_TEST_URL		"/MainServlet"


	
	

	
	iS32 *LinkMode = NULL ;
	iS32 *sims = NULL ;
	iS8 addr[32] = {0} ;
	iS8 host[32] = {0} ;
	iS8 url[32] = {0} ;
	if ( selectItem == 0 ) {

		strcpy ( addr , BAIDU_ADDR ) ;
		strcpy ( host , BAIDU_HOST ) ;
		strcpy ( url , BAIDU_URL ) ;

	} else if ( selectItem == 1 ) {

		strcpy ( addr , I51_ADDR ) ;
		strcpy ( host , I51_HOST ) ;
		strcpy ( url , I51_URL ) ;		
	} else if ( selectItem == 2 ) {

		strcpy ( addr , I51_TEST_ADDR ) ;
		strcpy ( host , I51_TEST_HOST ) ;
		strcpy ( url , I51_TEST_URL ) ;		
	}else if ( selectItem == 3 ) {

		strcpy ( addr , I51_PAY_ADDR ) ;
		strcpy ( host , I51_PAY_HOST ) ;
		strcpy ( url , I51_PAY_URL ) ;		

	}else if ( selectItem == 4 ) {

		strcpy ( addr , I51_PAY_TEST_ADDR ) ;
		strcpy ( host , I51_PAY_TEST_HOST ) ;
		strcpy ( url , I51_PAY_TEST_URL ) ;		
	}else {

		i51AdeOsLog ( i51_LOG_SC , "SL - DL : i51TestSocInit Select error" ) ;		
		return ;
	}
	LinkMode =  i51AdeSocGetLinkLayerMode () ;
	sims = i51AdeSmsGetSims () ;
	i51TestDl.linkmode = LinkMode[1] ;
	i51TestDl.simnum = sims[1] ;
	i51TestDl.state = i51_DL_STATE_INIT ;
	i51AdeSocInetAton ( addr , &i51TestDl.server.ipaddr ) ;
	i51TestDl.server.port = 80 ;

	if ( selectItem >=0 && selectItem <=2 ) {
		
		i51TestSocBuildHttpHeader (url , host , i51TestDl.server.port) ;
	}else {

		i51SlDlBuildRequest (url , host , i51TestDl.server.port) ;
	}

	
}

static void i51TestSocBuildLinkLayerCallback (iBOOL ret)
{
	if (i51TestDl.monitortimer > 0) {
		
		i51AdeOsTimerStop ( i51TestDl.monitortimer) ;  
		i51TestDl.monitortimer = 0 ;
	}
	
	if (!ret) {

		i51AdeOsLog ( i51_LOG_SC , "SL - DL : i51TestSocBuildLinkLayerCallback F" ) ;
		return ;
	}

	i51TestDl.timer = i51AdeOsTimerCreate (1, 1) ;
	if ( i51TestDl.timer == 0  ) {

		i51TestSocSetNetStateCode ( iTRUE , NET_CODE_TIMER ) ;
		return ;
	}
	if (!i51AdeOsTimerStart ( i51TestDl.timer , i51TestDlDownloading , 100 ) ) {

		i51TestSocSetNetStateCode ( iTRUE , NET_CODE_TIMER ) ;
	}
	
}

static void i51TestSocStart (iU32 selectItem )
{

	i51TestSocInit (selectItem) ;
	i51TestSocTimerStart (1) ;
	i51TestDl.layerid = i51AdeSocCreateLinkLayer (i51TestDl.linkmode ,i51TestDl.simnum) ;
	if ( !i51TestDl.layerid ) {

		i51AdeOsLog ( i51_LOG_SC , "SL - DL : i51TestSocStart C Link FAIL" ) ;
	}
	i51AdeSocOpenLinkLayer ( (iU32)gaid , i51TestDl.layerid  , i51TestSocBuildLinkLayerCallback ) ;
}

static void i51TestSocDrawMenu ()
{

	#define I51_SIMPLEPORTAL_CAN_WIDTH	240
	#define I51_SIMPLEPORTAL_CAN_HEIGHT	32
	
	iU16 LCDWidth = 0 , LCDHeight = 0 ;
	iU32 *panelInfo = iNULL ;
	iU8 *panel = iNULL ;
	iU16 i = 0 , j= 0 , m = 0 , n = 0 ;
	iS8 text[32] ;
	iS8 pointstr[3][10] = {"." , ".." , "..."} ;
	static iU32 H = 0;
	
	i51AdeMmiGetScreenScale ( &LCDWidth , &LCDHeight ) ;
	panelInfo = i51AdeMmiGetPanel () ;
	panel = (iU8*)panelInfo[1] ;


	i51TestSocFillRect ( panel , LCDWidth , LCDHeight , 0 , 0 , 0 ) ;

	n = LCDHeight/32 ;
	for ( i = 1 ; i < n; i++ ) {

		i51TestSocFillRect ( panel , LCDWidth , 1 , 0 , i*I51_SIMPLEPORTAL_CAN_HEIGHT , 0x4A69 ) ;
	}


	for ( i = 0 ; i < n ; i ++ ) {
		
		iU16 color = 0xffff ;

		if ( m < i51TestSocCount ) {

			memset ( text , 0 , sizeof(text) ) ;
			sprintf ( text , "%d   %s" , i + 1 , i51TestSocList[m]) ;
			if ( i51_KEY_NUM1 == KeyDownNumber && 0 == i ) color = 0x7e0 ;
			else if ( i51_KEY_NUM2 == KeyDownNumber && 1 == i ) color = 0x7e0 ;
			else if ( i51_KEY_NUM3 == KeyDownNumber && 2 == i ) color = 0x7e0 ;
			else color = 0xffff ; 
			if ( !showNetstateCode && SelectDl== i ) {

				strcat ( text , pointstr[H] ) ;
			}
			CORENRDrawText (10 , i*32 +  (32-12)/2 , text , color ) ;		
		
		}


		m++ ;
	}

	if ( showNetstateCode ) {

		iS8 text[10] = {10} ;
		sprintf ( text , "%d  %s" , NetstateCode , netcode) ;
		CORENRDrawText (LCDWidth - 80 , SelectDl *32 +  (32-12)/2 , text , 0x07c0 ) ;	
		H  = 0 ;
	}

	i51AdeMmiUpdateScreen () ;
	H++ ;
	if ( H == 2 )
		H = 0 ;
	


}

iKIT iBOOL i51KitMain ( iS32 aid , iU32 message , void* parameter )
{

	//	TODO :	消息循环

	switch ( message )
	{
		case i51_MSG_PAINT :
			i51TestSocDrawMenu () ;
		break ;
	
		case i51_MSG_RUN :
		gaid = aid ;
		i51AdeOsScreenAlwaysLight (iTRUE) ;
		i51TestSocRegisterTouch ( 0 , 0* 32 , 240 , 32 , 1 , i51TestSocPadCB1) ;
		i51TestSocRegisterTouch ( 0 , 1* 32 , 240 , 32 , 1 , i51TestSocPadCB2) ;
		i51TestSocRegisterTouch ( 0 , 2* 32 , 240 , 32 , 1 , i51TestSocPadCB3) ;
		i51TestSocRegisterTouch ( 0 , 3* 32 , 240 , 32 , 1 , i51TestSocPadCB4) ;
		i51TestSocRegisterTouch ( 0 , 4* 32 , 240 , 32 , 1 , i51TestSocPadCB5) ;
		break ;
		
		case i51_MSG_INIT :
		{
	
			i51KIT_STATIC_KIT_SET(Adapter,parameter);
			i51KIT_STATIC_KIT_ASSERT(Adapter);
	
		}
		break ;

		case i51_MSG_KEYUP :
		{

			if ( (iU32)parameter == i51_KEY_RIGHT ) {
				
					//i51KernelSendRequest ( aid , L"Demo" , i51_REQ_EXIT , 0 , 0 ) ;

			}
			switch ( (iU32 ) parameter ) {
				case i51_KEY_NUM1 :
					i51TestSocPadCB1 ( 0 , 0 , 0 ) ;
				break ;
				case i51_KEY_NUM2 :
					i51TestSocPadCB2 ( 0 , 0 , 0 ) ;
				break ;
				case i51_KEY_NUM3 :
					i51TestSocPadCB3 ( 0 , 0 , 0 ) ;
				case i51_KEY_NUM4 :
					i51TestSocPadCB4 ( 0 , 0 , 0 ) ;
				case i51_KEY_NUM5 :
					i51TestSocPadCB5 ( 0 , 0 , 0 ) ;
				break ;
			}
		
			
		KeyDownNumber = -1 ;
		}
		break ;

		case i51_MSG_KEYDOWN :
			KeyDownNumber = (iU32 ) parameter ;
		break ;
		
		case i51_MSG_PADUP :
			i51TestSocTouchDo (i51_MSG_PADUP , (iU32)parameter) ;
		break ;
		case i51_MSG_PADDOWN :
			i51TestSocTouchDo (i51_MSG_PADDOWN , (iU32)parameter) ;
		break ;
		case i51_MSG_PADMOVE :
			i51TestSocTouchDo (i51_MSG_PADMOVE , (iU32)parameter) ;		
		case i51_MSG_PAUSE :
		break ;

		case i51_MSG_EXIT :
			i51AdeOsScreenAlwaysLight (iFALSE) ;
		break ;
		

	}

	

	return iTRUE ;

}

iKIT iU32* i51KitPort ()
{

	//	TODO : Initialize API Port for external layer
	
	//	i51KIT_DYNAMIC_KIT_MAPING_BEGIN(DlDemo)
	//	i51KIT_DYNAMIC_KIT_MAPING_END 

	//	return i51KIT_DYNAMIC_KIT_GET_MAPING(DlDemo) ;

}
