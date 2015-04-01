
#include "i51KitAde.h"
#include "i51Installer.h"

i51KIT_STATIC_KIT_DEF(Adapter);
#define I51_INSTALLER_TOUCH_COUNT	5

//#define I51_INSTALLER_MENU

//#define  I51_INSTALLER_LCD
#ifdef I51_INSTALLER_LCD
#define  I51_INSTALLER_LCD_WIDTH	176
#define  I51_INSTALLER_LCD_HEIGHT	220
#endif

typedef void (*i51InstallerTouch) ( iU16 event , iS32 x , iS32 y ) ;
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
	i51InstallerTouch TouchCallBack ;


}I51_INSTALLER_TOUCH_INFO ;

static iSAPTEMPLATE i51SapBooterTemplate ;
//static iSAPMESSAGEBOX i51SapBooterMessagebox ;
static iSAPBUTTON  button1 = {0}, button2 = {0} ;
I51_INSTALLER_TOUCH_INFO i51InstallerTouchList[I51_INSTALLER_TOUCH_COUNT] ;
//注册触摸事件的个数
static iU16 i51InstallerRegisterTouchCount = 0  ;
//	显示的模式，0为界面，1为弹出框
static iU16 i51DrawMode = 0 ;
static iU16 i51InstallerAppName[32] = {0} ;
static iU32 KeyDownNumber = 0 ;
static iU32 appid = 0 ; 

//#define I51_INSTALLER_ENGLISH
#ifndef I51_INSTALLER_ENGLISH

//	取消
static iU32 i51InstallerCanelStrlen = 2 ;
static iU32 i51InstallerCanelStrWidth = 16 ;
static iU32 i51InstallerCanelStrHeight = 21 ;
static iU8 i51InstallerCanelStr[] = {

/*--  文字:  取  --*/
/*--  微软雅黑12;  此字体下对应的点阵为：宽x高=16x21   --*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0x77,0xC7,0x77,0xE6,
0x7F,0xE6,0x77,0xEE,0x77,0x6E,0x7F,0x7C,0x77,0x7C,0x77,0x38,0x7F,0xF8,0xFF,0xFC,
0x07,0xEE,0x07,0x87,0x00,0x00,0x00,0x00,0x00,0x00,

/*--  文字:  消  --*/
/*--  微软雅黑12;  此字体下对应的点阵为：宽x高=16x21   --*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xEE,0x67,0xFF,0x6F,0x3F,0x7C,
0x03,0xE0,0xE0,0x60,0x7F,0xFF,0x0E,0x07,0x7F,0xFF,0x7E,0x07,0x7F,0xFF,0xEE,0x07,
0xEE,0x07,0xEE,0x7E,0x00,0x00,0x00,0x00,0x00,0x00,

};

//	重试

static iU32 i51InstallerResetStrlen = 2 ;
static iU32 i51InstallerResetWidth = 16 ;
static iU32 i51InstallerResetHeight = 21 ;
static iU8 i51InstallerResetStr[] = {

/*--  文字:  重  --*/
/*--  微软雅黑12;  此字体下对应的点阵为：宽x高=16x21   --*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7F,0xFE,0x01,0x80,0xFF,0xFF,
0x01,0x80,0x7F,0xFE,0x71,0x8E,0x7F,0xFE,0x71,0x8E,0x7F,0xFE,0x01,0x80,0x7F,0xFE,
0x01,0x80,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,

/*--  文字:  试  --*/
/*--  微软雅黑12;  此字体下对应的点阵为：宽x高=16x21   --*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xE0,0x3E,0xF0,0x37,0x38,0x30,0x3F,0xFF,
0x00,0x30,0xF0,0x38,0x70,0x38,0x7F,0xF8,0x73,0xB8,0x73,0xB8,0x7F,0xB8,0x7F,0x9B,
0x7B,0xFF,0x7F,0x1F,0x60,0x0F,0x00,0x00,0x00,0x00,


} ;
//	正在下载
static  iU32 i51InstallerDowndingStrlen = 4 ;
static iU32 i51InstallerDowndingStrWidth = 12 ;
static iU32 i51InstallerDowndingStrHeight = 12 ;
static iU8 i51InstallerDowndingStr[] = {

/*--  文字:  正  --*/
/*--  宋体9;  此字体下对应的点阵为：宽x高=12x12   --*/
/*--  宽度不是8的倍数，现调整为：宽度x高度=16x12  --*/
0x00,0x40,0xFF,0xE0,0x04,0x00,0x04,0x00,0x24,0x00,0x27,0xC0,0x24,0x00,0x24,0x00,
0x24,0x00,0x24,0x00,0xFF,0xE0,0x00,0x00,

/*--  文字:  在  --*/
/*--  宋体9;  此字体下对应的点阵为：宽x高=12x12   --*/
/*--  宽度不是8的倍数，现调整为：宽度x高度=16x12  --*/
0x08,0x00,0xFF,0xE0,0x08,0x00,0x12,0x00,0x22,0x00,0x22,0x00,0x6F,0xC0,0xA2,0x00,
0x22,0x00,0x22,0x00,0x3F,0xE0,0x00,0x00,

/*--  文字:  下  --*/
/*--  宋体9;  此字体下对应的点阵为：宽x高=12x12   --*/
/*--  宽度不是8的倍数，现调整为：宽度x高度=16x12  --*/
0x00,0x40,0xFF,0xE0,0x04,0x00,0x04,0x00,0x07,0x00,0x04,0x80,0x04,0x40,0x04,0x00,
0x04,0x00,0x04,0x00,0x04,0x00,0x00,0x00,

/*--  文字:  载  --*/
/*--  宋体9;  此字体下对应的点阵为：宽x高=12x12   --*/
/*--  宽度不是8的倍数，现调整为：宽度x高度=16x12  --*/
0x12,0x80,0x7E,0x40,0x12,0x00,0xFF,0xE0,0x22,0x00,0xFE,0x40,0x52,0x80,0x7D,0x00,
0x11,0x20,0xFE,0xA0,0x14,0x60,0x00,0x00,

};

//	应用启动中
static iU32 i51InstallerAppStartStrlen = 5 ;
static iU32 i51InstallerAppStartStrWidth = 14 ;
static iU32 i51InstallerAppStartStrHeight = 20 ;
static iU8 i51InstallerAppStartStr[] = {


/*--  文字:  应  --*/
/*--  微软雅黑11;  此字体下对应的点阵为：宽x高=14x20   --*/
/*--  宽度不是8的倍数，现调整为：宽度x高度=16x20  --*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x80,0x03,0x80,0x7F,0xFC,
0x70,0x00,0x73,0xB8,0x7B,0xB8,0x7D,0xB8,0x7D,0xF0,0x6F,0xF0,0xEE,0x70,0xE0,0x60,
0xFF,0xFC,0x00,0x00,0x00,0x00,0x00,0x00,

/*--  文字:  用  --*/
/*--  微软雅黑11;  此字体下对应的点阵为：宽x高=14x20   --*/
/*--  宽度不是8的倍数，现调整为：宽度x高度=16x20  --*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7F,0xFC,0x73,0x9C,0x73,0x9C,
0x7F,0xFC,0x73,0x9C,0x73,0x9C,0x7F,0xFC,0x63,0x9C,0x63,0x9C,0x63,0x9C,0xE3,0x9C,
0xC3,0xF8,0x00,0x00,0x00,0x00,0x00,0x00,

/*--  文字:  启  --*/
/*--  微软雅黑11;  此字体下对应的点阵为：宽x高=14x20   --*/
/*--  宽度不是8的倍数，现调整为：宽度x高度=16x20  --*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x07,0x00,0x7F,0xFC,0x70,0x1C,
0x70,0x1C,0x7F,0xFC,0x70,0x00,0x70,0x00,0x7F,0xFC,0x7C,0x1C,0xFC,0x1C,0xFF,0xFC,
0xDC,0x1C,0x00,0x00,0x00,0x00,0x00,0x00,

/*--  文字:  动  --*/
/*--  微软雅黑11;  此字体下对应的点阵为：宽x高=14x20   --*/
/*--  宽度不是8的倍数，现调整为：宽度x高度=16x20  --*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0xFE,0xC0,0x00,0xC0,
0x03,0xFC,0x00,0xDC,0xFF,0xDC,0x30,0xDC,0x71,0xDC,0x6D,0xDC,0xFF,0x9C,0xF7,0x9C,
0x07,0x78,0x00,0x00,0x00,0x00,0x00,0x00,

/*--  文字:  中  --*/
/*--  微软雅黑11;  此字体下对应的点阵为：宽x高=14x20   --*/
/*--  宽度不是8的倍数，现调整为：宽度x高度=16x20  --*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x00,0x03,0x00,0xFF,0xFC,
0xE3,0x1C,0xE3,0x1C,0xE3,0x1C,0xE3,0x1C,0xFF,0xFC,0xE3,0x1C,0x03,0x00,0x03,0x00,
0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

} ;

//	正在安装
static iU32 i51InstallerInstallingStrlen = 4 ;
static iU32 i51InstallerInstallingStrWidth = 12 ;
static iU32 i51InstallerInstallingStrHeight = 12 ;
static iU8 i51InstallerInstallingStr[] = {

	/*--  文字:  正  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=12x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=16x12  --*/
	0x00,0x40,0xFF,0xE0,0x04,0x00,0x04,0x00,0x24,0x00,0x27,0xC0,0x24,0x00,0x24,0x00,
	0x24,0x00,0x24,0x00,0xFF,0xE0,0x00,0x00,

	/*--  文字:  在  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=12x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=16x12  --*/
	0x08,0x00,0xFF,0xE0,0x08,0x00,0x12,0x00,0x22,0x00,0x22,0x00,0x6F,0xC0,0xA2,0x00,
	0x22,0x00,0x22,0x00,0x3F,0xE0,0x00,0x00,

	/*--  文字:  安  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=12x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=16x12  --*/
	0x04,0x00,0x7F,0xE0,0x40,0x20,0x88,0x40,0x08,0x00,0xFF,0xE0,0x11,0x00,0x19,0x00,
	0x06,0x00,0x0D,0x80,0xF0,0x60,0x00,0x00,

	/*--  文字:  装  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=12x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=16x12  --*/
	0x11,0x00,0x91,0x00,0x5F,0xE0,0x31,0x00,0xD7,0xC0,0x14,0x00,0xFF,0xE0,0x0A,0x40,
	0x32,0x80,0xD5,0x00,0x18,0xE0,0x00,0x00,

} ;

