
#include "i51KitAde.h"
#include "i51KitFontDemo.h"
#include "i51KitG2.h"
#include "i51KitStd.h"

i51KIT_STATIC_KIT_DEF(Adapter);
i51KIT_DYNAMIC_KIT_DEF(i51KitG2);
i51KIT_DYNAMIC_KIT_DEF(i51KitStd);

static iU8    UseCach = iTRUE;
static iFILE TypeheadHandle = iNULL;

void TestFontDraw(void)
{
    static iCOLOR Color=0;
    iRECT DrawRect;
    static iHANDLE VP1 = iNULL;
    static iU16 X = 0,Y = 0;
//    iU16 * Str = (iU16 *)"深圳豆游网络科技";
//    iU16 * Str = (iU16 *)"我的一是在不了有和人这中大为上个国";
//    iU16 * Str = (iU16 *)"豆游科技豆游科技豆游科技";
    //iU8 * Str = (iU8 *)"的一是在不了有和人这中大为上个国我以要他时来用们生到作地于出就分对成会可主发年动同工也能下过子说产种面而方后多定行学法所民得经十三之进着等部度家电力里如水化高自二理起小物现实加量都两体制机当使点从业本去把性好应开它合还因由其些然前外天政四日那社义事平形相全表间样与关各重新线内数正心反你明看原又么利比或但质气第向道命此变条只没结解问意建月公无系军很情者最立代想已通并提直题党程展五果料象员革位入常文总次品式活设及管特件长求老头基资边流路级少图山统接知较将组见计别她手角期根论运农指几九区强放决西被干做必战先回则任取据处队南给色光门即保治北造百规热领七海口东导器压志世金增争济阶油思术极交受联什认六共权收证改清己美再采转更单风切打白教速花带安场身车例真务具万每目至达走积示议声报斗完类八离华名确才科张信马节话米整空元况今集温传土许步群广石记需段研界拉林律叫且究观越织装影算低持音众书布复容儿须际商非验连断深难近矿千周委素技备半办青省列习响约支般史感劳便团往酸历市克何除消构府称太准精值号率族维划选标写存候毛亲快效斯院查江型眼王按格养易置派层片始却专状育厂京识适属圆包火住调满县局照参红细引听该铁价严";
//    iU16 * Str = (iU16 *)"的一是在不了有和人这中大为上个国我以要他时来用们生到作地于出就分对成会可主发年动同工也能下过子说产种面而方后多定行学法所民得经十三之进着等部度家电力里如水化高自二理起小物现实加量都两体制机当使点从业本去把性好应开它合还因由其些然前外天政四日那社义事平形相全表间样与关各重新线内数正心反你明看原又么利比或但质气第向道命此变条只没结解问意建月公无系军很情者最立代想已通并提直题党程展五果料象员革位入常文总次品式活设及管特件长求老头基资边流路级少图山统接知较将组见计别她手角期根论运农指几九区强放决西被干做必战先回则任取据处队南给色光门即保治北造百规热领七海口东导器压志世金增争济阶油思术极交受联什认六共权收证改清己美再采转更单风切打白教速花带安场身车例真务具万每目至达走积示议声报斗完类八离华名确才科张信马节话米整空元况今集温传土许步群广石记需段研界拉林律叫且究观越织装影算低持音众书布复容儿须际商非验连断深难近矿千周委素技备半办青省列习响约支般史感劳便团往酸历市克何除消构府称太准精值号率族维划选标写存候毛亲快效斯院查江型眼王按格养易置派层片始却专状育厂京识适属圆包火住调满县局照参红细引听该铁价严";
    iU16 * Str = (iU16 *)"  1967年,里奇参与的第一个项目是multics,这是一个操作系统项目,被设计在大型计算机主机上运行,但是由于整个目标过于庞大,糅合了太多的特性,multics的性能很低,最终以失败而告终.multics项目的开发者之一肯.汤普逊则继续为ge-5开发软件,并最终编写了一个新操作系统项目,能够支持同时的多用户操作,也就是后来的unix.";
//    iS8 * Str = "6.strlen的结果要在运行的时候才能计算出来,是用来计算字符串的长度,不是类型占内存的大小.";
//    iU16 * Str = (iU16 *)"里奇参与的第一个项目!!!!!!!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}~";
//    iU16 * Str = (iU16 *)"里奇参与的第一个项目!!!!!!!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}~";
//    iU16 * Str = (iU16 *)"回主菜单";
    iU8 i;	

    DrawRect.X = 0;
    DrawRect.Y = 0;
    DrawRect.Width = 320;
    DrawRect.Height= 480;

#if 0
if(VP1==iNULL)
{
    VP1 = i51KitG2VPCreate(4, &DrawRect);
    i51KitG2VPSetActivity(VP1);
    DrawRect.Y = 20;
    i51KitG2FontDraw(TypeheadHandle,0,(void *)Str, 0, &DrawRect, 0,2,2, 0XF000, 0XFF00, i51KITG2_FONT_UNDERLINE|i51KITG2_FONT_BKCOLOR);
    DrawRect.Y = 80;
    i51KitG2FontDraw(TypeheadHandle,1,(void *)Str, 0, &DrawRect, 0,2,2, 0X0F00, 0XFF00, i51KITG2_FONT_UNDERLINE|i51KITG2_FONT_BKCOLOR);
    DrawRect.Y = 160;
    i51KitG2FontDraw(TypeheadHandle,2,(void *)Str, 0, &DrawRect, 0,2,2, 0X00F0, 0XFF00, i51KITG2_FONT_UNDERLINE|i51KITG2_FONT_BKCOLOR);
    DrawRect.Y = 250;
    i51KitG2FontDraw(TypeheadHandle,2,(void *)Str, 0, &DrawRect, 0,4,4, 0X00F0, 0XFF00, i51KITG2_FONT_UNDERLINE|i51KITG2_FONT_BKCOLOR);
}


    DrawRect.Y = 20;
//    i51KitG2FontDraw((void *)Str, 66, &DrawRect, 0, Color+500, 0XFF00, i51KITG2_FONT_UNDERLINE|i51KITG2_FONT_BKCOLOR);
    DrawRect.Y = 40;
//    i51KitG2FontDraw( TypeheadHandle , 0,(void *)Str, 0, &DrawRect, 2,2,0, Color+500, 0XFF00, i51KITG2_FONT_UNDERLINE|i51KITG2_FONT_BKCOLOR);
    i51KitG2VPRender(VP1, iNULL);
    i51KitG2Update();
#else
    Color += 5000;
    i51KitG2FontDraw(TypeheadHandle,2,(void *)Str, 0, &DrawRect, &DrawRect,0,0,0, Color, 0XFF00, i51KITG2_FONT_UNDERLINE|i51KITG2_FONT_BKCOLOR);
    i51KitG2Update();
#endif

}


