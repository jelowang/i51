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
	50266 + FUNBOX_OFFSET,		//1 mmSABBgMoonCanva.gif
	86428 + FUNBOX_OFFSET,		//2 mmSABBgStoreyCanva.gif
	149116 + FUNBOX_OFFSET,		//3 mmSABLevelCanva.gif
	243038 + FUNBOX_OFFSET,		//4 mmSABLogoCanva.gif
	291117 + FUNBOX_OFFSET,		//5 mmSABMainmenuCanva.gif
	351249 + FUNBOX_OFFSET,		//6 mmSABMainmenuLogoCanva.gif
	375799 + FUNBOX_OFFSET,		//7 mmSABMainmenuResCanva.gif
	391206 + FUNBOX_OFFSET,		//8 mmSABPauseCanva.gif
	410889 + FUNBOX_OFFSET,		//9 mmSABSceneCanva.gif
	433456 + FUNBOX_OFFSET,		//10 mmSABUIResCanva.gif
	444936 + FUNBOX_OFFSET,		//11 mmSABUIResultCanva.gif
	
};

#endif
