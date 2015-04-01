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
	14908 + FUNBOX_OFFSET,		//1 mmSABBgMoonCanva.gif
	30290 + FUNBOX_OFFSET,		//2 mmSABBgStoreyCanva.gif
	69044 + FUNBOX_OFFSET,		//3 mmSABLevelCanva.gif
	131680 + FUNBOX_OFFSET,		//4 mmSABLogoCanva.gif
	156898 + FUNBOX_OFFSET,		//5 mmSABMainmenuCanva.gif
	188016 + FUNBOX_OFFSET,		//6 mmSABMainmenuLogoCanva.gif
	207439 + FUNBOX_OFFSET,		//7 mmSABMainmenuResCanva.gif
	218568 + FUNBOX_OFFSET,		//8 mmSABPauseCanva.gif
	238251 + FUNBOX_OFFSET,		//9 mmSABSceneCanva.gif
	257578 + FUNBOX_OFFSET,		//10 mmSABUIResCanva.gif
	263476 + FUNBOX_OFFSET,		//11 mmSABUIResultCanva.GIF
	
};

#endif