//	i51
static iU32 i51Installeri51Strlen = 3 ;
static iU32 i51Installeri51StrWidth = 8 ;
static iU32 i51Installeri51StrHeight = 16 ;
static iU8 i51Installeri51Str[] = {
	
	/*--  文字:  i  --*/
	/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
	0x00,0x00,0x00,0x30,0x30,0x00,0x00,0x70,0x10,0x10,0x10,0x10,0x10,0x7C,0x00,0x00,

	/*--  文字:  5  --*/
	/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
	0x00,0x00,0x00,0x7E,0x40,0x40,0x40,0x58,0x64,0x02,0x02,0x42,0x44,0x38,0x00,0x00,

	/*--  文字:  1  --*/
	/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
	0x00,0x00,0x00,0x10,0x70,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x7C,0x00,0x00,

} ;
#else


//	取消
static iU32 i51InstallerCanelStrlen = 6 ;
static iU32 i51InstallerCanelStrWidth = 9 ;
static iU32 i51InstallerCanelStrHeight = 16 ;
static iU8 i51InstallerCanelStr[] = {

/*--  文字:  C  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=9x16   --*/
/*--  宽度不是8的倍数，现调整为：宽度x高度=16x16  --*/
0x00,0x00,0x00,0x00,0x00,0x00,0x3F,0x00,0x63,0x00,0x63,0x00,0xC0,0x00,0xC0,0x00,
0xC0,0x00,0xC0,0x00,0xC0,0x00,0x63,0x00,0x66,0x00,0x3C,0x00,0x00,0x00,0x00,0x00,

/*--  文字:  a  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=9x16   --*/
/*--  宽度不是8的倍数，现调整为：宽度x高度=16x16  --*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3E,0x00,
0x63,0x00,0x1F,0x00,0x33,0x00,0x63,0x00,0x63,0x00,0x3F,0x80,0x00,0x00,0x00,0x00,

/*--  文字:  n  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=9x16   --*/
/*--  宽度不是8的倍数，现调整为：宽度x高度=16x16  --*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFE,0x00,
0x73,0x00,0x63,0x00,0x63,0x00,0x63,0x00,0x63,0x00,0xF7,0x80,0x00,0x00,0x00,0x00,

/*--  文字:  c  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=9x16   --*/
/*--  宽度不是8的倍数，现调整为：宽度x高度=16x16  --*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1E,0x00,
0x33,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x33,0x00,0x1E,0x00,0x00,0x00,0x00,0x00,

/*--  文字:  e  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=9x16   --*/
/*--  宽度不是8的倍数，现调整为：宽度x高度=16x16  --*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3E,0x00,
0x63,0x00,0x7F,0x00,0x60,0x00,0x60,0x00,0x63,0x00,0x3E,0x00,0x00,0x00,0x00,0x00,

/*--  文字:  l  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=9x16   --*/
/*--  宽度不是8的倍数，现调整为：宽度x高度=16x16  --*/
0x00,0x00,0x00,0x00,0x00,0x00,0x78,0x00,0x18,0x00,0x18,0x00,0x18,0x00,0x18,0x00,
0x18,0x00,0x18,0x00,0x18,0x00,0x18,0x00,0x18,0x00,0x7E,0x00,0x00,0x00,0x00,0x00,




};

//	重试

static iU32 i51InstallerResetStrlen = 5 ;
static iU32 i51InstallerResetWidth = 9 ;
static iU32 i51InstallerResetHeight = 16 ;
static iU8 i51InstallerResetStr[] = {

/*--  文字:  R  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=9x16   --*/
/*--  宽度不是8的倍数，现调整为：宽度x高度=16x16  --*/
0x00,0x00,0x00,0x00,0x00,0x00,0xFE,0x00,0x63,0x00,0x63,0x00,0x63,0x00,0x7E,0x00,
0x6C,0x00,0x6C,0x00,0x66,0x00,0x66,0x00,0x63,0x00,0xF3,0x80,0x00,0x00,0x00,0x00,

/*--  文字:  e  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=9x16   --*/
/*--  宽度不是8的倍数，现调整为：宽度x高度=16x16  --*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3E,0x00,
0x63,0x00,0x7F,0x00,0x60,0x00,0x60,0x00,0x63,0x00,0x3E,0x00,0x00,0x00,0x00,0x00,

/*--  文字:  t  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=9x16   --*/
/*--  宽度不是8的倍数，现调整为：宽度x高度=16x16  --*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x00,0x18,0x00,0x7E,0x00,
0x18,0x00,0x18,0x00,0x18,0x00,0x18,0x00,0x18,0x00,0x0E,0x00,0x00,0x00,0x00,0x00,

/*--  文字:  r  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=9x16   --*/
/*--  宽度不是8的倍数，现调整为：宽度x高度=16x16  --*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0x00,
0x3B,0x00,0x30,0x00,0x30,0x00,0x30,0x00,0x30,0x00,0xFC,0x00,0x00,0x00,0x00,0x00,

/*--  文字:  y  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=9x16   --*/
/*--  宽度不是8的倍数，现调整为：宽度x高度=16x16  --*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF7,0x80,
0x63,0x00,0x36,0x00,0x36,0x00,0x3C,0x00,0x1C,0x00,0x18,0x00,0x18,0x00,0xF0,0x00,






} ;
//	正在下载
static  iU32 i51InstallerDowndingStrlen = 11 ;
static iU32 i51InstallerDowndingStrWidth = 6 ;
static iU32 i51InstallerDowndingStrHeight = 12 ;
static iU8 i51InstallerDowndingStr[] = {

/*--  文字:  d  --*/
/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
0x00,0x00,0x18,0x08,0x08,0x38,0x48,0x48,0x48,0x3C,0x00,0x00,

/*--  文字:  o  --*/
/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
0x00,0x00,0x00,0x00,0x00,0x30,0x48,0x48,0x48,0x30,0x00,0x00,

/*--  文字:  w  --*/
/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
0x00,0x00,0x00,0x00,0x00,0xA8,0xA8,0x70,0x50,0x50,0x00,0x00,

/*--  文字:  n  --*/
/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
0x00,0x00,0x00,0x00,0x00,0xF0,0x48,0x48,0x48,0xEC,0x00,0x00,

/*--  文字:  l  --*/
/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
0x00,0x00,0xE0,0x20,0x20,0x20,0x20,0x20,0x20,0xF8,0x00,0x00,

/*--  文字:  o  --*/
/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
0x00,0x00,0x00,0x00,0x00,0x30,0x48,0x48,0x48,0x30,0x00,0x00,

/*--  文字:  a  --*/
/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
0x00,0x00,0x00,0x00,0x00,0x30,0x48,0x38,0x48,0x3C,0x00,0x00,

/*--  文字:  d  --*/
/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
0x00,0x00,0x18,0x08,0x08,0x38,0x48,0x48,0x48,0x3C,0x00,0x00,

/*--  文字:  i  --*/
/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
0x00,0x00,0x20,0x00,0x00,0x60,0x20,0x20,0x20,0x70,0x00,0x00,

/*--  文字:  n  --*/
/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
0x00,0x00,0x00,0x00,0x00,0xF0,0x48,0x48,0x48,0xEC,0x00,0x00,

/*--  文字:  g  --*/
/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
0x00,0x00,0x00,0x00,0x00,0x3C,0x48,0x30,0x40,0x78,0x44,0x38,



};

