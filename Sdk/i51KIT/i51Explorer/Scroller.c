
#include "i51KitAde.h"
#include "Scroller.h"

i51KIT_STATIC_KIT_DEF(Adapter);


ScrollContent GobalVar;
// typedef enum tagBASE_KEY_MSG {
// 	BASE_KEY_DOWN       = 1/*i51_KEY_DOWN*/,
// 	BASE_KEY_UP         /*= i51_KEY_UP*/,
// 	BASE_KEY_LONG_PRESS /*= i51_KEY_LONG_PRESS*/,
// 	BASE_KEY_REPEAT     /*= i51_KEY_REPEAT*/,
// 	BASE_KEY_MAX        /*= i51_KEY_EVENT_MAX*/
// }BASE_KEY_MEG;

//iU32 

void i51ScrollExit()
{
	i51ListDestroy(&GobalVar.lv.ls);
	i51ListDestroy(&GobalVar.packList);
}
void i51ScrollSetState(STATE state)
{   
	int looper;
    GobalVar.state = state;
	GobalVar.bNeedFlushBg = iTRUE;
    for (looper = 0 ;looper < BTN_ID_MAX; looper++)
    {
		GobalVar.btn[looper].bNeedFlushed = iTRUE;
		GobalVar.btn[looper].preState = ITEM_STATE_IDLE;
        GobalVar.btn[looper].curState = ITEM_STATE_IDLE;
    }
}
void i51ScrollGetFileInfo ()
{
	i51FILEINFO  fiAppDir;
   i51FILEINFO  fIAppFile;
   iBOOL bSysDir;
   Node addNode,*pCurNode;
   iFILE fAppDir = iNULL;
   iFILE fAppFile = iNULL;
   iS32 looper;
   iS32 curLooper;
   static iWCHAR wszTempPath[MAX_LEN_OF_PATH_STR/2];
   static iWCHAR wszTemp[MAX_LEN_OF_PATH_STR/2];
   if ((fAppDir = i51AdeOsFileFind(L"*",&fiAppDir)) <= 0)
   {
	   return;
   }

   do
   {
	   bSysDir = iFALSE;
	   for(looper = 0; looper < SYS_INFO_DIR_ID_MAX;looper ++)
	   {   
		   memset(wszTempPath,0,sizeof(wszTempPath));
		   i51AdeStdAscii2Unicode(GobalVar.strSysInfoDir[looper],wszTempPath);
		   if (!i51AdeStdWStrcmp(fiAppDir.name,wszTempPath))
		   {
			   bSysDir = iTRUE;
			   i51AdeStdWStrcpy(wszTemp,wszTempPath);
			   i51AdeStdWStrcat(wszTemp,L"\\*");
			   if ((fAppFile = i51AdeOsFileFind(wszTemp,&fIAppFile)) <= 0)
			   {
				   continue;
			   }
			   do 
			   { 
				   if (i51PublicCheckUSMatchTail(fIAppFile.name,L".i51"))
				   {
#if 0
				       i51AdeStdWStrcpy(addNode.wszCaption,fIAppFile.name);
					   i51ListInsertNode(&GobalVar.lv.ls,&addNode);
					   i51ListPrint(&GobalVar.lv.ls);
#endif
				   }
				   else if (i51PublicCheckUSMatchTail(fIAppFile.name,L".kit"))
				   {
#if 0
					   i51AdeStdWStrcpy(addNode.wszCaption,fIAppFile.name);
					   i51ListInsertNode(&GobalVar.lv.ls,&addNode);
					   i51ListPrint(&GobalVar.lv.ls);
#endif
				   }
			   } while (i51AdeOsFileFindNext(fAppFile,&fIAppFile));
		   }
	   }
	   if (!bSysDir)
	   {
		   memset(wszTemp,0,sizeof(wszTemp));
		   i51AdeStdWStrcpy(wszTemp,fiAppDir.name);
		   i51AdeStdWStrcat(wszTemp,L"\\");
		   i51AdeStdWStrcat(wszTemp,fiAppDir.name);
		   i51AdeStdWStrcat(wszTemp,L".i51");
		   if (i51AdeOsFileIsExist(wszTemp))
		   {
			   i51AdeStdWStrcpy(addNode.wszCaption,fiAppDir.name);
			   addNode.type = ITEM_TYPE_APP;
			   i51ListInsertNodeSort(&GobalVar.lv.ls,&addNode);
		   }
		   //////////////
		   memset(wszTemp,0,sizeof(wszTemp));
		   i51AdeStdWStrcpy(wszTemp,fiAppDir.name);
		   i51AdeStdWStrcat(wszTemp,L"\\");
		   i51AdeStdWStrcat(wszTemp,fiAppDir.name);
		   i51AdeStdWStrcat(wszTemp,L".package");
		   if (i51AdeOsFileIsExist(wszTemp))
		   {
			   i51AdeStdWStrcpy(addNode.wszCaption,fiAppDir.name);
			   addNode.type = ITEM_TYPE_PACK;
			   i51ListInsertNodeSort(&GobalVar.packList,&addNode);
		   }
	   }

   }
   while(i51AdeOsFileFindNext(fAppDir,&fiAppDir));

FIND_END:
   if (fAppDir > 0)
   {
	   i51AdeOsFileFindClose(fAppDir);
   }
   if (fAppFile > 0)
   {
	   i51AdeOsFileFindClose(fAppFile);
   }
   
   pCurNode = GobalVar.lv.ls.pHead;

 //  _T("aaaa");
   pCurNode = GobalVar.lv.ls.pHead;
   i51AdeStdWStrcpy(addNode.wszCaption,W_UNPACK_TOOL);
   addNode.type = ITEM_TYPE_TOOL;
   if(pCurNode)
   {
	   i51ListInsertNodeBeforeOneNode(&GobalVar.lv.ls,&addNode,pCurNode);
   }
#if 0
   i51AdeStdWStrcpy(addNode.wszCaption, W_SYS_TOOL);
   addNode.type = ITEM_TYPE_TOOL;
   if(pCurNode)
   {
	   i51ListInsertNodeBeforeOneNode(&GobalVar.lv.ls,&addNode,pCurNode);
   }
#endif
}

iCOLOR i51ScrollGetRanColor()
{
// 	COLOR_TYPE_RED = 0,
// 		COLOR_TYPE_ORANGE,
// 		COLOR_TYPE_YELLOW,
// 		COLOR_TYPE_GREEN,
// 		COLOR_TYPE_CHING,
// 		COLOR_TYPE_BLUE,
// 		COLOR_TYPE_PURPLE,
	static int index = 0;
	iCOLOR ret = COLOR_ORANGE;
	if (index >= COLOR_TYPE_MAX)
	{
		index = 0;
	}
	switch(index)
	{
#if 0
	case COLOR_TYPE_RED:
		ret = COLOR_RED;
		break;
#endif
	case COLOR_TYPE_ORANGE:
		ret = COLOR_ORANGE;
		break;
	case COLOR_TYPE_YELLOW:
		ret = COLOR_YELLOW;
		break;
	case COLOR_TYPE_GREEN:
		ret = COLOR_GREEN;
		break;
	case COLOR_TYPE_CHING:
		ret = COLOR_CHING;
		break;
	case COLOR_TYPE_BLUE:
		ret = COLOR_BLUE;
		break;
	case COLOR_TYPE_PURPLE:
		ret = COLOR_PURPLE;
		break;
	default:
		break;
	}
	index ++;
	return ret;
}

