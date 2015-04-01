#ifndef CREATE_CONTAINTER_H
#define CREATE_CONTAINTER_H

#include "i51KitAde.h"
#include "i51KitStd.h"
#include "i51KitG2.h"

#include "i51Public.h"

//#define  APP_DIR              "cf"   //应用目录
//#define  RESOURCE_NUM         3      //资源的个数
//#define  RESOURCE_NAMES       {"cf", "a", "b"}  //资源名字
//#define  CONTAINER_NAMES      {"game.container", "a.a", "b.b"}  //容器名字
//#define  RESOURCE_IMAGE_NUMS  {47, 47, 47}   //每个资源文件的图片数量

typedef enum
{
	DECODE_DOING = 0,
	DECODE_FALSE,
	DECODE_FINISH
};

typedef struct  
{
	iU8 state;
	iU16 percent;  //解码百分比
	iU8 order;     //当前解第几个资源
	iU8 resource_num;
	iHANDLE container_handle;
	iFILE res_fd;
	iU8 num[GAME_RESOURCE_NUM];
	iU8 finish[GAME_RESOURCE_NUM];
	iBOOL doing[GAME_RESOURCE_NUM];
}decode_struct;

extern void create_image_container(char *dir, char *resource_name, char *container_name, iU32 image_num);
extern iBOOL check_image_container(char (*res_name)[33], char (*ctn_name)[33], iU32 image_num[], iU32 resource_num);
extern void create_image_container_init(void);
extern void container_exit(void);
extern decode_struct decode;
extern i51Public_struct *pfunc;

#endif