//	正在安装
static iU32 i51InstallerInstallingStrlen = 10 ;
static iU32 i51InstallerInstallingStrWidth = 6 ;
static iU32 i51InstallerInstallingStrHeight = 12 ;
static iU8 i51InstallerInstallingStr[] = {

/*--  文字:  i  --*/
/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
0x00,0x00,0x20,0x00,0x00,0x60,0x20,0x20,0x20,0x70,0x00,0x00,

/*--  文字:  n  --*/
/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
0x00,0x00,0x00,0x00,0x00,0xF0,0x48,0x48,0x48,0xEC,0x00,0x00,

/*--  文字:  s  --*/
/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
0x00,0x00,0x00,0x00,0x00,0x78,0x40,0x30,0x08,0x78,0x00,0x00,

/*--  文字:  t  --*/
/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
0x00,0x00,0x00,0x20,0x20,0x70,0x20,0x20,0x20,0x18,0x00,0x00,

/*--  文字:  a  --*/
/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
0x00,0x00,0x00,0x00,0x00,0x30,0x48,0x38,0x48,0x3C,0x00,0x00,

/*--  文字:  l  --*/
/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
0x00,0x00,0xE0,0x20,0x20,0x20,0x20,0x20,0x20,0xF8,0x00,0x00,

/*--  文字:  l  --*/
/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
0x00,0x00,0xE0,0x20,0x20,0x20,0x20,0x20,0x20,0xF8,0x00,0x00,

/*--  文字:  i  --*/
/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
0x00,0x00,0x20,0x00,0x00,0x60,0x20,0x20,0x20,0x70,0x00,0x00,

/*--  文字:  n  --*/
/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
0x00,0x00,0x00,0x00,0x00,0xF0,0x48,0x48,0x48,0xEC,0x00,0x00,

/*--  文字:  g  --*/
/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
0x00,0x00,0x00,0x00,0x00,0x3C,0x48,0x30,0x40,0x78,0x44,0x38,


} ;



