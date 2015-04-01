const mmRectangle mm_sab_raw_image[] = {
	//背景
	//{0, 291, 191, 28,},
	
	//支架
	{1,60, 62,21,},
	{1,82, 83,18,},

	//红鸟
	{1,1,	22,22,},
	{24,1,	22,22,},
	{47,1,	22,22,},
	{120,1,	20,25,},
	//小小鸟
	{141,1,	16,16,},
	{158,1,	16,15,},
	//黄鸟
	{177,2,	28,30,},
	{177,33,27,30,},
	//黑鸟
	{96,29,	39,29,},
	{136,30,39,29,},

	//小猪
	{94,1, 23,24,},
	{70,1, 23,24,},

	//大猪
	{91,60, 38,38,},
	{132,60, 38,38,},
	
	{93,249,40,38},//猴子
	
	//鸟和猪爆炸的效果
	{1,27, 28,29,},
	{30,27, 31,32,},
	{63,27, 31,32,},
	//木块特效
	{1,190,49,23,},
	{1,217,75,40,},
	{78,187, 77,47,},
	//冰块特效
	{92,192, 49,23,},
	{130,184, 75,40,},
    {172,184, 77,47,},

	//大木块
	{12, 101, 10, 84,},
	{23, 101, 10, 84,},
	//中木块
	{56,101, 10, 42,},
	{67,101, 10, 42,},
	//小木块
	{78,101,10, 20,},
	{78,122,10, 20,},
	
	//大石块
	{1, 101, 10, 84,},
	{1, 101, 10, 84,},
	//中石块
	{78,143, 10,42,},
	{89,143, 10,42,},
	//小石块
	{89,101,10,20},
	{89,122,10,20},
	//毛石块
	{75, 71,	10, 10,},
	{75, 71,	10, 10,},
	
	//大冰块
	{34, 101,	10, 84,},
	{45, 101,	10, 84,},
	//中冰块
	{56, 144,	10, 41,},
	{67, 144,	10, 41,},
	//小冰块
	{100,101,	10, 20,},
	{132,101,	10, 20,},
	
	//粗木块
	{111,101,	20, 41,},
	{132, 101,	20, 41,},
	//粗石块
	{121,143,	20, 41,},
	{141, 143,	20, 41,},
	//三角石块
	{153, 103,	39, 39},
	{162, 144,	39, 39},
	//粗冰块
	{100,143,20,20},
	{100,164,20,20},
	
	// 爆炸特效
	{1,278,		43,41},
	
	{212,26,20,101},//地板
	{65,258,21,23},//灯笼,自动往上找东西来绑
	{65,258,21,23},//灯笼1，不会自动往上找东西来绑
	//{100,252,37,32},//猴子
	{143,254,28,28},//月饼
	
	{66,288,23,30},//帽子
	
	{226,135,10,10},//微小的静态障碍物
	{226,135,10,20},//小的静态障碍物
	{226,135,10,40},//中等静态障碍物
	{226,135,10,80},//大的静态障碍物

};