void i51ScrollInitSetPos()
{
#define  BLOCK_HEIGHT  60
	Node *pCurNode;
	iU32 *lcd_address;
	iS16 startX,startY,tempW,tempH;
	i51AdeMmiGetScreenScale(&GobalVar.screenW,&GobalVar.screenH);
	lcd_address =  i51AdeMmiGetPanel();
	if (iNULL == lcd_address)
	{
		return;
	}
	GobalVar.pScreen = (iU8 *)lcd_address;
   
	//i51FrameItemInitRect(&GobalVar.lv.rcCtr,0,40,GobalVar.screenW,(GobalVar.screenH-40-40));
	i51FrameItemInitRect(&GobalVar.lv.rcCtr,0,0,GobalVar.screenW,(GobalVar.screenH-0-0));
	startX = 0;
	startY = GobalVar.lv.rcCtr.Y;
	pCurNode = GobalVar.lv.ls.pHead;
	while (pCurNode)
	{
		pCurNode->rc.X = startX;
		pCurNode->rc.Y = startY;
		pCurNode->rc.Width = GobalVar.lv.rcCtr.Width;
		pCurNode->rc.Height = BLOCK_HEIGHT;
		pCurNode->icIdleCompleten = COLOR_WHITE;
		pCurNode->icIdleUnCompleten = COLOR_RED;
		if(pCurNode->type == ITEM_TYPE_APP)
		{
			pCurNode->txtCompleten = COLOR_BLACK;
			pCurNode->txtUnCompleten = COLOR_RED;
		}
		else if(pCurNode->type == ITEM_TYPE_TOOL)
		{
			pCurNode->txtCompleten = COLOR_BLUE;
			pCurNode->txtUnCompleten = COLOR_RED;
		}

		pCurNode->icDown = COLOR_BIGBLUE;
		pCurNode->icon = i51ScrollGetRanColor();
		pCurNode->preState = ITEM_STATE_IDLE;
		pCurNode->curState = ITEM_STATE_IDLE;
#if !defined(WIN32)
		if(pCurNode->type == ITEM_TYPE_APP)
		{
			pCurNode->bCompleten = i51KernelCheckAppCompleten(pCurNode->wszCaption);
		}
		else if(pCurNode->type == ITEM_TYPE_TOOL)
		{
			pCurNode->bCompleten = iTRUE;
		}
#else
		pCurNode->bCompleten = iTRUE;
#endif
		//i51FrameShiftXY(&pCurNode->rc,GobalVar.lv.rcCtr.X,GobalVar.lv.rcCtr.Y);
		startY += pCurNode->rc.Height;
		if(pCurNode->pNext)
		{
           pCurNode = pCurNode->pNext;
		}
		else
		{
			break;
		}
	}
	tempW = 5;
	tempH = GobalVar.lv.rcCtr.Height*GobalVar.lv.rcCtr.Height/(GobalVar.lv.ls.sum*BLOCK_HEIGHT);
	startX = GobalVar.lv.rcCtr.Width - tempW-3;
    startY = GobalVar.lv.rcCtr.Y;
	i51FrameItemInitRect(&GobalVar.lv.bar.rc,startX,startY,tempW,tempH);
	//i51FrameShiftXY(&GobalVar.lv.bar.rc,GobalVar.lv.rcCtr.X,GobalVar.lv.rcCtr.Y);
	GobalVar.lv.bar.icIdle = COLOR_BLACK;
	GobalVar.lv.yRel = START_Y;
	GobalVar.lv.dir = SCROLL_DIRT_VERTICAL_UP;
	GobalVar.lv.hAbs = GobalVar.lv.ls.sum*BLOCK_HEIGHT;
	GobalVar.lv.bBarVisible = iTRUE;
	//LOG("List Total Height = %d",(GobalVar.lv.ls.sum*48));
}
void i51ScrollInitImageButtons()
{
	//i51WidgetCreateImageButton(&GobalVar.btn[BTN_ID_APP],,,c,);
	//iBOOL i51WidgetCreateImageButton(ImageButton *pBtn,iU32 imgAD,iRECT *pimageRc,iRECT *ptouchRc,iCOLOR bgIcidle,iCOLOR bgIcDown);
	iRECT tempRc;
	iS16 startX,startY,btnVSpace;
	tempRc.Height = 60;
	tempRc.Width = 150;
	tempRc.X = (GobalVar.screenW - tempRc.Width)/2;
	btnVSpace = tempRc.Height/2;
	startY = (GobalVar.screenH - (tempRc.Height*2 + btnVSpace*1))/2;


	tempRc.Y = startY;
	i51WidgetCreateImageButton(&GobalVar.btn[BTN_ID_UNPACKAGE_UNPACK],BTN_ID_UNPACKAGE_UNPACK,L"UnPackage",iNULL,&tempRc,iNULL,COLOR_LOWBLUE,COLOR_BIGBLUE);

	tempRc.Y += tempRc.Height+btnVSpace;
	i51WidgetCreateImageButton(&GobalVar.btn[BTN_ID_UNPACKAGE_BACK],BTN_ID_UNPACKAGE_BACK,L"Back",iNULL,&tempRc,iNULL,COLOR_LOWBLUE,COLOR_BIGBLUE);
}

void i51ScrollInit()
{   
	iS32 ret;
   iU32 tCount1,tCount2;
	memset(&GobalVar,0,sizeof(GobalVar));
	/////////////
	{
		   iS32 looper;
		   for(looper = 0; looper < SYS_INFO_DIR_ID_MAX;looper ++)
		   {
			   switch(looper)
			   {
			   case SYS_INFO_DIR_51PKPAY:
				   //i51AdeStdWStrcpy(GobalVar.strSysInfoDir[looper],"51pkpay");
				   strcpy(GobalVar.strSysInfoDir[looper],"51pkpay");
				   break;
			   case SYS_INFO_DIR_I51SYS:
				   //i51AdeStdWStrcpy(GobalVar.strSysInfoDir[looper],"..\\i51SYS");
				   strcpy(GobalVar.strSysInfoDir[looper],"..\\i51SYS");
				   break;
			   case SYS_INFO_DIR_PKIGAPI:
				   //i51AdeStdWStrcpy(GobalVar.strSysInfoDir[looper],"PKiGAPI");
				   strcpy(GobalVar.strSysInfoDir[looper],"PKiGAPI");
				   break;
			   }
		   }
	}
	/////////////

	i51ListInit(&GobalVar.lv.ls);
	i51ListInit(&GobalVar.packList);
	tCount1 = i51AdeOsGetTick();
    i51ScrollGetFileInfo();
	//SLOG("befor sort");
	
	i51ListPrint(&GobalVar.lv.ls);
	
	//i51ListSort(&GobalVar.lv.ls);
	tCount2 = i51AdeOsGetTick();
	SLOG(" = %d ",(tCount2-tCount1));
	//SLOG("after sort");
	//i51ListPrint(&GobalVar.lv.ls);
	i51ScrollInitSetPos();
	i51ScrollInitImageButtons();
	i51TimerInit();
	
	i51TimerRestart(TIMER_ID_AUTO_SCROLL);
	i51ScrollSetState(STATE_APP);
}


#if 0
void i51ScrollRenderLoop()
{
   static int flag = 1;
   iRECT rc;
   if (flag)
   {
	   flag = 0;
	   i51KitG2CleanScreenEx(COLOR_BLACK,iNULL);
	   if (GobalVar.lv.hAbs < GobalVar.lv.rcCtr.Height)
	   {
		   //i51FrameItemInitRect(&rc,0,GobalVar.lv.yRel,GobalVar.lv.rcCtr.Width,(GobalVar.lv.rcCtr.Height-GobalVar.lv.hAbs));
		   if (GobalVar.lv.ls.pHead)
		   {
			   i51KitG2CleanScreenEx(GobalVar.lv.ls.pHead->icIdleCompleten,&GobalVar.lv.rcCtr);
		   }

	   }
	   i51WidgetScrollDraw(&GobalVar.lv,0,0,&GobalVar.lv.rcCtr,SCROLL_REDRAW_LIMIT_NONE);

	   i51AdeMmiUpdateScreen();
   }
}
#endif

