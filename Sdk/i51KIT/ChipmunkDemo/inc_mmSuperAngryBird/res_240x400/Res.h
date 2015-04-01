#ifndef __RES_INC__
#define __RES_INC__
/****************************************
相关说明: 
图片前有8个字节来存储相关信息 + FUNBOX_OFFSET,和以前一样
声音文件前面有1个字节用来表示声音文件类型
*****************************************/
#define FUNBOX_OFFSET 8
unsigned int mmResList[] = { 
	0 + FUNBOX_OFFSET,
	18504 + FUNBOX_OFFSET,		//1 mmSABBgMoonCanva.gif
	33886 + FUNBOX_OFFSET,		//2 mmSABBgStoreyCanva.gif
	87136 + FUNBOX_OFFSET,		//3 mmSABLevelCanva.gif
	153010 + FUNBOX_OFFSET,		//4 mmSABLogoCanva.gif
	179023 + FUNBOX_OFFSET,		//5 mmSABMainmenuCanva.gif
	217417 + FUNBOX_OFFSET,		//6 mmSABMainmenuLogoCanva.gif
	236840 + FUNBOX_OFFSET,		//7 mmSABMainmenuResCanva.gif
	247969 + FUNBOX_OFFSET,		//8 mmSABPauseCanva.gif
	267652 + FUNBOX_OFFSET,		//9 mmSABSceneCanva.gif
	286979 + FUNBOX_OFFSET,		//10 mmSABUIResCanva.gif
	292877 + FUNBOX_OFFSET,		//11 mmSABUIResultCanva.GIF
	
};

#endif