//	应用启动中
static iU32 i51InstallerAppStartStrlen = 7 ;
static iU32 i51InstallerAppStartStrWidth = 8 ;
static iU32 i51InstallerAppStartStrHeight = 21 ;
static iU8 i51InstallerAppStartStr[] = {

/*--  文字:  s  --*/
/*--  微软雅黑12;  此字体下对应的点阵为：宽x高=7x21   --*/
/*--  宽度不是8的倍数，现调整为：宽度x高度=8x21  --*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7C,0xE6,0xE0,0xF0,0x7C,0x1E,0x0E,0xCE,
0x7C,0x00,0x00,0x00,0x00,

/*--  文字:  t  --*/
/*--  微软雅黑12;  此字体下对应的点阵为：宽x高=7x21   --*/
/*--  宽度不是8的倍数，现调整为：宽度x高度=8x21  --*/
0x00,0x00,0x00,0x00,0x00,0x70,0x70,0x70,0xFC,0x70,0x70,0x70,0x70,0x70,0x70,0x70,
0x3C,0x00,0x00,0x00,0x00,

/*--  文字:  a  --*/
/*--  微软雅黑12;  此字体下对应的点阵为：宽x高=7x21   --*/
/*--  宽度不是8的倍数，现调整为：宽度x高度=8x21  --*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3E,0x66,0x06,0x0E,0x7E,0xE2,0xE6,0xE6,
0x7E,0x00,0x00,0x00,0x00,

/*--  文字:  r  --*/
/*--  微软雅黑12;  此字体下对应的点阵为：宽x高=7x21   --*/
/*--  宽度不是8的倍数，现调整为：宽度x高度=8x21  --*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7E,0x78,0x70,0x70,0x70,0x70,0x70,0x70,
0x70,0x00,0x00,0x00,0x00,

/*--  文字:  t  --*/
/*--  微软雅黑12;  此字体下对应的点阵为：宽x高=7x21   --*/
/*--  宽度不是8的倍数，现调整为：宽度x高度=8x21  --*/
0x00,0x00,0x00,0x00,0x00,0x70,0x70,0x70,0xFC,0x70,0x70,0x70,0x70,0x70,0x70,0x70,
0x3C,0x00,0x00,0x00,0x00,

/*--  文字:  u  --*/
/*--  微软雅黑12;  此字体下对应的点阵为：宽x高=7x21   --*/
/*--  宽度不是8的倍数，现调整为：宽度x高度=8x21  --*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF2,0xF2,0xF2,0xF2,0xF2,0xF2,0x72,0x76,
0x3E,0x00,0x00,0x00,0x00,

/*--  文字:  p  --*/
/*--  微软雅黑12;  此字体下对应的点阵为：宽x高=7x21   --*/
/*--  宽度不是8的倍数，现调整为：宽度x高度=8x21  --*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7E,0x7A,0x70,0x70,0x70,0x70,0x70,0x7A,
0x7E,0x70,0x70,0x70,0x70,



} ;

//	i51应用启动中
static iU32 i51Installeri51Strlen = 11 ;
static iU32 i51Installeri51StrWidth = 9 ;
static iU32 i51Installeri51StrHeight = 16 ;
static iU8 i51Installeri51Str[] = {
	
/*--  文字:  i  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=9x16   --*/
/*--  宽度不是8的倍数，现调整为：宽度x高度=16x16  --*/
0x00,0x00,0x00,0x00,0x00,0x00,0x38,0x00,0x38,0x00,0x00,0x00,0x00,0x00,0x78,0x00,
0x18,0x00,0x18,0x00,0x18,0x00,0x18,0x00,0x18,0x00,0x7E,0x00,0x00,0x00,0x00,0x00,

/*--  文字:  5  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=9x16   --*/
/*--  宽度不是8的倍数，现调整为：宽度x高度=16x16  --*/
0x00,0x00,0x00,0x00,0x00,0x00,0x7F,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x7C,0x00,
0x76,0x00,0x03,0x00,0x03,0x00,0x63,0x00,0x66,0x00,0x3C,0x00,0x00,0x00,0x00,0x00,

/*--  文字:  1  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=9x16   --*/
/*--  宽度不是8的倍数，现调整为：宽度x高度=16x16  --*/
0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x00,0x78,0x00,0x18,0x00,0x18,0x00,0x18,0x00,
0x18,0x00,0x18,0x00,0x18,0x00,0x18,0x00,0x18,0x00,0x7E,0x00,0x00,0x00,0x00,0x00,

/*--  文字:     --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=9x16   --*/
/*--  宽度不是8的倍数，现调整为：宽度x高度=16x16  --*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

/*--  文字:  s  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=9x16   --*/
/*--  宽度不是8的倍数，现调整为：宽度x高度=16x16  --*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3F,0x00,
0x63,0x00,0x60,0x00,0x3E,0x00,0x03,0x00,0x63,0x00,0x7E,0x00,0x00,0x00,0x00,0x00,

/*--  文字:  t  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=9x16   --*/
/*--  宽度不是8的倍数，现调整为：宽度x高度=16x16  --*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x00,0x18,0x00,0x7E,0x00,
0x18,0x00,0x18,0x00,0x18,0x00,0x18,0x00,0x18,0x00,0x0E,0x00,0x00,0x00,0x00,0x00,

/*--  文字:  a  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=9x16   --*/
/*--  宽度不是8的倍数，现调整为：宽度x高度=16x16  --*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3E,0x00,
0x63,0x00,0x1F,0x00,0x33,0x00,0x63,0x00,0x63,0x00,0x3F,0x80,0x00,0x00,0x00,0x00,

/*--  文字:  r  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=9x16   --*/
/*--  宽度不是8的倍数，现调整为：宽度x高度=16x16  --*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0x00,
0x3B,0x00,0x30,0x00,0x30,0x00,0x30,0x00,0x30,0x00,0xFC,0x00,0x00,0x00,0x00,0x00,

/*--  文字:  t  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=9x16   --*/
/*--  宽度不是8的倍数，现调整为：宽度x高度=16x16  --*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x00,0x18,0x00,0x7E,0x00,
0x18,0x00,0x18,0x00,0x18,0x00,0x18,0x00,0x18,0x00,0x0E,0x00,0x00,0x00,0x00,0x00,

/*--  文字:  u  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=9x16   --*/
/*--  宽度不是8的倍数，现调整为：宽度x高度=16x16  --*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xE7,0x00,
0x63,0x00,0x63,0x00,0x63,0x00,0x63,0x00,0x67,0x00,0x3F,0x80,0x00,0x00,0x00,0x00,

/*--  文字:  p  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=9x16   --*/
/*--  宽度不是8的倍数，现调整为：宽度x高度=16x16  --*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFC,0x00,
0x76,0x00,0x63,0x00,0x63,0x00,0x63,0x00,0x66,0x00,0x7C,0x00,0x60,0x00,0xF0,0x00,



} ;
#endif

static iBOOL i51InstallerTempDraw ( iSAPTEMPLATE* template ) ;
static void TestAppCallback ( iCHAR* currentstep , iS32 state , iU32 step , iU32 total   , iU32 curdl , iU32 dlcount ) ;
static void i51InstallExit (iS32 aid) ;

static void i51InstallerRegisterTouch ( iU16 x , iU16 y , iU16 width , iU16 height , iU16 magnification , i51InstallerTouch TouchCallBack )
{
	//	 注册触屏区域
	i51InstallerTouchList[i51InstallerRegisterTouchCount].x = x ;
	i51InstallerTouchList[i51InstallerRegisterTouchCount].y = y ;
	i51InstallerTouchList[i51InstallerRegisterTouchCount].width = width ;
	i51InstallerTouchList[i51InstallerRegisterTouchCount].height = height ;
	i51InstallerTouchList[i51InstallerRegisterTouchCount].magnification = magnification  ;
	i51InstallerTouchList[i51InstallerRegisterTouchCount].TouchCallBack = TouchCallBack ;
	
	i51InstallerRegisterTouchCount ++ ;
	
}

static void i51InstallerClearTouch ()
{
	
	//	 触屏信息清空
	
	memset ( i51InstallerTouchList , 0 ,  sizeof ( i51InstallerTouchList ) ) ;
	i51InstallerRegisterTouchCount = 0 ;
	
}

static void i51InstallerTouchDo ( iU16 event , iU32 value )
{

	iU16 i = 0 ;
	iU32 x= 0 , y = 0 ;

	x =  ( value >>16 ) & 0x0000ffff ;
	y = value & 0x0000ffff ;

	for ( i = 0 ; i < i51InstallerRegisterTouchCount ; i++ ) {
		
		if ( 	x > i51InstallerTouchList[i].x - i51InstallerTouchList[i].width * (i51InstallerTouchList[i].magnification - 1 ) && 
			x < i51InstallerTouchList[i].x + i51InstallerTouchList[i].width + i51InstallerTouchList[i].width * (i51InstallerTouchList[i].magnification - 1 ) &&
			y > i51InstallerTouchList[i].y - i51InstallerTouchList[i].height* (i51InstallerTouchList[i].magnification - 1 ) && 
			y < i51InstallerTouchList[i].y + i51InstallerTouchList[i].height + i51InstallerTouchList[i].height* (i51InstallerTouchList[i].magnification - 1 )) {
			
			if ( i51_MSG_PADDOWN == event ) {
				
				i51InstallerTouchList[i].IsDown = iTRUE ;
				
			} 
			
			if ( i51InstallerTouchList[i].IsDown && i51_MSG_PADUP == event ) {
				
				if ( i51InstallerTouchList[i].TouchCallBack )
					i51InstallerTouchList[i].TouchCallBack ( event , x , y ) ;
			} 
			
			
			break ;	
			
		}
	}
	
	
}


static void i51InstallerPadLeftCB( iU16 event , iS32 x , iS32 y )
{
	//	重试，触摸处理

	if ( i51DrawMode == 1 ) {

		i51DrawMode = 0 ;
		i51KernelInstallApplication (i51InstallerAppName , TestAppCallback) ;
		memset ( &i51SapBooterTemplate.button[1] , 0 , sizeof(iSAPBUTTON) ) ;
		i51InstallerTempDraw (&i51SapBooterTemplate) ; 		
	}
}

static void i51InstallerPadRightCB( iU16 event , iS32 x , iS32 y )
{
	//	取消，触摸处理
	
	i51InstallExit (appid) ;
	
}

#if 1
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

	if ( panel == iNULL || panel[0] == 0 || panel[1] == 0 ) {

		 i51AdeOsLog ( 0 , "KIT - INS CORENRDrawTextModel NO BUFFER" ) ;
		return ;
	}
		
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

static void CORENRDrawText ( iS32 x , iS32 y , const iCHAR* text , iU16 color ) {

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
#else
static void CORENRDrawText ( iS32 x , iS32 y , const iCHAR* text , iU16 color ) {
}
#endif

static  iBOOL i51InstallerDrawText( iU8 * TypeheadData, iU32 TypeheadNum, iU32 TypeheadWidth, iU32 TypeheadHeight, iS16 TypeheadGap, iCOLOR Color, iS32 X, iS32 Y)
{
    // author : Otto
    // since : 2011.10.19
    // (C) 2011 , PKIG Tech. Co., Ltd.

    // iU16 * TypeheadData : 字模数据
    // iU32 TypeheadNum : 需要绘制的字模个数
    // iU32 TypeheadWidth : 字模宽度
    // iU32 TypeheadHeight : 字模高度
    // iS16 TypeheadGap : 字间距
    // iCOLOR Color : 绘制颜色
    // iU32 X : 绘制开始X坐标
    // iU32 Y : 绘制开始Y坐标

    // notes: 绘制字模到屏幕上
    iU8 TempTypehead1 = 0;
    iU8 TempTypehead2 = 0;
    iU16 ScreenWidth = 0;
    iU16 ScreenHeight= 0;
    iU16 * ScreenBuffer = iNULL;
    iU32 * LcdInfoArr = iNULL;
    iU32 CurrDrawWord = 0;
    iU32 CurrTypeheadPos = 0;
    iU32 CurrScreenBufferPos = 0;
    iU32 OldScreenBufferPos = 0;
    iU32 Temp = 0;

    if( i51AdeMmiGetScreenScale ( &ScreenWidth , &ScreenHeight ) == 0 )
    {
        return iFALSE;
    }

    LcdInfoArr =  i51AdeMmiGetPanel();
    if( LcdInfoArr == iNULL ||LcdInfoArr[0] == 0 || LcdInfoArr[1] == 0 )
    {
    	  i51AdeOsLog ( 0 , "KIT - INS i51InstallerDrawText NO BUFFER" ) ;
        return iFALSE;
    }
  
    ScreenBuffer = (iU16 *) LcdInfoArr[1] ;

    CurrScreenBufferPos = Y*ScreenWidth + X;
    OldScreenBufferPos = CurrScreenBufferPos;
	
    for( CurrDrawWord=0; CurrDrawWord<TypeheadNum; CurrDrawWord++)
    {
        for( Temp =0; Temp < TypeheadHeight; Temp++ ) 
        {
            TempTypehead1 = TypeheadData[CurrTypeheadPos++];
            if(TempTypehead1&(0X80)) ScreenBuffer[CurrScreenBufferPos] = Color;
            if(TempTypehead1&(0X40)) ScreenBuffer[CurrScreenBufferPos+1] = Color;
            if(TempTypehead1&(0X20)) ScreenBuffer[CurrScreenBufferPos+2] = Color;
            if(TempTypehead1&(0X10)) ScreenBuffer[CurrScreenBufferPos+3] = Color;
            if(TempTypehead1&(0X08)) ScreenBuffer[CurrScreenBufferPos+4] = Color;
            if(TempTypehead1&(0X04)) ScreenBuffer[CurrScreenBufferPos+5] = Color;
            if(TempTypehead1&(0X02)) ScreenBuffer[CurrScreenBufferPos+6] = Color;
            if(TempTypehead1&(0X01)) ScreenBuffer[CurrScreenBufferPos+7] = Color;
            if( TypeheadWidth<9 ) goto LOOP;			
            TempTypehead2 = TypeheadData[CurrTypeheadPos++];
            if(TempTypehead2&(0X80)) ScreenBuffer[CurrScreenBufferPos+8] = Color;
            if(TempTypehead2&(0X40)) ScreenBuffer[CurrScreenBufferPos+9] = Color;
            if(TempTypehead2&(0X20)) ScreenBuffer[CurrScreenBufferPos+10] = Color;
            if(TempTypehead2&(0X10)) ScreenBuffer[CurrScreenBufferPos+11] = Color;
            if(TempTypehead2&(0X08)) ScreenBuffer[CurrScreenBufferPos+12] = Color;
            if(TempTypehead2&(0X04)) ScreenBuffer[CurrScreenBufferPos+13] = Color;
            if(TempTypehead2&(0X02)) ScreenBuffer[CurrScreenBufferPos+14] = Color;
            if(TempTypehead2&(0X01)) ScreenBuffer[CurrScreenBufferPos+15] = Color;
            LOOP:
            CurrScreenBufferPos += ScreenWidth;
        }
		
        CurrScreenBufferPos = OldScreenBufferPos + TypeheadWidth + TypeheadGap;
        OldScreenBufferPos = CurrScreenBufferPos;
		
    }

    return iTRUE;
	
}



static void i51InstallerDrawRect ( iU8* panel , iS32 width , iS32 height , iS32 desx , iS32 desy , iCOLOR color)
{
	//	author : william
	//	since : 20111010
	//	PKIG Tech. Co., Ltd.
	//	画矩形

#if 0
	iS32 Rectx , Recty , RectWidth , RectHeight ;
	iU16 LCDWidth , LCDHeight ;
	iU8 * p = NULL  ;
	iU16 i ;

	
	i51AdeMmiGetScreenScale ( &LCDWidth , &LCDHeight ) ;
	
	if ( desx > LCDWidth - 1 ||  desy > LCDHeight - 1 || width <= 0 || height <= 0 || desx + width <= 0 || desy + height <= 0    )
		return  ;

	if ( desx < 0 ) {

		Rectx = 0 ;
		RectWidth = desx + width ;
		if ( RectWidth >  LCDWidth - 1 ) {

			RectWidth = LCDWidth - 1 ;

		}
		
	} else {

		Rectx = desx ;
		RectWidth = width ;
		if ( desx + width > LCDWidth - 1 ) {

			RectWidth = LCDWidth - 1 - desx ;
	
		}

	}

	if ( desy < 0  ) {

		Recty = 0 ;
		RectHeight = desy + height ;
		if ( RectHeight >  LCDHeight - 1 ) {

			RectHeight = LCDHeight - 1 ;

		}


	} else {

		Recty = desy ;
		RectHeight = height ;
		if ( desy + height > LCDHeight - 1 ) {

			RectHeight = LCDHeight - 1 - desy ;
	
		}	
	}

	for ( i = 0 ; i <= RectHeight ; i ++ ) {


		if ( i == 0 || i == RectHeight - 1 ) {
			
			p = panel + (Recty + i ) * LCDWidth * 2 +  Rectx * 2 ;
			i51AdeStdMemset16 ((iU16*) p , color , RectWidth * 2 ) ;
			

		} else {

			p = panel + (Recty + i ) * LCDWidth * 2 +  Rectx * 2 ;
			i51AdeStdMemset16 ( (iU16*)p , color , 2 ) ;		
			p = panel + (Recty + i ) * LCDWidth * 2 + ( Rectx + RectWidth ) * 2 ;
			i51AdeStdMemset16 ( (iU16*)p , color , 2 ) ;		

		}

	}
#endif

#if 1
	iS32 CleanX, CleanY;
	iS32 CleanW, CleanH;
	iS32 LcdBufPos = 0;
	iS32 TempX, TempY;
	iU16 lcdwidth , lcdheight ;
	iU16 *buffer = iNULL , *OneRowColorBuf = iNULL ;
	iU16 w = 0 ;
	i51AdeMmiGetScreenScale ( &lcdwidth , &lcdheight ) ;

	OneRowColorBuf = (iU16*)i51AdeOsMalloc(lcdwidth*2, __FILE__, __LINE__) ;

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

	if( (CleanX+CleanW)>lcdwidth) CleanW = lcdwidth-CleanX;
	if( (CleanY+CleanH)>lcdheight) CleanH = lcdheight-CleanY;

	for( TempX=0; TempX<CleanW ; TempX++)
	{
	    OneRowColorBuf[TempX] = color;
	}

	buffer = (iU16*)panel ;
	w  = CleanW ;
	CleanW = (CleanW<<1);	
	LcdBufPos = CleanY*lcdwidth + CleanX;
	for( TempY=0; TempY<CleanH; TempY++)
	{
	    if ( TempY == 0 || TempY == CleanH -1 ) {
			
	    	i51AdeStdMemcpy16(&(buffer[LcdBufPos]), OneRowColorBuf, CleanW);
	    } else {
	    
		buffer[LcdBufPos] = color ;
		buffer[LcdBufPos +w -1 ] = color ;
	    }
	    LcdBufPos += lcdwidth;
	}
	i51AdeOsFree(OneRowColorBuf) ;
#endif
}


static void i51InstallerFillRect ( iU8* panel , iS32 width , iS32 height , iS32 desx , iS32 desy , iCOLOR color  )
{

	//	author : william
	//	since : 20111010
	//	PKIG Tech. Co., Ltd.
	//	填充矩形

	#if 0
	iS32 Rectx , Recty , RectWidth , RectHeight ;
	iU16 LCDWidth , LCDHeight ;
	iU8 * p = NULL  ;
	iU16 i  , j ;

	
	i51AdeMmiGetScreenScale ( &LCDWidth , &LCDHeight ) ;
	
	if ( desx > LCDWidth - 1 ||  desy > LCDHeight - 1 || width < 0 || height <=0 || desx + width < 0 || desy + height < 0    )
		return  ;

	if ( desx < 0 ) {

		Rectx = 0 ;
		RectWidth = desx + width ;
		if ( RectWidth >  LCDWidth - 1 ) {

			RectWidth = LCDWidth - 1 ;

		}
		
	} else {

		Rectx = desx ;
		RectWidth = width ;
		if ( desx + width > LCDWidth - 1 ) {

			RectWidth = LCDWidth - 1 - desx ;
	
		}

	}

	if ( desy < 0  ) {

		Recty = 0 ;
		RectHeight = desy + height ;
		if ( RectHeight >  LCDHeight - 1 ) {

			RectHeight = LCDHeight - 1 ;

		}


	} else {

		Recty = desy ;
		RectHeight = height ;
		if ( desy + height > LCDHeight - 1 ) {

			RectHeight = LCDHeight - 1 - desy ;
	
		}	
	}

	for ( i = 0 ; i <= RectHeight; i ++ ) {

		for ( j = 0 ; j <= RectWidth ; j ++  ) {


			p = panel + (Recty + i ) * LCDWidth * 2 +  Rectx * 2 ;
			*(p+j*2) = ( color & 0xFF00 ) >> 8 ;
			*(p+j*2+1) =  color & 0x00FF  ;

		}


		/*
		
		p = panel + (Recty + i ) * LCDWidth * 2 +  Rectx * 2 ;
		i51AdeStdMemset16 ( (iU16*)p , color , RectWidth * 2 ) ;

		*/

	}
	#endif
	