const mmRectangle mm_sab_ui_image[] = {
	//通过 失败 关卡
	// 0
	{1,2, 	25, 48},
	{1,52,	25, 49},
	{1,103,	24, 50},
	
	//数字0 - 4
	// 3
	{27, 1,		21,19},
	{27, 20,	21,19},
	{27, 39,	21,19},
	{27, 58,	21,19},
	{27, 77,	21,19},
	//数字5 - 9
	{48, 1,		21,19},
	{48, 20,	21,19},
	{48, 39,	21,19},
	{48, 58,	21,19},
	{48, 77,	21,19},
	//按钮
	{280,2,		53,54},//刷新
	{160,59,	53,54},//下一关
	{220,2,	53,53},//关卡选择
	{220,64,	53,53},//主菜单
	
	//声音（5个）
	// 17
	{340,1,	54,55,},//声音
	{54,130,	27,26,},//问号---不要
	{354,115,	50,48,},//三角？？
	{164,1,52,53},//暂停
	{352,60,	53,53,},//静音
	
	//小星星
	// 22
	{178,120,		35,36,},
	{52,96,		26,26,},//不要
	
	{301, 81, 20, 82},//当前得分
	{173, 1, 13, 36},//IMG_ANGRYBIRD_UI_WORD_STAGE,
	{324, 81, 21, 82},//最高得分
	
	{244, 64,	16, 16},//IMG_ANGRYBIRD_UI_SCORENUMBER_0,
	{244, 81,	16, 16},
	{244, 99,	16, 16},
	{244, 118,16, 16},
	{244, 139,	16, 16},
	{278, 65,	16, 16},
	{278, 83,	16, 16},
	{278, 101,	16, 16},
	{278, 121,	16, 16},
	{278, 143,	16, 16},//IMG_ANGRYBIRD_UI_SCORENUMBER_9 = IMG_ANGRYBIRD_UI_SCORENUMBER_0 + 9,
	
	{61, 111, 26, 17},//IMG_ANGRYBIRD_UI_SCORENUMBER_RED_0,
	{61, 101, 27, 10},//IMG_ANGRYBIRD_UI_SCORENUMBER_RED_1,
	{30, 111, 26, 17},//IMG_ANGRYBIRD_UI_SCORENUMBER_BLUE_0,
	{30, 101, 28, 10},//IMG_ANGRYBIRD_UI_SCORENUMBER_BLUE_1,
	{61, 141, 26, 17},//IMG_ANGRYBIRD_UI_SCORENUMBER_YELLOW_0,
	{61, 131, 28, 10},//IMG_ANGRYBIRD_UI_SCORENUMBER_YELLOW_1,
	{29, 141, 26, 17},//IMG_ANGRYBIRD_UI_SCORENUMBER_BLACK_0,
	{29, 131, 27, 10},//IMG_ANGRYBIRD_UI_SCORENUMBER_BLACK_1,
	{95, 141, 26, 17},//IMG_ANGRYBIRD_UI_SCORENUMBER_PIG_0,
	{95, 125, 26, 16},//IMG_ANGRYBIRD_UI_SCORENUMBER_PIG_5,
	
	
	{132,132,23,20},//IMG_ANGRYBIRD_UI_SCORENUMBER_LANTERN_0,
	{132,111,23,19},//IMG_ANGRYBIRD_UI_SCORENUMBER_LANTERN_3,
	
	
	{6,155,		6,6},// path point
	{70, 1,		87, 87},//new record
	
	//{0, 162, 240, 3},//画地板的线
	{0,161,240,5},
};

const mmRectangle mmSBAUIHotAreas[] = {//2011.5.31
	/*{0, 0, 320, 80},	// hint area
	{40,25,50,50},// sound button
	{90,20,50,50},// main menu button
	{150,20,50,50},// stage selector btn
	{210,20,50,50},// again btn				
	{150, 70, 0, 0},				// continue*/
	{0, 0, 320, 176},	// hint area
	{16,40,59,61},// sound button
	{91,42,62,64},// main menu button
	{165,42,62,64},// stage selector btn
	{239,42,61,64},// again btn				
	{139, 128, 43, 45},				// continue
	
};

const mmRectangle mmSABResultAreas[]  = {
		
		/*{40,55,248,381},

		{250,185},//关卡
		
		{170,100},//最高纪录
		{130,100},//当前记录
		{80,160},//画星星
		
		{120,300},//新纪录
		
		{250,235},//通过或失败
		
		{30,100,40,39}, //result selector btn
		{30,225,40,39}, //result again btn
		{30,350,40,39}, //result next level btn*/
		
		{40,55,248,381},

		{250,185},//关卡
		
		{170,100},//最高纪录
		{130,100},//当前记录
		{80,180},//画星星
		
		{120,300},//新纪录
		
		{250,235},//通过或失败
		
		{21,100,40,39}, //result selector btn
		{21,225,40,39}, //result again btn
		{21,350,40,39}, //result next level btn
		
		
	};