iBOOL i51ScrollCheckinZoom(iS16 x0,iS16 y0,iS16 x1,iS16 y1,iS16 offset)
{
	return ( (x1>x0-offset)&&(x1<(x0+offset))&&(y1>y0-offset)&&(y1<(y0+offset)) );
}
void i51ScrollLoadAppEffect()
{
	iRECT rect_up,rect_up_bg,rect_down,rect_down_bg,temp_rect;
	iU16 temp_width,temp_height;
	iS16 temp_total_y,move_offset,move_times,or_move_offset;
	int k;
	move_offset = 2;
	or_move_offset = move_offset;
	rect_down.X = 0;
	rect_down.Width = GobalVar.screenW;
	rect_down.Y = 0;
	rect_down.Height = GobalVar.screenH;

	rect_down_bg.X = 0;
	rect_down_bg.Width = GobalVar.screenW;
	rect_down_bg.Height = move_offset;
	k = 0;
	for(;;)
	{
		if(rect_down.Y > GobalVar.screenH || move_offset > GobalVar.screenH)
		{
			break;
		}
		move_offset = k*15;
		i51FrameMovePanel(&rect_down,0,rect_down.Y+move_offset);
		rect_down_bg.Y = rect_down.Y;
		rect_down_bg.Height = move_offset;
		i51KitG2CleanScreenEx(COLOR_BLACK,&rect_down_bg);
		i51AdeMmiUpdateScreen();
		rect_down.Y+=move_offset;
		rect_down.Height-=move_offset;
		k++;
	}
}
void i51ScrollDrawUnPackaging(int Index)
{  
#define  BLOCK_WIDTH 40
#define  BLOCK_SPACE 5
#define  BLOCK_SUM 4
   iRECT rcBlock[BLOCK_SUM];
   iCOLOR icBlock[BLOCK_SUM];
   int finalIndex;
   iS16 startX,startY;
   int looper;
   startX = (GobalVar.screenW - (2*BLOCK_WIDTH+BLOCK_SPACE))/2;
   startY = GobalVar.screenH/4;

   for (looper = 0 ; looper < BLOCK_SUM ; looper++)
   {
	   rcBlock[looper].Width = BLOCK_WIDTH;
	   rcBlock[looper].Height = BLOCK_WIDTH;
	   switch(looper)
	   {
	   case 0:
		   rcBlock[looper].X = startX;
		   rcBlock[looper].Y = startY;
		   break;
	   case 1:
		   rcBlock[looper].X = startX+BLOCK_WIDTH+BLOCK_SPACE;
		   rcBlock[looper].Y = startY;
		   break;
	   case 2:
		   rcBlock[looper].X = startX;
		   rcBlock[looper].Y = startY+BLOCK_WIDTH+BLOCK_SPACE;
		   break;
	   case 3:
		   rcBlock[looper].X =  startX+BLOCK_WIDTH+BLOCK_SPACE;
		   rcBlock[looper].Y = startY+BLOCK_WIDTH+BLOCK_SPACE;
		   break;
	   default:
		   break;
	   }
   }
   i51KitG2CleanScreenEx(COLOR_BLACK,iNULL);
   finalIndex = Index%BLOCK_SUM;

   for (looper = 0 ; looper < BLOCK_SUM ; looper++)
   {
	   if (looper == Index)
	   {
		   continue;
	   }
	   else
	   {
	        i51KitG2CleanScreenEx(COLOR_GREEN,&rcBlock[looper]);
	   }
   }
}
iBOOL i51ScrollPublicUnPackage(char *app_name ,unzip_type uz_type)
{
	iU16 filename[66];
	iCHAR filename_ascii[132];
	static iWCHAR wszAppName[128];
	iFILE fd;
	iBOOL ret = iFALSE;
	iU16 path[64] = {0} ;
	iU32 CurrAppNum = 0;
	i51FILEINFO fileinfo = {0} ;
	iFILE file = 0 ;
	iU16 temp_name[ 256 ] ={0} ;
	int temp_cpy_start_pos = 0;
	iCHAR path_ascii[128];
	i51AdeStdAscii2Unicode(app_name,wszAppName);
	if (uz_type == uz_type_only_i51_package)
	{
		i51AdeStdWSprintf(filename, 66*2, "%s\\%s%s",app_name,app_name, ".package");
		if(i51AdeOsFileIsExist(filename))
		{
			fd = i51AdeOsFileOpen(filename, i51_OS_FL_WRITE|i51_OS_FL_EXISTING_OPEN, __FILE__ , __LINE__ );
			if(fd <= 0)
			{
				return ret;
			}
			else
			{
				i51AdeStdWSprintf(filename, 66*2, "%s", app_name);
				ret = i51KernelExtractPackage(fd, filename);   //解包
				i51AdeOsFileClose(fd);  //关闭文件
				if(ret)
				{
					i51AdeStdUnicode2Ascii(filename,filename_ascii);
					//iLog("解压成功的应用 = %s",filename_ascii);
					return ret;
				}
			}
		}
	}
	else if (uz_type == uz_type_every_package)
	{
		i51AdeStdWSprintf(filename, 66*2, "%s\\*",app_name);
		if ((file = i51AdeOsFileFind (filename , &fileinfo )) <= 0 ) {
			goto FIND_END;
		}

		do {
			memset (path , 0 , sizeof(path)) ;
			i51AdeStdWSprintf(filename, 66*2, "%s", app_name);
			i51AdeStdWStrcpy (path ,filename) ;
			i51AdeStdWStrcat (path ,L"\\") ;
			i51AdeStdWStrcat (path ,fileinfo.name) ;
			temp_cpy_start_pos = i51AdeStdWStrlen(fileinfo.name) - i51AdeStdWStrlen(W_PACKAGE_TYPE);
			if (temp_cpy_start_pos >= 0)
			{
				memcpy(temp_name,fileinfo.name+temp_cpy_start_pos,i51AdeStdWStrlen(W_PACKAGE_TYPE)*2);
			}

			if (i51AdeOsFileIsExist(path) && !i51AdeStdWStrcmp(temp_name,W_PACKAGE_TYPE) && temp_cpy_start_pos >= 0)
			{  
				i51AdeStdUnicode2Ascii(path,path_ascii);
				fd = i51AdeOsFileOpen(path, i51_OS_FL_WRITE|i51_OS_FL_EXISTING_OPEN, __FILE__ , __LINE__ );
				if(fd <= 0)
				{
					return ret;
				}
				else
				{
					i51AdeStdWSprintf(filename, 66*2, "%s", app_name);
					ret = i51KernelExtractPackage(fd, filename);   //解包
					i51AdeOsFileClose(fd);  //关闭文件
					if(ret)
					{
						i51AdeStdUnicode2Ascii(path,filename_ascii);
						//iLog("解压成功的应用 = %s",filename_ascii);
						i51AdeOsFileDelete(path);
						////
						//mr.unzip_process++;
						i51ScrollDrawUnPackaging(GobalVar.unPackIndex);
						GobalVar.unPackIndex++;
						//father_public_render_process(mr.unzip_process);
						
						i51AdeMmiUpdateScreen();

						////						return ret;
					}
				}
			}
		}while ( i51AdeOsFileFindNext(file , &fileinfo)) ;


FIND_END:	
		if ( file > 0 )
			i51AdeOsFileFindClose (file) ;
		///////////
		memset (path , 0 , sizeof(path)) ;
		i51AdeStdWSprintf(path, 66*2, "%s\\%s.package", app_name,app_name);
		if (!i51AdeOsFileIsExist(path))
		{
			 
			 return iTRUE;
		}
		else
		{
			return iFALSE;
		}
		//		return CurrAppNum;
	}

	return ret;
}
#if 0
void i51ScrollInputLoop()
{
//#define  PEN_DOWN_INDEX 0
#define  PEN_MOVE_FINAL_FIRST_INDEX 0
#define  PEN_MOVE_FINAL_LAST_INDEX 1
#define  MOVE_STAND  10
   int index,i;
   Node *pCurNode;
    iS16 curSpeed;
   int timeCount = 0;
   index = 0;
   
   while (!llp_get_input(&GobalVar.inputArr[index]) && index < MAX_INPUT_SUM -1)
   {   
	   //LOG("llp_get_input success!!");
	   if (GobalVar.inputArr[index].type == LLP_PEN_INPUT)
	   {  
		   if (GobalVar.inputArr[index].u.pen.type == LLP_PEN_DOWN)
		   {
			   memcpy(&GobalVar.penArr[PEN_MOVE_FINAL_FIRST_INDEX],&GobalVar.inputArr[index].u.pen,sizeof(LLPPhonePen));
			   memcpy(&GobalVar.penDown,&GobalVar.inputArr[index].u.pen,sizeof(LLPPhonePen));
			   LOG("GobalVar.penDown.y = %d",GobalVar.penDown.y);
			   GobalVar.bPenDownActived = iTRUE;
			   GobalVar.count = 1;
 			   GobalVar.bScrollStarted = iFALSE;
 			   //GobalVar.lv.bBarVisible = iFALSE;
			   //i51TimerRestart(TIMER_ID_LONG_PRESS);
			   if (GobalVar.lv.bAutoScrolling)
			   {
				   GobalVar.lv.bAutoScrolling = iFALSE;
				   i51TimerStop(TIMER_ID_AUTO_SCROLL);
			   }
			   i51TimerRestart(TIMER_ID_DOWN);
		   }
		   else  if (GobalVar.inputArr[index].u.pen.type == LLP_PEN_UP)
		   {
			   //GobalVar.lv.bBarVisible = iFALSE;
			   LOG("GobalVar.penUp.y = %d",GobalVar.inputArr[index].u.pen.y);
			   //LOG("LLP_PEN_UP GobalVar.count = %d",GobalVar.count);
			   if (GobalVar.count >= 1 && (GobalVar.inputArr[index].u.pen.time - GobalVar.penDown.time) < 250)
			   {
				   LOG("y1 = %d  y0 = %d  t1 = %d  t0 = %d",GobalVar.inputArr[index].u.pen.y,GobalVar.penDown.y,GobalVar.inputArr[index].u.pen.time,GobalVar.penDown.time);
				   //SLOG("yc = %d  tc = %d",(GobalVar.inputArr[index].u.pen.y-GobalVar.penDown.y),(GobalVar.inputArr[index].u.pen.time-GobalVar.penDown.time));
				   if (GobalVar.inputArr[index].u.pen.time - GobalVar.penDown.time > 0)
				   {
					   
					   //if ((GobalVar.inputArr[index].u.pen.y-GobalVar.penDown.y)*1000/(GobalVar.inputArr[index].u.pen.time - GobalVar.penDown.time) >= 50)
					   LOG("ps = %d",(MAX(GobalVar.inputArr[index].u.pen.y,GobalVar.penDown.y)-MIN(GobalVar.inputArr[index].u.pen.y,GobalVar.penDown.y))*1000/(GobalVar.inputArr[index].u.pen.time - GobalVar.penDown.time));
					   curSpeed = (MAX(GobalVar.inputArr[index].u.pen.y,GobalVar.penDown.y)-MIN(GobalVar.inputArr[index].u.pen.y,GobalVar.penDown.y))*1000/(GobalVar.inputArr[index].u.pen.time - GobalVar.penDown.time);
					   SLOG("CS = %d yOffset = %d   tCount = %d",curSpeed,(GobalVar.inputArr[index].u.pen.y-GobalVar.penDown.y),(GobalVar.inputArr[index].u.pen.time - GobalVar.penDown.time));
					   if ( curSpeed >= SCROLL_SPEED_DEP_RANGE_200)
					   {
                           LOG("Fast Move");
						   if (GobalVar.inputArr[index].u.pen.y-GobalVar.penDown.y < 0)
						   {
							   GobalVar.lv.dir = SCROLL_DIRT_VERTICAL_UP;
						   }
						   else
						   {
							   GobalVar.lv.dir = SCROLL_DIRT_VERTICAL_DOWN;
						   }
						   LOG(" GobalVar.lv.dir  111111 = %d",GobalVar.lv.dir);
						   if(curSpeed >= SCROLL_SPEED_DEP_RANGE_200 && curSpeed <= SCROLL_SPEED_DEP_RANGE_300)
						   {
							   GobalVar.lv.scrollSpeed = 600;
							   GobalVar.lv.scrollRange = 200;
						   }
						   else if(curSpeed >= SCROLL_SPEED_DEP_RANGE_300 && curSpeed <= SCROLL_SPEED_DEP_RANGE_400)
						   {
							   GobalVar.lv.scrollSpeed = 600;
							   GobalVar.lv.scrollRange = 300;
						   }
#if 1
                          else if(curSpeed >= SCROLL_SPEED_DEP_RANGE_400 && curSpeed <= SCROLL_SPEED_DEP_RANGE_600)
						  {
							  GobalVar.lv.scrollSpeed = 1000;
							  GobalVar.lv.scrollRange = 400;
						  }
						  else if (curSpeed >= SCROLL_SPEED_DEP_RANGE_600 && curSpeed <= SCROLL_SPEED_DEP_RANGE_800)
						  {
							  GobalVar.lv.scrollSpeed = 1500;
							  GobalVar.lv.scrollRange = 500;
						  }
						  else if (curSpeed >= SCROLL_SPEED_DEP_RANGE_800 && curSpeed <= SCROLL_SPEED_DEP_RANGE_1000)
						  {
							  GobalVar.lv.scrollSpeed = 1700;
							  GobalVar.lv.scrollRange = 700;
						  }
						  else if (curSpeed >= SCROLL_SPEED_DEP_RANGE_1000)
						  {
							  GobalVar.lv.scrollSpeed = 1800;
							  GobalVar.lv.scrollRange = 1000;
						  }
#endif
						   GobalVar.lv.scrollCurOffSet = 0;
						   GobalVar.lv.bAutoScrolling = iTRUE;
						   i51TimerRestart(TIMER_ID_RENDER_AUTO_SCROLL);
						   //////////////
						   i51TimerStop(TIMER_ID_DOWN);
						   memset(GobalVar.penArr,0,sizeof(GobalVar.penArr));
						   GobalVar.count = 0;
						   GobalVar.bScrollStarted = iFALSE;
						   //////////////
						   return;
					   }
				   }
			   }
              // if (i51TimerCheckActived(TIMER_ID_DOWN))
               {
			//	   timeCount = i51TimerGetTickCount(TIMER_ID_DOWN);
				   timeCount = (GobalVar.inputArr[index].u.pen.time - GobalVar.penDown.time);
			//	   SLOG("tC2 = %d",timeCount);
               }
			   //SLOG("here!!");
			   if (!GobalVar.bScrollStarted && i51ScrollCheckinZoom(GobalVar.penDown.x,GobalVar.penDown.y,GobalVar.inputArr[index].u.pen.x,GobalVar.inputArr[index].u.pen.y,MOVE_STAND) && timeCount > 60)
			   {  
				   SLOG("RUN APP COUNT=%d  =  %d   %d   %d   %d tCount = %d",GobalVar.count,GobalVar.penDown.x,GobalVar.penDown.y,GobalVar.inputArr[index].u.pen.x,GobalVar.inputArr[index].u.pen.y,(GobalVar.inputArr[index].u.pen.time - GobalVar.penDown.time));
				   pCurNode = i51WidgetScrollGetNodeInfo(&GobalVar.lv,GobalVar.inputArr[index].u.pen.x,GobalVar.inputArr[index].u.pen.y);
				   if (pCurNode)
				   {  
					   i51WidgetScrollSetItemActived(&GobalVar.lv,GobalVar.inputArr[index].u.pen.x,GobalVar.inputArr[index].u.pen.y);
					   i51WidgetScrollDrawBar(&GobalVar.lv);
					   i51AdeMmiUpdateScreen();
#if !defined WIN32
					   i51AdeOsSleep(100);
#endif
					   i51WidgetScrollSetAllIdleState(&GobalVar.lv);
					   i51WidgetScrollDrawBar(&GobalVar.lv);
					   i51AdeMmiUpdateScreen();
					   if (pCurNode->type == ITEM_TYPE_APP)
					   {
						   GobalVar.bSendSEQ = iTRUE;
						   i51ScrollLoadAppEffect();
						   i51KernelSendRequest(GobalVar.aid,pCurNode->wszCaption,i51_REQ_LOADEX,0,0);
					   }
					   else if (pCurNode->type == ITEM_TYPE_TOOL)
					   {
						   if (!i51AdeStdWStrcmp(pCurNode->wszCaption,W_SYS_TOOL))
						   {
						   }
						   else if (!i51AdeStdWStrcmp(pCurNode->wszCaption,W_UNPACK_TOOL))
						   {
							   i51ScrollSetState(STATE_UNPACKAGE);
						   }
					   }

				   }
				   else
				   {
				      SLOG("this app is not exist!!");
				   }
			   }
			   else
			   {
				   i51WidgetScrollSetAllIdleState(&GobalVar.lv);
				   i51WidgetScrollDrawBar(&GobalVar.lv);
				   i51AdeMmiUpdateScreen();
				   //i51ScrollLoadAppEffect();
			   }
			   i51TimerStop(TIMER_ID_DOWN);
			   memset(GobalVar.penArr,0,sizeof(GobalVar.penArr));
			   GobalVar.count = 0;
			   GobalVar.bScrollStarted = iFALSE;
              
			   break;
		   }
		   else if (GobalVar.inputArr[index].u.pen.type == LLP_PEN_MOVE)
		   {  
			   GobalVar.bNewInputMove = iTRUE;
			   //LOG("GobalVar.bNewInputMove = iTRUE;");
			   //if(GobalVar.bScrollStarted)
			   {   
				   if ( 1 == GobalVar.count)
				   {
					   memcpy(&GobalVar.penArr[PEN_MOVE_FINAL_LAST_INDEX],&GobalVar.inputArr[index].u.pen,sizeof(LLPPhonePen));
					   GobalVar.count++;
				   }
				   else if ( 2 == GobalVar.count)
				   {
					   if (GobalVar.bPenDownActived)
					   {
						   memcpy(&GobalVar.penArr[PEN_MOVE_FINAL_LAST_INDEX],&GobalVar.inputArr[index].u.pen,sizeof(LLPPhonePen));
					   }
					   else
					   { 
						   LOG(" 2 == GobalVar.count memcpy pen");
						   memcpy(&GobalVar.penArr[PEN_MOVE_FINAL_FIRST_INDEX],&GobalVar.penArr[PEN_MOVE_FINAL_LAST_INDEX],sizeof(LLPPhonePen));
						   memcpy(&GobalVar.penArr[PEN_MOVE_FINAL_LAST_INDEX],&GobalVar.inputArr[index].u.pen,sizeof(LLPPhonePen));
					   }
				   }
				   //LOG("move = %d",GobalVar.inputArr[index].u.pen.y);
			   }

		   }
	   }
	   //LOG("input GobalVar.count = %d",GobalVar.count);
   }

}
#endif
void i51ScrollLogicApps()
{
	
}
void i51ScrollRenderApps()
{
	iRECT rc;
	int offset;
	int curTickOut;
	int curRealOffSet;

	if (GobalVar.bNeedFlushBg)
	{
		GobalVar.bNeedFlushBg = iFALSE;
		i51KitG2CleanScreenEx(COLOR_BLACK,iNULL);
		if (GobalVar.lv.hAbs < GobalVar.lv.rcCtr.Height)
		{
			//i51FrameItemInitRect(&rc,0,GobalVar.lv.yRel,GobalVar.lv.rcCtr.Width,(GobalVar.lv.rcCtr.Height-GobalVar.lv.hAbs));
			if (GobalVar.lv.ls.pHead)
			{
				i51KitG2CleanScreenEx(GobalVar.lv.ls.pHead->icIdleCompleten,&GobalVar.lv.rcCtr);
			}

		}
		i51WidgetScrollDraw(&GobalVar.lv,0,0,&GobalVar.lv.rcCtr,SCROLL_REDRAW_LIMIT_NONE);

		i51AdeMmiUpdateScreen();
	}





	if (i51TimerCheckActived(TIMER_ID_RENDER_AUTO_SCROLL) && GobalVar.lv.bAutoScrolling)
	{  
		if (GobalVar.lv.scrollCurOffSet >= GobalVar.lv.scrollRange)
		{   
			LOG("autoScroll finished!!");
			i51TimerStop(TIMER_ID_RENDER_AUTO_SCROLL);
			GobalVar.lv.bAutoScrolling = iFALSE;
			return;
		}
		else
		{
			curTickOut = i51TimerGetTickCount(TIMER_ID_RENDER_AUTO_SCROLL);
			if (curTickOut > 0)
			{
				if (curTickOut > 0)
				{
					//curRealOffSet = curTickOut*GobalVar.lv.scrollSpeed/1000;
					curRealOffSet = 20;
					if (GobalVar.lv.scrollCurOffSet + curRealOffSet > GobalVar.lv.scrollRange)
					{
						curRealOffSet = GobalVar.lv.scrollRange - GobalVar.lv.scrollCurOffSet;
					}
					LOG("curTickOut = %d,curRealOffSet = %d",curTickOut,curRealOffSet);
					LOG(" GobalVar.lv.dir  22222 = %d",GobalVar.lv.dir);
					i51WidgetMoveScrollRender(&GobalVar.lv,GobalVar.lv.dir,curRealOffSet);
					GobalVar.lv.scrollCurOffSet += curRealOffSet;
				}
				else
				{
					LOG("curTickOut <= 0");
				}
				i51TimerRestart(TIMER_ID_RENDER_AUTO_SCROLL);
			}

		}
		return;
	}
#if 1
	if (GobalVar.count == 1 && GobalVar.bPenDownActived)
	{
		//if (i51ScrollCheckinZoom(GobalVar.penArr[PEN_DOWN_INDEX].x,GobalVar.penArr[PEN_DOWN_INDEX].y,GobalVar.penArr[PEN_MOVE_FINAL_PRE_INDEX].x,GobalVar.penArr[PEN_MOVE_FINAL_PRE_INDEX].y,MOVE_STAND))
		if(i51TimerCheckFinished(TIMER_ID_DOWN,TIME_COUNT_DOWN))
		{  
			LOG("here!!!(GobalVar.count == 1 && GobalVar.bPenDownActived)");
			i51WidgetScrollSetItemActived(&GobalVar.lv,GobalVar.penArr[PEN_MOVE_FINAL_FIRST_INDEX].x,GobalVar.penArr[PEN_MOVE_FINAL_FIRST_INDEX].y);
			i51WidgetScrollDrawBar(&GobalVar.lv);
			i51AdeMmiUpdateScreen();
			i51TimerStop(TIMER_ID_DOWN);
		}
	}
	else if (GobalVar.count == 2 && GobalVar.bNewInputMove)
	{   
		//GobalVar.bNewInputMove = iFALSE;
		//LOG("DDDDD f_y = %d  l_y = %d ",GobalVar.penArr[PEN_MOVE_FINAL_FIRST_INDEX].y,GobalVar.penArr[PEN_MOVE_FINAL_LAST_INDEX].y);
		if (i51ScrollCheckinZoom(GobalVar.penArr[PEN_MOVE_FINAL_FIRST_INDEX].x,GobalVar.penArr[PEN_MOVE_FINAL_FIRST_INDEX].y,GobalVar.penArr[PEN_MOVE_FINAL_LAST_INDEX].x,GobalVar.penArr[PEN_MOVE_FINAL_LAST_INDEX].y,MOVE_STAND))
		{  
			//if (i51TimerCheckFinished(TIMER_ID_LONG_PRESS,TIME_COUNT_LONG_PRESS))
			if(GobalVar.bPenDownActived && i51TimerCheckFinished(TIMER_ID_DOWN,TIME_COUNT_DOWN))
			{
				//LOG("long press!!!!!!!!!!!!!!!!!!!!!!!!!!1");
				//i51WidgetScrollDraw()
				LOG("in limited area = %d  x0=%d y0=%d  x1=%d y1=%d ",GobalVar.count,GobalVar.penArr[PEN_MOVE_FINAL_FIRST_INDEX].x,GobalVar.penArr[PEN_MOVE_FINAL_FIRST_INDEX].y,GobalVar.penArr[PEN_MOVE_FINAL_LAST_INDEX].x,GobalVar.penArr[PEN_MOVE_FINAL_LAST_INDEX].y);
				i51WidgetScrollSetItemActived(&GobalVar.lv,GobalVar.penArr[PEN_MOVE_FINAL_LAST_INDEX].x,GobalVar.penArr[PEN_MOVE_FINAL_LAST_INDEX].y);
				i51WidgetScrollDrawBar(&GobalVar.lv);
				i51AdeMmiUpdateScreen();
				i51TimerStop(TIMER_ID_DOWN);
			}
			return;
		}
		else
		{
			i51TimerStop(TIMER_ID_DOWN);
			GobalVar.bScrollStarted = iTRUE;
			//GobalVar.lv.bBarVisible = iTRUE;
			i51WidgetScrollSetAllIdleState(&GobalVar.lv);
			offset = (GobalVar.penArr[PEN_MOVE_FINAL_LAST_INDEX].y-GobalVar.penArr[PEN_MOVE_FINAL_FIRST_INDEX].y);
			if (offset < 0)
			{
				i51WidgetMoveScrollRender(&GobalVar.lv,SCROLL_DIRT_VERTICAL_UP,-offset);
			}
			if (offset >0)
			{
				i51WidgetMoveScrollRender(&GobalVar.lv,SCROLL_DIRT_VERTICAL_DOWN,(offset));
			}
			LOG("move  = %d   = %d",GobalVar.penArr[PEN_MOVE_FINAL_LAST_INDEX].y,GobalVar.penArr[PEN_MOVE_FINAL_FIRST_INDEX].y);
		}

		GobalVar.count = 1;
		memcpy(&GobalVar.penArr[PEN_MOVE_FINAL_FIRST_INDEX],&GobalVar.penArr[PEN_MOVE_FINAL_LAST_INDEX],sizeof(LLPPhonePen));
	}
#endif	
}
void i51ScrollLogicLoop()
{  
    switch(GobalVar.state)
	{
	case STATE_APP:
		  break;
	case STATE_UNPACKAGE:
		  break;
	case STATE_UNPACAGING:
		break;
	default:
		break;
	}
}
void i51ScrollPublicDrawImageButton(ImageButton *pBtn)
{
	if (pBtn)
	{
		if (pBtn->bNeedFlushed)
		{
			pBtn->bNeedFlushed = iFALSE;
			pBtn->preState = pBtn->curState;
			if (ITEM_STATE_IDLE == pBtn->curState)
			{
				i51KitG2CleanScreenEx(pBtn->bgIcidle,&pBtn->imgRc);
				i51PublicDrawSysWStr(pBtn->wszCaption,&pBtn->imgRc,COLOR_WHITE,DRAW_STYLE_H_MID,DRAW_STYLE_V_MID);
			}
			else if (ITEM_STATE_DOWN == pBtn->curState)
			{
				i51KitG2CleanScreenEx(pBtn->bgIcDown,&pBtn->imgRc);
				i51PublicDrawSysWStr(pBtn->wszCaption,&pBtn->imgRc,COLOR_WHITE,DRAW_STYLE_H_MID,DRAW_STYLE_V_MID);
			}
		}
	}
}
void i51ScrollRenderUnPackage()
{
   int looper;
   if (GobalVar.bNeedFlushBg)
   {
	   GobalVar.bNeedFlushBg = iFALSE;
	   i51KitG2CleanScreenEx(COLOR_WHITE,iNULL);
   }
   for(looper = BTN_ID_UNPACKAGE_UNPACK; looper <= BTN_ID_UNPACKAGE_BACK; looper++)
   {
	   i51ScrollPublicDrawImageButton(&GobalVar.btn[looper]);
#if 0
	   if (GobalVar.btn[looper].bNeedFlushed)
	   {
		   GobalVar.btn[looper].bNeedFlushed = iFALSE;
		   GobalVar.btn[looper].preState = GobalVar.btn[looper].curState;
		   if (ITEM_STATE_IDLE == GobalVar.btn[looper].curState)
		   {
			   i51KitG2CleanScreenEx(GobalVar.btn[looper].bgIcidle,&GobalVar.btn[looper].imgRc);
			   i51PublicDrawSysWStr(GobalVar.btn[looper].wszCaption,&GobalVar.btn[looper].imgRc,COLOR_BLACK,DRAW_STYLE_H_MID,DRAW_STYLE_V_MID);
		   }
		   else if (ITEM_STATE_DOWN == GobalVar.btn[looper].curState)
		   {
			   i51KitG2CleanScreenEx(GobalVar.btn[looper].bgIcDown,&GobalVar.btn[looper].imgRc);
			   i51PublicDrawSysWStr(GobalVar.btn[looper].wszCaption,&GobalVar.btn[looper].imgRc,COLOR_BLACK,DRAW_STYLE_H_MID,DRAW_STYLE_V_MID);
		   }
	   }
#endif

   }
   i51AdeMmiUpdateScreen();
}
void i51ScrollRenderUnPackaging()
{

}