#if 1
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
#endif
}

static iSAPTEMPLATE* i51InstallerCreateTemplate ( iU32 type , iHANDLE* bgimage , iCOLOR bgcolor ) 
{
	//	author : william
	//	since : 20111009
	//	PKIG Tech. Co., Ltd.
	//	创建界面模板。

	i51AdeStdMemset16 ( &i51SapBooterTemplate , 0 , sizeof (i51SapBooterTemplate) ) ;
	i51SapBooterTemplate.type = type ;
	i51SapBooterTemplate.bgimage = bgimage ;
	i51SapBooterTemplate.bgcolor = bgcolor ;

	return &i51SapBooterTemplate ;
	
}

static iBOOL i51InstallerTempInsertButtom ( iSAPTEMPLATE* template , iSAPBUTTON* Button  ) 
{
	//	author : william
	//	since : 20111009
	//	PKIG Tech. Co., Ltd.
	//	将按钮添加到模板。

	iU16 i ;

	for ( i = 0 ; i < I51_BUTTON_CONTROL_MAX_COUNT ; i ++  ) {

		if ( !(template->button[i].used))
			break ;
	}

	if ( i ==  I51_BUTTON_CONTROL_MAX_COUNT ) 
		return iFALSE ;

	i51AdeStdMemcpy16 ( &(template->button[i]) , Button , sizeof ( iSAPBUTTON )  ) ;
	template->button[i].used = iTRUE ;

	return iTRUE ;
	
}

static iBOOL i51InstallerTempInsertProgress ( iSAPTEMPLATE* template , iSAPPROGRESS* progress  ) 
{
	//	author : william
	//	since : 20111009
	//	PKIG Tech. Co., Ltd.
	//	将进度条添加到模板。

	iU16 i ;
	for ( i = 0 ; i < I51_PROCESS_CONTROL_MAX_COUNT ; i ++  ) {

		if ( !(template->process[i].used))
			break ;
	}

	if ( i ==  I51_PROCESS_CONTROL_MAX_COUNT ) 
		return iFALSE ;

	i51AdeStdMemcpy16 ( &(template->process[i]) , progress , sizeof ( iSAPPROGRESS )  ) ;
	template->process[i].used = iTRUE ;

	return iTRUE ;
	
}

static iBOOL i51InstallerThempInsertTip ( iSAPTEMPLATE* template , i51SAPTIP* tip ) 
{
	//	author : william
	//	since : 20111009
	//	PKIG Tech. Co., Ltd.
	//	将布告栏添加到模板。

	iU16 i ;

	for ( i = 0 ; i < I51_TIP_CONTROL_MAX_COUNT ; i ++  ) {

		if ( !(template->tip[i].used))
			break ;
	}

	if ( i ==  I51_TIP_CONTROL_MAX_COUNT ) 
		return iFALSE ;

	i51AdeStdMemcpy16 ( &(template->tip[i]) , tip , sizeof ( i51SAPTIP )  ) ;
	template->tip[i].used = iTRUE ;

	return iTRUE ;
	
}

