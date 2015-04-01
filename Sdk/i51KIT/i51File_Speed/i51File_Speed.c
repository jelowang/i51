#include "i51.h"
#include "i51KitAde.h"
#include "i51KitG2.h"
#include "i51KitStd.h"

#include "i51File_Speed.h"

i51KIT_STATIC_KIT_DEF(Adapter);
i51KIT_DYNAMIC_KIT_DEF(i51KitG2);
i51KIT_DYNAMIC_KIT_DEF(i51KitStd);

#define  FILE_SIZE  1024*300

static iU16 *i51fileWR = NULL;
static iFILE pFile = 0;
static iU32 results = 0 ;
static iU32 nWritten;
static iU8* g_Data = NULL;
static iS32 g_padcount = 0;
static iS32 tick_s = 0;
static iS32 tick_e = 0;
iFILE TypeheadHandle = iNULL;
iCHAR TestBuff[64] = {0};
iS32 Mips = 0;
static iBOOL rest = 0;
static iS32 g_fontheight =  1;
iS32 g_WRspeed[16] = {0};

static void test_InitData()
{
	g_Data =  (iU8*)i51AdeOsMalloc(FILE_SIZE,__FILE__,__LINE__);
	i51AdeStdMemset16(g_Data,'a',FILE_SIZE);
	i51KitG2CleanScreen(i51KitG2COLOR(7, 24, 40),NULL);
	i51fileWR = L"i51fileWR.log";

	i51AdeOsLog(0,"g_Data:0%x",g_Data);
}

static void test_TextShow(iCHAR * text,iU16 Y_height,iCOLOR color)
{
	iU16 lwidth = 0 ;
	iU16 lheight = 0 ;
	iRECT rect = {0};
	iU32 DrawWidth = 0;
	iU32 DrawHeight = 0;
	iU16 HeightEnd = 0;
	iU16 HeightHead = 0;

	i51AdeMmiGetScreenScale ( &lwidth , &lheight ) ;
	HeightEnd = lheight/12;
	HeightHead = lheight/10;

	rect.X = 5;
	rect.Y = HeightHead+Y_height;
	rect.Height = lheight - HeightHead-HeightEnd-1;
	rect.Width = lwidth-6;

	i51AdeOsLog(0,"text:%s,TypeheadHandle:%s",text,TypeheadHandle);
	i51KitG2FontDraw (TypeheadHandle,0,text ,0,&rect , &rect,0, 0,  0,  color , 0, 0);
	i51AdeOsLog(0,"i51KitG2FontDraw finish");
	i51KitG2Update();
}

static void test_File_WriteSpeed()
{
	i51AdeOsLog(0,"test_File_WriteSpeed[1111111111111]");

	if (i51AdeOsFileIsExist(i51fileWR) == iTRUE)
	{
		pFile = i51AdeOsFileOpen(i51fileWR, i51_OS_FL_EXISTING_OPEN|i51_OS_FL_WRITE,__FILE__,__LINE__);
	}
	else
	{
		pFile = i51AdeOsFileOpen(i51fileWR, i51_OS_FL_ALWAYS_CREATE|i51_OS_FL_WRITE,__FILE__,__LINE__);
	}
	if (pFile !=0)
	{
		i51AdeOsLog(0,"pFile:%d,g_Data:0%x",pFile,g_Data);

		tick_s = i51AdeOsGetTick();
		i51AdeOsFileWrite(pFile,g_Data,FILE_SIZE,0);
		tick_e = i51AdeOsGetTick();

		Mips = (tick_e - tick_s);
		i51AdeOsFileClose(pFile);
		memset(TestBuff,0,sizeof(TestBuff));
		sprintf(TestBuff,"i51AdeOsFileWrite 300KB total:%d ms,speed:%d KB/ms",Mips,300/Mips);
		i51AdeOsLog(0,"TestBuff%s",TestBuff);
		test_TextShow(TestBuff,g_fontheight,i51KitG2COLOR(255,0,0));
	}
}

static void test_File_ReadSpeed()
{
	i51AdeOsLog(0,"test_File_ReadSpeed enter");
	pFile = i51AdeOsFileOpen(i51fileWR, i51_OS_FL_EXISTING_OPEN|i51_OS_FL_READ,__FILE__,__LINE__);
	i51AdeOsLog(0,"test_File_ReadSpeed pFile:0%x",pFile);
	if (pFile !=0)
	{
		//i51AdeOsFileGetSize(pFile,&size);

		tick_s = i51AdeOsGetTick();
		i51AdeOsFileRead ( pFile , g_Data , FILE_SIZE , 0) ;
		tick_e = i51AdeOsGetTick();

		Mips = (tick_e - tick_s);
		i51AdeOsFileClose(pFile);
		memset(TestBuff,0,sizeof(TestBuff));
		sprintf(TestBuff,"i51AdeOsFileRead 300KB total:%d ms,speed:%d KB/ms",Mips,300/Mips);
		test_TextShow(TestBuff,g_fontheight,i51KitG2COLOR(255,0,0));
	}

}