iKIT iBOOL i51KitMain ( iS32 aid , iU32 message , void* parameter )
{

	//	TODO :	消息循环

	switch ( message )
	{
	        case i51_MSG_KEYUP :
			if ( UseCach)
			{
				UseCach = iFALSE;
				i51KitG2FontCachEnable(TypeheadHandle, 2);
			}
			else
			{
				UseCach = iTRUE;
				i51KitG2FontCachDisable(TypeheadHandle, 2);
			}
	             break;
			 
		case i51_MSG_PAINT :
			TestFontDraw();
			break ;
		case i51_MSG_INIT :
		{
	
			i51KIT_STATIC_KIT_SET(Adapter,parameter);
			i51KIT_STATIC_KIT_ASSERT(Adapter);
	             //	加载i51KitStd动态库
	             i51KIT_DYNAMIC_KIT_LOAD(i51KitStd,L"PKiGAPI\\i51KitStd.kit");
	             if ( iNULL == i51KIT_DYNAMIC_KIT_PROTOTYPE(i51KitStd) ) 
	             {
	                 return 0 ;
	             }
			 
	             //	加载i51KitStd动态库
	             i51KIT_DYNAMIC_KIT_LOAD(i51KitG2,L"PKiGAPI\\i51KitG2.kit");
	             if ( iNULL == i51KIT_DYNAMIC_KIT_PROTOTYPE(i51KitG2) ) 
	             {
	                 return 0 ;
	             }

TypeheadHandle = i51KitStdResOpen(L"i51KitFontDemo\\Typehead.re");
i51KitG2FontCachEnable(TypeheadHandle, 2);
//i51KitG2VPEnable();
i51KitG2CleanScreen(0, iNULL) ;
i51KitG2Update();
		}
		break ;

		case i51_MSG_PAUSE :
		break ;

		case i51_MSG_EXIT :
		break ;
		

	}

	

	return iTRUE ;

}