static iBOOL i51InstallerTempDraw ( iSAPTEMPLATE* template ) 
{
	//	author : william
	//	since : 20111009
	//	PKIG Tech. Co., Ltd.
	//	绘制模板。


	iU16 i  ;
	iU32 *panelInfo =  NULL ;
	iU8 *panel = NULL ;
	iU16 LCDWidth ,  LCDHeight ;
	iS32 x , y ;
	iU16 text16bit[I51_TIP_INFO_MAX_LENGTH] ;
	panelInfo = i51AdeMmiGetPanel () ;
	if ( panelInfo == iNULL || panelInfo[0] == 0 || panelInfo[1] == 0 ) {

		i51AdeOsLog ( 0 , "KIT - INS i51InstallerDrawText NO BUFFER" ) ;
		return iFALSE ;
	}
	panel = (iU8*)panelInfo[1] ;

	i51AdeMmiGetScreenScale ( &LCDWidth , &LCDHeight ) ;

	#ifdef I51_INSTALLER_LCD
		LCDWidth = I51_INSTALLER_LCD_WIDTH ;
		LCDHeight = I51_INSTALLER_LCD_HEIGHT ;
	#endif

	i51InstallerFillRect ( panel , LCDWidth , LCDHeight , 0 , 0 , template->bgcolor ) ;
	
	
	i51InstallerFillRect ( panel , LCDWidth , 3 , 0 , 0 , 0x4A69 ) ;
	i51InstallerFillRect ( panel , LCDWidth , 3 , 0 , 2 , 0x4A49 ) ;
	i51InstallerFillRect ( panel , LCDWidth , 3 , 0 , 5 , 0x41E8 ) ;
	i51InstallerFillRect ( panel , LCDWidth , 3 , 0 , 8 , 0x3186 ) ;
	i51InstallerFillRect ( panel , LCDWidth , 3 , 0 , 11 , 0x2104) ;

#ifdef I51_INSTALLER_ENGLISH

	i51InstallerDrawText ( i51Installeri51Str ,
						i51Installeri51Strlen,
						i51Installeri51StrWidth,
						i51Installeri51StrHeight,
						0,
						65535,
						(LCDWidth - i51Installeri51StrWidth*i51Installeri51Strlen )/2 ,
						9 ) ;
#else
	i51InstallerDrawText ( i51Installeri51Str ,
						i51Installeri51Strlen,
						i51Installeri51StrWidth,
						i51Installeri51StrHeight,
						0,
						65535,
						(LCDWidth - (i51InstallerAppStartStrWidth + 2)*i51InstallerAppStartStrlen )/2 ,
						9 ) ;


	i51InstallerDrawText ( i51InstallerAppStartStr ,
						i51InstallerAppStartStrlen,
						i51InstallerAppStartStrWidth ,
						i51InstallerAppStartStrHeight ,
						0,
						65535,
						(LCDWidth - (i51InstallerAppStartStrWidth + 2)*i51InstallerAppStartStrlen )/2 +i51Installeri51StrWidth * i51Installeri51Strlen + 2  ,
						9 - 2 ) ;
#endif


	if ( template->bgimage != 0 ) {

		//画背景图

	}
	
	for ( i = 0 ; i < I51_BUTTON_CONTROL_MAX_COUNT ; i ++  ) {
		
		if ( template->button[i].used ) {
			

				if ( template->button[i].NomalImg == 0 ) {
					//素模板按钮
					i51InstallerFillRect ( panel , template->button[i].width , template->button[i].height , template->button[i].x , template->button[i].y , template->button[i].buttonNormalColor ) ;
					x = template->button[i].x + ( template->button[i].width - template->button[i].TextWidth *template->button[i].TextLength )/2 ;
					y = template->button[i].y + ( template->button[i].height - template->button[i].TextHeght )/2 ;
					i51InstallerDrawText ( template->button[i].text ,
										template->button[i].TextLength,
										template->button[i].TextWidth,
										template->button[i].TextHeght,
										0,
										template->button[i].TextColor,
										template->button[i].x + 5 ,
										template->button[i].y + 5 ) ;

				} else {


				}
				
			
		}

	}
	
	

	for ( i = 0 ; i < I51_TIP_CONTROL_MAX_COUNT ; i ++  ) {

		if ( template->tip[i].used ) {
			
				//	公布栏里能够显示多少行
				iU32 lineCount = 0 ;
				iU32 tipheight = 0 ;
				iU16 m = 0 ;
				iU16 n = 0;
				iCHAR text[50] ;
				i51InstallerFillRect ( panel , template->tip[i].width , template->tip[i].height , template->tip[i].x , template->tip[i].y , template->tip[i].FillColor ) ;		
				//lineCount = (template->tip[i].height -template->tip[i].TextY - 5) /( template->tip[i].TextHeght+5) ;
				tipheight = template->process[0].y - 20 - (template->tip[i].y + template->tip[i].TextY)  ;
				lineCount = tipheight/( i51InstallerDowndingStrHeight+5) ;
				if ( template->tip[i].TextCount> lineCount && I51_TIP_INFO_MAX_COUNT > lineCount)
					n = template->tip[i].TextCount - lineCount ;
				for ( m = n ; m < template->tip[i].TextCount ; m ++ ) {
				memset ( text16bit , 0 , sizeof(text16bit)) ;
				i51AdeStdAscii2Unicode ((iCHAR*)(template->tip[i].text[m] + 1)  , text16bit) ;
#ifdef I51_INSTALLER_MENU
				if ( template->tip[i].text[m][0] == '1' ) {
				i51InstallerDrawText ( i51InstallerDowndingStr ,
					i51InstallerDowndingStrlen,
					i51InstallerDowndingStrWidth,
					i51InstallerDowndingStrHeight,
					0,
					65535 ,
//					template->tip[i].TextColor,
					template->tip[i].x + template->tip[i].TextX ,
					template->tip[i].y + template->tip[i].TextX + (i51InstallerDowndingStrHeight+5)*(m-n) ) ;

					memset ( text , 0 , sizeof(text) ) ;
					sprintf ( text , "%s..." ,template->tip[i].text[m] + 1) ;
					CORENRDrawText ( template->tip[i].x + template->tip[i].TextX + i51InstallerDowndingStrWidth*i51InstallerDowndingStrlen + 5 ,
									template->tip[i].y + template->tip[i].TextY + (i51InstallerDowndingStrHeight+5)*(m-n) ,
									text ,
//									template->tip[i].TextColor
									65535
									);  
				
				} else  if ( template->tip[i].text[m][0] == '2' ){

				i51InstallerDrawText ( i51InstallerDownCompliteStr,
					4,
					i51InstallerDowndingStrWidth,
					i51InstallerDowndingStrHeight,
					2,
//					template->tip[i].TextColor,
					0x07c0 ,
					template->tip[i].x + template->tip[i].TextX ,
					template->tip[i].y + template->tip[i].TextX + (i51InstallerDowndingStrHeight+5)*(m-n) ) ;

//				memset ( text , 0 , sizeof(text) ) ;
//				sprintf ( text , "%s..." ,template->tip[i].text[m] + 1) ;
				CORENRDrawText ( template->tip[i].x + template->tip[i].TextX + (12+2)*4 + 5 ,
								template->tip[i].y + template->tip[i].TextY + (i51InstallerDowndingStrHeight+5)*(m-n) ,
								(iCHAR*)template->tip[i].text[m] + 1 ,
//								template->tip[i].TextColor
								0x07c0
								);  

				}else  if ( template->tip[i].text[m][0] == '3' ) {
				
				i51InstallerDrawText ( i51InstallerFailStr,
					10,
					i51InstallerDowndingStrWidth,
					i51InstallerDowndingStrHeight,
					2,
//					template->tip[i].TextColor,
					0xF800 ,
					template->tip[i].x + template->tip[i].TextX ,
					template->tip[i].y + template->tip[i].TextX + (i51InstallerDowndingStrHeight+5)*(m-n) ) ;
				}
#else
				if ( template->tip[i].text[m][0] != 0 ) {

			//	能够显示英文+ 点的个数
				iU32 TextCount = 0 ;
				iU32 width = 0 ;
				width = LCDWidth - 40 -( template->tip[i].x + template->tip[i].TextX + i51InstallerDowndingStrWidth*i51InstallerDowndingStrlen + 5) - 3*4 ;
				TextCount = width/9 + 3 ;
	
				i51InstallerDrawText ( i51InstallerDowndingStr ,
					i51InstallerDowndingStrlen,
					i51InstallerDowndingStrWidth,
					i51InstallerDowndingStrHeight,
					0,
					65535 ,
//					template->tip[i].TextColor,
					template->tip[i].x + template->tip[i].TextX ,
					template->tip[i].y + template->tip[i].TextX + (i51InstallerDowndingStrHeight+5)*(m-n) ) ;

					memset ( text , 0 , sizeof(text) ) ;
					sprintf ( text , "%s..." ,template->tip[i].text[m] + 1) ;
					
					if ( strlen (text) > TextCount ) {

							text[TextCount] = '\0' ;
							text[TextCount-1] = '.' ;
							text[TextCount-2] = '.' ;
							text[TextCount-3] = '.' ;
					}
					
					CORENRDrawText ( template->tip[i].x + template->tip[i].TextX + i51InstallerDowndingStrWidth*i51InstallerDowndingStrlen + 5 ,
									template->tip[i].y + template->tip[i].TextY + (i51InstallerDowndingStrHeight+5)*(m-n) ,
									text ,
//									template->tip[i].TextColor
									65535
									);  

					if ( template->tip[i].text[m][0] == '2' )
						CORENRDrawText ( LCDWidth - 40 ,
						template->tip[i].y + template->tip[i].TextY + (i51InstallerDowndingStrHeight+5)*(m-n) ,
						"[OK]" ,
	//									template->tip[i].TextColor
						0x07c0
						);  
					else if (template->tip[i].text[m][0] == '3' && template->tip[i].text[m][1] != 0 )
						CORENRDrawText ( LCDWidth - 40 ,
						template->tip[i].y + template->tip[i].TextY + (i51InstallerDowndingStrHeight+5)*(m-n) ,
						"[NO]" ,
	//									template->tip[i].TextColor
						0xF800
						);  
				
				} 
#endif

			}

		}	
			
	}

	
	for ( i = 0 ; i < I51_PROCESS_CONTROL_MAX_COUNT ; i ++  ) {

		if ( template->process[i].used ) {

				i51InstallerDrawRect ( panel ,
									template->process[i].width , 
									template->process[i].height ,
									template->process[i].x ,
									template->process[i].y , 
									template->process[i].EmptyColor ) ;
				i51InstallerFillRect ( panel ,
									template->process[i].seek ,
									template->process[i].height - 4 , 
									template->process[i].x + 2  , 
									template->process[i].y + 2 , 
									template->process[i].FillColor ) ;
			

				{
					iCHAR field[20] = {0} ;
					iCHAR ProgressStr[10] = {0} ;	
					iU8 PointStr[3][4] = { "." ,".." , "..." } ;
					iU32 posx = 0 , posy = 0 ;
					static iU16 m = 0 ;
					iU32 n = (template->process[i].seek * 100)/(template->process[i].width-4) ;

					sprintf ( ProgressStr , "%d%%" ,n ) ;

					if ( template->process[i].dlcount == 0 ) {
						
						sprintf ( field , "%d/%d" ,0 , 0) ;
					}else {

						sprintf ( field , "%d/%d" ,template->process[i].curdl , template->process[i].dlcount) ;
					}

					posx= template->process[i].x + 20 ;
					posy = template->process[i].y - 20 ;
					i51InstallerDrawText ( i51InstallerInstallingStr,
					i51InstallerInstallingStrlen,
					i51InstallerInstallingStrWidth,
					i51InstallerInstallingStrHeight,
					0,
					template->tip[i].TextColor,
					posx,
					posy ) ;

	
					posx =  posx + i51InstallerInstallingStrWidth*i51InstallerInstallingStrlen +2 ;		
					CORENRDrawText (posx ,
					posy ,
					(iCHAR*)PointStr[m] ,
					i51SapBooterTemplate.tip[0].TextColor); 

					posx = posx + 35 ;
					CORENRDrawText ( posx,
					posy ,
					(iCHAR*)field ,
					template->tip[i].TextColor); 

					posx =  LCDWidth/2 - 16 ;
					posy  = template->process[i].y + i51InstallerInstallingStrHeight ;
					CORENRDrawText ( posx,
					posy ,
					(iCHAR*)ProgressStr ,
					template->tip[i].TextColor); 
					if ( m == 2 )
						m = 0 ;
					else
						m ++ ;

				}					
			} 
			
		}
			
	i51InstallerFillRect ( panel , LCDWidth , 1, 0 , 30 , 42545) ;
	i51AdeMmiUpdateScreen () ;
}