static void test_Exit()
{
	i51AdeOsLog(0,"test_Exit Enter Clean!g_Data:0%x,i51fileWR:0%x",g_Data,i51fileWR);
	if (g_Data != NULL)
	{
		i51AdeOsFree(g_Data);
		g_Data = NULL;
	}
	if (i51fileWR!=NULL)
		i51fileWR = NULL;
	i51AdeOsLog(0,"Exit __I51__ g_Data:0%x,i51fileWR:0%x",g_Data,i51fileWR);

}

static void i51KeyEvent( iU16 event,iU32 para)
{
	static iBOOL keyStatus[13] = {0};
	iU8 keyFouns = 0;

	switch ( (iU32 ) para ) {
		case i51_KEY_NUM1 :
			{
				if (g_padcount>20) break;
				if (g_padcount ==20)
				{
					iS32 ws = 0;
					iS32 rs = 0;
					iS32 i = 0;
					i51AdeOsLog(0,"g_fontheight%d",g_fontheight);
					test_TextShow("Finish!",g_fontheight+15,i51KitG2COLOR(255,0,0));

					for (i = 0;i<20;i++)
					{
						if (i%2==0)
						{
							ws += g_WRspeed[i];
						}
						else
						{
							rs += g_WRspeed[i];
						}
					}

					memset(TestBuff,0,sizeof(TestBuff));
					sprintf(TestBuff,"i51AdeOsFileWrite speed:%d KB/ms",3000/ws);
					test_TextShow(TestBuff,g_fontheight+30,i51KitG2COLOR(0,255,0));

					memset(TestBuff,0,sizeof(TestBuff));
					sprintf(TestBuff,"i51AdeOsFileRead speed:%d KB/ms",3000/rs);
					test_TextShow(TestBuff,g_fontheight+40,i51KitG2COLOR(0,255,0));
				}
				else
				{
					if (g_padcount%2==0)
					{
						test_File_WriteSpeed();
					}
					else
					{
						test_File_ReadSpeed();
					}
					g_WRspeed[g_padcount]=Mips;
					g_fontheight += 12;

				}	
				g_padcount++;
			}
			break ;
	}

}

iKIT iBOOL i51KitMain ( iS32 aid , iU32 message , void* parameter )
{

	//	TODO :	ÏûÏ¢Ñ­»·

	switch ( message )
	{
	case i51_MSG_PAINT :
		break ;

	case i51_MSG_RUN :
		{
			i51AdeOsLog(0,"Run Start[0000000]");
			TypeheadHandle = i51KitStdResOpen(L"i51File_Speed\\Typehead.re");
			rest = i51KitG2FontCachEnable(TypeheadHandle,0);
			test_InitData();
		}
		break ;

	case i51_MSG_INIT :
		{
			i51KIT_STATIC_KIT_SET(Adapter,parameter);
			i51KIT_STATIC_KIT_ASSERT(Adapter);

			i51KIT_DYNAMIC_KIT_LOAD(i51KitStd,L"PKiGAPI\\i51KitStd.kit");
			if ( iNULL == i51KIT_DYNAMIC_KIT_PROTOTYPE(i51KitStd) )
			{
				return 0; 
			}

			i51KIT_DYNAMIC_KIT_LOAD(i51KitG2,L"PKiGAPI\\i51KitG2.kit");			if ( iNULL == i51KIT_DYNAMIC_KIT_PROTOTYPE(i51KitG2) ) 
			{
				return 0;
			}
		}
		break ;
	case i51_MSG_KEYDOWN:
		{
			i51KeyEvent(i51_MSG_KEYDOWN,(iU32 ) parameter);
		}
		break;
	case i51_MSG_PADUP:
		{
			if (g_padcount>16) break;
			if (g_padcount ==16)
			{
				iS32 ws = 0;
				iS32 rs = 0;
				iS32 i = 0;
				i51AdeOsLog(0,"g_fontheight%d",g_fontheight);
				test_TextShow("Finish!",g_fontheight+20,i51KitG2COLOR(255,0,0));

				for (i = 0;i<16;i++)
				{
					if (i%2==0)
					{
						ws += g_WRspeed[i];
					}
					else
					{
						rs += g_WRspeed[i];
					}
				}

				memset(TestBuff,0,sizeof(TestBuff));
				sprintf(TestBuff,"i51AdeOsFileWrite speed:%d KB/ms",3000/ws);
				test_TextShow(TestBuff,g_fontheight+30,i51KitG2COLOR(0,255,0));

				memset(TestBuff,0,sizeof(TestBuff));
				sprintf(TestBuff,"i51AdeOsFileRead speed:%d KB/ms",3000/rs);
				test_TextShow(TestBuff,g_fontheight+40,i51KitG2COLOR(0,255,0));
			}
			else
			{
				if (g_padcount%2==0)
				{
					test_File_WriteSpeed();
				}
				else
				{
					test_File_ReadSpeed();
				}
				g_WRspeed[g_padcount]=Mips;
				g_fontheight += 12;

			}	
			g_padcount++;	
		}	
		break;
	case i51_MSG_PAUSE :
		break ;

	case i51_MSG_EXIT :
		{
			rest = i51KitG2FontCachDisable(TypeheadHandle,0);
			i51KitStdResClose(TypeheadHandle);
			TypeheadHandle = iNULL;

			i51KIT_DYNAMIC_KIT_UNLOAD(i51KitStd);
			i51KIT_DYNAMIC_KIT_UNLOAD(i51KitG2);

			test_Exit();
		}
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

	return 0 ;

}