void i51ScrollRenderLoop()
{  
	switch(GobalVar.state)
	{
	case STATE_APP:
		i51ScrollRenderApps();
		break;
	case STATE_UNPACKAGE:
		i51ScrollRenderUnPackage();
		break;
	case STATE_UNPACAGING:
		i51ScrollRenderUnPackaging();
		break;
	default:
		break;
	}
}
void i51ScrollInputApps()

{
	//#define  PEN_DOWN_INDEX 0

	int index,i;
	Node *pCurNode;
	iS16 curSpeed;
	int timeCount = 0;
	index = 0;

	while (!llp_get_input(&GobalVar.inputArr[index]) && index < MAX_INPUT_SUM -1)
	{   
		//LOG("llp_get_input success!!");
		if (GobalVar.inputArr[index].type == LLP_PEN_INPUT)
		{  
			if (GobalVar.inputArr[index].u.pen.type == LLP_PEN_DOWN)
			{
				memcpy(&GobalVar.penArr[PEN_MOVE_FINAL_FIRST_INDEX],&GobalVar.inputArr[index].u.pen,sizeof(LLPPhonePen));
				memcpy(&GobalVar.penDown,&GobalVar.inputArr[index].u.pen,sizeof(LLPPhonePen));
				LOG("GobalVar.penDown.y = %d",GobalVar.penDown.y);
				GobalVar.bPenDownActived = iTRUE;
				GobalVar.count = 1;
				GobalVar.bScrollStarted = iFALSE;
				//GobalVar.lv.bBarVisible = iFALSE;
				//i51TimerRestart(TIMER_ID_LONG_PRESS);
				if (GobalVar.lv.bAutoScrolling)
				{
					GobalVar.lv.bAutoScrolling = iFALSE;
					i51TimerStop(TIMER_ID_AUTO_SCROLL);
				}
				i51TimerRestart(TIMER_ID_DOWN);
			}
			else  if (GobalVar.inputArr[index].u.pen.type == LLP_PEN_UP)
			{
				//GobalVar.lv.bBarVisible = iFALSE;
				LOG("GobalVar.penUp.y = %d",GobalVar.inputArr[index].u.pen.y);
				//LOG("LLP_PEN_UP GobalVar.count = %d",GobalVar.count);
				if (GobalVar.count >= 1 && (GobalVar.inputArr[index].u.pen.time - GobalVar.penDown.time) < 250)
				{
					LOG("y1 = %d  y0 = %d  t1 = %d  t0 = %d",GobalVar.inputArr[index].u.pen.y,GobalVar.penDown.y,GobalVar.inputArr[index].u.pen.time,GobalVar.penDown.time);
					//SLOG("yc = %d  tc = %d",(GobalVar.inputArr[index].u.pen.y-GobalVar.penDown.y),(GobalVar.inputArr[index].u.pen.time-GobalVar.penDown.time));
					if (GobalVar.inputArr[index].u.pen.time - GobalVar.penDown.time > 0)
					{

						//if ((GobalVar.inputArr[index].u.pen.y-GobalVar.penDown.y)*1000/(GobalVar.inputArr[index].u.pen.time - GobalVar.penDown.time) >= 50)
						LOG("ps = %d",(MAX(GobalVar.inputArr[index].u.pen.y,GobalVar.penDown.y)-MIN(GobalVar.inputArr[index].u.pen.y,GobalVar.penDown.y))*1000/(GobalVar.inputArr[index].u.pen.time - GobalVar.penDown.time));
						curSpeed = (MAX(GobalVar.inputArr[index].u.pen.y,GobalVar.penDown.y)-MIN(GobalVar.inputArr[index].u.pen.y,GobalVar.penDown.y))*1000/(GobalVar.inputArr[index].u.pen.time - GobalVar.penDown.time);
						SLOG("CS = %d yOffset = %d   tCount = %d",curSpeed,(GobalVar.inputArr[index].u.pen.y-GobalVar.penDown.y),(GobalVar.inputArr[index].u.pen.time - GobalVar.penDown.time));
						if ( curSpeed >= SCROLL_SPEED_DEP_RANGE_200)
						{
							LOG("Fast Move");
							if (GobalVar.inputArr[index].u.pen.y-GobalVar.penDown.y < 0)
							{
								GobalVar.lv.dir = SCROLL_DIRT_VERTICAL_UP;
							}
							else
							{
								GobalVar.lv.dir = SCROLL_DIRT_VERTICAL_DOWN;
							}
							LOG(" GobalVar.lv.dir  111111 = %d",GobalVar.lv.dir);
							if(curSpeed >= SCROLL_SPEED_DEP_RANGE_200 && curSpeed <= SCROLL_SPEED_DEP_RANGE_300)
							{
								GobalVar.lv.scrollSpeed = 600;
								GobalVar.lv.scrollRange = 200;
							}
							else if(curSpeed >= SCROLL_SPEED_DEP_RANGE_300 && curSpeed <= SCROLL_SPEED_DEP_RANGE_400)
							{
								GobalVar.lv.scrollSpeed = 600;
								GobalVar.lv.scrollRange = 300;
							}
#if 1
							else if(curSpeed >= SCROLL_SPEED_DEP_RANGE_400 && curSpeed <= SCROLL_SPEED_DEP_RANGE_600)
							{
								GobalVar.lv.scrollSpeed = 1000;
								GobalVar.lv.scrollRange = 400;
							}
							else if (curSpeed >= SCROLL_SPEED_DEP_RANGE_600 && curSpeed <= SCROLL_SPEED_DEP_RANGE_800)
							{
								GobalVar.lv.scrollSpeed = 1500;
								GobalVar.lv.scrollRange = 500;
							}
							else if (curSpeed >= SCROLL_SPEED_DEP_RANGE_800 && curSpeed <= SCROLL_SPEED_DEP_RANGE_1000)
							{
								GobalVar.lv.scrollSpeed = 1700;
								GobalVar.lv.scrollRange = 700;
							}
							else if (curSpeed >= SCROLL_SPEED_DEP_RANGE_1000)
							{
								GobalVar.lv.scrollSpeed = 1800;
								GobalVar.lv.scrollRange = 1000;
							}
#endif
							GobalVar.lv.scrollCurOffSet = 0;
							GobalVar.lv.bAutoScrolling = iTRUE;
							i51TimerRestart(TIMER_ID_RENDER_AUTO_SCROLL);
							//////////////
							i51TimerStop(TIMER_ID_DOWN);
							memset(GobalVar.penArr,0,sizeof(GobalVar.penArr));
							GobalVar.count = 0;
							GobalVar.bScrollStarted = iFALSE;
							//////////////
							return;
						}
					}
				}
				// if (i51TimerCheckActived(TIMER_ID_DOWN))
				{
					//	   timeCount = i51TimerGetTickCount(TIMER_ID_DOWN);
					timeCount = (GobalVar.inputArr[index].u.pen.time - GobalVar.penDown.time);
					//	   SLOG("tC2 = %d",timeCount);
				}
				//SLOG("here!!");
				if (!GobalVar.bScrollStarted && i51ScrollCheckinZoom(GobalVar.penDown.x,GobalVar.penDown.y,GobalVar.inputArr[index].u.pen.x,GobalVar.inputArr[index].u.pen.y,MOVE_STAND) && timeCount > 60)
				{  
					SLOG("RUN APP COUNT=%d  =  %d   %d   %d   %d tCount = %d",GobalVar.count,GobalVar.penDown.x,GobalVar.penDown.y,GobalVar.inputArr[index].u.pen.x,GobalVar.inputArr[index].u.pen.y,(GobalVar.inputArr[index].u.pen.time - GobalVar.penDown.time));
					pCurNode = i51WidgetScrollGetNodeInfo(&GobalVar.lv,GobalVar.inputArr[index].u.pen.x,GobalVar.inputArr[index].u.pen.y);
					if (pCurNode)
					{  
						i51WidgetScrollSetItemActived(&GobalVar.lv,GobalVar.inputArr[index].u.pen.x,GobalVar.inputArr[index].u.pen.y);
						i51WidgetScrollDrawBar(&GobalVar.lv);
						i51AdeMmiUpdateScreen();
#if !defined WIN32
						i51AdeOsSleep(100);
#endif
						i51WidgetScrollSetAllIdleState(&GobalVar.lv);
						i51WidgetScrollDrawBar(&GobalVar.lv);
						i51AdeMmiUpdateScreen();
						if (pCurNode->type == ITEM_TYPE_APP)
						{
							GobalVar.bSendSEQ = iTRUE;
							i51ScrollLoadAppEffect();
							i51KernelSendRequest(GobalVar.aid,pCurNode->wszCaption,i51_REQ_LOADEX,0,0);
						}
						else if (pCurNode->type == ITEM_TYPE_TOOL)
						{
							if (!i51AdeStdWStrcmp(pCurNode->wszCaption,W_SYS_TOOL))
							{
							}
							else if (!i51AdeStdWStrcmp(pCurNode->wszCaption,W_UNPACK_TOOL))
							{
								i51ScrollSetState(STATE_UNPACKAGE);
							}
						}

					}
					else
					{
						SLOG("this app is not exist!!");
					}
				}
				else
				{
					i51WidgetScrollSetAllIdleState(&GobalVar.lv);
					i51WidgetScrollDrawBar(&GobalVar.lv);
					i51AdeMmiUpdateScreen();
					//i51ScrollLoadAppEffect();
				}
				i51TimerStop(TIMER_ID_DOWN);
				memset(GobalVar.penArr,0,sizeof(GobalVar.penArr));
				GobalVar.count = 0;
				GobalVar.bScrollStarted = iFALSE;

				break;
			}
			else if (GobalVar.inputArr[index].u.pen.type == LLP_PEN_MOVE)
			{  
				GobalVar.bNewInputMove = iTRUE;
				//LOG("GobalVar.bNewInputMove = iTRUE;");
				//if(GobalVar.bScrollStarted)
				{   
					if ( 1 == GobalVar.count)
					{
						memcpy(&GobalVar.penArr[PEN_MOVE_FINAL_LAST_INDEX],&GobalVar.inputArr[index].u.pen,sizeof(LLPPhonePen));
						GobalVar.count++;
					}
					else if ( 2 == GobalVar.count)
					{
						if (GobalVar.bPenDownActived)
						{
							memcpy(&GobalVar.penArr[PEN_MOVE_FINAL_LAST_INDEX],&GobalVar.inputArr[index].u.pen,sizeof(LLPPhonePen));
						}
						else
						{ 
							LOG(" 2 == GobalVar.count memcpy pen");
							memcpy(&GobalVar.penArr[PEN_MOVE_FINAL_FIRST_INDEX],&GobalVar.penArr[PEN_MOVE_FINAL_LAST_INDEX],sizeof(LLPPhonePen));
							memcpy(&GobalVar.penArr[PEN_MOVE_FINAL_LAST_INDEX],&GobalVar.inputArr[index].u.pen,sizeof(LLPPhonePen));
						}
					}
					//LOG("move = %d",GobalVar.inputArr[index].u.pen.y);
				}

			}
		}
		//LOG("input GobalVar.count = %d",GobalVar.count);
	}

}
iBOOL i51ScrollPublicPenInRect(LLPPhonePen *pPen , iRECT *pRect)
{
   iBOOL ret = iFALSE;
   if (!pPen || !pRect)
   {
	   return ret;
   }
   else
   {
	   return (pPen->x >= pRect->X) && (pPen->x <= pRect->X + pRect->Width) && (pPen->y >= pRect->Y) && (pPen->y <= pRect->Y + pRect->Height);
   }
}
void i51ScrollInputUnPackage()
{
    LLPInputStruct input;
	int looper;
	Node *pCurNode,addNode;
	static iCHAR strTemp[256]; 
	if (!llp_get_input(&input))
	{
		if (input.type == LLP_PEN_INPUT)
		{   
			for(looper = BTN_ID_UNPACKAGE_UNPACK; looper <= BTN_ID_UNPACKAGE_BACK; looper++)
			{
				if (i51ScrollPublicPenInRect(&input.u.pen,&GobalVar.btn[looper].touchRc))
				{
					if (input.u.pen.type == LLP_PEN_MOVE || input.u.pen.type == LLP_PEN_DOWN)
					{
						if (GobalVar.btn[looper].curState != ITEM_STATE_DOWN)
						{
							GobalVar.btn[looper].curState = ITEM_STATE_DOWN;
							GobalVar.btn[looper].bNeedFlushed = iTRUE;
						}
					}
					else if (input.u.pen.type == LLP_PEN_UP)
					{
						if (GobalVar.btn[looper].curState != ITEM_STATE_IDLE)
						{
							GobalVar.btn[looper].curState = ITEM_STATE_IDLE;
							GobalVar.btn[looper].bNeedFlushed = iTRUE;
                            i51ScrollPublicDrawImageButton(&GobalVar.btn[looper]);
							i51AdeMmiUpdateScreen();
							if (GobalVar.btn[looper].id == BTN_ID_UNPACKAGE_BACK)
							{
								i51ScrollSetState(STATE_APP);
							}
							else if (GobalVar.btn[looper].id == BTN_ID_UNPACKAGE_UNPACK)
							{
								if (GobalVar.packList.sum > 0)
								{         
									GobalVar.unPackIndex = 0;
									pCurNode = GobalVar.packList.pHead;
									while (pCurNode)
									{   
										i51AdeStdUnicode2Ascii(pCurNode->wszCaption,strTemp);
										if (i51ScrollPublicUnPackage(strTemp,uz_type_every_package))
										{
											i51AdeStdWStrcpy(addNode.wszCaption,pCurNode->wszCaption);
											addNode.type = ITEM_TYPE_APP;
											i51ListInsertNode(&GobalVar.lv.ls,&addNode);
											i51ListDelNodeByCaption(&GobalVar.packList,pCurNode->wszCaption);
										}

										if (pCurNode->pNext)
										{
											pCurNode = pCurNode->pNext;
										}
										else
										{
											break;
										}
									}
									i51ScrollInitSetPos();
									i51ScrollSetState(STATE_UNPACKAGE);
								}
							}
						}
					}
				}
				else
				{
					if (input.u.pen.type == LLP_PEN_MOVE)
					{
						if (GobalVar.btn[looper].curState != ITEM_STATE_IDLE)
						{
							GobalVar.btn[looper].curState = ITEM_STATE_IDLE;
							GobalVar.btn[looper].bNeedFlushed = iTRUE;
						}
					}
				}
			}

		}
	}
}
void i51ScrollInputUnPacking()
{

}
void i51ScrollInputLoop()
{  
	switch(GobalVar.state)
	{
	case STATE_APP:
		i51ScrollInputApps();
		break;
	case STATE_UNPACKAGE:
		i51ScrollInputUnPackage();
		break;
	case STATE_UNPACAGING:
		i51ScrollInputUnPacking();
		break;
	default:
		break;
	}
}
iKIT iBOOL i51KitMain ( iS32 aid , iU32 message , void* parameter )
{
	switch ( message )
	{
		case i51_MSG_PAINT :
			{
				static iBOOL flag = 1;
				if (flag)
				{
					flag = 0;
					i51ScrollInit();
					GobalVar.aid = aid;
				}
				if (GobalVar.bSendSEQ)
				{
					break;
				}
				i51ScrollInputLoop();
				i51ScrollRenderLoop();
                i51ScrollLogicLoop();
				//i51AdeMmiUpdateScreen();
			}
			
		break ;
	
		case i51_MSG_RUN :
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
			i51ScrollExit();
		break ;
		case i51_MSG_KEYDOWN:
		case i51_MSG_KEYUP:
			if (GobalVar.bSendSEQ)
			{
				break;
			}
			llp_input_key(message, (int)parameter);
			break;
		case i51_MSG_PADDOWN:
		case i51_MSG_PADUP:
		case i51_MSG_PADMOVE:
			if (GobalVar.bSendSEQ)
			{
				break;
			}
			//LOG("GET MSG i51_MSG_PAD = %d",message);
			llp_input_pen(message, (int)parameter);
			break;
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