/*
static iSAPMESSAGEBOX* i51InstallerCreateMessagebox ( iU32 type , iHANDLE* bgimage , iCOLOR color1  ,iCOLOR color2  ,iCOLOR color3  , iS32 x , iS32 y , iS32 width ,iS32 height , iS32 TextX , iS32 TextY ,iCHAR * text , iS32 TextLength , iCOLOR TextColor ) 
{
	//	author : william
	//	since : 20111009
	//	PKIG Tech. Co., Ltd.
	//	创建界面提示框。

	i51AdeStdMemset16 ( &i51SapBooterMessagebox , 0 , sizeof (i51SapBooterMessagebox) ) ;
	i51SapBooterMessagebox.type = type ;
	i51SapBooterMessagebox.bgimage = bgimage ;
	i51SapBooterMessagebox.color1= color1 ;
	i51SapBooterMessagebox.color2= color2 ;
	i51SapBooterMessagebox.color3= color3 ;
	i51SapBooterMessagebox.x = x ;
	i51SapBooterMessagebox.y = y ;
	i51SapBooterMessagebox.width = width ;
	i51SapBooterMessagebox.height = height ;
	i51SapBooterMessagebox.TextX = TextX ;
	i51SapBooterMessagebox.TextY = TextY ;
	i51AdeStdMemcpy16 ( i51SapBooterMessagebox.text , text ,  TextLength ) ;
	i51SapBooterMessagebox.TextColor = TextColor ;
	
	return &i51SapBooterMessagebox ;
	
}
*/

/*
static iBOOL i51InstallerMessageboxInsertButtom ( iSAPMESSAGEBOX* messagebox , iSAPBUTTON* Button  ) 
{
	//	author : william
	//	since : 20111009
	//	PKIG Tech. Co., Ltd.
	//	将按钮添加到提示框。

	iU16 i ;

	for ( i = 0 ; i < I51_BUTTON_CONTROL_MAX_COUNT ; i ++  ) {

		if ( !(messagebox->button[i].used))
			break ;
	}

	if ( i ==  I51_BUTTON_CONTROL_MAX_COUNT ) 
		return iFALSE ;

	i51AdeStdMemcpy16 ( &(messagebox->button[i]) , Button , sizeof ( iSAPBUTTON )  ) ;
	messagebox->button[i].used = iTRUE ;

	return iTRUE ;
	
}
*/

static void i51InstallDrawMenu ()
{

	
	iSAPTEMPLATE * ptemplate ; 
	iSAPPROGRESS progress = {0} ;
	i51SAPTIP tip = {0} ;
	iS32 flag = 0  ;
	iU16 LCDWidth , LCDHeight ;

//	iSAPMESSAGEBOX * pMessagebox ;

	i51AdeMmiGetScreenScale ( &LCDWidth , &LCDHeight ) ;

	#ifdef I51_INSTALLER_LCD
		LCDWidth = I51_INSTALLER_LCD_WIDTH ;
		LCDHeight = I51_INSTALLER_LCD_HEIGHT ;
	#endif

	ptemplate = i51InstallerCreateTemplate ( i51_SAP_GUI_TEMPLATE_NOT_ADEPTIVE , 0 , 0 ) ;

/****************左按钮************************/
	button1.x = 0 ;
//	button1.y = 296*100/320*LCDHeight/100 ;
	button1.y = LCDHeight - i51InstallerResetHeight - 10 ;
	button1.width = 38 ;
	button1.height = 30 ;
	button1.buttonNormalColor = 0 ;
	button1.buttonSelectColor = 0 ;
	button1.text = i51InstallerResetStr ;
	button1.TextColor = 65535 ;
	button1.TextWidth= i51InstallerResetWidth;
	button1.TextHeght= i51InstallerResetHeight;
	button1.TextLength = i51InstallerResetStrlen ;

/****************右按钮************************/
//	button2.x = 197*100/240*LCDWidth/100 ;
	button2.x = LCDWidth - i51InstallerCanelStrWidth*i51InstallerCanelStrlen - 10 ;
	button2.y =  LCDHeight - i51InstallerCanelStrHeight - 10 ;
	button2.width = 38 ; 
	button2.height = 30 ;
	button2.buttonNormalColor = 0 ;
	button2.buttonSelectColor = 0 ;
	button2.text = i51InstallerCanelStr ;
	button2.TextColor = 65535 ;
	button2.TextWidth = i51InstallerCanelStrWidth;
	button2.TextHeght = i51InstallerCanelStrHeight;
	button2.TextLength = i51InstallerCanelStrlen ;

/****************公告栏************************/
//	i51AdeStdMemcpy16 (  tip.text , "tip: i51pk , game start" , 23 ) ;
	tip.x = 0 ;
	tip.y = 30 ;
	tip.FillColor = 0 ;
	tip.width = LCDWidth ;
	tip.height = 200*100/320*LCDHeight/100 ;
	tip.TextX = 10 ;
	tip.TextY = 10 ;
	tip.TextColor = 65535 ;
	tip.TextWidth = i51InstallerDowndingStrWidth ;
	tip.TextHeght = i51InstallerDowndingStrHeight;
	tip.TextLength = 12 ;
	
	

/****************进度条************************/
	progress.x = 42*100/240*LCDWidth/100 ;
	progress.y = 271*100/320*LCDHeight/100 ;
	progress.width = 156*100/240*LCDWidth/100 ;
	progress.height = 8 ;
	progress.seek = 0 ;
	progress.EmptyColor = 65535 ;
	progress.FillColor = 65535 ;
	progress.PointColor = 65535 ;
	
	//	按钮插入模板
//	i51InstallerTempInsertButtom ( ptemplate , &button1 ) ;
	i51InstallerTempInsertButtom ( ptemplate , &button2 ) ;
	//	公告栏插入模板
	i51InstallerThempInsertTip ( ptemplate , &tip ) ;
	//	进度条插入模板
	i51InstallerTempInsertProgress ( ptemplate , &progress ) ;
	//	显示模板
//	i51InstallerTempDraw ( ptemplate ) ;

//	ptemplate->process[0].seek = 20 ;
//	i51InstallerTempUpdateProcess ( ptemplate , &ptemplate->process[0] ) ;
//	i51InstallerTempDraw ( ptemplate ) ;
//	pMessagebox = i51InstallerCreateMessagebox ( i51_SAP_GUI_MSG_SIMPLE , 0 , 0 , 65535 , 12420 ,
//											2 , 205 , 233 , 112  ,
//											10 , 10 , "download fail !" , strlen("download fail !") , 0  ) ;
//	i51InstallerMessageboxInsertButtom ( pMessagebox , &button1  ) ;
//	i51InstallerMessageboxInsertButtom ( pMessagebox , &button2  ) ;

}



static void i51InstallerAddTipInfo ( iCHAR* text )
{

	iU32 n = i51SapBooterTemplate.tip[0].TextCount ;
	iU32 i = 0 ;
	if ( n < I51_TIP_INFO_MAX_COUNT ) {
		
		strcpy ( i51SapBooterTemplate.tip[0].text[n] , text ) ;
	} else {

		for ( i = 0 ; i < I51_TIP_INFO_MAX_COUNT - 1 ; i++ ) {

			memcpy ( i51SapBooterTemplate.tip[0].text[i] ,
					i51SapBooterTemplate.tip[0].text[i+1] , 
					sizeof( i51SapBooterTemplate.tip[0].text[0] ) ) ;
		}

		memset ( i51SapBooterTemplate.tip[0].text[I51_TIP_INFO_MAX_COUNT - 1] ,
				0 , 
				sizeof( i51SapBooterTemplate.tip[0].text[0] ) ) ;
		strcpy ( i51SapBooterTemplate.tip[0].text[I51_TIP_INFO_MAX_COUNT - 1] , text  ) ;
		
	}

	i51SapBooterTemplate.tip[0].TextCount ++ ;

}

static void TestAppCallback ( iCHAR* currentstep , iS32 state , iU32 step , iU32 total   , iU32 curdl , iU32 dlcount )
{
	//	 进度条偏移长度
	iU32 len ;
	iU16 n = 0 ;
	static iU16 m = 0 ;
	//	比较长度
	iU32 compareLen ;
	iCHAR text[I51_TIP_INFO_MAX_LENGTH + 1] = {0} ;

	n = i51SapBooterTemplate.tip[0].TextCount ;
	i51SapBooterTemplate.process[0].curdl = curdl ;
	i51SapBooterTemplate.process[0].dlcount = dlcount ;

	if (  state < 0 ) {

		//	第一次出现< 0 ，不可能前面没有提示信息		
		i51AdeOsLog ( 0 , "KIT - INS : TestAppCallback , %d" , state ) ;
		i51DrawMode = 1 ;
		i51SapBooterTemplate.tip[0].text[n-1][0] = '3' ;
		i51InstallerTempInsertButtom ( &i51SapBooterTemplate , &button1 ) ;
		i51InstallerTempDraw (&i51SapBooterTemplate) ;

	}  else {

		
		
		if ( n == 0 ) {

			memset (text , 0 ,I51_TIP_INFO_MAX_LENGTH + 1 ) ;
			strcpy ( text , "1" ) ;
			strcat ( text , currentstep ) ;
			i51InstallerAddTipInfo (text) ;
		}else if ( step == total && total > 0 ) {

			//	当前现在的文件下载完成
			i51SapBooterTemplate.tip[0].text[n-1][0] = '2' ;
			
		} else if ( strcmp( i51SapBooterTemplate.tip[0].text[n-1] + 1 , currentstep ) != 0 ) {

			//	新文件下载
			memset (text , 0 ,I51_TIP_INFO_MAX_LENGTH + 1 ) ;
			strcpy ( text , "1" ) ;
			strcat ( text , currentstep ) ;
			i51InstallerAddTipInfo (text) ;
		}else {

			//	下载中
			i51SapBooterTemplate.tip[0].text[n-1][0] = '1' ;
		}

		if ( total ) 
			len = step*100/total*(i51SapBooterTemplate.process[0].width-4)/100 ;
		else
			len = 0 ;
		i51SapBooterTemplate.process[0].seek = (iS32)len ;
	}
//	i51InstallerTempDraw (&i51SapBooterTemplate) ;
//	i51InstallerUpdateProgress () ;
	if ( state == 1 ) {
		
		//	下载完成，运行app
		i51KernelSendRequest ( appid , i51InstallerAppName , i51_REQ_LOADEX , 0 , 0 ) ;

	}


}


static void i51InstallExit (iS32 aid)
{
	//	需要调用推出安装的函数，此接口还未加上
		i51KernelInstallApplicationStop () ;
		i51KernelSendRequest ( aid , L"Demo" , i51_REQ_EXIT , 0 , 0 ) ;
}


iKIT iBOOL i51KitMain ( iS32 aid , iU32 message , void* parameter )
{

	static iBOOL flag = iFALSE ;
	static iU16 m = 0 ;

	//	TODO :	消息循环
	switch ( message )
	{


		case i51_MSG_KEYUP :
			if ( (iU32)parameter == i51_KEY_RIGHT && KeyDownNumber ==  i51_KEY_RIGHT ) {
					i51InstallExit (aid) ;
	
			}else if ( (iU32)parameter == i51_KEY_LEFT ) {

				if ( i51DrawMode == 1 ) {

					i51DrawMode = 0 ;
					i51KernelInstallApplication (i51InstallerAppName , TestAppCallback) ;
					memset ( &i51SapBooterTemplate.button[1] , 0 , sizeof(iSAPBUTTON) ) ;
					i51InstallerTempDraw (&i51SapBooterTemplate) ; 
				}
			}
			KeyDownNumber = -1 ;
		break ;
		
		case i51_MSG_PAINT :
			#if 0
			if ( !flag ) {

				i51InstallerTempDraw (&i51SapBooterTemplate) ;
				i51InstallerUpdateProgress () ;
				flag = iTRUE ;
			} else {

				if ( i51DrawMode == 0 )
					i51InstallerUpdateProgress () ;
			}
			#else
			if ( m == 2 )
				if ( i51DrawMode == 0 )
					i51InstallerTempDraw (&i51SapBooterTemplate) ;
			else
				i51AdeMmiUpdateScreen () ;
			#endif
			if ( m == 2 )
				m = 0 ;
			m ++ ;
		break ;

		case i51_MSG_RUN :
		{
			appid  = aid ;
			i51DrawMode = 0 ;
			memset ( i51InstallerAppName , 0 , sizeof(i51InstallerAppName) ) ;
	i51AdeStdWStrcpy ( i51InstallerAppName , (iU16*)parameter ) ;
//			i51AdeStdWStrcpy ( i51InstallerAppName , L"CutFruit" ) ;
			i51AdeOsScreenAlwaysLight (iTRUE) ;
			i51InstallerClearTouch () ;
			i51InstallDrawMenu () ;
			
			i51InstallerRegisterTouch ( button1.x ,
									  button1.y ,
									  button1.width ,
									  button1.height ,
									  1 ,
									  i51InstallerPadLeftCB) ;
			
			i51InstallerRegisterTouch ( button2.x ,
									  button2.y ,
									  button2.width ,
									  button2.height ,
									  1 ,
									  i51InstallerPadRightCB) ;
			i51KernelInstallApplication (i51InstallerAppName , TestAppCallback) ;
			

		}
		break ;
		case i51_MSG_INIT :
		{	

			i51KIT_STATIC_KIT_SET(Adapter,parameter);
			i51KIT_STATIC_KIT_ASSERT(Adapter);

		}
		break ;

		case i51_MSG_PAUSE :
		{
			//	挂起
			i51KernelInstallApplicationStop () ;
//			flag = iFALSE ;
		}
		break ;

		case i51_MSG_RESUME :
		{
			//	恢复
			i51InstallerTempDraw (&i51SapBooterTemplate) ;
			i51KernelInstallApplication (i51InstallerAppName , TestAppCallback) ;
//			flag = iTRUE ;
		}
		break ;

		case i51_MSG_EXIT :
			flag = iFALSE ;
			i51AdeOsScreenAlwaysLight (iFALSE) ;
		break ;

		case i51_MSG_KEYDOWN :
			KeyDownNumber = (iU32 ) parameter ;
		break ;
		case i51_MSG_PADUP :
			i51InstallerTouchDo (i51_MSG_PADUP , (iU32)parameter) ;
		break ;
		case i51_MSG_PADDOWN :
			i51InstallerTouchDo (i51_MSG_PADDOWN , (iU32)parameter) ;
		break ;
		case i51_MSG_PADMOVE :
			i51InstallerTouchDo (i51_MSG_PADMOVE , (iU32)parameter) ;
		break ;

		

	}

	

	return iTRUE ;

}

iKIT iU32* i51KitPort ()
{

	//	TODO : Initialize API Port for external layer

	return 0 ;

}