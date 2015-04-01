
//#include "math.h"
#include "string.h"
#include "i51KitAde.h"
#include "i51KitG2.h"
#include "i51KitStd.h"

#include "i51KitG2Font.h"

i51KIT_DYNAMIC_KIT_DEF(i51KitStd);

// 测试开关
#define DEBUG_I51KITG2 0
#define LCDBUFF_DEBUG  0
#define i51KITG2_DEBUG  0
#define ROTATE_SPECIAE 1

#if DEBUG_I51KITG2
#include "Image_Hex.h"
#endif

// 求X的绝对值
#define I51KITG2_ABS(X) (((X)>0)?(X):(-X))

// 图片容器版本号
#define I51KITG2_IMAGE_CONTAINER_VERSION 0X02

// 图片容器版本号在容器文件内的偏移量
#define I51KITG2_OFFSET_CONTAINER_VERSION 0X08
// 图片容器文件版本号在容器文件内的偏移量
#define I51KITG2_OFFSET_FILE_VERSION 0X09
// 图片容器中第一张图片数据是否存在相对图片容器头的偏移量
#define I51KITG2_OFFSET_START_DATA  0X11
// 图片容器中图片数据长度相对于入口的偏移量
#define I51KITG2_OFFSET_DATALENGTH  0X05
// 图片容器中图片宽度相对于入口的偏移量
#define I51KITG2_OFFSET_IMAGEWIDTH  0X09
// 图片容器中图片高度相对于入口的偏移量
#define I51KITG2_OFFSET_IMAGEHEIGHT 0X0B
// 图片容器中图片透明色相对于入口的偏移量
#define I51KITG2_OFFSET_COLORALPHA  0X0D
// 图片容器中图片编码方式相对于入口的偏移量
#define I51KITG2_OFFSET_DECODETYPE  0X11
// 图片容器中图片是否有带Alpha的点数量相对于入口的偏移量
#define I51KITG2_OFFSET_ALPHANUMER     0X12
// 图片容器中图片像素信息相对于入口的偏移量
#define I51KITG2_OFFSET_PIXELDATA     0X14

// 图片编码方式为正常编码,按行优先存储每个像素的信息
#define I51KITG2_DECODE_NORMAL         1
// 图片编码方式为线性压缩编码方式
// 把图像像素信息按行优先进行线性编码，数据格式如下：
// x,y,w,RGB数据，其中x，y是非透明色的起点坐标，w是连续有多少个非透明色，x，y分别占用1.5字节，w占两个字节。
#define I51KITG2_DECODE_LINEAR           2
// 图片编码方式为离散压缩方式
// 把图像像素信息按行优先进行离散方式编码，数据格式如下：
// Y,w,X1,X2……Xw，图像信息，其中Y，是非透明色的Y坐标，w是这一行有多少个非透明色，
// X1,X2……Xw分别是每一个非透明色的X坐标，Y，w和Xi都占1.5个字节，当w为奇数时，Xw占两个字节。
#define I51KITG2_DECODE_SCATTER        3

// 显示图片时把原图全部显示到目标区域的标记,也就是图片区域传空时使用
//#define I51KITG2_DRAW_IMAGE_ALL        0XFFF

// 从文件中读取图片信息的Buffer大小
#define I51KITG2_LENGTH_DATA_BUFF     0X400

// 显示图片时按正常方向显示
#define I51KITG2_DRAW_IAMGE_NORMAL    0X01
// 显示图片时按镜像方向显示
#define I51KITG2_DRAW_IAMGE_MIRROR    0X02
// 显示图片时按倒立方向显示
#define I51KITG2_DRAW_IAMGE_HANDSTAND 0X04
// 显示图片时按镜像倒立方式显示
#define I51KITG2_DRAW_IAMGE_MIRROR_HANDSTAND ( (I51KITG2_DRAW_IAMGE_MIRROR)|(I51KITG2_DRAW_IAMGE_HANDSTAND))
// 显示图片时按任意角度旋转
#define I51KITG2_DRAW_IAMGE_ROTATE   0XEE

// 分段压缩的图片文件解压后每段最大长度
#define I51KITG2_LENGTH_PART_UMCOMPRESS  32768

// 元素类型是图片
#define I51KITG2_ELEMENT_TYPE_PICTURE    1
// 元素类型是点
#define I51KITG2_ELEMENT_TYPE_POINT        2
// 元素类型是直线
#define I51KITG2_ELEMENT_TYPE_LINE           3
// 元素类型是多边形
#define I51KITG2_ELEMENT_TYPE_POLYGON   4
// 元素类型是弧线
#define I51KITG2_ELEMENT_TYPE_ARC            5
// 元素类型是圆
#define I51KITG2_ELEMENT_TYPE_CIRCLE       6
// 元素类型是填充多边形
#define I51KITG2_ELEMENT_TYPE_FILLPOLYGON  7
// 元素类型是填充弧形
#define I51KITG2_ELEMENT_TYPE_FILLARC      8
// 元素类型是填充圆形
#define I51KITG2_ELEMENT_TYPE_FILLCRICLE  9
// 元素类型是文字
#define I51KITG2_ELEMENT_TYPE_WORDS  10
// 元素类型是填充图片
#define I51KITG2_ELEMENT_TYPE_FILLIMAGE  11

// 系统最多能创建画板个数
#define I51KITG2_MAX_NUMBER_VIRTUAL_PANEL 10
// 图片容器内最多存放的图片数量
#define I51KITG2_CONTAINER_NUMBER_IMAGE 256
// 圆周率
//#define I51KITG2_PI 3.1416

// 字库文件文件类型地址
#define i51KITG2_FONT_FILETYPE 4
// 字库文件字模类型地址
#define i51KITG2_FONT_MODELTYPE 9
// 字库文件字模宽度地址
#define i51KITG2_FONT_MODELWIDTH 10
// 字库文件字模高度地址
#define i51KITG2_FONT_MODELHEIGHT 11
// 字库文件常用汉字个数地址
#define i51KITG2_FONT_NORMALWORDS_NUM 12
// 字库文件常用字符字模偏移量地址
#define i51KITG2_FONT_NORMALWORDS_OFFSET 14
// 字库文件所有字符字模偏移量地址
#define i51KITG2_FONT_ALLWORDS_OFFSET 18
// 汉字个数
#define i51KITG2_FONT_WORDS_NUMBER 8178

// 旋转类型是绕点旋转
#define i51KITG2_ROTATE_POINT 0X01
// 旋转是绕X轴方向
#define i51KITG2_ROTATE_XLINE 0X02
// 旋转是绕Y轴方向
#define i51KITG2_ROTATE_YLINE 0X03

typedef struct
{
    // author : Otto
    // since : 2011.10.11
    // (C) 2011 , PKIG Tech. Co., Ltd.

    // 字库资源文件句柄
    iFILE FontFileHandle;
    // 字库字模文件在资源文件中的ID
    iFILE TypeheadID;
    // 字模高度	
    iU8   WordHeight;
    // 字模宽度	
    iU8   WordWidth;
    // 取字模类型
    iU8  TypeheadType;
    // 常用汉字个数	
    iS16  NormalUseWordsNum;	
    // 常用汉字内存地址，如果无Cach则为iNULL
    iU32  NormalWordsEntry;
    // 常用字符字模在文件内的开始位置偏移量
    iU32  NormalWordsOffset;
    // 全部字符字模在文件内的开始位置偏移量
    iU32  AllWordsOffset;
    // 字库文件在资源文件中的开始位置
    iU32  TypeheadOffset;
}i51KitG2Font;

// 字库管理器的个数
#define MAX_TYPEHEAD_NUMBER 3
// 字库管理器
static i51KitG2Font i51KitG2FontManage[MAX_TYPEHEAD_NUMBER];
// 一个字的字模最多占用字节数
// 最大支持文字字高和字宽都占16位,一个字占2*16,32个字节
// 目前支持的字模是从左往右，从上往下的顺序存储
#define NUMBER_TYPE_MATRIX           32

typedef struct
{

    // author : Otto
    // since : 2011.9.29
    // (C) 2011 , PKIG Tech. Co., Ltd.

    // 文字的机内码
    iU16 CharactersKeywords;
    // 文字的具体字模
    iU8   TypeMatrix[NUMBER_TYPE_MATRIX];
}i51KitFontWord;

//static i51KitFontWord * i51KitFontNormalWordStock = iNULL;

typedef struct
{

    // author : Otto
    // since : 2011.8.19
    // (C) 2011 , PKIG Tech. Co., Ltd.

    // notes:  管理图片容器中一张图片

    // 容器内是否有图片数据的标记
    iU8 FlagImageExist;
    // 内存中是否有图片数据的标记
    iU8 FlagCopyRAM;
	
    // 图片编码方式
    // 图片有三种编码方式

    // 0X01：正常编码，按行优先存储每个像素的信息。
    // 0X02：线性压缩编码方式，把图像像素信息按行优先进行线性编码，数据格式如下：x,y,w,RGB数据，
    //       其中x，y是非透明色的起点坐标，w是连续有多少个非透明色，x，y分别占用1.5字节，w占两个字节。
    // 0X03：离散压缩方式，把图像像素信息按行优先进行离散方式编码，数据格式如下：Y,w,X1,X2……Xw，图像信息，
    //       其中Y，是非透明色的Y坐标，w是这一行有多少个非透明色，X1,X2……Xw分别是每一个非透明色的X坐标，Y，
    //       w和Xi都占1.5个字节，当w为奇数时，Xw占两个字节。
    iU8 DecodeType;
    // 图片是否有数据点带Alpha的标记
    iU16 AlphaPixelNum;
    // 图片宽度	
    iU16 ImageWidth;	
    // 图片高度
    iU16 ImageHeight;	
    // 图片数据总长度	
    iU32 LengthTotalData;	
    // 图片数据基本点长度	
    iU32 LengthIamgeData;	
    // 图片数据的入口
    // 图片数据在容器文件中的开始地址偏移量
    iU32 ImageEntry;
    // 图片数据的内存开始地址
    iU32 MemoryEntry;
    // 图片是第几次加载进内存的标记
    iU32 SetMemoryCopyCount;
} i51ImageIndex ;

// 图片是第几次加载进内存的计数
static iU32 SetMemoryCopyCount = 0;
// 当前使用的图片管理器编号
static iU32 CurrContainerManageNum = 0;
// 图片加载进内存是否是在resume状态
static iU8  RESUNE_MEMORY_COPY = iFALSE;

typedef struct
{

    // author : Otto
    // since : 2011.8.19
    // (C) 2011 , PKIG Tech. Co., Ltd.

    // notes:  管理图片容器

    // 打开的图片容器文件指针
    iFILE FileHandle;
    // 容器内图片总数
    iU16 ImageTotalNumber;
    // 容器内图片最大ID
    iU16 ImageMaxId;
    // 管理容器内图片状态的索引表
    i51ImageIndex * ImageIndexTable;
} i51ImageContainer ;


typedef struct
{

    // author : Otto
    // since : 2011.9.3
    // (C) 2011 , PKIG Tech. Co., Ltd.

    // notes:  虚拟画板中的元素信息

    // 元素编号
    iU16 ElementNumber;
    // 元素类型
    iU8 ElementType;
    // 元素Alpha值
    iU8 ElemenAlpha;
    // 元素信息
    union
    {
        // 图像
        struct
        {
            // 绘制图片所在容器的句柄
            iHANDLE ContainerHandle;
            // 绘制方向
            iU8  DrawDirection;
            // 绘制图象句柄
            iU32 ImageHandle;
            // 旋转图片的旋转角度			
            iS32 DarwAngle    ;
            // 图片的原始区域
            iRECT SrcRect;
            // 图片绘制到屏幕上的区域
            iRECT DesRect;
            // 旋转图片移动的X坐标
            iS32 SrcMoveX;
            // 旋转图片移动的Y坐标
            iS32 SrcMoveY;
            // 旋转图片的旋转中心X坐标
            iS32 RotateCenterX;
            // 旋转图片的旋转中心Y坐标
            iS32 RotateCenterY;
            // 旋转图片的旋转类型
            iU32 RotateType;
        }Image;

        // 点
        struct
        {
            // 绘制点的X坐标
            iS32 X;
            // 绘制点的Y坐标
            iS32 Y;
            // 绘制点的颜色值
            iCOLOR Color;
        }Point;

        // 直线
        struct
        {
            // 绘制直线的两个端点坐标
            iS32 Points[2][2];
            // 绘制直线的颜色
            iCOLOR Color;
        }Line;

        // 多边形
        struct
        {
            // 绘制多边形的顶点数
            iU32 VerticeNumber;
            // 绘制多边形的顶点坐标指针
            iU32 PointerPoints;
            // 绘制多边形的颜色
            iCOLOR Color;
        }Polygon;

        // 弧线
        struct
        {
            // 弧线上的起点、终点和弧线上一点的坐标
             iS32 Points[3][2];
            // 绘制弧线的颜色
             iCOLOR Color;
        }Arc;

        // 圆
        struct
        {
            // 圆心X坐标
            iS32 X;
            // 圆心Y坐标
            iS32 Y;
            // 圆半径
            iU32 R;
            // 绘制圆的颜色
            iCOLOR Color;
        }Circle;

        // 填充多边形
        struct
        {
            // 填充多边形的顶点数
            iU32 VerticeNumber;
            // 填充多边形的顶点坐标指针
            iU32 PointerPoints;
            // 填充多边形的颜色
            iCOLOR Color;
        }FillPolygon;

        // 填充弧形
        struct
        {
            // 填充弧线的起点、终点和弧线上一点的坐标
             iS32 Points[3][2];
            // 填充弧线的颜色
             iCOLOR Color;
        }FillArc;

        // 填充圆
        struct
        {
            // 填充圆的圆心X坐标
            iS32 X;
            // 填充圆的圆心Y坐标
            iS32 Y;
            // 填充圆的半径
            iU32 R;
            // 填充圆的颜色
            iCOLOR Color;
        }FillCircle;

        struct
        {
            // 字库所在资源文件Handle
            iFILE ReFileHandle;
            // 字库文件在资源文件中的ID			
            iU32 TypeheadID;
            // 文字句柄			
            iU32 WordsHandle;
            // 文字绘制区域			
            iRECT   DrawRect;
            // 文字原始区域			
            iRECT   SrcRect;
            // 绘制文字颜色			
            iCOLOR fgcolor; 		
            // 绘制背景颜色			
            iCOLOR bgcolor;
            // 绘制文字方向			
            iU16 DrawDirect;
            // 绘制文字特效			
            iU16 Effect; 
            // 绘制文字个数			
            iU16 WordsNum;
            // 绘制文字行间距
            iS16 WordsRowGap;		
            // 绘制位子列间距
            iS16 WordsColumnGap;			
	}Words;
        // 图像
        struct
        {
            // 绘制图片所在容器的句柄
            iHANDLE ContainerHandle;
            // 绘制图象句柄
            iU32 ImageHandle;
            // 图片填充区域的图片原始区域
            iRECT SrcRect;
            // 图片填充区域的屏幕区域
            iRECT DesRect;
        }Fill;		
    }ElementData;

} i51KitG2VPElement;

typedef struct
{

    // author : Otto
    // since : 2011.9.5
    // (C) 2011 , PKIG Tech. Co., Ltd.

    // notes:  一张虚拟画板的信息

    // 画板在管理器中的编号
    iU16 VP_ID;
    // 画板句柄
    iHANDLE VPHandle;
    // 画板是否已经画满
    iBOOL VpIsFull;
    // 画板是否有区域能绘制	
    iBOOL VpIsDraw;
    // 画板是否循环
    iBOOL VpIsLoop;
    // 画板内最多元素个数
    iU16 VPMaxElement;
    // 画板内当前已经绘制元素个数
    iU16 VPCurrentElement;
    // 画板相对屏幕的Rect  
    iRECT VPActualRect;
    // 绘制的区域
    iRECT VPDrawRect;
    // 元素信息列表
    i51KitG2VPElement * VPElementDetails;  
}i51KitG2VPStructure;

typedef struct
{

    // author : Otto
    // since : 2011.9.5
    // (C) 2011 , PKIG Tech. Co., Ltd.

    // notes:  虚拟画板管理器

    // 是否使用虚拟画板标记
    iU8 UseVirtualPanel;
    // 创建的画板数量
    iU16 TotalVPNumber;
    // 最多画板数量
    iU16 MaxVPNumber;
    // 当前活动画板编号
    iU16 ActivityVp_ID;
    // 当前活动画板指针
    i51KitG2VPStructure * CurrentVpActivity;
    // 当前绘制画板指针
    i51KitG2VPStructure * CurrentVpRender;
    // 虚拟画板信息列表
    i51KitG2VPStructure * VPDetails[I51KITG2_MAX_NUMBER_VIRTUAL_PANEL];  
} i51KitG2VPManage;

typedef struct
{
    // 资源文件指针
    iFILE SrcFile;
    // 解码后存放文件指针
    iFILE DesFilePointer;
    // 解码后保存数据文件的路径和文件名	
    iU16   DesFileName[50];	
    // 图片ID数组地址
    iU32 ImageId;
    // 最大图片ID
    iU16 MaxImageID;
    // 图片总数	
    iU16 TotallImage;	
    // 已经解码玩图片数量
    iU16 CompleteImage;	
    // 正在解码的图片ID
    iU16 GoingImageID;	
    // 解码进度回调函数	
    void (*Process)(iU32  , iU32 , iHANDLE);
    // 解码完成回调函数
    void (*Done)(iHANDLE);
	
}i51KitG2ZlibImage;

typedef struct i51ContainerManage
{
	// 打开的容器句柄
	i51ImageContainer * ContainerHandle;
	// 容器中图片加载进内存的数量
	iS32 MemoryCopyImageNum;
	// 容器中图片加载进内存的图片ID
	iU32 * ImageID;
	// 容器中图片是第几次被加载进内存的
	iU32 * MemCopyCount;
	// 管理器是否空闲的标记
	iU8 Flag;
}i51ContainerManage;

// 图片容器管理器的个数
#define MAX_ContainerManage_NUM	10
// 图片容器管理器
i51ContainerManage ContainerManage[MAX_ContainerManage_NUM];

// 当前屏幕宽度
static iU16 ScreenWidth = 0;

// 当前屏幕高度
static iU16 ScreenHeight = 0;

// 显存
static iU16* LcdBuffer = 0;

// 屏幕信息数组
static iU32 * LcdInfoArr;

// 图片容器指针
//static i51ImageContainer CurrentImageContainer ;
//正在解码图片容器指针
static i51ImageContainer * ZlibImageContainer ;

// 当前虚拟画板管理器
static i51KitG2VPManage i51CurrentVpManage;
// 当前活动的虚拟画板
static i51KitG2VPStructure * i51CurrentVpActivity = iNULL;
// 当前需要绘制的虚拟画板
static i51KitG2VPStructure * i51CurrentVpRender = iNULL;
// 是否使用虚拟画板标记
static iU8 i51UseVirtualPanel = iFALSE;
// 是否绘制到虚拟面板的标记
static iU8 i51DrawVirtualPanel = iFALSE;
  
// 图片旋转中心坐标
//static iS32 RotateCenterX = 120;
//static iS32 RotateCenterY = 160;

// 解码图片定时器ID
static iU32 TimeIdZlibDecodeImage = iNULL;
// 解码图片标记
static iU32 FlagDecodeImage = iNULL;
// 解码图片数据结构
static i51KitG2ZlibImage ManageZlibDecodeImage;

// 一行或者一列宽度的颜色buf
static iU16 * OneRowColorBuf = iNULL;

//系统当前的Alpha值
static iALPHA AlphaSys = i51KITG2_ALPHA_NONEED;
// 原图的Alpha
static iALPHA AlphaSrc = i51KITG2_ALPHA_NONEED;
// 目的地的Alpha
static iALPHA AlphaDes = 0;

// sin0~90的查找表，表中数据是把正确数据放到了16384倍，也就是左移了14位
static iS32 SinData[91] = 
{
    0, 285,  571,  857, 1142, 1427, 1712, 1996, 2280, 2563, 2845,
    3126, 3406, 3685, 3963, 4240, 4516, 4790, 5062, 5334, 5603,
    5871, 6137, 6401, 6663, 6924, 7182, 7438, 7691, 7943, 8191,
    8438, 8682, 8923, 9161, 9397, 9630, 9860, 10086, 10310, 10531,
    10748, 10963, 11173, 11381, 11585, 11785, 11982, 12175, 12365, 12550,
    12732, 12910, 13084, 13254, 13420, 13582, 13740, 13894, 14043, 14188,
    14329, 14466, 14598, 14725, 14848, 14967, 15081, 15190, 15295, 15395,
    15491, 15582, 15668, 15749, 15825, 15897, 15964, 16025, 16082, 16135,
    16182, 16224, 16261, 16294, 16321, 16344, 16361, 16374, 16381, 16384,
};

#if DEBUG_I51KITG2
// 测试用变量，程序是否第一次运行标记
 static iU8 FLAG_FIRSTRUN  = iTRUE;
iU8 DecodeComplete = iFALSE;
iU8    UseCach = iFALSE;
iHANDLE CurrentImageContainer = iNULL;
iFILE TypeheadHandle = iNULL;
iU32 ZoomRateX = 100;
iU32 ZoomRateY = 100;
static iHANDLE VP_BK = iNULL;
static iHANDLE VP_Road = iNULL;
static iHANDLE VP_Snidget  = iNULL;
static iHANDLE VP_Bee = iNULL;
static iU8 Flag = iTRUE;
#endif

i51KIT_STATIC_KIT_DEF(Adapter);

static iCOLOR i51KitG2GetNewColor (iCOLOR dpixel, iCOLOR spixel)
{
/**
 * @fn		unsigned short calculate_alpha (unsigned short dpixel, unsigned short spixel, unsigned char alpha)
 * @brief	计算alpha值。alpha算法：dest <- (dest*(255 - alpha) + src * alpha) / 255
 * 
 * @param	dpixel	目的像素值
 * @param	spixel	源像素值
 * @param	alpha	alpha值
 * 
 * @return	返回alpha运算结果
 */
    iU32 rd, gd, bd;
    iU32 rs, gs, bs;
	
    if( AlphaSrc==0XFF ) return spixel;
    if( AlphaSrc==0 ) return dpixel;

    rs = (spixel) & 0XF800;
    gs = (spixel) & 0X07E0;
    bs = spixel & 0X1F;
                                
    rd = (dpixel) & 0XF800;
    gd = (dpixel) & 0X07E0;
    bd = dpixel & 0X1F;
                                
    rd = ((rd*AlphaDes+ rs * AlphaSrc)>>8)&0XF800;
    gd = ((gd*AlphaDes+ gs * AlphaSrc)>>8)&0X7E0;
    bd = ((bd*AlphaDes+ bs * AlphaSrc)>>8)&0X1F;
    return (bd +(gd)+ (rd ));

}

static iCOLOR i51KitG2GetNewColorEx (iCOLOR dpixel, iCOLOR spixel, iALPHA SrcAlpha)
{
/**
 * @fn		unsigned short calculate_alpha (unsigned short dpixel, unsigned short spixel, unsigned char alpha)
 * @brief	计算alpha值。alpha算法：dest <- (dest*(255 - alpha) + src * alpha) / 255
 * 
 * @param	dpixel	目的像素值
 * @param	spixel	源像素值
 * @param	alpha	alpha值
 * 
 * @return	返回alpha运算结果
 */
    iU32 rd, gd, bd;
    iU32 rs, gs, bs;
    iALPHA DesAlpha = 0XFF - SrcAlpha;

    if( SrcAlpha==0XFF ) return spixel;
    if( SrcAlpha==0 ) return dpixel;
	
    rs = (spixel) & 0XF800;
    gs = (spixel) & 0X07E0;
    bs = spixel & 0X1F;
                                
    rd = (dpixel) & 0XF800;
    gd = (dpixel) & 0X07E0;
    bd = dpixel & 0X1F;
                                
    rd = ((rd*DesAlpha+ rs * SrcAlpha)>>8)&0XF800;
    gd = ((gd*DesAlpha+ gs * SrcAlpha)>>8)&0X7E0;
    bd = ((bd*DesAlpha+ bs * SrcAlpha)>>8)&0X1F;
    return (bd +(gd)+ (rd ));

}

static  iU16 * MMIAPICOM_Wstrcat( iU16* dst, const iU16 * src  )
{
    // author : Otto
    // since : 2011.8.19
    // (C) 2011 , PKIG Tech. Co., Ltd.

    // notes:  字符串连接函数
  
    iU16 * cp = dst;
  
    if( iNULL== src || iNULL == dst )
    {
        return dst;
    }

    // find end of dst
    while( *cp )
    {
        cp++;
    }

     // copy src to end of dst
    while( iNULL != (*cp++ = *src++) ) ;   /* copy src to end of dst */

    // return dst
    return( dst );            
}

#if LCDBUFF_DEBUG
static iBOOL i51KitG2CheckLcdbuffer( iS32 StartBuffer , iS32 Length )
{
    iS32 Max =ScreenHeight*ScreenWidth;

    if( Length<0 )
    {
        iLog( "KIT - G2 : lcdbuffer Len"); 
        return iFALSE;
    }
	
    if( StartBuffer<0||StartBuffer>=Max ||(StartBuffer+Length)>Max)
    {
        iLog( "KIT - G2 : lcdbuffer "); 
        return iFALSE;
    }
    return iTRUE;
}
#endif

static iU16 CountWordsNum( iCHAR * Str)
{
    // 统计中英文混编的字符串字符个数
    iU16 WordsNum = 0;
    iU8 Ch = 0;
    iU16 Temp = 0;
    iS32 StrLength = strlen(Str);
    for( Temp=0; Temp<StrLength; Temp++ )
    {
        WordsNum++;
        Ch = Str[Temp];
        if( Ch>0XA0) Temp++;
    }
    return WordsNum;
}

static iBOOL i51KitG2FontFindKey(iU16 Key, iU16 * KeyPos, i51KitG2Font * CurrTypehead)
{
    // 查找常用字库中的汉字，常用字库是按照汉字的机内码为关键字升序排序
    // 因为是有序序列，所以查找算法使用的是二分查找
    iS16 StartWord =0;
    iS16 EndWord = CurrTypehead->NormalUseWordsNum-1;
    iS16 Temp;
    i51KitFontWord * NormalWordStock = iNULL;

    int Temp1 = 0;
	
    NormalWordStock = ( i51KitFontWord * )(CurrTypehead->NormalWordsEntry);
    if( !NormalWordStock )
    {
        return iFALSE;
    }
	
    while(StartWord<EndWord)
    {	
        Temp1 ++;
        Temp = ((StartWord+EndWord)>>1);
        if( NormalWordStock[Temp].CharactersKeywords>Key)
        {
            EndWord = Temp-1;
        }
        else if( NormalWordStock[Temp].CharactersKeywords<Key)
        {
            StartWord = Temp+1;
        }
        else
        {
            *KeyPos = Temp;
            return iTRUE;
        }
    }

    if( NormalWordStock[StartWord].CharactersKeywords==Key ) 
    {
        * KeyPos = StartWord;
        return iTRUE;
    }
	
    return iFALSE;

}

static iBOOL i51KitG2Create( void )
{ 
    // author : Otto
    // since : Aug 10 2011
    // (C) 2011 , PKIG Tech. Co., Ltd.
  
    // notes : 初始化i51KitG2系统

    iU32 Temp = 0;
#if i51KITG2_DEBUG
    iLog("Start i51KitG2Create");
#endif
    if( i51AdeMmiGetScreenScale ( &ScreenWidth , &ScreenHeight ) == 0 ) goto ERROR;

    LcdInfoArr =  i51AdeMmiGetPanel();
    if( iNULL == LcdInfoArr ) goto ERROR;

    LcdBuffer = (iU16 *) LcdInfoArr[1] ;
  
    i51CurrentVpManage.ActivityVp_ID = 0;
    i51CurrentVpManage.CurrentVpActivity = iNULL;
    i51CurrentVpManage.CurrentVpRender = iNULL;
    i51CurrentVpManage.MaxVPNumber = I51KITG2_MAX_NUMBER_VIRTUAL_PANEL;
    i51CurrentVpManage.TotalVPNumber = 0;
    i51CurrentVpManage.UseVirtualPanel = iFALSE;
    i51CurrentVpManage.VPDetails[0] = iNULL;

//    CurrentImageContainer.FileHandle  = iNULL;
//    CurrentImageContainer.ImageIndexTable = iNULL;
//    CurrentImageContainer.ImageMaxId = 0;
//    CurrentImageContainer.ImageTotalNumber = 0;

//    ZlibImageContainer.FileHandle  = iNULL;
//    ZlibImageContainer.ImageIndexTable = iNULL;
//    ZlibImageContainer.ImageMaxId = 0;
  //  ZlibImageContainer.ImageTotalNumber = 0;
    ZlibImageContainer = iNULL;

    for( Temp=0; Temp<MAX_TYPEHEAD_NUMBER; Temp++ )
    {
        i51KitG2FontManage[Temp].FontFileHandle = iNULL;
        i51KitG2FontManage[Temp].TypeheadID = iNULL;
        i51KitG2FontManage[Temp].NormalUseWordsNum = iNULL;
        i51KitG2FontManage[Temp].NormalWordsEntry = iNULL;
        i51KitG2FontManage[Temp].NormalWordsOffset = iNULL;
        i51KitG2FontManage[Temp].AllWordsOffset = iNULL;
        i51KitG2FontManage[Temp].TypeheadType = iNULL;
        i51KitG2FontManage[Temp].WordHeight = 0;
        i51KitG2FontManage[Temp].WordWidth = 0;
        i51KitG2FontManage[Temp].TypeheadOffset = 0;
    }

	FlagDecodeImage = iFALSE;

    memset ( ContainerManage, 0 , (sizeof(i51ContainerManage))*MAX_ContainerManage_NUM) ;

    SetMemoryCopyCount = 0;
    CurrContainerManageNum = 0;
    RESUNE_MEMORY_COPY = iFALSE;	
	
    if( ScreenHeight>ScreenWidth)
    {
        OneRowColorBuf = ( iU16 * )i51AdeOsMalloc( (ScreenHeight<<1),  __FILE__ , __LINE__ );
    }
    else
    {
        OneRowColorBuf = ( iU16 * )i51AdeOsMalloc( (ScreenWidth<<1),  __FILE__ , __LINE__ );
    }
    if( iNULL==OneRowColorBuf)
    {
        iLog("KIT - G2 :init malloc");
        goto ERROR;
    }
	
#if i51KITG2_DEBUG
    iLog("End i51KitG2Create");
#endif
    return iTRUE;
	


ERROR:
    iLog("KIT - G2 :init ");
    return iFALSE;
	
}

static iBOOL i51KitG2Release( void  )
{

    // author : Otto
    // since : Aug 10 2011
    // (C) 2011 , PKIG Tech. Co., Ltd.

    // notes : 退出i51KitG2系统

    ScreenWidth = 0;
    ScreenHeight = 0;
    LcdBuffer = 0;

    if(OneRowColorBuf)
    {
        i51AdeOsFree(OneRowColorBuf);
        OneRowColorBuf = iNULL;
    }
	
    return iTRUE;
	
}

static iBOOL i51KitG2CheckFileCRC ( iFILE handle )
{
    iU32 nCRC = 0;
    
    if(iNULL==i51AdeOsFileSetSeek(handle, 0, i51_OS_FL_SEEK_HEAD)) goto ERROR;
    if(iNULL==i51AdeOsFileReadInt32 ( handle , (iU32*)&nCRC)) goto ERROR ;

    if(i51AdeCnfGetEndian()&&nCRC == 9) return iTRUE;
    else if(nCRC==1) return iTRUE;
    else return iFALSE;


ERROR:
    iLog("KIT - G2 : RW check crc");
    return iFALSE;
}

static iBOOL i51KitG2WriteFileCRC ( iFILE handle )
{

    iU32 nCRC = 1 ;

    if(i51AdeCnfGetEndian()) nCRC = 9;
    else nCRC = 1;

    if(iNULL==i51AdeOsFileSetSeek(handle, 0, i51_OS_FL_SEEK_HEAD)) goto ERROR;
    i51AdeOsFileWriteInt32( handle, &nCRC);
    i51AdeOsFileFlush(handle);

    return iTRUE;

ERROR:
    iLog("KIT - G2 : RW write crc");
    return iFALSE;
	
}

static  void QuickSort1(iS16 pData[],iU16 left,iU16 right)
{

    // author : Otto
    // since : Aug 15 2011
    // (C) 2011 , PKIG Tech. Co., Ltd.

    // notes : 数组pData按升序排序
#if 0
    iS16 i,j;
    iS16 middle,iTemp;
    i = left;
    j = right;
    middle = pData[left];
    do{
      while( (pData[i]<middle) && (i<right) )//从左扫描小于中值的数
      i++;
      while( ( pData[j]>middle) && (j>left) )//从右扫描大于中值的数
      j--;
      if(i<=j)//找到了一对值
     {
      //交换
      iTemp = pData[i];
      pData[i] = pData[j];
      pData[j] = iTemp;
      i++;
      j--;
     }
    }while(i<=j);//如果两边扫描的下标交错,就停止（完成一次）
    //当左边部分有值(left<j),递归左半边
    if(left<j)
    QuickSort1(pData,left,j);
    //当右边部分有值(right>i),递归右半边
    if(right>i)
    QuickSort1(pData,i,right);
#endif	
#if 1
// 选择排序
	iS16 i,j;
	iS16 MinData,MinPos;
	iS16 Temp;

	for(i=0;i<=right;i++)
	{
		MinData= pData[i];
		MinPos = i;
		for(j=i+1;j<=right;j++)
		{
			if(pData[j]<MinData)
			{
				MinData = pData[j];
				MinPos = j;
			}
		}
		
		if(MinPos==i) continue;
		
		Temp = pData[i];
		pData[i] = pData[MinPos];
		pData[MinPos] = Temp;		
	}
	
#endif
}

static  void QuickSort2(iS16 pData[][2],iU16 left,iU16 right)
{

	// author : Otto
	// since : Aug 15 2011
	// (C) 2011 , PKIG Tech. Co., Ltd.

	// notes : 数组pData按pData[][1]升序排序
#if 0
	iS16 TempLeft = 0, TempRight = 0;
	iS16 MiddleData = 0, TempData;
	MiddleData = pData[left][1];
	TempLeft = left;
	TempRight = right;

	while(TempLeft<TempRight)
	{
		//从左扫描大于中值的数
		while((pData[TempLeft][1]<=MiddleData)&&(TempLeft<right)) TempLeft++;

		//从右扫描小于中值的数
		while((pData[TempRight][1]>=MiddleData)&&(TempRight>left)) TempRight--;

		// 交换
		if( TempLeft<TempRight)
		{
			TempData= pData[TempRight][0];
			pData[TempRight][0] = pData[TempLeft][0];
			pData[TempLeft][0] = TempData;
			
			TempData = pData[TempRight][1];
			pData[TempRight][1] = pData[TempLeft][1];
			pData[TempLeft][1] = TempData;

			TempLeft++;
			TempRight--;
		}
		
	}

	if(TempRight>left) 	QuickSort2(pData, left,TempRight);

	if(TempLeft<right) 	QuickSort2(pData, TempLeft,right);
#endif	
#if 1
// 选择排序
	iS16 i,j;
	iS16 MinData,MinPos;
	iS16 Temp;

	for(i=0;i<=right;i++)
	{
		MinData= pData[i][1];
		MinPos = i;
		for(j=i+1;j<=right;j++)
		{
			if(pData[j][1]<MinData)
			{
				MinData = pData[j][1];
				MinPos = j;
			}
		}
		
		if(MinPos==i) continue;
		
		Temp = pData[i][0];
		pData[i][0] = pData[MinPos][0];
		pData[MinPos][0] = Temp;
		
		Temp = pData[i][1];
		pData[i][1] = pData[MinPos][1];
		pData[MinPos][1] = Temp;
		
	}
#endif
}

static iS32 i51KitG2Sin( iS32 Angle)
{
    //iS32 Angle: 需要计算的角度值
    // notes : 返回角度Angle的正玄放大16384倍(相当于左移14位)的值
    iS32 ActualAngle = 0;
    iS32 Ret = 0;

    ActualAngle = Angle%360;
    if( ActualAngle<0) ActualAngle = 360+ActualAngle;

    if(ActualAngle<=90) Ret = SinData[ActualAngle];
    else if ( ActualAngle<=180) Ret = SinData[180-ActualAngle];
    else if ( ActualAngle<=270) Ret = -SinData[ActualAngle-180];
    else Ret = -SinData[360-ActualAngle];

    return Ret;
	
}

static iS32 i51KitG2Cos( iS32 Angle)
{
    //iS32 Angle: 需要计算的角度值
    // notes : 返回角度Angle的正玄放大10000倍的值
    return i51KitG2Sin(Angle+90);
}

static iBOOL i51KitDrawWaterLine( iS32 Point[][2], iRECT VPDrawRect, iRECT VPActualRect, iCOLOR*ColorBuf)
{
// 绘制一条水平线
// DrawRect需要在屏幕上绘制的区域
// ActualRect,VP的实际大小和相对屏幕的偏移
//  Point[][2],水平线的起点和终点坐标
    iS32 DrawX1, DrawX2;
    // 绘制区域	
    iS16 DrawStartX = 0;
    iS16 DrawStartY = 0;
    iS16 DrawWidth = 0;
    iS32 BuffPos = 0;
    iU32 iTemp = 0;
	
    DrawX1 = Point[0][0];
    DrawX2 = Point[1][0];

	if( DrawX2<DrawX1 ) return iFALSE;
   
    if(!i51UseVirtualPanel)
    {
        // 绘制是直接在物理层
        DrawStartX = VPDrawRect.X;
        DrawStartY = Point[0][1];
    }
    else
    {
        // 绘制是在VP上,基本图形接口VP是否循环都按不循环处理
        DrawStartX = VPDrawRect.X + VPActualRect.X;
        DrawStartY = Point[0][1]+ VPActualRect.Y;
    }

    DrawWidth = VPDrawRect.Width;
	
    // 绘制区域和屏幕没有交集
    if( DrawStartY>=ScreenHeight  || DrawStartY>=(VPDrawRect.Y+VPDrawRect.Height)
    || DrawStartY< VPDrawRect.Y || DrawStartY< 0  || DrawStartX>=ScreenWidth ) return iTRUE;
    // 待绘制水平线与绘制区域没有交集
    if( DrawX1>=(DrawStartX+DrawWidth) || DrawX2<DrawStartX)  return iTRUE;
    // 绘制水平线结束点在绘制区域右边
    if( DrawX2 > (DrawStartX+DrawWidth ) ) DrawX2 = DrawStartX + DrawWidth;
    // 绘制水平线开始点在绘制区域左边
    if( DrawX1<DrawStartX )  DrawX1= DrawStartX;
    // 绘制水平线开始点在屏幕左边
    if( DrawX1<0 )  DrawX1 = 0;

    // 绘制水平线		
    DrawWidth = DrawX2- DrawX1;
    if( DrawWidth<1 ) return iTRUE;
    BuffPos = ScreenWidth * DrawStartY+ DrawX1;		
#if LCDBUFF_DEBUG
i51KitG2CheckLcdbuffer(BuffPos,DrawWidth);
#endif
    if(AlphaSys==i51KITG2_ALPHA_NONEED)
    {
        i51AdeStdMemcpy16(&LcdBuffer[BuffPos], ColorBuf, (DrawWidth<<1));
    }
    else
    {
        for( iTemp=0; iTemp<DrawWidth; iTemp++ )
        {
            LcdBuffer[BuffPos]=i51KitG2GetNewColor(LcdBuffer[BuffPos], ColorBuf[iTemp]);
            BuffPos++;
        }
    }
    return iTRUE;

}

static iBOOL i51KitDrawDottedLine( iS16 Y, iU16 Num, iS16 X[], iCOLOR * ColorBuf )
{

    // author : Otto
    // since : Aug 15 2011
    // (C) 2011 , PKIG Tech. Co., Ltd.

    // iU16 Y : 绘制虚线的Y坐标
    // iU16 Num : 绘制虚线的顶点个数
    // iU16 X[] : 绘制虚线 的X坐标集
    // iCOLOR Color : 绘制虚线的颜色
  
    // notes : 绘制水平虚线
  
    iU32 i = 0, j = 1;//, k = 0;
    // 当前绘制的元素在VP上的ID
//    iU16 CurrDrawID;
    // 创建的VP实际大小	
    iRECT VPActualRect;
    // 需要绘制的VP区域	
    iRECT VPDrawRect;
    iS32 Point[2][2];	

    if( i51UseVirtualPanel )
    {
        VPActualRect.X = i51CurrentVpRender->VPActualRect.X;
        VPActualRect.Y = i51CurrentVpRender->VPActualRect.Y;
        VPActualRect.Width= i51CurrentVpRender->VPActualRect.Width;
        VPActualRect.Height= i51CurrentVpRender->VPActualRect.Height;

        VPDrawRect.X = i51CurrentVpRender->VPDrawRect.X;
        VPDrawRect.Y = i51CurrentVpRender->VPDrawRect.Y;
        VPDrawRect.Width = i51CurrentVpRender->VPDrawRect.Width;
        VPDrawRect.Height= i51CurrentVpRender->VPDrawRect.Height;
    }
    else
    {
        VPActualRect.X = 0;
        VPActualRect.Y = 0;
        VPActualRect.Width = ScreenWidth;
        VPActualRect.Height = ScreenHeight;

        VPDrawRect.X = 0;
        VPDrawRect.Y = 0;
        VPDrawRect.Width = ScreenWidth;
        VPDrawRect.Height = ScreenHeight;
    }
	
	
   QuickSort1( X, 0, Num-1);

#if 1
    for( i=1; i<Num; i++ )
    {
        if( X[i]!=X[j-1] )
        {
            X[j++] = X[i];
        }
    }
#endif

    Point[0][1] = Y;
    Point[1][1] = Y;
//j=Num;
    for( i=0; i<j-1; i+=2 )
    {
        Point[0][0] = (iS32)X[i];
        Point[1][0] = (iS32)X[i+1];
        i51KitDrawWaterLine(Point, VPDrawRect, VPActualRect, ColorBuf);		
    }
  
    if( (j%2) )
    {
// 剩余一个顶点需要绘制
    }
	
    return iTRUE;
	
}

static iBOOL ZlibDecodeInit( void )
{
    iBOOL FlagReturn = iFALSE;
    iU8   FlagImageExist = 0;	
    iU8   ContainerVersion = 0;
    iU32  SaveFileOffset = 0;
    iU32  iTemp = 0;
    iU32  WrittenDataLength = 0;
//    iU32  NumerRam = 0;
    iU32 MaxImageID = 0;
    iU32 CRC = 0;
    iU32 FileVer = 0;
    iU16 TempWrite16 = 0;
    iU32 TempWrite32 = 0;

    if( iNULL==ManageZlibDecodeImage.SrcFile)
    {
        iLog("KIT - G2 : image resource decode init");
        return iFALSE;
    }

    MaxImageID = ManageZlibDecodeImage.MaxImageID;
    // 解码图片时初始化容器  
    ManageZlibDecodeImage.DesFilePointer =  i51AdeOsFileOpen( ManageZlibDecodeImage.DesFileName, i51_OS_FL_WRITE |i51_OS_FL_ALWAYS_CREATE, __FILE__ , __LINE__ );
    if( ManageZlibDecodeImage.DesFilePointer == iNULL ) 
    {
        iLog("KIT - G2 :Open file decode init");
        return iFALSE;
    }

    TempWrite32 = CRC;
    FlagReturn = i51AdeOsFileWriteInt32( ManageZlibDecodeImage.DesFilePointer,  &TempWrite32);
    if( iFALSE == FlagReturn ) goto ERROR;
	
    FlagReturn =  i51AdeOsFileWrite(ManageZlibDecodeImage.DesFilePointer, "i51C", 4, &WrittenDataLength);
    if( iFALSE == FlagReturn ) goto ERROR;

    ContainerVersion  = I51KITG2_IMAGE_CONTAINER_VERSION;
    FlagReturn =  i51AdeOsFileWriteInt8(ManageZlibDecodeImage.DesFilePointer, &ContainerVersion);
    if( iFALSE == FlagReturn ) goto ERROR;
	
    TempWrite32 = FileVer;
    FlagReturn = i51AdeOsFileWriteInt32( ManageZlibDecodeImage.DesFilePointer,  &TempWrite32);
    if( iFALSE == FlagReturn ) goto ERROR;
	
    TempWrite16 = ManageZlibDecodeImage.TotallImage;
    FlagReturn = i51AdeOsFileWriteInt16( ManageZlibDecodeImage.DesFilePointer,  &TempWrite16);
    if( iFALSE == FlagReturn ) goto ERROR;
  
    TempWrite16 = ManageZlibDecodeImage.MaxImageID;
    FlagReturn = i51AdeOsFileWriteInt16( ManageZlibDecodeImage.DesFilePointer,  &TempWrite16);
    if( iFALSE == FlagReturn ) goto ERROR;
  
    SaveFileOffset = 0X0;
    FlagImageExist = 0;
    for( iTemp=0; iTemp<MaxImageID; iTemp++ )
    {
  
        FlagReturn = i51AdeOsFileWriteInt8( ManageZlibDecodeImage.DesFilePointer,  &FlagImageExist);
        if( iFALSE == FlagReturn ) goto ERROR;
  
        TempWrite32 = SaveFileOffset;
        FlagReturn = i51AdeOsFileWriteInt32( ManageZlibDecodeImage.DesFilePointer,  &TempWrite32);
        if( iFALSE == FlagReturn ) goto ERROR;
  
    }

    ZlibImageContainer = ( i51ImageContainer* )i51AdeOsMalloc( sizeof(i51ImageContainer), __FILE__ , __LINE__ );
    if( iNULL==ZlibImageContainer )
    {
        iLog("KIT - G2 : malloc ZlibImageContainer");
        goto ERROR;
    }
    memset(ZlibImageContainer, 0, sizeof(i51ImageContainer) );
	
    ZlibImageContainer->ImageTotalNumber= ManageZlibDecodeImage.TotallImage;
    ZlibImageContainer->FileHandle = ManageZlibDecodeImage.DesFilePointer;
    ZlibImageContainer->ImageMaxId = ManageZlibDecodeImage.MaxImageID;

    ZlibImageContainer->ImageIndexTable = ( i51ImageIndex * )i51AdeOsMalloc((sizeof(i51ImageIndex))*(MaxImageID), (__FILE__) , __LINE__ );
    if( iFALSE == ZlibImageContainer->ImageIndexTable )
    {
        iLog("KIT - G2 :malloc decode init");
        goto ERROR;
    }
    memset(ZlibImageContainer->ImageIndexTable, 0, sizeof(i51ImageIndex)*MaxImageID );
	
    for( iTemp = 0; iTemp<MaxImageID; iTemp++ )
    {
        ZlibImageContainer->ImageIndexTable[iTemp].FlagImageExist = 0;
        ZlibImageContainer->ImageIndexTable[iTemp].FlagCopyRAM = iFALSE;
    }

    i51AdeOsFileFlush(ManageZlibDecodeImage.DesFilePointer);

    iTemp = 0;	 
    while(1)
    {
        if( iFALSE==ContainerManage[iTemp].Flag )
        {
    	     ContainerManage[iTemp].Flag = iTRUE;
            ContainerManage[iTemp].ContainerHandle = ZlibImageContainer;
//iLog("open ContainerManage[%d].ContainerHandle:%d",iTemp,ContainerManage[iTemp].ContainerHandle);
            break;
        }
        iTemp++;
        if( iTemp>=MAX_ContainerManage_NUM )		
        {
            iLog("KIT - G2 : contain full");
            goto ERROR;			
        }
    }
	
    return iTRUE;

ERROR:
    if( ZlibImageContainer )
    {
        i51AdeOsFree(ZlibImageContainer->ImageIndexTable);
        ZlibImageContainer->ImageIndexTable = iNULL;
    	
        i51AdeOsFree(ZlibImageContainer);
        ZlibImageContainer = iNULL;
    }	
    iLog("KIT - G2 : RW decode init");
    return iFALSE;
	
}

static iBOOL ZlibDecodeOneImageFromFile( iFILE SrcFile , iU32 SrcSeek, iFILE DesFile, iU32 DesSeek, iHANDLE Container, iU32 ImageID )
{
    // author : Otto
    // since : Aug 31 2011
    // (C) 2011 , PKIG Tech. Co., Ltd.

    // iFILE SrcFile : 需要解码的数据存放文件指针
    // iU32 SrcSeek : 需要解码的数据存放在原始文件中的开始地址
    // iFILE DesFile : 解码后的数据存放文件指针
    // iU32 DesFile : 解码后的数据存放目的文件中的开始地址
    // iHANDLE Container : 容器句柄
    // iU32 ImageID : 当前解码的图片在容器文件中的图片ID
    
    // notes : 从文件SrcFile中解压一张图片到文件DesFile中
    // SrcFile文件中存放的图片数据是分段用Zlib压缩的
    // 具体文件格式请参考文档"The_i51_DC_dynamic_library_G2_i51_Image_Format.xls"

    // 从文件中读取的CRC
    iU32 CRC_Read = 0;
    // 目标数据长度，即解压后文件长度
    iU32 LengthDesData = 0;
    // 原始数据长度，即压缩后文件长度
    iU32 LengthSrcData = 0;
    // 解压后段最长长度,默认设置为32K
    iU32 LengthPartMax = 0;
    // 段数量
    iU16 NumberParts = 0;
    // 当前段解压前数据长度
    iU32 LengthPartSrc = 0;
    // 当前段解压后数据长度
    iU32 LengthPartDes = 0;
    // 函数返回值
    iBOOL FlagReturn = iFALSE;
    // 解码前图片数据开始位置 在资源文件中的偏移量
    iU32 SeekFileResource = 0;
    // 解码后图片数据开始位置 在容器文件中的偏移量
    iU32 SeekFileContainer = 0;
    // 文件当前偏移量
//    iU32 FileSeekCureent = 0;
    iU32 LengFactReadData = 0;
    // 实际一次写入文件中的数据长度  
    iU32 LengFactWriteData = 0;
    iU32 iTemp = 0;
    iU32 GoingImageID = ImageID;	
    iU8 FlagImageExist = 1;

    // 解压前段数据buffer
    iU8  * DataSrcCurrentPart = iNULL;
    // 解压后段数据buffer
    iU8  * DataDesCurrentPart = iNULL;
    iU32 DesTempPos = 0;	
    iU32 SrcTempPos = 0;	

    // 图片带调色板
    iU8 PaletteImage = iFALSE;
    // 调色板中颜色总数
    iU16 PaletteColorNum = 0;
    // 调色板颜色索引表
    iCOLOR * PaletteColorMem = iNULL;
    // 一个数据块的长度	:=6+连续点个数
    iS32 BlockLength = 0;
    // 当前数据块位置
    iS32 BlockPlace = 0;
    // 颜色临时变量	
//    iCOLOR TempColor = 0;
    // 颜色指针地址
    iCOLOR * ColorPoint = iNULL;
    // 段数据头	
    iU8 TempHead[6];
    iS32 Temp = 0;	

    i51ImageContainer * ImageContainer = (i51ImageContainer *)Container;


    // 移动文件内指针到容器文件DesSeek位置
    SeekFileContainer = DesSeek;
    FlagReturn = i51AdeOsFileSetSeek( DesFile, DesSeek, i51_OS_FL_SEEK_HEAD) ;
    if( iFALSE == FlagReturn )
    {
		goto ERROR;
    }

    // 获得当前图片数据在原始文件中的偏移量	
    SeekFileResource =  SrcSeek;

    // 移动文件内指针到特定资源数据开始处
    FlagReturn = i51AdeOsFileSetSeek( SrcFile , SeekFileResource, i51_OS_FL_SEEK_HEAD ) ;
    if( iFALSE == FlagReturn )
    {
		goto ERROR;
    }

    FlagReturn = i51AdeOsFileReadInt32( SrcFile, &CRC_Read);
    if( iFALSE == FlagReturn )
    {
		goto ERROR;
    }

    FlagReturn = i51AdeOsFileReadInt32( SrcFile, &LengthDesData);
    if( iFALSE == FlagReturn )
    {
		goto ERROR;
    }

    FlagReturn = i51AdeOsFileReadInt32( SrcFile, &LengthSrcData);
    if( iFALSE == FlagReturn )
    {
		goto ERROR;
    }
  
  
    FlagReturn = i51AdeOsFileReadInt32( SrcFile, &LengthPartMax);
    if( iFALSE == FlagReturn )
    {
		goto ERROR;
    }
  
    FlagReturn = i51AdeOsFileReadInt16( SrcFile, (iU16 *)(&NumberParts));
    if( iFALSE == FlagReturn )
    {
		goto ERROR;
    }

    if( NumberParts==1 && (LengthDesData+514)<=I51KITG2_LENGTH_PART_UMCOMPRESS )
    {
        // LengthDesData是解码出来的文件长度，如果i51G是8位图，则LengthDesData是解码出来的16位图的长度，也就是去除了调色板信息，下标已经替换成了颜色值
        // 调色板最大长度为2+256*2=514个字节
        DataDesCurrentPart = i51AdeOsMalloc( LengthDesData+514, __FILE__ , __LINE__ );
    }
    else
    {
        DataDesCurrentPart = i51AdeOsMalloc(I51KITG2_LENGTH_PART_UMCOMPRESS, __FILE__ , __LINE__ );
    }
    if( iFALSE == DataDesCurrentPart  ) 
    {
        iLog("KIT - G2 : malloc src decode one ");
        goto ERROR;
    }

    for( iTemp=0; iTemp<NumberParts; iTemp++ )
    {
        DesTempPos = 0;
		
        FlagReturn = i51AdeOsFileReadInt32( SrcFile, (iU32 *)(&LengthPartSrc));
        if( iFALSE == FlagReturn )
        {
            goto ERROR;
        }

        if( iNULL==DataSrcCurrentPart)
        {
            if( NumberParts==1 && (LengthDesData+514)<=I51KITG2_LENGTH_PART_UMCOMPRESS*2 )
            {
                // LengthDesData是解码出来的文件长度，如果i51G是8位图，则LengthDesData是解码出来的16位图的长度，也就是去除了调色板信息，下标已经替换成了颜色值
                // 调色板最大长度为2+256*2=514个字节
                if( LengthPartSrc>LengthDesData)
                {
                    DataSrcCurrentPart = i51AdeOsMalloc(LengthPartSrc+514, __FILE__ , __LINE__ );
                }
                else
                {
                    DataSrcCurrentPart = i51AdeOsMalloc(LengthDesData+514, __FILE__ , __LINE__ );
                }
            }
            else
            {
                DataSrcCurrentPart = i51AdeOsMalloc(I51KITG2_LENGTH_PART_UMCOMPRESS*2, __FILE__ , __LINE__ );
            }
            if( iNULL == DataSrcCurrentPart )
            {
                iLog("KIT - G2 : malloc des decode one");
                goto ERROR;
            }
        }
			

        FlagReturn = i51AdeOsFileRead( SrcFile, DataSrcCurrentPart, LengthPartSrc, &LengFactReadData);
        if( iFALSE == FlagReturn )
        {
            goto ERROR;
        }

        LengthPartDes = I51KITG2_LENGTH_PART_UMCOMPRESS;
        FlagReturn = i51AdeStdUncompress( DataDesCurrentPart, &LengthPartDes, DataSrcCurrentPart, LengthPartSrc);
        if( iFALSE == FlagReturn )
        {
            iLog("KIT - G2 : Uncompress");
            goto ERROR;
        }

        if( iTemp==0 )
        {
            DesTempPos = I51KITG2_OFFSET_PIXELDATA;
            if(DataDesCurrentPart[0]!='i'||DataDesCurrentPart[1]!='5'||DataDesCurrentPart[2]!='1'||DataDesCurrentPart[3]!='G')
            {
                iLog("KIT - G2 : file form");
                goto ERROR;
            }

            if( DataDesCurrentPart[4]>0X40)
            {
                PaletteImage = iTRUE;
                PaletteColorNum = (DataDesCurrentPart[I51KITG2_OFFSET_PIXELDATA]) | (DataDesCurrentPart[I51KITG2_OFFSET_PIXELDATA+1]<<8); 
                PaletteColorMem = i51AdeOsMalloc(PaletteColorNum*2, __FILE__ , __LINE__ );
                if ( iNULL==PaletteColorMem )
                {
                    iLog("KIT - G2 : malloc PaletteColor" );
                    goto ERROR;
                }
                i51AdeStdMemcpy16(PaletteColorMem, &(DataDesCurrentPart[I51KITG2_OFFSET_PIXELDATA+2]), PaletteColorNum*2 );
				
            }
			
            if( iNULL==ImageContainer )
            {
                iLog("KIT - G2 : ZlibImageContainer null" );
		   goto ERROR;
            }
            ImageContainer->ImageIndexTable[GoingImageID].DecodeType = DataDesCurrentPart[I51KITG2_OFFSET_DECODETYPE];
            if(ImageContainer->ImageIndexTable[GoingImageID].DecodeType!=I51KITG2_DECODE_LINEAR)
            {
                iLog("KIT - G2 : 解码图片格式");
                goto ERROR;				
            }
            ImageContainer->ImageIndexTable[GoingImageID].ImageHeight= (DataDesCurrentPart[I51KITG2_OFFSET_IMAGEHEIGHT])|(DataDesCurrentPart[I51KITG2_OFFSET_IMAGEHEIGHT+1]<<8);
            ImageContainer->ImageIndexTable[GoingImageID].ImageWidth= (DataDesCurrentPart[I51KITG2_OFFSET_IMAGEWIDTH])|(DataDesCurrentPart[I51KITG2_OFFSET_IMAGEWIDTH+1]<<8);
            ImageContainer->ImageIndexTable[GoingImageID].AlphaPixelNum= (DataDesCurrentPart[I51KITG2_OFFSET_ALPHANUMER])|(DataDesCurrentPart[I51KITG2_OFFSET_ALPHANUMER+1]<<8);
            ImageContainer->ImageIndexTable[GoingImageID].LengthIamgeData =
   		 	(DataDesCurrentPart[I51KITG2_OFFSET_DATALENGTH])|(DataDesCurrentPart[I51KITG2_OFFSET_DATALENGTH+1]<<8)
   		     |(DataDesCurrentPart[I51KITG2_OFFSET_DATALENGTH+2]<<16)|(DataDesCurrentPart[I51KITG2_OFFSET_DATALENGTH+3]<<24);
            ImageContainer->ImageIndexTable[GoingImageID].LengthTotalData = ImageContainer->ImageIndexTable[GoingImageID].LengthIamgeData ;
            if( ImageContainer->ImageIndexTable[GoingImageID].AlphaPixelNum>0)
            {
                ImageContainer->ImageIndexTable[GoingImageID].LengthTotalData+=ImageContainer->ImageIndexTable[GoingImageID].AlphaPixelNum*8 + 12;
            }

        }

        if(PaletteImage)
        {
            //带索引表的图片需要先把索引转为颜色
            if( iTemp==0 )
            {
//                ZlibImageContainer->ImageIndexTable[GoingImageID].LengthIamgeData = 0;
                i51AdeStdMemcpy16(DataSrcCurrentPart, DataDesCurrentPart, I51KITG2_OFFSET_ALPHANUMER);
                DataSrcCurrentPart[I51KITG2_OFFSET_ALPHANUMER] = DataDesCurrentPart[I51KITG2_OFFSET_ALPHANUMER+1];
                DataSrcCurrentPart[I51KITG2_OFFSET_ALPHANUMER+1] = DataDesCurrentPart[I51KITG2_OFFSET_ALPHANUMER];
                DesTempPos = I51KITG2_OFFSET_PIXELDATA + PaletteColorNum*2 +2;
                SrcTempPos = I51KITG2_OFFSET_PIXELDATA;
                BlockLength = 0;
                BlockPlace = 0;
            }
            else
            {
                DesTempPos = 0;
                SrcTempPos = 0;
            }

            while(DesTempPos<LengthPartDes)
            {
                if( BlockPlace==0 )
                {
                    // 块重新开始
                    if( (LengthPartDes-DesTempPos)>=6 )
                    {
                        // 段剩余数据长度大于等于块头长度
                        BlockPlace = 6;
                        BlockLength = DataDesCurrentPart[DesTempPos+4] | (DataDesCurrentPart[DesTempPos+5]<<8);
                        i51AdeStdMemcpy16(&DataSrcCurrentPart[SrcTempPos], &DataDesCurrentPart[DesTempPos], 6);
                        i51AdeStdMemcpy16(TempHead, &DataDesCurrentPart[DesTempPos], 6);
                        DesTempPos += 6;
                        SrcTempPos += 6;
                    }
                    else
                    {
                        // 段剩余数据小于块头长度，把剩余数据保存
                        BlockPlace = LengthPartDes-DesTempPos;
                        for( Temp = 0; Temp < BlockPlace; Temp++ )
                        {
                            TempHead[Temp] = DataDesCurrentPart[DesTempPos+Temp];
                        }
                        break; 						
                    }
                }
                else if( BlockPlace<6 )				
                {
                    for( Temp = BlockPlace; Temp < 6; Temp++ )
                    {
                        TempHead[Temp] = DataDesCurrentPart[DesTempPos+Temp-BlockPlace];
                    }
                    i51AdeStdMemcpy16(&DataSrcCurrentPart[SrcTempPos], TempHead, 6);
                    BlockLength = TempHead[4] | (TempHead[5]<<8);
                    DesTempPos += 6-BlockPlace;
                    SrcTempPos += 6;
                    BlockPlace = 6;
                }
                else
                {
                    BlockLength = TempHead[4] | (TempHead[5]<<8);
                    BlockLength -= BlockPlace - 6;
                }

                ColorPoint = (iCOLOR*)(((iU32)DataSrcCurrentPart)+SrcTempPos);

                if( DesTempPos+BlockLength > LengthPartDes)
                {
                    BlockLength = LengthPartDes - DesTempPos;
                    BlockPlace = 6 + BlockLength;
                }
                else				
                {
                    BlockPlace = 0;
                }
				
                for( Temp=0; Temp<BlockLength; Temp++ )
                {
                    ColorPoint[Temp] = PaletteColorMem[DataDesCurrentPart[DesTempPos]];
                    DesTempPos++;
                    SrcTempPos += 2;
                }

            }

            if(i51AdeCnfGetEndian()==1)
            {
                // 大端的机器需要把颜色值和坐标值的高地位交换
                if( iTemp==0 )
                {
                	Temp = I51KITG2_OFFSET_PIXELDATA;
                }
                else
                {
                	Temp = 0;
                }
				
                while(Temp<SrcTempPos)
                {
                    DataSrcCurrentPart[Temp] += DataSrcCurrentPart[Temp+1];
                    DataSrcCurrentPart[Temp+1] = DataSrcCurrentPart[Temp] - DataSrcCurrentPart[Temp+1];
                    DataSrcCurrentPart[Temp] = DataSrcCurrentPart[Temp] - DataSrcCurrentPart[Temp+1];
                    Temp += 2;
                }
            }
            FlagReturn = i51AdeOsFileWrite( DesFile, DataSrcCurrentPart, SrcTempPos, &LengFactWriteData );
            if( iFALSE == FlagReturn ) goto ERROR;
			
        }
        else
        {
            if(i51AdeCnfGetEndian()==1)
            {
                // 大端的机器需要把颜色值和坐标值的高地位交换
                while(DesTempPos<LengthPartDes)
                {
                    DataDesCurrentPart[DesTempPos] += DataDesCurrentPart[DesTempPos+1];
                    DataDesCurrentPart[DesTempPos+1] = DataDesCurrentPart[DesTempPos] - DataDesCurrentPart[DesTempPos+1];
                    DataDesCurrentPart[DesTempPos] = DataDesCurrentPart[DesTempPos] - DataDesCurrentPart[DesTempPos+1];
                    DesTempPos += 2;
                }
            }
            FlagReturn = i51AdeOsFileWrite( DesFile, DataDesCurrentPart, LengthPartDes, &LengFactWriteData );
            if( iFALSE == FlagReturn ) goto ERROR;
        }
    }

    ImageContainer->ImageIndexTable[GoingImageID].FlagImageExist = 1;
    ImageContainer->ImageIndexTable[GoingImageID].FlagCopyRAM = iFALSE;
    ImageContainer->ImageIndexTable[GoingImageID].ImageEntry = SeekFileContainer;

    FlagReturn = i51AdeOsFileSetSeek( DesFile , ( I51KITG2_OFFSET_START_DATA + ImageID*5 ), i51_OS_FL_SEEK_HEAD ) ;
    if( iFALSE == FlagReturn )
    {
        goto ERROR;
    }

    FlagImageExist = 1;
    FlagReturn = i51AdeOsFileWriteInt8( DesFile, &FlagImageExist);
    if( iFALSE == FlagReturn )
    {
        goto ERROR;
    }

    FlagReturn = i51AdeOsFileWriteInt32( DesFile, &SeekFileContainer );
    if( iFALSE == FlagReturn )
    {
        goto ERROR;
    }
		
    FlagReturn = i51AdeOsFileSetSeek( DesFile , 0, i51_OS_FL_SEEK_TAIL) ;
    if( iFALSE == FlagReturn )
    {
        goto ERROR;
    }

    i51AdeOsFileFlush( DesFile );

    if(PaletteColorMem)
    {
        i51AdeOsFree(PaletteColorMem);
        PaletteColorMem = iNULL;
    }
    i51AdeOsFree(DataDesCurrentPart);
    DataDesCurrentPart = iNULL;
    i51AdeOsFree(DataSrcCurrentPart);
    DataSrcCurrentPart = iNULL;

    return iTRUE;

    ERROR:
        iLog("KIT - G2 : decode one : %d",GoingImageID);
        if(PaletteColorMem)
        {
            i51AdeOsFree(PaletteColorMem);
            PaletteColorMem = iNULL;
        }
        i51AdeOsFree(DataDesCurrentPart);
        DataDesCurrentPart = iNULL;
        i51AdeOsFree(DataSrcCurrentPart);
        DataSrcCurrentPart = iNULL;
        return iFALSE;

}

static iBOOL ZlibDecodeOneImageFromMemory( iHANDLE SrcData, iFILE DesFile, iU32 DesSeek, iHANDLE Container, iU32 ImageID )
{
    // author : Otto
    // since : Aug 31 2011
    // (C) 2011 , PKIG Tech. Co., Ltd.

    // iHANDLE SrcData : 需要解码的数据在内存中的地址
    // iFILE DesFile : 解码后的数据存放文件指针
    // iU32 DesFile : 解码后的数据存放目的文件中的开始地址
    // iHANDLE Container : 容器句柄
    // iU32 ImageID : 当前解码的图片在容器文件中的图片ID
    
    // notes : 从文件SrcFile中解压一张图片到文件DesFile中
    // SrcFile文件中存放的图片数据是分段用Zlib压缩的
    // 具体文件格式请参考文档"The_i51_DC_dynamic_library_G2_i51_Image_Format.xls"

    // 从文件中读取的CRC
//    iU32 CRC_Read = 0;
    // 目标数据长度，即解压后文件长度
    iU32 LengthDesData = 0;
    // 原始数据长度，即压缩后文件长度
    iU32 LengthSrcData = 0;
    // 解压后段最长长度,默认设置为32K
//    iU32 LengthPartMax = 0;
    // 段数量
    iU16 NumberParts = 0;
    // 当前段解压前数据长度
    iU32 LengthPartSrc = 0;
    // 当前段解压后数据长度
    iU32 LengthPartDes = 0;
    // 函数返回值
    iBOOL FlagReturn = iFALSE;
    // 解码前图片数据开始位置 在资源文件中的偏移量
//    iU32 SeekFileResource = 0;
    // 解码后图片数据开始位置 在容器文件中的偏移量
    iU32 SeekFileContainer = 0;
    // 文件当前偏移量
//    iU32 FileSeekCureent = 0;
//    iU32 LengFactReadData = 0;
    // 实际一次写入文件中的数据长度  
    iU32 LengFactWriteData = 0;
    iU32 iTemp = 0;
    iU32 GoingImageID = ImageID;	
    iU8 FlagImageExist = 1;

    // 解压后段数据buffer
    iU8  * DataDesCurrentPart = iNULL;
    // 解压后八位图数据转为16位buff
    iU8  * DataDesTempPart = iNULL;
    iU32 DesTempPos = 0;	
//    iU32 SrcTempPos = 0;	

    // 图片带调色板
    iU8 PaletteImage = iFALSE;
    // 调色板中颜色总数
    iU16 PaletteColorNum = 0;
    // 调色板颜色索引表
    iCOLOR * PaletteColorMem = iNULL;
    // 一个数据块的长度	:=6+连续点个数
    iS32 BlockLength = 0;
    // 当前数据块位置
    iS32 BlockPlace = 0;
    // 颜色临时变量	
//    iCOLOR TempColor = 0;
    // 颜色指针地址
    iCOLOR * ColorPoint = iNULL;
    // 段数据头	
    iU8 TempHead[6];
    iS32 Temp = 0;	
    iU32 SrcPos = 0;
    iU32 DesPos = 0;	
    iU8 * SrcDataPoint = iNULL;

    i51ImageContainer * ImageContainer = (i51ImageContainer *)Container;
    if( iNULL ==ImageContainer )
    {
        iLog("KIT - G2 : decode F mem Container");
        goto ERROR;		
    }

    if( iNULL ==SrcData)
    {
        iLog("KIT - G2 : decode F mem SrcData");
        goto ERROR;		
    }

    SrcDataPoint = (iU8*)SrcData;
	
    // 移动文件内指针到容器文件DesSeek位置
    SeekFileContainer = DesSeek;
    FlagReturn = i51AdeOsFileSetSeek( DesFile, DesSeek, i51_OS_FL_SEEK_HEAD) ;
    if( iFALSE == FlagReturn )
    {
		goto ERROR;
    }

    LengthDesData = SrcDataPoint[4]|(SrcDataPoint[5]<<8)|(SrcDataPoint[6]<<16)|(SrcDataPoint[7]<<24);
    LengthSrcData = SrcDataPoint[8]|(SrcDataPoint[9]<<8)|(SrcDataPoint[10]<<16)|(SrcDataPoint[11]<<24);
    NumberParts = SrcDataPoint[16]|(SrcDataPoint[17]<<8);

    if( NumberParts==1 && (LengthDesData+514)<=I51KITG2_LENGTH_PART_UMCOMPRESS )
    {
        // LengthDesData是解码出来的文件长度，如果i51G是8位图，则LengthDesData是解码出来的16位图的长度，也就是去除了调色板信息，下标已经替换成了颜色值
        // 调色板最大长度为2+256*2=514个字节
        DataDesCurrentPart = i51AdeOsMalloc( LengthDesData+514, __FILE__ , __LINE__ );
    }
    else
    {
        DataDesCurrentPart = i51AdeOsMalloc(I51KITG2_LENGTH_PART_UMCOMPRESS, __FILE__ , __LINE__ );
    }
    if( iFALSE == DataDesCurrentPart  ) 
    {
        iLog("KIT - G2 : malloc src decode one ");
        goto ERROR;
    }

    SrcPos = 18;
    DesPos = 0;
    for( iTemp=0; iTemp<NumberParts; iTemp++ )
    {
        DesTempPos = 0;
		
        LengthPartSrc = SrcDataPoint[SrcPos]|(SrcDataPoint[SrcPos+1]<<8)|(SrcDataPoint[SrcPos+2]<<16)|(SrcDataPoint[SrcPos+3]<<24);
        SrcPos += 4;

        if( iNULL==DataDesTempPart)
        {
            if( NumberParts==1 && (LengthDesData+514)<=I51KITG2_LENGTH_PART_UMCOMPRESS*2 )
            {
                // LengthDesData是解码出来的文件长度，如果i51G是8位图，则LengthDesData是解码出来的16位图的长度，也就是去除了调色板信息，下标已经替换成了颜色值
                // 调色板最大长度为2+256*2=514个字节
                if( LengthPartSrc>LengthDesData)
                {
                    DataDesTempPart = i51AdeOsMalloc(LengthPartSrc+514, __FILE__ , __LINE__ );
                }
                else
                {
                    DataDesTempPart = i51AdeOsMalloc(LengthDesData+514, __FILE__ , __LINE__ );
                }
            }
            else
            {
                DataDesTempPart = i51AdeOsMalloc(I51KITG2_LENGTH_PART_UMCOMPRESS*2, __FILE__ , __LINE__ );
            }
            if( iNULL == DataDesTempPart )
            {
                iLog("KIT - G2 : malloc des decode one");
                goto ERROR;
            }
        }
			
        LengthPartDes = 32768;

        FlagReturn = i51AdeStdUncompress( DataDesCurrentPart, &LengthPartDes, &SrcDataPoint[SrcPos], LengthPartSrc);
        SrcPos += LengthPartSrc;	
        if( iFALSE == FlagReturn ) goto ERROR;

        if( iTemp==0 )
        {
            DesTempPos = I51KITG2_OFFSET_PIXELDATA;
            if(DataDesCurrentPart[0]!='i'||DataDesCurrentPart[1]!='5'||DataDesCurrentPart[2]!='1'||DataDesCurrentPart[3]!='G')
            {
                iLog("KIT - G2 : file form");
                goto ERROR;
            }

            if( DataDesCurrentPart[4]>0X40)
            {
                PaletteImage = iTRUE;
                PaletteColorNum = (DataDesCurrentPart[I51KITG2_OFFSET_PIXELDATA]) | (DataDesCurrentPart[I51KITG2_OFFSET_PIXELDATA+1]<<8); 
                PaletteColorMem = i51AdeOsMalloc(PaletteColorNum*2, __FILE__ , __LINE__ );
                if ( iNULL==PaletteColorMem )
                {
                    iLog("KIT - G2 : malloc PaletteColor" );
                    goto ERROR;
                }
                i51AdeStdMemcpy16(PaletteColorMem, &(DataDesCurrentPart[I51KITG2_OFFSET_PIXELDATA+2]), PaletteColorNum*2 );
				
            }

            ImageContainer->ImageIndexTable[GoingImageID].DecodeType = DataDesCurrentPart[I51KITG2_OFFSET_DECODETYPE];
            if(ImageContainer->ImageIndexTable[GoingImageID].DecodeType!=I51KITG2_DECODE_LINEAR)
            {
                iLog("KIT - G2 : 解码图片格式");
                goto ERROR;				
            }
            ImageContainer->ImageIndexTable[GoingImageID].ImageHeight= (DataDesCurrentPart[I51KITG2_OFFSET_IMAGEHEIGHT])|(DataDesCurrentPart[I51KITG2_OFFSET_IMAGEHEIGHT+1]<<8);
            ImageContainer->ImageIndexTable[GoingImageID].ImageWidth= (DataDesCurrentPart[I51KITG2_OFFSET_IMAGEWIDTH])|(DataDesCurrentPart[I51KITG2_OFFSET_IMAGEWIDTH+1]<<8);
            ImageContainer->ImageIndexTable[GoingImageID].AlphaPixelNum= (DataDesCurrentPart[I51KITG2_OFFSET_ALPHANUMER])|(DataDesCurrentPart[I51KITG2_OFFSET_ALPHANUMER+1]<<8);
            ImageContainer->ImageIndexTable[GoingImageID].LengthIamgeData =
   		 	(DataDesCurrentPart[I51KITG2_OFFSET_DATALENGTH])|(DataDesCurrentPart[I51KITG2_OFFSET_DATALENGTH+1]<<8)
   		     |(DataDesCurrentPart[I51KITG2_OFFSET_DATALENGTH+2]<<16)|(DataDesCurrentPart[I51KITG2_OFFSET_DATALENGTH+3]<<24);
            ImageContainer->ImageIndexTable[GoingImageID].LengthTotalData = ImageContainer->ImageIndexTable[GoingImageID].LengthIamgeData ;
            if( ImageContainer->ImageIndexTable[GoingImageID].AlphaPixelNum>0)
            {
                ImageContainer->ImageIndexTable[GoingImageID].LengthTotalData+=ImageContainer->ImageIndexTable[GoingImageID].AlphaPixelNum*8 + 12;
            }

        }

        if(PaletteImage)
        {
            //带索引表的图片需要先把索引转为颜色
            if( iTemp==0 )
            {
//                ZlibImageContainer->ImageIndexTable[GoingImageID].LengthIamgeData = 0;
                i51AdeStdMemcpy16(DataDesTempPart, DataDesCurrentPart, I51KITG2_OFFSET_PIXELDATA);
                DesPos = I51KITG2_OFFSET_PIXELDATA + PaletteColorNum*2 +2;
                DesTempPos = I51KITG2_OFFSET_PIXELDATA;
                BlockLength = 0;
                BlockPlace = 0;
            }
            else
            {
                DesPos = 0;
                DesTempPos = 0;
            }

            while(DesPos<LengthPartDes)
            {
                if( BlockPlace==0 )
                {
                    // 块重新开始
                    if( (LengthPartDes-DesPos)>=6 )
                    {
                        // 段剩余数据长度大于等于块头长度
                        BlockPlace = 6;
                        BlockLength = DataDesCurrentPart[DesPos+4] | (DataDesCurrentPart[DesPos+5]<<8);
                        i51AdeStdMemcpy16(&DataDesTempPart[DesTempPos], &DataDesCurrentPart[DesPos], 6);
                        i51AdeStdMemcpy16(TempHead, &DataDesCurrentPart[DesPos], 6);
                        DesTempPos += 6;
                        DesPos += 6;
                    }
                    else
                    {
                        // 段剩余数据小于块头长度，把剩余数据保存
                        BlockPlace = LengthPartDes-DesPos;
                        for( Temp = 0; Temp < BlockPlace; Temp++ )
                        {
                            TempHead[Temp] = DataDesCurrentPart[DesPos+Temp];
                        }
                        break; 						
                    }
                }
                else if( BlockPlace<6 )				
                {
                    for( Temp = BlockPlace; Temp < 6; Temp++ )
                    {
                        TempHead[Temp] = DataDesCurrentPart[DesPos+Temp-BlockPlace];
                    }
                    i51AdeStdMemcpy16(&DataDesTempPart[DesTempPos], TempHead, 6);
                    BlockLength = TempHead[4] | (TempHead[5]<<8);
                    DesPos += 6-BlockPlace;
                    DesTempPos += 6;
                    BlockPlace = 6;
                }
                else
                {
                    BlockLength = TempHead[4] | (TempHead[5]<<8);
                    BlockLength -= BlockPlace - 6;
                }

                ColorPoint = (iCOLOR*)(((iU32)DataDesTempPart)+DesTempPos);

                if( DesPos+BlockLength > LengthPartDes)
                {
                    BlockLength = LengthPartDes - DesPos;
                    BlockPlace = 6 + BlockLength;
                }
                else				
                {
                    BlockPlace = 0;
                }
				
                for( Temp=0; Temp<BlockLength; Temp++ )
                {
                    ColorPoint[Temp] = PaletteColorMem[DataDesCurrentPart[DesPos]];
                    DesPos++;
                    DesTempPos += 2;
                }

            }

            if(i51AdeCnfGetEndian()==1)
            {
                // 大端的机器需要把颜色值和坐标值的高地位交换
                if( iTemp==0 )
                {
                	Temp = I51KITG2_OFFSET_PIXELDATA;
                }
                else
                {
                	Temp = 0;
                }
				
                while(Temp<DesTempPos)
                {
                    DataDesTempPart[Temp] += DataDesTempPart[Temp+1];
                    DataDesTempPart[Temp+1] = DataDesTempPart[Temp] - DataDesTempPart[Temp+1];
                    DataDesTempPart[Temp] = DataDesTempPart[Temp] - DataDesTempPart[Temp+1];
                    Temp += 2;
                }
            }
            FlagReturn = i51AdeOsFileWrite( DesFile, DataDesTempPart, DesTempPos, &LengFactWriteData );
            if( iFALSE == FlagReturn ) goto ERROR;
			
        }
        else
        {
            if(i51AdeCnfGetEndian()==1)
            {
                // 大端的机器需要把颜色值和坐标值的高地位交换
                while(DesTempPos<LengthPartDes)
                {
                    DataDesCurrentPart[DesTempPos] += DataDesCurrentPart[DesTempPos+1];
                    DataDesCurrentPart[DesTempPos+1] = DataDesCurrentPart[DesTempPos] - DataDesCurrentPart[DesTempPos+1];
                    DataDesCurrentPart[DesTempPos] = DataDesCurrentPart[DesTempPos] - DataDesCurrentPart[DesTempPos+1];
                    DesTempPos += 2;
                }
            }
            FlagReturn = i51AdeOsFileWrite( DesFile, DataDesCurrentPart, LengthPartDes, &LengFactWriteData );
            if( iFALSE == FlagReturn ) goto ERROR;
        }
    }

    ImageContainer->ImageIndexTable[GoingImageID].FlagImageExist = 1;
    ImageContainer->ImageIndexTable[GoingImageID].FlagCopyRAM = iFALSE;
    ImageContainer->ImageIndexTable[GoingImageID].ImageEntry = SeekFileContainer;

    FlagReturn = i51AdeOsFileSetSeek( DesFile , ( I51KITG2_OFFSET_START_DATA + ImageID*5 ), i51_OS_FL_SEEK_HEAD ) ;
    if( iFALSE == FlagReturn )
    {
        goto ERROR;
    }

    FlagImageExist = 1;
    FlagReturn = i51AdeOsFileWriteInt8( DesFile, &FlagImageExist);
    if( iFALSE == FlagReturn )
    {
        goto ERROR;
    }

    FlagReturn = i51AdeOsFileWriteInt32( DesFile, &SeekFileContainer );
    if( iFALSE == FlagReturn )
    {
        goto ERROR;
    }
		
    FlagReturn = i51AdeOsFileSetSeek( DesFile , 0, i51_OS_FL_SEEK_TAIL) ;
    if( iFALSE == FlagReturn )
    {
        goto ERROR;
    }

    i51AdeOsFileFlush( DesFile );

    if(PaletteColorMem)
    {
        i51AdeOsFree(PaletteColorMem);
        PaletteColorMem = iNULL;
    }
    i51AdeOsFree(DataDesCurrentPart);
    DataDesCurrentPart = iNULL;
    i51AdeOsFree(DataDesTempPart);
    DataDesTempPart = iNULL;

    return iTRUE;

    ERROR:
        iLog("KIT - G2 : decode one : %d",GoingImageID);
        if(PaletteColorMem)
        {
            i51AdeOsFree(PaletteColorMem);
            PaletteColorMem = iNULL;
        }
        i51AdeOsFree(DataDesCurrentPart);
        DataDesCurrentPart = iNULL;
        i51AdeOsFree(DataDesTempPart);
        DataDesTempPart = iNULL;
        return iFALSE;

}

static void  ZlibDecodeAllImage( iU32 ID)
{
    // author : Otto
    // since : Sep 8 2011
    // (C) 2011 , PKIG Tech. Co., Ltd.

    // iFILE file : i51文件指针
    // iU32 TotallImage : i51文件数量
    //  iU16* path ：容器文件所在路径指针

    // notes : 生成一个图片容器文件,并返回他的指针

    // 容器文件格式如下：
    // 1.图片总数 四个字节
    // 2.内存有拷贝的图片数量 四个字节
    // 3.图片是否需要拷贝到内存的标记 一个字节
    // 4.图片数据在文件中的偏移量 四个字节
    // 5.图片宽度
    // 6.图片高度
    // 7.编码方式
    // 8.透明色
    // 9.图片RGB数据
    // 3,4的数据需要TotallImage份
    // 5,6,7,8,9的数据需要TotallImage份

    iU32 * ImageId ;
    iS32 iTemp = 0;	

    iFILE SrcFile = iNULL;
    iFILE DesFile = iNULL;
    iHANDLE Container = iNULL;
    iU32 DesSeek = 0;
    iU32 SrcSeek = 0;
    iU32 ImageID = 0;
    iU8 FlagReturn = iFALSE;

    // 解码第一张图片时需要创建容器并初始化变量
    if( ManageZlibDecodeImage.CompleteImage == 0 )
    {
        if( iFALSE == ZlibDecodeInit() )
        {
            iLog("KIT - G2 : ZlibDecodeInit");
            goto ERROR;
        }
    }

    {
        SrcFile = ManageZlibDecodeImage.SrcFile;
        DesFile = ManageZlibDecodeImage.DesFilePointer;
        Container = (iHANDLE)(ZlibImageContainer);
        // 移动文件内指针到容器文件末尾
        FlagReturn = i51AdeOsFileSetSeek( DesFile, 0, i51_OS_FL_SEEK_TAIL) ;
        if( iFALSE == FlagReturn )
        {
    		goto ERROR;
        }
    	
        // 获得当前容器文件内指针位置
        FlagReturn = i51AdeOsFileGetSeek( DesFile , (iS32 *)(&DesSeek), i51_OS_FL_SEEK_HEAD ) ;
        if( iFALSE == FlagReturn )
        {
    		goto ERROR;
        }

        // 获得指定ID号的资源在资源文件中偏移量	
        ImageID = ManageZlibDecodeImage.GoingImageID;
        SrcSeek =  i51KitStdResGetContentEntry (  ManageZlibDecodeImage.SrcFile, ImageID) ;
        if( iFALSE == FlagReturn )
        {
            iLog("KIT - G2 :offset decode one");
            goto ERROR;
        }

        if( iFALSE == ZlibDecodeOneImageFromFile( SrcFile , SrcSeek, DesFile, DesSeek, Container, ImageID ) )
        {
            goto ERROR;
        }
    }

    ManageZlibDecodeImage.CompleteImage++;

    if( ManageZlibDecodeImage.CompleteImage < ManageZlibDecodeImage.TotallImage )	
    {
        ImageId = (iU32*)ManageZlibDecodeImage.ImageId;
        ManageZlibDecodeImage.GoingImageID = ImageId[ManageZlibDecodeImage.CompleteImage];

        ManageZlibDecodeImage.Process(   ManageZlibDecodeImage.CompleteImage, ManageZlibDecodeImage.TotallImage, ZlibImageContainer);

        return ;

    }

    i51AdeOsTimerStop( TimeIdZlibDecodeImage );         
    TimeIdZlibDecodeImage = iNULL;
    FlagDecodeImage = iFALSE;

//    i51AdeOsFileClose(ManageZlibDecodeImage.DesFilePointer);

    i51AdeOsFree((void *)ManageZlibDecodeImage.ImageId);
    ManageZlibDecodeImage.ImageId = iNULL;
	
//    ManageZlibDecodeImage.DesFilePointer = iNULL;

    i51KitG2WriteFileCRC(ManageZlibDecodeImage.DesFilePointer);

    i51AdeOsFileFlush( ManageZlibDecodeImage.DesFilePointer);
	
     ManageZlibDecodeImage.Process(   ManageZlibDecodeImage.CompleteImage, ManageZlibDecodeImage.TotallImage, ZlibImageContainer);
    ManageZlibDecodeImage.Done( ZlibImageContainer );

    return ;
	
    ERROR:

    iTemp = 0;	 
    while(1)
    {
        if(ZlibImageContainer &&  ContainerManage[iTemp].Flag  && ContainerManage[iTemp].ContainerHandle==ZlibImageContainer )
        {
    	     ContainerManage[iTemp].Flag = iFALSE;
            break;
        }
        iTemp++;
        if( iTemp>=MAX_ContainerManage_NUM )		
        {
            iLog("KIT - G2 : decode all zlib");
		break;
        }
    }		
	
        i51AdeOsFileClose( ManageZlibDecodeImage.DesFilePointer );
        i51AdeOsFree((void *)ManageZlibDecodeImage.ImageId);
        ManageZlibDecodeImage.ImageId = iNULL;
        if( ZlibImageContainer )
        {
	        i51AdeOsFree((void *)ZlibImageContainer->ImageIndexTable);
	        ZlibImageContainer->ImageIndexTable = iNULL;
	        i51AdeOsFree((void *)ZlibImageContainer);
	        ZlibImageContainer = iNULL;
        }

        if( i51AdeOsFileIsExist(ManageZlibDecodeImage.DesFileName) )
        {
            i51AdeOsFileDelete(ManageZlibDecodeImage.DesFileName);
        }
        i51AdeOsTimerStop( TimeIdZlibDecodeImage );         
        TimeIdZlibDecodeImage = iNULL;
        FlagDecodeImage = iFALSE;
        ManageZlibDecodeImage.Done(iNULL);
        iLog("KIT - G2 :decode all");

        return ;		

}

static iBOOL i51KitG2GetActualRect(iRECT * ImageRect, iRECT * DesRect, iRECT *  SrcRect,  iRECT * DrawDesRect,  iRECT * DrawSrcRect, iU8 Direction )
{
 // 经过计算后，如果图像没有需要绘制的区域则返回iFALSE，否则返回iTURE
    // 图片绘制的区域
    iS16 SrcStartX = 0;
    iS16 SrcStartY = 0;
    iS16 SrcWidth = 0;
    iS16 SrcHeight = 0;

    // 原始图像的长宽	
    iS16 ImageWidth = 0;
    iS16 ImageHeight = 0;
	
     // 绘制到屏幕上的区域
    iS16 DesStartX = 0;
    iS16 DesStartY = 0;
    iS16 DesEndX = 0;
    iS16 DesEndY = 0;
    iS16 DesWidth = 0;
    iS16 DesHeight = 0;

    // 经过计算绘制到屏幕上的高宽
    iS16 DrawWidth = 0;
    iS16 DrawHeight = 0;	
	
    // VP的位置RECT
    iRECT * VPActualRect = iNULL;
    // VP需要绘制区域的RECT
    iRECT * VPDrawRect = iNULL;

    if(i51UseVirtualPanel)
    {
        VPActualRect = &(i51CurrentVpRender->VPActualRect);
        VPDrawRect = &(i51CurrentVpRender->VPDrawRect);
    }
	
    ImageWidth = ImageRect->Width;
    ImageHeight = ImageRect->Height;
	
    if( SrcRect)
    {
        SrcStartX = SrcRect->X;
        SrcStartY = SrcRect->Y;
        SrcWidth = SrcRect->Width;
        SrcHeight = SrcRect->Height;
    }
    else
    {
        SrcStartX = 0;
        SrcStartY = 0;
        SrcWidth = ImageRect->Width;
        SrcHeight = ImageRect->Height;
    }
	
    if( i51UseVirtualPanel == iFALSE )
    {
        if( DesRect)
        {
            DesStartX = DesRect->X;
            DesStartY = DesRect->Y;
            DesWidth = DesRect->Width;
            DesHeight = DesRect->Height;
        }
        else
        {
            DesStartX = 0;
            DesStartY = 0;
            DesWidth = ScreenWidth;
            DesHeight = ScreenHeight;
        }
    }
    else
    {
        if( DesRect)
        {
            DesStartX = DesRect->X + VPActualRect->X;
            DesStartY = DesRect->Y + VPActualRect->Y;
            DesWidth = DesRect->Width;
            DesHeight = DesRect->Height;
        }
        else
        {
            DesStartX = VPActualRect->X;
            DesStartY = VPActualRect->Y;
            DesWidth = VPActualRect->Width;
            DesHeight = VPActualRect->Height;
        }
		
        // 计算使用VP时绘制的屏幕区域
        DesEndX = DesStartX + DesWidth;
        DesEndY = DesStartY + DesHeight;
        if( DesStartX<VPDrawRect->X )
	{
		SrcWidth -= VPDrawRect->X - DesStartX ;
		SrcStartX += VPDrawRect->X - DesStartX ;
		DesStartX = VPDrawRect->X;
	}
        if( DesStartY<VPDrawRect->Y )
	{
		SrcHeight -= VPDrawRect->Y - DesStartY ; 
		SrcStartY += VPDrawRect->Y - DesStartY ; 
		DesStartY = VPDrawRect->Y;
	}
        if( DesEndX>(VPDrawRect->X+VPDrawRect->Width) ) DesEndX = VPDrawRect->X+VPDrawRect->Width;
        if( DesEndY>(VPDrawRect->Y+VPDrawRect->Height) ) DesEndY = VPDrawRect->Y+VPDrawRect->Height;
        DesWidth = DesEndX - DesStartX;
        DesHeight = DesEndY - DesStartY;
        if( DesWidth<=0 || DesHeight<=0 ) return iFALSE;
    }
	
    // 图片开始绘制的区域已经超出图片大小则直接返回
    if( SrcStartX>=ImageWidth || SrcStartY>=ImageHeight) return iFALSE;
	
    // 图片绘制区域和目的地区域高宽选择交集
    if( SrcWidth<DesWidth)	
        DrawWidth = SrcWidth;
    else
        DrawWidth = DesWidth;
	
    if( SrcHeight<DesHeight)	
        DrawHeight= SrcHeight;
    else
        DrawHeight = DesHeight;

    // 源图片裁剪位置开始在图片左边则偏移坐标
    if( SrcStartX < 0 )
    {
        DrawWidth += SrcStartX;
        DesStartX -= SrcStartX;
        SrcStartX = 0;			
    }
	
    // 源图片裁剪位置开始在图片上方则偏移坐标
    if( SrcStartY<0)
    {
        DrawHeight += SrcStartY;
        DesStartY -= SrcStartY;
        SrcStartY = 0;
    }

    // 绘制开始区域在屏幕左边
    if( DesStartX<0 )
    {
        DrawWidth += DesStartX;
        if( Direction!=I51KITG2_DRAW_IAMGE_MIRROR )
        {
            SrcStartX -= DesStartX;
        }
        DesStartX = 0;			
    }

    // 绘制开始区域在屏幕上方
    if( DesStartY<0 )
    {
        DrawHeight += DesStartY;
        SrcStartY -= DesStartY;
        DesStartY = 0;			
    }

    ImageWidth = ImageRect->Width;
    ImageHeight = ImageRect->Height;

    // 图片绘制区域超出图片大小则缩小绘制区域
    if( (SrcStartX+DrawWidth) > ImageWidth )
    {
        DrawWidth = ImageWidth - SrcStartX;
    }

    if( (SrcStartY+DrawHeight) > ImageHeight )
    {
        DrawHeight = ImageHeight - SrcStartY;
    }

    // 屏幕绘制区域超出屏幕大小则缩小绘制区域
    if( (DesStartX+DrawWidth) > ScreenWidth)
    {
        if( Direction==I51KITG2_DRAW_IAMGE_MIRROR )
        {
        	SrcStartX += DesStartX+DrawWidth- ScreenWidth;
        }
        DrawWidth = ScreenWidth - DesStartX;
    }

    if( (DesStartY+DrawHeight) > ScreenHeight)
    {
        DrawHeight = ScreenHeight - DesStartY;
    }

    if( (DrawHeight<=0) || ( DrawWidth<=0 ) ) return iFALSE;

    DrawSrcRect->X = SrcStartX;
    DrawSrcRect->Y = SrcStartY;
    DrawSrcRect->Width= DrawWidth;
    DrawSrcRect->Height= DrawHeight;

    DrawDesRect->X = DesStartX;
    DrawDesRect->Y = DesStartY;
    DrawDesRect->Width = DrawWidth;
    DrawDesRect->Height= DrawHeight;

    return iTRUE;

}
static iBOOL DrawRAMLinearImage  ( iHANDLE ContainerHandle, iU32 ImageID , iRECT* DesRect , iRECT* SrcRect , iU8 Direction)
{
    // author : Otto
    // since : Aug 15 2011
    // (C) 2011 , PKIG Tech. Co., Ltd.

    // iU32 ImageID : 绘制图像的ID号
    // iRECT* DesRect : 绘制图像到屏幕上的区域
    // iRECT* SrcRect : 绘制图像的区域
    // iU8 Direction : 绘制图像的方向
  
    // notes : 将图像容器内ID号为ImageID的图片SrcRect区域
    // 按照Direction的方向绘制到屏幕的DesRect区域
    // 图片的数据在内存中有备份，需要的数据从内存中读取
    // 这张图片的编码方式为I51KITG2_DECODE_LINEAR 

    // 图片容器地址入口
    i51ImageContainer * ImageContainer = iNULL;
    // 图片索引表地址
    i51ImageIndex * ImageIndexTable = iNULL;
    // 图片宽度和高度
    iU32 ImageWidth = 0;
    iU32 ImageHeight = 0;
    // 容器文件指针地址
    iFILE File = iNULL;
    // 文件操作函数返回值
//    iU8   FlagReturn = iFALSE;
    // 读取文件的缓冲区
//    iU8   ReadDataBuff[6] = {0};
    iU16 * TempReadDataBuff = iNULL;
    // 图片数据文件开始地址
    iU32 DataEntry = 0;
    iU32 CopyDataEntry = 0;
    // 读取到的数据长度
//    iU32  LengthReadData = 0;
    // 累计读取到的数据长度
    iU32  LengthTotalReadData = 0;
    // 需要读取的数据长度
//    iU32  LengthNeedReadData = 0;
    // 一次需要拷贝到缓冲区的颜色数量
    iS32 LengthCopyColor = 0;
    // 图片文件总长度
    iU32 LengthIamgeData = 0;

    // 绘制倒立图片时图片高度起始位置
    iS16 FillStartY = 0;
    iS16 FillEndY = 0;

    // 当前数据段在屏幕上的开始绘制位置
    iS16 TempDrawX = 0;
    iS16 TempDrawY = 0;
    // 图片绘制的左上角和右下角坐标  
    iS16 SrcStartX = 0;
    iS16 SrcStartY = 0;
    iS16 SrcEndX = 0;
    iS16 SrcEndY = 0;
    // 绘制到屏幕上的左上角和右下角坐标  
    iS16 DrawStartX = 0;
    iS16 DrawStartY = 0;
    iS16 DrawEndX = 0;
    iS16 DrawEndY = 0;
    // 实际绘制的图片长度和宽度  
    iS16 DrawWidth = 0;
    iS16 DrawHeight = 0;
//    iU16 iTempColor = 0;
    iS32 iTemp = 0;

    // ImageX
    iS32 CurrentImageX = 0;
    iS32 CurrentImageY = 0;
    iS32 CurrentImageWidth = 0;


    iU32 LcdBufferPostion = 0;

    // 读到的一段数据首地址
    iU16 * TempReadDataBuff1 = iNULL;
    // 带Alpha信息的数据点个数
    iU16 TotalAlphaNumber = 0;
    // 从文件中读取的数据长度	
    iU32 LengthNeedReadData = 0;
    // 开闭空间读取文件
    iU8  * ImageDataEntry = iNULL;
    // 从容器中读到的文件总长度
    iU32 LengthTotalRead = 0;
    // 文件操作函数返回值
    iU8   FlagReturn = iFALSE;
    iRECT DrawDesRect , DrawSrcRect, ImageRect;

    if( iNULL== ContainerHandle) return iFALSE;
    ImageContainer = (i51ImageContainer *)ContainerHandle;
    ImageIndexTable = ( i51ImageIndex *  )(ImageContainer->ImageIndexTable);
    File = ImageContainer->FileHandle;
    if( iNULL==ImageIndexTable || iNULL==File ) return iFALSE;

    ImageWidth = ImageIndexTable[ImageID].ImageWidth;
    ImageHeight = ImageIndexTable[ImageID].ImageHeight;
    LengthIamgeData = ImageIndexTable[ImageID].LengthIamgeData;
    TotalAlphaNumber = ImageIndexTable[ImageID].AlphaPixelNum;

    ImageRect.Width = ImageWidth;
    ImageRect.Height = ImageHeight;
    if(iFALSE==i51KitG2GetActualRect(&ImageRect, DesRect, SrcRect, &DrawDesRect, &DrawSrcRect, Direction))
    {
        return iTRUE;
    }

    DrawWidth = DrawSrcRect.Width;
    DrawHeight= DrawSrcRect.Height;

    SrcStartX = DrawSrcRect.X;
    SrcStartY = DrawSrcRect.Y;
    SrcEndX = DrawSrcRect.X + DrawWidth;
    SrcEndY = DrawSrcRect.Y + DrawHeight;

    DrawStartX = DrawDesRect.X;
    DrawStartY = DrawDesRect.Y;
    DrawEndX = DrawDesRect.X + DrawWidth;
    DrawEndY = DrawDesRect.Y + DrawHeight;

    if( Direction&(I51KITG2_DRAW_IAMGE_HANDSTAND) )
    {
        FillStartY = ImageHeight - SrcStartY - DrawHeight;
        FillEndY = FillStartY + DrawHeight;
    }
    else
    {
        FillStartY = SrcStartY;
        FillEndY = FillStartY + DrawHeight;
    }

	if( 0)
//	if( Direction&(I51KITG2_DRAW_IAMGE_MIRROR)  )
    {
        SrcStartX = ImageWidth - SrcStartX - DrawWidth;
        SrcEndX = SrcStartX + DrawWidth;
    }

    if( iFALSE==ImageIndexTable[ImageID].FlagCopyRAM )
    {
        DataEntry = ImageIndexTable[ImageID].ImageEntry;
        FlagReturn = i51AdeOsFileSetSeek( File, (DataEntry), i51_OS_FL_SEEK_HEAD );
        if( iFALSE == FlagReturn ) goto ERROR;                      
    
        LengthNeedReadData =ImageIndexTable[ImageID].LengthTotalData;
    
        ImageDataEntry = i51AdeOsMalloc(LengthNeedReadData, __FILE__ , __LINE__ );
        if( iNULL == ImageDataEntry ) 
        {
            iLog("KIT - G2 : malloc linear normal");
            return iFALSE;
        }
    	
        i51AdeOsFileRead( File, ImageDataEntry, LengthNeedReadData, &LengthTotalRead);
        if( iFALSE == FlagReturn || LengthNeedReadData!=LengthTotalRead) 
        {
            i51AdeOsFree(ImageDataEntry);
            ImageDataEntry = iNULL;
            goto ERROR;                
        }
    }
    else
    {
        ImageDataEntry = (iU8*)ImageIndexTable[ImageID].MemoryEntry;
    }

    DataEntry = (iU32)ImageDataEntry;
    LengthTotalReadData = I51KITG2_OFFSET_PIXELDATA;
    DataEntry += I51KITG2_OFFSET_PIXELDATA;

    if(AlphaSys!=i51KITG2_ALPHA_NONEED) goto ALPHA_NEED;
    {
        while(1)
        {
            TempReadDataBuff1 = (iU16 *)(DataEntry);   
            CurrentImageX = TempReadDataBuff1[0];
            CurrentImageY = TempReadDataBuff1[1];
            CurrentImageWidth = TempReadDataBuff1[2];
            DataEntry += 6;
            CopyDataEntry = DataEntry;
            LengthCopyColor =  CurrentImageWidth;

            LengthTotalReadData += 6 + (CurrentImageWidth<<1);
            DataEntry += ( CurrentImageWidth<<1 );
			
            // 图片当前数据段X开始坐标在图片绘制区域右边或者Y坐标在图片绘制区域上方            
            if( (CurrentImageY < FillStartY) || (CurrentImageX > SrcEndX) || (CurrentImageY>=FillEndY) )
            {
                if( LengthTotalReadData> (LengthIamgeData-3) ) break;
                continue;
            }
            
            // 图片需要镜像显示，映射X坐标
            if( Direction&(I51KITG2_DRAW_IAMGE_MIRROR) )
            {
//				CurrentImageX = ImageWidth - CurrentImageX - CurrentImageWidth;
            }

            // 图片需要倒立显示，映射Y坐标
            if( Direction&(I51KITG2_DRAW_IAMGE_HANDSTAND) )
            {
                // 图像高度结束点坐标是图像高度-1
                CurrentImageY = ImageHeight -1 - CurrentImageY ;
            }

            // 当前数据段长度超出图片绘制区域右边
            if( (CurrentImageX+CurrentImageWidth) >SrcEndX )
            {
                LengthCopyColor -= ((CurrentImageX+CurrentImageWidth-SrcEndX));
            }
			
            // 图片当前数据段X开始坐标在图片绘制区域左边            
            if( CurrentImageX<SrcStartX )
            {
//                if( (iNULL)==(Direction&(I51KITG2_DRAW_IAMGE_MIRROR)) )
                {
                    CopyDataEntry += ((SrcStartX - CurrentImageX)<<1);
                }

                LengthCopyColor -= ((SrcStartX - CurrentImageX));
                CurrentImageX = SrcStartX;
                if( LengthCopyColor<=0 ) 
                {
                    if( LengthTotalReadData> (LengthIamgeData-3) ) break;
                    continue;
                }
            }

             // 计算当前数据段在屏幕上开始显示的位置
             TempDrawX = DrawStartX;
             TempDrawY = DrawStartY + CurrentImageY - SrcStartY;

            // 图片当前数据段X开始坐标在图片开始绘制绘制区域右边
            // 需要移动屏幕开始绘制区域
            if( CurrentImageX>SrcStartX )
            {
                TempDrawX += ((CurrentImageX-SrcStartX));
            }

            if( TempDrawX >DrawEndX )
            {
                if( LengthTotalReadData> (LengthIamgeData-3) ) break;
                continue;
            }

            if( TempDrawY > DrawEndY ) break;
			
            
            TempReadDataBuff = (iU16*)CopyDataEntry;
            if( Direction&(I51KITG2_DRAW_IAMGE_MIRROR) )
            {
                LcdBufferPostion = TempDrawY*ScreenWidth+DrawStartX+DrawWidth - CurrentImageX + SrcStartX-1;
//                LcdBufferPostion = TempDrawY*ScreenWidth+TempDrawX + LengthCopyColor-1;
//                 i51AdeStdMemcpy16((iU16 *)ReadDataBuff, (void *)(CopyDataEntry), (LengthCopyColor<<1));
                for( iTemp = 0; iTemp <LengthCopyColor; iTemp++ )
                {
#if LCDBUFF_DEBUG
i51KitG2CheckLcdbuffer(LcdBufferPostion,1);
#endif
                     LcdBuffer[LcdBufferPostion--] = TempReadDataBuff[iTemp];
                }
            }
            else
            {
#if LCDBUFF_DEBUG
i51KitG2CheckLcdbuffer(TempDrawY*ScreenWidth+TempDrawX,LengthCopyColor);
#endif
                i51AdeStdMemcpy16( &LcdBuffer[ TempDrawY*ScreenWidth+TempDrawX], TempReadDataBuff, (LengthCopyColor<<1));
            }
            if( LengthTotalReadData> (LengthIamgeData-3) ) break;
	 }

        if(TotalAlphaNumber>0)
        {
            iS32 CurrAlphaNum = 0;
            iS32 CurrBuffPos = LengthIamgeData;		
            iS16 AlphaPosX = 0;
            iS16 AlphaPosY = 0;
            iALPHA AlphaNumber = 0;
            iS16 AlphaColor = 0;
            iS32 LcdBuffPos = 0;
            iU32 TempHeight = ImageHeight -1;
            iU32 TempWidth = ImageWidth-1;
			
            DataEntry = (iU32)ImageDataEntry+CurrBuffPos + 12;
			
            for( CurrAlphaNum=0; CurrAlphaNum<TotalAlphaNumber;CurrAlphaNum++)
            {
                TempReadDataBuff1 = (iU16 *)(DataEntry);   
                AlphaPosX = TempReadDataBuff1[0];
                AlphaPosY = TempReadDataBuff1[1];
                AlphaNumber = (iALPHA)TempReadDataBuff1[2];
                AlphaColor = TempReadDataBuff1[3];
                DataEntry += 8;
				
                // 图片需要镜像显示，映射X坐标
                if( Direction&(I51KITG2_DRAW_IAMGE_MIRROR) )
                {
                    AlphaPosX = TempWidth - AlphaPosX ;
                }
                if(AlphaPosY>=FillEndY) break;
                if( AlphaPosX>=SrcEndX || AlphaPosX<SrcStartX || AlphaPosY<FillStartY ) continue;				
    
                // 图片需要倒立显示，映射Y坐标
                if( Direction&(I51KITG2_DRAW_IAMGE_HANDSTAND) )
                {
                    // 图像高度结束点坐标是图像高度-1
                    AlphaPosY = TempHeight - AlphaPosY ;
                }
                LcdBuffPos = (AlphaPosY-SrcStartY+DrawStartY)*ScreenWidth+AlphaPosX-SrcStartX+DrawStartX;
                LcdBuffer[LcdBuffPos] = i51KitG2GetNewColorEx(LcdBuffer[LcdBuffPos], AlphaColor, AlphaNumber);
            }
        }
    }
     goto FUN_RETURN;
	 
ALPHA_NEED:
    {
        while(1)
        {
            TempReadDataBuff1 = (iU16 *)(DataEntry);   
            CurrentImageX = TempReadDataBuff1[0];
            CurrentImageY = TempReadDataBuff1[1];
            CurrentImageWidth = TempReadDataBuff1[2];
            DataEntry += 6;
            CopyDataEntry = DataEntry;
            LengthCopyColor =  CurrentImageWidth;

            LengthTotalReadData += 6 + (CurrentImageWidth<<1);
            DataEntry += ( CurrentImageWidth<<1 );
			
            // 图片当前数据段X开始坐标在图片绘制区域右边或者Y坐标在图片绘制区域上方            
            if( (CurrentImageY < FillStartY) || (CurrentImageX > SrcEndX) || (CurrentImageY>=FillEndY) )
            {
                if( LengthTotalReadData> (LengthIamgeData-3) ) break;
                continue;
            }
            
            // 图片需要镜像显示，映射X坐标
            if( Direction&(I51KITG2_DRAW_IAMGE_MIRROR) )
            {
                CurrentImageX = ImageWidth - CurrentImageX - CurrentImageWidth;
            }

            // 图片需要倒立显示，映射Y坐标
            if( Direction&(I51KITG2_DRAW_IAMGE_HANDSTAND) )
            {
                // 图像高度结束点坐标是图像高度-1
                CurrentImageY = ImageHeight -1 - CurrentImageY ;
            }

            // 当前数据段长度超出图片绘制区域右边
            if( (CurrentImageX+CurrentImageWidth) >SrcEndX )
            {
                LengthCopyColor -= ((CurrentImageX+CurrentImageWidth-SrcEndX));
            }
			
            // 图片当前数据段X开始坐标在图片绘制区域左边            
            if( CurrentImageX<SrcStartX )
            {
                if( (iNULL)==(Direction&(I51KITG2_DRAW_IAMGE_MIRROR)) )
                {
                    CopyDataEntry += ((SrcStartX - CurrentImageX)<<1);
                }

                LengthCopyColor -= ((SrcStartX - CurrentImageX));
                if( LengthCopyColor<=0 ) 
                {
                    if( LengthTotalReadData> (LengthIamgeData-3) ) break;
                    continue;
                }
            }

             // 计算当前数据段在屏幕上开始显示的位置
             TempDrawX = DrawStartX;
             TempDrawY = DrawStartY + CurrentImageY - SrcStartY;

            // 图片当前数据段X开始坐标在图片开始绘制绘制区域右边
            // 需要移动屏幕开始绘制区域
            if( CurrentImageX>SrcStartX )
            {
                TempDrawX += ((CurrentImageX-SrcStartX));
            }

            if( TempDrawX >DrawEndX )
            {
                if( LengthTotalReadData> (LengthIamgeData-3) ) break;
                continue;
            }

            if( TempDrawY > DrawEndY ) break;
			
            
            TempReadDataBuff = (iU16*)CopyDataEntry;
            if( Direction&(I51KITG2_DRAW_IAMGE_MIRROR) )
            {
                LcdBufferPostion = TempDrawY*ScreenWidth+TempDrawX + LengthCopyColor;
//                 i51AdeStdMemcpy16((iU16 *)ReadDataBuff, (void *)(CopyDataEntry), (LengthCopyColor<<1));
                for( iTemp = 0; iTemp <LengthCopyColor; iTemp++ )
                {
#if LCDBUFF_DEBUG
i51KitG2CheckLcdbuffer(LcdBufferPostion,1);
#endif
//                     LcdBuffer[LcdBufferPostion--] = TempReadDataBuff[iTemp];
                     LcdBuffer[ LcdBufferPostion] = i51KitG2GetNewColor(LcdBuffer[ LcdBufferPostion], TempReadDataBuff[iTemp]);
                     LcdBufferPostion--;
                }
            }
            else
            {
                LcdBufferPostion = TempDrawY*ScreenWidth+TempDrawX;
                for( iTemp = 0; iTemp <LengthCopyColor; iTemp++ )
                {
#if LCDBUFF_DEBUG
i51KitG2CheckLcdbuffer(LcdBufferPostion,1);
#endif
                    LcdBuffer[ LcdBufferPostion] = i51KitG2GetNewColor(LcdBuffer[ LcdBufferPostion], TempReadDataBuff[iTemp]);
                    LcdBufferPostion++;
                }
            }
            if( LengthTotalReadData> (LengthIamgeData-3) ) break;
	 }

        if(TotalAlphaNumber>0)
        {
            iS32 CurrAlphaNum = 0;
            iS32 CurrBuffPos = LengthIamgeData;		
            iS16 AlphaPosX = 0;
            iS16 AlphaPosY = 0;
            iALPHA AlphaNumber = 0;
            iS16 AlphaColor = 0;
            iS32 LcdBuffPos = 0;
            iU32 TempHeight = ImageHeight -1;
            iU32 TempWidth = ImageWidth-1;
			
            DataEntry = (iU32)ImageDataEntry+CurrBuffPos + 12;
			
            for( CurrAlphaNum=0; CurrAlphaNum<TotalAlphaNumber;CurrAlphaNum++)
            {
                TempReadDataBuff1 = (iU16 *)(DataEntry);   
                AlphaPosX = TempReadDataBuff1[0];
                AlphaPosY = TempReadDataBuff1[1];
                AlphaNumber = (iALPHA)TempReadDataBuff1[2];
                AlphaColor = TempReadDataBuff1[3];
                DataEntry += 8;
				
                // 图片需要镜像显示，映射X坐标
                if( Direction&(I51KITG2_DRAW_IAMGE_MIRROR) )
                {
                    AlphaPosX = TempWidth - AlphaPosX ;
                }
                if(AlphaPosY>=FillEndY) break;
                if( AlphaPosX>=SrcEndX || AlphaPosX<SrcStartX || AlphaPosY<FillStartY ) continue;				
    
                // 图片需要倒立显示，映射Y坐标
                if( Direction&(I51KITG2_DRAW_IAMGE_HANDSTAND) )
                {
                    // 图像高度结束点坐标是图像高度-1
                    AlphaPosY = TempHeight - AlphaPosY ;
                }
                LcdBuffPos = (AlphaPosY-SrcStartY+DrawStartY)*ScreenWidth+AlphaPosX-SrcStartX+DrawStartX;
                LcdBuffer[LcdBuffPos] = i51KitG2GetNewColorEx(LcdBuffer[LcdBuffPos], AlphaColor, AlphaNumber*AlphaSrc/255);
            }
        }    
    }

FUN_RETURN:	
    if( iFALSE==ImageIndexTable[ImageID].FlagCopyRAM )
    {
        i51AdeOsFree(ImageDataEntry);
        ImageDataEntry = iNULL;
    }
    return iTRUE;

ERROR:
    iLog("KIT - G2 : RW file linear");
    if( iFALSE==ImageIndexTable[ImageID].FlagCopyRAM )
    {
        i51AdeOsFree(ImageDataEntry);
        ImageDataEntry = iNULL;
    }
    return iFALSE;
		
}

static iBOOL i51KitG2RotatePoint(iS32 SrcX, iS32 SrcY, iS32 RotateCenterX,  iS32 RotateCenterY ,iS32 * DesX, iS32 * DesY, iS32 Angle)
{
    // A:原始点与原点之间相对X轴正方向的夹角
    // B:原始点和目的点与原点之间的夹角,即Angle
    // C:目的点与原点之间相对X轴正方向的夹角

    iS32 SinA = 0, SinB = 0, SinC = 0;
    iS32 CosA = 0, CosB = 0, CosC = 0;
    iS32 CenterX = RotateCenterX;
    iS32 CenterY = RotateCenterY;

    CosA = SrcX - CenterX;
    SinA = SrcY-CenterY;

    Angle = -Angle;
    SinB = i51KitG2Sin(Angle);
    CosB = i51KitG2Cos(Angle);

    SinC = SinA*CosB - CosA*SinB;
    CosC = CosA*CosB + SinA*SinB;

    *DesY = ((SinC+(CenterY<<14))>>14);
    *DesX = ((CosC+(CenterX<<14))>>14);

//    *DesX = (((SrcX-CenterX)*CosB+(SrcY-CenterY)*SinB)>>14) + CenterX;
//    *DesY = (((CenterX-SrcX)*SinB+(SrcY-CenterY)*CosB)>>14) + CenterY;
    return iTRUE;
	
}

static iBOOL i51KitG2GetGirthPoints(iRECT SrcRect, iRECT DesRect, iS16 PointS[][2], iS32 SrcMoveX, iS32 SrcMoveY,
	iS32 RotateCenterX,  iS32 RotateCenterY ,iU32 RotateType, iS32 Angle )
{
     // iRECT SrcRect : 需要变换的区域
     // iS16 PointS[][2] : 变换后周边的点新坐标
     // iS32 Angle :旋转角度
#define IMAGE_STRETCH 0
    // 旋转区域左上角坐标
    iS32 UpLeftX = 0, UpLeftY = 0;
    // 当前旋转点坐标	 
    iS32 SrcX= 0, SrcY = 0;
    iS32 SrcX1= 0, SrcY1 = 0;
    // 当前点旋转后坐标
    iS32 DesX = 0, DesY = 0;
    // 待旋转区域的长宽	
    iS32 SrcW, SrcH;
    iS32 Location = 0;
    iS32 EdgePoints[4][2] = {0};
    iS32 DesStartX = 0, DesStartY = 0;
    iS32 DesEndX = 0, DesEndY = 0;
#if IMAGE_STRETCH
    iS32 DesPoints[5][2] = {0};
    iS32 OldX = 0, OldY = 0;
    iS32 iTemp = 0, Min = 0, Num = 0 , Temp = 0;
#endif

//    UpLeftX = SrcRect.X + SrcMoveX;
//    UpLeftY = SrcRect.Y + SrcMoveY;
    UpLeftX = SrcMoveX;
    UpLeftY = SrcMoveY;
    SrcW = SrcRect.Width;
    SrcH = SrcRect.Height;

    DesStartX = DesRect.X;
    DesStartY = DesRect.Y;
    DesEndX = DesRect.Width + DesStartX;
    DesEndY = DesRect.Height + DesStartY;
	
    i51KitG2RotatePoint(UpLeftX, UpLeftY, RotateCenterX, RotateCenterY, &EdgePoints[0][0], &EdgePoints[0][1], Angle);
    i51KitG2RotatePoint(UpLeftX+SrcW, UpLeftY, RotateCenterX, RotateCenterY, &EdgePoints[1][0], &EdgePoints[1][1], Angle);
    i51KitG2RotatePoint(UpLeftX, UpLeftY+SrcH, RotateCenterX, RotateCenterY, &EdgePoints[2][0], &EdgePoints[2][1], Angle);
    i51KitG2RotatePoint(UpLeftX+SrcW, UpLeftY+SrcH, RotateCenterX, RotateCenterY, &EdgePoints[3][0], &EdgePoints[3][1], Angle);

    if( EdgePoints[0][0]<DesStartX&&EdgePoints[1][0]<DesStartX&&EdgePoints[2][0]<DesStartX&&EdgePoints[3][0]<DesStartX) return iFALSE;
    if( EdgePoints[0][1]<DesStartY&&EdgePoints[1][1]<DesStartY&&EdgePoints[2][1]<DesStartY&&EdgePoints[3][1]<DesStartY) return iFALSE;
    if( EdgePoints[0][0]>DesEndX&&EdgePoints[1][0]>DesEndX&&EdgePoints[2][0]>DesEndX&&EdgePoints[3][0]>DesEndX) return iFALSE;
    if( EdgePoints[0][1]>DesEndY&&EdgePoints[1][1]>DesEndY&&EdgePoints[2][1]>DesEndY&&EdgePoints[3][1]>DesEndY) return iFALSE;

    if( RotateType==i51KITG2_ROTATE_XLINE) goto XLINE;

    SrcX = UpLeftX;
    SrcY = UpLeftY;

//iLog("Y:");
    for ( Location = 0; Location <SrcW; Location++)
    {
        i51KitG2RotatePoint(SrcX, SrcY, RotateCenterX, RotateCenterY, &DesX, &DesY, Angle);
        PointS[Location][0] = DesX-SrcX;
        PointS[Location][1] = DesY-SrcY;
//iLog("%d\t%d",PointS[Location][0],PointS[Location][1]);
        SrcX++;
    }	

    if( RotateType==i51KITG2_ROTATE_YLINE) goto CHECK;

XLINE:
    SrcH = SrcW + SrcH;
    SrcX = UpLeftX;
    SrcY = UpLeftY;
    i51KitG2RotatePoint(SrcX, SrcY, RotateCenterX,  RotateCenterY , &DesX, &DesY, Angle);
    SrcX1 = DesX;
    SrcY1 = DesY;
#if IMAGE_STRETCH	
    OldX = DesX;
    OldY = DesY;
#endif
//iLog("X:")
    for ( Location = SrcW; Location <SrcH; Location++)
    {
        i51KitG2RotatePoint(SrcX, SrcY, RotateCenterX, RotateCenterY,&DesX, &DesY, Angle);
#if IMAGE_STRETCH		
DesPoints[0][0] = OldX;
DesPoints[0][1] = OldY;
DesPoints[1][0] = OldX-1;
DesPoints[1][1] = OldY;
DesPoints[2][0] = OldX;
DesPoints[2][1] = OldY-1;
DesPoints[3][0] = OldX+1;
DesPoints[3][1] = OldY;
DesPoints[4][0] = OldX;
DesPoints[4][1] = OldY+1;

Min = I51KITG2_ABS(DesPoints[0][0]-DesX) + I51KITG2_ABS(DesPoints[0][1]-DesY) ;
Num = 0;
for( iTemp=1; iTemp<5; iTemp++ )
{
    Temp = I51KITG2_ABS(DesPoints[iTemp][0]-DesX) + I51KITG2_ABS(DesPoints[iTemp][1]-DesY) ;
    if( Temp<Min)
    {
        Min = Temp;
        Num = iTemp;
    }
}
DesX = DesPoints[Num][0];
DesY = DesPoints[Num][1];
OldX = DesX;
OldY = DesY;
#endif
        PointS[Location][0] = DesX-SrcX1;
        PointS[Location][1] = DesY-SrcY1 ;
//iLog("%d\t%d",PointS[Location][0],PointS[Location][1]);
        SrcY++;
        SrcY1++;		
    }	

CHECK:
    for ( Location = 0; Location <SrcW; Location++)
    {
        PointS[Location][0] += SrcMoveX;
        PointS[Location][1] += SrcMoveY;
#if DEBUG_I51KITG2
//		 iLog("PointS[%d][0]=%d,PointS[%d][0]=%d",Location,PointS[Location][0],Location,PointS[Location][1]);
#endif
    }	

    return iTRUE;
	
}

static iBOOL RotateRamLinearImage  ( iHANDLE ContainerHandle,iU32 ImageID , iRECT* DesRect , iRECT* SrcRect , iS16 ChangeData[][2])
{
    // author : Otto
    // since : Aug 15 2011
    // (C) 2011 , PKIG Tech. Co., Ltd.

    // iU32 ImageID : 绘制图像的ID号
    // iRECT* DesRect : 绘制图像到屏幕上的区域
    // iRECT* SrcRect : 绘制图像的区域
    // iS32 Angle : 旋转角度
  
    // notes : 将图像容器内ID号为ImageID的图片SrcRect区域
    // 按照Angle的角度旋转后绘制到屏幕的DesRect区域
    // 图片的数据在内存中有备份，需要的数据从内存中读取
    // 这张图片的编码方式为I51KITG2_DECODE_LINEAR     

   // 图片容器地址入口
    i51ImageContainer * ImageContainer = iNULL;
    // 图片索引表地址
    i51ImageIndex * ImageIndexTable = iNULL;
    // 图片宽度和高度
    iU32 ImageWidth = 0;
    iU32 ImageHeight = 0;
    // 容器文件指针地址
    iFILE File = iNULL;
    // 文件操作函数返回值
//    iU8   FlagReturn = iFALSE;
    // 读取文件的缓冲区
    iU8  * ReadDataBuff = iNULL;
    // 从ReadDataBuff拷贝到显存的数据起始地址
    iU32 AddStartCopy = 0;
    // 图片数据文件开始地址
    iU32 DataEntry = 0;
    // 读取到的数据长度
//    iU32  LengthReadData = 0;
    // 累计读取到的数据长度
    iU32  LengthTotalReadData = 0;
    // 需要读取的数据长度
//    iU32  LengthNeedReadData = 0;
    // 一次需要拷贝到显存的数据长度  
    iS32 LengthCopyData = 0;
    // 图片文件总长度
    iU32 LengthIamgeData = 0;
//    iU32 LcdBufferPostion = 0;
  
//    iU16 iTempX = 0;
//    iU16 iTempY = 0;
    // 图片绘制的左上角和右下角坐标  
    iS16 SrcStartX = 0;
    iS16 SrcStartY = 0;
    iS16 SrcEndX = 0;
    iS16 SrcEndY = 0;
    // 绘制到屏幕上的左上角和右下角坐标  
    iS16 DrawStartX = 0;
    iS16 DrawStartY = 0;
    iS16 DrawEndX = 0;
    iS16 DrawEndY = 0;
    iU16 iTemp = 0;

    // ImageX
    iS32 CurrentImageX = 0;
    iS32 CurrentImageY = 0;
    iS32 CurrentImageWidth = 0;

    // 当前点的原始坐标	
    iS32 CurrX = 0, CurrY = 0;
    // 当前点选择后的坐标
    iS32 RotateX = 0, RotateY = 0;
    // 颜色buff
    iU16 * ColorBuff = iNULL;
    iS32 DrawW = 0;
    iS32 DrawH = 0;
     iU16* TempReadDataBuff1 = iNULL;
    iS32 AddsDirectionY = 0;
    iS32 BuffPos = 0;
    iCOLOR TempColor = 0;
    // 带Alpha信息的数据点个数
    iU16 TotalAlphaNumber = 0;
    // 从文件中读取的数据长度	
    iU32 LengthNeedReadData = 0;
    // 开闭空间读取文件
    iU8  * ImageDataEntry = iNULL;
    // 从容器中读到的文件总长度
    iU32 LengthTotalRead = 0;
    // 文件操作函数返回值
    iU8   FlagReturn = iFALSE;
	
    if( iNULL== ContainerHandle) return iFALSE;
    ImageContainer = ( i51ImageContainer * )ContainerHandle;

    ImageIndexTable = ( i51ImageIndex *  )(ImageContainer->ImageIndexTable);
    File = ImageContainer->FileHandle;
    if( iNULL==ImageIndexTable || iNULL==File ) return iFALSE;

    DataEntry = ImageIndexTable[ImageID].MemoryEntry;
    ImageWidth = ImageIndexTable[ImageID].ImageWidth;
    ImageHeight = ImageIndexTable[ImageID].ImageHeight;
    LengthIamgeData = ImageIndexTable[ImageID].LengthIamgeData;
    TotalAlphaNumber = ImageIndexTable[ImageID].AlphaPixelNum;

    DrawW = SrcRect->Width;
    DrawH = SrcRect->Height;
    SrcStartX = SrcRect->X;
    SrcStartY = SrcRect->Y;
    SrcEndX = SrcStartX +DrawW;
    SrcEndY = SrcStartY + DrawH;  
	
    DrawStartX = DesRect->X;
    DrawStartY = DesRect->Y;
    DrawEndX = DrawStartX + DesRect->Width - 1;
    DrawEndY = DrawStartY + DesRect->Height;
#if DEBUG_I51KITG2
//	 iLog("SrcStartX=%d,SrcStartY=%d,SrcEndX=%d,SrcEndY=%d",SrcStartX,SrcStartY,SrcEndX,SrcEndY);
#endif

    if( iFALSE==ImageIndexTable[ImageID].FlagCopyRAM )
    {
        DataEntry = ImageIndexTable[ImageID].ImageEntry;
        FlagReturn = i51AdeOsFileSetSeek( File, (DataEntry), i51_OS_FL_SEEK_HEAD );
        if( iFALSE == FlagReturn ) goto ERROR;                      
    
        LengthNeedReadData =ImageIndexTable[ImageID].LengthTotalData;
    
        ImageDataEntry = i51AdeOsMalloc(LengthNeedReadData, __FILE__ , __LINE__ );
        if( iNULL == ImageDataEntry ) 
        {
            iLog("KIT - G2 : malloc linear normal");
            return iFALSE;
        }
    	
        i51AdeOsFileRead( File, ImageDataEntry, LengthNeedReadData, &LengthTotalRead);
        if( iFALSE == FlagReturn || LengthNeedReadData!=LengthTotalRead) 
        {
            i51AdeOsFree(ImageDataEntry);
            ImageDataEntry = iNULL;
            goto ERROR;                
        }
    }
    else
    {
        ImageDataEntry = (iU8*)ImageIndexTable[ImageID].MemoryEntry;
    }

    DataEntry = (iU32)ImageDataEntry;
    LengthTotalReadData = I51KITG2_OFFSET_PIXELDATA;
    DataEntry += I51KITG2_OFFSET_PIXELDATA;
  
    if(AlphaSys==i51KITG2_ALPHA_NONEED)
    {
        while(1)
        {
            TempReadDataBuff1 = (iU16 *)(DataEntry);   
            CurrentImageX = TempReadDataBuff1[0];
            CurrentImageY = TempReadDataBuff1[1];
            CurrentImageWidth = TempReadDataBuff1[2];
            
            DataEntry += 6;
            LengthTotalReadData += 6 + (CurrentImageWidth<<1);

            if( (CurrentImageY < SrcStartY) || (CurrentImageX >= SrcEndX) )
            {
                DataEntry += ( CurrentImageWidth<<1 );
                continue;
            }
            
            if( CurrentImageY >= SrcEndY ) break;

            ColorBuff = (iU16*)DataEntry;
            DataEntry += ( CurrentImageWidth<<1 );
            LengthCopyData = CurrentImageWidth;
            AddStartCopy = (iU32)ReadDataBuff;
            
            if( CurrentImageX<SrcStartX )
            {
                LengthCopyData -= SrcStartX - CurrentImageX;
                CurrentImageX = SrcStartX;
                ColorBuff = &ColorBuff[SrcStartX - CurrentImageX];
                if( LengthCopyData<0 ) continue;
            }
            
            if( (CurrentImageX+LengthCopyData) > SrcEndX )
            {
                LengthCopyData = SrcEndX - CurrentImageX;
            }
    
            AddsDirectionY = CurrentImageY-SrcStartY+DrawW;
            CurrY = CurrentImageY - SrcStartY;
            CurrX = CurrentImageX - SrcStartX;
            for( iTemp=0; iTemp<LengthCopyData; iTemp++ )
            {
                RotateX = CurrX + ChangeData[CurrX][0] + ChangeData[AddsDirectionY][0] ;
                RotateY = CurrY + ChangeData[CurrX][1] + ChangeData[AddsDirectionY][1] ;
                CurrX++;
                if( RotateY<DrawStartY|| RotateY>=DrawEndY ||RotateX<DrawStartX||RotateX>=DrawEndX) continue;
    #if LCDBUFF_DEBUG
    i51KitG2CheckLcdbuffer(RotateY*ScreenWidth+ RotateX,1);
    #endif
#if DEBUG_I51KITG2
// iLog("CurrX=%d,CurrY=%d,RotateX=%d,RotateY=%d",CurrX,CurrY,RotateX,RotateY);
#endif
                BuffPos = RotateY*ScreenWidth+ RotateX;
                LcdBuffer[BuffPos] = ColorBuff[iTemp];
                LcdBuffer[BuffPos+1] = ColorBuff[iTemp];
            }
            
            if( (CurrentImageY == SrcEndY) && ((CurrentImageX+LengthCopyData) >= SrcEndX)) break;
            
            if( LengthTotalReadData> (LengthIamgeData-3) ) break;
            
        }
		
        if(TotalAlphaNumber>0)
        {
            iS32 CurrAlphaNum = 0;
            iS32 CurrBuffPos = LengthIamgeData;		
            iS16 AlphaPosX = 0;
            iS16 AlphaPosY = 0;
            iALPHA AlphaNumber = 0;
            iS16 AlphaColor = 0;
            iS32 LcdBuffPos = 0;
//            iU32 TempHeight = ImageHeight -1;
//            iU32 TempWidth = ImageWidth-1;
			
            DataEntry = (iU32)ImageDataEntry+CurrBuffPos + 12;
			
            for( CurrAlphaNum=0; CurrAlphaNum<TotalAlphaNumber;CurrAlphaNum++)
            {
                TempReadDataBuff1 = (iU16 *)(DataEntry);   
                AlphaPosX = TempReadDataBuff1[0];
                AlphaPosY = TempReadDataBuff1[1];
                AlphaNumber = (iALPHA)TempReadDataBuff1[2];
                AlphaColor = TempReadDataBuff1[3];
                DataEntry += 8;
				
                if(AlphaPosY>=SrcEndY) break;
                if( AlphaPosX>=SrcEndX || AlphaPosX<SrcStartX || AlphaPosY<SrcStartY) continue;				

                CurrX = AlphaPosX - SrcStartX;
                CurrY = AlphaPosY - SrcStartY;
                AddsDirectionY = AlphaPosY-SrcStartY+DrawW;
                RotateX =  CurrX + ChangeData[CurrX][0] + ChangeData[AddsDirectionY][0] ;
                RotateY = AlphaPosY - SrcStartY + ChangeData[CurrX][1] + ChangeData[AddsDirectionY][1] ;
                if( RotateY<DrawStartY|| RotateY>=DrawEndY ||RotateX<DrawStartX||RotateX>=DrawEndX) continue;

                LcdBuffPos =RotateY*ScreenWidth+RotateX;
                LcdBuffer[LcdBuffPos] = i51KitG2GetNewColorEx(LcdBuffer[LcdBuffPos], AlphaColor, AlphaNumber);
            }
        }
    }
    else
    {
        while(1)
        {
            TempReadDataBuff1 = (iU16 *)(DataEntry);   
            CurrentImageX = TempReadDataBuff1[0];
            CurrentImageY = TempReadDataBuff1[1];
            CurrentImageWidth = TempReadDataBuff1[2];
            
            DataEntry += 6;
            LengthTotalReadData += 6 + (CurrentImageWidth<<1);

            if( (CurrentImageY < SrcStartY) || (CurrentImageX > SrcEndX) )
            {
                DataEntry += ( CurrentImageWidth<<1 );
                continue;
            }
            
            if( CurrentImageY > SrcEndY ) break;
            ColorBuff = (iU16*)DataEntry;
            DataEntry += ( CurrentImageWidth<<1 );
            LengthCopyData = CurrentImageWidth;
            AddStartCopy = (iU32)ReadDataBuff;
            
            if( CurrentImageX<SrcStartX )
            {
                LengthCopyData -= SrcStartX - CurrentImageX;
                CurrentImageX = SrcStartX;
                ColorBuff = &ColorBuff[SrcStartX - CurrentImageX];
                if( LengthCopyData<0 ) continue;
            }
            
            if( (CurrentImageX+LengthCopyData) > SrcEndX )
            {
                LengthCopyData = SrcEndX - CurrentImageX;
            }
    
            AddsDirectionY = CurrentImageY-SrcStartY+DrawW;
            CurrY = CurrentImageY - SrcStartY;
            CurrX = CurrentImageX - SrcStartX;
            for( iTemp=0; iTemp<LengthCopyData; iTemp++ )
            {
                RotateX = CurrX + ChangeData[CurrX][0] + ChangeData[AddsDirectionY][0] ;
                RotateY = CurrY + ChangeData[CurrX][1] + ChangeData[AddsDirectionY][1] ;
                CurrX++;
                if( RotateY<DrawStartY|| RotateY>=DrawEndY ||RotateX<DrawStartX||RotateX>=DrawEndX) continue;
    #if LCDBUFF_DEBUG
    i51KitG2CheckLcdbuffer(RotateY*ScreenWidth+ RotateX,1);
    #endif
                BuffPos = RotateY*ScreenWidth+ RotateX;
//                BuffPos = RotateY*ScreenWidth+ RotateX;
                TempColor = i51KitG2GetNewColor(LcdBuffer[BuffPos], ColorBuff[iTemp]);
                LcdBuffer[BuffPos] = TempColor;
                LcdBuffer[BuffPos+1] = TempColor;
            }
            
            if( (CurrentImageY == SrcEndY) && ((CurrentImageX+LengthCopyData) >= SrcEndX)) break;
            
            if( LengthTotalReadData> (LengthIamgeData-3) ) break;
            
        }    
        if(TotalAlphaNumber>0)
        {
            iS32 CurrAlphaNum = 0;
            iS32 CurrBuffPos = LengthIamgeData;		
            iS16 AlphaPosX = 0;
            iS16 AlphaPosY = 0;
            iALPHA AlphaNumber = 0;
            iS16 AlphaColor = 0;
            iS32 LcdBuffPos = 0;
//            iU32 TempHeight = ImageHeight -1;
//            iU32 TempWidth = ImageWidth-1;
			
            DataEntry = (iU32)ImageDataEntry+CurrBuffPos + 12;
			
            for( CurrAlphaNum=0; CurrAlphaNum<TotalAlphaNumber;CurrAlphaNum++)
            {
                TempReadDataBuff1 = (iU16 *)(DataEntry);   
                AlphaPosX = TempReadDataBuff1[0];
                AlphaPosY = TempReadDataBuff1[1];
                AlphaNumber = (iALPHA)TempReadDataBuff1[2];
                AlphaColor = TempReadDataBuff1[3];
                DataEntry += 8;
				
                if(AlphaPosY>=SrcEndY) break;
                if( AlphaPosX>=SrcEndX || AlphaPosX<SrcStartX || AlphaPosY<SrcStartY) continue;				

                CurrX = AlphaPosX - SrcStartX;
                CurrY = AlphaPosY - SrcStartY;
                AddsDirectionY = AlphaPosY-SrcStartY+DrawW;
                RotateX =  CurrX + ChangeData[CurrX][0] + ChangeData[AddsDirectionY][0] ;
                RotateY = AlphaPosY - SrcStartY + ChangeData[CurrX][1] + ChangeData[AddsDirectionY][1] ;
                if( RotateY<DrawStartY|| RotateY>=DrawEndY ||RotateX<DrawStartX||RotateX>=DrawEndX) continue;

                LcdBuffPos =RotateY*ScreenWidth+RotateX;
                LcdBuffer[LcdBuffPos] = i51KitG2GetNewColorEx(LcdBuffer[LcdBuffPos], AlphaColor, AlphaNumber*AlphaSrc/255);
            }
        }
    }
#if DEBUG_I51KITG2
//iLog("Rotate OK" );
#endif	
    if( iFALSE==ImageIndexTable[ImageID].FlagCopyRAM )
    {
        i51AdeOsFree(ImageDataEntry);
        ImageDataEntry = iNULL;
    }
    return iTRUE;

ERROR:
    iLog("KIT - G2 : RW rotate");
    if( iFALSE==ImageIndexTable[ImageID].FlagCopyRAM )
    {
        i51AdeOsFree(ImageDataEntry);
        ImageDataEntry = iNULL;
    }
    return iFALSE;
	
}

#if 0
static iBOOL i51KitG2ImageDecode2File( iFILE SrcFile, iFILE DesFile)
{
    // author : Otto
    // since : Aug 31 2011
    // (C) 2011 , PKIG Tech. Co., Ltd.

    // iFILE SrcFile : 需要解码的数据存放文件指针
    // iFILE DesFile : 解码后的数据存放文件指针

    // notes : 从文件SrcFile中解压一张图片到文件DesFile中
    // SrcFile文件中存放的图片数据是分段用Zlib压缩的
    // 具体文件格式请参考文档"The_i51_DC_dynamic_library_G2_i51_Image_Format.xls"

    // 从文件中读取的CRC
    iU32 CRC_Read = 0;
    // 读取文件信息，计算得到的CRC
    iU32 CRC_Calculate = 0;
    // 目标数据长度，即解压后文件长度
    iU32 LengthDesData = 0;
    // 原始数据长度，即压缩前文件长度
    iU32 LengthSrcData = 0;
    // 解压后段最长长度,默认设置为32K
    iU32 LengthPartMax = 0;
    // 段数量
    iU32 NumberParts = 0;
    // 当前段解压前数据长度
    iU64 LengthPartSrc = 0;
    // 当前段解压后数据长度
    iU64 LengthPartDes = 0;
    // 函数返回值
    iBOOL FlagReturn = iFALSE;
    // 图片数据开始位置偏移量
    iU32 FileSeekStart = 0;
    // 文件当前偏移量
    iU32 FileSeekCureent = 0;
    // 解压前段数据buffer
    iU8  * DataSrcCurrentPart = iNULL;
    // 解压后段数据buffer
    iU8  * DataDesCurrentPart = iNULL;
    // 实际从文件中一次读取到的数据长度  
    iU32 LengFactReadData = 0;
    // 实际一次写入文件中的数据长度  
    iU32 LengFactWriteData = 0;
    iU32 iTemp = 0;
    iU32 FlagFirst = iTRUE;

    DataSrcCurrentPart = i51AdeOsMalloc(I51KITG2_LENGTH_PART_UMCOMPRESS, __FILE__ , __LINE__ );
    if( iNULL == DataSrcCurrentPart )	return iFALSE;
	
    DataDesCurrentPart = i51AdeOsMalloc(I51KITG2_LENGTH_PART_UMCOMPRESS, __FILE__ , __LINE__ );
    if( iNULL == DataDesCurrentPart )
    {
        i51AdeOsFree(DataSrcCurrentPart);
        DataSrcCurrentPart = iNULL;		
        return iFALSE;
    }

    FlagReturn = i51AdeOsFileGetSeek( SrcFile, (iS32*)(&FileSeekStart), i51_OS_FL_SEEK_HEAD );
    if( iFALSE == FlagReturn ) return iFALSE;

    FlagReturn = i51AdeOsFileReadInt32( SrcFile, &CRC_Read);
    if( iFALSE == FlagReturn ) return iFALSE;
  
    FlagReturn = i51AdeOsFileReadInt32( SrcFile, &LengthSrcData);
    if( iFALSE == FlagReturn ) return iFALSE;
  
    FlagReturn = i51AdeOsFileReadInt32( SrcFile, &LengthDesData);
    if( iFALSE == FlagReturn ) return iFALSE;
  
    FlagReturn = i51AdeOsFileReadInt32( SrcFile, &LengthPartMax);
    if( iFALSE == FlagReturn ) return iFALSE;
  
    FlagReturn = i51AdeOsFileReadInt16( SrcFile, (iU16*)(&NumberParts));
    if( iFALSE == FlagReturn ) return iFALSE;

    for( iTemp=0; iTemp<NumberParts; iTemp++ )
    {
        FlagReturn = i51AdeOsFileReadInt32( SrcFile, (iU32*)(&LengthPartSrc));
        if( iFALSE == FlagReturn ) return iFALSE;

        FlagReturn = i51AdeOsFileRead( SrcFile, DataSrcCurrentPart, LengthPartSrc, &LengFactReadData);
        if( (iFALSE == FlagReturn) || (LengthPartSrc!=LengFactReadData)  ) return iFALSE;

        LengthPartDes = I51KITG2_LENGTH_PART_UMCOMPRESS;
        FlagReturn = i51AdeStdUncompress( DataDesCurrentPart, &LengthPartDes, DataSrcCurrentPart, LengthPartSrc);
        if( iFALSE == FlagReturn ) return iFALSE;

        if( LengthPartDes < I51KITG2_LENGTH_PART_UMCOMPRESS)
        {
           FlagFirst = iFALSE;
        }
        FlagReturn = i51AdeOsFileWrite( DesFile, DataDesCurrentPart, LengthPartDes, &LengFactWriteData );
        if( (iFALSE == FlagReturn) || (LengthPartDes!=LengFactWriteData)  ) return iFALSE;
    }

    i51AdeOsFileFlush( DesFile );

    return iTRUE;
  
}
#endif

#if DEBUG_I51KITG2

static iBOOL i51KitG2DrawLineEx( iS32 Point[][2], iCOLOR Color)
{

    // author : Otto
    // since : Aug 10 2011
    // (C) 2011 , PKIG Tech. Co., Ltd.

    // iU32 Point[][2] :
    //    Point[0][0]是需要绘制线段的起点X坐标
    //    Point[0][1]是需要绘制线段的起点Y坐标
    //    Point[1][0]是需要绘制线段的终点X坐标
    //    Point[1][1]是需要绘制线段的终点Y坐标
    // iCOLOR Color : 绘制线段的颜色

    // notes : 以颜色Color绘制一条从点(Point[0][0], Point[0][1])到点(Point[1][0], Point[1][1])的线段
    // 绘制算法使用的是对称直线生成算法，对称直线生成算法基于这样一个事实：直线以中点为界，其两边是对称的,
    // 因而可以利用这个对称性，对Bresenham算法进行改进，使得每进行一次判断就可以生成相对于直线中点的两个
    // 对称点。如此以来，直线就由两端向中间生成

    iS32 StartX = Point[0][0];
    iS32 StartY = Point[0][1];
    iS32 EndX = Point[1][0];
    iS32 EndY = Point[1][1];

    iS32 TempX1, TempY1;	
    iS32 TempX2, TempY2;	

    //起点与终点X和Y的差值
    iS32 dx = 0;
    iS32 dy = 0;
    iS32 dx2 = 0;
    iS32 dy2 = 0;

    // 决策量
    iS32 DecisionTag = 0;

    // 需要绘制次数
    iU32 HalfLength = 0;

    //当前绘制位置
    iU32 Location = 0;

    //校准Y值
    iS16 RegulationY = 1;

    // 绘制区域	
    iS16 DrawRectStartX = 0;
    iS16 DrawRectStartY = 0;
    iS16 DrawRectEndX = ScreenWidth;
    iS16 DrawRectEndY = ScreenHeight;
    iU32 BuffPos = 0;
    iS32 Temp1 = 0;

	
    iU16 * ScreenBuffer = iNULL;
    iU32 * LcdInfoArr = iNULL;
    iU16 ScreenWidth = 0;
    iU16 ScreenHeight= 0;
	
    if( i51AdeMmiGetScreenScale ( &ScreenWidth , &ScreenHeight ) == 0 )
    {
        return iFALSE;
    }

    LcdInfoArr =  i51AdeMmiGetPanel();
    if( iNULL == LcdInfoArr )
    {
        return iFALSE;
    }
  
    ScreenBuffer = (iU16 *) LcdInfoArr[1] ;	
    for( Temp1=0; Temp1<2; Temp1++ )
    {
        if( Point[Temp1][0]>2000||Point[Temp1][0]<-1500
         ||Point[Temp1][1]>2000||Point[Temp1][1]<-1500		
        )
        {
            return iFALSE;
        }
    }     	

    DrawRectStartX = 0;
    DrawRectStartY = 0;
    DrawRectEndX = ScreenWidth;
    DrawRectEndY = ScreenHeight;
    StartX = Point[0][0];
    StartY = Point[0][1];
    EndX = Point[1][0];
    EndY = Point[1][1];

    //  如果需要绘制的线段的两端点都在屏幕外，且在屏幕同侧
    //  则绘制的线段是屏幕没有交集
    if( (StartX<0&&EndX<0)
      ||(StartX>=ScreenWidth&&EndX>=ScreenWidth)
      ||(StartY<0&&EndY<0)
      ||(StartY>=ScreenHeight&&EndY>=ScreenHeight)
      )
    {
        return iTRUE;
    }

    //如果终点在起点左边则交换他们的位置
    if ( EndX < StartX )
    {
        EndX += StartX;
        StartX = EndX - StartX;
        EndX = EndX - StartX;
  
        EndY += StartY;
        StartY = EndY - StartY;
        EndY = EndY - StartY;
    }

    //计算起点与终点X和Y的差值
    dx = EndX - StartX;
    if( EndY > StartY )
    {
        dy = EndY - StartY;
        RegulationY = 1;
    }
    else
    {
        dy = StartY - EndY;
        RegulationY = -1;
    }

    //起点与终点X和Y的差值放大一倍
    dx2 = dx << 1;
    dy2 = dy << 1;
  
    //决策量赋初值
    if( dx>=dy){
        DecisionTag = dy2 - dx;
        HalfLength = (dx+1) >> 1;
    } else{
        DecisionTag = dy - dx2;
        HalfLength = (dy+1) >> 1;
    }

    for ( Location = 0; Location <= HalfLength; Location++)
    {
        TempX1 = StartX;
        TempY1 = StartY;
        TempX2 = EndX;
        TempY2 = EndY;
		
         if( TempX1>=DrawRectStartX && TempX1<DrawRectEndX && TempY1>=DrawRectStartY&& TempY1<DrawRectEndY )
        {
              ScreenBuffer[ (ScreenWidth*TempY1 + TempX1)] = Color;
        }

         if( TempX2>=DrawRectStartX && TempX2<DrawRectEndX && TempY2>=DrawRectStartY&& TempY2<DrawRectEndY )
        {
              ScreenBuffer[ (ScreenWidth*TempY2 + TempX2)] = Color;
        }

        if( dx>= dy ){
            // 当DecisionTag>0时，始端y向加RegulationY，末端y向减RegulationY
            if (DecisionTag > 0)
            {
                StartY += RegulationY;
                EndY -= RegulationY;    
                DecisionTag -= dx2;
            }
    
            //始端x向加1，末端x向减1
            StartX++;    
            EndX--;
            DecisionTag += dy2;
        }
        else
        {
            // 当DecisionTag<0时，始端X向加1，末端X向减1
            if (DecisionTag < 0)
            {
                StartX++;    
                EndX--;
                DecisionTag += dy2;
           }
    
            //始端y向加RegulationY，末端y向减RegulationY
            StartY += RegulationY;
            EndY -= RegulationY;          
            DecisionTag -= dx2;

        }
    }

    return iTRUE;

}

static iU32 i51KitG2GetImageAttrEx(iU32  SrcData, iU32 * ImageWidth, iU32 * ImageHeight)
{
    // 前面四个字节是保存的图片宽度和高度	
    iU8 * SrcDataPoint = (iU8 * )(SrcData);
    iU16 ImageW = 0;
    iU16 ImageH = 0;	

    if( SrcData==iNULL ) return iFALSE;

    ImageW = 	SrcDataPoint[0]|(SrcDataPoint[1]<<8);
    ImageH = 	SrcDataPoint[2]|(SrcDataPoint[3]<<8);
//    i51AdeStdMemcpy16(&ImageW, &SrcDataPoint[0], 2);
//    i51AdeStdMemcpy16(&ImageH, &SrcDataPoint[2], 2);

    *ImageWidth = ImageW;
    *ImageHeight = ImageH;
	
    return iTRUE;

}

static iU32 i51KitG2ImageCreate( iU32  SrcData)
{
    // author : Otto
    // since : 10.20 2011
    // (C) 2011 , PKIG Tech. Co., Ltd.

    // iU32 SrcData : 需要解码的数据存放指针

    // notes : 解压一张图片到内内存中
    // SrcFile文件中存放的图片数据是分段用Zlib压缩的
    // 具体文件格式请参考文档"The_i51_DC_dynamic_library_G2_i51_Image_Format.xls"

    // 段数量
    iU16 NumberParts = 0;
    // 当前段解压前数据长度
    iU32 LengthPartSrc = 0;
    // 当前段解压后数据长度
    iU32 LengthPartDes = 0;
    // 函数返回值
    iBOOL FlagReturn = iFALSE;
    // 解压后数据buffer
    iU8  * DesDataPoint = iNULL;
    iU32   DesDataLength = iNULL;
    iU32   SrcDataLength = iNULL;
    iU32 SrcPos = 0;
    iU32 DesPos = 0;	
    iU32 iTemp = 0;
    iU32 DesTempPos = 0;
    // 图片带调色板
    iU8 PaletteImage = iFALSE;
    // 调色板中颜色总数
    iU16 PaletteColorNum = 0;
    // 调色板颜色索引表
    iCOLOR * PaletteColorMem = iNULL;
    // 一段数据解码的临时存放处
    iU8  * TempDesData = iNULL;
    // 一个数据块的长度	:=6+连续点个数
    iS32 BlockLength = 0;
    // 当前数据块位置
    iS32 BlockPlace = 0;
    // 颜色临时变量	
    iCOLOR TempColor = 0;
    // 颜色指针地址
    iCOLOR * ColorPoint = iNULL;
    // 段数据头	
    iU8 TempHead[6];
    iS32 Temp = 0;		
    // 前面四个字节是保存的图片宽度和高度	
    iU8 * SrcDataPoint = iNULL;// = (iU8 * )(SrcData+4);
    iU16 ImageW = 0;
    iU16 ImageH = 0;	


    if( SrcData==iNULL ) return iFALSE;
	
    SrcDataPoint = (iU8 * )(SrcData);
    ImageW = 	SrcDataPoint[0]|(SrcDataPoint[1]<<8);
    ImageH = 	SrcDataPoint[2]|(SrcDataPoint[3]<<8);
	
    // 前面四个字节是保存的图片宽度和高度	
    SrcDataPoint = (iU8 * )(SrcData+4);
    DesDataLength = SrcDataPoint[4]|(SrcDataPoint[5]<<8)|(SrcDataPoint[6]<<16)|(SrcDataPoint[7]<<24);
    SrcDataLength = SrcDataPoint[8]|(SrcDataPoint[9]<<8)|(SrcDataPoint[10]<<16)|(SrcDataPoint[11]<<24);
    NumberParts = SrcDataPoint[16]|(SrcDataPoint[17]<<8);

    DesDataPoint = ( iU8 *)i51AdeOsMalloc( DesDataLength,  __FILE__ , __LINE__ );
    if( iNULL == DesDataPoint )
    {
        iLog("KIT - G2 : image create malloc des");
        goto ERROR;
    }

    if( NumberParts==1 && DesDataLength+514<32768 )
    {
        TempDesData = ( iU8 *)i51AdeOsMalloc( DesDataLength+514,  __FILE__ , __LINE__ );
    }
    else
    {
        TempDesData = ( iU8 *)i51AdeOsMalloc( 32768,  __FILE__ , __LINE__ );
    }
    if( iNULL == TempDesData )
    {
        iLog("KIT - G2 : image create malloc temp");
        goto ERROR;
    }

    SrcPos = 18;
    DesPos = 0;
	
    for( iTemp=0; iTemp<NumberParts; iTemp++ )
    {
        LengthPartSrc = SrcDataPoint[SrcPos]|(SrcDataPoint[SrcPos+1]<<8)|(SrcDataPoint[SrcPos+2]<<16)|(SrcDataPoint[SrcPos+3]<<24);
        SrcPos += 4;
		
        LengthPartDes = 32768;

        FlagReturn = i51AdeStdUncompress( TempDesData, &LengthPartDes, &SrcDataPoint[SrcPos], LengthPartSrc);
        if( iFALSE == FlagReturn ) goto ERROR;

        if( iTemp==0 )
        {
            if( TempDesData[4]>0X40)
            {
                PaletteImage = iTRUE;
                PaletteColorNum = (TempDesData[20]) | (TempDesData[21]<<8); 
                PaletteColorMem = i51AdeOsMalloc(PaletteColorNum*2, __FILE__ , __LINE__ );
                if ( iNULL==PaletteColorMem )
                {
                    iLog("KIT - G2 : mage create malloc PaletteColor" );
                    goto ERROR;
                }
                i51AdeStdMemcpy16(PaletteColorMem, &(TempDesData[22]), PaletteColorNum*2 );				
            }
        }
		
        SrcPos += LengthPartSrc;
        if( iFALSE==PaletteImage )
        {
//            DesTempPos = DesPos;
            i51AdeStdMemcpy16(&(DesDataPoint[DesPos]), TempDesData, LengthPartDes);				
            DesPos += LengthPartDes;
        }
        else
        {
            //带索引表的图片需要先把索引转为颜色
            if( iTemp==0 )
            {
                i51AdeStdMemcpy16(DesDataPoint,TempDesData, 20);				
                DesTempPos = 20 + PaletteColorNum*2 +2;
                DesPos = 20;
                BlockLength = 0;
                BlockPlace = 0;
            }
            else
            {
                DesTempPos = 0;
            }

            while(DesTempPos<LengthPartDes)
            {
                if( BlockPlace==0 )
                {
                    // 块重新开始
                    if( (LengthPartDes-DesTempPos)>=6 )
                    {
                        // 段剩余数据长度大于等于块头长度
                        BlockPlace = 6;
                        BlockLength = TempDesData[DesTempPos+4] | (TempDesData[DesTempPos+5]<<8);
                        i51AdeStdMemcpy16(&DesDataPoint[DesPos], &TempDesData[DesTempPos], 6);
                        i51AdeStdMemcpy16(TempHead, &TempDesData[DesTempPos], 6);
                        DesTempPos += 6;
                        DesPos += 6;
                    }
                    else
                    {
                        // 段剩余数据小于块头长度，把剩余数据保存
                        BlockPlace = LengthPartDes-DesTempPos;
                        for( Temp = 0; Temp < BlockPlace; Temp++ )
                        {
                            TempHead[Temp] = TempDesData[DesTempPos+Temp];
                        }
                        DesTempPos = 0;						
                        break; 						
                    }
                }
                else if( BlockPlace<6 )				
                {
                    for( Temp = BlockPlace; Temp < 6; Temp++ )
                    {
                        TempHead[Temp] = TempDesData[DesTempPos+Temp-BlockPlace];
                    }
                    //i51AdeStdMemcpy16(&TempDesData[DesPos], TempHead, 6);
                    i51AdeStdMemcpy16(&DesDataPoint[DesPos], TempHead, 6);
                    BlockLength = TempHead[4] | (TempHead[5]<<8);
                    DesTempPos += 6-BlockPlace;
                    DesPos += 6;
                    BlockPlace = 6;
                }
                else
                {
                    BlockLength = TempHead[4] | (TempHead[5]<<8);
                    BlockLength -= BlockPlace - 6;
                }

                ColorPoint = (iCOLOR*)(((iU32)DesDataPoint)+DesPos);

                if( DesTempPos+BlockLength > LengthPartDes)
                {
                    BlockLength = LengthPartDes - DesTempPos;
                    BlockPlace = 6 + BlockLength;
                }
                else				
                {
                    BlockPlace = 0;
                }
				
                for( Temp=0; Temp<BlockLength; Temp++ )
                {
                    ColorPoint[Temp] = PaletteColorMem[TempDesData[DesTempPos]];
                    DesTempPos++;
                    DesPos += 2;
                }
            	}
     }

		
    }

    if(PaletteColorMem)
    {
        i51AdeOsFree(PaletteColorMem);
        PaletteColorMem = iNULL;
    }
 	
    if(TempDesData)
    {
        i51AdeOsFree(TempDesData);
        TempDesData = iNULL;
    }
	
    if(i51AdeCnfGetEndian()==1)
    {
        DesTempPos = 0X14; 
        while(DesTempPos<DesPos)
        {
            DesDataPoint[DesTempPos] += DesDataPoint[DesTempPos+1];
            DesDataPoint[DesTempPos+1] = DesDataPoint[DesTempPos] - DesDataPoint[DesTempPos+1];
            DesDataPoint[DesTempPos] = DesDataPoint[DesTempPos] - DesDataPoint[DesTempPos+1];
            DesTempPos += 2;
        }
    }

    return (iU32)(DesDataPoint);

 ERROR:

    if(PaletteColorMem)
    {
        i51AdeOsFree(PaletteColorMem);
        PaletteColorMem = iNULL;
    }
 	
    if(TempDesData)
    {
        i51AdeOsFree(TempDesData);
        TempDesData = iNULL;
    }

    if(DesDataPoint)
    {
        i51AdeOsFree(DesDataPoint);
        DesDataPoint = iNULL;
    }

    iLog("KIT - G2 : create image ");
	
    return iFALSE;
	
}

static iBOOL i51KitG2ImageDestory( iU32  SrcData)
{
    // author : Otto
    // since : 10.20 2011
    // (C) 2011 , PKIG Tech. Co., Ltd.

    // iU32 SrcData : 需要销毁的图片数据地址

    // notes : 销毁由i51KitG2ImageCreate函数创建的图片
    
    if( iNULL == SrcData ) return iFALSE;
	
    i51AdeOsFree((void *)SrcData);
    SrcData = 0;
    return iTRUE;

}


static iCOLOR i51KitG2GetNewColorEx1 (iCOLOR dpixel, iCOLOR spixel, iALPHA SrcAlpha)
{
/**
 * @fn		unsigned short calculate_alpha (unsigned short dpixel, unsigned short spixel, unsigned char alpha)
 * @brief	计算alpha值。alpha算法：dest <- (dest*(255 - alpha) + src * alpha) / 255
 * 
 * @param	dpixel	目的像素值
 * @param	spixel	源像素值
 * @param	alpha	alpha值
 * 
 * @return	返回alpha运算结果
 */
    iU32 rd, gd, bd;
    iU32 rs, gs, bs;
    iALPHA DesAlpha = 0XFF - SrcAlpha;
	
    rs = (spixel) & 0XF800;
    gs = (spixel) & 0X07E0;
    bs = spixel & 0X1F;
                                
    rd = (dpixel) & 0XF800;
    gd = (dpixel) & 0X07E0;
    bd = dpixel & 0X1F;
                                
    rd = ((rd*DesAlpha+ rs * SrcAlpha)>>8)&0XF800;
    gd = ((gd*DesAlpha+ gs * SrcAlpha)>>8)&0X7E0;
    bd = ((bd*DesAlpha+ bs * SrcAlpha)>>8)&0X1F;
    return (bd +(gd)+ (rd ));

}

static iBOOL i51KitG2DrawImageEx(iU32 ImageData, iRECT * SrcRect, iRECT * DesRect)
{
    iU8 * ImageDataPoint = iNULL;
    iU32 ImageWidth = 0;
    iU32 ImageHeight = 0;
    iU16 * TempReadDataBuff = iNULL;
    iU32 CopyDataEntry = 0;
    // 累计读取到的数据长度
    iU32  LengthTotalReadData = 0;
    // 一次需要拷贝到缓冲区的颜色数量
    iS32 LengthCopyColor = 0;
    // 图片文件总长度
    iU32 LengthIamgeData = 0;

    // 当前数据段在屏幕上的开始绘制位置
    iS16 TempDrawX = 0;
    iS16 TempDrawY = 0;
    // 图片绘制的左上角和右下角坐标  
    iS16 SrcStartX = 0;
    iS16 SrcStartY = 0;
    iS16 SrcEndX = 0;
    iS16 SrcEndY = 0;
    // 屏幕绘制的左上角和右下角坐标  
    iS16 DesStartX = 0;
    iS16 DesStartY = 0;
    iS16 DesEndX = 0;
    iS16 DesEndY = 0;
    iS32 iTemp = 0;

    // ImageX
    iS32 CurrentImageX = 0;
    iS32 CurrentImageY = 0;
    iS32 CurrentImageWidth = 0;


    iU32 LcdBufferPostion = 0;

    // 读到的一段数据首地址
    iU16 * TempReadDataBuff1 = iNULL;

    iU16 * ScreenBuffer = iNULL;
    iU32 * LcdInfoArr = iNULL;
    iS16 SrcW, SrcH, DesW, DesH;
    iS16 DrawW, DrawH;
    iU16 ScreenWidth = 0;
    iU16 ScreenHeight= 0;
    iU16 CurrColor = 0;
    iU16 TotalAlphaNumber = 0;

    ImageDataPoint = (iU8 *)ImageData;
    if( iNULL==ImageDataPoint ) return iFALSE;
	
    if( ImageDataPoint[0X11]!=0X02 )
    {
    	iLog("文件格式错误，请重新生成图片文件!");
    	return iFALSE;
    }

    
    if( i51AdeMmiGetScreenScale ( &ScreenWidth , &ScreenHeight ) == 0 )
    {
        return iFALSE;
    }

    LcdInfoArr =  i51AdeMmiGetPanel();
    if( iNULL == LcdInfoArr )
    {
        return iFALSE;
    }
  
    ScreenBuffer = (iU16 *) LcdInfoArr[1] ;
	
//    i51AdeStdMemcpy16(&ImageWidth, &ImageDataPoint[0X09], 2);
//    i51AdeStdMemcpy16(&ImageHeight, &ImageDataPoint[0X0B], 2);
//    i51AdeStdMemcpy16(&LengthIamgeData, &ImageDataPoint[0X05], 4);
ImageWidth = ImageDataPoint[9]|(ImageDataPoint[10]<<8);
ImageHeight = ImageDataPoint[11]|(ImageDataPoint[12]<<8);
LengthIamgeData = ImageDataPoint[5]|(ImageDataPoint[6]<<8)|(ImageDataPoint[7]<<16)|(ImageDataPoint[8]<<24);
TotalAlphaNumber = ImageDataPoint[18]|(ImageDataPoint[19]<<8);
//iLog("ImageWidth=%d",ImageWidth);
//iLog("ImageHeight=%d",ImageHeight);
//iLog("LengthIamgeData=%d",LengthIamgeData);

    if(SrcRect)
    {
        SrcStartX = SrcRect->X;
        SrcStartY = SrcRect->Y;
        SrcW = SrcRect->Width;
        SrcH = SrcRect->Height;
    }
    else
    {
        SrcStartX = 0;
        SrcStartY = 0;
        SrcW = ImageWidth;
        SrcH = ImageHeight;
    }
//iLog("SrcW=%d",SrcW);
//iLog("SrcH=%d",SrcH);

    if( DesRect )
    {
        DesStartX= DesRect->X;
        DesStartY= DesRect->Y;
        DesW = DesRect->Width;
        DesH = DesRect->Height;
    }
    else
    {
        DesStartX= 0;
        DesStartY= 0;
        DesW = ScreenWidth;
        DesH = ScreenHeight;
    }
	
    if(SrcStartX<0)
    {
        SrcW += SrcStartX;
		DesW += SrcStartX;
		DesStartX -= SrcStartX;
        SrcStartX= 0;		
    }

    if(SrcStartY<0)
    {
        SrcH += SrcStartY;
		DesH += SrcStartY;
		DesStartY -= SrcStartY;
        SrcStartY= 0;		
    }

    if(DesStartX<0)
    {
        DesW += DesStartX;
		SrcW += DesStartX;
		SrcStartX -= DesStartX;
        DesStartX= 0;		
    }

    if(DesStartY<0)
    {
        DesH += DesStartY;
		SrcH += DesStartY;
		SrcStartY -= DesStartY;
        DesStartY= 0;		
    }

    if( SrcStartX+SrcW > ImageWidth) SrcW = ImageWidth - SrcStartX;
    if( SrcStartY+SrcH > ImageHeight) SrcH= ImageHeight- SrcStartY;
	
    if( DesStartX+DesW > ScreenWidth) DesW = ScreenWidth - DesStartX;
    if( DesStartY+DesH > ScreenHeight) DesH= ScreenHeight- DesStartY;

    if( SrcW<DesW ) DrawW = SrcW;
    else DrawW = DesW;

    if( SrcH<DesH ) DrawH = SrcH;
    else DrawH = DesH;
	
	ImageDataPoint += 0X14;
    LengthTotalReadData = 0X14;	
    SrcEndX = SrcStartX + DrawW;
    SrcEndY = SrcStartY + DrawH;
    DesEndX = DesStartX + DrawW;
    DesEndY = DesStartY + DrawH;

	if(SrcRect)
	{
		if( SrcStartX>=SrcRect->X + SrcRect->Width || SrcEndX<SrcRect->X )
		{
			return iTRUE;
		}
		if( SrcStartY>=SrcRect->Y + SrcRect->Height|| SrcEndY<SrcRect->Y )
		{
			return iTRUE;
		}
	}
	else
	{
		if( SrcStartX>=ImageWidth || SrcEndX<0 )
		{
			return iTRUE;
		}
		if( SrcStartY>=ImageHeight || SrcEndY<0 )
		{
			return iTRUE;
		}
	}
	
//iLog("DesStartX=%d",DesStartX);
//iLog("DesStartY=%d",DesStartY);
//iLog("DesEndX=%d",DesEndX);
//iLog("DesEndY=%d",DesEndY);
	
    while(1)
    {
        TempReadDataBuff1 = (iU16 *)(ImageDataPoint);   
        CurrentImageX = TempReadDataBuff1[0];
        CurrentImageY = TempReadDataBuff1[1];
        CurrentImageWidth = TempReadDataBuff1[2];
//CurrentImageX = ImageDataPoint[1]|(ImageDataPoint[0]<<8);
//CurrentImageY = ImageDataPoint[3]|(ImageDataPoint[2]<<8);
//CurrentImageWidth = ImageDataPoint[5]|(ImageDataPoint[4]<<8);
//iLog("CurrentImageX=%d",CurrentImageX);
//iLog("CurrentImageY=%d",CurrentImageY);
//iLog("CurrentImageWidth=%d",CurrentImageWidth);
        ImageDataPoint += 6;
        CopyDataEntry = (iU32)ImageDataPoint;
        LengthCopyColor =  CurrentImageWidth;

        LengthTotalReadData += 6 + (CurrentImageWidth<<1);
        ImageDataPoint += ( CurrentImageWidth<<1 );
		
        if( CurrentImageY>= SrcEndY) break;
		
        // 图片当前数据段X开始坐标在图片绘制区域右边或者Y坐标在图片绘制区域上方            
        if( (CurrentImageY < SrcStartY) || (CurrentImageX >= SrcEndX))
        {
            if( LengthTotalReadData> (LengthIamgeData-3) ) break;
            continue;
        }
        
        // 当前数据段长度超出图片绘制区域右边
        if( (CurrentImageX+CurrentImageWidth) >SrcEndX )
        {
            LengthCopyColor -= ((CurrentImageX+CurrentImageWidth-SrcEndX));
        }
		
        // 图片当前数据段X开始坐标在图片绘制区域左边            
        if( CurrentImageX<SrcStartX )
        {
            LengthCopyColor -= ((SrcStartX - CurrentImageX));
            CopyDataEntry += ((SrcStartX - CurrentImageX)<<1);
            if( LengthCopyColor<=0 ) 
            {
                if( LengthTotalReadData> (LengthIamgeData-3) ) break;
                continue;
            }
        }

         // 计算当前数据段在屏幕上开始显示的位置
         TempDrawX = DesStartX;
         TempDrawY = DesStartY + CurrentImageY - SrcStartY;

        // 图片当前数据段X开始坐标在图片开始绘制绘制区域右边
        // 需要移动屏幕开始绘制区域
        if( CurrentImageX>SrcStartX )
        {
            TempDrawX += ((CurrentImageX-SrcStartX));
        }

        if( TempDrawX >= DesEndX)
        {
            if( LengthTotalReadData> (LengthIamgeData-3) ) break;
            continue;
        }
        
        TempReadDataBuff = (iU16*)CopyDataEntry;
        i51AdeStdMemcpy16( &ScreenBuffer[ TempDrawY*ScreenWidth+TempDrawX], TempReadDataBuff, (LengthCopyColor<<1));
        if( LengthTotalReadData> (LengthIamgeData-3) ) break;
		
     }
	
        if(TotalAlphaNumber>0)
        {
            iS32 CurrAlphaNum = 0;
            iS32 CurrBuffPos = LengthIamgeData;		
            iS16 AlphaPosX = 0;
            iS16 AlphaPosY = 0;
            iALPHA AlphaNumber = 0;
            iS16 AlphaColor = 0;
            iS32 LcdBuffPos = 0;
            iU32 TempHeight = ImageHeight -1;
            iU32 TempWidth = ImageWidth-1;
			
            ImageDataPoint = (iU8*)((iU32)ImageData+CurrBuffPos + 12);
			
            for( CurrAlphaNum=0; CurrAlphaNum<TotalAlphaNumber;CurrAlphaNum++)
            {
                TempReadDataBuff1 = (iU16 *)(ImageDataPoint);   
                AlphaPosX = TempReadDataBuff1[0];
                AlphaPosY = TempReadDataBuff1[1];
                AlphaNumber = (iALPHA)TempReadDataBuff1[2];
                AlphaColor = TempReadDataBuff1[3];
                ImageDataPoint += 8;
				
                if(AlphaPosY>=SrcEndY) break;
                if( AlphaPosX>=SrcEndX || AlphaPosX<SrcStartX || AlphaPosY<SrcStartY) continue;				

                LcdBuffPos = (AlphaPosY-SrcStartY+DesStartY)*ScreenWidth+AlphaPosX-SrcStartX+DesStartX;
                ScreenBuffer[LcdBuffPos] = i51KitG2GetNewColorEx1(ScreenBuffer[LcdBuffPos], AlphaColor, AlphaNumber);
            }
        }		
    return iTRUE;
	
}

static iBOOL i51KitG2MirrorImageEx(iU32 ImageData, iRECT * SrcRect, iRECT * DesRect)
{
    iU8 * ImageDataPoint = iNULL;
    iU32 ImageWidth = 0;
    iU32 ImageHeight = 0;
    iU16 * TempReadDataBuff = iNULL;
    iU32 CopyDataEntry = 0;
    // 累计读取到的数据长度
    iU32  LengthTotalReadData = 0;
    // 一次需要拷贝到缓冲区的颜色数量
    iS32 LengthCopyColor = 0;
    // 图片文件总长度
    iU32 LengthIamgeData = 0;

    // 当前数据段在屏幕上的开始绘制位置
    iS16 TempDrawX = 0;
    iS16 TempDrawY = 0;
    // 图片绘制的左上角和右下角坐标  
    iS16 SrcStartX = 0;
    iS16 SrcStartY = 0;
    iS16 SrcEndX = 0;
    iS16 SrcEndY = 0;
    // 屏幕绘制的左上角和右下角坐标  
    iS16 DesStartX = 0;
    iS16 DesStartY = 0;
    iS16 DesEndX = 0;
    iS16 DesEndY = 0;
    iS32 iTemp = 0;

    // ImageX
    iS32 CurrentImageX = 0;
    iS32 CurrentImageY = 0;
    iS32 CurrentImageWidth = 0;


    iU32 LcdBufferPostion = 0;

    // 读到的一段数据首地址
    iU16 * TempReadDataBuff1 = iNULL;

    iU16 * ScreenBuffer = iNULL;
    iU32 * LcdInfoArr = iNULL;
    iS16 SrcW, SrcH, DesW, DesH;
    iS16 DrawW, DrawH;
    iU16 ScreenWidth = 0;
    iU16 ScreenHeight= 0;
    iU16 CurrColor = 0;
    iU16 TotalAlphaNumber = 0;

    ImageDataPoint = (iU8 *)ImageData;
    if( iNULL==ImageDataPoint ) return iFALSE;
	
    if( ImageDataPoint[0X11]!=0X02 )
    {
    	iLog("文件格式错误，请重新生成图片文件!");
    	return iFALSE;
    }

    
    if( i51AdeMmiGetScreenScale ( &ScreenWidth , &ScreenHeight ) == 0 )
    {
        return iFALSE;
    }

    LcdInfoArr =  i51AdeMmiGetPanel();
    if( iNULL == LcdInfoArr )
    {
        return iFALSE;
    }
  
    ScreenBuffer = (iU16 *) LcdInfoArr[1] ;
	
ImageWidth = ImageDataPoint[9]|(ImageDataPoint[10]<<8);
ImageHeight = ImageDataPoint[11]|(ImageDataPoint[12]<<8);
LengthIamgeData = ImageDataPoint[5]|(ImageDataPoint[6]<<8)|(ImageDataPoint[7]<<16)|(ImageDataPoint[8]<<24);
TotalAlphaNumber = ImageDataPoint[18]|(ImageDataPoint[19]<<8);

    if(SrcRect)
    {
        SrcStartX = SrcRect->X;
        SrcStartY = SrcRect->Y;
        SrcW = SrcRect->Width;
        SrcH = SrcRect->Height;
    }
    else
    {
        SrcStartX = 0;
        SrcStartY = 0;
        SrcW = ImageWidth;
        SrcH = ImageHeight;
    }

    if( DesRect )
    {
        DesStartX= DesRect->X;
        DesStartY= DesRect->Y;
        DesW = DesRect->Width;
        DesH = DesRect->Height;
    }
    else
    {
        DesStartX= 0;
        DesStartY= 0;
        DesW = ScreenWidth;
        DesH = ScreenHeight;
    }

    if(SrcStartX<0)
    {
        SrcW += SrcStartX;
		DesStartX -= SrcStartX;
        SrcStartX= 0;		
    }

    if(SrcStartY<0)
    {
        SrcH += SrcStartY;
		DesStartY -= SrcStartY;
        SrcStartY= 0;		
    }

    if(DesStartX<0)
    {
        DesW += DesStartX;
		SrcStartX -= DesStartX;
        DesStartX= 0;		
    }

    if(DesStartY<0)
    {
        DesH += DesStartY;
		SrcStartY -= DesStartY;
        DesStartY= 0;		
    }


    if( SrcStartX+SrcW > ImageWidth) SrcW = ImageWidth - SrcStartX;
    if( SrcStartY+SrcH > ImageHeight) SrcH= ImageHeight- SrcStartY;
	
    if( DesStartX+DesW > ScreenWidth) DesW = ScreenWidth - DesStartX;
    if( DesStartY+DesH > ScreenHeight) DesH= ScreenHeight- DesStartY;

    if( SrcW<DesW ) DrawW = SrcW;
    else DrawW = DesW;

    if( SrcH<DesH ) DrawH = SrcH;
    else DrawH = DesH;

	
    ImageDataPoint += 0X14;
    LengthTotalReadData = 0X14;	
    SrcEndX = SrcStartX + DrawW;
    SrcEndY = SrcStartY + DrawH;
    DesEndX = DesStartX + DrawW;
    DesEndY = DesStartY + DrawH;

	SrcStartX = ImageWidth - SrcStartX - DrawW;
	SrcEndX = SrcStartX + DrawW;
	
    while(1)
    {
        TempReadDataBuff1 = (iU16 *)(ImageDataPoint);   
        CurrentImageX = TempReadDataBuff1[0];
        CurrentImageY = TempReadDataBuff1[1];
        CurrentImageWidth = TempReadDataBuff1[2];

        ImageDataPoint += 6;
        CopyDataEntry = (iU32)ImageDataPoint;
        LengthCopyColor =  CurrentImageWidth;

        LengthTotalReadData += 6 + (CurrentImageWidth<<1);
        ImageDataPoint += ( CurrentImageWidth<<1 );
		
        if( CurrentImageY>= SrcEndY) break;
		
        // 图片当前数据段X开始坐标在图片绘制区域右边或者Y坐标在图片绘制区域上方            
        if( (CurrentImageY < SrcStartY) || (CurrentImageX >= SrcEndX))
        {
            if( LengthTotalReadData> (LengthIamgeData-3) ) break;
            continue;
        }
        
		CurrentImageX = ImageWidth - CurrentImageX - CurrentImageWidth;

        // 当前数据段长度超出图片绘制区域右边
        if( (CurrentImageX+CurrentImageWidth) >SrcEndX )
        {
            LengthCopyColor -= ((CurrentImageX+CurrentImageWidth-SrcEndX));
        }

        // 图片当前数据段X开始坐标在图片绘制区域左边            
        if( CurrentImageX<SrcStartX )
        {
            LengthCopyColor -= ((SrcStartX - CurrentImageX));
//            CopyDataEntry += ((SrcStartX - CurrentImageX)<<1);
            if( LengthCopyColor<=0 ) 
            {
                if( LengthTotalReadData> (LengthIamgeData-3) ) break;
                continue;
            }
        }

         // 计算当前数据段在屏幕上开始显示的位置
         TempDrawX = DesStartX;
         TempDrawY = DesStartY + CurrentImageY - SrcStartY;

        // 图片当前数据段X开始坐标在图片开始绘制绘制区域右边
        // 需要移动屏幕开始绘制区域
        if( CurrentImageX>SrcStartX )
        {
            TempDrawX += ((CurrentImageX-SrcStartX));
        }

        if( TempDrawX >= DesEndX)
        {
            if( LengthTotalReadData> (LengthIamgeData-3) ) break;
            continue;
        }
        
        TempReadDataBuff = (iU16*)CopyDataEntry;
		LcdBufferPostion = TempDrawY*ScreenWidth+TempDrawX + LengthCopyColor-1;
		for( iTemp = 0; iTemp <LengthCopyColor; iTemp++ )
		{
			 ScreenBuffer[LcdBufferPostion--] = TempReadDataBuff[iTemp];
		}

        if( LengthTotalReadData> (LengthIamgeData-3) ) break;
		
     }
	
        if(TotalAlphaNumber>0)
        {
            iS32 CurrAlphaNum = 0;
            iS32 CurrBuffPos = LengthIamgeData;		
            iS16 AlphaPosX = 0;
            iS16 AlphaPosY = 0;
            iALPHA AlphaNumber = 0;
            iS16 AlphaColor = 0;
            iS32 LcdBuffPos = 0;
            iU32 TempHeight = ImageHeight -1;
            iU32 TempWidth = ImageWidth-1;
			
            ImageDataPoint = (iU8*)((iU32)ImageData+CurrBuffPos + 12);
			
            for( CurrAlphaNum=0; CurrAlphaNum<TotalAlphaNumber;CurrAlphaNum++)
            {
                TempReadDataBuff1 = (iU16 *)(ImageDataPoint);   
                AlphaPosX = TempReadDataBuff1[0];
                AlphaPosY = TempReadDataBuff1[1];
                AlphaNumber = (iALPHA)TempReadDataBuff1[2];
                AlphaColor = TempReadDataBuff1[3];
                ImageDataPoint += 8;
				
				AlphaPosX = TempWidth - AlphaPosX ;

                if(AlphaPosY>=SrcEndY) break;
                if( AlphaPosX>=SrcEndX || AlphaPosX<SrcStartX || AlphaPosY<SrcStartY) continue;				

                LcdBuffPos = (AlphaPosY+DesStartY)*ScreenWidth+AlphaPosX-SrcStartX+DesStartX;
                ScreenBuffer[LcdBuffPos] = i51KitG2GetNewColorEx(ScreenBuffer[LcdBuffPos], AlphaColor, AlphaNumber);
            }
        }		
    return iTRUE;
	
}


static iBOOL i51KitG2FlipImageEx(iU32 ImageData, iRECT * SrcRect, iRECT * DesRect)
{
    iU8 * ImageDataPoint = iNULL;
    iU32 ImageWidth = 0;
    iU32 ImageHeight = 0;
    iU16 * TempReadDataBuff = iNULL;
    iU32 CopyDataEntry = 0;
    // 累计读取到的数据长度
    iU32  LengthTotalReadData = 0;
    // 一次需要拷贝到缓冲区的颜色数量
    iS32 LengthCopyColor = 0;
    // 图片文件总长度
    iU32 LengthIamgeData = 0;

    // 当前数据段在屏幕上的开始绘制位置
    iS16 TempDrawX = 0;
    iS16 TempDrawY = 0;
    // 图片绘制的左上角和右下角坐标  
    iS16 SrcStartX = 0;
    iS16 SrcStartY = 0;
    iS16 SrcEndX = 0;
    iS16 SrcEndY = 0;
    // 屏幕绘制的左上角和右下角坐标  
    iS16 DesStartX = 0;
    iS16 DesStartY = 0;
    iS16 DesEndX = 0;
    iS16 DesEndY = 0;
    iS32 iTemp = 0;

    // ImageX
    iS32 CurrentImageX = 0;
    iS32 CurrentImageY = 0;
    iS32 CurrentImageWidth = 0;


    iU32 LcdBufferPostion = 0;

    // 读到的一段数据首地址
    iU16 * TempReadDataBuff1 = iNULL;

    iU16 * ScreenBuffer = iNULL;
    iU32 * LcdInfoArr = iNULL;
    iS16 SrcW, SrcH, DesW, DesH;
    iS16 DrawW, DrawH;
    iU16 ScreenWidth = 0;
    iU16 ScreenHeight= 0;
    iU16 CurrColor = 0;
    iU16 TotalAlphaNumber = 0;

    ImageDataPoint = (iU8 *)ImageData;
    if( iNULL==ImageDataPoint ) return iFALSE;
	
    if( ImageDataPoint[0X11]!=0X02 )
    {
    	iLog("文件格式错误，请重新生成图片文件!");
    	return iFALSE;
    }

    
    if( i51AdeMmiGetScreenScale ( &ScreenWidth , &ScreenHeight ) == 0 )
    {
        return iFALSE;
    }

    LcdInfoArr =  i51AdeMmiGetPanel();
    if( iNULL == LcdInfoArr )
    {
        return iFALSE;
    }
  
    ScreenBuffer = (iU16 *) LcdInfoArr[1] ;
	
ImageWidth = ImageDataPoint[9]|(ImageDataPoint[10]<<8);
ImageHeight = ImageDataPoint[11]|(ImageDataPoint[12]<<8);
LengthIamgeData = ImageDataPoint[5]|(ImageDataPoint[6]<<8)|(ImageDataPoint[7]<<16)|(ImageDataPoint[8]<<24);
TotalAlphaNumber = ImageDataPoint[18]|(ImageDataPoint[19]<<8);

    if(SrcRect)
    {
        SrcStartX = SrcRect->X;
        SrcStartY = SrcRect->Y;
        SrcW = SrcRect->Width;
        SrcH = SrcRect->Height;
    }
    else
    {
        SrcStartX = 0;
        SrcStartY = 0;
        SrcW = ImageWidth;
        SrcH = ImageHeight;
    }

    if( DesRect )
    {
        DesStartX= DesRect->X;
        DesStartY= DesRect->Y;
        DesW = DesRect->Width;
        DesH = DesRect->Height;
    }
    else
    {
        DesStartX= 0;
        DesStartY= 0;
        DesW = ScreenWidth;
        DesH = ScreenHeight;
    }

    if(SrcStartX<0)
    {
        SrcW += SrcStartX;
		DesStartX -= SrcStartX;
        SrcStartX= 0;		
    }

    if(SrcStartY<0)
    {
        SrcH += SrcStartY;
		DesStartY -= SrcStartY;
        SrcStartY= 0;		
    }

    if(DesStartX<0)
    {
        DesW += DesStartX;
		SrcStartX -= DesStartX;
        DesStartX= 0;		
    }

    if(DesStartY<0)
    {
        DesH += DesStartY;
		SrcStartY -= DesStartY;
        DesStartY= 0;		
    }


    if( SrcStartX+SrcW > ImageWidth) SrcW = ImageWidth - SrcStartX;
    if( SrcStartY+SrcH > ImageHeight) SrcH= ImageHeight- SrcStartY;
	
    if( DesStartX+DesW > ScreenWidth) DesW = ScreenWidth - DesStartX;
    if( DesStartY+DesH > ScreenHeight) DesH= ScreenHeight- DesStartY;

    if( SrcW<DesW ) DrawW = SrcW;
    else DrawW = DesW;

    if( SrcH<DesH ) DrawH = SrcH;
    else DrawH = DesH;

	
    ImageDataPoint += 0X14;
    LengthTotalReadData = 0X14;	
    SrcEndX = SrcStartX + DrawW;
    SrcEndY = SrcStartY + DrawH;
    DesEndX = DesStartX + DrawW;
    DesEndY = DesStartY + DrawH;

	SrcStartY = ImageHeight - SrcStartY - DrawH;
	SrcEndY = SrcStartY+ DrawH;
	
    while(1)
    {
        TempReadDataBuff1 = (iU16 *)(ImageDataPoint);   
        CurrentImageX = TempReadDataBuff1[0];
        CurrentImageY = TempReadDataBuff1[1];
        CurrentImageWidth = TempReadDataBuff1[2];

        ImageDataPoint += 6;
        CopyDataEntry = (iU32)ImageDataPoint;
        LengthCopyColor =  CurrentImageWidth;

        LengthTotalReadData += 6 + (CurrentImageWidth<<1);
        ImageDataPoint += ( CurrentImageWidth<<1 );
		
		CurrentImageY = ImageHeight -1 - CurrentImageY ;

        // 图片当前数据段X开始坐标在图片绘制区域右边或者Y坐标在图片绘制区域上方            
        if( (CurrentImageY < SrcStartY) || (CurrentImageY >= SrcEndY) || (CurrentImageX >= SrcEndX))
        {
            if( LengthTotalReadData> (LengthIamgeData-3) ) break;
            continue;
        }
        
        // 当前数据段长度超出图片绘制区域右边
        if( (CurrentImageX+CurrentImageWidth) >SrcEndX )
        {
            LengthCopyColor -= ((CurrentImageX+CurrentImageWidth-SrcEndX));
        }


        // 图片当前数据段X开始坐标在图片绘制区域左边            
        if( CurrentImageX<SrcStartX )
        {
            LengthCopyColor -= ((SrcStartX - CurrentImageX));
            CopyDataEntry += ((SrcStartX - CurrentImageX)<<1);
            if( LengthCopyColor<=0 ) 
            {
                if( LengthTotalReadData> (LengthIamgeData-3) ) break;
                continue;
            }
        }

         // 计算当前数据段在屏幕上开始显示的位置
         TempDrawX = DesStartX;
         TempDrawY = DesStartY + CurrentImageY - SrcStartY;

        // 图片当前数据段X开始坐标在图片开始绘制绘制区域右边
        // 需要移动屏幕开始绘制区域
        if( CurrentImageX>SrcStartX )
        {
            TempDrawX += ((CurrentImageX-SrcStartX));
        }

        if( TempDrawX >= DesEndX)
        {
            if( LengthTotalReadData> (LengthIamgeData-3) ) break;
            continue;
        }
        
        if( TempDrawY > DesEndY ) break;
		TempReadDataBuff = (iU16*)CopyDataEntry;
		i51AdeStdMemcpy16( &ScreenBuffer[ TempDrawY*ScreenWidth+TempDrawX], TempReadDataBuff, (LengthCopyColor<<1));

        if( LengthTotalReadData> (LengthIamgeData-3) ) break;
		
     }
	
        if(TotalAlphaNumber>0)
        {
            iS32 CurrAlphaNum = 0;
            iS32 CurrBuffPos = LengthIamgeData;		
            iS16 AlphaPosX = 0;
            iS16 AlphaPosY = 0;
            iALPHA AlphaNumber = 0;
            iS16 AlphaColor = 0;
            iS32 LcdBuffPos = 0;
            iU32 TempHeight = ImageHeight -1;
            iU32 TempWidth = ImageWidth-1;
			
            ImageDataPoint = (iU8*)((iU32)ImageData+CurrBuffPos + 12);
			
            for( CurrAlphaNum=0; CurrAlphaNum<TotalAlphaNumber;CurrAlphaNum++)
            {
                TempReadDataBuff1 = (iU16 *)(ImageDataPoint);   
                AlphaPosX = TempReadDataBuff1[0];
                AlphaPosY = TempReadDataBuff1[1];
                AlphaNumber = (iALPHA)TempReadDataBuff1[2];
                AlphaColor = TempReadDataBuff1[3];
                ImageDataPoint += 8;
				
                if(AlphaPosY>=SrcEndY) break;
                if( AlphaPosX>=SrcEndX || AlphaPosX<SrcStartX || AlphaPosY<SrcStartY) continue;				
				AlphaPosY = TempHeight - AlphaPosY ;

                LcdBuffPos = (AlphaPosY+DesStartY)*ScreenWidth+AlphaPosX-SrcStartX+DesStartX;
                ScreenBuffer[LcdBuffPos] = i51KitG2GetNewColorEx(ScreenBuffer[LcdBuffPos], AlphaColor, AlphaNumber);
            }
        }		
    return iTRUE;
	
}

static iS32 i51KitG2SinEx( iS32 Angle)
{
    //iS32 Angle: 需要计算的角度值
    // notes : 返回角度Angle的正玄放大16384倍(相当于左移14位)的值
    iS32 ActualAngle = 0;
    iS32 Ret = 0;

    ActualAngle = Angle%360;
    if( ActualAngle<0) ActualAngle = 360+ActualAngle;

    if(ActualAngle<=90) Ret = SinData[ActualAngle];
    else if ( ActualAngle<=180) Ret = SinData[180-ActualAngle];
    else if ( ActualAngle<=270) Ret = -SinData[ActualAngle-180];
    else Ret = -SinData[360-ActualAngle];

    return Ret;
	
}

static iS32 i51KitG2CosEx( iS32 Angle)
{
    //iS32 Angle: 需要计算的角度值
    // notes : 返回角度Angle的正玄放大10000倍的值
    return i51KitG2Sin(Angle+90);
}

static iBOOL i51KitG2RotatePointEx(iS32 SrcX, iS32 SrcY, iS32 RotateCenterX,  iS32 RotateCenterY ,iS32 * DesX, iS32 * DesY, iS32 Angle)
{
    // A:原始点与原点之间相对X轴正方向的夹角
    // B:原始点和目的点与原点之间的夹角,即Angle
    // C:目的点与原点之间相对X轴正方向的夹角

    iS32 SinA = 0, SinB = 0, SinC = 0;
    iS32 CosA = 0, CosB = 0, CosC = 0;
    iS32 CenterX = RotateCenterX;
    iS32 CenterY = RotateCenterY;

    CosA = SrcX - CenterX;
    SinA = SrcY-CenterY;

    Angle = -Angle;
    SinB = i51KitG2SinEx(Angle);
    CosB = i51KitG2CosEx(Angle);

    SinC = SinA*CosB - CosA*SinB;
    CosC = CosA*CosB + SinA*SinB;

    *DesY = ((SinC+(CenterY<<14))>>14);
    *DesX = ((CosC+(CenterX<<14))>>14);

//    *DesX = (((SrcX-CenterX)*CosB+(SrcY-CenterY)*SinB)>>14) + CenterX;
//    *DesY = (((CenterX-SrcX)*SinB+(SrcY-CenterY)*CosB)>>14) + CenterY;
    return iTRUE;
	
}

static iBOOL i51KitG2GetGirthPointsEx(iRECT SrcRect, iRECT DesRect, iS16 PointS[][2], iS32 SrcMoveX, iS32 SrcMoveY,
	iS32 RotateCenterX,  iS32 RotateCenterY ,iU32 RotateType, iS32 Angle )
{
     // iRECT SrcRect : 需要变换的区域
     // iS16 PointS[][2] : 变换后周边的点新坐标
     // iS32 Angle :旋转角度
#define IMAGE_STRETCH 0
    // 旋转区域左上角坐标
    iS32 UpLeftX = 0, UpLeftY = 0;
    // 当前旋转点坐标	 
    iS32 SrcX= 0, SrcY = 0;
    iS32 SrcX1= 0, SrcY1 = 0;
    // 当前点旋转后坐标
    iS32 DesX = 0, DesY = 0;
    // 待旋转区域的长宽	
    iS32 SrcW, SrcH;
    iS32 Location = 0;
    iS32 EdgePoints[4][2] = {0};
    iS32 DesStartX = 0, DesStartY = 0;
    iS32 DesEndX = 0, DesEndY = 0;
#if IMAGE_STRETCH
    iS32 DesPoints[5][2] = {0};
    iS32 OldX = 0, OldY = 0;
    iS32 iTemp = 0, Min = 0, Num = 0 , Temp = 0;
#endif

//    UpLeftX = SrcRect.X + SrcMoveX;
//    UpLeftY = SrcRect.Y + SrcMoveY;
    UpLeftX = SrcMoveX;
    UpLeftY = SrcMoveY;
    SrcW = SrcRect.Width;
    SrcH = SrcRect.Height;

    DesStartX = DesRect.X;
    DesStartY = DesRect.Y;
    DesEndX = DesRect.Width + DesStartX;
    DesEndY = DesRect.Height + DesStartY;
	
    i51KitG2RotatePointEx(UpLeftX, UpLeftY, RotateCenterX, RotateCenterY, &EdgePoints[0][0], &EdgePoints[0][1], Angle);
    i51KitG2RotatePointEx(UpLeftX+SrcW, UpLeftY, RotateCenterX, RotateCenterY, &EdgePoints[1][0], &EdgePoints[1][1], Angle);
    i51KitG2RotatePointEx(UpLeftX, UpLeftY+SrcH, RotateCenterX, RotateCenterY, &EdgePoints[2][0], &EdgePoints[2][1], Angle);
    i51KitG2RotatePointEx(UpLeftX+SrcW, UpLeftY+SrcH, RotateCenterX, RotateCenterY, &EdgePoints[3][0], &EdgePoints[3][1], Angle);

    if( EdgePoints[0][0]<DesStartX&&EdgePoints[1][0]<DesStartX&&EdgePoints[2][0]<DesStartX&&EdgePoints[3][0]<DesStartX) return iFALSE;
    if( EdgePoints[0][1]<DesStartY&&EdgePoints[1][1]<DesStartY&&EdgePoints[2][1]<DesStartY&&EdgePoints[3][1]<DesStartY) return iFALSE;
    if( EdgePoints[0][0]>DesEndX&&EdgePoints[1][0]>DesEndX&&EdgePoints[2][0]>DesEndX&&EdgePoints[3][0]>DesEndX) return iFALSE;
    if( EdgePoints[0][1]>DesEndY&&EdgePoints[1][1]>DesEndY&&EdgePoints[2][1]>DesEndY&&EdgePoints[3][1]>DesEndY) return iFALSE;

    SrcX = UpLeftX;
    SrcY = UpLeftY;

    for ( Location = 0; Location <SrcW; Location++)
    {
        i51KitG2RotatePointEx(SrcX, SrcY, RotateCenterX, RotateCenterY, &DesX, &DesY, Angle);
        PointS[Location][0] = DesX-SrcX;
        PointS[Location][1] = DesY-SrcY;
        SrcX++;
    }	

    SrcH = SrcW + SrcH;
    SrcX = UpLeftX;
    SrcY = UpLeftY;
    i51KitG2RotatePointEx(SrcX, SrcY, RotateCenterX,  RotateCenterY , &DesX, &DesY, Angle);
    SrcX1 = DesX;
    SrcY1 = DesY;

    for ( Location = SrcW; Location <SrcH; Location++)
    {
        i51KitG2RotatePointEx(SrcX, SrcY, RotateCenterX, RotateCenterY,&DesX, &DesY, Angle);
        PointS[Location][0] = DesX-SrcX1;
        PointS[Location][1] = DesY-SrcY1 ;
        SrcY++;
        SrcY1++;		
    }	

    for ( Location = 0; Location <SrcW; Location++)
    {
        PointS[Location][0] += SrcMoveX;
        PointS[Location][1] += SrcMoveY;
    }	

    return iTRUE;
	
}


static iBOOL RotateRamLinearImageEx  ( iU32 ImageData , iRECT* DesRect , iRECT* SrcRect , iS16 ChangeData[][2])
{
    // author : Otto
    // since : Aug 15 2011
    // (C) 2011 , PKIG Tech. Co., Ltd.

    // iU32 ImageID : 绘制图像的ID号
    // iRECT* DesRect : 绘制图像到屏幕上的区域
    // iRECT* SrcRect : 绘制图像的区域
    // iS32 Angle : 旋转角度
  
    // notes : 将图像容器内ID号为ImageID的图片SrcRect区域
    // 按照Angle的角度旋转后绘制到屏幕的DesRect区域
    // 图片的数据在内存中有备份，需要的数据从内存中读取
    // 这张图片的编码方式为I51KITG2_DECODE_LINEAR     

    // 图片宽度和高度
    iU32 ImageWidth = 0;
    iU32 ImageHeight = 0;
    // 图片数据文件开始地址
    iU32 DataEntry = 0;
    // 读取到的数据长度
//    iU32  LengthReadData = 0;
    // 累计读取到的数据长度
    iU32  LengthTotalReadData = 0;
    // 需要读取的数据长度
//    iU32  LengthNeedReadData = 0;
    // 一次需要拷贝到显存的数据长度  
    iS32 LengthCopyData = 0;
    // 图片文件总长度
    iU32 LengthIamgeData = 0;
//    iU32 LcdBufferPostion = 0;
  
//    iU16 iTempX = 0;
//    iU16 iTempY = 0;
    // 图片绘制的左上角和右下角坐标  
    iS16 SrcStartX = 0;
    iS16 SrcStartY = 0;
    iS16 SrcEndX = 0;
    iS16 SrcEndY = 0;
    // 绘制到屏幕上的左上角和右下角坐标  
    iS16 DrawStartX = 0;
    iS16 DrawStartY = 0;
    iS16 DrawEndX = 0;
    iS16 DrawEndY = 0;
    iU16 iTemp = 0;

    // ImageX
    iS32 CurrentImageX = 0;
    iS32 CurrentImageY = 0;
    iS32 CurrentImageWidth = 0;

    // 当前点的原始坐标	
    iS32 CurrX = 0, CurrY = 0;
    // 当前点选择后的坐标
    iS32 RotateX = 0, RotateY = 0;
    // 颜色buff
    iU16 * ColorBuff = iNULL;
    iS32 DrawW = 0;
    iS32 DrawH = 0;
     iU16* TempReadDataBuff1 = iNULL;
    iS32 AddsDirectionY = 0;
    iS32 BuffPos = 0;
    iCOLOR TempColor = 0;
    // 带Alpha信息的数据点个数
    iU16 TotalAlphaNumber = 0;
    // 从文件中读取的数据长度	
    iU32 LengthNeedReadData = 0;
    // 开闭空间读取文件
    iU8  * ImageDataEntry = iNULL;
    // 从容器中读到的文件总长度
    iU32 LengthTotalRead = 0;
    // 文件操作函数返回值
    iU8   FlagReturn = iFALSE;
    iU16 * ScreenBuffer = iNULL;
    iU32 * LcdInfoArr = iNULL;
    iU16 ScreenWidth = 0;
    iU16 ScreenHeight= 0;
	
    if( i51AdeMmiGetScreenScale ( &ScreenWidth , &ScreenHeight ) == 0 )
    {
        return iFALSE;
    }

    LcdInfoArr =  i51AdeMmiGetPanel();
    if( iNULL == LcdInfoArr )
    {
        return iFALSE;
    }
  
    ScreenBuffer = (iU16 *) LcdInfoArr[1] ;	
    DataEntry = ImageData;
    ImageDataEntry = (iU8*)DataEntry;
	ImageWidth = ImageDataEntry[9]|(ImageDataEntry[10]<<8);
	ImageHeight = ImageDataEntry[11]|(ImageDataEntry[12]<<8);
	LengthIamgeData = ImageDataEntry[5]|(ImageDataEntry[6]<<8)|(ImageDataEntry[7]<<16)|(ImageDataEntry[8]<<24);
	TotalAlphaNumber = ImageDataEntry[18]|(ImageDataEntry[19]<<8);

    DrawW = SrcRect->Width;
    DrawH = SrcRect->Height;
    SrcStartX = SrcRect->X;
    SrcStartY = SrcRect->Y;
    SrcEndX = SrcStartX +DrawW;
    SrcEndY = SrcStartY + DrawH;  
	
    DrawStartX = DesRect->X;
    DrawStartY = DesRect->Y;
    DrawEndX = DrawStartX + DesRect->Width - 1;
    DrawEndY = DrawStartY + DesRect->Height;


    DataEntry = (iU32)ImageDataEntry;
    LengthTotalReadData = 0X14;
    DataEntry += 0X14;
  
    while(1)
    {
        TempReadDataBuff1 = (iU16 *)(DataEntry);   
        CurrentImageX = TempReadDataBuff1[0];
        CurrentImageY = TempReadDataBuff1[1];
        CurrentImageWidth = TempReadDataBuff1[2];
        
        DataEntry += 6;
        LengthTotalReadData += 6 + (CurrentImageWidth<<1);

        if( (CurrentImageY < SrcStartY) || (CurrentImageX >= SrcEndX) )
        {
            DataEntry += ( CurrentImageWidth<<1 );
            continue;
        }
        
        if( CurrentImageY >= SrcEndY ) break;

        ColorBuff = (iU16*)DataEntry;
        DataEntry += ( CurrentImageWidth<<1 );
        LengthCopyData = CurrentImageWidth;
        
        if( CurrentImageX<SrcStartX )
        {
            LengthCopyData -= SrcStartX - CurrentImageX;
            CurrentImageX = SrcStartX;
            ColorBuff = &ColorBuff[SrcStartX - CurrentImageX];
            if( LengthCopyData<0 ) continue;
        }
        
        if( (CurrentImageX+LengthCopyData) > SrcEndX )
        {
            LengthCopyData = SrcEndX - CurrentImageX;
        }

        AddsDirectionY = CurrentImageY-SrcStartY+DrawW;
        CurrY = CurrentImageY - SrcStartY;
        CurrX = CurrentImageX - SrcStartX;
        for( iTemp=0; iTemp<LengthCopyData; iTemp++ )
        {
            RotateX = CurrX + ChangeData[CurrX][0] + ChangeData[AddsDirectionY][0] ;
            RotateY = CurrY + ChangeData[CurrX][1] + ChangeData[AddsDirectionY][1] ;
            CurrX++;
            if( RotateY<DrawStartY|| RotateY>=DrawEndY ||RotateX<DrawStartX||RotateX>=DrawEndX) continue;
            BuffPos = RotateY*ScreenWidth+ RotateX;
            ScreenBuffer[BuffPos] = ColorBuff[iTemp];
            ScreenBuffer[BuffPos+1] = ColorBuff[iTemp];
        }
        
        if( (CurrentImageY == SrcEndY) && ((CurrentImageX+LengthCopyData) >= SrcEndX)) break;
        
        if( LengthTotalReadData> (LengthIamgeData-3) ) break;
        
    }
	
    if(TotalAlphaNumber>0)
    {
        iS32 CurrAlphaNum = 0;
        iS32 CurrBuffPos = LengthIamgeData;		
        iS16 AlphaPosX = 0;
        iS16 AlphaPosY = 0;
        iALPHA AlphaNumber = 0;
        iS16 AlphaColor = 0;
        iS32 LcdBuffPos = 0;
        iU32 TempHeight = ImageHeight -1;
        iU32 TempWidth = ImageWidth-1;
		
        DataEntry = (iU32)ImageDataEntry+CurrBuffPos + 12;
		
        for( CurrAlphaNum=0; CurrAlphaNum<TotalAlphaNumber;CurrAlphaNum++)
        {
            TempReadDataBuff1 = (iU16 *)(DataEntry);   
            AlphaPosX = TempReadDataBuff1[0];
            AlphaPosY = TempReadDataBuff1[1];
            AlphaNumber = (iALPHA)TempReadDataBuff1[2];
            AlphaColor = TempReadDataBuff1[3];
            DataEntry += 8;
			
            if(AlphaPosY>=SrcEndY) break;
            if( AlphaPosX>=SrcEndX || AlphaPosX<SrcStartX || AlphaPosY<SrcStartY) continue;				

            CurrX = AlphaPosX - SrcStartX;
            CurrY = AlphaPosY - SrcStartY;
            AddsDirectionY = AlphaPosY-SrcStartY+DrawW;
            RotateX =  CurrX + ChangeData[CurrX][0] + ChangeData[AddsDirectionY][0] ;
            RotateY = AlphaPosY - SrcStartY + ChangeData[CurrX][1] + ChangeData[AddsDirectionY][1] ;
            if( RotateY<DrawStartY|| RotateY>=DrawEndY ||RotateX<DrawStartX||RotateX>=DrawEndX) continue;

            LcdBuffPos =RotateY*ScreenWidth+RotateX;
            ScreenBuffer[LcdBuffPos] = i51KitG2GetNewColorEx(ScreenBuffer[LcdBuffPos], AlphaColor, AlphaNumber);
        }
    }

    return iTRUE;

}

static iBOOL i51KitG2RotateImageEx(iU32 ImageData, iRECT* DesRect , iRECT* SrcRect, 
	iS32 SrcMoveX , iS32 SrcMoveY, iS32 RotateCenterX , iS32 RotateCenterY , iU32 RotateType, iS32 Angle )
{
	// author : Otto
	// since : Aug 29 2011
	// (C) 2011 , PKIG Tech. Co., Ltd.

	//ContainerHandle	待绘制图片所在容器的句柄
	//ImageID	图片ID
	//DesRect	图片绘制的目的区域
	//ImageHeight	图片绘制的裁剪区
	//SrcMoveX	图片旋转前平移至点（SrcMoveX,SrcMoveY）
	//SrcMoveY	图片旋转前平移至点（SrcMoveX,SrcMoveY）
	//RotateCenterX 图片旋转中心X坐标
	//RotateCenterY 图片旋转中心Y坐标
	//RotateType	旋转类型
	//Angle 旋转角度
	
	// notes : 按顺时针方向旋转一定角度后绘制ImageContainer容器内编号为ImageID的图像
	// SrcRect 为空时在点(DesRect->X, DesRect->Y)处开始绘制整个图片
	// DesRect 为空则绘制从屏幕左上角开始

#define ROTATE_MAX_GIRTH_HALF 600

	iU8 * ImageDataPoint = iNULL;
	iU32 ImageWidth = 0;
	iU32 ImageHeight = 0;
	 // 图片编码方式
	iU8   DecodeType = 0;
	iRECT ImageRect;
	iRECT DrawDesRect;
	iRECT DrawSrcRect;
	iU8 Ret = iFALSE;
	iS16 * GirthPoint = iNULL;
    iU16 TotalAlphaNumber = 0;
    iU32 LengthIamgeData = 0;

    ImageDataPoint = (iU8 *)ImageData;
    if( iNULL==ImageDataPoint ) return iFALSE;
	
    if( ImageDataPoint[0X11]!=0X02 )
    {
    	iLog("文件格式错误，请重新生成图片文件!");
    	return iFALSE;
    }

	ImageWidth = ImageDataPoint[9]|(ImageDataPoint[10]<<8);
	ImageHeight = ImageDataPoint[11]|(ImageDataPoint[12]<<8);
	LengthIamgeData = ImageDataPoint[5]|(ImageDataPoint[6]<<8)|(ImageDataPoint[7]<<16)|(ImageDataPoint[8]<<24);
	TotalAlphaNumber = ImageDataPoint[18]|(ImageDataPoint[19]<<8);

   if( iNULL==SrcRect )
    {
        DrawSrcRect.X = 0;
        DrawSrcRect.Y = 0;
        DrawSrcRect.Width = ImageWidth;
        DrawSrcRect.Height = ImageHeight;
    }
    else
    {
        DrawSrcRect.X = SrcRect->X;
        DrawSrcRect.Y = SrcRect->Y;
        DrawSrcRect.Width = SrcRect->Width;
        DrawSrcRect.Height = SrcRect->Height;
    }

    if( iNULL==DesRect )
    {
        DrawDesRect.X = 0;
        DrawDesRect.Y = 0;
        DrawDesRect.Width = ScreenWidth;
        DrawDesRect.Height = ScreenHeight;
    }
    else
    {
        DrawDesRect.X = DesRect->X;
        DrawDesRect.Y = DesRect->Y;
        DrawDesRect.Width = DesRect->Width;
        DrawDesRect.Height = DesRect->Height;
    }
    

    ImageRect.X = 0;
    ImageRect.Y = 0;
    ImageRect.Width= ImageWidth;
    ImageRect.Height= ImageHeight;

    if(DrawSrcRect.X<0) { DrawSrcRect.Width+= DrawSrcRect.X; DrawSrcRect.X = 0;}
//    else if( DrawSrcRect.X>=ScreenWidth|| DrawSrcRect.X>=ImageRect.Width) return iTRUE;
    else if( DrawSrcRect.X>=ImageRect.Width) return iTRUE;
//    if( (DrawSrcRect.X+DrawSrcRect.Width) > ScreenWidth ) DrawSrcRect.Width = ScreenWidth -DrawSrcRect.X;
    if( (DrawSrcRect.X+DrawSrcRect.Width) > ImageRect.Width ) DrawSrcRect.Width = ImageRect.Width -DrawSrcRect.X;

    if(DrawSrcRect.Y<0) { DrawSrcRect.Height+= DrawSrcRect.Y; DrawSrcRect.Y = 0;}
    else if( DrawSrcRect.Y>=ImageRect.Height) return iTRUE;
//    else if( DrawSrcRect.Y>=ScreenHeight|| DrawSrcRect.Y>=ImageRect.Height) return iTRUE;
//    if( (DrawSrcRect.X+DrawSrcRect.Width) > ScreenWidth ) DrawSrcRect.Width = ScreenWidth -DrawSrcRect.X;
    if( (DrawSrcRect.Y+DrawSrcRect.Height) > ImageRect.Height) DrawSrcRect.Height= ImageRect.Height-DrawSrcRect.Y;

    if(DrawDesRect.X<0) { DrawDesRect.Width+= DrawDesRect.X; DrawDesRect.X = 0;}
    else if( DrawDesRect.X>=ScreenWidth) return iTRUE;
    if( (DrawDesRect.X+DrawDesRect.Width) > ScreenWidth ) DrawDesRect.Width = ScreenWidth -DrawDesRect.X;
		
    if(DrawDesRect.Y<0) { DrawDesRect.Height+= DrawDesRect.Y; DrawDesRect.Y = 0;}
    else if( DrawDesRect.Y>=ScreenHeight) return iTRUE;
    if( (DrawDesRect.Y+DrawDesRect.Height) > ScreenHeight) DrawDesRect.Height= ScreenHeight-DrawDesRect.Y;
		
    if( (DrawSrcRect.Height+DrawSrcRect.Width)>=ROTATE_MAX_GIRTH_HALF  )   return iFALSE;
	
    GirthPoint = (iS16*)(i51AdeOsMalloc((DrawSrcRect.Height+DrawSrcRect.Width)*4,__FILE__ , __LINE__ ));

    if(iNULL==GirthPoint) 
    {
        iLog("KIT - G2 : malloc rotate");
        return iFALSE;		
    }
	
    if( iNULL==i51KitG2GetGirthPoints(DrawSrcRect, DrawDesRect,(iS16(*)[2])GirthPoint, SrcMoveX, SrcMoveY, RotateCenterX, RotateCenterY,RotateType, Angle))
    {
        i51AdeOsFree(GirthPoint);
        GirthPoint = iNULL;
        return iTRUE;
    }	

	RotateRamLinearImageEx((iU32)ImageDataPoint, &(DrawDesRect),  &(DrawSrcRect), (iS16(*)[2])GirthPoint);
	i51AdeOsFree(GirthPoint);
    GirthPoint = iNULL;

	return iTRUE;
	
}

#endif

iKIT iBOOL i51KitG2SetAlpha( iALPHA Alpha )
{
    // 设置系统透明度
    AlphaSys = Alpha;
    AlphaSrc = Alpha;
    AlphaDes = 0XFF-Alpha;
	 
    return iTRUE;
}

iKIT iBOOL i51KitG2CleanScreen(iCOLOR Color, iRECT *Rect)
{
    // author : Otto
    // since : Sep 27 2011
    // (C) 2011 , PKIG Tech. Co., Ltd.

    // iCOLOR Color : 填充屏幕的颜色
    //iRECT *Rect : 填充屏幕的区域

    // Notes : 以指定的颜色清屏幕区域

    iS32 CleanX, CleanY;
    iS32 CleanW, CleanH;
    iS32 LcdBufPos = 0;
    iS32 TempX, TempY;

 #if i51KITG2_DEBUG
    iLog( "G2CleanScreen Start , %x, %x", Color, Rect) ;
#endif

    if( Rect)
    {
        CleanX = Rect->X;
        CleanY = Rect->Y;
        CleanW = Rect->Width;
        CleanH = Rect->Height;
    }
    else
    {
        CleanX = 0;
        CleanY = 0;
        CleanW = ScreenWidth;
        CleanH = ScreenHeight;
    }

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

    if( (CleanX+CleanW)>ScreenWidth) CleanW = ScreenWidth-CleanX;
    if( (CleanY+CleanH)>ScreenHeight) CleanH = ScreenHeight-CleanY;

    if(AlphaSys==i51KITG2_ALPHA_NONEED)
    {
        for( TempX=0; TempX<CleanW ; TempX++)
        {
            OneRowColorBuf[TempX] = Color;
        }
    
        CleanW = (CleanW<<1);	
        LcdBufPos = CleanY*ScreenWidth + CleanX;
        for( TempY=0; TempY<CleanH; TempY++)
        {
            i51AdeStdMemcpy16(&(LcdBuffer[LcdBufPos]), OneRowColorBuf, CleanW);
            LcdBufPos += ScreenWidth;
        }
    }
    else
    {
        for( TempY=0; TempY<CleanH; TempY++)
        {
            LcdBufPos = (CleanY+TempY)*ScreenWidth + CleanX;
            for( TempX=0; TempX<CleanW ; TempX++)
            {
                LcdBuffer[LcdBufPos] = i51KitG2GetNewColor(LcdBuffer[LcdBufPos], Color);
                LcdBufPos++;
            }
        }
    }
	
  #if i51KITG2_DEBUG
    iLog( "G2CleanScreen End , %x, %x", Color, Rect) ;
#endif
    return iTRUE;
	
}

iKIT iBOOL i51KitG2DrawPoint( iS32 X, iS32 Y, iCOLOR Color )
{

    // author : Otto
    // since : Aug 10 2011
    // (C) 2011 , PKIG Tech. Co., Ltd.

    // iS32 X : 需要绘制点的X坐标
    // iS32 Y : 需要绘制点的Y坐标
    // iCOLOR Color : 绘制点颜色

    // notes : 在坐标(X, Y)处画一个Color颜色的点

    // VP上当前绘制的元素编号
    iU16 CurrDrawID;
    // 在屏幕上绘制的坐标	
    iS32 DrawX = 0, DrawY = 0;   
    // 创建的VP实际大小	
    iRECT * VPActualRect = iNULL;
    // 创建的VP在屏幕上的左上和右下坐标
    iS32 VP_TOP_LEFT_X = 0;
    iS32 VP_TOP_LEFT_Y = 0;
    iS32 VP_LOVER_RIGHT_X = 0;
    iS32 VP_LOVER_RIGHT_Y = 0;
    // 需要绘制的VP区域	
    iRECT * VPDrawRect = iNULL;
    // 绘制区域	
    iS16 DrawRectStartX = 0;
    iS16 DrawRectStartY = 0;
    iS16 DrawRectEndX = ScreenWidth;
    iS16 DrawRectEndY = ScreenHeight;
    iU32 BuffPos = 0;

 #if i51KITG2_DEBUG
    iLog( "G2DrawPoint Start , %x, %x, %x", X, Y, Color) ;
#endif
  
    // 在虚拟面板上绘制，实际上只是进行一些赋值操作
    if( i51UseVirtualPanel && i51DrawVirtualPanel )
    {
        if( i51CurrentVpManage.TotalVPNumber == 0 ) return iFALSE;

        if( iNULL == i51CurrentVpActivity )	return iFALSE;
		
        CurrDrawID = i51CurrentVpActivity->VPCurrentElement;
        // 画板已经画满则直接返回
        if(  i51CurrentVpActivity->VpIsFull ) return iFALSE;
      
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementType = I51KITG2_ELEMENT_TYPE_POINT;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementNumber = CurrDrawID;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElemenAlpha = AlphaSys;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Point.X = X;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Point.Y = Y;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Point.Color= Color;
        i51CurrentVpActivity->VPCurrentElement =  CurrDrawID +1;
        if( (CurrDrawID+1) >= i51CurrentVpActivity->VPMaxElement )	i51CurrentVpActivity->VpIsFull = iTRUE;
        return iTRUE;
    }

    if( i51UseVirtualPanel )
    {
        VPActualRect = &(i51CurrentVpRender->VPActualRect);
        VPDrawRect = &(i51CurrentVpRender->VPDrawRect);
  
        VP_TOP_LEFT_X = VPActualRect->X;
        VP_TOP_LEFT_Y = VPActualRect->Y;
        VP_LOVER_RIGHT_X = VP_TOP_LEFT_X + VPActualRect->Width;
        VP_LOVER_RIGHT_Y = VP_TOP_LEFT_Y + VPActualRect->Height;

        // 在屏幕上的绘制区域
        DrawRectStartX = VPDrawRect->X;
        DrawRectStartY = VPDrawRect->Y;
        DrawRectEndX = DrawRectStartX + VPDrawRect->Width;
        DrawRectEndY = DrawRectStartY + VPDrawRect->Height;

        if( DrawRectStartX < 0 ) DrawRectStartX = 0;
        if( DrawRectStartY < 0 ) DrawRectStartX = 0;
        if( DrawRectEndX >= ScreenWidth ) DrawRectEndX = ScreenWidth;
        if( DrawRectEndY >= ScreenHeight) DrawRectEndY = ScreenHeight;

        // 需要绘制的点在屏幕上的坐标
        DrawX = X + VPActualRect->X;
        DrawY = Y + VPActualRect->Y;
    }
    else
    {
        DrawX = X;
        DrawY = Y;
    }
	
    if( DrawX >= DrawRectEndX) return iTRUE;
    if( DrawY >= DrawRectEndY) return iTRUE;
    if( DrawX < DrawRectStartX ) return iTRUE;
    if( DrawY < DrawRectStartX ) return iTRUE;

#if LCDBUFF_DEBUG
i51KitG2CheckLcdbuffer(ScreenWidth * DrawY + DrawX,1);
#endif
    if(AlphaSys==i51KITG2_ALPHA_NONEED)
    {
        LcdBuffer[ (ScreenWidth * DrawY + DrawX) ] = Color;
    }
    else
    {
        BuffPos = ScreenWidth * DrawY + DrawX;
        LcdBuffer[ BuffPos] = i51KitG2GetNewColor(LcdBuffer[BuffPos], Color);
    }
		

 #if i51KITG2_DEBUG
    iLog( "G2DrawPoint End , %x, %x, %x", X, Y, Color) ;
#endif

    return iTRUE;

}

iKIT iBOOL i51KitG2DrawLine( iS32 Point[][2], iCOLOR Color)
{

    // author : Otto
    // since : Aug 10 2011
    // (C) 2011 , PKIG Tech. Co., Ltd.

    // iU32 Point[][2] :
    //    Point[0][0]是需要绘制线段的起点X坐标
    //    Point[0][1]是需要绘制线段的起点Y坐标
    //    Point[1][0]是需要绘制线段的终点X坐标
    //    Point[1][1]是需要绘制线段的终点Y坐标
    // iCOLOR Color : 绘制线段的颜色

    // notes : 以颜色Color绘制一条从点(Point[0][0], Point[0][1])到点(Point[1][0], Point[1][1])的线段
    // 绘制算法使用的是对称直线生成算法，对称直线生成算法基于这样一个事实：直线以中点为界，其两边是对称的,
    // 因而可以利用这个对称性，对Bresenham算法进行改进，使得每进行一次判断就可以生成相对于直线中点的两个
    // 对称点。如此以来，直线就由两端向中间生成

    iS32 StartX = Point[0][0];
    iS32 StartY = Point[0][1];
    iS32 EndX = Point[1][0];
    iS32 EndY = Point[1][1];

    iS32 TempX1, TempY1;	
    iS32 TempX2, TempY2;	

    //起点与终点X和Y的差值
    iS32 dx = 0;
    iS32 dy = 0;
    iS32 dx2 = 0;
    iS32 dy2 = 0;

    // 决策量
    iS32 DecisionTag = 0;

    // 需要绘制次数
    iU32 HalfLength = 0;

    //当前绘制位置
    iU32 Location = 0;

    //校准Y值
    iS16 RegulationY = 1;

    // 当前绘制的元素在VP上的ID
    iU16 CurrDrawID;

    // 创建的VP实际大小	
    iRECT * VPActualRect = iNULL;
    // 创建的VP在屏幕上的左上和右下坐标
    iS32 VP_TOP_LEFT_X = 0;
    iS32 VP_TOP_LEFT_Y = 0;
    iS32 VP_LOVER_RIGHT_X = 0;
    iS32 VP_LOVER_RIGHT_Y = 0;
    // 需要绘制的VP区域	
    iRECT * VPDrawRect = iNULL;
    // 绘制区域	
    iS16 DrawRectStartX = 0;
    iS16 DrawRectStartY = 0;
    iS16 DrawRectEndX = ScreenWidth;
    iS16 DrawRectEndY = ScreenHeight;
    iU32 BuffPos = 0;
    iS32 Temp1 = 0;
	
 #if i51KITG2_DEBUG
    iLog( "G2DrawLine Start , %x, %x", Point, Color) ;
#endif

    for( Temp1=0; Temp1<2; Temp1++ )
    {
        if( Point[Temp1][0]>2000||Point[Temp1][0]<-1500
         ||Point[Temp1][1]>2000||Point[Temp1][1]<-1500		
        )
        {
 #if i51KITG2_DEBUG
    iLog( "G2DrawLine Point[%X]:%X , %x", Temp1,Point[Temp1][0],Point[Temp1][1]) ;
#endif
            return iFALSE;
        }
    }     	

    // 在虚拟面板上绘制，实际上只是进行一些赋值操作
    if( i51UseVirtualPanel && i51DrawVirtualPanel )
    {
        if( i51CurrentVpManage.TotalVPNumber == 0 ) return iFALSE;

        if( iNULL == i51CurrentVpActivity )	return iFALSE;
		
        CurrDrawID = i51CurrentVpActivity->VPCurrentElement;
        // 画板已经画满则直接返回
        if(  i51CurrentVpActivity->VpIsFull ) return iFALSE;
      
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementType = I51KITG2_ELEMENT_TYPE_LINE;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementNumber = CurrDrawID;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElemenAlpha = AlphaSys;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Line.Points[0][0] = Point[0][0];
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Line.Points[0][1] = Point[0][1];
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Line.Points[1][0] = Point[1][0];
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Line.Points[1][1] = Point[1][1];
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Line.Color= Color;
        i51CurrentVpActivity->VPCurrentElement =  CurrDrawID +1;
        if( (CurrDrawID+1) >= i51CurrentVpActivity->VPMaxElement )	i51CurrentVpActivity->VpIsFull = iTRUE;
        return iTRUE;
    }

    if( i51UseVirtualPanel )
    {
        VPActualRect = &(i51CurrentVpRender->VPActualRect);
        VPDrawRect = &(i51CurrentVpRender->VPDrawRect);
  
        VP_TOP_LEFT_X = VPActualRect->X;
        VP_TOP_LEFT_Y = VPActualRect->Y;
        VP_LOVER_RIGHT_X = VP_TOP_LEFT_X + VPActualRect->Width;
        VP_LOVER_RIGHT_Y = VP_TOP_LEFT_Y + VPActualRect->Height;

        // 在屏幕上的绘制区域
        DrawRectStartX = VPDrawRect->X;
        DrawRectStartY = VPDrawRect->Y;
        DrawRectEndX = DrawRectStartX + VPDrawRect->Width;
        DrawRectEndY = DrawRectStartY + VPDrawRect->Height;

        if( DrawRectStartX < 0 ) DrawRectStartX = 0;
        if( DrawRectStartY < 0 ) DrawRectStartX = 0;
        if( DrawRectEndX >= ScreenWidth ) DrawRectEndX = ScreenWidth;
        if( DrawRectEndY >= ScreenHeight) DrawRectEndY = ScreenHeight;

        StartX = Point[0][0] +  VPActualRect->X;
        StartY = Point[0][1] +  VPActualRect->Y;
        EndX = Point[1][0] +  VPActualRect->X;
        EndY = Point[1][1] +  VPActualRect->Y;
    }
    else
    {
        DrawRectStartX = 0;
        DrawRectStartY = 0;
        DrawRectEndX = ScreenWidth;
        DrawRectEndY = ScreenHeight;
        StartX = Point[0][0];
        StartY = Point[0][1];
        EndX = Point[1][0];
        EndY = Point[1][1];
    }

    //  如果需要绘制的线段的两端点都在屏幕外，且在屏幕同侧
    //  则绘制的线段是屏幕没有交集
    if( (StartX<0&&EndX<0)
      ||(StartX>=ScreenWidth&&EndX>=ScreenWidth)
      ||(StartY<0&&EndY<0)
      ||(StartY>=ScreenHeight&&EndY>=ScreenHeight)
      )
    {
        return iTRUE;
    }

    //如果终点在起点左边则交换他们的位置
    if ( EndX < StartX )
    {
        EndX += StartX;
        StartX = EndX - StartX;
        EndX = EndX - StartX;
  
        EndY += StartY;
        StartY = EndY - StartY;
        EndY = EndY - StartY;
    }

    //计算起点与终点X和Y的差值
    dx = EndX - StartX;
    if( EndY > StartY )
    {
        dy = EndY - StartY;
        RegulationY = 1;
    }
    else
    {
        dy = StartY - EndY;
        RegulationY = -1;
    }

    //起点与终点X和Y的差值放大一倍
    dx2 = dx << 1;
    dy2 = dy << 1;
  
    //决策量赋初值
    if( dx>=dy){
        DecisionTag = dy2 - dx;
        HalfLength = (dx+1) >> 1;
    } else{
        DecisionTag = dy - dx2;
        HalfLength = (dy+1) >> 1;
    }

    for ( Location = 0; Location <= HalfLength; Location++)
    {
        TempX1 = StartX;
        TempY1 = StartY;
        TempX2 = EndX;
        TempY2 = EndY;
		
         if( TempX1>=DrawRectStartX && TempX1<DrawRectEndX && TempY1>=DrawRectStartY&& TempY1<DrawRectEndY )
        {
#if LCDBUFF_DEBUG
i51KitG2CheckLcdbuffer(ScreenWidth*TempY1 + TempX1,1);
#endif
            if(AlphaSys==i51KITG2_ALPHA_NONEED)
            {
                LcdBuffer[ (ScreenWidth*TempY1 + TempX1)] = Color;
            }
            else
            {
                BuffPos = ScreenWidth*TempY1 + TempX1;
                LcdBuffer[BuffPos] = i51KitG2GetNewColor(LcdBuffer[BuffPos], Color);
            }
        }

         if( TempX2>=DrawRectStartX && TempX2<DrawRectEndX && TempY2>=DrawRectStartY&& TempY2<DrawRectEndY )
        {
#if LCDBUFF_DEBUG
i51KitG2CheckLcdbuffer(ScreenWidth*TempY2 + TempX2,1);
#endif
            if(AlphaSys==i51KITG2_ALPHA_NONEED)
            {
                LcdBuffer[ (ScreenWidth*TempY2 + TempX2)] = Color;
            }
            else
            {
                BuffPos = ScreenWidth*TempY2 + TempX2;
                LcdBuffer[BuffPos] = i51KitG2GetNewColor(LcdBuffer[BuffPos], Color);
            }
        }

        if( dx>= dy ){
            // 当DecisionTag>0时，始端y向加RegulationY，末端y向减RegulationY
            if (DecisionTag > 0)
            {
                StartY += RegulationY;
                EndY -= RegulationY;    
                DecisionTag -= dx2;
            }
    
            //始端x向加1，末端x向减1
            StartX++;    
            EndX--;
            DecisionTag += dy2;
        }
        else
        {
            // 当DecisionTag<0时，始端X向加1，末端X向减1
            if (DecisionTag < 0)
            {
                StartX++;    
                EndX--;
                DecisionTag += dy2;
           }
    
            //始端y向加RegulationY，末端y向减RegulationY
            StartY += RegulationY;
            EndY -= RegulationY;          
            DecisionTag -= dx2;

        }
    }

 #if i51KITG2_DEBUG
    iLog( "G2DrawLine Start , %x, %x", Point, Color) ;
#endif

    return iTRUE;

}

iKIT iBOOL i51KitG2DrawPolygon( iU32 VerticeNumber, iS32 Point[][2], iCOLOR Color)
{

    // author : Otto
    // since : Aug 11 2011
    // (C) 2011 , PKIG Tech. Co., Ltd.

    // iU32 VerticeNumber : 多边形顶点个数,必须大于等于3
    // iS32 Point[][2] :
    //    Point[0][0]是需要绘制多边形的第一个顶点X坐标
    //    Point[0][1]是需要绘制多边形的第一个顶点Y坐标
    //    Point[Num-1][0]是需要绘制多边形的最后一个顶点X坐标
    //    Point[Num-1][1]是需要绘制多边形的最后一个顶点X坐标
    // iCOLOR Color : 绘制多边形的颜色

    // notes : 函数功能是绘制有VerticeNumber个顶点,顶点坐标用Point存放的多边形
    //    Point数组中存放的顶点必须是有序的，例如多边形由线段AB，
    //    线段BC，线段CD，线段DE，线段EA连接组成，
    //    则Point[0][0]存放顶点A的X坐标,Point[0][1]存放顶点A的Y坐标
    //    则Point[1][0]存放顶点B的X坐标,Point[1][1]存放顶点B的Y坐标
    //    则Point[2][0]存放顶点C的X坐标,Point[2][1]存放顶点C的Y坐标
    //    则Point[3][0]存放顶点D的X坐标,Point[3][1]存放顶点D的Y坐标
    //    则Point[4][0]存放顶点E的X坐标,Point[4][1]存放顶点E的Y坐标

    iU32 CurrentSide = 0;
    iS32 CurrentPoint[2][2] = {0};
    iU32 CopyLength = 0;
	
    // 当前绘制的元素在VP上的ID
    iU16 CurrDrawID;
    iU32 PonterPoints;
    iS32 Temp1 = 0;

 #if i51KITG2_DEBUG
    iLog( "G2DrawPolygon Start , %x, %x, %x", VerticeNumber, Point,Color) ;
#endif

    if( VerticeNumber < 3 ) return iFALSE;

    for( Temp1=0; Temp1<VerticeNumber; Temp1++ )
    {
        if( Point[Temp1][0]>2000||Point[Temp1][0]<-1500
         ||Point[Temp1][1]>2000||Point[Temp1][1]<-1500		
        )
        {
 #if i51KITG2_DEBUG
    iLog( "G2DrawPolygon Point[%X]:%X , %x", Temp1,Point[Temp1][0],Point[Temp1][1]) ;
#endif
            return iFALSE;
        }
    }     	
    // 在虚拟面板上绘制，实际上只是进行一些赋值操作
    if( i51UseVirtualPanel && i51DrawVirtualPanel )
    {
        if( i51CurrentVpManage.TotalVPNumber == 0 ) return iFALSE;

        if( iNULL == i51CurrentVpActivity )	return iFALSE;
		
        CurrDrawID = i51CurrentVpActivity->VPCurrentElement;
        // 画板已经画满则直接返回
        if(  i51CurrentVpActivity->VpIsFull ) return iFALSE;
      
        CopyLength = sizeof(iS32) * 2*VerticeNumber;
        PonterPoints = ( iU32 )i51AdeOsMalloc( CopyLength,  __FILE__ , __LINE__ );
        if( !PonterPoints) return iFALSE;
        i51AdeStdMemcpy16( (void *)PonterPoints, (void*)Point, CopyLength);  

        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementType = I51KITG2_ELEMENT_TYPE_POLYGON;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementNumber = CurrDrawID;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElemenAlpha = AlphaSys;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Polygon.VerticeNumber = VerticeNumber;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Polygon.PointerPoints = PonterPoints;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Polygon.Color= Color;
        i51CurrentVpActivity->VPCurrentElement =  CurrDrawID +1;
        if( (CurrDrawID+1) >= i51CurrentVpActivity->VPMaxElement )	i51CurrentVpActivity->VpIsFull = iTRUE;
        return iTRUE;
    }

    CopyLength = sizeof(iU32) * 4;

    for( CurrentSide=0; CurrentSide<VerticeNumber-1; CurrentSide++ )
    {
          i51AdeStdMemcpy16( CurrentPoint, &Point[CurrentSide][0], CopyLength);  
         i51KitG2DrawLine( CurrentPoint, Color);
    }

    CurrentPoint[0][0] = Point[0][0];
    CurrentPoint[0][1] = Point[0][1];
    CurrentPoint[1][0] = Point[VerticeNumber-1][0];
    CurrentPoint[1][1] = Point[VerticeNumber-1][1];
    i51KitG2DrawLine( CurrentPoint, Color);

 #if i51KITG2_DEBUG
    iLog( "G2DrawPolygon End , %x, %x, %x", VerticeNumber, Point,Color) ;
#endif
    return iTRUE;
	
}

iKIT iBOOL i51KitG2DrawArc( iS32 Point[][2], iCOLOR Color )
{

    // author : Otto
    // since : Aug 11 2011
    // (C) 2011 , PKIG Tech. Co., Ltd.

    // iS32 Point[][2] :
    //    Point[0][0]是需要绘制弧线的起点X坐标
    //    Point[0][1]是需要绘制弧线的起点Y坐标
    //    Point[1][0]是经过弧线上任意点的X坐标
    //    Point[1][1]是经过弧线上任意点的Y坐标
    //    Point[2][0]是需要绘制弧线的终点X坐标
    //    Point[2][1]是需要绘制弧线的终点Y坐标
    // iCOLOR Color : 绘制弧线的颜色

    // notes : 绘制以(Point[0][0], Point[0][1])为起点,(Point[2][0],Point[2][1])为终点,经过点(Point[1][0],Point[1][1])的弧线
    // 已知ABC三点,求△ABC外接圆圆心的公式为：
    //    CenterX=(Qy+K1Px-K2Qx-Py)/(K2-K1)
    //    CenterY=(K1K2Qx+K2Py-K1Qy-K1K2Px)/(K2-K1)
    //    其中(Px, Py)是线段AB中点坐标, (Qx, Qy)是线段BC的中点坐标, K1是线段AB的中垂线斜率, K2是线段BC的中垂线斜率

    //计算X的绝对值
    #define ABSOLUTE(X) (X) > 0 ? (X) :(-(X))
    #define CLOCK_WISE    1
    #define ANTI_CLOCK_WISE 2

    iS32 OldX = 0;
    iS32 OldY = 0;
    iS32 CenterX = 0;
    iS32 CenterY = 0;
    iS32 Molecular = 0;
    iS32 Denominator = 0;
    iS32 CurrentX = 0;
    iS32 CurrentY = 0;
    iS16 RegulationY = 1;
    iS16 RegulationX = 1;
    iS32 SquareR = 0;
    iS32 Xa, Xb, Xc, Ya, Yb, Yc;
    iS32 PerpendicularIntersectionY = 0;
    iS32 SquareDifferential[3] = {0};
    iU8  AirDirection = CLOCK_WISE;

    iS32 TempX, TempY;
    // 当前绘制的元素在VP上的ID
    iU16 CurrDrawID;
   // 创建的VP实际大小	
    iRECT * VPActualRect = iNULL;
    // 创建的VP在屏幕上的左上和右下坐标
    iS32 VP_TOP_LEFT_X = 0;
    iS32 VP_TOP_LEFT_Y = 0;
    iS32 VP_LOVER_RIGHT_X = 0;
    iS32 VP_LOVER_RIGHT_Y = 0;
    // 需要绘制的VP区域	
    iRECT * VPDrawRect = iNULL;
    // 绘制区域	
    iS16 DrawRectStartX = 0;
    iS16 DrawRectStartY = 0;
    iS16 DrawRectEndX = ScreenWidth;
    iS16 DrawRectEndY = ScreenHeight;
    iU32 BuffPos = 0;
    iS32 Temp1 = 0;
	
 #if i51KITG2_DEBUG
    iLog( "G2DrawArc Start , %x, %x", Point,Color) ;
#endif
    for( Temp1=0; Temp1<3; Temp1++ )
    {
        if( Point[Temp1][0]>2000||Point[Temp1][0]<-1500
         ||Point[Temp1][1]>2000||Point[Temp1][1]<-1500		
        )
        {
 #if i51KITG2_DEBUG
    iLog( "G2DrawArc Point[%X]:%X , %x", Temp1,Point[Temp1][0],Point[Temp1][1]) ;
#endif
            return iFALSE;
        }
    }     	
	
    // 在虚拟面板上绘制，实际上只是进行一些赋值操作
    if( i51UseVirtualPanel && i51DrawVirtualPanel )
    {
        if( i51CurrentVpManage.TotalVPNumber == 0 ) return iFALSE;

        if( iNULL == i51CurrentVpActivity )	return iFALSE;
		
        CurrDrawID = i51CurrentVpActivity->VPCurrentElement;
        // 画板已经画满则直接返回
        if(  i51CurrentVpActivity->VpIsFull ) return iFALSE;
      
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementType = I51KITG2_ELEMENT_TYPE_ARC;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementNumber = CurrDrawID;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElemenAlpha = AlphaSys;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Arc.Points[0][0] = Point[0][0];
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Arc.Points[0][1] = Point[0][1];
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Arc.Points[1][0] = Point[1][0];
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Arc.Points[1][1] = Point[1][1];
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Arc.Points[2][0] = Point[2][0];
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Arc.Points[2][1] = Point[2][1];
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Arc.Color= Color;
        i51CurrentVpActivity->VPCurrentElement =  CurrDrawID +1;
        if( (CurrDrawID+1) >= i51CurrentVpActivity->VPMaxElement )	i51CurrentVpActivity->VpIsFull = iTRUE;

        return iTRUE;

    }

    if( i51UseVirtualPanel )
    {
        VPActualRect = &(i51CurrentVpRender->VPActualRect);
        VPDrawRect = &(i51CurrentVpRender->VPDrawRect);
  
        VP_TOP_LEFT_X = VPActualRect->X;
        VP_TOP_LEFT_Y = VPActualRect->Y;
        VP_LOVER_RIGHT_X = VP_TOP_LEFT_X + VPActualRect->Width;
        VP_LOVER_RIGHT_Y = VP_TOP_LEFT_Y + VPActualRect->Height;

        // 在屏幕上的绘制区域
        DrawRectStartX = VPDrawRect->X;
        DrawRectStartY = VPDrawRect->Y;
        DrawRectEndX = DrawRectStartX + VPDrawRect->Width;
        DrawRectEndY = DrawRectStartY + VPDrawRect->Height;

        if( DrawRectStartX < 0 ) DrawRectStartX = 0;
        if( DrawRectStartY < 0 ) DrawRectStartX = 0;
        if( DrawRectEndX >= ScreenWidth ) DrawRectEndX = ScreenWidth;
        if( DrawRectEndY >= ScreenHeight) DrawRectEndY = ScreenHeight;
		
        Xa = Point[0][0] + VP_TOP_LEFT_X;
        Ya = Point[0][1] + VP_TOP_LEFT_Y;
        Xb = Point[1][0] + VP_TOP_LEFT_X;
        Yb = Point[1][1] + VP_TOP_LEFT_Y;
        Xc = Point[2][0] + VP_TOP_LEFT_X;
        Yc = Point[2][1] + VP_TOP_LEFT_Y;
    }
    else
    {
        DrawRectStartX = 0;
        DrawRectStartY = 0;
        DrawRectEndX = ScreenWidth;
        DrawRectEndY = ScreenHeight;
        Xa = Point[0][0];
        Ya = Point[0][1];
        Xb = Point[1][0];
        Yb = Point[1][1];
        Xc = Point[2][0];
        Yc = Point[2][1];
    }
	


    //不在同一直线上三点才能确定一个圆,如果三点中有两点重合则无法确定圆
    if( Xa==Xb&&Ya==Yb ) return iFALSE;
    if( Xa==Xc&&Ya==Yc ) return iFALSE;
    if( Xb==Xc&&Yb==Yc ) return iFALSE;

    //三点在同一直线上无法确定圆
    if( Xa!=Xc && Xa!=Xb )
    {
         if( (((Ya-Yc)*100)/(Xa-Xc)) ==(((Ya-Yb)*100)/(Xa-Xb)) ) return iFALSE;
    }
    if( Xa==Xb && Xa==Xc ) return iFALSE;
    if( Ya==Yb && Ya==Yc ) return iFALSE;

    //设置左边的顶点为圆弧的起点
    if( Xa > Xc )
    {
        Xa += Xc;
        Xc = Xa - Xc;
        Xa = Xa - Xc;
  
        Ya += Yc;
        Yc = Ya - Yc;
        Ya = Ya - Yc;
    }

    //计算圆心X坐标
    Molecular = (Yc-Ya)*(Yc-Yb)*(Yb-Ya) + ( Xa*Xa-Xb*Xb)*(Yc-Yb) + (Xc*Xc - Xb*Xb)*(Yb-Ya);
    Denominator = ( (Xb-Xc)*(Ya-Yb) + (Xb-Xa)*(Yb-Yc) ) << 1;
    CenterX = ((Molecular*10)/Denominator+5)/10;

    //计算圆心Y坐标
    Molecular = (Xb-Xa)*(Xc-Xb)*(Xc-Xa) + (Xb-Xc)*(Ya+Yb)*(Yb-Ya) + (Xb-Xa)*(Yb+Yc)*(Yc-Yb);
    Denominator = ( (Xb-Xc)*(Yb-Ya) + (Xb-Xa)*(Yc-Yb) ) << 1;
    CenterY = ((Molecular*10)/Denominator + 5 ) / 10;


    //计算圆半径的平方
    SquareR = ( Xc - CenterX ) * ( Xc - CenterX ) + ( Yc - CenterY ) * ( Yc - CenterY );


    //计算经过点B且与直线AC垂直的直线与直线AC的交点X坐标
    //  Molecular = (Yb-Ya)*(Xc-Xa)*(Ya-Yc) - (Xc-Xa)*(Xc-Xa)*Xb - (Yc-Ya)*(Yc-Ya) * Xa;
    //  Denominator = (Yc-Ya)*(Ya-Yc) - (Xc-Xa)*(Xc-Xa);
    Molecular = (Yc-Ya)*(Ya*Yb-Yb*Yc+Xa*Xb-Xc*Xb) + (Xa-Xc)*(Ya*Xc-Yc*Xa);
    Denominator = (Yc-Ya)*(Ya-Yc) - (Xc-Xa)*(Xc-Xa);
    PerpendicularIntersectionY =  ((Molecular*10)/Denominator + 5 ) / 10;

    //判断画圆弧方向,如果B点在垂点下方则圆弧方向为逆时针,否则为顺时针
    if( Yb > PerpendicularIntersectionY )
    {
        AirDirection = ANTI_CLOCK_WISE;
    }
    else if( Yb==PerpendicularIntersectionY )
    {
        if(  Xb<CenterX )
        {
             AirDirection = ANTI_CLOCK_WISE;
        }
    }

    //判断画圆弧方向为顺时针时X,Y的变化趋势,根据对称性,如果为逆时针则趋势是反向
    if ( Ya < CenterY )
    {
        RegulationX = 1;
    }
    else if ( Ya > CenterY )
    {
        RegulationX = -1;
    }

    if ( Xa < CenterX )
    {
        RegulationY = -1;
    }
    else if ( Xa > CenterX )
    {
        RegulationY = 1;
    }

    if( AirDirection == ANTI_CLOCK_WISE )
    {
        RegulationX = -RegulationX;
        RegulationY = -RegulationY;
    }

    if( Ya==CenterY)
    {
        if( Xa > CenterX )
        {
             RegulationX = -1;
        }
        else
        {
            RegulationX = 1;
        }
    }

    if( Xa==CenterX )
    {
        if( Ya > CenterY )
        {
              RegulationY = -1;
        }
        else
        {
             RegulationY = 1;
        }
    }

    CurrentX = Xa;
    CurrentY = Ya;
    TempX = CurrentX;
    TempY = CurrentY;

    if( TempX>=DrawRectStartX&&TempX<DrawRectEndX&&TempY>=DrawRectStartY&&TempY<DrawRectEndY )
    {
#if LCDBUFF_DEBUG
i51KitG2CheckLcdbuffer(ScreenWidth*TempY+ TempX,1);
#endif
        if(AlphaSys==i51KITG2_ALPHA_NONEED)
        {
            LcdBuffer[ (ScreenWidth*TempY + TempX)] = Color;
        }
        else
        {
            BuffPos = ScreenWidth*TempY + TempX;
            LcdBuffer[BuffPos] = i51KitG2GetNewColor(LcdBuffer[BuffPos], Color);
        }
    }

    while(1)
    {
        if( CurrentX==Xc&&CurrentY==Yc) break;

  
        OldX = CurrentX;
        OldY = CurrentY;

        SquareDifferential[0] = ABSOLUTE( (CurrentX+RegulationX-CenterX)*(CurrentX+RegulationX-CenterX)\
                                                         +(CurrentY-CenterY)*(CurrentY-CenterY) - SquareR);
        SquareDifferential[1] = ABSOLUTE( (CurrentY +RegulationY -CenterY)*(CurrentY +RegulationY -CenterY)\
                                                         +(CurrentX-CenterX)*(CurrentX-CenterX) - SquareR);
        SquareDifferential[2] = ABSOLUTE( (CurrentY +RegulationY -CenterY)*(CurrentY +RegulationY -CenterY)\
                                                         +(CurrentX+RegulationX-CenterX)*(CurrentX+RegulationX-CenterX) - SquareR);
        if( SquareDifferential[0]<SquareDifferential[1] )
        {
            if( SquareDifferential[0]<SquareDifferential[2] )
            {
                CurrentX += RegulationX ;
            }
            else
            {
                CurrentX += RegulationX ;
                CurrentY += RegulationY ;
            }
        }
        else
        {
            if( SquareDifferential[1]<SquareDifferential[2] )
            {
                CurrentY += RegulationY ;
            }
            else
            {
                CurrentX += RegulationX ;
                CurrentY += RegulationY ;
            }
        }
  
        TempX = CurrentX;
        TempY = CurrentY;
  
        if( TempX>=DrawRectStartX&&TempX<DrawRectEndX&&TempY>=DrawRectStartY&&TempY<DrawRectEndY )
        {
#if LCDBUFF_DEBUG
i51KitG2CheckLcdbuffer(ScreenWidth*TempY+ TempX,1);
#endif
            if(AlphaSys==i51KITG2_ALPHA_NONEED)
            {
                LcdBuffer[ (ScreenWidth*TempY + TempX)] = Color;
            }
            else
            {
                BuffPos = ScreenWidth*TempY + TempX;
                LcdBuffer[BuffPos] = i51KitG2GetNewColor(LcdBuffer[BuffPos], Color);
            }
        }
  
        if( CurrentX==CenterX && OldX!=CurrentX )
        {
            RegulationY = -RegulationY;
        }

        if( CurrentY==CenterY && OldY!=CurrentY )
        {
            RegulationX = -RegulationX;
        }
    }

  #undef ABSOLUTE
  #undef CLOCK_WISE
  #undef ANTI_CLOCK_WISE

 #if i51KITG2_DEBUG
    iLog( "G2DrawArc End , %x, %x", Point,Color) ;
#endif

    return iTRUE;

}

iKIT iBOOL i51KitG2DrawCircle( iS32 X, iS32 Y, iU32 R, iCOLOR Color )
{

    // author : Otto
    // since : Aug 12 2011
    // (C) 2011 , PKIG Tech. Co., Ltd.

    // iS32 X : 圆心X坐标
    // iS32 Y : 圆心Y坐标
    // iU32 R : 圆半径
    // iCOLOR Color : 绘制弧线的颜色

    // notes : 绘制以(X, Y)为圆心,半径为R的圆
    // 根据圆的对称性，我们找到圆上一点可以确定与之X轴对称、Y轴对称已经中心对称的另外三点
    // 所以计算时只需要通过计算确认一个象限内圆上的点,其他三个象限的点可以根据对称性直接获得

    #define ABSOLUTE(X) (X) > 0 ? (X) :(-(X))

    iS32 SquareR = 0;
    iS32 OldX = 0;
    iS32 OldY = 0;
    iS32 EndX = 0;
    iS32 EndY = 0;
    iS32 CenterX = X;
    iS32 CenterY = Y;
    iS32 Center2X = X+X;
    iS32 Center2Y = Y+Y;
    iS32 CurrentX = 0;
    iS32 CurrentY = 0;
    iS32 SymmetryPoints[3][2] ;
    iS32 SquareDifferential[3] = {0};

    iS32 DrawX = 0, DrawY = 0;   
    iS32 TempX1, TempY1;	
    iS32 TempX2, TempY2;	
    iS32 TempX3, TempY3;	
    iS32 TempX4, TempY4;	

    // 当前绘制的元素在VP上的ID
    iU16 CurrDrawID;

    // 创建的VP实际大小	
    iRECT * VPActualRect = iNULL;
    // 创建的VP在屏幕上的左上和右下坐标
    iS32 VP_TOP_LEFT_X = 0;
    iS32 VP_TOP_LEFT_Y = 0;
    iS32 VP_LOVER_RIGHT_X = 0;
    iS32 VP_LOVER_RIGHT_Y = 0;
    // 需要绘制的VP区域	
    iRECT * VPDrawRect = iNULL;
    // 绘制区域	
    iS16 DrawRectStartX = 0;
    iS16 DrawRectStartY = 0;
    iS16 DrawRectEndX = ScreenWidth;
    iS16 DrawRectEndY = ScreenHeight;
    iU32 BuffPos = 0;
    iS32 Temp1 = 0;
	
 #if i51KITG2_DEBUG
    iLog( "G2DrawCircle start , %x, %x, %x, %x", X,Y,R,Color) ;
#endif
    if( X>2000||X<-1500 ||Y>2000||Y<-1500||R>500 )
    {
#if i51KITG2_DEBUG
iLog( "G2DrawCircle: %X,%X,%X", X,Y,R) ;
#endif
        return iFALSE;
    }

    // 在虚拟面板上绘制，实际上只是进行一些赋值操作
    if( i51UseVirtualPanel && i51DrawVirtualPanel )
    {
        if( i51CurrentVpManage.TotalVPNumber == 0 ) return iFALSE;

        if( iNULL == i51CurrentVpActivity )	return iFALSE;
		
        CurrDrawID = i51CurrentVpActivity->VPCurrentElement;
        // 画板已经画满则直接返回
        if(  i51CurrentVpActivity->VpIsFull ) return iFALSE;
      
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementType = I51KITG2_ELEMENT_TYPE_CIRCLE;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementNumber = CurrDrawID;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElemenAlpha = AlphaSys;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Circle.X = X;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Circle.Y = Y;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Circle.R = R;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Circle.Color= Color;
        i51CurrentVpActivity->VPCurrentElement =  CurrDrawID +1;
        if( (CurrDrawID+1) >= i51CurrentVpActivity->VPMaxElement )	i51CurrentVpActivity->VpIsFull = iTRUE;
        return iTRUE;
    }

    if( i51UseVirtualPanel )
    {
        VPActualRect = &(i51CurrentVpRender->VPActualRect);
        VPDrawRect = &(i51CurrentVpRender->VPDrawRect);
  
        VP_TOP_LEFT_X = VPActualRect->X;
        VP_TOP_LEFT_Y = VPActualRect->Y;
        VP_LOVER_RIGHT_X = VP_TOP_LEFT_X + VPActualRect->Width;
        VP_LOVER_RIGHT_Y = VP_TOP_LEFT_Y + VPActualRect->Height;

        // 在屏幕上的绘制区域
        DrawRectStartX = VPDrawRect->X;
        DrawRectStartY = VPDrawRect->Y;
        DrawRectEndX = DrawRectStartX + VPDrawRect->Width;
        DrawRectEndY = DrawRectStartY + VPDrawRect->Height;

        if( DrawRectStartX < 0 ) DrawRectStartX = 0;
        if( DrawRectStartY < 0 ) DrawRectStartX = 0;
        if( DrawRectEndX >= ScreenWidth ) DrawRectEndX = ScreenWidth;
        if( DrawRectEndY >= ScreenHeight) DrawRectEndY = ScreenHeight;

        DrawX = X + VP_TOP_LEFT_X;
        DrawY = Y +VP_TOP_LEFT_Y;
    }
    else
    {
        DrawRectStartX = 0;
        DrawRectStartY = 0;
        DrawRectEndX = ScreenWidth;
        DrawRectEndY = ScreenHeight;

        DrawX = X;
        DrawY = Y;
    }

    SquareR = R * R;

    CurrentX = DrawX - R;
    CurrentY = DrawY;

    EndX = DrawX;
    EndY = DrawY - R;

    CenterX = DrawX;
    CenterY = DrawY;
    Center2X = DrawX+DrawX;
    Center2Y = DrawY+DrawY;

    while(1)
    {
        OldX = CurrentX;
        OldY = CurrentY;

        SymmetryPoints[0][0] = Center2X - CurrentX;
        SymmetryPoints[0][1] = CurrentY;
        SymmetryPoints[1][0] = SymmetryPoints[0][0];
        SymmetryPoints[1][1] = Center2Y - CurrentY;
        SymmetryPoints[2][0] = CurrentX;
        SymmetryPoints[2][1] = SymmetryPoints[1][1];

        TempX1 = CurrentX;
        TempY1 = CurrentY;
        TempX2 = SymmetryPoints[0][0];
        TempY2 = SymmetryPoints[0][1];
        TempX3 = SymmetryPoints[1][0];
        TempY3 = SymmetryPoints[1][1];
        TempX4 = SymmetryPoints[2][0];
        TempY4 = SymmetryPoints[2][1];
		
         if( TempX1>=DrawRectStartX && TempX1<DrawRectEndX && TempY1>=DrawRectStartY&& TempY1<DrawRectEndY )
        {
#if LCDBUFF_DEBUG
i51KitG2CheckLcdbuffer(ScreenWidth*TempY1,1);
#endif
            if(AlphaSys==i51KITG2_ALPHA_NONEED)
            {
                LcdBuffer[ (ScreenWidth*TempY1 + TempX1)] = Color;
            }
            else
            {
                BuffPos = ScreenWidth*TempY1 + TempX1;
                LcdBuffer[BuffPos] = i51KitG2GetNewColor(LcdBuffer[BuffPos], Color);
            }
        }
	
         if( TempX2>=DrawRectStartX && TempX2<DrawRectEndX && TempY2>=DrawRectStartY&& TempY2<DrawRectEndY )
        {
#if LCDBUFF_DEBUG
i51KitG2CheckLcdbuffer(ScreenWidth*TempY2 + TempX2,1);
#endif
            if(AlphaSys==i51KITG2_ALPHA_NONEED)
            {
                LcdBuffer[ (ScreenWidth*TempY2 + TempX2)] = Color;
            }
            else
            {
                BuffPos = ScreenWidth*TempY2 + TempX2;
                LcdBuffer[BuffPos] = i51KitG2GetNewColor(LcdBuffer[BuffPos], Color);
            }
        }
	
         if( TempX3>=DrawRectStartX && TempX3<DrawRectEndX && TempY3>=DrawRectStartY&& TempY3<DrawRectEndY )
        {
#if LCDBUFF_DEBUG
i51KitG2CheckLcdbuffer(ScreenWidth*TempY3 + TempX3,1);
#endif
            if(AlphaSys==i51KITG2_ALPHA_NONEED)
            {
                LcdBuffer[ (ScreenWidth*TempY3 + TempX3)] = Color;
            }
            else
            {
                BuffPos = ScreenWidth*TempY3 + TempX3;
                LcdBuffer[BuffPos] = i51KitG2GetNewColor(LcdBuffer[BuffPos], Color);
            }
         }
	
         if( TempX4>=DrawRectStartX && TempX4<DrawRectEndX && TempY4>=DrawRectStartY&& TempY4<DrawRectEndY )
        {
#if LCDBUFF_DEBUG
i51KitG2CheckLcdbuffer(ScreenWidth*TempY4 + TempX4,1);
#endif
            if(AlphaSys==i51KITG2_ALPHA_NONEED)
            {
                LcdBuffer[ (ScreenWidth*TempY4 + TempX4)] = Color;
            }
            else
            {
                BuffPos = ScreenWidth*TempY4 + TempX4;
                LcdBuffer[BuffPos] = i51KitG2GetNewColor(LcdBuffer[BuffPos], Color);
            }
        }
	
        if( CurrentX==EndX && CurrentY==EndY )
        {
            break;
        }

        SquareDifferential[0] = ABSOLUTE( (CurrentX+1-CenterX)*(CurrentX+1-CenterX)\
                                                         +(CurrentY-CenterY)*(CurrentY-CenterY) - SquareR);
        SquareDifferential[1] = ABSOLUTE( (CurrentY - 1 -CenterY)*(CurrentY - 1 -CenterY)\
                                                         +(CurrentX-CenterX)*(CurrentX-CenterX) - SquareR);
        SquareDifferential[2] = ABSOLUTE( (CurrentY -1  -CenterY)*(CurrentY -1 -CenterY)\
                                                         +(CurrentX+1-CenterX)*(CurrentX+1-CenterX) - SquareR);
        if( SquareDifferential[0]<SquareDifferential[1] )
        {
            if( SquareDifferential[0]<SquareDifferential[2] )
            {
                CurrentX ++ ;
            }
            else
            {
                CurrentX ++ ;
                CurrentY -- ;
            }
        }
        else
        {
            if( SquareDifferential[1]<SquareDifferential[2] )
            {
                CurrentY -- ;
            }
            else
            {
                CurrentX ++ ;
                CurrentY -- ;
            }
        }

       }

    #undef ABSOLUTE

 #if i51KITG2_DEBUG
    iLog( "G2DrawCircle end , %x, %x, %x, %x", X,Y,R,Color) ;
#endif

    return iTRUE;
}


iKIT iBOOL i51KitG2FillPolygon( iS32 VerticeNumber, iS32 Point[][2], iCOLOR Color)
{ 	

    // author : Otto
    // since : Aug 15 2011
    // (C) 2011 , PKIG Tech. Co., Ltd.

    // iU32 VerticeNumber : 多边形顶点个数,必须大于等于3
    // iS32 Point[][2] :
    //    Point[0][0]是需要填充多边形的第一个顶点X坐标
    //    Point[0][1]是需要填充多边形的第一个顶点Y坐标
    //    Point[Num-1][0]是需要填充多边形的最后一个顶点X坐标
    //    Point[Num-1][1]是需要填充多边形的最后一个顶点X坐标
    // iCOLOR Color : 填充多边形的颜色

    // notes : 函数功能是填充有Num个顶点,顶点坐标用Point存放的多边形
    //    Point数组中存放的顶点必须是有序的，例如多边形由线段AB，
    //    线段BC，线段CD，线段DE，线段EA连接组成，
    //    则Point[0][0]存放顶点A的X坐标,Point[0][1]存放顶点A的Y坐标
    //    则Point[1][0]存放顶点B的X坐标,Point[1][1]存放顶点B的Y坐标
    //    则Point[2][0]存放顶点C的X坐标,Point[2][1]存放顶点C的Y坐标
    //    则Point[3][0]存放顶点D的X坐标,Point[3][1]存放顶点D的Y坐标
    //    则Point[4][0]存放顶点E的X坐标,Point[4][1]存放顶点E的Y坐标

    #define MAX_POINTS 2048
    #define MAX_LINE_POINTS 50
    //边缘坐标集
    iS16 * IntersectionCoordinates = iNULL;
    //一行中的边缘点坐标集
    iS16 IntersectionX[MAX_LINE_POINTS];
    //一行中的边缘点个
    iU16 IntersectionNumberX = 0;
    //当前扫描到的行数
    iU16 CurrentLine = 0;
    //当前边缘点
    iU16 CurrentIntersection = 0;
    //边缘点个数
    iU16 IntersectionNum = 0;

    iS32 EndOldY = -1;
    iS32 EndOldX = -1;
    iS32 StartOldY = -1;
    iS32 StartOldX = -1;
    iS32 EndX = Point[1][0];
    iS32 EndY = Point[1][1];
    iS32 StartX = Point[0][0];
    iS32 StartY = Point[0][1];

    //起点与终点X和Y的差值
    iS32 dx = 0;
    iS32 dy = 0;
    iS32 dx2 = 0;
    iS32 dy2 = 0;

    // 决策量
    iS32 DecisionTag = 0;

    // 需要绘制次数
    iU32 HalfLength = 0;

    //当前绘制位置
    iU32 Location = 0;

    //校准Y值
    iS16 RegulationY = 1;

    iU16 CurrDrawID;
//    iU16 DrawRectStartX = 0;
//    iU16 DrawRectStartY = 0;
//    iU16 DrawRectEndX = 0;
//    iU16 DrawRectEndY = 0;
    iU32 PonterPoints = 0;  
    iU32 CopyLength = 0; 
    iCOLOR * ColorBuf = iNULL;
    iRECT * VPActualRect;
    iS16 Temp1 = 0, Temp2 = 0;
	
    if( VerticeNumber < 3 ) return iFALSE;

 #if i51KITG2_DEBUG
    iLog( "G2FillPolygon start , %x, %x, %x", VerticeNumber,Point,Color) ;
#endif

    for( Temp1=0; Temp1<VerticeNumber; Temp1++ )
    {
        if( Point[Temp1][0]>2000||Point[Temp1][0]<-1500
         ||Point[Temp1][1]>2000||Point[Temp1][1]<-1500		
        )
        {
 #if i51KITG2_DEBUG
    iLog( "G2FillPolygon Point[%X]:%X , %x", Temp1,Point[Temp1][0],Point[Temp1][1]) ;
#endif
            return iFALSE;
        }
    }
    // 在虚拟面板上绘制，实际上只是进行一些赋值操作
    if( i51UseVirtualPanel && i51DrawVirtualPanel )
    {
        if( i51CurrentVpManage.TotalVPNumber == 0 ) return iFALSE;

        if( iNULL == i51CurrentVpActivity )	return iFALSE;
		
        CurrDrawID = i51CurrentVpActivity->VPCurrentElement;
        // 画板已经画满则直接返回
        if(  i51CurrentVpActivity->VpIsFull ) return iFALSE;
      
        CopyLength = sizeof(iS32) * 2*VerticeNumber;
        PonterPoints = ( iU32)(i51AdeOsMalloc( CopyLength,  __FILE__  , __LINE__ ));
        if( PonterPoints==0 ) return iFALSE;		
         i51AdeStdMemcpy16( (void *)(PonterPoints), (void *)(Point), CopyLength);  

        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementType = I51KITG2_ELEMENT_TYPE_FILLPOLYGON;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementNumber = CurrDrawID;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElemenAlpha = AlphaSys;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.FillPolygon.VerticeNumber = VerticeNumber;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.FillPolygon.PointerPoints = PonterPoints;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.FillPolygon.Color= Color;
        i51CurrentVpActivity->VPCurrentElement =  CurrDrawID +1;
        if( (CurrDrawID+1) >= i51CurrentVpActivity->VPMaxElement )	i51CurrentVpActivity->VpIsFull = iTRUE;
        return iTRUE;
    }

IntersectionCoordinates = (iS16*)(i51AdeOsMalloc(MAX_POINTS*4,__FILE__ , __LINE__ ));
    //统计多边形边缘
    VPActualRect = &(i51CurrentVpRender->VPActualRect);
    for( CurrentLine=0; CurrentLine<VerticeNumber; CurrentLine++ )
    {
        if( i51UseVirtualPanel )
        {
            StartX = Point[CurrentLine][0] + VPActualRect->X;
            StartY = Point[CurrentLine][1] + VPActualRect->Y;
            EndX = Point[(CurrentLine+1)%VerticeNumber][0] + VPActualRect->X;
            EndY = Point[(CurrentLine+1)%VerticeNumber][1] + VPActualRect->Y;
        }
        else
        {
            StartX = Point[CurrentLine][0];
            StartY = Point[CurrentLine][1];
            EndX = Point[(CurrentLine+1)%VerticeNumber][0];
            EndY = Point[(CurrentLine+1)%VerticeNumber][1];
        }	

        //如果终点在起点左边则交换他们的位置
        if ( EndX < StartX )
        {
            EndX += StartX;
            StartX = EndX - StartX;
            EndX = EndX - StartX;
    
            EndY += StartY;
            StartY = EndY - StartY;
            EndY = EndY - StartY;
        }

        //计算起点与终点X和Y的差值
        dx = EndX - StartX;
        if( EndY > StartY )
        {
            dy = EndY - StartY;
            RegulationY = 1;
        }
        else
        {
            dy = StartY - EndY;
            RegulationY = -1;
        }
  
        //起点与终点X和Y的差值放大一倍
        dx2 = dx << 1;
        dy2 = dy << 1;
    
        //决策量赋初值
        if( dx>=dy){
            DecisionTag = dy2 - dx;
            HalfLength = (dx+1) >> 1;
        } else{
            DecisionTag = dy - dx2;
            HalfLength = (dy+1) >> 1;
        }

        for ( Location = 0; Location <= HalfLength; Location++)
        {
            if( CurrentIntersection>=MAX_POINTS-2 ) goto ERROR;
            //同一直线同一Y值只取一个X
            if( (StartOldY!=StartY&&EndOldY!=StartY)||Location==0)
            {
                StartOldY = StartY;
                StartOldX = StartX;
                IntersectionCoordinates[CurrentIntersection<<1] = StartX;
                IntersectionCoordinates[(CurrentIntersection<<1)+1]= StartY;
                CurrentIntersection++;				
            }

            //同一直线同一Y值只取一个X
            if( (EndOldY != EndY  && EndY!=StartY) || Location==0 )
            {
                EndOldY = EndY;
                EndOldX = EndX;
                IntersectionCoordinates[CurrentIntersection<<1] = EndX;
                IntersectionCoordinates[(CurrentIntersection<<1)+1]= EndY;
                CurrentIntersection++;				
            }
    
            if( dx>= dy ){
                // 当DecisionTag>0时，始端y向加RegulationY，末端y向减RegulationY
                if (DecisionTag > 0)
                {
                    StartY += RegulationY;
                    EndY -= RegulationY;    
                    DecisionTag -= dx2;
                }
      
                //始端x向加1，末端x向减1
                StartX++;    
                EndX--;
                DecisionTag += dy2;
            }
            else
            {
                // 当DecisionTag<0时，始端X向加1，末端X向减1
                if (DecisionTag < 0)
                {
                    StartX++;    
                    EndX--;
                    DecisionTag += dy2;
                }
      
                //始端y向加RegulationY，末端y向减RegulationY
                StartY += RegulationY;
                EndY -= RegulationY;          
                DecisionTag -= dx2;

            }
        }
    }

    if(ScreenHeight>ScreenWidth) Temp2 = ScreenHeight;
    else Temp2 = ScreenWidth;
    for( Temp1=0; Temp1<Temp2; Temp1++)
    {
        OneRowColorBuf[Temp1] = Color;
    }
    ColorBuf = OneRowColorBuf;
	
    IntersectionNum = CurrentIntersection;

    //边缘坐标按Y升序排序
   QuickSort2( (iS16(*)[2])IntersectionCoordinates , 0, CurrentIntersection-1);

    memset ( IntersectionX, 0 , MAX_LINE_POINTS*2 ) ;

    IntersectionNumberX = 0;
    StartY = IntersectionCoordinates[1];
    IntersectionX[0] = IntersectionCoordinates[0];
    for( CurrentIntersection=0; CurrentIntersection<IntersectionNum; CurrentIntersection++ )
    {
        if( StartY != IntersectionCoordinates[(CurrentIntersection<<1)+1] )
        {
            i51KitDrawDottedLine( StartY, IntersectionNumberX, IntersectionX, ColorBuf);
            IntersectionNumberX = 0;
            StartY = IntersectionCoordinates[(CurrentIntersection<<1)+1];
        }
        IntersectionX[IntersectionNumberX++] = IntersectionCoordinates[CurrentIntersection<<1];
        if( IntersectionNumberX>=MAX_LINE_POINTS) goto ERROR;
    }

    i51KitDrawDottedLine( StartY, IntersectionNumberX, IntersectionX, ColorBuf);

    i51AdeOsFree(IntersectionCoordinates);

 #if i51KITG2_DEBUG
    iLog( "G2FillPolygon end , %x, %x, %x", VerticeNumber,Point,Color) ;
#endif

    return iTRUE;
	
    ERROR:
       i51AdeOsFree(IntersectionCoordinates);
       IntersectionCoordinates = iNULL;
       return iFALSE;
    
}

iKIT iBOOL i51KitG2FillArc( iS32 Point[][2], iCOLOR Color )
{

    // author : Otto
    // since : Aug 15 2011
    // (C) 2011 , PKIG Tech. Co., Ltd.

    // iS32 Point[][2] :
    //    Point[0][0]是需要填充弧线的起点X坐标
    //    Point[0][1]是需要填充弧线的起点Y坐标
    //    Point[1][0]是经过弧线上任意点的X坐标
    //    Point[2][1]是经过弧线上任意点的Y坐标
    //    Point[2][0]是需要填充弧线的终点X坐标
    //    Point[2][1]是需要填充弧线的终点Y坐标
    // iCOLOR Color : 填充弧线的颜色

    // notes : 填充以(Point[0][0], Point[0][1])为起点,(Point[2][0],Point[2][1])为终点,经过点(Point[1][0],Point[1][1])的弧线
    // 已知ABC三点,求△ABC外接圆圆心的公式为：
    //    CenterX=(Qy+K1Px-K2Qx-Py)/(K2-K1)
    //    CenterY=(K1K2Qx+K2Py-K1Qy-K1K2Px)/(K2-K1)
    //    其中(Px, Py)是线段AB中点坐标, (Qx, Qy)是线段BC的中点坐标, K1是线段AB的中垂线斜率, K2是线段BC的中垂线斜率

    //计算X的绝对值
    #define ABSOLUTE(X) (X) > 0 ? (X) :(-(X))
    #define CLOCK_WISE    1
    #define ANTI_CLOCK_WISE 2

    iS32 OldX = 0;
    iS32 OldY = 0;
    iS32 CenterX = 0;
    iS32 CenterY = 0;
    iS32 SquareR = 0;
    iS32 CurrentX = 0;
    iS32 CurrentY = 0;
    iS32 Molecular = 0;
    iS32 Denominator = 0;
    iS16 RegulationY = 1;
    iS16 RegulationX = 1;
    iS32 Xa, Xb, Xc, Ya, Yb, Yc;
    iS32 SegmentEndpoints[2][2];
    iS32 PerpendicularIntersectionY = 0;
    iS32 SquareDifferential[3] = {0};
    iU8  AirDirection = CLOCK_WISE;

    // 当前绘制的元素在VP上的ID
    iU16 CurrDrawID;
    iS32 Temp1 = 0;
	
 #if i51KITG2_DEBUG
    iLog( "G2FillArc start , %x, %x", Point,Color) ;
#endif

    for( Temp1=0; Temp1<3; Temp1++ )
    {
        if( Point[Temp1][0]>2000||Point[Temp1][0]<-1500
         ||Point[Temp1][1]>2000||Point[Temp1][1]<-1500		
        )
        {
 #if i51KITG2_DEBUG
    iLog( "G2FillArc Point[%X]:%X , %x", Temp1,Point[Temp1][0],Point[Temp1][1]) ;
#endif
            return iFALSE;
        }
    }     	

    // 在虚拟面板上绘制，实际上只是进行一些赋值操作
    if( i51UseVirtualPanel && i51DrawVirtualPanel )
    {
        if( i51CurrentVpManage.TotalVPNumber == 0 ) return iFALSE;

        if( iNULL == i51CurrentVpActivity )	return iFALSE;
		
        CurrDrawID = i51CurrentVpActivity->VPCurrentElement;
        // 画板已经画满则直接返回
        if(  i51CurrentVpActivity->VpIsFull ) return iFALSE;
      
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementType = I51KITG2_ELEMENT_TYPE_FILLARC;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementNumber = CurrDrawID;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElemenAlpha = AlphaSys;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.FillArc.Points[0][0] = Point[0][0];
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.FillArc.Points[0][1] = Point[0][1];
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.FillArc.Points[1][0] = Point[1][0];
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.FillArc.Points[1][1] = Point[1][1];
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.FillArc.Points[2][0] = Point[2][0];
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.FillArc.Points[2][1] = Point[2][1];
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.FillArc.Color= Color;
        i51CurrentVpActivity->VPCurrentElement =  CurrDrawID +1;
        if( (CurrDrawID+1) >= i51CurrentVpActivity->VPMaxElement )	i51CurrentVpActivity->VpIsFull = iTRUE;
        return iTRUE;
    }


    Xa = Point[0][0];
    Ya = Point[0][1];
    Xb = Point[1][0];
    Yb = Point[1][1];
    Xc = Point[2][0];
    Yc = Point[2][1];
	

    //不在同一直线上三点才能确定一个圆,如果三点中有两点重合则无法确定圆
    if( Xa==Xb&&Ya==Yb ) return iFALSE;
    if( Xa==Xc&&Ya==Yc ) return iFALSE;
    if( Xb==Xc&&Yb==Yc ) return iFALSE;

    //三点在同一直线上无法确定圆
    if( Xa!=Xc && Xa!=Xb )
    {
        if( (((Ya-Yc)*100)/(Xa-Xc)) ==(((Ya-Yb)*100)/(Xa-Xb)) ) return iFALSE ;
    }
    if( Xa==Xb && Xa==Xc ) return iFALSE;
    if( Ya==Yb && Ya==Yc ) return iFALSE;

    //设置左边的顶点为圆弧的起点
    if( Xa > Xc )
    {
        Xa += Xc;
        Xc = Xa - Xc;
        Xa = Xa - Xc;
  
        Ya += Yc;
        Yc = Ya - Yc;
        Ya = Ya - Yc;
    }

    //计算圆心X坐标
    Molecular = (Yc-Ya)*(Yc-Yb)*(Yb-Ya) + ( Xa*Xa-Xb*Xb)*(Yc-Yb) + (Xc*Xc - Xb*Xb)*(Yb-Ya);
    Denominator = ( (Xb-Xc)*(Ya-Yb) + (Xb-Xa)*(Yb-Yc) ) << 1;
    CenterX = ((Molecular*10)/Denominator+5)/10;

    //计算圆心Y坐标
    Molecular = (Xb-Xa)*(Xc-Xb)*(Xc-Xa) + (Xb-Xc)*(Ya+Yb)*(Yb-Ya) + (Xb-Xa)*(Yb+Yc)*(Yc-Yb);
    Denominator = ( (Xb-Xc)*(Yb-Ya) + (Xb-Xa)*(Yc-Yb) ) << 1;
    CenterY = ((Molecular*10)/Denominator + 5 ) / 10;


    //计算圆半径的平方
    SquareR = ( Xc - CenterX ) * ( Xc - CenterX ) + ( Yc - CenterY ) * ( Yc - CenterY );


    //计算经过点B且与直线AC垂直的直线与直线AC的交点X坐标
    //  Molecular = (Yb-Ya)*(Xc-Xa)*(Ya-Yc) - (Xc-Xa)*(Xc-Xa)*Xb - (Yc-Ya)*(Yc-Ya) * Xa;
    //  Denominator = (Yc-Ya)*(Ya-Yc) - (Xc-Xa)*(Xc-Xa);
    Molecular = (Yc-Ya)*(Ya*Yb-Yb*Yc+Xa*Xb-Xc*Xb) + (Xa-Xc)*(Ya*Xc-Yc*Xa);
    Denominator = (Yc-Ya)*(Ya-Yc) - (Xc-Xa)*(Xc-Xa);
    PerpendicularIntersectionY =  ((Molecular*10)/Denominator + 5 ) / 10;

    //判断画圆弧方向,如果B点在垂点下方则圆弧方向为逆时针,否则为顺时针
    if( Yb > PerpendicularIntersectionY )
    {
        AirDirection = ANTI_CLOCK_WISE;
    }
    else if( Yb==PerpendicularIntersectionY )
    {
        if(  Xb<CenterX )
        {
            AirDirection = ANTI_CLOCK_WISE;
        }
    }

    //判断画圆弧方向为顺时针时X,Y的变化趋势,根据对称性,如果为逆时针则趋势是反向
    if ( Ya < CenterY )
    {
        RegulationX = 1;
    }
    else if ( Ya > CenterY )
    {
        RegulationX = -1;
    }

    if ( Xa < CenterX )
    {
        RegulationY = -1;
    }
    else if ( Xa > CenterX )
    {
        RegulationY = 1;
    }

    if( AirDirection == ANTI_CLOCK_WISE )
    {
        RegulationX = -RegulationX;
        RegulationY = -RegulationY;
    }

    if( Ya==CenterY)
    {
        if( Xa > CenterX )
        {
            RegulationX = -1;
        }
        else
        {
            RegulationX = 1;
        }
    }

    if( Xa==CenterX )
    {
        if( Ya > CenterY )
        {
            RegulationY = -1;
        }
        else
        {
            RegulationY = 1;
       }
    }

    CurrentX = Xa;
    CurrentY = Ya;

    SegmentEndpoints[0][0] = Xa;
    SegmentEndpoints[0][1] = Ya;

    while(1)
    {
        if( CurrentX==Xc&&CurrentY==Yc) break;

  
        OldX = CurrentX;
        OldY = CurrentY;

        SquareDifferential[0] = ABSOLUTE( (CurrentX+RegulationX-CenterX)*(CurrentX+RegulationX-CenterX)\
                                                         +(CurrentY-CenterY)*(CurrentY-CenterY) - SquareR);
        SquareDifferential[1] = ABSOLUTE( (CurrentY +RegulationY -CenterY)*(CurrentY +RegulationY -CenterY)\
                                                         +(CurrentX-CenterX)*(CurrentX-CenterX) - SquareR);
        SquareDifferential[2] = ABSOLUTE( (CurrentY +RegulationY -CenterY)*(CurrentY +RegulationY -CenterY)\
                                                         +(CurrentX+RegulationX-CenterX)*(CurrentX+RegulationX-CenterX) - SquareR);
        if( SquareDifferential[0]<SquareDifferential[1] )
       {
            if( SquareDifferential[0]<SquareDifferential[2] )
            {
                CurrentX += RegulationX ;
            }
            else
            {
                CurrentX += RegulationX ;
                CurrentY += RegulationY ;
            }
        }
        else
        {
            if( SquareDifferential[1]<SquareDifferential[2] )
            {
                CurrentY += RegulationY ;
            }
            else
            {
                CurrentX += RegulationX ;
                CurrentY += RegulationY ;
            }
        }

        SegmentEndpoints[1][0] = CurrentX;
        SegmentEndpoints[1][1] = CurrentY;
        i51KitG2DrawLine( SegmentEndpoints, Color );

        if( CurrentX==CenterX && OldX!=CurrentX )
        {
            RegulationY = -RegulationY;
        }

        if( CurrentY==CenterY && OldY!=CurrentY )
        {
            RegulationX = -RegulationX;
        }
    }

    #undef ABSOLUTE
    #undef CLOCK_WISE
    #undef ANTI_CLOCK_WISE
 #if i51KITG2_DEBUG
    iLog( "G2FillArc end , %x, %x", Point,Color) ;
#endif

    return iTRUE;

}

iKIT iBOOL i51KitG2FillCircle( iS32 X, iS32 Y, iU32 R, iCOLOR Color )
{

    // author : Otto
    // since : Aug 15 2011
    // (C) 2011 , PKIG Tech. Co., Ltd.

    // iS32 X : 圆心X坐标
    // iS32 Y : 圆心Y坐标
    // iU32 R : 圆半径
    // iCOLOR Color : 填充圆的颜色

    // notes : 填充以(X, Y)为圆心,半径为R的圆
    // 根据圆的对称性，我们找到圆上一点可以确定与之X轴对称、Y轴对称已经中心对称的另外三点
    // 所以计算时只需要通过计算确认一个象限内圆上的点,其他三个象限的点可以根据对称性直接获得
    // 此算法采用的是空间优先算法

    #define ABSOLUTE(X) (X) > 0 ? (X) :(-(X))

    iS32 SquareR = 0;
    iS32 OldX = 0;
    iS32 OldY = 0;
    iS32 EndX = 0;
    iS32 EndY = 0;
    iS32 CenterX = X;
    iS32 CenterY = Y;
    iS32 Center2X = X+X;
    iS32 Center2Y = Y+Y;
    iS32 CurrentX = 0;
    iS32 CurrentY = 0;
    iS32 SymmetryPoints[3][2] ;
    iS32 SquareDifferential[3] = {0};
    iS32 Point[2][2];

    iS32 DrawX = 0, DrawY = 0;  
	
    // 当前绘制的元素在VP上的ID
    iU16 CurrDrawID;
    // 创建的VP实际大小	
    iRECT VPActualRect;
    // 需要绘制的VP区域	
    iRECT VPDrawRect;

    iCOLOR * ColorBuf = iNULL;
    iS16 Temp1 = 0;//, Temp2 = 0;
 #if i51KITG2_DEBUG
    iLog( "G2FillCircle start , %x, %x, %x, %x",X,Y, R,Color) ;
#endif

    if( X>2000||X<-1500 ||Y>2000||Y<-1500||R>500 )
    {
#if i51KITG2_DEBUG
iLog( "G2FillCircle: %X,%X,%X", X,Y,R) ;
#endif
        return iFALSE;
    }


    // 在虚拟面板上绘制，实际上只是进行一些赋值操作
    if( i51UseVirtualPanel && i51DrawVirtualPanel )
    {
        if( i51CurrentVpManage.TotalVPNumber == 0 ) return iFALSE;

        if( iNULL == i51CurrentVpActivity )	return iFALSE;
		
        CurrDrawID = i51CurrentVpActivity->VPCurrentElement;
        // 画板已经画满则直接返回
        if(  i51CurrentVpActivity->VpIsFull ) return iFALSE;
      
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementType = I51KITG2_ELEMENT_TYPE_FILLCRICLE;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementNumber = CurrDrawID;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElemenAlpha = AlphaSys;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.FillCircle.X = X;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.FillCircle.Y = Y;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.FillCircle.R = R;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.FillCircle.Color= Color;
        i51CurrentVpActivity->VPCurrentElement =  CurrDrawID +1;
        if( (CurrDrawID+1) >= i51CurrentVpActivity->VPMaxElement )	i51CurrentVpActivity->VpIsFull = iTRUE;
        return iTRUE;
    }

    if( i51UseVirtualPanel )
    {
        VPActualRect.X = i51CurrentVpRender->VPActualRect.X;
        VPActualRect.Y = i51CurrentVpRender->VPActualRect.Y;
        VPActualRect.Width= i51CurrentVpRender->VPActualRect.Width;
        VPActualRect.Height= i51CurrentVpRender->VPActualRect.Height;

        VPDrawRect.X = i51CurrentVpRender->VPDrawRect.X;
        VPDrawRect.Y = i51CurrentVpRender->VPDrawRect.Y;
        VPDrawRect.Width = i51CurrentVpRender->VPDrawRect.Width;
        VPDrawRect.Height= i51CurrentVpRender->VPDrawRect.Height;
  
        DrawX = X + VPActualRect.X;
        DrawY = Y + VPActualRect.Y;
    }
    else
    {
        VPActualRect.X = 0;
        VPActualRect.Y = 0;
        VPActualRect.Width = ScreenWidth;
        VPActualRect.Height = ScreenHeight;

        VPDrawRect.X = 0;
        VPDrawRect.Y = 0;
        VPDrawRect.Width = ScreenWidth;
        VPDrawRect.Height = ScreenHeight;
		
        DrawX = X;
        DrawY = Y;
    }

//    Temp2 = R + R ;
	if(Color!=OneRowColorBuf[0])
	{
	    for( Temp1=0; Temp1<ScreenWidth; Temp1++)
	    {
	        OneRowColorBuf[Temp1] = Color;
	    }
	}
    ColorBuf = OneRowColorBuf;

    SquareR = R * R;

    CurrentX = DrawX - R;
    CurrentY = DrawY;

    EndX = DrawX;
    EndY = DrawY - R;

    CenterX = DrawX;
    CenterY = DrawY;
    Center2X = DrawX+DrawX;
    Center2Y = DrawY+DrawY;

    //绘制与X轴平行的直径
    Point[0][0] = CurrentX;
    Point[0][1] = CurrentY;
    Point[1][0] = CurrentX + R + R;
    Point[1][1] = CurrentY;
    i51KitDrawWaterLine(Point, VPDrawRect, VPActualRect, ColorBuf);

    while(1)
    {
        if( CurrentX==EndX && CurrentY==EndY )
        {
            break;
        }

        OldX = CurrentX;
        OldY = CurrentY;

        SquareDifferential[0] = ABSOLUTE( (CurrentX+1-CenterX)*(CurrentX+1-CenterX)\
                                                         +(CurrentY-CenterY)*(CurrentY-CenterY) - SquareR);
        SquareDifferential[1] = ABSOLUTE( (CurrentY - 1 -CenterY)*(CurrentY - 1 -CenterY)\
                                                         +(CurrentX-CenterX)*(CurrentX-CenterX) - SquareR);
        SquareDifferential[2] = ABSOLUTE( (CurrentY -1  -CenterY)*(CurrentY -1 -CenterY)\
                                                         +(CurrentX+1-CenterX)*(CurrentX+1-CenterX) - SquareR);
        if( SquareDifferential[0]<SquareDifferential[1] )
        {
            if( SquareDifferential[0]<SquareDifferential[2] )
            {
                CurrentX ++ ;
            }
            else
            {
                CurrentX ++ ;
                CurrentY -- ;
            }
        }
        else
        {
            if( SquareDifferential[1]<SquareDifferential[2] )
            {
                CurrentY -- ;
            }
            else
            {
                CurrentX ++ ;
                CurrentY -- ;
            }
        }

        SymmetryPoints[0][0] = Center2X - CurrentX;
        SymmetryPoints[0][1] = CurrentY;
        SymmetryPoints[1][0] = SymmetryPoints[0][0];
        SymmetryPoints[1][1] = Center2Y - CurrentY;
        SymmetryPoints[2][0] = CurrentX;
        SymmetryPoints[2][1] = SymmetryPoints[1][1];

        //绘制一三象限的图像
        Point[0][0] = CurrentX;
        Point[0][1] = CurrentY;
        Point[1][0] = SymmetryPoints[0][0];
        Point[1][1] = CurrentY;
        i51KitDrawWaterLine(Point, VPDrawRect, VPActualRect, ColorBuf);


        //绘制二四象限的图像
        Point[0][0] = SymmetryPoints[2][0];
        Point[0][1] = SymmetryPoints[2][1];
        Point[1][0] = SymmetryPoints[1][0];
        Point[1][1] = SymmetryPoints[1][1];
        i51KitDrawWaterLine(Point, VPDrawRect, VPActualRect, ColorBuf);


    }

    #undef ABSOLUTE
 #if i51KITG2_DEBUG
    iLog( "G2FillCircle end , %x, %x, %x, %x",X,Y, R,Color) ;
#endif
    return iTRUE;
}

iKIT iBOOL i51KitG2CreateImageContainer ( iFILE File , iU32* ImageId , iU32 TotallImage , iU16* ContainerPathName, 
	void(*Process)( iU32 , iU32  , iHANDLE) , void (*Done)(iHANDLE) )
{

    // File		文件指针
    // ImageId		图像资源ID数组
    // TotallImage		图像资源ID总数
    // ContainerPathName 创建的容器文件完整文件名
    // Process		回调函数
    // Done		回调函数

    // 最大图片ID
    iU32 MaxImageID=0;
    iU32 Temp = 0;

 #if i51KITG2_DEBUG
    iLog( "G2CreateImageContainer start , %x, %x, %x, %x, %x, %x",
    File,ImageId, TotallImage,ContainerPathName, Process, Done) ;
#endif

    if( iNULL != TimeIdZlibDecodeImage) return iTRUE;

    if(iNULL==File||iNULL==ImageId||TotallImage==0||iNULL==ContainerPathName||iNULL==Process||iNULL==Done)
    {
        iLog("KIT - G2 : Creat container par");
        return iFALSE;
    }

    ManageZlibDecodeImage.DesFileName[0] = '\0';
    MMIAPICOM_Wstrcat( ManageZlibDecodeImage.DesFileName, ContainerPathName);
    ManageZlibDecodeImage.CompleteImage = 0;
    ManageZlibDecodeImage.Done = Done;
    ManageZlibDecodeImage.GoingImageID = ImageId[0];
    ManageZlibDecodeImage.ImageId = (iU32 )i51AdeOsMalloc( sizeof(iU32)*TotallImage,   __FILE__ , __LINE__ );
    if( ManageZlibDecodeImage.ImageId == 0) return iFALSE;
    i51AdeStdMemcpy16( (void *)(ManageZlibDecodeImage.ImageId), ImageId, sizeof(iU32)*TotallImage);
    ManageZlibDecodeImage.Process = Process;
    ManageZlibDecodeImage.SrcFile = File;
    ManageZlibDecodeImage.TotallImage = TotallImage;

    for( Temp = 0; Temp<TotallImage; Temp++ )
    {
        if( ImageId[Temp]>MaxImageID)
        {
            MaxImageID = ImageId[Temp];
        }
    }
    ManageZlibDecodeImage.MaxImageID = MaxImageID+1;

    // 创建解码定时器	
    TimeIdZlibDecodeImage = i51AdeOsTimerCreate( 1,1);
     FlagDecodeImage = iTRUE;
     ZlibImageContainer = iNULL;
	 
    i51AdeOsTimerStart( TimeIdZlibDecodeImage, ZlibDecodeAllImage, 30);

  #if i51KITG2_DEBUG
    iLog( "G2CreateImageContainer end , %x, %x, %x, %x, %x, %x",
    File,ImageId, TotallImage,ContainerPathName, Process, Done) ;
#endif
   return iTRUE;
}
	
iKIT iBOOL i51KitG2ContainerImageIdExist( iHANDLE ContainerHandle, iU32 ImageId)
{
    // author : Otto
    // since : 2011.12.29
    // (C) 2011 , PKIG Tech. Co., Ltd.

    // notes : 判断指定的ID号在容器文件中是否存在相对于的图片
    // 存在返回iTRUE，否则返回iFALSE

    // 图片容器地址入口
    i51ImageContainer * ImageContainer = iNULL;

    ImageContainer = (i51ImageContainer * )ContainerHandle;
	
    if( (iNULL==ImageContainer) || (ImageId>=ImageContainer->ImageMaxId) ) return iFALSE;

    if(ImageContainer->ImageIndexTable[ImageId].FlagImageExist ) return iTRUE;
    else return iFALSE;
	
}

iKIT iBOOL i51KitG2ContainerDeleteImage( iHANDLE ContainerHandle, iU32 ImageId)
{
    // author : Otto
    // since : 2011.12.29
    // (C) 2011 , PKIG Tech. Co., Ltd.

    // notes : 删除容器文件中指定ID号的图片文件

    // 图片容器地址入口
    i51ImageContainer * ImageContainer = iNULL;
    iFILE FileHandle = iNULL;
    iU8 Data[5] = {0};	

    ImageContainer = (i51ImageContainer * )ContainerHandle;
    FileHandle = ImageContainer->FileHandle;
	
    if( (iNULL==ImageContainer) || (iNULL==FileHandle) ||(ImageId>=ImageContainer->ImageMaxId) ) return iFALSE;

    if(iTRUE== ImageContainer->ImageIndexTable[ImageId].FlagImageExist )
    {
        memset(Data,0,5);
        if( iFALSE==i51AdeOsFileSetSeek( FileHandle, I51KITG2_OFFSET_START_DATA+ImageId*5, i51_OS_FL_SEEK_HEAD) )
        {
            return iFALSE;
        }

        if( iFALSE==i51AdeOsFileWrite(FileHandle, Data, 5, iNULL))
        {
            return iFALSE;
        }

        ImageContainer->ImageIndexTable[ImageId].FlagImageExist = iFALSE;
    }

    return iTRUE;
	
}

iKIT iBOOL i51KitG2UpdataContainerFromImage ( iHANDLE ContainerHandle , void * ImageMess, iU8 MessType, iU32 DesImageId)
{
    // author : Otto
    // since : 2011.12.29
    // (C) 2011 , PKIG Tech. Co., Ltd.

    // notes : 更新容器文件中的图片


    // 图片容器地址入口
    i51ImageContainer * ImageContainer = iNULL;

    iFILE SrcFile = iNULL;
    iFILE DesFile = iNULL;
    iU32 DesSeek = 0;
    iU32 SrcSeek = 0;
    iU8 FlagReturn = iFALSE;

    if(iNULL==(iU32)ImageMess) return iFALSE;

    ImageContainer = (i51ImageContainer * )ContainerHandle;
	
    if( (iNULL==ImageContainer) || (DesImageId>=ImageContainer->ImageMaxId) ) return iFALSE;

    DesFile = ImageContainer->FileHandle;
	
    // 移动文件内指针到容器文件末尾
    FlagReturn = i51AdeOsFileSetSeek( DesFile, 0, i51_OS_FL_SEEK_TAIL) ;
    if( iFALSE == FlagReturn ) goto ERROR;
	
    // 获得当前容器文件内指针位置
    FlagReturn = i51AdeOsFileGetSeek( DesFile , (iS32 *)(&DesSeek), i51_OS_FL_SEEK_HEAD ) ;
    if( iFALSE == FlagReturn ) goto ERROR;

    if( MessType==i51KitG2_MESSAGE_TYPE_FILE )
    {
        SrcFile = (iFILE)(ImageMess);
        SrcSeek =  0;

        if( iFALSE == ZlibDecodeOneImageFromFile( SrcFile , SrcSeek, DesFile, DesSeek, ContainerHandle, DesImageId ) );
        {
		goto ERROR;
        }
    }
    else
    {
        if( iFALSE==ZlibDecodeOneImageFromMemory(ImageMess, DesFile, DesSeek, ContainerHandle, DesImageId) )
        {
            goto ERROR;
        }
    }

    return iTRUE;

ERROR:
    iLog("KIT - G2 : updata C I");
    return iFALSE;
	
}

iKIT iBOOL i51KitG2UpdataContainerFromRe ( iHANDLE ContainerHandle , void * ReMess, iU32 SrcImageId , iU8 MessType, iU32 DesImageId)
{
    // author : Otto
    // since : 2011.12.29
    // (C) 2011 , PKIG Tech. Co., Ltd.

    // notes : 更新容器文件中的图片

    // 图片容器地址入口
    i51ImageContainer * ImageContainer = iNULL;


    iFILE SrcFile = iNULL;
    iFILE DesFile = iNULL;
    iU32 DesSeek = 0;
    iU32 SrcSeek = 0;
    iU8 FlagReturn = iFALSE;
    iU32 CurrDataPos = 0;
    iU8 * SrcData = iNULL;	
    iU32 TextCount = 0 ;
    iU32 BinCount = 0 ;
	
    if(iNULL==(iU32)ReMess) return iFALSE;

    ImageContainer = (i51ImageContainer * )ContainerHandle;

    if( (iNULL==ImageContainer) || (DesImageId>=ImageContainer->ImageMaxId) ) return iFALSE;

    DesFile = ImageContainer->FileHandle;

    // 移动文件内指针到容器文件末尾
    FlagReturn = i51AdeOsFileSetSeek( DesFile, 0, i51_OS_FL_SEEK_TAIL) ;
    if( iFALSE == FlagReturn ) goto ERROR;
	
    // 获得当前容器文件内指针位置
    FlagReturn = i51AdeOsFileGetSeek( DesFile , (iS32 *)(&DesSeek), i51_OS_FL_SEEK_HEAD ) ;
    if( iFALSE == FlagReturn ) goto ERROR;

    if( MessType==i51KitG2_MESSAGE_TYPE_FILE )
    {
        SrcFile = (iFILE)(ReMess);
        SrcSeek =  i51KitStdResGetContentEntry(SrcFile,SrcImageId);
		
        if( iFALSE == ZlibDecodeOneImageFromFile( SrcFile , SrcSeek, DesFile, DesSeek, ContainerHandle, DesImageId ) )
        {
            goto ERROR;
        }
		
    }
    else
    {
        CurrDataPos = 14;
        
        SrcData= (iU8*)((iU8*)ReMess+CurrDataPos);
       
        TextCount = SrcData[0] | (SrcData[1]<<8)| (SrcData[2]<<16)| (SrcData[3]<<24);
        BinCount = SrcData[4] | (SrcData[5]<<8)| (SrcData[6]<<16)| (SrcData[7]<<24);
        
        if ( SrcImageId+ 1 > BinCount  )  goto ERROR;

        CurrDataPos = 22 + 40 * (TextCount+ SrcImageId )+ 32;

        SrcData= (iU8*)((iU8*)ReMess+CurrDataPos);
        CurrDataPos= SrcData[0] | (SrcData[1]<<8)| (SrcData[2]<<16)| (SrcData[3]<<24);

        SrcData= (iU8*)((iU8*)ReMess+CurrDataPos);
        if( iFALSE==ZlibDecodeOneImageFromMemory(SrcData, DesFile, DesSeek, ContainerHandle, DesImageId) )
        {
            goto ERROR;
        }
    }
	

    return iTRUE;
	
    ERROR:
        iLog("KIT - G2 : updata C R");
        return iFALSE;
    
}

iKIT iBOOL i51KitG2GetContainerAttr(iHANDLE ContainerHandle, iU32 * TotalCapacity, iU32 * UsedCapacity, iU32 * ResidueCapacity, iU32 * UsableMinID )
{
    // author : Otto
    // since : 2011.12.29
    // (C) 2011 , PKIG Tech. Co., Ltd.

    // notes : 获得容器属性

    // 图片容器地址入口
    i51ImageContainer * ImageContainer = iNULL;
    iU32 TotalCapacityNum = 0;
    iU32 UsedCapacityNum = 0;
    iU32 ResidueCapacityNum = 0;
    iU32 UsableMinIDNum = 0;
    iU32 Temp = 0, UsedTemp = 0, ResidueTemp = 0 ;	
    i51ImageIndex * ImageIndex = iNULL;
	
    ImageContainer = (i51ImageContainer * )ContainerHandle;
	
    if( (iNULL==ImageContainer)) return iFALSE;

    ImageIndex = (i51ImageIndex *)ImageContainer->ImageIndexTable;
    if( (iNULL==ImageIndex)) return iFALSE;
	
    TotalCapacityNum = ImageContainer->ImageMaxId;
    UsedCapacityNum = ImageContainer->ImageTotalNumber;
    ResidueCapacityNum = TotalCapacityNum - UsedCapacityNum;

    for( Temp=0; Temp<TotalCapacityNum; Temp++ )
    {
        if(ImageIndex[Temp].FlagImageExist ) 
        {
            UsedTemp ++;
        }
        else
        {
            if(ResidueTemp==0)
            {
                UsableMinIDNum = Temp;
            }
            ResidueTemp++;
        }
    }

    if( (UsedCapacityNum!=UsedTemp) || (ResidueCapacityNum!=ResidueTemp) ) return iFALSE;

    if(TotalCapacity) *TotalCapacity = TotalCapacityNum;
    if(UsedCapacity) *UsedCapacity = UsedCapacityNum;
    if(ResidueCapacity) *ResidueCapacity = ResidueCapacityNum;
    if(UsableMinID&&ResidueCapacityNum) *UsableMinID = UsableMinIDNum;

    return iTRUE;
	 
}

iKIT iBOOL i51KitG2CloseImageContainer ( iHANDLE Handle)
{

    // author : Otto
    // since : Aug 20 2011
    // (C) 2011 , PKIG Tech. Co., Ltd.

    //  iHANDLE ImageContainer ：容器文件指针

    // notes : 释放容器文件占的资源并关闭容器

    // 图片容器地址入口
    i51ImageContainer * CloseImageContainer = iNULL;
    // 文件操作函数返回值
    iU8   FlagReturn = iFALSE;
    // 图片索引表地址
    i51ImageIndex * ImageIndexTable = iNULL;
    iU32 AllImageNum = 0;
    iU32 Temp;

 #if i51KITG2_DEBUG
    iLog( "G2CloseImageContainer start , %x",Handle);
#endif

    CloseImageContainer = (i51ImageContainer*)Handle;

    if( CloseImageContainer==iNULL ) return iFALSE;

    Temp = 0;
    while(1)
    {
        if( CloseImageContainer==ContainerManage[Temp].ContainerHandle && ContainerManage[Temp].Flag )
        {
//iLog("close ContainerManage[%d].ContainerHandle:%d",Temp,ContainerManage[Temp].ContainerHandle);
    	     ContainerManage[Temp].Flag = iFALSE;
            ContainerManage[Temp].ContainerHandle = iNULL;
            if ( ContainerManage[Temp].ImageID )
            {
                i51AdeOsFree( ContainerManage[Temp].ImageID );
                ContainerManage[Temp].ImageID = iNULL;
            }

            if ( ContainerManage[Temp].MemCopyCount)
            {
                i51AdeOsFree( ContainerManage[Temp].MemCopyCount);
                ContainerManage[Temp].MemCopyCount= iNULL;

            }

            break;
        }
        Temp++;
        if( Temp>=MAX_ContainerManage_NUM )		
        {
            iLog("KIT - G2 : close handle err:%X",Handle);
            return iFALSE;			
        }
    }
	
    ImageIndexTable = CloseImageContainer->ImageIndexTable;
    if( iNULL==ImageIndexTable ) return iFALSE;
	
    AllImageNum = 	CloseImageContainer->ImageMaxId;

    for( Temp=0; Temp<AllImageNum; Temp++ )
    {
        if( ImageIndexTable[Temp].FlagImageExist==0 ) continue;
        if( ImageIndexTable[Temp].FlagCopyRAM )
        {
            FlagReturn = i51AdeOsFree( (void *)(ImageIndexTable[Temp].MemoryEntry));
            ImageIndexTable[Temp].MemoryEntry = iNULL;
            ImageIndexTable[Temp].FlagCopyRAM = iFALSE;
            if( iFALSE == FlagReturn ) return iFALSE;
        }
    }
	
    if( CloseImageContainer->ImageIndexTable!=iNULL )
    {
        FlagReturn = i51AdeOsFree( CloseImageContainer->ImageIndexTable );
        CloseImageContainer->ImageIndexTable = iNULL;
        if( iFALSE == FlagReturn ) goto ERROR;;
    }

    if( CloseImageContainer->FileHandle )
    {
        FlagReturn = i51AdeOsFileClose( CloseImageContainer->FileHandle );
        if( iFALSE == FlagReturn )  goto ERROR;
        CloseImageContainer->FileHandle = iNULL;
    }

    i51AdeOsFree(CloseImageContainer);
    CloseImageContainer = iNULL;

	if(FlagDecodeImage && Handle==(iHANDLE)(ZlibImageContainer) )
	{
           if( i51AdeOsFileIsExist(ManageZlibDecodeImage.DesFileName) )
           {
               i51AdeOsFileDelete(ManageZlibDecodeImage.DesFileName);
           }

	    if( ManageZlibDecodeImage.ImageId )
	    {
	    	i51AdeOsFree((void*)ManageZlibDecodeImage.ImageId );
		ManageZlibDecodeImage.ImageId  = iNULL;
	    }
   			
	}
 #if i51KITG2_DEBUG
    iLog( "G2CloseImageContainer end , %x",Handle);
#endif
    return iTRUE;

ERROR:
    iLog("KIT - G2 : close container");
    return iFALSE;
}

iKIT iBOOL i51KitG2CheckImageContainer ( iU16* ContainerPathName )
{
    // author : Otto
    // since : Aug 20 2011
    // (C) 2011 , PKIG Tech. Co., Ltd.

    //  iU16* path ：容器文件所在路径指针
    //  iU16 * Name : 容器文件名

    // notes : 判断容器文件是否合法

    // 打开的i51IMIC文件指针
    iFILE Fp_i51IMIC = iNULL;
    // i51IMIC完整文件路径加文件名
    iU16 Name_i51IMIC[50] = { 0 } ;

 #if i51KITG2_DEBUG
    iLog( "G2CheckImageContainer start , %x",ContainerPathName);
#endif

	
    MMIAPICOM_Wstrcat( Name_i51IMIC, ContainerPathName);

    // 检测需要打开的容器文件是否存在
    if( !i51AdeOsFileIsExist(Name_i51IMIC) )   return iFALSE;

    // 打开已经存在的容器文件
    Fp_i51IMIC =  i51AdeOsFileOpen( Name_i51IMIC , i51_OS_FL_READ | i51_OS_FL_EXISTING_OPEN, __FILE__ , __LINE__ );
    if( Fp_i51IMIC==0 )
    {
        iLog("KIT - G2 : open check container");
        return iFALSE;
    }

    if( !i51KitG2CheckFileCRC(Fp_i51IMIC)) 
    {
        i51AdeOsFileClose(Fp_i51IMIC);
        return iFALSE;
    }

    if( !i51AdeOsFileClose(Fp_i51IMIC)) return iFALSE;

 #if i51KITG2_DEBUG
    iLog( "G2CheckImageContainer end , %x",ContainerPathName);
#endif
    return iTRUE;

}

static iHANDLE i51KitG2CreatEmptyImageContainer( iU16* ContainerPathName)
{
    // author : Otto
    // since : Aug 20 2011
    // (C) 2011 , PKIG Tech. Co., Ltd.

    //  iU16* path ：容器文件所在路径指针
    //  iU16 * Name : 容器文件名

    // notes : 打开图像容器文件
    // 成功打开返回该容器文件的指针,否则返回iNULL

    // 打开的i51IMIC文件指针
    iFILE Fp_i51IMIC = iNULL;
    // 图片索引表地址
    i51ImageIndex * ImageIndexTable = iNULL;
    // 图片容器地址入口
    i51ImageContainer * ImageContainerEntry = iNULL;
    // 文件操作函数返回值
    iU8   FlagReturn = iFALSE;
    // 检测文件格式的临时变量
    iU8   FlagFile[4] = {'i','5','1','C'};
    // 从文件中读取的数据实际长度
//    iU32 LengthReadData = 0;
//    iU32 HeadDataPos = 0;	
//    iU32 iTemp = 0;
    iU32 CRC = 1;
    iU8 ContainerVer = I51KITG2_IMAGE_CONTAINER_VERSION;
    iU32 FileVer = 0;
//    iU8   DataModle = 0;

    // 检测需要打开的容器文件是否存在
    if( i51AdeOsFileIsExist(ContainerPathName) ) 
    {
        return iFALSE;
    }

    // 打开已经存在的容器文件
    Fp_i51IMIC =  i51AdeOsFileOpen( ContainerPathName, i51_OS_FL_WRITE| i51_OS_FL_ALWAYS_CREATE, __FILE__ , __LINE__ );
    if( Fp_i51IMIC==0 ) goto ERROR;

    // 写CRC
    FlagReturn = i51AdeOsFileWriteInt32( Fp_i51IMIC, &(CRC) );
    if( (iFALSE == FlagReturn))    goto ERROR;

    // 写入文件标示符
    FlagReturn = i51AdeOsFileWrite( Fp_i51IMIC, FlagFile, 4, iNULL);   
    if( (iFALSE == FlagReturn))    goto ERROR;
	
    // 开辟内存存放容器，并初始化容器  
    ImageContainerEntry = ( i51ImageContainer * )i51AdeOsMalloc( sizeof(i51ImageContainer),   __FILE__ , __LINE__ );
    if( iFALSE == ImageContainerEntry ) goto ERROR;

    ImageContainerEntry->FileHandle = Fp_i51IMIC;
    ImageContainerEntry->ImageIndexTable = iNULL;
    ImageContainerEntry->ImageTotalNumber = 0;
    ImageContainerEntry->ImageMaxId = 256;

    FlagReturn = i51AdeOsFileWriteInt8( Fp_i51IMIC, &ContainerVer );
    if( iFALSE == FlagReturn) goto ERROR;

    FlagReturn = i51AdeOsFileWriteInt32( Fp_i51IMIC, &FileVer );
    if( iFALSE == FlagReturn  ) goto ERROR;

    FlagReturn = i51AdeOsFileWriteInt16( Fp_i51IMIC, &(ImageContainerEntry->ImageTotalNumber) );
    if( iFALSE == FlagReturn ) goto ERROR;

    FlagReturn = i51AdeOsFileWriteInt16( Fp_i51IMIC, &(ImageContainerEntry->ImageMaxId) );
    if( iFALSE == FlagReturn ) goto ERROR;

    // 开辟容器索引表内存空间
    ImageIndexTable = ( i51ImageIndex * )i51AdeOsMalloc( (sizeof(i51ImageIndex))*(ImageContainerEntry->ImageMaxId), __FILE__ , __LINE__ );
    if( iFALSE == ImageIndexTable ) goto ERROR;

    memset(ImageIndexTable,0,((sizeof(i51ImageIndex))*(ImageContainerEntry->ImageMaxId)));

    FlagReturn = i51AdeOsFileWrite( Fp_i51IMIC, ImageIndexTable, (sizeof(i51ImageIndex))*(ImageContainerEntry->ImageMaxId), 0);
    if( iFALSE == FlagReturn ) goto ERROR;

    ImageContainerEntry->ImageIndexTable = ImageIndexTable;

    i51AdeOsFileFlush(Fp_i51IMIC);
	
    return ImageContainerEntry;
	
    ERROR:

        i51AdeOsFileClose( Fp_i51IMIC );
        i51AdeOsFree(ImageContainerEntry);
        ImageContainerEntry = iNULL;
        i51AdeOsFree(ImageIndexTable);
        ImageIndexTable = iNULL;
        iLog("KIT - G2 : RW empty contain");
        return iFALSE;

}

iKIT iHANDLE i51KitG2OpenImageContainer ( iU16* ContainerPathName)
{
    // author : Otto
    // since : Aug 20 2011
    // (C) 2011 , PKIG Tech. Co., Ltd.

    //  iU16* path ：容器文件所在路径指针
    //  iU16 * Name : 容器文件名

    // notes : 打开图像容器文件
    // 成功打开返回该容器文件的指针,否则返回iNULL

    // 打开的i51IMIC文件指针
    iFILE Fp_i51IMIC = iNULL;
    // i51IMIC完整文件路径加文件名
    iU16 Name_i51IMIC[50] = { 0 } ;
    // 图片索引表地址
    i51ImageIndex * ImageIndexTable = iNULL;
    // 图片容器地址入口
    i51ImageContainer * ImageContainerEntry = iNULL;
    // 文件操作函数返回值
    iU8   FlagReturn = iFALSE;
    // 检测文件格式的临时变量
    iU8   FlagFile[4] = {0};
    // 文件头当前偏移位置
//    iU32 FileHeadOffset = 0;
    // 当前图片文件的数据长度
//    iU32 CurrentDataLength = 0;
    // 从文件中读取的数据实际长度
    iU32 LengthReadData = 0;
    iU32 HeadDataPos = 0;	
    iU32 iTemp = 0;
    iU32 CRC = 0;
    iU8 ContainerVer = 0;
    iU32 FileVer = 0;
//    iU8   DataModle = 0;
    iU8 * HeadDataBuff = iNULL;
    iU8 * ImageDataBuff = iNULL;

 #if i51KITG2_DEBUG
    iLog( "G2OpenImageContainer start , %x",ContainerPathName);
#endif
	
    MMIAPICOM_Wstrcat( Name_i51IMIC, ContainerPathName);
    // 检测需要打开的容器文件是否存在
    if( !i51AdeOsFileIsExist(Name_i51IMIC) ) 
    {
        ImageContainerEntry = i51KitG2CreatEmptyImageContainer(Name_i51IMIC);
        if( ImageContainerEntry )
        {
            iTemp = 0;	 
            while(1)
            {
                if( iFALSE==ContainerManage[iTemp].Flag )
                {
                    ContainerManage[iTemp].Flag = iTRUE;
                    ContainerManage[iTemp].ContainerHandle = ImageContainerEntry;
                    break;
                }
                iTemp++;
                if( iTemp>=MAX_ContainerManage_NUM )		
                {
                    iLog("KIT - G2 : contain full");
                    i51KitG2CloseImageContainer((iHANDLE)(ImageContainerEntry));
                    return iFALSE;
                }
            }
            return ImageContainerEntry;
        }
        else return iFALSE;
    }

    // 打开已经存在的容器文件
    Fp_i51IMIC =  i51AdeOsFileOpen( Name_i51IMIC , i51_OS_FL_WRITE| i51_OS_FL_ALWAYS_OPEN, __FILE__ , __LINE__ );
    if( Fp_i51IMIC==0 ) goto ERROR;

    // 读CRC
    FlagReturn = i51AdeOsFileReadInt32( Fp_i51IMIC, &(CRC) );
    if( (iFALSE == FlagReturn))    goto ERROR;

    // 检测打开的容器里面存放的文件是否为"i51G"或者"i51C"格式
    FlagReturn = i51AdeOsFileRead( Fp_i51IMIC, FlagFile, 4, &LengthReadData);   
    if( (iFALSE == FlagReturn) || (LengthReadData!=4) || (FlagFile[0]!='i')
	|| (FlagFile[1]!='5') || (FlagFile[2]!='1') ||(FlagFile[3]!='G'&&FlagFile[3]!='C') )
    goto ERROR;
	
    // 开辟内存存放容器，并初始化容器  
    ImageContainerEntry = ( i51ImageContainer * )i51AdeOsMalloc( sizeof(i51ImageContainer),   __FILE__ , __LINE__ );
    if( iFALSE == ImageContainerEntry ) goto ERROR;

    ImageContainerEntry->FileHandle = Fp_i51IMIC;
    ImageContainerEntry->ImageIndexTable = iNULL;
    ImageContainerEntry->ImageTotalNumber = 0;
    ImageContainerEntry->ImageMaxId = 0;

    FlagReturn = i51AdeOsFileReadInt8( Fp_i51IMIC, &ContainerVer );
    if( iFALSE == FlagReturn && ContainerVer <= I51KITG2_IMAGE_CONTAINER_VERSION ) goto ERROR;

    FlagReturn = i51AdeOsFileReadInt32( Fp_i51IMIC, &FileVer );
    if( iFALSE == FlagReturn  ) goto ERROR;

    FlagReturn = i51AdeOsFileReadInt16( Fp_i51IMIC, &(ImageContainerEntry->ImageTotalNumber) );
    if( iFALSE == FlagReturn ) goto ERROR;

    FlagReturn = i51AdeOsFileReadInt16( Fp_i51IMIC, &(ImageContainerEntry->ImageMaxId) );
    if( iFALSE == FlagReturn ) goto ERROR;

    // 开辟容器索引表内存空间
    ImageIndexTable = ( i51ImageIndex * )i51AdeOsMalloc( (sizeof(i51ImageIndex))*(ImageContainerEntry->ImageMaxId), __FILE__ , __LINE__ );
    if( iFALSE == ImageIndexTable ) goto ERROR;

    HeadDataBuff = (iU8*)i51AdeOsMalloc((ImageContainerEntry->ImageMaxId*5),  __FILE__ , __LINE__ );
    if( iFALSE == HeadDataBuff) goto ERROR;

    FlagReturn = i51AdeOsFileRead( Fp_i51IMIC, HeadDataBuff, (ImageContainerEntry->ImageMaxId*5), iNULL);
    if( iFALSE == FlagReturn ) goto ERROR;

    ImageDataBuff = (iU8*)i51AdeOsMalloc(I51KITG2_OFFSET_PIXELDATA,  __FILE__ , __LINE__ );
    if( iFALSE == ImageDataBuff) goto ERROR;

    // 初始化容器索引表
    HeadDataPos = 0;
    for( iTemp=0; iTemp<ImageContainerEntry->ImageMaxId; iTemp++ )
    {
        ImageIndexTable[iTemp].FlagCopyRAM = iFALSE;

        ImageIndexTable[iTemp].FlagImageExist = HeadDataBuff[HeadDataPos];

        if(ImageIndexTable[iTemp].FlagImageExist==0)
        {
            HeadDataPos += 5;
            continue;
        }

        ImageIndexTable[iTemp].ImageEntry = (HeadDataBuff[HeadDataPos+1])|(HeadDataBuff[HeadDataPos+2]<<8)
			|(HeadDataBuff[HeadDataPos+3]<<16)|(HeadDataBuff[HeadDataPos+4]<<24);

        FlagReturn = i51AdeOsFileSetSeek( Fp_i51IMIC, (ImageIndexTable[iTemp].ImageEntry), i51_OS_FL_SEEK_HEAD );
        if( iFALSE == FlagReturn ) goto ERROR;

        FlagReturn = i51AdeOsFileRead( Fp_i51IMIC, ImageDataBuff, I51KITG2_OFFSET_PIXELDATA, iNULL);
        if( iFALSE == FlagReturn ) goto ERROR;

//        ImageIndexTable[iTemp].LengthIamgeData= 
//			(ImageDataBuff[I51KITG2_OFFSET_DATALENGTH]) |(ImageDataBuff[I51KITG2_OFFSET_DATALENGTH+1]<<8)\
//			|(ImageDataBuff[I51KITG2_OFFSET_DATALENGTH+2]<<16)|(ImageDataBuff[I51KITG2_OFFSET_DATALENGTH+3]<<24); 
        ImageIndexTable[iTemp].ImageWidth = (ImageDataBuff[I51KITG2_OFFSET_IMAGEWIDTH]) |(ImageDataBuff[I51KITG2_OFFSET_IMAGEWIDTH+1]<<8); 
        ImageIndexTable[iTemp].ImageHeight= (ImageDataBuff[I51KITG2_OFFSET_IMAGEHEIGHT]) |(ImageDataBuff[I51KITG2_OFFSET_IMAGEHEIGHT+1]<<8); 
        ImageIndexTable[iTemp].DecodeType= (ImageDataBuff[I51KITG2_OFFSET_DECODETYPE]); 
        ImageIndexTable[iTemp].AlphaPixelNum= (ImageDataBuff[I51KITG2_OFFSET_ALPHANUMER])|(ImageDataBuff[I51KITG2_OFFSET_ALPHANUMER+1]<<8);
        ImageIndexTable[iTemp].LengthIamgeData =
        	(ImageDataBuff[I51KITG2_OFFSET_DATALENGTH])|(ImageDataBuff[I51KITG2_OFFSET_DATALENGTH+1]<<8)
           |(ImageDataBuff[I51KITG2_OFFSET_DATALENGTH+2]<<16)|(ImageDataBuff[I51KITG2_OFFSET_DATALENGTH+3]<<24);
        ImageIndexTable[iTemp].LengthTotalData = ImageIndexTable[iTemp].LengthIamgeData ;
        if(ImageIndexTable[iTemp].AlphaPixelNum>0)
        {
            ImageIndexTable[iTemp].LengthTotalData+=ImageIndexTable[iTemp].AlphaPixelNum*8 + 12;
        }
//        DataModle= (ImageDataBuff[I51KITG2_OFFSET_DATAMODEL]); 
//        if( DataModle!=i51AdeCnfGetEndian() ) goto ERROR;
        HeadDataPos += 5;
		
  }

    ImageContainerEntry->ImageIndexTable = ImageIndexTable;

//    CurrentImageContainer.FileHandle  = ImageContainerEntry->FileHandle;
//    CurrentImageContainer.ImageIndexTable = ImageContainerEntry->ImageIndexTable;
//    CurrentImageContainer.ImageMaxId = 0;
//    CurrentImageContainer.ImageTotalNumber = ImageContainerEntry->ImageTotalNumber;

 #if i51KITG2_DEBUG
    iLog( "G2OpenImageContainer end , %x",ContainerPathName);
#endif

    i51AdeOsFree(HeadDataBuff);
    i51AdeOsFree(ImageDataBuff);
    HeadDataBuff = iNULL;
    ImageDataBuff = iNULL;

    iTemp = 0;	 
    while(1)
    {
        if( iFALSE==ContainerManage[iTemp].Flag )
        {
    	     ContainerManage[iTemp].Flag = iTRUE;
            ContainerManage[iTemp].ContainerHandle = ImageContainerEntry;
//iLog("open ContainerManage[%d].ContainerHandle:%d",iTemp,ContainerManage[iTemp].ContainerHandle);
            break;
        }
        iTemp++;
        if( iTemp>=MAX_ContainerManage_NUM )		
        {
            iLog("KIT - G2 : contain full");
            goto ERROR;			
        }
    }

    return ImageContainerEntry;
	
    ERROR:

        i51AdeOsFree(HeadDataBuff);
        i51AdeOsFree(ImageDataBuff);
        i51AdeOsFree(ImageIndexTable);
        i51AdeOsFree(ImageContainerEntry);
        HeadDataBuff = iNULL;
        ImageDataBuff = iNULL;
        ImageIndexTable = iNULL;
        ImageContainerEntry = iNULL;
		
//        i51KitG2CloseImageContainer((iHANDLE)(ZlibImageContainer));
//		ZlibImageContainer = iNULL;

        i51AdeOsFileClose( Fp_i51IMIC );
        Fp_i51IMIC = 0;

        iLog("KIT - G2 : RW open contain");
        return iFALSE;

}

iKIT iBOOL i51KitG2DestroyImageContainer ( iHANDLE ImageContainer, iU16* ContainerPathName )
{

    // author : Otto
    // since : Aug 20 2011
    // (C) 2011 , PKIG Tech. Co., Ltd.

    //  iHANDLE ImageContainer ：容器文件指针

    // notes : 释放容器文件占的资源并关闭容器
    //            删除路径Path中的容器文件,如果文件存在
  
    // i51IMIC完整文件路径加文件名
    iU16 Name_i51IMIC[50] = { 0 } ;
    // 文件操作函数返回值
    iU8   FlagReturn = iFALSE;

 #if i51KITG2_DEBUG
    iLog( "G2DestroyImageContainer start , %x,%x",ImageContainer,ContainerPathName);
#endif
    FlagReturn = i51KitG2CloseImageContainer(ImageContainer);
    if( iFALSE == FlagReturn )  return iFALSE;

    MMIAPICOM_Wstrcat( Name_i51IMIC, ContainerPathName);

    if( i51AdeOsFileIsExist(Name_i51IMIC) )
    {
        FlagReturn = i51AdeOsFileDelete(Name_i51IMIC);
        if( iFALSE == FlagReturn )  return iFALSE;
    }

 #if i51KITG2_DEBUG
    iLog( "G2DestroyImageContainer end , %x,%x",ImageContainer,ContainerPathName);
#endif
    return iTRUE;

}

iKIT iBOOL i51KitG2SetContainerContentVersion( iHANDLE Handle, iU32 Version)
{
    // author : Otto
    // since : 2011.10.10
    // (C) 2011 , PKIG Tech. Co., Ltd.

    //  iHANDLE handle: 容器句柄
    //  iU32 Version: 容器内容版本号
    // notes : 设置容器内容版本号

    iU8  Flag = iFALSE;
    iFILE File = ((i51ImageContainer * )Handle)->FileHandle;
 #if i51KITG2_DEBUG
    iLog( "G2SetContainerContentVersion start , %x,%x",Handle,Version);
#endif

    Flag = i51AdeOsFileSetSeek( File, I51KITG2_OFFSET_FILE_VERSION, i51_OS_FL_SEEK_HEAD );
    if( iNULL==Flag ) goto ERROR;

    Flag = i51AdeOsFileWriteInt32( File, &Version);
    if( iNULL==Flag ) goto ERROR;

    Flag = i51AdeOsFileFlush(File);
    if( iNULL==Flag ) goto ERROR;

    Flag = i51KitG2WriteFileCRC(File);
    if( iNULL==Flag ) goto ERROR;

 #if i51KITG2_DEBUG
    iLog( "G2SetContainerContentVersion end , %x,%x",Handle,Version);
#endif

    return iTRUE;

ERROR:
    iLog("KIT - G2 : RW set version");
    return iFALSE;
	
}
	
iKIT iBOOL i51KitG2GetContainerContentVersion( iHANDLE Handle, iU32 *Version)
{
    // author : Otto
    // since : 2011.10.10
    // (C) 2011 , PKIG Tech. Co., Ltd.

    //  iHANDLE handle: 容器句柄
    //  iU32 Version: 容器内容版本号
    // notes : 获得容器内容版本号

    iU8  Flag = iFALSE;
    iFILE File = ((i51ImageContainer * )Handle)->FileHandle;
	
 #if i51KITG2_DEBUG
    iLog( "G2GetContainerContentVersion start , %x,%x",Handle,Version);
#endif

    if( iNULL==Handle || iNULL==File ) return iFALSE;
	
    Flag = i51AdeOsFileSetSeek( File, I51KITG2_OFFSET_FILE_VERSION, i51_OS_FL_SEEK_HEAD );
    if( iNULL==Flag ) goto ERROR;
	
    Flag = i51AdeOsFileReadInt32( File, Version);
    if( iNULL==Flag ) goto ERROR;

 #if i51KITG2_DEBUG
    iLog( "G2GetContainerContentVersion end , %x,%x",Handle,Version);
#endif

    return iTRUE;

ERROR:
    iLog("KIT - G2 : RW get version");
    return iFALSE;
}
	
iKIT iU32 i51KitG2SetImageMemoryCopy( iHANDLE Handle, iU32 TotalImage, iU32 ImageId[])
{
    // author : Otto
    // since : Sep 27 2011
    // (C) 2011 , PKIG Tech. Co., Ltd.

    //  iHANDLE handle: 容器句柄
    //  iU32 TotalImage: 需要设置的图片总数
    //  iU32 ImageId[] : 图片ID

    // return iU32: 拷贝成功的图片数量    
    // notes : 设置容器内图片拷贝到内存中，如果ImageId为空则全部拷贝

    // 容器内图片总数
    iU32 AllImageNum = 0;
    // 设置成功的图片数量	
    iU32 SetOkNum = 0;
    // 需要设置的图片数量	
    iU32 NeedSetNum = 0;
    // 图片索引表地址
    i51ImageIndex * ImageIndexTable = iNULL;
    // 图片容器地址入口
    i51ImageContainer * ImageContainerEntry = iNULL;
    // 图片容器文件
    iFILE ImageContainerFile = iNULL;
    // 文件操作返回值监控	
    iBOOL FlagReturn =iFALSE;
    // 当前拷贝的图片ID
    iU32 CurrCopyID = 0 ;	
    // 当前图片文件的数据长度
    iU32 CurrentDataLength = 0;
    // 从文件中读取的数据实际长度
    iU32 LengthReadData = 0;
    iS32 Temp1 = 0;

 #if i51KITG2_DEBUG
    iLog( "G2SetImageMemoryCopy start , %x,%x, %x",Handle,TotalImage, ImageId);
#endif
	
    if( iNULL == Handle ) return iFALSE;

//iLog("Handle:0X%X",Handle);

    ImageContainerEntry = (i51ImageContainer * )Handle;

//iLog("S1");
    ImageIndexTable = ImageContainerEntry->ImageIndexTable;

//iLog("S2");
    AllImageNum = ImageContainerEntry->ImageMaxId;

//iLog("AllImageNum:%d",AllImageNum);
    ImageContainerFile = ImageContainerEntry->FileHandle;

    if( AllImageNum<TotalImage ) NeedSetNum = AllImageNum;
    else NeedSetNum = TotalImage;

    if( iNULL==ImageId )  NeedSetNum = AllImageNum;

//iLog("NeedSetNum:%d",NeedSetNum);
    if( iFALSE == RESUNE_MEMORY_COPY )
    {
        SetMemoryCopyCount ++;
    }
//iLog("SetMemoryCopyCount:%d",SetMemoryCopyCount);

    for( Temp1 = 0; Temp1 < NeedSetNum; Temp1++)	
    {
//iLog("Temp1:%d",Temp1);
        if( ImageId)
        {
            CurrCopyID = ImageId[Temp1];
        }
        else
        {
            CurrCopyID = Temp1;
        }

//iLog("T1");
        if( ImageIndexTable[CurrCopyID].FlagImageExist==0) continue;
		
        if( CurrCopyID>AllImageNum ) continue;
		
//iLog("T2");
        if( ImageIndexTable[CurrCopyID].FlagCopyRAM ) continue;

        // 文件格式前四位是文件类型标示符，第五位版本号，六位开始的连续四位是图片数据总长度
//        FlagReturn = i51AdeOsFileSetSeek( ImageContainerFile, (ImageIndexTable[CurrCopyID].ImageEntry+5), i51_OS_FL_SEEK_HEAD );
//        if( iFALSE == FlagReturn )  goto ERROR;

           CurrentDataLength = ImageIndexTable[CurrCopyID].LengthTotalData;
//        FlagReturn = i51AdeOsFileReadInt32( ImageContainerFile, &CurrentDataLength);
//        if( iFALSE == FlagReturn )  goto ERROR;

        // 文件格式前四位是文件类型标示符，第五位版本号，六位开始的连续四位是图片数据总长度
        FlagReturn = i51AdeOsFileSetSeek( ImageContainerFile, (ImageIndexTable[CurrCopyID].ImageEntry), i51_OS_FL_SEEK_HEAD );

//iLog("T3");
        ImageIndexTable[CurrCopyID].MemoryEntry = ( iU32)(i51AdeOsMalloc( CurrentDataLength,  __FILE__ , __LINE__ ));
        if( iFALSE == ImageIndexTable[CurrCopyID].MemoryEntry ) 
        {
            iLog("KIT - G2 : malloc set memory");
            return SetOkNum;
        }

        FlagReturn = i51AdeOsFileRead( ImageContainerFile, (void *)(ImageIndexTable[CurrCopyID].MemoryEntry), CurrentDataLength,&LengthReadData);
        if( iFALSE == FlagReturn || ( LengthReadData!= (CurrentDataLength) ) ) goto ERROR;

        ImageIndexTable[CurrCopyID].FlagCopyRAM = iTRUE;
        if( RESUNE_MEMORY_COPY )
        {
            ImageIndexTable[CurrCopyID].SetMemoryCopyCount = ContainerManage[CurrContainerManageNum].MemCopyCount[Temp1];
        }
        else
        {
            ImageIndexTable[CurrCopyID].SetMemoryCopyCount = SetMemoryCopyCount;
        }
        SetOkNum ++;
    }

 #if i51KITG2_DEBUG
    iLog( "G2SetImageMemoryCopy end , %x,%x, %x",Handle,TotalImage, ImageId);
#endif

    RESUNE_MEMORY_COPY = 0;

    return SetOkNum;

ERROR:
    i51AdeOsFree( (void *)ImageIndexTable[CurrCopyID].MemoryEntry);
    ImageIndexTable[CurrCopyID].MemoryEntry = iNULL;
    iLog("KIT - G2 : RW set memory");
    return SetOkNum;
	
}

iKIT iU32 i51KitG2UnsetImageMemoryCopy( iHANDLE Handle, iU32 TotalImage, iU32 ImageId[])
{
    // author : Otto
    // since : Sep 27 2011
    // (C) 2011 , PKIG Tech. Co., Ltd.

    //  iHANDLE handle: 容器句柄
    //  iU32 TotalImage: 需要设置的图片总数
    //  iU32 ImageId[] : 图片ID

    // return iU32: 取消拷贝成功的图片数量    
    // notes : 取消容器内图片拷贝到内存中，如果ImageId为空则全部取消
    
    // 容器内图片总数
    iU32 AllImageNum = 0;
    // 设置成功的图片数量	
    iU32 SetOkNum = 0;
    // 需要设置的图片数量	
    iU32 NeedSetNum = 0;
    // 图片索引表地址
    i51ImageIndex * ImageIndexTable = iNULL;
    // 图片容器地址入口
    i51ImageContainer * ImageContainerEntry = iNULL;
    // 操作返回值监控	
    iBOOL FlagReturn =iFALSE;
    // 当前拷贝的图片ID
    iU32 CurrCopyID = 0 ;	
    iS32 Temp1 = 0;

 #if i51KITG2_DEBUG
    iLog( "G2UnsetImageMemoryCopy start , %x,%x, %x",Handle,TotalImage, ImageId);
#endif
	
    if( iNULL == Handle ) return iFALSE;
	
    ImageContainerEntry = (i51ImageContainer * )Handle;

    ImageIndexTable = ImageContainerEntry->ImageIndexTable;

    AllImageNum = ImageContainerEntry->ImageMaxId;

    if( AllImageNum<TotalImage ) NeedSetNum = AllImageNum;
    else NeedSetNum = TotalImage;

    if( iNULL==ImageId )  NeedSetNum = AllImageNum;

    for( Temp1 = 0; Temp1 < NeedSetNum; Temp1++)	
    {
        if( ImageId)
        {
            CurrCopyID = ImageId[Temp1];
        }
        else
        {
            CurrCopyID = Temp1;
        }
		
        if( CurrCopyID>AllImageNum ) continue;
		
        if( ImageIndexTable[CurrCopyID].FlagImageExist==0) continue;
		
        if( ImageIndexTable[CurrCopyID].FlagCopyRAM == iFALSE ) continue;
		
        FlagReturn = i51AdeOsFree( (void *)(ImageIndexTable[CurrCopyID].MemoryEntry));
        ImageIndexTable[CurrCopyID].MemoryEntry = iNULL;
        if( iFALSE == FlagReturn ) return iFALSE;

        ImageIndexTable[CurrCopyID].FlagCopyRAM = iFALSE;
        ImageIndexTable[CurrCopyID].MemoryEntry = iNULL;
        SetOkNum ++;
    }
	
 #if i51KITG2_DEBUG
    iLog( "G2UnsetImageMemoryCopy end , %x,%x, %x",Handle,TotalImage, ImageId);
#endif
    return SetOkNum;
	
}

iKIT iBOOL i51KitG2StopDecode( iHANDLE Handle)
{
	// 停止解码
	if(TimeIdZlibDecodeImage)
	{
		if( iNULL==ZlibImageContainer || ZlibImageContainer->FileHandle != (iFILE)(Handle) )
		{
			iLog("KIT - G2 : stop decode handle:%d",(iU32)Handle);
			return iFALSE;
		}
		
		if( iFALSE== i51AdeOsTimerStop( TimeIdZlibDecodeImage ))
		{
			iLog("KIT - G2 : stop decode time:%d",TimeIdZlibDecodeImage);
			return iFALSE;
		}
		TimeIdZlibDecodeImage = iNULL;
	}

	if(FlagDecodeImage && ZlibImageContainer )
	{
		i51KitG2CloseImageContainer(ZlibImageContainer);
	}

	return iTRUE;
}

iKIT iBOOL i51KitG2GetImageAttr ( iHANDLE ContainerHandle,iU32 ImageID , iU32 * ImageWidth, iU32 * ImageHeight, iU8 *MemoryCopy)
{
    //  iHANDLE handle: 容器句柄
    //  iU32 TotalImage: 需要设置的图片总数
    //  iU32 ImageId : 图片ID
    //  iU32 * ImageWidth: 获得的图片宽度
    //  iU32 * ImageHeight:获得的图片高度
    
    // 图片容器地址入口
    i51ImageContainer * ImageContainer = iNULL;
    // 图片索引表地址
    i51ImageIndex * ImageIndexTable = iNULL;

 #if i51KITG2_DEBUG
    iLog( "G2GetImageAttr start , %x,%x, %x,%x",ContainerHandle,ImageID, ImageWidth,ImageHeight);
#endif

    if( iNULL==ContainerHandle ) return iFALSE;
    ImageContainer =  (i51ImageContainer * )ContainerHandle;
    ImageIndexTable = ( i51ImageIndex *  )(ImageContainer->ImageIndexTable);
    if( iNULL == ImageIndexTable )  return iFALSE;
    if(ImageIndexTable[ImageID].FlagImageExist==0) return iFALSE;    

    if(ImageWidth) *ImageWidth = ImageIndexTable[ImageID].ImageWidth;
    if(ImageHeight) *ImageHeight = ImageIndexTable[ImageID].ImageHeight;
    if(MemoryCopy) *MemoryCopy = ImageIndexTable[ImageID].FlagCopyRAM;
	
 #if i51KITG2_DEBUG
    iLog( "G2GetImageAttr end , %x,%x, %x,%x",ContainerHandle,ImageID, ImageWidth,ImageHeight);
#endif
    return iTRUE;
  
}

iKIT iBOOL i51KitG2DrawImage ( iHANDLE ContainerHandle,iU32 ImageID , iRECT* DesRect , iRECT* SrcRect )
{
    // author : Otto
    // since : Aug 29 2011
    // (C) 2011 , PKIG Tech. Co., Ltd.

    //  iHANDLE ImageContainer ：容器文件指针
    //  iU16 ImageID  ：容器内图片文件ID
    //  iRECT* DesRect :   绘制目的地区域
    //  iRECT* SrcRect :    绘制图片原始区域
    
    // notes : 绘制ImageContainer容器内编号为ImageID的图像
    // SrcRect 为空时在点(DesRect->X, DesRect->Y)处开始绘制整个图片
    // DesRect 为空则绘制从屏幕左上角开始

    // 图片容器地址入口
    i51ImageContainer * ImageContainer = iNULL;
    // 图片索引表地址
    i51ImageIndex * ImageIndexTable = iNULL;
    // 容器文件指针地址
    iFILE File = iNULL;
     // 图片编码方式
    iU8   DecodeType = 0;
    iU16 CurrDrawID = 0;
    iU8 Ret = iFALSE;
    iS32 ImageWidth = 0;
    iS32 ImageHeight = 0;	

 #if i51KITG2_DEBUG
    iLog( "G2DrawImage start , %x,%x, %x,%x",ContainerHandle,ImageID, DesRect,SrcRect);
#endif
	
    if( iNULL== ContainerHandle) return iFALSE;
    ImageContainer = (i51ImageContainer *)ContainerHandle;
    ImageIndexTable = ( i51ImageIndex *  )(ImageContainer->ImageIndexTable);
    File = ImageContainer->FileHandle;
    if( iNULL == ImageIndexTable || iNULL==File)  return iFALSE;

    if(ImageIndexTable[ImageID].FlagImageExist==0 ) return iFALSE;
	
    DecodeType = ImageIndexTable[ImageID].DecodeType;
    ImageWidth = ImageIndexTable[ImageID].ImageWidth;
    ImageHeight = ImageIndexTable[ImageID].ImageHeight;

    if(DecodeType!=I51KITG2_DECODE_LINEAR)
    {
    	iLog("文件格式错误，请重新生成图片文件!");
    	return iFALSE;
    }

    // 在虚拟面板上绘制，实际上只是进行一些赋值操作
    if( i51UseVirtualPanel && i51DrawVirtualPanel )
    {
        if( i51CurrentVpManage.TotalVPNumber == 0 ) return iFALSE;

        if( iNULL == i51CurrentVpActivity )	return iFALSE;
		
        // 画板已经画满则直接返回
        if(  i51CurrentVpActivity->VpIsFull ) return iFALSE;
      
        CurrDrawID = i51CurrentVpActivity->VPCurrentElement;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementType = I51KITG2_ELEMENT_TYPE_PICTURE;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementNumber = CurrDrawID;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElemenAlpha = AlphaSys;
        if( SrcRect )
        {
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.SrcRect.X = SrcRect->X;
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.SrcRect.Y = SrcRect->Y;
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.SrcRect.Width = SrcRect->Width;
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.SrcRect.Height = SrcRect->Height;
        }
        else
        {
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.SrcRect.X = 0;
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.SrcRect.Y = 0;
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.SrcRect.Width = ImageWidth;
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.SrcRect.Height = ImageHeight;
        }
		
        if( DesRect )		
        {
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.DesRect.X = DesRect->X;
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.DesRect.Y = DesRect->Y;
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.DesRect.Width = DesRect->Width;
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.DesRect.Height = DesRect->Height;
        }
        else
        {
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.DesRect.X =0;
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.DesRect.Y =0;
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.DesRect.Width = i51CurrentVpActivity->VPActualRect.Width;
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.DesRect.Height = i51CurrentVpActivity->VPActualRect.Height;
        }

        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.DrawDirection = I51KITG2_DRAW_IAMGE_NORMAL;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.ImageHandle = ImageID;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.ContainerHandle = ContainerHandle;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.DarwAngle = 0;
        i51CurrentVpActivity->VPCurrentElement =  CurrDrawID +1;
        if( (CurrDrawID+1) >= i51CurrentVpActivity->VPMaxElement )	i51CurrentVpActivity->VpIsFull = iTRUE;
        return iTRUE;
    }
 
    if( ImageIndexTable[ImageID].FlagCopyRAM )
    {
        // 线性压缩编码方式
        // 把图像像素信息按行优先进行线性编码，数据格式如下：
        // x,y,w,RGB数据，其中x，y是非透明色的起点坐标，w是连续有多少个非透明色，x，y分别占用1.5字节，w占两个字节。
        Ret = DrawRAMLinearImage( ContainerHandle, ImageID,  DesRect, SrcRect, I51KITG2_DRAW_IAMGE_NORMAL);
    }
    else
    {  
        // 显示线性压缩编码图片
        // 把图像像素信息按行优先进行线性编码，数据格式如下：
        // x,y,w,RGB数据，其中x，y是非透明色的起点坐标，w是连续有多少个非透明色，x，y分别占用1.5字节，w占两个字节。
        Ret = DrawRAMLinearImage( ContainerHandle, ImageID, DesRect, SrcRect, I51KITG2_DRAW_IAMGE_NORMAL);
    }

 #if i51KITG2_DEBUG
    iLog( "G2DrawImage end , %x,%x, %x,%x",ContainerHandle,ImageID, DesRect,SrcRect);
#endif
	
    return Ret;
}


iKIT iBOOL  i51KitG2MirrorImage  (  iHANDLE ContainerHandle, iU32 ImageID , iRECT* DesRect , iRECT* SrcRect )
{
    // author : Otto
    // since : Aug 29 2011
    // (C) 2011 , PKIG Tech. Co., Ltd.

    //  iHANDLE ImageContainer ：容器文件指针
    //  iU16 ImageID  ：容器内图片文件ID
    //  iRECT* DesRect :   绘制目的地区域
    //  iRECT* SrcRect :    绘制图片原始区域

    // notes : 按镜像显示方式绘制ImageContainer容器内编号为ImageID的图像
    // SrcRect 为空时在点(DesRect->X, DesRect->Y)处开始绘制整个图片
    // DesRect 为空则绘制从屏幕左上角开始

    // 图片容器地址入口
    i51ImageContainer * ImageContainer = iNULL;
    // 图片索引表地址
    i51ImageIndex * ImageIndexTable = iNULL;
    // 容器文件指针地址
    iFILE File = iNULL;
     // 图片编码方式
    iU8   DecodeType = 0;
    iU16 CurrDrawID = 0;
    iU8 Ret = iFALSE;

    iS32 ImageWidth = 0;
    iS32 ImageHeight = 0;	

 #if i51KITG2_DEBUG
    iLog( "G2MirrorImage start , %x,%x, %x,%x",ContainerHandle,ImageID, DesRect,SrcRect);
#endif
	

    if( iNULL== ContainerHandle) return iFALSE;
    ImageContainer = (i51ImageContainer * )ContainerHandle;
    ImageIndexTable = ( i51ImageIndex *  )(ImageContainer->ImageIndexTable);
    File = ImageContainer->FileHandle;
    if( iNULL == ImageIndexTable || iNULL==File)  return iFALSE;
	
    if(ImageIndexTable[ImageID].FlagImageExist==0 ) return iFALSE;

    DecodeType = ImageIndexTable[ImageID].DecodeType;
    ImageWidth = ImageIndexTable[ImageID].ImageWidth;
    ImageHeight = ImageIndexTable[ImageID].ImageHeight;

    if(DecodeType!=I51KITG2_DECODE_LINEAR)
    {
    	iLog("文件格式错误，请重新生成图片文件!");
    	return iFALSE;
    }

  
    // 在虚拟面板上绘制，实际上只是进行一些赋值操作
    if( i51UseVirtualPanel && i51DrawVirtualPanel )
    {
        if( i51CurrentVpManage.TotalVPNumber == 0 ) return iFALSE;

        if( iNULL == i51CurrentVpActivity )	return iFALSE;
		
        // 画板已经画满则直接返回
        if(  i51CurrentVpActivity->VpIsFull ) return iFALSE;
      
        CurrDrawID = i51CurrentVpActivity->VPCurrentElement;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementType = I51KITG2_ELEMENT_TYPE_PICTURE;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementNumber = CurrDrawID;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElemenAlpha = AlphaSys;
        if( SrcRect )
        {
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.SrcRect.X = SrcRect->X;
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.SrcRect.Y = SrcRect->Y;
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.SrcRect.Width = SrcRect->Width;
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.SrcRect.Height = SrcRect->Height;
        }
        else
        {
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.SrcRect.X = 0;
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.SrcRect.Y = 0;
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.SrcRect.Width = ImageWidth;
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.SrcRect.Height = ImageHeight;
        }
		
        if( DesRect )		
        {
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.DesRect.X = DesRect->X;
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.DesRect.Y = DesRect->Y;
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.DesRect.Width = DesRect->Width;
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.DesRect.Height = DesRect->Height;
        }
        else
        {
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.DesRect.X =0;
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.DesRect.Y =0;
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.DesRect.Width = i51CurrentVpActivity->VPActualRect.Width;
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.DesRect.Height = i51CurrentVpActivity->VPActualRect.Height;
        }

        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.DrawDirection = I51KITG2_DRAW_IAMGE_MIRROR;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.ImageHandle = ImageID;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.ContainerHandle = ContainerHandle;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.DarwAngle = 0;
        i51CurrentVpActivity->VPCurrentElement =  CurrDrawID +1;
        if( (CurrDrawID+1) >= i51CurrentVpActivity->VPMaxElement )	i51CurrentVpActivity->VpIsFull = iTRUE;
        return iTRUE;
    }
 
    if( ImageIndexTable[ImageID].FlagCopyRAM )
    {
        // 线性压缩编码方式
        // 把图像像素信息按行优先进行线性编码，数据格式如下：
        // x,y,w,RGB数据，其中x，y是非透明色的起点坐标，w是连续有多少个非透明色，x，y分别占用1.5字节，w占两个字节。
        Ret = DrawRAMLinearImage( ContainerHandle, ImageID,  DesRect, SrcRect, I51KITG2_DRAW_IAMGE_MIRROR);
    }
    else
    {  
        // 显示线性压缩编码图片
        // 把图像像素信息按行优先进行线性编码，数据格式如下：
        // x,y,w,RGB数据，其中x，y是非透明色的起点坐标，w是连续有多少个非透明色，x，y分别占用1.5字节，w占两个字节。
        Ret = DrawRAMLinearImage( ContainerHandle, ImageID, DesRect,  SrcRect, I51KITG2_DRAW_IAMGE_MIRROR);
    }

 #if i51KITG2_DEBUG
    iLog( "G2MirrorImage end , %x,%x, %x,%x",ContainerHandle,ImageID, DesRect,SrcRect);
#endif
	
    return Ret;
	
}

iKIT iBOOL   i51KitG2FlipImage   (  iHANDLE ContainerHandle, iU32 ImageID , iRECT* DesRect , iRECT* SrcRect )
{
    // author : Otto
    // since : Aug 29 2011
    // (C) 2011 , PKIG Tech. Co., Ltd.

    //  iHANDLE ImageContainer ：容器文件指针
    //  iU16 ImageID  ：容器内图片文件ID
    //  iRECT* DesRect :   绘制目的地区域
    //  iRECT* SrcRect :    绘制图片原始区域

    // notes : 按自下而上的显示方式绘制ImageContainer容器内编号为ImageID的图像
    // SrcRect 为空时在点(DesRect->X, DesRect->Y)处开始绘制整个图片
    // DesRect 为空则绘制从屏幕左上角开始

    // 图片容器地址入口
    i51ImageContainer * ImageContainer = iNULL;
    // 图片索引表地址
    i51ImageIndex * ImageIndexTable = iNULL;
    // 容器文件指针地址
    iFILE File = iNULL;
     // 图片编码方式
    iU8   DecodeType = 0;
    iU16 CurrDrawID = 0;
    iU8 Ret = 0;

    iS32 ImageWidth = 0;
    iS32 ImageHeight = 0;	

 #if i51KITG2_DEBUG
    iLog( "G2FlipImage start , %x,%x, %x,%x",ContainerHandle,ImageID, DesRect,SrcRect);
#endif
	
    if( iNULL== ContainerHandle) return iFALSE;
    ImageContainer = (i51ImageContainer *)ContainerHandle;
    ImageIndexTable = ( i51ImageIndex *  )(ImageContainer->ImageIndexTable);
    File = ImageContainer->FileHandle;
    if( iNULL == ImageIndexTable || iNULL==File)  return iFALSE;
	
    if(ImageIndexTable[ImageID].FlagImageExist==0 ) return iFALSE;

    DecodeType = ImageIndexTable[ImageID].DecodeType;
    ImageWidth = ImageIndexTable[ImageID].ImageWidth;
    ImageHeight = ImageIndexTable[ImageID].ImageHeight;
  
    if(DecodeType!=I51KITG2_DECODE_LINEAR)
    {
    	iLog("文件格式错误，请重新生成图片文件!");
    	return iFALSE;
    }

    // 在虚拟面板上绘制，实际上只是进行一些赋值操作
    if( i51UseVirtualPanel && i51DrawVirtualPanel )
    {
        if( i51CurrentVpManage.TotalVPNumber == 0 ) return iFALSE;

        if( iNULL == i51CurrentVpActivity )	return iFALSE;
		
        // 画板已经画满则直接返回
        if(  i51CurrentVpActivity->VpIsFull ) return iFALSE;
      
        CurrDrawID = i51CurrentVpActivity->VPCurrentElement;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementType = I51KITG2_ELEMENT_TYPE_PICTURE;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementNumber = CurrDrawID;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElemenAlpha = AlphaSys;
        if( SrcRect )
        {
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.SrcRect.X = SrcRect->X;
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.SrcRect.Y = SrcRect->Y;
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.SrcRect.Width = SrcRect->Width;
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.SrcRect.Height = SrcRect->Height;
        }
        else
        {
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.SrcRect.X = 0;
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.SrcRect.Y = 0;
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.SrcRect.Width = ImageWidth;
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.SrcRect.Height = ImageHeight;
        }
		
        if( DesRect )		
        {
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.DesRect.X = DesRect->X;
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.DesRect.Y = DesRect->Y;
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.DesRect.Width = DesRect->Width;
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.DesRect.Height = DesRect->Height;
        }
        else
        {
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.DesRect.X =0;
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.DesRect.Y =0;
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.DesRect.Width = i51CurrentVpActivity->VPActualRect.Width;
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.DesRect.Height = i51CurrentVpActivity->VPActualRect.Height;

        }

        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.DrawDirection = I51KITG2_DRAW_IAMGE_HANDSTAND;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.ImageHandle = ImageID;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.ContainerHandle = ContainerHandle;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.DarwAngle = 0;
        i51CurrentVpActivity->VPCurrentElement =  CurrDrawID +1;
        if( (CurrDrawID+1) >= i51CurrentVpActivity->VPMaxElement )	i51CurrentVpActivity->VpIsFull = iTRUE;
        return iTRUE;
    }
 
    if( ImageIndexTable[ImageID].FlagCopyRAM )
    {
        // 线性压缩编码方式
        // 把图像像素信息按行优先进行线性编码，数据格式如下：
        // x,y,w,RGB数据，其中x，y是非透明色的起点坐标，w是连续有多少个非透明色，x，y分别占用1.5字节，w占两个字节。
        Ret = DrawRAMLinearImage( ContainerHandle, ImageID,  DesRect, SrcRect, I51KITG2_DRAW_IAMGE_HANDSTAND);
    }
    else
    {  
        // 显示线性压缩编码图片
        // 把图像像素信息按行优先进行线性编码，数据格式如下：
        // x,y,w,RGB数据，其中x，y是非透明色的起点坐标，w是连续有多少个非透明色，x，y分别占用1.5字节，w占两个字节。
        Ret = DrawRAMLinearImage( ContainerHandle, ImageID, DesRect,  SrcRect, I51KITG2_DRAW_IAMGE_HANDSTAND);
    }

 #if i51KITG2_DEBUG
    iLog( "G2FlipImage end , %x,%x, %x,%x",ContainerHandle,ImageID, DesRect,SrcRect);
#endif
	
    return Ret;
	
}

#if ROTATE_SPECIAE

static iBOOL   i51G2DrawBasePixByAngle( iU16 *SrcBuf, iU32 SrcBufLen, iS32 SrcMoveX , iS32 SrcMoveY,	
iS32 CenterX , iS32 CenterY ,iRECT* inDesRect, iRECT* inSrcRect, iS32 Angle)
{
	iS32 i = 0, j = 0;
	iU16 *SrcTmp = iNULL, *DrawTmp = iNULL;
	iU16 *SrcBufPtr = iNULL;
	iS16 CurrentImageX = 0,  CurrentImageY = 0, CurrentImageWidth = 0;
	iS16 StartX=0, EndX=0, Width = 0;
	iS16 SrcStartX=0, SrcStartY=0, SrcEndX=0, SrcEndY=0;
	iS16 StartY = 0;
	iU32 DataSize = 0;
	iS32 TempX = 0;
	iS32 TempY = 0;
	iS32 OffSetX = 0;
	iS32 OffSetY = 0;
	iS32 DesStartX = 0, DesStartY = 0, DesEndX = 0, DesEndY = 0;
	iU8 Flag = iTRUE;
	
	DesStartX = inDesRect->X;
	DesStartY = inDesRect->Y;
	DesEndX = DesStartX + inDesRect->Width;
	DesEndY = DesStartY + inDesRect->Height;

 	switch( Angle )
	{
		case 0:
			// 点A(x,y)旋转0°后对应坐标点:
			// B(x+ SrcMoveX -inSrcRect->X, y+ SrcMoveY  -inSrcRect->Y )
			// 简化为:B(y+OffSetX,x+OffSetY);
			OffSetX = SrcMoveX - inSrcRect->X ;
			OffSetY = SrcMoveY - inSrcRect->Y ;
			break;
		case 90:
			// 点A(x,y)旋转90°后对应坐标点:
			// B(inSrcRect->Y - SrcMoveY + CenterY + CenterX-y, SrcMoveX - inSrcRect->X + CenterY - CenterX+x)
			// 简化为:B(y+OffSetX,x+OffSetY);
			OffSetY = SrcMoveX - inSrcRect->X + CenterY - CenterX;
			OffSetX = inSrcRect->Y - SrcMoveY + CenterY + CenterX;
			break;
		case 180:	
			// 点A(x,y)旋转180°后对应坐标点:
			// B(2*CenterX+inSrcRect->X-SrcMoveX-x,)
			// 简化为:B(OffSetX-x,OffSetY-y,2*CenterY+inSrcRect->Y-SrcMoveY);
			OffSetY = 2*CenterY+inSrcRect->Y-SrcMoveY;
			OffSetX = 2*CenterX+inSrcRect->X-SrcMoveX;
			break;
		case 270:	
			// 点A(x,y)旋转270°后对应坐标点:
			// B(CenterX - CenterY  + SrcMoveY - inSrcRect->Y +y, CenterY + CenterX - SrcMoveX + inSrcRect->X - x )
			// 简化为:B(OffSetX+x,OffSetY+y);
			OffSetY = CenterY + CenterX - SrcMoveX + inSrcRect->X;
			OffSetX = CenterX - CenterY  + SrcMoveY - inSrcRect->Y ;
			break;
		default:
			iLog(" KIT - G2 : angle %d", Angle);
			return iFALSE;
	}

 	//定位到图片数据的第一行
	SrcBufPtr = SrcBuf;   
	CurrentImageX = SrcBufPtr[0];
	CurrentImageY = SrcBufPtr[1];
	CurrentImageWidth = SrcBufPtr[2];
	while (CurrentImageY < inSrcRect->Y)
	{
		DataSize += (6 + CurrentImageWidth*2);

		SrcBufPtr += (3 + CurrentImageWidth);
		CurrentImageX = SrcBufPtr[0];
		CurrentImageY = SrcBufPtr[1];
		CurrentImageWidth = SrcBufPtr[2];
		if (DataSize >= SrcBufLen)
		{
			return iFALSE;
		}
	}

	SrcStartX = inSrcRect->X;
	SrcEndX   =  SrcStartX + inSrcRect->Width;
	SrcStartY = inSrcRect->Y;
	SrcEndY   = SrcStartY + inSrcRect->Height;

 //	if (0xFF == AlphaSys)
	{
		Flag = iTRUE;
		while (Flag)
		{
			CurrentImageX = SrcBufPtr[0];
			CurrentImageY = SrcBufPtr[1];
			CurrentImageWidth = SrcBufPtr[2];

			DataSize += (6 + CurrentImageWidth*2);
			if (DataSize > SrcBufLen || CurrentImageY >= SrcEndY)
			{
				break;
			}

			if ( CurrentImageX >= SrcEndX)
			{
				SrcBufPtr += (3 + CurrentImageWidth); 
				continue;
			}

			if( CurrentImageX<SrcStartX )
			{
			
				SrcBufPtr += 3+(SrcStartX-CurrentImageX);
				CurrentImageWidth -= (SrcStartX-CurrentImageX);
				CurrentImageX = SrcStartX;
			}
			else
			{
				SrcBufPtr += 3; 
			}


			switch (Angle)
			{
				case 0:
					StartX = OffSetX + CurrentImageX;
					StartY = OffSetY + CurrentImageY;
					if( StartY>=DesStartY && StartY< DesEndY )
					{
						Width = CurrentImageWidth;
						SrcTmp = SrcBufPtr;
						if( StartX<DesStartX )
						{
							Width -= DesStartX - StartX;
							SrcTmp += DesStartX - StartX;
							StartX = DesStartX;
						}
						if( StartX+Width>DesEndX )
						{
							Width = DesEndX - StartX;
						}
					}
					else if( StartY>=DesEndY )
					{
						Flag = iFALSE;
						break;
					}
					else
					{
						break;
					}
					
					DrawTmp = LcdBuffer + ScreenWidth*StartY + StartX;
					for (j=0; j<Width; j++)
					{
						*DrawTmp = i51KitG2GetNewColorEx(*DrawTmp,*SrcTmp,AlphaSys);
						SrcTmp++;
						DrawTmp ++;
					}			
					break;
				case 90:
					StartX = OffSetX - CurrentImageY;
					StartY = OffSetY + CurrentImageX;
					if( StartX>=DesStartX && StartX< DesEndX && StartY <DesEndY )
					{
						Width = CurrentImageWidth;
						SrcTmp = SrcBufPtr;
						if( StartY<DesStartY )
						{
							Width -= DesStartY - StartY;
							SrcTmp += DesStartY - StartY;
							StartY = DesStartY;
						}
						if( StartY+Width>DesEndY )
						{
							Width = DesEndY - StartY;
						}
					}
					else if( StartX<DesStartX )
					{
						Flag = iFALSE;
						break;
					}
					else
					{
						break;
					}
					
					DrawTmp = LcdBuffer + ScreenWidth*StartY + StartX;
					for (j=0; j<Width; j++)
					{
						*DrawTmp = i51KitG2GetNewColorEx(*DrawTmp,*SrcTmp,AlphaSys);
						SrcTmp++;
						DrawTmp += ScreenWidth;
					}			
					break;
				case 180:
					StartX = OffSetX - CurrentImageX;
					StartY = OffSetY - CurrentImageY;
					if( StartY>=DesStartY && StartY< DesEndY && StartX>= DesStartX )
					{
						Width = CurrentImageWidth;
						SrcTmp = SrcBufPtr;
						if( StartX>=DesEndX )
						{
							Width -= StartX - DesEndX +1;
							SrcTmp += StartX - DesEndX +1;
							StartX = DesEndX-1;
						}
						if( StartX-Width<DesStartX )
						{
							Width = StartX-DesStartX+1;
						}
					}
					else if( StartY<DesStartY )
					{
						Flag = iFALSE;
						break;
					}
					else 
					{
 						break;
					}
					
					DrawTmp = LcdBuffer + ScreenWidth*StartY + StartX;
					for (j=0; j<Width; j++)
					{
//						*DrawTmp = *SrcTmp;
						*DrawTmp = i51KitG2GetNewColorEx(*DrawTmp,*SrcTmp,AlphaSys);
						SrcTmp++;
						DrawTmp --;
					}			
					break;
				case 270:
					StartX = OffSetX + CurrentImageY;
					StartY = OffSetY - CurrentImageX;
					if( StartX>=DesStartX && StartX< DesEndX && StartY >=DesStartY )
					{
						Width = CurrentImageWidth;
						SrcTmp = SrcBufPtr;
						if( StartY>=DesEndY )
						{
							Width -= StartY- DesEndY -1;
							SrcTmp += StartY- DesEndY -1;
							StartY = DesEndY -1 ;
						}
						if( StartY-Width<DesStartY )
						{
							Width = StartY - DesStartY +1;
						}
					}
					else if( StartX>=DesEndX )
					{
						Flag = iFALSE;
						break;
					}
					else
					{
						break;
					}
					
					DrawTmp = LcdBuffer + ScreenWidth*StartY + StartX;
					for (j=0; j<Width; j++)
					{
//						*DrawTmp = *SrcTmp;
						*DrawTmp = i51KitG2GetNewColorEx(*DrawTmp,*SrcTmp,AlphaSys);
						SrcTmp++;
						DrawTmp -= ScreenWidth;
					}			
					break;
				default:
					return iFALSE;
					break;
			}

			SrcBufPtr += CurrentImageWidth; 

			if (DataSize > SrcBufLen-3) break;

		}
	}
 
	return iTRUE;

}

static iBOOL   i51KitG2RotateImageEx1( iHANDLE ContainerHandle, iU32 ImageID , iRECT* DesRect , iRECT* SrcRect, 
									 iS32 SrcMoveX , iS32 SrcMoveY, iS32 RotateCenterX , iS32 RotateCenterY , iU32 RotateType, iS32 Angle )
{
	// author : yepan
	// since : 2012-05-30
	// (C) 2012 , PKIG Tech. Co., Ltd.

	//ContainerHandle	待绘制图片所在容器的句柄
	//ImageID	图片ID
	//DesRect	图片绘制的目的区域
	//SrcRect	图片绘制的裁剪区
	//SrcMoveX	图片旋转前平移至点（SrcMoveX,SrcMoveY）
	//SrcMoveY	图片旋转前平移至点（SrcMoveX,SrcMoveY）
	//RotateCenterX	图片旋转中心X坐标
	//RotateCenterY	图片旋转中心Y坐标
	//RotateType	旋转类型
	//Angle	旋转角度 只处理90，180，270特殊角度

	// notes : 按顺时针方向旋转一定角度后绘制ContainerHandle容器内编号为ImageID的图像

	iBOOL	Ret = iTRUE;

	// 图片容器地址入口
	i51ImageContainer * ImageContainer = iNULL;
	// 图片索引表地址
	i51ImageIndex * ImageIndexTable = iNULL;
	// 容器文件指针地址
	iFILE File = iNULL;

	// 图片宽度和高度
	iU32 ImageWidth = 0, ImageHeight = 0;
	//图片数据总长度/图片基本数据长度
	iU32 LengthTotalData = 0, LengthIamgeData = 0;
	iU32 ReserveOffset = 0x0c;
	// 带Alpha信息的数据点个数
	iU16 TotalAlphaNumber = 0;

	// 图片数据文件开始地址
	iU32 DataEntry = 0;
	//图片数据缓存指针
	iU8  *ImageDataEntry = iNULL;

	//读取的数据长度
	iU32	ReadLenght = 0;	

	//角度转换到0~360
	if( (Angle%90)!=0 )
	{
		iLog("KIT - G2 : angle no special");
		return iFALSE;
	}

	ImageContainer = (i51ImageContainer *)ContainerHandle;
	if ( iNULL == ImageContainer )
	{
		return iFALSE;
	}

	ImageIndexTable = ImageContainer->ImageIndexTable;
	File = ImageContainer->FileHandle;
	if ( iNULL == ImageIndexTable || iNULL == File)
	{
		return iFALSE;
	}

	DataEntry = ImageIndexTable[ImageID].MemoryEntry;
	LengthTotalData = ImageIndexTable[ImageID].LengthTotalData;
	LengthIamgeData = ImageIndexTable[ImageID].LengthIamgeData;
	TotalAlphaNumber = ImageIndexTable[ImageID].AlphaPixelNum;
	ImageWidth = ImageIndexTable[ImageID].ImageWidth;
	ImageHeight = ImageIndexTable[ImageID].ImageHeight;

	//加载图片数据到内存
	if( iFALSE==ImageIndexTable[ImageID].FlagCopyRAM )
	{
		DataEntry = ImageIndexTable[ImageID].ImageEntry;
		if( iFALSE == i51AdeOsFileSetSeek( File, DataEntry, i51_OS_FL_SEEK_HEAD ) )
		{
			return iFALSE;
		}

		ImageDataEntry = i51AdeOsMalloc(LengthTotalData, __FILE__ , __LINE__ );
		if( iNULL == ImageDataEntry ) 
		{
			iLog("KIT - G2 : i51KitG2RotateImageEx malloc err");
			return iFALSE;
		}

		Ret = i51AdeOsFileRead( File, ImageDataEntry, LengthTotalData, &ReadLenght);
		if( iFALSE == Ret || LengthTotalData!=ReadLenght) 
		{
			i51AdeOsFree(ImageDataEntry);
			ImageDataEntry = iNULL;
			iLog(" KIT - G2 : rotate read data");
			return iFALSE;             
		}
	}
	else
	{
		ImageDataEntry = (iU8*)ImageIndexTable[ImageID].MemoryEntry;
	}

	DataEntry = (iU32)ImageDataEntry;
	DataEntry += I51KITG2_OFFSET_PIXELDATA;

	//画基本像素
 	if ( iFALSE == i51G2DrawBasePixByAngle((iU16 *)DataEntry, (iU32)(LengthIamgeData-I51KITG2_OFFSET_PIXELDATA),
						SrcMoveX, SrcMoveY, RotateCenterX, RotateCenterY,DesRect, SrcRect,  Angle) )
	{
		if( iFALSE==ImageIndexTable[ImageID].FlagCopyRAM  && ImageDataEntry )
		{
			i51AdeOsFree(ImageDataEntry);
			ImageDataEntry = iNULL;
		}
		return iFALSE;
	}

	//画alpha像素
	if(TotalAlphaNumber>0)
	{
		iS32 CurrAlphaNum = 0;
		iS32 LcdBuffPos = 0;
		iU16 *ReadDataBuf = iNULL;
		iALPHA AlphaNumber = 0;
		iS16 AlphaColor = 0;

		iS16 SrcAlphaX = 0, SrcAlphaY = 0, DesAlphaX=0, DesAlphaY=0;
		iS16 OffSetX=0, OffSetY=0;

		iS32 DesStartX = 0, DesStartY = 0, DesEndX = 0, DesEndY = 0;
		iS32 SrcStartX = 0, SrcStartY = 0, SrcEndX = 0, SrcEndY = 0;

		DesStartX = DesRect->X;
		DesStartY = DesRect->Y;
		DesEndX = DesStartX + DesRect->Width;
		DesEndY = DesStartY + DesRect->Height;

		SrcStartX = SrcRect->X;
		SrcStartY = SrcRect->Y;
		SrcEndX = SrcStartX + SrcRect->Width;
		SrcEndY = SrcStartY + SrcRect->Height;

		DataEntry = (iU32)ImageDataEntry;
		DataEntry += (LengthIamgeData+ReserveOffset);
		ReadDataBuf = (iU16 *)DataEntry;

		switch( Angle )
		{
			case 0:
				// 点A(x,y)旋转0°后对应坐标点:
				// B(x+ SrcMoveX -inSrcRect->X, y+ SrcMoveY  -inSrcRect->Y )
				// 简化为:B(y+OffSetX,x+OffSetY);
				OffSetX = SrcMoveX - SrcRect->X ;
				OffSetY = SrcMoveY - SrcRect->Y ;
				break;
			case 90:
				// 点A(x,y)旋转90°后对应坐标点:
				// B(SrcRect->Y - SrcMoveY + RotateCenterY + RotateCenterX-y, SrcMoveX - SrcRect->X + RotateCenterY - RotateCenterX+x)
				// 简化为:B(y+OffSetX,x+OffSetY);
				OffSetY = SrcMoveX - SrcRect->X + RotateCenterY - RotateCenterX;
				OffSetX = SrcRect->Y - SrcMoveY + RotateCenterY + RotateCenterX;
				break;
			case 180:	
				// 点A(x,y)旋转180°后对应坐标点:
				// B(2*RotateCenterX+SrcRect->X-SrcMoveX-x,)
				// 简化为:B(OffSetX-x,OffSetY-y,2*RotateCenterY+SrcRect->Y-SrcMoveY);
				OffSetY = 2*RotateCenterY+SrcRect->Y-SrcMoveY;
				OffSetX = 2*RotateCenterX+SrcRect->X-SrcMoveX;
				break;
			case 270:	
				// 点A(x,y)旋转270°后对应坐标点:
				// B(RotateCenterX - RotateCenterY  + SrcMoveY - SrcRect->Y +y, RotateCenterY + RotateCenterX - SrcMoveX + SrcRect->X - x )
				// 简化为:B(OffSetX+x,OffSetY+y);
				OffSetY = RotateCenterY + RotateCenterX - SrcMoveX + SrcRect->X;
				OffSetX = RotateCenterX - RotateCenterY  + SrcMoveY - SrcRect->Y ;
				break;
			default:
				iLog(" KIT - G2 : angle %d", Angle);
				return iFALSE;
		}

		for (CurrAlphaNum=0; CurrAlphaNum<TotalAlphaNumber; CurrAlphaNum++)
		{
			SrcAlphaX   = ReadDataBuf[0];
			SrcAlphaY   = ReadDataBuf[1];
			AlphaNumber	  = (iALPHA)ReadDataBuf[2];
			AlphaColor    = ReadDataBuf[3];
			ReadDataBuf   += 4;

			if (SrcAlphaX < SrcStartX || SrcAlphaX >= SrcEndX ||
				SrcAlphaY < SrcStartY || SrcAlphaY >= SrcEndY )
			{
				continue;
			}


			switch (Angle)
			{
				case 0:
					DesAlphaX = OffSetX + SrcAlphaX;
					DesAlphaY = OffSetY + SrcAlphaY;
					break;
				case 90:
					DesAlphaX = OffSetX - SrcAlphaY;
					DesAlphaY = OffSetY + SrcAlphaX;
					break;
				case 180:
					DesAlphaX = OffSetX - SrcAlphaX;
					DesAlphaY = OffSetY - SrcAlphaY;

					break;
				case 270:
					DesAlphaX = OffSetX + SrcAlphaY;
					DesAlphaY = OffSetY - SrcAlphaX;
					break;
				default:
					if( iFALSE==ImageIndexTable[ImageID].FlagCopyRAM  && ImageDataEntry )
					{
						i51AdeOsFree(ImageDataEntry);
						ImageDataEntry = iNULL;
					}
					return iFALSE;
					break;
			}

			if (	DesAlphaX < DesStartX || DesAlphaX >= DesEndX 
			||	DesAlphaY < DesStartY || DesAlphaY >= DesEndY )
			{
				continue;
			}
			LcdBuffPos = ScreenWidth*DesAlphaY+ DesAlphaX;
			LcdBuffer[LcdBuffPos] = i51KitG2GetNewColorEx(LcdBuffer[LcdBuffPos], AlphaColor, AlphaNumber);
		}
	}

	if( iFALSE==ImageIndexTable[ImageID].FlagCopyRAM  && ImageDataEntry )
	{
		i51AdeOsFree(ImageDataEntry);
		ImageDataEntry = iNULL;
	}

	return iTRUE;

}
#endif

iKIT iBOOL   i51KitG2RotateImage   ( iHANDLE ContainerHandle, iU32 ImageID , iRECT* DesRect , iRECT* SrcRect, 
	iS32 SrcMoveX , iS32 SrcMoveY, iS32 RotateCenterX , iS32 RotateCenterY , iU32 RotateType, iS32 Angle )
{
    // author : Otto
    // since : Aug 29 2011
    // (C) 2011 , PKIG Tech. Co., Ltd.

    //ContainerHandle	待绘制图片所在容器的句柄
    //ImageID	图片ID
    //DesRect	图片绘制的目的区域
    //ImageHeight	图片绘制的裁剪区
    //SrcMoveX	图片旋转前平移至点（SrcMoveX,SrcMoveY）
    //SrcMoveY	图片旋转前平移至点（SrcMoveX,SrcMoveY）
    //RotateCenterX	图片旋转中心X坐标
    //RotateCenterY	图片旋转中心Y坐标
    //RotateType	旋转类型
    //Angle	旋转角度
    
    // notes : 按顺时针方向旋转一定角度后绘制ImageContainer容器内编号为ImageID的图像
    // SrcRect 为空时在点(DesRect->X, DesRect->Y)处开始绘制整个图片
    // DesRect 为空则绘制从屏幕左上角开始

#define ROTATE_MAX_GIRTH_HALF 600

    // 图片容器地址入口
    i51ImageContainer * ImageContainer = iNULL;
    // 图片索引表地址
    i51ImageIndex * ImageIndexTable = iNULL;
    // 容器文件指针地址
    iFILE File = iNULL;
     // 图片编码方式
    iU8   DecodeType = 0;
    iU16 CurrDrawID;
    iRECT * VPActualRect = iNULL;
    iRECT ImageRect;
    iRECT DrawDesRect;
    iRECT DrawSrcRect;
    iU8 Ret = iFALSE;
    iS16 * GirthPoint = iNULL;

 #if i51KITG2_DEBUG
    iLog( "G2RotateImage start , %x,%x,%x,%x,%x,%x,%x,%x,%x,%x"
    ,ContainerHandle,ImageID,DesRect,SrcRect,SrcMoveX,SrcMoveY,RotateCenterX,RotateCenterY,RotateType,Angle);
#endif

    if( iNULL== ContainerHandle) return iFALSE;
    ImageContainer = (i51ImageContainer * )ContainerHandle;
    ImageIndexTable = ( i51ImageIndex *  )(ImageContainer->ImageIndexTable);
    File = ImageContainer->FileHandle;
    if( iNULL == ImageIndexTable || iNULL==File)  return iFALSE;
	
    if(ImageIndexTable[ImageID].FlagImageExist==0 ) return iFALSE;

    DecodeType = ImageIndexTable[ImageID].DecodeType;
    if(DecodeType!=I51KITG2_DECODE_LINEAR)
    {
    	iLog("文件格式错误，请重新生成图片文件!");
    	return iFALSE;
    }


    // 在虚拟面板上绘制，实际上只是进行一些赋值操作
    if( i51UseVirtualPanel && i51DrawVirtualPanel )
    {
        if( i51CurrentVpManage.TotalVPNumber == 0 ) return iFALSE;

        if( iNULL == i51CurrentVpActivity )	return iFALSE;
		
        CurrDrawID = i51CurrentVpActivity->VPCurrentElement;
        // 画板已经画满则直接返回
        if(  i51CurrentVpActivity->VpIsFull ) return iFALSE;
      
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementType = I51KITG2_ELEMENT_TYPE_PICTURE;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementNumber = CurrDrawID;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElemenAlpha = AlphaSys;
        if( SrcRect )
        {
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.SrcRect.X = SrcRect->X;
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.SrcRect.Y = SrcRect->Y;
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.SrcRect.Width = SrcRect->Width;
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.SrcRect.Height = SrcRect->Height;
        }
        else
        {
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.SrcRect.X = 0;
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.SrcRect.Y = 0;
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.SrcRect.Width = ImageIndexTable[ImageID].ImageWidth;
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.SrcRect.Height = ImageIndexTable[ImageID].ImageHeight;
        }
		
        if( DesRect )		
        {
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.DesRect.X = DesRect->X;
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.DesRect.Y = DesRect->Y;
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.DesRect.Width = DesRect->Width;
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.DesRect.Height = DesRect->Height;
        }
        else
        {
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.DesRect.X =0;
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.DesRect.Y =0;
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.DesRect.Width = ScreenWidth;
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.DesRect.Height = ScreenHeight;
        }
		
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.DrawDirection = I51KITG2_DRAW_IAMGE_ROTATE;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.ImageHandle = ImageID;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.ContainerHandle = ContainerHandle;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.RotateCenterX = RotateCenterX;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.RotateCenterY = RotateCenterY;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.RotateType = RotateType;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.SrcMoveX = SrcMoveX;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.SrcMoveY = SrcMoveY;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.DarwAngle = Angle;
        i51CurrentVpActivity->VPCurrentElement =  CurrDrawID +1;
        if( (CurrDrawID+1) >= i51CurrentVpActivity->VPMaxElement )	i51CurrentVpActivity->VpIsFull = iTRUE;
        return iTRUE;
    }

    if(i51UseVirtualPanel) VPActualRect = &(i51CurrentVpRender->VPActualRect);
	
    if( iNULL==SrcRect )
    {
        DrawSrcRect.X = 0;
        DrawSrcRect.Y = 0;
        DrawSrcRect.Width = ImageIndexTable[ImageID].ImageWidth;
        DrawSrcRect.Height = ImageIndexTable[ImageID].ImageHeight;
    }
    else
    {
        DrawSrcRect.X = SrcRect->X;
        DrawSrcRect.Y = SrcRect->Y;
        DrawSrcRect.Width = SrcRect->Width;
        DrawSrcRect.Height = SrcRect->Height;
    }

    if( iNULL==DesRect )
    {
        if( i51UseVirtualPanel )
        {
            DrawDesRect.X = VPActualRect->X;
            DrawDesRect.Y = VPActualRect->Y;
            DrawDesRect.Width = VPActualRect->Width;
            DrawDesRect.Height = VPActualRect->Height;
        }
        else
        {
            DrawDesRect.X = 0;
            DrawDesRect.Y = 0;
            DrawDesRect.Width = ScreenWidth;
            DrawDesRect.Height = ScreenHeight;
        }
    }
    else
    {
        DrawDesRect.X = DesRect->X;
        DrawDesRect.Y = DesRect->Y;
        DrawDesRect.Width = DesRect->Width;
        DrawDesRect.Height = DesRect->Height;
        if( i51UseVirtualPanel )
        {
            if( (DrawDesRect.X+DrawDesRect.Width)>(VPActualRect->Width))
            {
                DrawDesRect.Width = VPActualRect->Width - DrawDesRect.X;
            }
			
            if( (DrawDesRect.Y+DrawDesRect.Height)>(VPActualRect->Height))
            {
                DrawDesRect.Height= VPActualRect->Height- DrawDesRect.Y;
            }
			
            DrawDesRect.X += VPActualRect->X;
            DrawDesRect.Y += VPActualRect->Y;
        }
    }
    

    ImageRect.X = 0;
    ImageRect.Y = 0;
    ImageRect.Width= ImageIndexTable[ImageID].ImageWidth;
    ImageRect.Height= ImageIndexTable[ImageID].ImageHeight;

    if(DrawSrcRect.X<0) { DrawSrcRect.Width+= DrawSrcRect.X; DrawSrcRect.X = 0;}
    else if( DrawSrcRect.X>=ImageRect.Width) return iTRUE;
    if( (DrawSrcRect.X+DrawSrcRect.Width) > ImageRect.Width ) DrawSrcRect.Width = ImageRect.Width -DrawSrcRect.X;

    if(DrawSrcRect.Y<0) { DrawSrcRect.Height+= DrawSrcRect.Y; DrawSrcRect.Y = 0;}
    else if( DrawSrcRect.Y>=ImageRect.Height) return iTRUE;
    if( (DrawSrcRect.Y+DrawSrcRect.Height) > ImageRect.Height) DrawSrcRect.Height= ImageRect.Height-DrawSrcRect.Y;

    if(DrawDesRect.X<0) { DrawDesRect.Width+= DrawDesRect.X; DrawDesRect.X = 0;}
    else if( DrawDesRect.X>=ScreenWidth) return iTRUE;
    if( (DrawDesRect.X+DrawDesRect.Width) > ScreenWidth ) DrawDesRect.Width = ScreenWidth -DrawDesRect.X;
		
    if(DrawDesRect.Y<0) { DrawDesRect.Height+= DrawDesRect.Y; DrawDesRect.Y = 0;}
    else if( DrawDesRect.Y>=ScreenHeight) return iTRUE;
    if( (DrawDesRect.Y+DrawDesRect.Height) > ScreenHeight) DrawDesRect.Height= ScreenHeight-DrawDesRect.Y;

    if( DrawSrcRect.Width<=0 || DrawSrcRect.Height<=0 || DrawDesRect.Width<=0 || DrawDesRect.Height<=0 )
    {
    	return iTRUE;
    }
		
    DecodeType = ImageIndexTable[ImageID].DecodeType;

	//角度转换到0~360
	Angle = Angle%360;
	if( Angle<0 ) Angle += 360;
	
//	while ( Angle < 0 ){Angle += 360;}
//	while ( Angle >= 360 ){Angle -= 360;}

#if ROTATE_SPECIAE
	if ((Angle%90)==0 )
	{
		return i51KitG2RotateImageEx1(ContainerHandle, ImageID , &DrawDesRect , &DrawSrcRect, 
			SrcMoveX , SrcMoveY, RotateCenterX , RotateCenterY , RotateType, Angle );
	}
#endif

    if( (DrawSrcRect.Height+DrawSrcRect.Width)>=ROTATE_MAX_GIRTH_HALF  )   return iFALSE;
	
    GirthPoint = (iS16*)(i51AdeOsMalloc((DrawSrcRect.Height+DrawSrcRect.Width)*4,__FILE__ , __LINE__ ));

    if(iNULL==GirthPoint) 
    {
        iLog("KIT - G2 : malloc rotate");
        return iFALSE;		
    }
	
    if( iNULL==i51KitG2GetGirthPoints(DrawSrcRect, DrawDesRect,(iS16(*)[2])GirthPoint, SrcMoveX, SrcMoveY, RotateCenterX, RotateCenterY,RotateType, Angle))
    {
        i51AdeOsFree(GirthPoint);
        GirthPoint = iNULL;
        return iTRUE;
    }

    if( ImageIndexTable[ImageID].FlagCopyRAM )
    {
            // 线性压缩编码方式
            // 把图像像素信息按行优先进行线性编码，数据格式如下：
            // x,y,w,RGB数据，其中x，y是非透明色的起点坐标，w是连续有多少个非透明色，x，y分别占用1.5字节，w占两个字节。
            Ret = RotateRamLinearImage(ContainerHandle, ImageID, &(DrawDesRect),  &(DrawSrcRect), (iS16(*)[2])GirthPoint);
    }
    else
    {  
            // 显示线性压缩编码图片
            // 把图像像素信息按行优先进行线性编码，数据格式如下：
            // x,y,w,RGB数据，其中x，y是非透明色的起点坐标，w是连续有多少个非透明色，x，y分别占用1.5字节，w占两个字节。
            Ret = RotateRamLinearImage(ContainerHandle, ImageID, &(DrawDesRect),  &(DrawSrcRect), (iS16(*)[2])GirthPoint);
    }
	
#undef ROTATE_MAX_GIRTH_HALF
    i51AdeOsFree(GirthPoint);
    GirthPoint = iNULL;

 #if i51KITG2_DEBUG
    iLog( "G2RotateImage start , %x,%x,%x,%x,%x,%x,%x,%x,%x,%x"
    ,ContainerHandle,ImageID,DesRect,SrcRect,SrcMoveX,SrcMoveY,RotateCenterX,RotateCenterY,RotateType,Angle);
#endif

    return Ret;

}


iKIT iBOOL i51KitG2ImageFillRect(iHANDLE ContainerHandle,iU32 ImageID , iRECT* DesRect , iRECT* SrcRect )
{
    // author : Otto
    // since : 2011.11.7
    // (C) 2011 , PKIG Tech. Co., Ltd.

    //  iHANDLE ImageContainer ：容器文件指针
    //  iU16 ImageID  ：容器内图片文件ID
    //  iRECT* DesRect :   绘制目的地区域
    //  iRECT* SrcRect :    绘制图片原始区域
    
    // notes : 绘制ImageContainer容器内编号为ImageID的图像
    // SrcRect 为空时在点(DesRect->X, DesRect->Y)处开始绘制整个图片
    // DesRect 为空则绘制从屏幕左上角开始

    // 图片容器地址入口
    i51ImageContainer * ImageContainer = iNULL;
    // 图片索引表地址
    i51ImageIndex * ImageIndexTable = iNULL;
    // 容器文件指针地址
    iFILE File = iNULL;
     // 图片编码方式
    iU8   DecodeType = 0;
//    iRECT DrawSrcRect;
    iRECT CurrFillRect;
    iU16 CurrDrawID;
    iS32 ImageWidth = 0;
    iS32 ImageHeight = 0;	

     // 绘制到屏幕上的区域
    iS16 DesStartX = 0;
    iS16 DesStartY = 0;
    iS16 DesEndX = 0;
    iS16 DesEndY = 0;
    iS16 DesWidth = 0;
    iS16 DesHeight = 0;

     // 绘制图片的区域
    iS16 SrcStartX = 0;
    iS16 SrcStartY = 0;
    iS16 SrcWidth = 0;
    iS16 SrcHeight = 0;

    // VP的位置RECT
    iRECT * VPActualRect = iNULL;
    // VP需要绘制区域的RECT
    iRECT * VPDrawRect = iNULL;

 #if i51KITG2_DEBUG
    iLog( "G2ImageFillRect start , %x,%x,%x,%x"
    ,ContainerHandle,ImageID,DesRect,SrcRect);
#endif

    if( iNULL== ContainerHandle) return iFALSE;
    ImageContainer = (i51ImageContainer *)ContainerHandle;
    ImageIndexTable = ( i51ImageIndex *  )(ImageContainer->ImageIndexTable);
    File = ImageContainer->FileHandle;
    if( iNULL == ImageIndexTable || iNULL==File)  return iFALSE;

    if(ImageIndexTable[ImageID].FlagImageExist==0 ) return iFALSE;
	
    DecodeType = ImageIndexTable[ImageID].DecodeType;
    ImageWidth = ImageIndexTable[ImageID].ImageWidth;
    ImageHeight = ImageIndexTable[ImageID].ImageHeight;

    if(DecodeType!=I51KITG2_DECODE_LINEAR)
    {
    	iLog("文件格式错误，请重新生成图片文件!");
    	return iFALSE;
    }

    if(i51UseVirtualPanel)
    {
        VPActualRect = &(i51CurrentVpRender->VPActualRect);
        VPDrawRect = &(i51CurrentVpRender->VPDrawRect);
    }
	
	
    // 在虚拟面板上绘制，实际上只是进行一些赋值操作
    if( i51UseVirtualPanel && i51DrawVirtualPanel )
    {
        if( i51CurrentVpManage.TotalVPNumber == 0 ) return iFALSE;

        if( iNULL == i51CurrentVpActivity )	return iFALSE;
		
        // 画板已经画满则直接返回
        if(  i51CurrentVpActivity->VpIsFull ) return iFALSE;
      
        CurrDrawID = i51CurrentVpActivity->VPCurrentElement;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementType = I51KITG2_ELEMENT_TYPE_FILLIMAGE;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementNumber = CurrDrawID;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElemenAlpha = AlphaSys;
        if( SrcRect )
        {
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Fill.SrcRect.X = SrcRect->X;
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Fill.SrcRect.Y = SrcRect->Y;
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Fill.SrcRect.Width = SrcRect->Width;
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Fill.SrcRect.Height = SrcRect->Height;
        }
        else
        {
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Fill.SrcRect.X = 0;
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Fill.SrcRect.Y = 0;
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Fill.SrcRect.Width = ImageWidth;
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Fill.SrcRect.Height = ImageHeight;
        }
		
        if( DesRect )		
        {
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Fill.DesRect.X = DesRect->X;
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Fill.DesRect.Y = DesRect->Y;
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Fill.DesRect.Width = DesRect->Width;
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Fill.DesRect.Height = DesRect->Height;
        }
        else
        {
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Fill.DesRect.X =0;
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Fill.DesRect.Y =0;
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Fill.DesRect.Width = i51CurrentVpActivity->VPActualRect.Width;
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Fill.DesRect.Height = i51CurrentVpActivity->VPActualRect.Height;
        }

        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Fill.ImageHandle = ImageID;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Fill.ContainerHandle = ContainerHandle;
        i51CurrentVpActivity->VPCurrentElement =  CurrDrawID +1;
        if( (CurrDrawID+1) >= i51CurrentVpActivity->VPMaxElement )	i51CurrentVpActivity->VpIsFull = iTRUE;
        return iTRUE;
    }

    if( SrcRect )
    {
        SrcStartX= SrcRect->X;
        SrcStartY= SrcRect->Y;
        SrcWidth= SrcRect->Width;
        SrcHeight= SrcRect->Height;
    }
    else
    {
        SrcStartX= 0;
        SrcStartY= 0;
        SrcWidth= ImageWidth;
        SrcHeight= ImageHeight;
    }
    if( SrcStartX>=ImageWidth || SrcStartY>=ImageHeight) return iTRUE;
    if( SrcStartX< 0 ) { SrcWidth+= SrcStartX; SrcStartX= 0; }
    if( SrcStartY< 0 ) { SrcHeight+= SrcStartY; SrcStartY= 0; }

    if( i51UseVirtualPanel == iFALSE )
    {
        if( DesRect)
        {
            DesStartX = DesRect->X;
            DesStartY = DesRect->Y;
            DesWidth = DesRect->Width;
            DesHeight = DesRect->Height;
        }
        else
        {
            DesStartX = 0;
            DesStartY = 0;
            DesWidth = ScreenWidth;
            DesHeight = ScreenHeight;
        }
    }
    else
    {
        if( DesRect)
        {
            DesStartX = DesRect->X ;
            DesStartY = DesRect->Y ;
            DesWidth = DesRect->Width;
            DesHeight = DesRect->Height;
        }
        else
        {
            DesStartX = 0;
            DesStartY = 0;
            DesWidth = VPActualRect->Width;
            DesHeight = VPActualRect->Height;
        }
#if 0		
        // 计算使用VP时绘制的屏幕区域
        DesEndX = DesStartX + DesWidth;
        DesEndY = DesStartY + DesHeight;
        if( DesStartX<VPDrawRect->X )  DesStartX = VPDrawRect->X;
        if( DesStartY<VPDrawRect->Y ) DesStartY = VPDrawRect->Y;
        if( DesEndX>(VPDrawRect->X+VPDrawRect->Width) ) DesEndX = VPDrawRect->X+VPDrawRect->Width;
        if( DesEndY>(VPDrawRect->Y+VPDrawRect->Height) ) DesEndY = VPDrawRect->Y+VPDrawRect->Height;
        DesWidth = DesEndX - DesStartX;
        DesHeight = DesEndY - DesStartY;
#endif
    }	
#if 0
    if( DesStartX>=ScreenWidth|| DesStartY>=ScreenHeight ) return iTRUE;
    if( DesStartX < 0 ) { DesWidth += DesStartX; DesStartX= 0; }
    if( DesStartY < 0 ) { DesHeight += DesStartY; DesStartY= 0; }
    if( DesWidth<=0 || DesHeight<=0 ) return iTRUE;
#endif
    DesEndX = DesStartX + DesWidth;
    DesEndY = DesStartY + DesHeight;
	
    CurrFillRect.X = DesStartX;
    CurrFillRect.Y = DesStartY;
    CurrFillRect.Width= DesWidth;
    CurrFillRect.Height= DesHeight;     

    if( ImageIndexTable[ImageID].FlagCopyRAM )
    {
        while(1)
        {
            // 线性压缩编码方式
            // 把图像像素信息按行优先进行线性编码，数据格式如下：
            // x,y,w,RGB数据，其中x，y是非透明色的起点坐标，w是连续有多少个非透明色，x，y分别占用1.5字节，w占两个字节。
            DrawRAMLinearImage( ContainerHandle, ImageID,  &(CurrFillRect), SrcRect, I51KITG2_DRAW_IAMGE_NORMAL);
            CurrFillRect.X += SrcWidth;
            CurrFillRect.Width -= SrcWidth;
            if( (CurrFillRect.X >= DesEndX) || (CurrFillRect.X>=ScreenWidth) || (CurrFillRect.Width<=0) )
            {
                CurrFillRect.X = DesStartX;
                CurrFillRect.Y += SrcHeight;
                CurrFillRect.Width= DesWidth;
                CurrFillRect.Height -= SrcHeight;
            }
            if( (CurrFillRect.Y >= DesEndY) || (CurrFillRect.Y>=ScreenHeight) || ( CurrFillRect.Height<=0))
            {
                break;
            }
        }
    }
    else
    {  
        while(1)
        {
            // 显示线性压缩编码图片
            // 把图像像素信息按行优先进行线性编码，数据格式如下：
            // x,y,w,RGB数据，其中x，y是非透明色的起点坐标，w是连续有多少个非透明色，x，y分别占用1.5字节，w占两个字节。
            DrawRAMLinearImage( ContainerHandle, ImageID, &(CurrFillRect),  SrcRect, I51KITG2_DRAW_IAMGE_NORMAL);
            CurrFillRect.X += SrcWidth;
            CurrFillRect.Width -= SrcWidth;
            if( (CurrFillRect.X >= DesEndX) || (CurrFillRect.X>=ScreenWidth) || (CurrFillRect.Width<=0) )
            {
                CurrFillRect.X = DesStartX;
                CurrFillRect.Y += SrcHeight;
                CurrFillRect.Width= DesWidth;
                CurrFillRect.Height -= SrcHeight;
            }
            if( (CurrFillRect.Y >= DesEndY) || (CurrFillRect.Y>=ScreenHeight) || ( CurrFillRect.Height<=0))
            {
                break;
            }
        }

    }
 #if i51KITG2_DEBUG
    iLog( "G2ImageFillRect end , %x,%x,%x,%x"
    ,ContainerHandle,ImageID,DesRect,SrcRect);
#endif

    return iTRUE;
	
}

iKIT iBOOL i51KitG2ScreenZoom( iRECT * DesRect, iRECT * SrcRect, iU32 ZoomRateX, iU32 ZoomRateY )
{
    // author : Otto
    // since : 2011.12.6
    // (C) 2011 , PKIG Tech. Co., Ltd.

    // iRECT DesRect: 影像缩放到的目的区域
    // iRECT SrcRect : 影像缩放的原始区域
    // iU32 ZoomRateX : 影像X方向缩放的比例
    // iU32 ZoomRateY : 影像Y方向缩放的比例
    
    // notes: 将物理Buffer进行缩放
    
    iS16 * NewPosX = iNULL;
    iS16 * NewPosY = iNULL;
    iU32 iTemp =0;	
    iS32 SrcStartX = 0;
    iS32 SrcStartY = 0;
    iS32 SrcEndX = 0;
    iS32 SrcEndY = 0;
    iS32 DesStartX = 0;
    iS32 DesStartY = 0;
    iS32 DesEndX = 0;
    iS32 DesEndY = 0;
    iS32 SrcCurrX = 0;
    iS32 SrcCurrY = 0;
    iS32 DesCurrX = 0;
    iS32 DesCurrY = 0;

    iU32 NewLcdBuffer = 0;
    iU32 OldLcdBuffer = 0;
    iU16 * CurrScreenBuffer = iNULL;
	
    if(ZoomRateX==100&&ZoomRateY==100) return iTRUE;

    if(ZoomRateX==0||ZoomRateY==0) return iFALSE;
	
    if(DesRect)
    {
        DesStartX = DesRect->X;
        DesStartY = DesRect->Y;
        DesEndX = DesRect->Width + DesStartX;
        DesEndX = DesRect->Height + DesStartY;

        if( DesStartX<0 ) DesStartX = 0;
        if( DesStartY<0 ) DesStartY = 0;
        if( DesEndX>ScreenWidth ) DesEndX = ScreenWidth;
        if( DesStartY>ScreenHeight ) DesEndY = ScreenHeight;
    	
        if( DesStartX>=ScreenWidth ) return iTRUE;
        if( DesStartY>=ScreenHeight) return iTRUE;
        if( DesEndX<0 ) return iTRUE;
        if( DesEndY<0 ) return iTRUE;
	
    }
    else
    {
        DesStartX = 0;
        DesStartY = 0;
        DesEndX = ScreenWidth;
        DesEndY = ScreenHeight;
    }

    if( SrcRect )
    {
        SrcStartX = SrcRect->X;
        SrcStartY = SrcRect->Y;
        SrcEndX = SrcRect->Width + SrcStartX;
        SrcEndY = SrcRect->Height + SrcStartY;

        if( SrcStartX<0 ) SrcStartX = 0;
        if( SrcStartY<0 ) SrcStartY = 0;
        if( SrcEndX >ScreenWidth ) SrcEndX = ScreenWidth;
        if( SrcEndY >ScreenHeight ) SrcEndY = ScreenHeight;
    	
        if( SrcStartX>=ScreenWidth ) return iTRUE;
        if( SrcStartY>=ScreenHeight) return iTRUE;
        if( SrcEndX <0 ) return iTRUE;
        if( SrcEndY <0 ) return iTRUE;

    }
    else
    {
        SrcStartX = 0;
        SrcStartY = 0;
        SrcEndX = ScreenWidth;
        SrcEndY = ScreenHeight;
    }

    NewPosX = (iS16*) i51AdeOsMalloc((ScreenWidth<<1),   __FILE__ , __LINE__ );
    if( iNULL==NewPosX ) goto ERROR;
    for( iTemp=0; iTemp<ScreenWidth; iTemp++ )
    {
        NewPosX[iTemp] = (iTemp*100)/ZoomRateX;
    }

    NewPosY = (iS16*) i51AdeOsMalloc((ScreenHeight<<1),   __FILE__ , __LINE__ );
    if( iNULL==NewPosX ) goto ERROR;
    for( iTemp=0; iTemp<ScreenHeight; iTemp++ )
    {
        NewPosY[iTemp] = (iTemp*100)/ZoomRateY;
    }

    CurrScreenBuffer = (iU16*) i51AdeOsMalloc(((ScreenHeight*ScreenWidth)<<1),   __FILE__ , __LINE__ );
    if( iNULL==CurrScreenBuffer ) goto ERROR;
    i51AdeStdMemcpy16(CurrScreenBuffer,LcdBuffer,((ScreenHeight*ScreenWidth)<<1));
	
    NewLcdBuffer = DesStartY*ScreenWidth + DesStartY;

    for( DesCurrY=DesStartY; DesCurrY<DesEndY; DesCurrY++ )
    {
        SrcCurrY = NewPosY[DesCurrY];
        if( SrcCurrY>=SrcEndY ) break;
		
        for( DesCurrX=DesStartX,iTemp = 0; DesCurrX<DesEndX;DesCurrX++,iTemp++)
        {
            SrcCurrX = NewPosX[DesCurrX];
            if( SrcCurrX>=SrcEndX ) break;
            OldLcdBuffer = SrcCurrY*ScreenWidth + SrcCurrX;
			
            LcdBuffer[NewLcdBuffer+iTemp] = CurrScreenBuffer[OldLcdBuffer];
        }
		
        NewLcdBuffer += ScreenWidth;
    }

    i51AdeOsFree(CurrScreenBuffer);
    i51AdeOsFree(NewPosX);
    i51AdeOsFree(NewPosY);

    CurrScreenBuffer = iNULL;
    NewPosX = iNULL;
    NewPosY = iNULL;

    return iTRUE;

ERROR:
    iLog("KIT - G2 : Zoom malloc");

    i51AdeOsFree(CurrScreenBuffer);
    i51AdeOsFree(NewPosX);
    i51AdeOsFree(NewPosY);

    CurrScreenBuffer = iNULL;
    NewPosX = iNULL;
    NewPosY = iNULL;

    return iFALSE;
	
}
	
static iBOOL i51KitG2FontSetUse ( iFILE ReFile, iU32 TypeheadID , i51KitG2Font * CurrTypehead ) 
{
    // author : Otto
    // since : 2011.10.11
    // (C) 2011 , PKIG Tech. Co., Ltd.

    // iFILE ReFile: 字库文件所在的资源文件句柄
    // iU32 TypeheadID : 字库文件在资源文件中的ID

    // 文件操作函数返回值
    iU8   FlagReturn = iFALSE;
    // 检测文件格式的临时变量
    iU8   FlagFile[4] = {0};
    // 从文件中读取的数据实际长度
    iU32 LengthReadData = 0;
    //  取字模类型	
    iU8  FontModelType = 0;
    //  字模宽度	
    iU8  FontModelWidth = 0;	
    //  字模高度
    iU8  FontModelHeight = 0;	
    //  常用字个数
    iU16 FontNormalWordsNum = 0;
    //  常用字字模偏移量
    iU32 FontNormalWordsModelOffset = 0;	
    //  所有字字模偏移量
    iU32 FontAllWordsModelOffset = 0;	
    // 字库文件在资源文件中的开始位置
    iU32 TypeheadOffset = 0;
	int Temp = 0;

//iLog("Start set use");
//    i51KitStdResSetContext(ReFile);
    if(iNULL==ReFile) return iFALSE;

	for( Temp=0; Temp<MAX_TYPEHEAD_NUMBER; Temp++ )
	{
        if( (ReFile == (i51KitG2FontManage[Temp].FontFileHandle)) 
		&& (TypeheadID== (i51KitG2FontManage[Temp].TypeheadID))  )
        {
			CurrTypehead->NormalWordsEntry = i51KitG2FontManage[Temp].NormalWordsEntry;
			CurrTypehead->FontFileHandle = i51KitG2FontManage[Temp].FontFileHandle;
			CurrTypehead->TypeheadID= i51KitG2FontManage[Temp].TypeheadID;
			CurrTypehead->WordHeight = i51KitG2FontManage[Temp].WordHeight;
			CurrTypehead->WordWidth = i51KitG2FontManage[Temp].WordWidth;
			CurrTypehead->TypeheadType = i51KitG2FontManage[Temp].TypeheadType;
			CurrTypehead->NormalUseWordsNum = i51KitG2FontManage[Temp].NormalUseWordsNum;	
			CurrTypehead->NormalWordsOffset = i51KitG2FontManage[Temp].NormalWordsOffset;
			CurrTypehead->AllWordsOffset = i51KitG2FontManage[Temp].AllWordsOffset;
			CurrTypehead->TypeheadOffset = i51KitG2FontManage[Temp].TypeheadOffset;
//			iLog("font in cache");
        	return iTRUE;
        }
		
	}
	
    TypeheadOffset = i51KitStdResGetContentEntry(ReFile, TypeheadID);
    FlagReturn = i51AdeOsFileSetSeek( ReFile, TypeheadOffset, i51_OS_FL_SEEK_HEAD );
    if( iFALSE == FlagReturn ) goto ERROR;                      

    // 前四个字节是CRC校验码
    FlagReturn = i51AdeOsFileSetSeek( ReFile, TypeheadOffset+i51KITG2_FONT_FILETYPE, i51_OS_FL_SEEK_HEAD );
    if( iFALSE == FlagReturn ) goto ERROR;                      

    // 检测打开的容器里面存放的文件是否为"i51F"格式
    FlagReturn = i51AdeOsFileRead( ReFile, FlagFile, 4, &LengthReadData);   
    if( (iFALSE == FlagReturn) || (LengthReadData!=4) 
    || (FlagFile[0]!='i') || (FlagFile[1]!='5') || (FlagFile[2]!='1') ||(FlagFile[3]!='F') )
    {
        iLog("KIT - G2 : file type font set use");
        goto ERROR;
    }

    FlagReturn = i51AdeOsFileSetSeek( ReFile, TypeheadOffset+i51KITG2_FONT_MODELTYPE, i51_OS_FL_SEEK_HEAD );
    if( iFALSE == FlagReturn ) goto ERROR;          

    FlagReturn = i51AdeOsFileRead( ReFile, &FontModelType, 1, &LengthReadData);   
    if( (iFALSE == FlagReturn) || (LengthReadData!=1) )
    {
       goto ERROR;
    }

    FlagReturn = i51AdeOsFileRead( ReFile, &FontModelWidth, 1, &LengthReadData);   
    if( (iFALSE == FlagReturn) || (LengthReadData!=1) )
    {
       goto ERROR;
    }

    FlagReturn = i51AdeOsFileRead( ReFile, &FontModelHeight, 1, &LengthReadData);   
    if( (iFALSE == FlagReturn) || (LengthReadData!=1) )
    {
       goto ERROR;
    }

    FlagReturn = i51AdeOsFileReadInt16( ReFile, &FontNormalWordsNum);   
    if( (iFALSE == FlagReturn) )       goto ERROR;
 
    FlagReturn = i51AdeOsFileReadInt32( ReFile, &FontNormalWordsModelOffset);   
    if( (iFALSE == FlagReturn))      goto ERROR;

    FlagReturn = i51AdeOsFileReadInt32( ReFile, &FontAllWordsModelOffset);   
    if( (iFALSE == FlagReturn))       goto ERROR;

    CurrTypehead->NormalWordsEntry = iNULL;
    CurrTypehead->FontFileHandle = ReFile;
    CurrTypehead->TypeheadID= TypeheadID;
    CurrTypehead->WordHeight = FontModelHeight;
    CurrTypehead->WordWidth = FontModelWidth;
    CurrTypehead->TypeheadType = FontModelType;
    CurrTypehead->NormalUseWordsNum = FontNormalWordsNum;	
    CurrTypehead->NormalWordsOffset = FontNormalWordsModelOffset+TypeheadOffset;
    CurrTypehead->AllWordsOffset = FontAllWordsModelOffset + TypeheadOffset;
    CurrTypehead->TypeheadOffset = TypeheadOffset;
//	iLog("font in file");
    return iTRUE;

ERROR:
    iLog("KIT - G2 : RW font set use");
    return iFALSE;
	
}

iKIT iBOOL i51KitG2FontCachEnable ( iFILE ReFile, iU32 TypeheadID  ) 
{
    // author : Otto
    // since : 2011.10.11
    // (C) 2011 , PKIG Tech. Co., Ltd.

    // iFILE ReFile : 字库所在资源文件句柄
    // iU32 TypeheadID  :字库在资源文件中的ID
    
    // notes: 把常用字加载进内存	

    // 打开的文件句柄	
    iFILE FileHandle = iNULL;
    // 文件操作函数返回值
    iU8   FlagReturn = iFALSE;
    // 从文件中读取的数据实际长度
    iU32 LengthReadData = 0;
    //  字模宽度	
    iU8  FontModelWidth = 0;	
    //  字模高度
    iU8  FontModelHeight = 0;	
    //  常用字个数
    iS16 FontNormalWordsNum = 0;
    //  常用字字模偏移量
    iS32 FontNormalWordsModelOffset = 0;	
    //  需要加载进内存的数据量
    iU32 NeedReadData = 0;	
    iS16 i = 0;
    // 常用字库管理	
    i51KitFontWord * NormalWordStock = iNULL;
    iU16 TempKeywords = 0;
    iU32 Temp = 0;
    iU32 UseTypeheadId = 0;
    	
 #if i51KITG2_DEBUG
    iLog( "G2FontCachEnable start , %x,%x"
    ,ReFile,TypeheadID);
#endif

    if( iNULL==ReFile ) return iFALSE;
	
    for( Temp=0; Temp<MAX_TYPEHEAD_NUMBER; Temp++ )
    {
        if( (ReFile == (i51KitG2FontManage[Temp].FontFileHandle)) 
		&& (TypeheadID== (i51KitG2FontManage[Temp].TypeheadID))  )
        {
        	return iTRUE;
        }
    }
	
    for( Temp=0; Temp<MAX_TYPEHEAD_NUMBER; Temp++ )
    {
        if( iNULL == (i51KitG2FontManage[Temp].FontFileHandle)  )
        {
            UseTypeheadId = Temp;
            break;
        }
    }
	
    if( Temp==MAX_TYPEHEAD_NUMBER)
    {
        return iFALSE;
    }

    if( i51KitG2FontManage[UseTypeheadId].NormalWordsEntry ) return iTRUE;

    i51KitG2FontSetUse(ReFile, TypeheadID, &i51KitG2FontManage[UseTypeheadId]);
    FileHandle = i51KitG2FontManage[UseTypeheadId].FontFileHandle;
    FontModelWidth =  i51KitG2FontManage[UseTypeheadId].WordWidth;
    FontModelHeight =  i51KitG2FontManage[UseTypeheadId].WordHeight;
    FontNormalWordsNum =  i51KitG2FontManage[UseTypeheadId].NormalUseWordsNum;
    FontNormalWordsModelOffset =  i51KitG2FontManage[UseTypeheadId].NormalWordsOffset;

    NeedReadData = (sizeof(i51KitFontWord) )* FontNormalWordsNum;

    FlagReturn = i51AdeOsFileSetSeek( FileHandle, FontNormalWordsModelOffset, i51_OS_FL_SEEK_HEAD );
    if( iFALSE == FlagReturn ) goto ERROR;                      

    NormalWordStock= (i51KitFontWord *)i51AdeOsMalloc( NeedReadData,  __FILE__ , __LINE__ );
    if( !NormalWordStock )
    {
        iLog("KIT - G2 : malloc font cach enable");
        return iFALSE;
    }

    NeedReadData = FontModelWidth*FontModelHeight+2;
    for( i=0; i<FontNormalWordsNum;i++)
    {
        FlagReturn = i51AdeOsFileRead( FileHandle, &NormalWordStock[i], NeedReadData, &LengthReadData);   
        if( (iFALSE == FlagReturn) || (LengthReadData!=NeedReadData) )
        {
           i51AdeOsFree(NormalWordStock);
           NormalWordStock = iNULL;
           goto ERROR;
        }
        if(i51AdeCnfGetEndian())
        {
            TempKeywords = NormalWordStock[i].CharactersKeywords;
            NormalWordStock[i].CharactersKeywords = ((TempKeywords<<8)&0XFF00)|((TempKeywords>>8)&0XFF);
        }
    }
    i51KitG2FontManage[UseTypeheadId].NormalWordsEntry = (iU32)NormalWordStock;
	
 #if i51KITG2_DEBUG
    iLog( "G2FontCachEnable end , %x,%x"
    ,ReFile,TypeheadID);
#endif

    return iTRUE;

ERROR:
    i51AdeOsFree(NormalWordStock);
    NormalWordStock = iNULL;
    iLog("KIT - G2 : RW font cach enable");
    return iFALSE;
}

iKIT iBOOL i51KitG2FontCachDisable(  iFILE ReFile, iU32 TypeheadID  ) 
{
    // author : Otto
    // since : 2011.10.11
    // (C) 2011 , PKIG Tech. Co., Ltd.

    // iFILE ReFile : 字库所在资源文件句柄
    // iU32 TypeheadID  :字库在资源文件中的ID

    // notes: 常用字不加载进内存

    iU32 Temp = 0;
    iU32 UseTypeheadId = 0;

 #if i51KITG2_DEBUG
    iLog( "G2FontCachDisable start , %x,%x"
    ,ReFile,TypeheadID);
#endif

    	
    for( Temp=0; Temp<MAX_TYPEHEAD_NUMBER; Temp++ )
    {
        if( i51KitG2FontManage[Temp].TypeheadID==TypeheadID && i51KitG2FontManage[Temp].FontFileHandle == ReFile )
        {
            UseTypeheadId = Temp;
            break;
        }
    }
    if( Temp==MAX_TYPEHEAD_NUMBER)
    {
        return iFALSE;
    }

    if(!i51KitG2FontManage[Temp].NormalWordsEntry) return iTRUE;
	
    if(i51AdeOsFree( (void *)i51KitG2FontManage[Temp].NormalWordsEntry))
    {
        i51KitG2FontManage[Temp].NormalWordsEntry = iNULL;

		i51KitG2FontManage[Temp].FontFileHandle = 0;
        return iTRUE;
    }

 #if i51KITG2_DEBUG
    iLog( "G2FontCachDisable end , %x,%x"
    ,ReFile,TypeheadID);
#endif

    return iFALSE;
	
}

iKIT iBOOL i51KitG2FontGetDemension ( iFILE ReFile, iU32 TypeheadID, iU32 WordsNum, iU32 RowWords,
	iU8 WordsDirection , iS16 RowGap, iS16 ColumnGap , iU32 * DrawWidth, iU32 * DrawHeight) 
{
    // author : Otto
    // since : 2011.10.11
    // (C) 2011 , PKIG Tech. Co., Ltd.

    // iFILE ReFile : 字库所在资源文件句柄
    // iU32 TypeheadID:字库在资源文件中的ID
    // iU32 WordsNum: 需要绘制的文字个数
    // iU32 RowWords: 每一行绘制的文字个数
    // iU8 WordsDirection : 绘制文字的方向
    // iS16 RowGap : 绘制文字的行距
    // iS16 ColumnGap : 绘制文字的列距
    // iU32 * DrawWidth : 返回文字绘制区域宽度
    // iU32 * DrawHeight : 返回文字绘制区域高度
    
    // notes: 计算一段文字绘制需要的矩形区域的长和宽
    
    //  字模高度
    iU8  FontModelHeight = 0;	
    //  需要绘制的文字有多少行	
    iU8  DrawRows = 0;
    iU32 TempDrawWidth = 0;
    iU32 TempDrawHeight = 0;	

//    iU32 Temp = 0;

	i51KitG2Font CurrTypehead;
    	
 #if i51KITG2_DEBUG
    iLog( "G2FontGetDemension start , %x,%x,%x,%x,%x,%x,%x,%x,%x"
    ,ReFile,TypeheadID,WordsNum,RowWords,WordsDirection,RowGap,ColumnGap,DrawWidth,DrawHeight);
#endif

	i51KitG2FontSetUse( ReFile, TypeheadID, &CurrTypehead);

    FontModelHeight = CurrTypehead.WordHeight;


    if( RowWords<WordsNum ) RowWords = WordsNum;

    if(RowWords==0) return iFALSE;	
    DrawRows = 	WordsNum/RowWords;

    TempDrawWidth = RowWords * ( FontModelHeight + ColumnGap );
    TempDrawHeight = DrawRows * ( FontModelHeight + RowGap );

    if( WordsDirection<i51KITG2_FONT_WORDS_ROTATE_90)
    { 
        // 字是横排	
        *DrawWidth = TempDrawWidth;
        *DrawHeight = TempDrawHeight;
    }
    else
    {
        // 字是竖排	
        *DrawHeight= TempDrawWidth;
        *DrawWidth = TempDrawHeight;
    }

 #if i51KITG2_DEBUG
    iLog( "G2FontGetDemension end , %x,%x,%x,%x,%x,%x,%x,%x,%x"
    ,ReFile,TypeheadID,WordsNum,RowWords,WordsDirection,RowGap,ColumnGap,DrawWidth,DrawHeight);
#endif
    return iTRUE;
	
}

iKIT iBOOL i51KitG2FontDraw (iFILE Handle, iU32 TypeheadID, void* text ,  iU16 WordsNum, 
	iRECT* DrawRect , iRECT * SrcRect, iU16 DrawDirect, iS16 WordsRowGap,   iS16 WordsColumnGap,  
	iCOLOR fgcolor , iCOLOR bgcolor, iU16 Effect )
{
    // author : Otto
    // since : 2011.10.11
    // (C) 2011 , PKIG Tech. Co., Ltd.

    // iFILE ReFile : 字库所在资源文件句柄
    // iU32 TypeheadID:字库在资源文件中的ID
    // void* text : 字符串指针
    // iU32 WordsNum : 绘制文字的个数，如果为零则全部绘制
    // iRECT* DrawRect : 文字绘制显示区域
    // iRECT* SrcRect : 文字绘制的区域
    // iU32 DrawDirect : 绘制文字的行列和文字的旋转方向
    // iS16 WordsRowGap : 绘制文字的行距
    // iS16 WordsColumnGap : 绘制文字的列距
    // iCOLOR fgcolor : 绘制文字的颜色
    // iCOLOR bgcolor : 绘制文字区域的背景色
    // iU32 Effect : 绘制文字的特效，包括是否需要下划线、是否背景填充

    // notes: 按照特定的规则绘制一段文字到屏幕上

    // 当前绘制文字的机内码
    iU16 WordKey = 0;
    // 当前绘制文字在常用字库中的编号
    iU16 FindWordKey = 0;
    // 临时变量	
    iS32 Temp=0,Temp1=0, Temp2=0;
    // 当前绘制的是第几个文字	
    iS32  CurrWords = 0;
    // 当前绘制文字的字模开始地址
    iU8   * TypeMatrix = iNULL;
    // 取字模	的一个字节存放位置
    iU8    CurrTypeMatrix1 = 0;
    iU8    CurrTypeMatrix2 = 0;

    // 字符串长度
    iU16 StrLength = 0;
    // 字符串指针
    iU8 * Words = (iU8 *)text;
    // 绘制VP时开闭内存保存字符串
    iU32  WordsHandle = 0;

    //  字模宽度	
    iU8  TypeheadWidth = 0;	
    //  字模高度
    iU8  TypeheadHeight = 0;	
    //  与绘制区域相交时需要绘制的字模高度
    iU8  TempTypeheadHeight = 0;	
    //  与绘制区域相交时需要绘制的字模掩码
    iU16  TempTypeheadMask = 0;	
    iU16  TempTypeheadMask1 = 0;	
    //  与绘制区域相交时需要绘制的字模位移位数
    iU8  TempTypeheadMove = 0;	
    // 字符数据长度
    iU8  DoubleTypeheadHeight = 0;	

    // 从文件中读取的一个字符字模缓冲
    iU8  ReadWords[32] = {0};
    // 绘制一个字符需要从文件中读的字模数据长度
    iU32 NeedReadLongth = 0;
    // 实际从文件中读取到的数据长度	
    iU32 LengthReadData = 0;
    // 所有字符在字库文件内的偏移量	
    iU32 AllWordsOffset = 0;
    // 当前需要绘制的文字在字库内的偏移量
    iU32 CurrWordsOffset = 0;
    // 字库文件指针
    iFILE FontFileHandle = iNULL;
    // 文件操作返回值
    iU8   FlagReturn = iFALSE;
    // 绘制文字是否需要下划线
    iU32 NeedUnderLine  = iFALSE;
    // 绘制文字是否需要背景色
    iU32 NeedBKColor  = iFALSE;

    // 绘制文字显示区域
    iS32 DrawWordsStartX = 0;	
    iS32 DrawWordsStartY = 0;	
    iS32 DrawUnderlinesStartX = 0;	
    iS32 DrawUnderlinesStartY = 0;	
    iS32 DrawWidth = 0;	
    iS32 DrawHeight = 0;	
    // 绘制文字的原始区域	
    iS32 SrcStartX = 0;
    iS32 SrcStartY = 0;
    iS32 SrcWidth = 0;
    iS32 SrcHeight = 0;	
    // 当前行绘制的宽度	
    iS32 TempDrawWidth = 0;	
    // 绘制的高度	
    iS32 TempDrawHeight = 0;	
    // 当前行原始的宽度	
    iS32 TempSrcWidth = 0;	
    // 原始的高度	
    iS32 TempSrcHeight = 0;	
    // 一行最多绘制汉字个数	
//    iS32 ChinaMaxColumn = 0;
    // 英文字宽度
    iS32 AscllWidth = 0;

    // 绘制文字当前行的屏幕开始位置	
    iS32 LcdBufferTypeheadPos = 0;
    // 绘制文字两行字模之间增量
    iS16 LcdBufferTypeheadRowAdds = 0;	
    // 绘制文字当前列的屏幕开始位置
    iS32 LcdBuffColumnPos = 0;
    // 绘制国标码文字两列之间的增量
    iS32 LcdBuffColumnChinaAdds = 0;
    // 绘制ASCLL码文字两列之间的增量
    iS32 LcdBuffColumnAscllAdds = 0;
    // 绘制文字当前行的屏幕开始位置
    iS32 LcdBuffRowPos = 0;
    // 绘制文字两行之间的增量
    iS32 LcdBuffRowAdds = 0;
	
    iU16 CurrDrawID;
    iRECT * VPActualRect = iNULL;
    iRECT VPDrawRect;
    // 需要绘制的下划线条数	
    iS16 UnderLineNumber =0; 	
    // 需要绘制的下划线的长度	
//    iS16 UnderLineLength =0; 	
    // 正常情况需要绘制的下划线的长度	
//    iS16 NormalUnderLineLength =0; 	
    // 最后一行需要绘制的下划线的长度	
//    iS32 LastUnderLineLength =0; 	
    // 需要绘制的文字个数
    iU32 DrawWordsNum = 0;
    // 绘制两条下划线LcdBuf增量
    iS32 UnderLinesAdds = 0;
    // 绘制文字行间距	
    iS32 RowGap;
    // 绘制文字字间距	
    iS32 ColumnGap;
    iS32 LcdBufferPos = 0;
    // 绘制的文字中是否包含ASCLL的标记	
//    iU8 DrawIncludeAscll = iFALSE;
    // 字模的每个位相对当前字模行的偏移量	
    iS16 PointAdd0 = 0;
    iS16 PointAdd1 = 0;
    iS16 PointAdd2 = 0;
    iS16 PointAdd3 = 0;
    iS16 PointAdd4 = 0;
    iS16 PointAdd5 = 0;
    iS16 PointAdd6 = 0;
    iS16 PointAdd7 = 0;
    iS16 PointAdd8 = 0;
    iS16 PointAdd9 = 0;
    iS16 PointAdd10 = 0;
    iS16 PointAdd11 = 0;
    iS16 PointAdd12 = 0;
    iS16 PointAdd13 = 0;
    iS16 PointAdd14 = 0;
    iS16 PointAdd15 = 0;

    iU8 TypeheadMask = 0XFF;
	
    // 英文字字模
    iS32  WordsTypehead = iNULL;
    // 当前使用的字库文件在字库管理器中的编号	
//    iU32  UseTypeheadId = 0;
    // 当前使用的字库管理器	
    i51KitG2Font CurrTypehead;
    // 当前使用的字库常用字管理器	
    i51KitFontWord * NormalWordStock = iNULL;
    // 英文字当前字母需要判断换行
    iU8  LineFeed = iTRUE;
    // 一个英文字由多少字母组成
    iU8  LetterNum = 0;

    // 原始区域与绘制区域左边宽度差
    iS32 MulLeftWidth = 0;	
    // 原始区域与绘制区域右边宽度差
    iS32 MulRightWidth  = 0;	
    // 原始区域与绘制区域上边宽度差
    iS32 MulTopWidth  = 0;	
    // 原始区域与绘制区域下边宽度差
    iS32 MulBottomWidth  = 0;	
 #if i51KITG2_DEBUG
    iLog( "G2FontDraw start , %x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x"
    ,Handle,TypeheadID,text,WordsNum,DrawRect,SrcRect,DrawDirect,WordsRowGap,WordsColumnGap,fgcolor,bgcolor,Effect);
#endif

    // 字符串为空直接返回
    if( iNULL == text ) return iTRUE;

    // 文字绘制为空直接返回
    if( iNULL == DrawRect ) return iFALSE;


    // 在虚拟面板上绘制，实际上只是进行一些赋值操作
    if( i51UseVirtualPanel && i51DrawVirtualPanel )
    {
        if( i51CurrentVpManage.TotalVPNumber == 0 ) return iFALSE;

        if( iNULL == i51CurrentVpActivity )	return iFALSE;
		
        // 画板已经画满则直接返回
        if(  i51CurrentVpActivity->VpIsFull ) return iFALSE;

        StrLength = strlen( (iCHAR*)text)+2;
        if(StrLength%2) StrLength++;
        WordsHandle = (iU32)i51AdeOsMalloc( StrLength,  __FILE__ , __LINE__ );
        if( iNULL == WordsHandle ) return iFALSE;
        memset((void *)WordsHandle,0,StrLength);
        i51AdeStdMemcpy16( (void *)WordsHandle, (void *)text, strlen( (iCHAR*)text));

        CurrDrawID = i51CurrentVpActivity->VPCurrentElement;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementType = I51KITG2_ELEMENT_TYPE_WORDS;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementNumber = CurrDrawID;

        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Words.WordsHandle = WordsHandle;

        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Words.DrawRect.X= DrawRect->X;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Words.DrawRect.Y = DrawRect->Y;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Words.DrawRect.Width= DrawRect->Width;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Words.DrawRect.Height = DrawRect->Height;

	if( SrcRect )
	{
	        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Words.SrcRect.X= SrcRect->X;
	        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Words.SrcRect.Y = SrcRect->Y;
	        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Words.SrcRect.Width= SrcRect->Width;
	        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Words.SrcRect.Height = SrcRect->Height;
	}
	else
	{
	        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Words.SrcRect.X= DrawRect->X;
	        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Words.SrcRect.Y = DrawRect->Y;
	        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Words.SrcRect.Width= DrawRect->Width;
	        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Words.SrcRect.Height = DrawRect->Height;
	}

        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Words.fgcolor = fgcolor;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Words.bgcolor = bgcolor;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Words.DrawDirect = DrawDirect;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Words.WordsNum = WordsNum;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Words.Effect = Effect;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Words.WordsRowGap= WordsRowGap;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Words.WordsColumnGap = WordsColumnGap;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Words.ReFileHandle = Handle;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Words.TypeheadID = TypeheadID;

        i51CurrentVpActivity->VPCurrentElement =  CurrDrawID +1;

        if( (CurrDrawID+1) >= i51CurrentVpActivity->VPMaxElement )	i51CurrentVpActivity->VpIsFull = iTRUE;
        return iTRUE;
    }

    DrawWordsStartX = DrawRect->X;
    DrawWordsStartY = DrawRect->Y;
    DrawWidth= DrawRect->Width;
    DrawHeight= DrawRect->Height;

    if( SrcRect )
    {
        SrcStartX = SrcRect->X;
        SrcStartY = SrcRect->Y;
        SrcWidth = SrcRect->Width;
        SrcHeight = SrcRect->Height;
    }
    else
    {
        SrcStartX = DrawRect->X;
        SrcStartY = DrawRect->Y;
        SrcWidth = DrawRect->Width;
        SrcHeight = DrawRect->Height;
    }

    if( DrawWordsStartX<SrcStartX )
    {
        DrawWidth -= SrcStartX-DrawWordsStartX; 
        DrawWordsStartX = SrcStartX;
    }

    if( (DrawWordsStartX+DrawWidth)>(SrcStartX+SrcWidth) )
    {
        DrawWidth = SrcStartX + SrcWidth - DrawWordsStartX;
    }

    if( DrawWordsStartY<SrcStartY)
    {
        DrawHeight -= SrcStartY -DrawWordsStartY;
        DrawWordsStartY = SrcStartY;
    }

    if( (DrawWordsStartY+DrawHeight) > (SrcStartY+SrcHeight) )
    {
        DrawHeight = SrcStartY+SrcHeight-DrawWordsStartY;
    }

    VPDrawRect.X = 0;
    VPDrawRect.Y = 0;
    VPDrawRect.Width= ScreenWidth;
    VPDrawRect.Height= ScreenHeight;
	
    if ( i51UseVirtualPanel )
    {
        VPActualRect = &(i51CurrentVpActivity->VPActualRect);

        VPDrawRect.X= (i51CurrentVpActivity->VPDrawRect).X;
        VPDrawRect.Y= (i51CurrentVpActivity->VPDrawRect).Y;
        VPDrawRect.Width= (i51CurrentVpActivity->VPDrawRect).Width;
        VPDrawRect.Height= (i51CurrentVpActivity->VPDrawRect).Height;

        DrawWordsStartX += VPActualRect->X;
        DrawWordsStartY += VPActualRect->Y;

        SrcStartX += VPActualRect->X;
        SrcStartY += VPActualRect->Y;
       		
    }
	
    if( DrawWordsStartX<VPDrawRect.X )
    {
        DrawWidth -= (VPDrawRect.X - DrawWordsStartX);
        DrawWordsStartX = VPDrawRect.X;
    }
	
    if( DrawWordsStartY<VPDrawRect.Y )
    {
        DrawHeight -= VPDrawRect.Y - DrawWordsStartY;
        DrawWordsStartY= VPDrawRect.Y;
    }

    if( (DrawWordsStartX+DrawWidth)> (VPDrawRect.X+VPDrawRect.Width) )
    {
        DrawWidth =  VPDrawRect.X+VPDrawRect.Width - DrawWordsStartX;
    }
		
    if( (DrawWordsStartY+DrawHeight)> (VPDrawRect.Y+VPDrawRect.Height) )
    {
        DrawHeight =  VPDrawRect.Y+VPDrawRect.Height- DrawWordsStartY;
    }

    if( DrawWordsStartX<0 )
    {
        DrawWidth += DrawWordsStartX;
        DrawWordsStartX = 0;
    }
	
    if( DrawWordsStartY<0 )
    {
        DrawHeight+= DrawWordsStartY;
        DrawWordsStartY = 0;
    }

#if 0	
    for( Temp=0; Temp<MAX_TYPEHEAD_NUMBER; Temp++ )
    {
        if( (i51KitG2FontManage[Temp].TypeheadID==TypeheadID && i51KitG2FontManage[Temp].FontFileHandle == Handle)
            ||i51KitG2FontManage[Temp].FontFileHandle == 0)
        {
            UseTypeheadId = Temp;
            break;
        }
    }
    if( Temp==MAX_TYPEHEAD_NUMBER || i51KitG2FontManage[Temp].FontFileHandle == 0 )
    {
        if( Temp==MAX_TYPEHEAD_NUMBER ) UseTypeheadId = 0;
		iLog("Temp:%d,UseTypeheadId:%d",Temp,UseTypeheadId);
        i51KitG2FontSetUse(Handle, TypeheadID, UseTypeheadId);
    }
#endif

//    CurrTypehead = &i51KitG2FontManage[UseTypeheadId];
	i51KitG2FontSetUse(Handle, TypeheadID, &CurrTypehead);
    TypeheadWidth = CurrTypehead.WordWidth;
    TypeheadHeight = CurrTypehead.WordHeight;
    AllWordsOffset = CurrTypehead.AllWordsOffset;
    FontFileHandle = CurrTypehead.FontFileHandle;
    NormalWordStock  = (i51KitFontWord *)(CurrTypehead.NormalWordsEntry);
		
    if( TypeheadHeight !=12 && TypeheadHeight !=14 && TypeheadHeight !=16 ) return iFALSE;

 //   if( TypeheadHeight>DrawHeight ) return iTRUE;
//    if( TypeheadHeight>DrawWidth ) return iTRUE;
//    if( DrawWordsStartX>(VPDrawRect.X+VPDrawRect.Width-TypeheadHeight)) return iTRUE;
//    if( DrawWordsStartY>(VPDrawRect.Y+VPDrawRect.Height-TypeheadHeight) ) return iTRUE;
    if( 0>=DrawHeight ) return iTRUE;
    if( 0>=DrawWidth ) return iTRUE;
    if( DrawWordsStartX>(VPDrawRect.X+VPDrawRect.Width)) return iTRUE;
    if( DrawWordsStartY>(VPDrawRect.Y+VPDrawRect.Height) ) return iTRUE;

    NeedUnderLine = Effect&i51KITG2_FONT_UNDERLINE;
    NeedBKColor = Effect&i51KITG2_FONT_BKCOLOR;

    RowGap = WordsRowGap;
    ColumnGap = WordsColumnGap;
	
    // 如果行距为零且需要下划线，则增加一行画下划线
    if( RowGap==0 && NeedUnderLine ) RowGap = 1;

    DoubleTypeheadHeight = TypeheadHeight<<1;
	
    NeedReadLongth = TypeheadWidth * TypeheadHeight;

    StrLength = CountWordsNum( (iCHAR *)text);
    if( (WordsNum)<StrLength && WordsNum>0 ) StrLength = WordsNum;
    DrawWordsNum = StrLength;

     LcdBufferPos = DrawWordsStartY*ScreenWidth+DrawWordsStartX;
	
    // 填空区域背景色
    if( NeedBKColor)
    {
        i51KitG2CleanScreen(bgcolor, DrawRect);
    }

    // 计算不同的行列方向的增量
    if( DrawDirect<i51KITG2_FONT_WORDS_ROTATE_90)
    {

        MulLeftWidth = DrawWordsStartX - SrcStartX;
        MulRightWidth = (SrcStartX + SrcWidth) - (DrawWordsStartX + DrawWidth) ;
        MulTopWidth = DrawWordsStartY - SrcStartY;
        MulBottomWidth = (SrcStartY + SrcHeight) - (DrawWordsStartY + DrawHeight) ; 
		
        DrawUnderlinesStartX = DrawWordsStartX;
        DrawUnderlinesStartY = DrawWordsStartY;
        LcdBufferTypeheadRowAdds= ScreenWidth;
        LcdBuffColumnChinaAdds = TypeheadHeight + ColumnGap;
        LcdBuffColumnAscllAdds = (TypeheadHeight>>1) + ((ColumnGap+1)>>1);
        LcdBuffRowAdds = ScreenWidth*(TypeheadHeight+RowGap);
        PointAdd0 = 0;
        PointAdd1 = 1;
        PointAdd2 = 2;
        PointAdd3 = 3;
        PointAdd4 = 4;
        PointAdd5 = 5;
        PointAdd6 = 6;
        PointAdd7 = 7;
        PointAdd8 = 8;
        PointAdd9 = 9;
        PointAdd10 = 10;
        PointAdd11 = 11;
        PointAdd12 = 12;
        PointAdd13 = 13;
        PointAdd14 = 14;
        PointAdd15 = 15;
    }
    else
    {

        MulLeftWidth = DrawWordsStartY - SrcStartY;
        MulRightWidth = (SrcStartY + SrcHeight) - (DrawWordsStartY + DrawHeight) ;
        MulTopWidth = (SrcStartX+SrcWidth) - (DrawWordsStartX+DrawWidth);
        MulBottomWidth = DrawWordsStartX - SrcStartX; 

        DrawUnderlinesStartX = DrawWordsStartX + DrawWidth - TypeheadHeight -1;
        DrawUnderlinesStartY = DrawWordsStartY;
        LcdBufferTypeheadRowAdds= -1;
        LcdBuffColumnChinaAdds = (TypeheadHeight + ColumnGap)*ScreenWidth;
        LcdBuffColumnAscllAdds = ((TypeheadHeight>>1) + ((ColumnGap+1)>>1))*ScreenWidth;
        LcdBuffRowAdds = -(TypeheadHeight+RowGap);
        DrawWordsStartX += DrawWidth - 1;
		
        DrawWidth = DrawWidth+DrawHeight;
        DrawHeight = DrawWidth - DrawHeight;
        DrawWidth = DrawWidth -DrawHeight;
		
        SrcWidth = SrcWidth+SrcHeight;
        SrcHeight = SrcWidth - SrcHeight;
        SrcWidth = SrcWidth -SrcHeight;
		
        PointAdd0 = 0;
        PointAdd1 = ScreenWidth;
        PointAdd2 = 2*ScreenWidth;
        PointAdd3 = 3*ScreenWidth;
        PointAdd4 = 4*ScreenWidth;
        PointAdd5 = 5*ScreenWidth;
        PointAdd6 = 6*ScreenWidth;
        PointAdd7 = 7*ScreenWidth;
        PointAdd8 = 8*ScreenWidth;
        PointAdd9 = 9*ScreenWidth;
        PointAdd10 = 10*ScreenWidth;
        PointAdd11 = 11*ScreenWidth;
        PointAdd12 = 12*ScreenWidth;
        PointAdd13 = 13*ScreenWidth;
        PointAdd14 = 14*ScreenWidth;
        PointAdd15 = 15*ScreenWidth;
    }

	
    // 开始绘制文字
    Temp = MulTopWidth%(TypeheadHeight+RowGap);
    if( RowGap>0 && Temp)
    {
    	if( (TypeheadHeight+RowGap - Temp )<=RowGap )
    	{
    		DrawWordsStartY += TypeheadHeight+RowGap - Temp;
			DrawHeight -= TypeheadHeight+RowGap - Temp;
    	}
    }

    Temp = MulLeftWidth%(TypeheadWidth+ColumnGap);
    if( ColumnGap>0 && Temp)
    {
    	if( (TypeheadWidth+ColumnGap - Temp )<=ColumnGap )
    	{
    		DrawWordsStartX += TypeheadWidth+ColumnGap - Temp;
			DrawWidth -= TypeheadWidth+ColumnGap - Temp;
    	}
    }
	
	if( DrawWidth<=0 || DrawHeight<=0 ) return iTRUE;
	
    LcdBufferTypeheadPos = DrawWordsStartY*ScreenWidth+DrawWordsStartX;
    LcdBuffColumnPos = LcdBufferTypeheadPos;
    LcdBuffRowPos = LcdBufferTypeheadPos;

    UnderLineNumber = 0;
    TempDrawWidth = 0;  
    AscllWidth = (TypeheadHeight>>1);	
	
    switch( TypeheadHeight)	
    {
        case 12 : WordsTypehead = (iU32)ASCLL_WORDS_12; TypeheadMask = 0XF0; break;
        case 14 : WordsTypehead = (iU32)ASCLL_WORDS_14; TypeheadMask = 0XFC;break;
        case 16 : WordsTypehead = (iU32)ASCLL_WORDS_16; TypeheadMask = 0XFF;break;
        default : return iFALSE;
    }
	
//	iLog("Start draw word");
    Temp = 0;	
    for(CurrWords=0;CurrWords<DrawWordsNum; CurrWords++ )
    {
        // 英文字
        if(Words[Temp]<0XA0)
        {
            if( (LineFeed==iFALSE) && ( (Words[Temp]>='a'&&Words[Temp]<='z')
            || (Words[Temp]>='A'&&Words[Temp]<='Z') || (Words[Temp]>='0'&&Words[Temp]<='9') ))
            {
                LineFeed = iTRUE;

            }
				
            if( LineFeed && ( (Words[Temp]>='a'&&Words[Temp]<='z') 
            || (Words[Temp]>='A'&&Words[Temp]<='Z') || (Words[Temp]>='0'&&Words[Temp]<='9') ))
            {
                LineFeed = iFALSE;
                LetterNum = 0;
                while(1)
                {
                    if(((Words[Temp+LetterNum]>='a'&&Words[Temp+LetterNum]<='z')
                   || (Words[Temp+LetterNum]>='A'&&Words[Temp+LetterNum]<='Z') 
                   || (Words[Temp+LetterNum]>='0'&&Words[Temp+LetterNum]<='9'))
                   && (CurrWords+LetterNum<DrawWordsNum))
                    {
                        LetterNum++;
                    }
                    else
                    {
                        break;
                    }
                }
                if( (TempSrcWidth+LetterNum*AscllWidth)>SrcWidth)
                {
                    TempSrcWidth = SrcWidth;
                }
            }

            if( (TempSrcWidth+AscllWidth)>SrcWidth)
            {
                // 换行
                if(Words[Temp]==' '&&Words[Temp+1]!=' ') 
                {
                    Temp++;
                    continue;
                }
			   
                if( TempSrcHeight>MulTopWidth)
                {
                    LcdBufferTypeheadPos = LcdBuffRowPos+LcdBuffRowAdds;
                    LcdBuffColumnPos = LcdBufferTypeheadPos;
                    LcdBuffRowPos = LcdBufferTypeheadPos;
                    TempDrawHeight += TypeheadHeight + RowGap; 
                    // 需要绘制的文字在绘制区域下方
                    if( TempDrawHeight>=DrawHeight ) break;					
                    UnderLineNumber++;				
                }
                else if( (TempSrcHeight+TypeheadHeight)>MulTopWidth )
                {
                    TempDrawHeight = TypeheadHeight -( MulTopWidth - TempSrcHeight ) + RowGap;
                    LcdBufferTypeheadPos = LcdBuffRowPos+LcdBuffRowAdds*(TempDrawHeight)/(TypeheadHeight+RowGap);
                    LcdBuffColumnPos = LcdBufferTypeheadPos;
                    LcdBuffRowPos = LcdBufferTypeheadPos;
                    // 需要绘制的文字在绘制区域下方
                    if( TempDrawHeight>=DrawHeight ) break;					
                    if( DrawDirect<i51KITG2_FONT_WORDS_ROTATE_90)
                    {
                        DrawUnderlinesStartY -= MulTopWidth - TempSrcHeight;
                    }
                    else
                    {
                        DrawUnderlinesStartX += MulTopWidth - TempSrcHeight;
                    }
                    UnderLineNumber++;				
                }
                TempSrcHeight += TypeheadHeight + RowGap;
                TempSrcWidth = 0;
                TempDrawWidth = 0;			   
            }

            if( (TempSrcWidth+AscllWidth)<=MulLeftWidth) 
            {
                // 需要绘制的文字在绘制区域左边
                TempSrcWidth += AscllWidth;
                Temp++;
                continue; 				
            }
			
            if( (TempSrcHeight+TypeheadHeight)<=MulTopWidth )
            {
                // 需要绘制的文字在绘制区域上方
                TempSrcWidth += AscllWidth;
                Temp++;
                continue;
            }

            if(TempDrawWidth>=DrawWidth)
            {
                // 需要绘制的文字在绘制区域右边
                TempSrcWidth += AscllWidth;
                Temp++;
                continue;
            }
			
            if( (TempSrcWidth>=MulLeftWidth)&&((TempDrawWidth+AscllWidth)<DrawWidth) 
            &&(TempSrcHeight>=MulTopWidth)&& ((TempDrawHeight+TypeheadHeight)<DrawHeight))
            {
                // 需要绘制的文字在绘制区域之中
                TempSrcWidth += AscllWidth;
                TempDrawWidth += AscllWidth;
                Temp2 = (Words[Temp]-0X20);			
                TypeMatrix = (iU8 *)(WordsTypehead+Temp2*TypeheadHeight);
                for(Temp1=0; Temp1<TypeheadHeight; Temp1++)
                {
                    CurrTypeMatrix1 = TypeMatrix[Temp1];
                    if(CurrTypeMatrix1&(0X80)) LcdBuffer[LcdBufferTypeheadPos+PointAdd0] = fgcolor;
                    if(CurrTypeMatrix1&(0X40)) LcdBuffer[LcdBufferTypeheadPos+PointAdd1] = fgcolor;
                    if(CurrTypeMatrix1&(0X20)) LcdBuffer[LcdBufferTypeheadPos+PointAdd2] = fgcolor;
                    if(CurrTypeMatrix1&(0X10)) LcdBuffer[LcdBufferTypeheadPos+PointAdd3] = fgcolor;
                    if(CurrTypeMatrix1&(0X08)) LcdBuffer[LcdBufferTypeheadPos+PointAdd4] = fgcolor;
                    if(CurrTypeMatrix1&(0X04)) LcdBuffer[LcdBufferTypeheadPos+PointAdd5] = fgcolor;
                    if(CurrTypeMatrix1&(0X02)) LcdBuffer[LcdBufferTypeheadPos+PointAdd6] = fgcolor;
                    if(CurrTypeMatrix1&(0X01)) LcdBuffer[LcdBufferTypeheadPos+PointAdd7] = fgcolor;
    
                    LcdBufferTypeheadPos += LcdBufferTypeheadRowAdds;
                }
                Temp ++;
                LcdBufferTypeheadPos = LcdBuffColumnPos + LcdBuffColumnAscllAdds;
                LcdBuffColumnPos = LcdBufferTypeheadPos;
                continue;			
            }
            else
            {
                // 需要绘制的文字与绘制区域边界相交
                iS32 WidthMul = AscllWidth;
                TempTypeheadHeight = TypeheadHeight;
                Temp1=0;
                if( TempDrawHeight==0 && TempSrcHeight<MulTopWidth)
                {
                    // 上相交
                    Temp1 = MulTopWidth - TempSrcHeight;
                }
                if( (TempDrawHeight+TypeheadHeight)>DrawHeight)
                {
                    // 下相交
                    TempTypeheadHeight = DrawHeight - TempDrawHeight + Temp1;
                    if(TempTypeheadHeight>TypeheadHeight) 	 TempTypeheadHeight = TypeheadHeight;			
                }

                TempTypeheadMove = 0;
                TempTypeheadMask = 0XFF;
                if( TempDrawWidth==0 && TempSrcWidth<MulLeftWidth)
                {
                    // 左相交
                    TempTypeheadMove = MulLeftWidth - TempSrcWidth;
//                    TempDrawWidth += TempSrcWidth + AscllWidth- MulLeftWidth;
                    WidthMul = TempSrcWidth + AscllWidth- MulLeftWidth;
                }
                if( (TempDrawWidth+AscllWidth)>DrawWidth)
                {
                    // 右相交
                    TempTypeheadMask = (TempTypeheadMask)>>(8 - DrawWidth + TempDrawWidth);
                    TempTypeheadMask = (TempTypeheadMask)<<(8 - DrawWidth + TempDrawWidth);
//                    TempDrawWidth += AscllWidth;
                }
//                else
                {
                    TempDrawWidth += WidthMul;
                }
                
                TempSrcWidth += AscllWidth;
                Temp2 = (Words[Temp]-0X20);			
                TypeMatrix = (iU8 *)(WordsTypehead+Temp2*TypeheadHeight);
                for( ;Temp1<TempTypeheadHeight; Temp1++)
                {
                    CurrTypeMatrix1 = ((TypeMatrix[Temp1]<<TempTypeheadMove)&TempTypeheadMask);
                    if(CurrTypeMatrix1&(0X80)) LcdBuffer[LcdBufferTypeheadPos+PointAdd0] = fgcolor;
                    if(CurrTypeMatrix1&(0X40)) LcdBuffer[LcdBufferTypeheadPos+PointAdd1] = fgcolor;
                    if(CurrTypeMatrix1&(0X20)) LcdBuffer[LcdBufferTypeheadPos+PointAdd2] = fgcolor;
                    if(CurrTypeMatrix1&(0X10)) LcdBuffer[LcdBufferTypeheadPos+PointAdd3] = fgcolor;
                    if(CurrTypeMatrix1&(0X08)) LcdBuffer[LcdBufferTypeheadPos+PointAdd4] = fgcolor;
                    if(CurrTypeMatrix1&(0X04)) LcdBuffer[LcdBufferTypeheadPos+PointAdd5] = fgcolor;
                    if(CurrTypeMatrix1&(0X02)) LcdBuffer[LcdBufferTypeheadPos+PointAdd6] = fgcolor;
                    if(CurrTypeMatrix1&(0X01)) LcdBuffer[LcdBufferTypeheadPos+PointAdd7] = fgcolor;
    
                    LcdBufferTypeheadPos += LcdBufferTypeheadRowAdds;
                }
                Temp ++;
                LcdBufferTypeheadPos = LcdBuffColumnPos + LcdBuffColumnAscllAdds*(AscllWidth-TempTypeheadMove)/AscllWidth;
                LcdBuffColumnPos = LcdBufferTypeheadPos;
                continue;			
            }
        }
		
        LineFeed = iTRUE;


        if( (TempSrcWidth+TypeheadHeight)>SrcWidth)
        {
            // 换行
            if( TempSrcHeight>MulTopWidth)
            {
                LcdBufferTypeheadPos = LcdBuffRowPos+LcdBuffRowAdds;
                LcdBuffColumnPos = LcdBufferTypeheadPos;
                LcdBuffRowPos = LcdBufferTypeheadPos;
                TempDrawHeight += TypeheadHeight + RowGap; 
                // 需要绘制的文字在绘制区域下方
                if( TempDrawHeight>=DrawHeight ) break;					
                UnderLineNumber++;				
            }
            else if( (TempSrcHeight+TypeheadHeight)>MulTopWidth )
            {
                TempDrawHeight = TypeheadHeight -( MulTopWidth - TempSrcHeight ) + RowGap;
                LcdBufferTypeheadPos = LcdBuffRowPos+LcdBuffRowAdds*(TempDrawHeight)/(TypeheadHeight+RowGap);
                LcdBuffColumnPos = LcdBufferTypeheadPos;
                LcdBuffRowPos = LcdBufferTypeheadPos;
                // 需要绘制的文字在绘制区域下方
                if( TempDrawHeight>=DrawHeight ) break;					
                if( DrawDirect<i51KITG2_FONT_WORDS_ROTATE_90)
                {
                    DrawUnderlinesStartY -= MulTopWidth - TempSrcHeight;
                }
                else
                {
                    DrawUnderlinesStartX += MulTopWidth - TempSrcHeight;
                }
                UnderLineNumber++;				
            }
            TempSrcHeight += TypeheadHeight + RowGap;
            TempSrcWidth = 0;
            TempDrawWidth = 0;			   
        }

        if( (TempSrcWidth+TypeheadHeight)<=MulLeftWidth) 
        {
            // 需要绘制的文字在绘制区域左边
            TempSrcWidth += TypeheadHeight+ColumnGap;
            Temp+=2;
            continue; 				
        }
		
		if( (TempSrcHeight+TypeheadHeight)<=MulTopWidth )
        {
            // 需要绘制的文字在绘制区域上方
            TempSrcWidth += TypeheadHeight+ColumnGap;
            Temp+=2;
            continue;
        }

        if(TempDrawWidth>=DrawWidth)
        {
            // 需要绘制的文字在绘制区域右边
            TempSrcWidth += TypeheadHeight+ColumnGap;
            Temp+=2;
            continue;
        }

        // 查找当前绘制的文字字模是否在内存中
        WordKey = (Words[Temp]<<8)|Words[Temp+1];
        if(i51KitG2FontFindKey(WordKey, &FindWordKey, &CurrTypehead))		
        {
            TypeMatrix = NormalWordStock[FindWordKey].TypeMatrix;
        }
        else
        {
            // 从文件中读取当前文字字模
            CurrWordsOffset = AllWordsOffset + ((Words[Temp]-0XA1)*94 + (Words[Temp+1]-0XA1))*NeedReadLongth;
            FlagReturn = i51AdeOsFileSetSeek( FontFileHandle, CurrWordsOffset, i51_OS_FL_SEEK_HEAD );
            if( iFALSE == FlagReturn )
            {
                LcdBufferTypeheadPos = LcdBuffColumnPos + LcdBuffColumnAscllAdds;
                LcdBuffColumnPos = LcdBufferTypeheadPos;
                continue;
            }

            FlagReturn = i51AdeOsFileRead( FontFileHandle, ReadWords, NeedReadLongth, &LengthReadData);   
            if( (iFALSE == FlagReturn) || (LengthReadData!=NeedReadLongth) ) 
            {
                LcdBufferTypeheadPos = LcdBuffColumnPos + LcdBuffColumnAscllAdds;
                LcdBuffColumnPos = LcdBufferTypeheadPos;
                continue;
            }

            TypeMatrix = ReadWords;
        }
		
        if( (TempSrcWidth>=MulLeftWidth)&&((TempDrawWidth+TypeheadHeight)<DrawWidth) 
        &&(TempSrcHeight>=MulTopWidth)&& ((TempDrawHeight+TypeheadHeight)<DrawHeight))
        {
            // 绘制一个文字
            for(Temp1=0; Temp1<DoubleTypeheadHeight; )
            {
                CurrTypeMatrix1 = TypeMatrix[Temp1++];
                CurrTypeMatrix2 = TypeMatrix[Temp1++]&TypeheadMask;
                if(CurrTypeMatrix1&(0X80)) LcdBuffer[LcdBufferTypeheadPos+PointAdd0] = fgcolor;
                if(CurrTypeMatrix1&(0X40)) LcdBuffer[LcdBufferTypeheadPos+PointAdd1] = fgcolor;
                if(CurrTypeMatrix1&(0X20)) LcdBuffer[LcdBufferTypeheadPos+PointAdd2] = fgcolor;
                if(CurrTypeMatrix1&(0X10)) LcdBuffer[LcdBufferTypeheadPos+PointAdd3] = fgcolor;
                if(CurrTypeMatrix1&(0X08)) LcdBuffer[LcdBufferTypeheadPos+PointAdd4] = fgcolor;
                if(CurrTypeMatrix1&(0X04)) LcdBuffer[LcdBufferTypeheadPos+PointAdd5] = fgcolor;
                if(CurrTypeMatrix1&(0X02)) LcdBuffer[LcdBufferTypeheadPos+PointAdd6] = fgcolor;
                if(CurrTypeMatrix1&(0X01)) LcdBuffer[LcdBufferTypeheadPos+PointAdd7] = fgcolor;
                if(CurrTypeMatrix2&(0X80)) LcdBuffer[LcdBufferTypeheadPos+PointAdd8] = fgcolor;
                if(CurrTypeMatrix2&(0X40)) LcdBuffer[LcdBufferTypeheadPos+PointAdd9] = fgcolor;
                if(CurrTypeMatrix2&(0X20)) LcdBuffer[LcdBufferTypeheadPos+PointAdd10] = fgcolor;
                if(CurrTypeMatrix2&(0X10)) LcdBuffer[LcdBufferTypeheadPos+PointAdd11] = fgcolor;
                if(CurrTypeMatrix2&(0X08)) LcdBuffer[LcdBufferTypeheadPos+PointAdd12] = fgcolor;
                if(CurrTypeMatrix2&(0X04)) LcdBuffer[LcdBufferTypeheadPos+PointAdd13] = fgcolor;
                if(CurrTypeMatrix2&(0X02)) LcdBuffer[LcdBufferTypeheadPos+PointAdd14] = fgcolor;
                if(CurrTypeMatrix2&(0X01)) LcdBuffer[LcdBufferTypeheadPos+PointAdd15] = fgcolor;
    
                LcdBufferTypeheadPos += LcdBufferTypeheadRowAdds;
    			
            }        

            Temp += 2;
            TempDrawWidth += ColumnGap + TypeheadHeight;
            TempSrcWidth += ColumnGap + TypeheadHeight;
            LcdBufferTypeheadPos = LcdBuffColumnPos + LcdBuffColumnChinaAdds;
            LcdBuffColumnPos = LcdBufferTypeheadPos;
			
        }		
        else
        {
            iS32 WidthMul = TypeheadHeight;
            // 需要绘制的文字与绘制区域边界相交
            TempTypeheadHeight = TypeheadHeight<<1;
            Temp1=0;
            if( TempDrawHeight==0 && TempSrcHeight<MulTopWidth)
            {
                // 上相交
                Temp1 = ((MulTopWidth - TempSrcHeight)<<1);
            }
            if( (TempDrawHeight+TypeheadHeight)>DrawHeight)
            {
                // 下相交
                TempTypeheadHeight = ((DrawHeight - TempDrawHeight)<<1) + Temp1;
                if( TempTypeheadHeight>(TypeheadHeight<<1)) TempTypeheadHeight = TypeheadHeight<<1;
            }

            TempTypeheadMove = 0;
            TempTypeheadMask = 0XFFFF;
            if( TempDrawWidth==0 && TempSrcWidth<MulLeftWidth)
            {
                // 左相交
                TempTypeheadMove = MulLeftWidth - TempSrcWidth;
//                TempDrawWidth += TempSrcWidth + TypeheadHeight- MulLeftWidth+ColumnGap;
                WidthMul = TempSrcWidth + TypeheadHeight- MulLeftWidth+ColumnGap;
            }
            if( (TempDrawWidth+TypeheadHeight)>DrawWidth)
            {
                // 右相交
                TempTypeheadMask = (TempTypeheadMask)>>(16 - DrawWidth + TempDrawWidth);
                TempTypeheadMask = (TempTypeheadMask)<<(16 - DrawWidth + TempDrawWidth);
            }
            {
                TempDrawWidth += WidthMul;
            }
			
            // 绘制一个文字
            for(; Temp1<TempTypeheadHeight; )
            {
                CurrTypeMatrix1 = TypeMatrix[Temp1++];
                CurrTypeMatrix2 = TypeMatrix[Temp1++]&TypeheadMask;
                TempTypeheadMask1 = ((( (CurrTypeMatrix1<<8)|CurrTypeMatrix2)<<TempTypeheadMove)&TempTypeheadMask);
                CurrTypeMatrix1 = (TempTypeheadMask1>>8)&0XFF;
                CurrTypeMatrix2 = (TempTypeheadMask1)&TypeheadMask;
                
                if(CurrTypeMatrix1&(0X80)) LcdBuffer[LcdBufferTypeheadPos+PointAdd0] = fgcolor;
                if(CurrTypeMatrix1&(0X40)) LcdBuffer[LcdBufferTypeheadPos+PointAdd1] = fgcolor;
                if(CurrTypeMatrix1&(0X20)) LcdBuffer[LcdBufferTypeheadPos+PointAdd2] = fgcolor;
                if(CurrTypeMatrix1&(0X10)) LcdBuffer[LcdBufferTypeheadPos+PointAdd3] = fgcolor;
                if(CurrTypeMatrix1&(0X08)) LcdBuffer[LcdBufferTypeheadPos+PointAdd4] = fgcolor;
                if(CurrTypeMatrix1&(0X04)) LcdBuffer[LcdBufferTypeheadPos+PointAdd5] = fgcolor;
                if(CurrTypeMatrix1&(0X02)) LcdBuffer[LcdBufferTypeheadPos+PointAdd6] = fgcolor;
                if(CurrTypeMatrix1&(0X01)) LcdBuffer[LcdBufferTypeheadPos+PointAdd7] = fgcolor;
                if(CurrTypeMatrix2&(0X80)) LcdBuffer[LcdBufferTypeheadPos+PointAdd8] = fgcolor;
                if(CurrTypeMatrix2&(0X40)) LcdBuffer[LcdBufferTypeheadPos+PointAdd9] = fgcolor;
                if(CurrTypeMatrix2&(0X20)) LcdBuffer[LcdBufferTypeheadPos+PointAdd10] = fgcolor;
                if(CurrTypeMatrix2&(0X10)) LcdBuffer[LcdBufferTypeheadPos+PointAdd11] = fgcolor;
                if(CurrTypeMatrix2&(0X08)) LcdBuffer[LcdBufferTypeheadPos+PointAdd12] = fgcolor;
                if(CurrTypeMatrix2&(0X04)) LcdBuffer[LcdBufferTypeheadPos+PointAdd13] = fgcolor;
                if(CurrTypeMatrix2&(0X02)) LcdBuffer[LcdBufferTypeheadPos+PointAdd14] = fgcolor;
                if(CurrTypeMatrix2&(0X01)) LcdBuffer[LcdBufferTypeheadPos+PointAdd15] = fgcolor;
    
                LcdBufferTypeheadPos += LcdBufferTypeheadRowAdds;
    			
            }      

            Temp += 2;
            TempSrcWidth += TypeheadHeight+ColumnGap;
            LcdBufferTypeheadPos = LcdBuffColumnPos + LcdBuffColumnChinaAdds*(TypeheadHeight-TempTypeheadMove)/TypeheadHeight;;
            LcdBuffColumnPos = LcdBufferTypeheadPos;
			
        }


    }            

    // 绘制下划线，如果需要
    if( NeedUnderLine )
    {
        if(DrawHeight<TypeheadHeight)
        {
            if( DrawDirect<i51KITG2_FONT_WORDS_ROTATE_90)
            {
                DrawUnderlinesStartY -=TypeheadHeight - ( MulTopWidth %(TypeheadHeight+RowGap));
            }
            else
            {
                DrawUnderlinesStartX +=TypeheadHeight - ( MulTopWidth %(TypeheadHeight+RowGap));
            }
        }
        if( (TempDrawHeight+TypeheadHeight+1)<DrawHeight&&(CurrWords==DrawWordsNum))
        {
            UnderLineNumber++;
        }
        // 计算不同的行列方向的增量
        if(DrawDirect<i51KITG2_FONT_WORDS_ROTATE_90)
        {
            UnderLinesAdds = (TypeheadHeight+RowGap)*ScreenWidth;
            LcdBufferPos = (DrawUnderlinesStartY+TypeheadHeight)*ScreenWidth + DrawUnderlinesStartX;
            for( Temp1=0; Temp1<DrawWidth; Temp1++ )
            {
                OneRowColorBuf[Temp1] = fgcolor;
            }
			
            for( Temp=0; Temp<UnderLineNumber; Temp++ )
            {
                i51AdeStdMemcpy16(&LcdBuffer[LcdBufferPos], OneRowColorBuf,(DrawWidth<<1));
                LcdBufferPos += UnderLinesAdds;
           }
        }
        else
        {
            UnderLinesAdds = -(TypeheadHeight+RowGap);
            LcdBufferPos = (DrawUnderlinesStartY)*ScreenWidth + DrawUnderlinesStartX;
            for( Temp=0; Temp<UnderLineNumber; Temp++ )
            {
                Temp1= LcdBufferPos;
                for( Temp2=0; Temp2<DrawWidth; Temp2++ )
                {
                    LcdBuffer[Temp1] = fgcolor;
                    Temp1 +=ScreenWidth;
                }
                LcdBufferPos +=  UnderLinesAdds;
            }
        }

    }

 #if i51KITG2_DEBUG
    iLog( "G2FontDraw end , %x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x"
    ,Handle,TypeheadID,text,WordsNum,DrawRect,SrcRect,DrawDirect,WordsRowGap,WordsColumnGap,fgcolor,bgcolor,Effect);
#endif
    return iTRUE;

}

#if DEBUG_I51KITG2
iKIT iBOOL i51KitG2FontDrawEx1
(
  void * TypeheadWords, iU8 * TypeheadData, iU32 TypeheadNum, 
  iU32 EnglishWidth, iU32 EnglishHeight, iU32 ChineseWidth, iU32 ChineseHeight,
  iS16 WordsRowGap,  iS16 WordsColumnGap, iCOLOR Color, iRECT * DesRect
)
{
    // author : Otto
    // since : 2011.10.19
    // (C) 2011 , PKIG Tech. Co., Ltd.

    // void * TypeheadWords : 字符串指针
    // iU16 * TypeheadData : 字模数据
    // iU32 TypeheadNum : 需要绘制的字模个数
    // iU32 EnglishWidth : 英文字字模宽度
    // iU32 EnglishHeight : 英文字字模高度
    // iU32 ChineseWidth : 中文字字模宽度
    // iU32 ChineseHeight : 中文字字模高度
    // iS16 WordsRowGap : 字行距
    // iS16 WordsColumnGap : 字间距    
    // iCOLOR Color : 绘制颜色
    // iRECT * DesRect : 绘制区域
 
    // notes: 绘制字模到屏幕上，绘制的方向是正常方向
    iU8 TempTypehead1 = 0;
    iU8 TempTypehead2 = 0;
    iU16 ScreenWidth = 0;
    iU16 ScreenHeight= 0;
    iU16 * ScreenBuffer = iNULL;
    iU32 * LcdInfoArr = iNULL;
    iU32 CurrDrawWord = 0;
    iU32 CurrTypeheadPos = 0;
    iU32 CurrWordsPos = 0;
    iU32 CurrScreenBufferPos = 0;
    iU32 OldScreenBufferPos = 0;
    iS32 Temp = 0;
    iS16 StartX = 0, StartY = 0, EndX = 0, EndY = 0;	
    iS16 CurrX = 0, CurrY = 0;		
    // 字符串指针
    iU8 * Words = (iU8 *)TypeheadWords;
    iU32 CurrWidth = 0;
    iU32 CurrHeight = 0;	
    iS32 StrLength = 0;
    iU16 WordsNum = 0;
    iU16 DrawNum = 0;
    iU16 LetterNum = 0; 	

    iU8 Ch = 0;

    // 统计需要绘制的字符个数	
    StrLength = strlen((iCHAR*)Words);
    for( Temp=0; Temp<StrLength; Temp++ )
    {
        WordsNum++;
        Ch = Words[Temp];
        if( Ch>0XA0) Temp++;
    }

    if( TypeheadNum==0 ) DrawNum = WordsNum;
    else if( TypeheadNum>WordsNum ) DrawNum = WordsNum;
    else DrawNum = TypeheadNum;
	
    if( i51AdeMmiGetScreenScale ( &ScreenWidth , &ScreenHeight ) == 0 )
    {
        return iFALSE;
    }

    LcdInfoArr =  i51AdeMmiGetPanel();
    if( iNULL == LcdInfoArr )
    {
        return iFALSE;
    }
  
    ScreenBuffer = (iU16 *) LcdInfoArr[1] ;

    if( DesRect==iNULL ) 
    {
        StartX = 0;
        StartY = 0;	
        EndX = ScreenWidth;
        EndY = ScreenHeight;
    }
    else
    {
        StartX = DesRect->X;
        StartY = DesRect->Y;
        EndX = DesRect->Width + StartX;
        EndY = DesRect->Height + StartY;
    }

    if(StartX<0) StartX = 0;
    if(StartY<0) StartY = 0;
    if(EndX>ScreenWidth) EndX = ScreenWidth;
    if(EndY>ScreenHeight) EndY = ScreenHeight;

    if( StartX>= ScreenWidth ) return iTRUE;
    if( StartY>= ScreenHeight ) return iTRUE;
    if( EndX<0) return iTRUE;
    if( EndY<0) return iTRUE;

    CurrX = StartX;
    CurrY = StartY;
    CurrScreenBufferPos = CurrY*ScreenWidth + CurrX;
    OldScreenBufferPos = CurrScreenBufferPos;
	
    for( CurrDrawWord=0; CurrDrawWord<DrawNum; CurrDrawWord++)
    {
        LetterNum = 1;			
        if( Words[CurrWordsPos]< 0XA0 )
        {
            CurrWidth = EnglishWidth;
            CurrHeight = EnglishHeight;

            // 统计英文单词中字母个数
            while(1)
            {
                if((Words[CurrWordsPos+LetterNum]>='a'&&Words[CurrWordsPos+LetterNum]<='z')
               || (Words[CurrWordsPos+LetterNum]>='A'&&Words[CurrWordsPos+LetterNum]<='Z') 
               || (Words[CurrWordsPos+LetterNum]>='0'&&Words[CurrWordsPos+LetterNum]<='9') )
                {
                    LetterNum++;
                }
                else
                {
                    break;
                }
            }
            CurrWordsPos += 1;
        }
        else
        {
             CurrWordsPos += 2;
        	CurrWidth = ChineseWidth;
             CurrHeight = ChineseHeight;
        }

        // 判断是否换行
        if( CurrX+LetterNum*CurrWidth >= EndX )
        {
            if( Words[CurrWordsPos-1]< 0XA0 )
            {
                if(Words[CurrWordsPos-1]==' ' && Words[CurrWordsPos]!=' ')
                {
                    CurrWordsPos ++;
                    CurrDrawWord ++;
                    CurrTypeheadPos += CurrHeight;		
                    if(CurrDrawWord>=DrawNum) return iTRUE;
                }
            }
            if(ChineseHeight>0)
            {
                CurrY += ChineseHeight+WordsRowGap;
                if( CurrY+ChineseHeight>=EndY ) return iTRUE;
            }
            else 
            {
                CurrY += EnglishHeight+WordsRowGap;
                if( CurrY+EnglishHeight>=EndY ) return iTRUE;
            }

            CurrX = StartX;				
            CurrScreenBufferPos = CurrY*ScreenWidth + CurrX;
            OldScreenBufferPos = CurrScreenBufferPos;
        }
			
        for( Temp =0; Temp < CurrHeight; Temp++ ) 
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
            if( CurrWidth<9 ) goto LOOP;			
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

        if(CurrWidth==ChineseWidth)
        {
            CurrScreenBufferPos = OldScreenBufferPos + CurrWidth + WordsColumnGap;
            CurrX += CurrWidth+WordsColumnGap;
        }
        else
        {
            CurrScreenBufferPos = OldScreenBufferPos + CurrWidth;
            CurrX += CurrWidth;
        }
        OldScreenBufferPos = CurrScreenBufferPos;
		
    }

    return iTRUE;
	
}

iKIT iBOOL i51KitG2FontDrawEx2
(
  void * TypeheadWords, iU8 * TypeheadData, iU32 TypeheadNum, 
  iU32 EnglishWidth, iU32 EnglishHeight, iU32 ChineseWidth, iU32 ChineseHeight,
  iS16 WordsRowGap,  iS16 WordsColumnGap, iCOLOR Color, iRECT * DesRect
)
{
    // author : Otto
    // since : 2011.10.19
    // (C) 2011 , PKIG Tech. Co., Ltd.

    // void * TypeheadWords : 字符串指针
    // iU16 * TypeheadData : 字模数据
    // iU32 TypeheadNum : 需要绘制的字模个数
    // iU32 EnglishWidth : 英文字字模宽度
    // iU32 EnglishHeight : 英文字字模高度
    // iU32 ChineseWidth : 中文字字模宽度
    // iU32 ChineseHeight : 中文字字模高度
    // iS16 WordsRowGap : 字行距
    // iS16 WordsColumnGap : 字间距    
    // iCOLOR Color : 绘制颜色
    // iRECT * DesRect : 绘制区域
 
    // notes: 绘制字模到屏幕上，绘制的方向是选择90°
    iU8 TempTypehead1 = 0;
    iU8 TempTypehead2 = 0;
    iU16 ScreenWidth = 0;
    iU16 ScreenHeight= 0;
    iU16 * ScreenBuffer = iNULL;
    iU32 * LcdInfoArr = iNULL;
    iU32 CurrDrawWord = 0;
    iU32 CurrTypeheadPos = 0;
    iU32 CurrWordsPos = 0;
    iU32 CurrScreenBufferPos = 0;
    iU32 OldScreenBufferPos = 0;
    iS32 Temp = 0;
    iS16 StartX = 0, StartY = 0, EndX = 0, EndY = 0;	
    iS16 CurrX = 0, CurrY = 0;		
    // 字符串指针
    iU8 * Words = (iU8 *)TypeheadWords;
    iU32 CurrWidth = 0;
    iU32 CurrHeight = 0;	
    iS32 StrLength = 0;
    iU16 WordsNum = 0;
    iU16 DrawNum = 0;
    iU16 LetterNum = 0; 	

    iU8 Ch = 0;

    // 统计需要绘制的字符个数	
    StrLength = strlen((iCHAR*)Words);
    for( Temp=0; Temp<StrLength; Temp++ )
    {
        WordsNum++;
        Ch = Words[Temp];
        if( Ch>0XA0) Temp++;
    }

    if( TypeheadNum==0 ) DrawNum = WordsNum;
    else if( TypeheadNum>WordsNum ) DrawNum = WordsNum;
    else DrawNum = TypeheadNum;
	
    if( i51AdeMmiGetScreenScale ( &ScreenWidth , &ScreenHeight ) == 0 )
    {
        return iFALSE;
    }

    LcdInfoArr =  i51AdeMmiGetPanel();
    if( iNULL == LcdInfoArr )
    {
        return iFALSE;
    }
  
    ScreenBuffer = (iU16 *) LcdInfoArr[1] ;

    if( DesRect==iNULL ) 
    {
        StartX = 0;
        StartY = 0;	
        EndX = ScreenWidth;
        EndY = ScreenHeight;
    }
    else
    {
        StartX = DesRect->X;
        StartY = DesRect->Y;
        EndX = DesRect->Width + StartX;
        EndY = DesRect->Height + StartY;
    }

    if(StartX<0) StartX = 0;
    if(StartY<0) StartY = 0;
    if(EndX>ScreenWidth) EndX = ScreenWidth;
    if(EndY>ScreenHeight) EndY = ScreenHeight;

    if( StartX>= ScreenWidth ) return iTRUE;
    if( StartY>= ScreenHeight ) return iTRUE;
    if( EndX<0) return iTRUE;
    if( EndY<0) return iTRUE;

    StartX += EndX;
    EndX = StartX - EndX;
    StartX = StartX - EndX;	
		
    CurrX = StartX;
    CurrY = StartY;
    CurrScreenBufferPos = CurrY*ScreenWidth + CurrX;
    OldScreenBufferPos = CurrScreenBufferPos;
	
    for( CurrDrawWord=0; CurrDrawWord<DrawNum; CurrDrawWord++)
    {
        LetterNum = 1;			
        if( Words[CurrWordsPos]< 0XA0 )
        {
            CurrWidth = EnglishWidth;
            CurrHeight = EnglishHeight;

            // 统计英文单词中字母个数
            while(1)
            {
                if((Words[CurrWordsPos+LetterNum]>='a'&&Words[CurrWordsPos+LetterNum]<='z')
               || (Words[CurrWordsPos+LetterNum]>='A'&&Words[CurrWordsPos+LetterNum]<='Z') 
               || (Words[CurrWordsPos+LetterNum]>='0'&&Words[CurrWordsPos+LetterNum]<='9') )
                {
                    LetterNum++;
                }
                else
                {
                    break;
                }
            }
            CurrWordsPos += 1;
        }
        else
        {
             CurrWordsPos += 2;
        	CurrWidth = ChineseWidth;
             CurrHeight = ChineseHeight;
        }

        // 判断是否换行
        if( CurrY+LetterNum*CurrWidth >= EndY )
        {
            if( Words[CurrWordsPos-1]< 0XA0 )
            {
                if(Words[CurrWordsPos-1]==' ' && Words[CurrWordsPos]!=' ')
                {
                    CurrWordsPos ++;
                    CurrDrawWord ++;
                    CurrTypeheadPos += CurrHeight;		
                    if(CurrDrawWord>=DrawNum) return iTRUE;
                }
            }
            if(ChineseHeight>0)
            {
                CurrX -= ChineseHeight+WordsRowGap;
                if( CurrX-ChineseHeight<EndX ) return iTRUE;
            }
            else 
            {
                CurrX -= EnglishHeight+WordsRowGap;
                if( CurrX-EnglishHeight<EndX) return iTRUE;
            }

            CurrY = StartY;				
            CurrScreenBufferPos = CurrY*ScreenWidth + CurrX;
            OldScreenBufferPos = CurrScreenBufferPos;
        }
			
        for( Temp =0; Temp < CurrHeight; Temp++ ) 
        {
            TempTypehead1 = TypeheadData[CurrTypeheadPos++];
            if(TempTypehead1&(0X80)) ScreenBuffer[CurrScreenBufferPos] = Color;
            if(TempTypehead1&(0X40)) ScreenBuffer[CurrScreenBufferPos+ScreenWidth] = Color;
            if(TempTypehead1&(0X20)) ScreenBuffer[CurrScreenBufferPos+ScreenWidth*2] = Color;
            if(TempTypehead1&(0X10)) ScreenBuffer[CurrScreenBufferPos+ScreenWidth*3] = Color;
            if(TempTypehead1&(0X08)) ScreenBuffer[CurrScreenBufferPos+ScreenWidth*4] = Color;
            if(TempTypehead1&(0X04)) ScreenBuffer[CurrScreenBufferPos+ScreenWidth*5] = Color;
            if(TempTypehead1&(0X02)) ScreenBuffer[CurrScreenBufferPos+ScreenWidth*6] = Color;
            if(TempTypehead1&(0X01)) ScreenBuffer[CurrScreenBufferPos+ScreenWidth*7] = Color;
            if( CurrWidth<9 ) goto LOOP;			
            TempTypehead2 = TypeheadData[CurrTypeheadPos++];
            if(TempTypehead2&(0X80)) ScreenBuffer[CurrScreenBufferPos+ScreenWidth*8] = Color;
            if(TempTypehead2&(0X40)) ScreenBuffer[CurrScreenBufferPos+ScreenWidth*9] = Color;
            if(TempTypehead2&(0X20)) ScreenBuffer[CurrScreenBufferPos+ScreenWidth*10] = Color;
            if(TempTypehead2&(0X10)) ScreenBuffer[CurrScreenBufferPos+ScreenWidth*11] = Color;
            if(TempTypehead2&(0X08)) ScreenBuffer[CurrScreenBufferPos+ScreenWidth*12] = Color;
            if(TempTypehead2&(0X04)) ScreenBuffer[CurrScreenBufferPos+ScreenWidth*13] = Color;
            if(TempTypehead2&(0X02)) ScreenBuffer[CurrScreenBufferPos+ScreenWidth*14] = Color;
            if(TempTypehead2&(0X01)) ScreenBuffer[CurrScreenBufferPos+ScreenWidth*15] = Color;
            LOOP:
            CurrScreenBufferPos --;
        }

        if(CurrWidth==ChineseWidth)
        {
            CurrScreenBufferPos = OldScreenBufferPos + (CurrWidth + WordsColumnGap)*ScreenWidth;
            CurrY += CurrWidth+WordsColumnGap;
        }
        else
        {
            CurrScreenBufferPos = OldScreenBufferPos + CurrWidth*ScreenWidth;
            CurrY += CurrWidth;
        }
        OldScreenBufferPos = CurrScreenBufferPos;
		
    }

    return iTRUE;
	
}

#endif

iKIT iBOOL i51KitG2VPEnable( void )
{
    // author : Otto
    // since : Sep 5 2011
    // (C) 2011 , PKIG Tech. Co., Ltd.

    // 调用此函数后所有G2的函数都是在虚拟画板上绘制
 #if i51KITG2_DEBUG
    iLog( "G2VPEnable start");
#endif

    i51UseVirtualPanel = iTRUE;
    i51CurrentVpManage.UseVirtualPanel = iTRUE;
    i51DrawVirtualPanel = iTRUE;

 #if i51KITG2_DEBUG
    iLog( "G2VPEnable end");
#endif
    return iTRUE;
}

iKIT iBOOL  i51KitG2VPDisable( void )
{
    // author : Otto
    // since : Sep 5 2011
    // (C) 2011 , PKIG Tech. Co., Ltd.

    // 调用此函数后不在使用画板

 #if i51KITG2_DEBUG
    iLog( "G2VPDisable start");
#endif

    i51UseVirtualPanel = iFALSE;
    i51CurrentVpManage.UseVirtualPanel = iFALSE;
  
 #if i51KITG2_DEBUG
    iLog( "G2VPDisable end");
#endif
    return iTRUE;
}

iKIT iHANDLE i51KitG2VPCreate ( iU16 TotalElement , iRECT * Rect )
{
    // author : Otto
    // since : Sep 5 2011
    // (C) 2011 , PKIG Tech. Co., Ltd.

    // iU32 TotalElement : 虚拟画板内同时最多装载的元素个数
    // iRECT Rect : 虚拟画板的属性，包括他的长宽以及相对屏幕的偏移量
    // iALPHA Alpha : 虚拟画板的透明度

    // notes :  创建一个虚拟画板

    iU16 iTemp;

    i51KitG2VPStructure * NewVp = iNULL;
    
 #if i51KITG2_DEBUG
    iLog( "G2VPCreate start, %x,%x", TotalElement,Rect);
#endif
  
	// 如果创建的画板数量过多则不允许创建新画板
    if( i51CurrentVpManage.TotalVPNumber >= i51CurrentVpManage.MaxVPNumber ) return iNULL;

    if( iNULL == Rect ) return iFALSE;

    if( (Rect->Width)<=0 || (Rect->Height)<=0 ) return iFALSE;

    // 分配画板内存  
    NewVp = ( i51KitG2VPStructure * )i51AdeOsMalloc((sizeof(i51KitG2VPStructure)), __FILE__ , __LINE__ );
    if( iNULL==NewVp ) return iNULL;

   // 分配画板内元素内存
    NewVp->VPElementDetails = iNULL;
    NewVp->VPElementDetails = ( i51KitG2VPElement * )i51AdeOsMalloc( ((sizeof(i51KitG2VPElement)) * TotalElement ), __FILE__ ,
__LINE__ );
    if( iNULL==NewVp->VPElementDetails )
    {
        i51AdeOsFree(NewVp);
        NewVp = iNULL;
        return iNULL;
    }

    // 画板能装载元素数量上限  
    NewVp->VPMaxElement = TotalElement;

    // 当前已经装载的元素个数  
    NewVp->VPCurrentElement = 0;

    // 虚拟画板句柄
    NewVp->VPHandle = (iHANDLE)(NewVp);

    // 虚拟画板属性
    NewVp->VPActualRect.X = Rect->X;
    NewVp->VPActualRect.Y = Rect->Y;
    NewVp->VPActualRect.Width= Rect->Width;
    NewVp->VPActualRect.Height= Rect->Height;

    NewVp->VpIsFull = iFALSE;

    NewVp->VpIsLoop = iFALSE;
	
    // 设置虚拟画板在管理器中的ID
    for( iTemp=0; iTemp<I51KITG2_MAX_NUMBER_VIRTUAL_PANEL; iTemp++ )
    {
        if( iNULL == (i51CurrentVpManage.VPDetails[iTemp]) )
        {
            NewVp->VP_ID= iTemp;
            i51CurrentVpManage.VPDetails[iTemp] = NewVp;
            break;
        }
    }

    if( iTemp>=I51KITG2_MAX_NUMBER_VIRTUAL_PANEL )
    {
        i51AdeOsFree(NewVp->VPElementDetails);
        NewVp->VPElementDetails = iNULL;
        i51AdeOsFree(NewVp);
        NewVp = iNULL;
        return iNULL;
    }

    NewVp->VpIsDraw = iTRUE;
	
    // VP在屏幕左边
    if((Rect->X+(Rect->Width))<0) NewVp->VpIsDraw = iFALSE;
    
    // VP在屏幕右边		
    if((Rect->X) >= ScreenWidth) NewVp->VpIsDraw = iFALSE;
    
    // VP在屏幕下方
    if((Rect->Y)>=ScreenHeight) NewVp->VpIsDraw = iFALSE;
    
    // VP在屏幕上方
    if( (Rect->Y+Rect->Height)<0) NewVp->VpIsDraw = iFALSE;

    if( i51CurrentVpManage.TotalVPNumber==0 )
    {
        i51CurrentVpActivity = NewVp;
    }
  
    i51CurrentVpManage.TotalVPNumber++;  

  #if i51KITG2_DEBUG
    iLog( "G2VPCreate end, %x,%x", TotalElement,Rect);
#endif

    return NewVp;

}

iKIT iBOOL i51KitG2VPDestroy ( iHANDLE vpanel )
{
    // author : Otto
    // since : Sep 5 2011
    // (C) 2011 , PKIG Tech. Co., Ltd.

    // iHANDLE vpanel : 待销毁虚拟画板句柄

    // notes :  销毁一个虚拟画板

    iU16 iTemp = 0;
	
    i51KitG2VPStructure * DestoryVp = ( i51KitG2VPStructure * )vpanel;

 #if i51KITG2_DEBUG
    iLog( "G2VPDestroy start, %x", vpanel);
#endif

    if( iNULL==DestoryVp ) return iFALSE;

    if( i51CurrentVpManage.TotalVPNumber<1) return iFALSE;

    for( iTemp=0; iTemp<DestoryVp->VPCurrentElement; iTemp++ )
    {
        if( DestoryVp->VPElementDetails[iTemp].ElementType==I51KITG2_ELEMENT_TYPE_POLYGON )
        {
            if( iFALSE==i51AdeOsFree((void *)( DestoryVp->VPElementDetails[iTemp].ElementData.Polygon.PointerPoints) ))
            {
                DestoryVp->VPElementDetails[iTemp].ElementData.Polygon.PointerPoints = iNULL;
                return iFALSE;
            }
            DestoryVp->VPElementDetails[iTemp].ElementData.Polygon.PointerPoints = iNULL;
        }
        else if( DestoryVp->VPElementDetails[iTemp].ElementType==I51KITG2_ELEMENT_TYPE_FILLPOLYGON )
        {
            if( iFALSE==i51AdeOsFree( (void *)( DestoryVp->VPElementDetails[iTemp].ElementData.FillPolygon.PointerPoints) ))
            {
                DestoryVp->VPElementDetails[iTemp].ElementData.FillPolygon.PointerPoints = iNULL;
                return iFALSE;
            }
            DestoryVp->VPElementDetails[iTemp].ElementData.FillPolygon.PointerPoints = iNULL;
        }
        else if( DestoryVp->VPElementDetails[iTemp].ElementType==I51KITG2_ELEMENT_TYPE_WORDS )
        {
            if( iFALSE==i51AdeOsFree( (void *)(DestoryVp->VPElementDetails[iTemp].ElementData.Words.WordsHandle)) )
            {
                DestoryVp->VPElementDetails[iTemp].ElementData.Words.WordsHandle = iNULL;
                return iFALSE;
            }
            DestoryVp->VPElementDetails[iTemp].ElementData.Words.WordsHandle = iNULL;
        }
    }
	
    if( (i51AdeOsFree(DestoryVp->VPElementDetails)) != iTRUE )
    {
       DestoryVp->VPElementDetails = iNULL;
       return iFALSE;
    }
   
    i51CurrentVpManage.VPDetails[DestoryVp->VP_ID] = iNULL;

    if( (i51AdeOsFree(DestoryVp)) != iTRUE )
    {
       DestoryVp = iNULL;
	return iFALSE;
    }
    i51CurrentVpManage.TotalVPNumber--;

  #if i51KITG2_DEBUG
    iLog( "G2VPDestroy end, %x", vpanel);
#endif
    return iTRUE;
  
}

iKIT iBOOL i51KitG2VPSetCoord ( iHANDLE vpanel , iS32 X, iS32 Y)
{
    // author : Otto
    // since : Sep 5 2011
    // (C) 2011 , PKIG Tech. Co., Ltd.

    // iHANDLE vpanel : 待移动虚拟画板句柄
    // iS32 X : 画板的新X坐标
    // iS32 Y : 画板的新Y坐标

    // notes :  移动一个VP,如果移动后VP与屏幕无交集则移动不成功

    i51KitG2VPStructure * MoveVp = ( i51KitG2VPStructure * )vpanel;
  #if i51KITG2_DEBUG
    iLog( "G2VPSetCoord start, %x,%x,%x", vpanel,X,Y);
#endif

    if( iNULL==MoveVp ) return iFALSE;

    MoveVp->VpIsDraw = iTRUE;
	
    // VP在屏幕左边
    if((X+(MoveVp->VPActualRect.Width))<0) MoveVp->VpIsDraw = iFALSE;
    
    // VP在屏幕右边		
    if((X)>=ScreenWidth) MoveVp->VpIsDraw = iFALSE;
    
    // VP在屏幕下方
    if((Y)>=ScreenHeight) MoveVp->VpIsDraw = iFALSE;
    
    // VP在屏幕上方
    if( (Y+((MoveVp->VPActualRect.Height)))<0) MoveVp->VpIsDraw = iFALSE;

    MoveVp->VPActualRect.X = X;
    MoveVp->VPActualRect.Y = Y;

 #if i51KITG2_DEBUG
    iLog( "G2VPSetCoord end, %x,%x,%x", vpanel,X,Y);
#endif
    return iTRUE;
  
}

iKIT iBOOL i51KitG2VPSetActivity ( iHANDLE vpanel )
{
    // author : Otto
    // since : Sep 5 2011
    // (C) 2011 , PKIG Tech. Co., Ltd.

    // iHANDLE vpanel : 待设置的虚拟画板句柄

    // notes :  设置画板vpanel为当前活动画板

    i51KitG2VPStructure * ActivityVp = ( i51KitG2VPStructure * )vpanel;
 #if i51KITG2_DEBUG
    iLog( "G2VPSetActivity start, %x", vpanel);
#endif
    if( iNULL==ActivityVp ) return iFALSE;

    if( i51CurrentVpManage.VPDetails[ActivityVp->VP_ID]!=ActivityVp ) return iFALSE;

    i51CurrentVpManage.ActivityVp_ID = ActivityVp->VP_ID;
    i51CurrentVpManage.CurrentVpActivity = ActivityVp;
    i51CurrentVpActivity = ActivityVp;

 #if i51KITG2_DEBUG
    iLog( "G2VPSetActivity end, %x", vpanel);
#endif
    return iTRUE;
  
}

iKIT iHANDLE i51KitG2VPGetActivity ( void )

{
    // author : Otto
    // since : Sep 5 2011
    // (C) 2011 , PKIG Tech. Co., Ltd.

    // notes : 返回当前活动画板的句柄

    return    i51CurrentVpActivity;
}

iKIT iBOOL i51KitG2VPClean ( iHANDLE vpanel )
{
    // author : Otto
    // since : Sep 5 2011
    // (C) 2011 , PKIG Tech. Co., Ltd.

    // iHANDLE vpanel : 待清空的虚拟画板句柄
    // notes : 清空虚拟画板

    iU16 iTemp = 0;
  
    i51KitG2VPStructure * ClearVp = ( i51KitG2VPStructure * )vpanel;

 #if i51KITG2_DEBUG
    iLog( "G2VPClean start, %x", vpanel);
#endif
    if( iNULL==ClearVp )     return iFALSE;

    if( i51CurrentVpManage.VPDetails[ClearVp->VP_ID]!=ClearVp ) return iFALSE;

    for( iTemp=0; iTemp<ClearVp->VPCurrentElement; iTemp++ )
    {
        if( ClearVp->VPElementDetails[iTemp].ElementType==I51KITG2_ELEMENT_TYPE_POLYGON )
        {
            if( iFALSE==i51AdeOsFree( (void *)(ClearVp->VPElementDetails[iTemp].ElementData.Polygon.PointerPoints) ))
            {
                return iFALSE;
            }
            ClearVp->VPElementDetails[iTemp].ElementData.Polygon.PointerPoints = iNULL;
        }
        else if( ClearVp->VPElementDetails[iTemp].ElementType==I51KITG2_ELEMENT_TYPE_FILLPOLYGON )
        {
            if( iFALSE==i51AdeOsFree( (void *)(ClearVp->VPElementDetails[iTemp].ElementData.FillPolygon.PointerPoints)) )
            {
                return iFALSE;
            }
            ClearVp->VPElementDetails[iTemp].ElementData.FillPolygon.PointerPoints = iNULL;
        }
        else if( ClearVp->VPElementDetails[iTemp].ElementType==I51KITG2_ELEMENT_TYPE_WORDS )
        {
            if( iFALSE==i51AdeOsFree( (void *)(ClearVp->VPElementDetails[iTemp].ElementData.Words.WordsHandle)) )
            {
                return iFALSE;
            }
            ClearVp->VPElementDetails[iTemp].ElementData.Words.WordsHandle = iNULL;
        }
    }

    ClearVp->VpIsFull = iFALSE;
    ClearVp->VPCurrentElement = 0 ;

 #if i51KITG2_DEBUG
    iLog( "G2VPClean end, %x", vpanel);
#endif

    return iTRUE;
  
}

iKIT iBOOL i51KitG2VPGetAtrribute ( iHANDLE vpanel , iRECT * Rect , iU16 * ResidueSpace  )
{
    // author : Otto
    // since : Sep 5 2011
    // (C) 2011 , PKIG Tech. Co., Ltd.

    // iHANDLE vpanel : 待获得属性的虚拟画板句柄
    // iRECT * Rect : 画板大小及坐标信息
    // iU16 * ResidueSpace : 画板剩余空间
    
    // notes : 获得一张虚拟画板的信息

    i51KitG2VPStructure * VpTemp = ( i51KitG2VPStructure * )vpanel;

#if i51KITG2_DEBUG
    iLog( "G2VPGetAtrribute start, %x,%x,%x,%x", vpanel,Rect,ResidueSpace );
#endif

    if( iNULL==VpTemp ) return iFALSE;

    if( i51CurrentVpManage.VPDetails[VpTemp->VP_ID]!=VpTemp ) return iFALSE;

    if(Rect)
    {
        Rect->X = VpTemp->VPActualRect.X;
        Rect->Y = VpTemp->VPActualRect.Y;
        Rect->Width= VpTemp->VPActualRect.Width;
        Rect->Height= VpTemp->VPActualRect.Height;
    }

    if(ResidueSpace)
    {
        *ResidueSpace = VpTemp->VPMaxElement - VpTemp->VPCurrentElement;
    }

#if i51KITG2_DEBUG
    iLog( "G2VPGetAtrribute end, %x,%x,%x,%x", vpanel,Rect,ResidueSpace );
#endif
    return iTRUE;
  
}

iKIT iBOOL i51KitG2VPiSFull( iHANDLE vpanel )
{
    // author : Otto
    // since : Sep 5 2011
    // (C) 2011 , PKIG Tech. Co., Ltd.

    // iHANDLE vpanel : 待获得属性的虚拟画板句柄
    // notes : 判断画板是否已经画满
#if i51KITG2_DEBUG
    iLog( "G2VPiSFull start, %x", vpanel);
#endif
    if( iNULL==vpanel ) return iFALSE;
#if i51KITG2_DEBUG
    iLog( "G2VPiSFull end, %x", vpanel);
#endif
    return (((i51KitG2VPStructure *)vpanel)->VpIsFull);

}

iKIT iBOOL i51KitG2VPRender ( iHANDLE vpanel , iRECT * DrawRect)
{
    // author : Otto
    // since : Sep 5 2011
    // (C) 2011 , PKIG Tech. Co., Ltd.

    // iHANDLE vpanel : 待Render的虚拟画板句柄
    // iRECT * DrawRect : 绘制到物理画板上的区域
    // notes :  将虚拟面板上的信息绘制到物理面板上
  
    iU16 iTemp = 0;
    iU16 NumberDrawImage     = 0;
    iU8   SysAlpha = 0;
	
    i51KitG2VPStructure * VpRender = ( i51KitG2VPStructure * )vpanel;

    i51KitG2VPElement * VPElement = iNULL;

#if i51KITG2_DEBUG
    iLog( "G2VPRender start, %x,%d", vpanel,DrawRect);
#endif
    if( iFALSE == i51UseVirtualPanel ) return iFALSE;
	
    if( iNULL==VpRender ) return iFALSE;

    if( i51CurrentVpManage.VPDetails[VpRender->VP_ID]!=VpRender ) return iFALSE;

    if( iFALSE == VpRender->VpIsDraw ) return iTRUE;
	
    i51DrawVirtualPanel = iFALSE;

    if(DrawRect)
    {
        VpRender->VPDrawRect.X = DrawRect->X;
        VpRender->VPDrawRect.Y= DrawRect->Y;
        VpRender->VPDrawRect.Width = DrawRect->Width;
        VpRender->VPDrawRect.Height = DrawRect->Height;
    }
    else
    {
        VpRender->VPDrawRect.X = 0;
        VpRender->VPDrawRect.Y= 0;
        VpRender->VPDrawRect.Width = ScreenWidth;
        VpRender->VPDrawRect.Height = ScreenHeight;
    }

    if( VpRender->VPDrawRect.X < 0  )
    {
        VpRender->VPDrawRect.Width += VpRender->VPDrawRect.X;
        VpRender->VPDrawRect.X = 0;
    }
	
    if( VpRender->VPDrawRect.Y < 0  )
    {
        VpRender->VPDrawRect.Height+= VpRender->VPDrawRect.Y;
        VpRender->VPDrawRect.Y = 0;
    }

    if( VpRender->VPDrawRect.X + VpRender->VPDrawRect.Width> ScreenWidth) 
    {
        VpRender->VPDrawRect.Width=  ScreenWidth- VpRender->VPDrawRect.X;	
    }
	
    if( VpRender->VPDrawRect.Y + VpRender->VPDrawRect.Height > ScreenHeight) 
    {
        VpRender->VPDrawRect.Height =  ScreenHeight- VpRender->VPDrawRect.Y;	
    }
	
    // 待绘制区域长度或者宽度小于等于0
    if( VpRender->VPDrawRect.Height<=0 || VpRender->VPDrawRect.Width<=0 ) return iFALSE;
	
    // 绘制开始区域在VP右边
    if( VpRender->VPDrawRect.X > (VpRender->VPActualRect.X + VpRender->VPActualRect.Width) )  return iFALSE;

    // 绘制结束区域在VP左边
    if( (VpRender->VPDrawRect.X + VpRender->VPDrawRect.Width) < VpRender->VPActualRect.X  )  return iFALSE;

    // 绘制开始区域在VP上方
    if( (VpRender->VPDrawRect.Y+ VpRender->VPDrawRect.Height) < VpRender->VPActualRect.Y )  return iFALSE;
	
    // 绘制结束区域在VP下方
    if( VpRender->VPDrawRect.Y > (VpRender->VPActualRect.Y + VpRender->VPActualRect.Height))  return iFALSE;
	
    i51CurrentVpManage.CurrentVpRender = VpRender;
    i51CurrentVpRender = VpRender;

    SysAlpha = AlphaSys;
    NumberDrawImage = VpRender->VPCurrentElement;
    for( iTemp=0; iTemp<NumberDrawImage; iTemp++ )
    {
        VPElement = &(VpRender->VPElementDetails[iTemp]);
        i51KitG2SetAlpha(VPElement->ElemenAlpha);
        switch( VPElement->ElementType )
        {
            case I51KITG2_ELEMENT_TYPE_PICTURE :
                switch( VPElement->ElementData.Image.DrawDirection)
                {
                    case I51KITG2_DRAW_IAMGE_NORMAL:
                        i51KitG2DrawImage(VPElement->ElementData.Image.ContainerHandle,
							VPElement->ElementData.Image.ImageHandle,
							&(VPElement->ElementData.Image.DesRect),
							&(VPElement->ElementData.Image.SrcRect) );
                        break;
                    case I51KITG2_DRAW_IAMGE_MIRROR :
                        i51KitG2MirrorImage(VPElement->ElementData.Image.ContainerHandle,
							VPElement->ElementData.Image.ImageHandle,
							&(VPElement->ElementData.Image.DesRect),
							&(VPElement->ElementData.Image.SrcRect) );
                        break;						
                    case I51KITG2_DRAW_IAMGE_HANDSTAND :
                        i51KitG2FlipImage(VPElement->ElementData.Image.ContainerHandle,
							VPElement->ElementData.Image.ImageHandle,
							&(VPElement->ElementData.Image.DesRect),
							&(VPElement->ElementData.Image.SrcRect) );
                        break;						
                    case I51KITG2_DRAW_IAMGE_ROTATE:
                        i51KitG2RotateImage(VPElement->ElementData.Image.ContainerHandle,
							VPElement->ElementData.Image.ImageHandle,
							&(VPElement->ElementData.Image.DesRect),
							&(VPElement->ElementData.Image.SrcRect),
							VPElement->ElementData.Image.SrcMoveX,
							VPElement->ElementData.Image.SrcMoveY,
							VPElement->ElementData.Image.RotateCenterX,
							VPElement->ElementData.Image.RotateCenterY,
							VPElement->ElementData.Image.RotateType,
							(VPElement->ElementData.Image.DarwAngle ) );
                        break;						
                    default :
                       goto ERROR;
                        break;						
                }
                break;
            case I51KITG2_ELEMENT_TYPE_POINT :
                i51KitG2DrawPoint(VPElement->ElementData.Point.X,
    				VPElement->ElementData.Point.Y,
    				VPElement->ElementData.Point.Color );
                break;
            case I51KITG2_ELEMENT_TYPE_LINE :
                i51KitG2DrawLine( VPElement->ElementData.Line.Points,
    				VPElement->ElementData.Line.Color );
                break;	
            case I51KITG2_ELEMENT_TYPE_POLYGON :
                i51KitG2DrawPolygon( VPElement->ElementData.Polygon.VerticeNumber,
    				(iS32(*)[2])VPElement->ElementData.Polygon.PointerPoints,
    				VPElement->ElementData.Polygon.Color );
                break;	
            case I51KITG2_ELEMENT_TYPE_ARC:
                i51KitG2DrawArc( VPElement->ElementData.Arc.Points ,
    				VPElement->ElementData.Arc.Color );
                break;	
            case I51KITG2_ELEMENT_TYPE_CIRCLE:
                i51KitG2DrawCircle( VPElement->ElementData.Circle.X  ,
    				VPElement->ElementData.Circle.Y,
    				VPElement->ElementData.Circle.R,
    				VPElement->ElementData.Circle.Color);
                break;	
            case I51KITG2_ELEMENT_TYPE_FILLPOLYGON :
                i51KitG2FillPolygon( VPElement->ElementData.FillPolygon.VerticeNumber,
    				(iS32(*)[2])(VPElement->ElementData.FillPolygon.PointerPoints),
    				VPElement->ElementData.FillPolygon.Color );
                break;	
            case I51KITG2_ELEMENT_TYPE_FILLARC :
                i51KitG2FillArc(VPElement->ElementData.FillArc.Points ,
    				VPElement->ElementData.FillArc.Color );
                break;	
            case I51KITG2_ELEMENT_TYPE_FILLCRICLE :
                i51KitG2FillCircle( VPElement->ElementData.FillCircle.X  ,
    				VPElement->ElementData.FillCircle.Y,
    				VPElement->ElementData.FillCircle.R,
    				VPElement->ElementData.FillCircle.Color);
                break;	
            case I51KITG2_ELEMENT_TYPE_WORDS :
                i51KitG2FontDraw(
					VPElement->ElementData.Words.ReFileHandle,
					VPElement->ElementData.Words.TypeheadID,
					(void *)VPElement->ElementData.Words.WordsHandle,
					VPElement->ElementData.Words.WordsNum,
					&(VPElement->ElementData.Words.DrawRect),
					&(VPElement->ElementData.Words.SrcRect),
					VPElement->ElementData.Words.DrawDirect,
					VPElement->ElementData.Words.WordsRowGap,
					VPElement->ElementData.Words.WordsColumnGap,
					VPElement->ElementData.Words.fgcolor,
					VPElement->ElementData.Words.bgcolor,
					VPElement->ElementData.Words.Effect);
				break;
            case I51KITG2_ELEMENT_TYPE_FILLIMAGE :
                i51KitG2ImageFillRect( VPElement->ElementData.Fill.ContainerHandle,
					VPElement->ElementData.Fill.ImageHandle,
					&(VPElement->ElementData.Fill.DesRect),
					&(VPElement->ElementData.Fill.SrcRect) );
                break;
				
            default :
                goto ERROR;
                break;
        }		
    }
  
    i51DrawVirtualPanel = iTRUE;

#if i51KITG2_DEBUG
    iLog( "G2VPRender end, %x,%d", vpanel,DrawRect);
#endif

   i51KitG2SetAlpha(SysAlpha);
   return iTRUE;
ERROR:
    iLog( "KIT - G2 : ElementData");
    i51KitG2SetAlpha(SysAlpha);
    return iFALSE;
	
}

iKIT iBOOL i51KitG2Update( void )
{
    // 实现局部刷新需要在这里做处理
    i51AdeMmiUpdateScreen();
    return iTRUE;
}

#if DEBUG_I51KITG2
#if 0
enum{
	BG,
	RUN_LEFT,
	RUN_RIGHT,
	WALL,
	BEE,
	LEAF,
	ORANGE,
	PINEAPPLE,
	STRAWBERRY,
	WATERMELON,
	APPLE,
	BOMB,
	FRUIT_BG1,
	FRUIT_BG2,
	REDBLUE_FISH
};
#endif
enum{
	BG,	
	RUN_LEFT,	
	RUN_RIGHT,	
	WALL,	
	BEE,	
	SPRITE,	
	prizenum,	
	shayu,
	test1,
	LEAF,
	ORANGE,
	PINEAPPLE,
	STRAWBERRY,
	WATERMELON,
	APPLE,
	BOMB,
	FRUIT_BG1,
	FRUIT_BG2,
	REDBLUE_FISH
};


void TestDrawLine( void )
{

  static iS32 VertexPosition = 1;
  static iS32 Point[2][2] = {-10,-10,250,-10};

  iCOLOR Color = 0X00;

  Color = i51KitG2COLOR( 0XFF, 0XFF, 0 );

    memset ( LcdBuffer , 0 , 240*320*2 ) ;
  
    i51KitG2DrawLine( Point, Color);
    i51AdeMmiUpdateScreen  () ;

    switch(VertexPosition)
    {
    case 1:
      if( Point[1][1]<ScreenHeight+10 )
      {
        Point[1][1]++;
      }
      else
      {
        Point[1][0]--;
        if( Point[1][0]<-10 )
        {
          VertexPosition = 2;
          Point[1][0] = 0;
          Point[1][1] = ScreenHeight +10;
        }
      }
      break;
    case 2:
      if( Point[0][0]<ScreenWidth+10 )
      {
        Point[0][0]++;
      }
      else
      {
        Point[0][1]++;
        if(Point[0][1]>ScreenHeight+10)
        {
          VertexPosition = 3;
          Point[0][0] = ScreenWidth +10;
          Point[0][1] = ScreenHeight +10;
        }
      }
      break;
    case 3:
      if( Point[1][1]>-10 )
      {
        Point[1][1]--;
      }
      else
      {
        Point[1][0]++;
        if(Point[1][0]>ScreenWidth+10)
        {
          VertexPosition = 4;
          Point[1][0] = ScreenWidth +10;
          Point[1][1] = -10;
        }
      }
      break;
    case 4:
      if( Point[0][0]>-10 )
      {
        Point[0][0]--;
      }
      else
      {
        Point[0][1]--;
        if(Point[0][1]<-10)
        {
          VertexPosition = 1;
          Point[0][0] = -10;
          Point[0][1] = -10;
        }
      }
      break;
    default :
      break;
  }
}

void TestDrawPolygon( void )
{

  iU32 VerticeNumber =0;
  iS32 Point[10][2];
  iCOLOR Color;

  memset ( LcdBuffer , 0 , 240*320*2 ) ;

  VerticeNumber = 3;
  Point[0][0] = 40;
  Point[0][1] = 10;
  Point[1][0] = 20;
  Point[1][1] = 30;
  Point[2][0] = 60;
  Point[2][1] = 30;
  Color =  i51KitG2COLOR(0XFF, 0, 0);
  i51KitG2DrawPolygon( VerticeNumber, Point, Color);

  VerticeNumber = 4;
  Point[0][0] = 80;
  Point[0][1] = 10;
  Point[1][0] = 110;
  Point[1][1] = 15;
  Point[2][0] = 140;
  Point[2][1] = 50;
  Point[3][0] = 110;
  Point[3][1] = 50;
  Color =  i51KitG2COLOR(0XFF, 0XFF, 0);
  i51KitG2DrawPolygon( VerticeNumber, Point, Color);

  VerticeNumber = 5;
  Point[0][0] = 10;
  Point[0][1] = 100;
  Point[1][0] = 20;
  Point[1][1] = 90;
  Point[2][0] = 60;
  Point[2][1] = 120;
  Point[3][0] = 150;
  Point[3][1] = 160;
  Point[4][0] = 30;
  Point[4][1] = 120;
  Color =  i51KitG2COLOR(0XFF, 0XFF, 0XFF);
  i51KitG2DrawPolygon( VerticeNumber, Point, Color);

  i51AdeMmiUpdateScreen  () ;

}

void TestDrawArc( void )
{
  iS32 Point[3][2];
  iCOLOR Color;
  iS32 i,j;
  iS8 RegulationY = 1, RegulationX = 1;
  iS8 RegulationY1 = 1, RegulationX1= 1;

  Point[0][0] = 10;
  Point[0][1] = 100;
  Point[1][0] = 150;
  Point[1][1] = 30;
  Point[2][0] = 182;
  Point[2][1] = 76;

  Point[0][0] = 54;
  Point[0][1] = 133;
  Point[1][0] = 150;
  Point[1][1] = 30;
  Point[2][0] = 178;
  Point[2][1] = 226;
  Color =  i51KitG2COLOR(0XFF, 0, 0);

  while(1)
  {
    memset ( LcdBuffer , 0 , 240*320*2 ) ;

    Color =  i51KitG2COLOR(0XFF, 0, 0);
    i51KitG2DrawArc( Point, Color );

    Color =  i51KitG2COLOR(0XFF, 0XFF, 0XFF);
    LcdBuffer[ (ScreenWidth * Point[1][1] + Point[1][0])  ] = Color ;
//     i51AdeStdMemcpy16( &LcdBuffer[ (ScreenWidth * Point[1][1] + Point[1][0])  ], &Color, 2);

    if( Point[2][0]<0)
    {
      RegulationX = 1;

       Point[0][0] += RegulationX1;
      if( Point[0][0]>ScreenWidth )  RegulationX1 = -1;
      if( Point[0][0]<0) RegulationX1 = 1;

    }

    if( Point[2][1]<0)
    {

      RegulationY = 1;
       Point[0][1] += RegulationY1;
      if( Point[0][1]>ScreenHeight)  RegulationY1 = -1;
      if( Point[0][1]<0) RegulationY1 = 1;
    }

    if( Point[2][0]>ScreenWidth )
    {
       Point[0][0] += RegulationX1;
      if( Point[0][0]>ScreenWidth )  RegulationX1 = -1;
      if( Point[0][0]<0) RegulationX1 = 1;
    
      RegulationX = -1;
    }
    if( Point[2][1]>ScreenHeight)
    {
      Point[0][1] += RegulationY1;
      if( Point[0][1]>ScreenHeight)  RegulationY1 = -1;
      if( Point[0][1]<0) RegulationY1 = 1;

      RegulationY = -1;
    }
    
    Point[2][0] += RegulationX;
    Point[2][1] += RegulationY;
    i51AdeMmiUpdateScreen  () ;
    for( i=0;i<1000;i++ )
      for( j=0;j<500;j++);
  };
}

void TestDarwCircle( void )
{
  iS32 CenterX = 110;
  iS32 CenterY = 110;
  iU32 CircleR = 10;
  iS32 i,j;
  iCOLOR Color =  i51KitG2COLOR(0XFF, 0, 0);

  iS8 RegulationY = 1, RegulationX = 1, RegulationR = 1;

  while(1)
  {

    memset ( LcdBuffer , 0 , 240*320*2 ) ;

    i51KitG2DrawCircle( CenterX, CenterY, CircleR, Color);
  
    i51AdeMmiUpdateScreen  () ;
  
    CircleR += RegulationR;
    CenterX += RegulationX;
    CenterY += RegulationY;

    if( CircleR<10 )
    {
      RegulationR = 1;
    }
    else if( CircleR > 100 )
    {
      RegulationR = -1;
    }

    if( CenterX<0 )
    {
      RegulationX = 1;
    }
    else if( CenterX > ScreenWidth )
    {
      RegulationX = -1;
    }

    if( CenterY<0 )
    {
      RegulationY = 1;
    }
    else if( CenterY > ScreenWidth )
    {
      RegulationY = -1;
    }
    for( i=0;i<1000;i++ )
      for( j=0;j<500;j++);
  }
}

void TestFillPolygon( void )
{
  iU32 VerticeNumber =0;
  iS32 Point[10][2];
  iCOLOR Color;
  iRECT DesRect;

  Point[0][0] = 9;
  Point[0][1] = -8;
  Point[1][0] = 209;
  Point[1][1] = -8;
  Point[2][0] = 209;
  Point[2][1] = 192;
  Point[3][0] = 9;
  Point[3][1] = 192;
  i51KitG2CleanScreen(0XAAAA,iNULL);
  Color =  i51KitG2COLOR(0XFF, 0, 0);
  i51KitG2FillPolygon( 4, Point, Color);
  i51KitG2Update();
//return ;

  DesRect.X =100;
  DesRect.Y = 100;
  DesRect.Width = 100;
  DesRect.Height = 100;
  i51KitG2CleanScreen(0XAAAA,iNULL);
  VerticeNumber = 3;
  Point[0][0] = 40;
  Point[0][1] = 10;
  Point[1][0] = 20;
  Point[1][1] = 30;
  Point[2][0] = 60;
  Point[2][1] = 30;
  Color =  i51KitG2COLOR(0XFF, 0, 0);
  i51KitG2FillPolygon( VerticeNumber, Point, Color);

  VerticeNumber = 4;
  Point[0][0] = 80;
  Point[0][1] = 10;
  Point[1][0] = 110;
  Point[1][1] = 15;
  Point[2][0] = 140;
  Point[2][1] = 50;
  Point[3][0] = 110;
  Point[3][1] = 50;
  Color =  i51KitG2COLOR(0XFF, 0XFF, 0);
  i51KitG2FillPolygon( VerticeNumber, Point, Color);
  i51KitG2DrawPolygon( VerticeNumber, Point, Color);

  VerticeNumber = 7;
  Point[0][0] = 120;
  Point[0][1] = 70;
  Point[1][0] = 220;
  Point[1][1] = 50;
  Point[2][0] = 250;
  Point[2][1] = 170;
  Point[3][0] = 120;
  Point[3][1] = 150;
  Point[4][0] = 200;
  Point[4][1] = 130;
  Point[5][0] = 120;
  Point[5][1] = 110;
  Point[6][0] = 200;
  Point[6][1] = 90;
  Color =  i51KitG2COLOR(0XF0, 0X0F, 0XF0);
  i51KitG2FillPolygon( VerticeNumber, Point, Color);
  i51KitG2DrawPolygon( VerticeNumber, Point, Color);

  VerticeNumber = 7;
  Point[0][0] = 10;
  Point[0][1] = 300;
  Point[1][0] = 40;
  Point[1][1] = 200;
  Point[2][0] = 70;
  Point[2][1] = 250;
  Point[3][0] = 90;
  Point[3][1] = 201;
  Point[4][0] = 110;
  Point[4][1] = 251;
  Point[5][0] = 130;
  Point[5][1] = 205;
  Point[6][0] = 150;
  Point[6][1] = 330;
  Color =  i51KitG2COLOR(0XFF, 0XFF, 0XFF);
  i51KitG2FillPolygon( VerticeNumber, Point, Color);
  i51KitG2DrawPolygon( VerticeNumber, Point, Color);

  i51AdeMmiUpdateScreen  () ;

}

void TestFillArc( void )
{
  iS32 i,j;
  iCOLOR Color;
  iS32 Point[3][2];
  iS8 RegulationY = 1, RegulationX = 1;
  iS8 RegulationY1 = 1, RegulationX1= 1;

  Point[0][0] = 10;
  Point[0][1] = 100;
  Point[1][0] = 150;
  Point[1][1] = 30;
  Point[2][0] = 182;
  Point[2][1] = 76;

  Point[0][0] = 54;
  Point[0][1] = 133;
  Point[1][0] = 150;
  Point[1][1] = 30;
  Point[2][0] = 178;
  Point[2][1] = 226;
  Color =  i51KitG2COLOR(0XFF, 0, 0);
//   i51AdeStdMemcpy16( &LcdBuffer[ (ScreenWidth * Point[1][1] + Point[1][0]) << 1 ], &Color, 2);


  while(1)
  {
    memset ( LcdBuffer , 0 , 240*320*2 ) ;

    Color =  i51KitG2COLOR(0XFF, 0, 0);
    i51KitG2FillArc( Point, Color );
    Color =  i51KitG2COLOR(0XFF, 0XFF, 0XFF);
     i51AdeStdMemcpy16( &LcdBuffer[ (ScreenWidth * Point[1][1] + Point[1][0]) ], &Color, 2);
    i51AdeMmiUpdateScreen  () ;

    if( Point[2][0]<0)
    {
      RegulationX = 1;

       Point[0][0] += RegulationX1;
      if( Point[0][0]>ScreenWidth )  RegulationX1 = -1;
      if( Point[0][0]<0) RegulationX1 = 1;

    }

    if( Point[2][1]<0)
    {

      RegulationY = 1;
       Point[0][1] += RegulationY1;
      if( Point[0][1]>ScreenHeight)  RegulationY1 = -1;
      if( Point[0][1]<0) RegulationY1 = 1;
    }

    if( Point[2][0]>ScreenWidth )
    {
       Point[0][0] += RegulationX1;
      if( Point[0][0]>ScreenWidth )  RegulationX1 = -1;
      if( Point[0][0]<0) RegulationX1 = 1;
    
      RegulationX = -1;
    }
    if( Point[2][1]>ScreenHeight)
    {
      Point[0][1] += RegulationY1;
      if( Point[0][1]>ScreenHeight)  RegulationY1 = -1;
      if( Point[0][1]<0) RegulationY1 = 1;

      RegulationY = -1;
    }
    
    Point[2][0] += RegulationX;
    Point[2][1] += RegulationY;
    for( i=0;i<1000;i++ )
      for( j=0;j<100;j++);
  }
}
void TestFillCircle( void )
{
  iS32 CenterX = 120;
  iS32 CenterY = 160;
  iU32 CircleR = 120;
  iS32 i,j;
static  iCOLOR Color =  i51KitG2COLOR(0XFF, 0, 0);

  iS8 RegulationY = 1, RegulationX = 1, RegulationR = 1;


    memset ( LcdBuffer , 0 , 480*320*2 ) ;
for(i=0;i<16;i++)
for(j=0;j<24;j++)
{
    i51KitG2FillCircle( 10+20*i, 10+20*j, 10, Color);
}
  Color+=5000;
    i51AdeMmiUpdateScreen  () ;

}

void Process(iU32 Step , iU32 Totall , iHANDLE Handle)
{
   // i51KitG2UseImageContainer(Handle);
//   CurrentImageContainer = Handle;
	if(Handle)
	{
		DecodeComplete = iFALSE;
    memset ( LcdBuffer , 0 , 240*320*2 ) ;
		i51KitG2DrawImage(Handle, Step-1, iNULL, iNULL);
		i51KitG2Update();
	}
	else
		DecodeComplete = iTRUE;
}

void Done( iHANDLE Handle)
{
	if(Handle)
	{
		CurrentImageContainer = Handle;
		DecodeComplete = iTRUE;
//		i51KitG2CloseImageContainer(Handle);
//		CurrentImageContainer = i51KitG2OpenImageContainer(L"i51KitG2\\i51IMIC");
//             i51KitG2SetImageMemoryCopy(CurrentImageContainer,0,iNULL);
	}
	else
		DecodeComplete = iFALSE;
}

void TestCreateImageContainer( void )
{
static iS32 ImageId = 0;
if( FLAG_FIRSTRUN )
{
	iU16 * Path = L"i51KitG2\\i51IMIC";
	iFILE  Test = i51KitStdResOpen ( L"i51KitG2\\i51KitG2.re" ) ;
	iU32 ImageId[11] = {0,1,2,3,4,5,6,7,8};

    if(i51KitG2CreateImageContainer ( Test , ImageId , 9 , Path , Process , Done ))
    {
    #if DEBUG_I51KITG2
        iLog("创建容器成功.\n");
    #endif
    }
FLAG_FIRSTRUN = iFALSE;
}
if(CurrentImageContainer)
{
	i51KitG2DrawImage(CurrentImageContainer, (ImageId/30)%9, iNULL, iNULL);
	i51KitG2Update();
	ImageId++;
}
}

void TestOpenImageContainer( void )
{
  iU16 * Path = L"i51KitG2\\i51IMIC";
  if(i51KitG2OpenImageContainer(Path))
  {
  #if DEBUG_I51KITG2
        iLog("打开容器成功.\n");
  #endif
  }
}

void TestCloseImageContainer( void )
{
//    iU16 * Path = L"i51KitG2\\i51IMIC";
//    CurrentImageContainer = i51KitG2OpenImageContainer(Path);

#if DEBUG_I51KITG2
//    if( CurrentImageContainer ) iLog("打开容器成功.\n");
#endif

    if( i51KitG2CloseImageContainer( (iHANDLE)(&CurrentImageContainer) ))
    {
    #if DEBUG_I51KITG2
        iLog("关闭容器成功.\n");
    #endif
    }
}

void TestDestroyImageContainer( void )
{
    iU16 * Path = L"i51KitG2\\i51IMIC";
    iHANDLE ImageContainer;

    if( (ImageContainer=i51KitG2OpenImageContainer(Path))!=iFALSE)
    {
       iLog("打开容器成功.\n");
    }
    if( i51KitG2DestroyImageContainer( ImageContainer, Path) )
    {
       iLog(" 销毁容器成功.\n");
    }
}

void TestDrawImage ( void )
{
	iU16 * Path = L"i51KitG2\\i51IMIC";
	static iFILE  Test ;
//	iU32 ImageId[5] = {BG,RUN_LEFT,RUN_RIGHT,WALL,BEE};
	iU32 ImageId[1] = {BEE};
       iU32 Ver = 0;

	if( FLAG_FIRSTRUN)
	{
		FLAG_FIRSTRUN = iFALSE;
		if( i51AdeOsFileIsExist(L"i51KitG2\\i51IMIC") )
		{
		       if(iFALSE==i51KitG2CheckImageContainer(L"i51KitG2\\i51IMIC")) return;
			   
			CurrentImageContainer = i51KitG2OpenImageContainer(Path);
			i51KitG2SetImageMemoryCopy(CurrentImageContainer,0,iNULL);
			DecodeComplete = iTRUE;
		}
		else
		{
			Test = i51KitStdResOpen ( L"i51KitG2\\i51KitG2.re" ) ;
			i51KitG2CreateImageContainer ( Test , ImageId , 1 , Path ,  Process , Done );
		}
	}
	
    memset ( LcdBuffer , 0XFF , 240*320*2 ) ;

	if(DecodeComplete)
	{
		i51KitG2VPDisable();
		i51KitG2DrawImage(CurrentImageContainer ,BEE, iNULL, iNULL);
	}
	i51KitG2Update();

}

#define TestMirror 	10
#define nin_l_j	11

void TestRotateImage ( void )
{
    //    iU16 * Path = L"E:\\i51\\i51APP\\i51KitG2\\";
    #define MAX_FRUIT 10
    iU16 * Path = L"i51KitG2\\i51IMIC";
    iRECT SrcRect, SrcRect1, DesRect1;
    static iS32 Angle = 0;
    static iS32 SrcMoveX[MAX_FRUIT] = {0,80,160,0,80,160,0,80,160,90};
    static iS32 SrcMoveY[MAX_FRUIT] = {10,10,10,90,90,90,170,170,170,250};
    static iS32 Xadds[MAX_FRUIT] = {0};
    static iS32 Yadds[MAX_FRUIT] = {0};
    static iS32 Aadds[MAX_FRUIT] = {15,-18,15,-15,10,-15,15,-10,18,-15};
    static iS32 MaxX[MAX_FRUIT] = {0};
	static iS32 CurrImage[MAX_FRUIT] = {0};
    iU32 ImageId[2] = {nin_l_j};
    static iFILE  Test ;
    i51ImageIndex * ImageIndexTable = iNULL;
    i51ImageContainer * ImageContainer = iNULL;
    iS32 iTemp =0;	
	if( FLAG_FIRSTRUN)
	{
		FLAG_FIRSTRUN = iFALSE;
		if( i51AdeOsFileIsExist(L"i51KitG2\\i51IMIC") )
		{
//		       if(iFALSE==i51KitG2CheckImageContainer(L"i51KitG2\\i51IMIC")) return;
			   
			CurrentImageContainer = i51KitG2OpenImageContainer(Path);
//			if(ScreenWidth>240){ ImageId[12] = ImageId[13]; ImageId[13] = ImageId[14];}
			i51KitG2SetImageMemoryCopy(CurrentImageContainer,0,iNULL);
			
			DecodeComplete = iTRUE;
		}
		else
		{
			Test = i51KitStdResOpen ( L"i51KitG2\\i51KitG2.re" ) ;
			i51KitG2CreateImageContainer ( Test , ImageId , 1, Path ,  Process , Done );
		}

	}

if(DecodeComplete)
{
    if( iNULL== CurrentImageContainer) return;
    ImageContainer = (i51ImageContainer *)CurrentImageContainer;
    ImageIndexTable = ( i51ImageIndex *  )(ImageContainer->ImageIndexTable);

SrcRect.X = 10;
SrcRect.Y= 10;
SrcRect.Width = 80;
SrcRect.Height = 80;

DesRect1.X = 100;
DesRect1.Y = 100;
DesRect1.Width= 100;
DesRect1.Height= 100;

Angle++;
i51KitG2CleanScreen(0XFFFF, iNULL);
i51KitG2RotateImage( CurrentImageContainer, 0, 0, 0,100,100,120,160,0,270);
//i51KitG2DrawImage(CurrentImageContainer, TestMirror, iNULL, iNULL);
//i51KitG2MirrorImage(CurrentImageContainer, nin_l_j, iNULL, iNULL);
}
    i51AdeMmiUpdateScreen  () ;

}


void TestDrawGraph( void )
{
	iS32 Point[10][2];
	iS32 VerticeNumber = 0;

	iCOLOR Color = 0XFFFF;

//	if( FLAG_FIRSTRUN )
	{
//		FLAG_FIRSTRUN = iFALSE;
		Point[0][0] = 0;
		Point[0][1] = 0;
		Point[1][0] = 239;
		Point[1][1] = 0;
		Point[2][0] = 239;
		Point[2][1] = 319;
		Point[3][0] = 0;
		Point[3][1] =  319;
		Color = i51KitG2COLOR( 0, 0, 0 );
//		i51KitG2FillPolygon( 4, Point, Color);
		
		Color = i51KitG2COLOR( 0XFF, 0, 0 );
		i51KitG2DrawPoint( 105, 80, Color)  ;
		i51KitG2DrawPoint( 110, 80, Color)  ;
		i51KitG2DrawPoint( 115, 80, Color)  ;
		i51KitG2DrawPoint( 120, 80, Color)  ;

		Point[0][0] = 15;
		Point[0][1] = 12;
		Point[1][0] = 115;
		Point[1][1] = 112;
		Color = i51KitG2COLOR( 0, 0XFF, 0 );
		i51KitG2DrawLine( Point, Color);
		
		VerticeNumber = 7;
		Point[0][0] = 120;
		Point[0][1] = 70;
		Point[1][0] = 220;
		Point[1][1] = 50;
		Point[2][0] = 220;
		Point[2][1] = 170;
		Point[3][0] = 120;
		Point[3][1] = 150;
		Point[4][0] = 200;
		Point[4][1] = 130;
		Point[5][0] = 120;
		Point[5][1] = 110;
		Point[6][0] = 200;
		Point[6][1] = 90;
		Color =  i51KitG2COLOR(0XF0, 0X0F, 0XF0);
		i51KitG2FillPolygon( VerticeNumber, Point, Color);
		
		VerticeNumber = 7;
		Point[0][0] = 10;
		Point[0][1] = 300;
		Point[1][0] = 40;
		Point[1][1] = 200;
		Point[2][0] = 70;
		Point[2][1] = 250;
		Point[3][0] = 90;
		Point[3][1] = 201;
		Point[4][0] = 110;
		Point[4][1] = 251;
		Point[5][0] = 130;
		Point[5][1] = 205;
		Point[6][0] = 150;
		Point[6][1] = 300;
		Color =  i51KitG2COLOR(0XF0, 0X0F, 0X0F);
		i51KitG2DrawPolygon( VerticeNumber, Point, Color);

		Point[0][0] = 70;
		Point[0][1] = 130;
		Point[1][0] = 120;
		Point[1][1] = 210;
		Point[2][0] = 170;
		Point[2][1] = 130;
		Color =  i51KitG2COLOR(0X0F, 0X0F, 0XFF);
		i51KitG2FillArc( Point, Color);
		i51KitG2DrawArc( Point, 0);

		Color =  i51KitG2COLOR(0XFF, 0X0F, 0X0F);
		i51KitG2DrawCircle( 50, 100, 30, Color);
		Color =  i51KitG2COLOR(0X0F, 0XFF, 0X0F);
		i51KitG2FillCircle( 50, 160, 30, Color);
	}		
}


void TestInit(void)
{
	iU16 * Path = L"i51KitG2\\i51IMIC";
	static iFILE  Test ;
//	iU32 ImageId[15] = {BG,RUN_LEFT,RUN_RIGHT,WALL,BEE,LEAF,
//	ORANGE,PINEAPPLE,STRAWBERRY,WATERMELON,APPLE,	BOMB,FRUIT_BG1,FRUIT_BG2,REDBLUE_FISH
//	};
	iU32 ImageId[9] = {BG,RUN_LEFT,RUN_RIGHT,WALL,BEE,SPRITE,prizenum,shayu,test1};
	
	if( FLAG_FIRSTRUN)
	{
		FLAG_FIRSTRUN = iFALSE;
		if( i51AdeOsFileIsExist(L"i51KitG2\\i51IMIC") )
		{
		       if(iFALSE==i51KitG2CheckImageContainer(L"i51KitG2\\i51IMIC")) return;
			   
			CurrentImageContainer = i51KitG2OpenImageContainer(Path);
//			i51KitG2SetImageMemoryCopy(CurrentImageContainer, iNULL, iNULL);
//			if(ScreenWidth>240) ImageId[12] = ImageId[13];
			DecodeComplete = iTRUE;
		}
		else
		{
			Test = i51KitStdResOpen ( L"i51KitG2\\i51KitG2.re" ) ;
			i51KitG2CreateImageContainer ( Test , ImageId , 9 , Path ,  Process , Done );
		}
//i51KernelSetCache(L"i51KitG2");
	}
}

void TestImage( void )
{
	iU16 * Path = L"i51KitG2\\i51IMIC";
	static iFILE  Test ;
	iRECT DesRect;
	iRECT SrcRect;

	if(FLAG_FIRSTRUN)    TestInit();

	if(DecodeComplete)
	{
//		i51KitG2DrawImage(CurrentImageContainer,BG, iNULL, iNULL);
//		i51KitG2SetAlpha(128);
//		i51KitG2DrawImage(CurrentImageContainer,shayu, iNULL, iNULL);
//		i51KitG2SetAlpha(0XFF);
//		i51KitG2MirrorImage(CurrentImageContainer,shayu, iNULL, iNULL);
//		i51KitG2FlipImage(CurrentImageContainer,shayu, iNULL, iNULL);
i51KitG2DrawImage( CurrentImageContainer, 0, 0, 0);
SrcRect.X = -20;
SrcRect.Y = 100;
SrcRect.Width = 240;
SrcRect.Height = 150;
i51KitG2DrawImage( CurrentImageContainer, 0, &SrcRect, 0);
SrcRect.Y = 150;
i51KitG2MirrorImage( CurrentImageContainer, 0, &SrcRect, 0);
SrcRect.X = 260;
SrcRect.Y = 200;
SrcRect.Width = 240;
SrcRect.Height = 150;
i51KitG2DrawImage( CurrentImageContainer, 0, &SrcRect, 0);
SrcRect.Y = 250;
i51KitG2MirrorImage( CurrentImageContainer, 0, &SrcRect, 0);
//		i51KitG2RotateImage(CurrentImageContainer,shayu, iNULL, &SrcRect,50,85,120,160,0,45);
//		i51KitG2SetAlpha(0XFF);
//		i51KitG2FlipImage(CurrentImageContainer,3, iNULL, iNULL);
//		i51KitG2MirrorImage(CurrentImageContainer,3, iNULL, iNULL);
		DesRect.X = 42;
		DesRect.Y = 50;
		DesRect.Width = 156;
		DesRect.Height = 196;
//		i51KitG2MirrorImage(CurrentImageContainer,1, &DesRect, iNULL);
//		i51KitG2MirrorImage(CurrentImageContainer,7, iNULL, iNULL);
//		i51KitG2DrawImage(CurrentImageContainer,CurrentImageContainer,2, &DesRect, iNULL);

		DesRect.X = 80;
		DesRect.Y = 0;
		DesRect.Width= ScreenWidth;
		DesRect.Height= ScreenHeight;
//		i51KitG2FlipImage(CurrentImageContainer,7, &DesRect, iNULL);
//		i51KitG2FlipImage(CurrentImageContainer,7,iNULL, iNULL);

		DesRect.X = 0;
		DesRect.Y = 280;
//		i51KitG2SetRotateCenter(DesRect.X, DesRect.Y);
//		i51KitG2RotateImage(CurrentImageContainer,1, &DesRect , iNULL, 45);
//i51KitG2DrawImage(CurrentImageContainer, test1, iNULL, iNULL);
		
		i51AdeMmiUpdateScreen  () ;
//		DecodeComplete = iFALSE;

	}
}

void TestVP( void)
{
	iRECT Rect, SrcRect, DesRect;
	static iS32 VP_X1 =0, VP_Y1 = 0;
	static iS32 VP_X2 =0, VP_Y2 = 0;
	static iS32 VP_X3 =0, VP_Y3 = 0;
	static iS16 SpikeX = 5, SpikeY = 5;
	static iS16 DrawImage = iFALSE;
	static iS16 SymbolNumber1= 0;
	static iS16 SymbolNumber2= 0;
	static iS16 BeeDir= 0;
	i51ImageIndex * ImageIndexTable = iNULL;
	i51ImageContainer * ImageContainer = iNULL;
	iS32 iTemp =0;	

	if(FLAG_FIRSTRUN)    TestInit();
	
	memset ( LcdBuffer , 0XFF , 240*320*2 ) ;

	if(DecodeComplete)
	{
		i51KitG2VPEnable();
		Rect.X = 0;
		Rect.Y = 0;
		Rect.Width = 240;
		Rect.Height = 640;
		VP_BK = i51KitG2VPCreate(2, &Rect);
		i51KitG2VPSetActivity(VP_BK);
//		i51KitG2SetVPiSLoop(VP_BK, iTRUE);
//    i51KitG2SetAlpha(0);
		i51KitG2DrawImage(CurrentImageContainer ,BG, iNULL, iNULL);
		DesRect.X = 0;
		DesRect.Y = 320;
		DesRect.Width = 240;
		DesRect.Height = 320;
		i51KitG2DrawImage(CurrentImageContainer ,BG, &DesRect, iNULL);

//    i51KitG2SetAlpha(150);
		Rect.X = 0;
		Rect.Y = 0;
		Rect.Width = 240;
		Rect.Height = 480;
		VP_Road = i51KitG2VPCreate(6, &Rect);
		i51KitG2VPSetActivity(VP_Road);
//		i51KitG2SetVPiSLoop(VP_Road, iTRUE);
		SrcRect.X = 0;
		SrcRect.Y = 0;
		SrcRect.Width = 24;
		SrcRect.Height = 160;
		DesRect.X = 0;
		DesRect.Y = 0;
		DesRect.Width = 24;
		DesRect.Height = 160;
		i51KitG2DrawImage(CurrentImageContainer ,WALL, &DesRect, &SrcRect);
		DesRect.Y = 160;
		i51KitG2DrawImage(CurrentImageContainer ,WALL, &DesRect, &SrcRect);
		DesRect.Y = 320;
		i51KitG2DrawImage(CurrentImageContainer ,WALL, &DesRect, &SrcRect);
		SrcRect.X = 24;
		DesRect.X = 216;
		i51KitG2DrawImage(CurrentImageContainer ,WALL, &DesRect, &SrcRect);
		DesRect.Y = 160;
		i51KitG2DrawImage(CurrentImageContainer ,WALL, &DesRect, &SrcRect);
		DesRect.Y = 0;
		i51KitG2DrawImage(CurrentImageContainer ,WALL, &DesRect, &SrcRect);

		VP_Snidget = i51KitG2VPCreate(2, &Rect);
		i51KitG2VPSetActivity(VP_Snidget);
		SrcRect.X = 0;
		SrcRect.Y = 0;
		SrcRect.Width = 25;
		SrcRect.Height = 39;
		DesRect.X = 24;
		DesRect.Y = 250;
		DesRect.Width = 25;
		DesRect.Height = 39;
		i51KitG2DrawImage(CurrentImageContainer ,RUN_LEFT, &DesRect, &SrcRect);
		DesRect.X = 191;
		i51KitG2DrawImage(CurrentImageContainer ,RUN_RIGHT, &DesRect, &SrcRect);

		Rect.X = 70;
		Rect.Y = 319;
		Rect.Width = 111;
		Rect.Height = 71;
		VP_Y3 = 319;
		VP_X3 = 70;
		VP_Bee= i51KitG2VPCreate(2, &Rect);
		i51KitG2VPSetActivity(VP_Bee);
		SrcRect.X = 0;
		SrcRect.Y = 0;
		SrcRect.Width = 111;
		SrcRect.Height = 71;
		DesRect.X = 0;
		DesRect.Y = 0;
		DesRect.Width = 25;
		DesRect.Height = 39;
		i51KitG2DrawImage(CurrentImageContainer ,BEE, &DesRect, &SrcRect);

		i51KitG2VPRender( VP_BK,iNULL);
		FLAG_FIRSTRUN = iFALSE;
		DecodeComplete = iFALSE;
		DrawImage = iTRUE;
	}
if(DrawImage)
{
//    i51KitG2SetAlpha(0);
	DesRect.X = 0;
	DesRect.Y= 0;
	DesRect.Width= 240;
	DesRect.Height= 320;
#if 1	
	i51KitG2VPSetCoord(VP_BK, VP_X1, VP_Y1);
	i51KitG2VPRender( VP_BK,&DesRect);
	VP_Y1 += 2;
	if( (VP_Y1>=0) || VP_Y1<-320)
	{
//		SpikeY= -SpikeY;
		VP_Y1 = -320;
	}
#endif
#if 1
	i51KitG2VPSetCoord(VP_Road, VP_X2, VP_Y2);
	i51KitG2VPRender( VP_Road,&DesRect);

	VP_Y2 += 10;
	if( (VP_Y2>=0) || VP_Y2<-160)
	{
//		SpikeY= -SpikeY;
		VP_Y2 = -160;
	}
//  i51KitG2SetAlpha(150);
	i51KitG2VPRender( VP_Snidget,&DesRect);
	i51KitG2VPClean(VP_Snidget);
	i51KitG2VPSetActivity(VP_Snidget);
	SymbolNumber1 = (SymbolNumber1+1)%4;
	SrcRect.X = 0;
	SrcRect.Y = SymbolNumber1*39;
	SrcRect.Width = 25;
	SrcRect.Height = 39;
	DesRect.X = 24;
	DesRect.Y = 250;
	DesRect.Width = 25;
	DesRect.Height = 39;
	i51KitG2DrawImage(CurrentImageContainer ,RUN_LEFT, &DesRect, &SrcRect);
	DesRect.X = 191;
	i51KitG2DrawImage(CurrentImageContainer ,RUN_RIGHT, &DesRect, &SrcRect);

	DesRect.X = VP_X3;
	DesRect.Y = VP_Y3;
	DesRect.Width = 111;
	DesRect.Height = 71;
	i51KitG2VPRender( VP_Bee,&DesRect);
	i51KitG2VPClean(VP_Bee);
	i51KitG2VPSetActivity(VP_Bee);
	SymbolNumber2 = (SymbolNumber2+1)%2;
	SrcRect.X = 0;
	SrcRect.Y = SymbolNumber2*71;
	SrcRect.Width = 111;
	SrcRect.Height = 71;
	i51KitG2VPSetCoord(VP_Bee, VP_X3, VP_Y3);
	if(BeeDir)
	{
		i51KitG2FlipImage(CurrentImageContainer ,4, iNULL, &SrcRect);
		VP_Y3 += 8;
	}
	else
	{
		i51KitG2DrawImage(CurrentImageContainer ,4, iNULL, &SrcRect);
		VP_Y3 -= 8;
	}
	
	if( VP_Y3>=320 )
	{
		VP_Y3 -= 8;
		BeeDir = 0;
	}
	if(VP_Y3<= -71)
	{
		VP_Y3 += 8;
		BeeDir = 1;
	}
#endif
	i51KitG2Update();
}
}

 void MGdrawChars(char* data, int offset, int length, int x, int y, int style) {
  iRECT drawrec;
 // DDRect desrect,tmp;
//   DDFont font=getFontStyle(style);
  drawrec.X=x;
  drawrec.Y=y;
  drawrec.Width=ScreenWidth;
  drawrec.Height=ScreenHeight;
/*
  GetCollideRect(&tmp,&rectClip,&desrect);
  CopyMyRectToI51Rect(&desrect,& drawrec);*/


 
	i51KitG2FontDraw(TypeheadHandle,0,(void *)(data+offset), length,&drawrec, &drawrec,
		0,0,0, 0XFF00, 0, 0);


}

void TestFontDraw(void)
{
char * Str = "VRFFFFFFFFFFFHJJAJJUKRRDTWWRRRHHHHHHHHHHHHHHHHHHH";
int i;
i51KitG2CleanScreen(0, 0);
for( i=0; i<strlen(Str); i++ )
{
	MGdrawChars(Str, i, 1, i*14, (i/15)*16, 0);
}
i51KitG2Update();

#if 0
    static iCOLOR Color=0;
    iRECT DrawRect, SrcRect;
    iRECT * RectTemp = iNULL;
    static iHANDLE VP1 = iNULL;
    static iS16 X = 0,Y = 240;
    static iU32 OldTime = 0;	
    static iU8 State = 4;	
//    iU16 * Str = (iU16 *)"深圳豆游网络科技";
//    iU16 * Str = (iU16 *)"我的一是在不了有和人这中大为上个国";
//    iU16 * Str = (iU16 *)"豆游科技豆游科技豆游科技";
//    iU16 * Str = (iU16 *)"的一是在不了有和人这中大为上个国我以要他时来用们生到作地于出就分对成会可主发年动同工也能下过子说产种面而方后多定行学法所民得经十三之进着等部度家电力里如水化高自二理起小物现实加量都两体制机当使点从业本去把性好应开它合还因由其些然前外天政四日那社义事平形相全表间样与关各重新线内数正心反你明看原又么利比或但质气第向道命此变条只没结解问意建月公无系军很情者最立代想已通并提直题党程展五果料象员革位入常文总次品式活设及管特件长求老头基资边流路级少图山统接知较将组见计别她手角期根论运农指几九区强放决西被干做必战先回则任取据处队南给色光门即保治北造百规热领七海口东导器压志世金增争济阶油思术极交受联什认六共权收证改清己美再采转更单风切打白教速花带安场身车例真务具万每目至达走积示议声报斗完类八离华名确才科张信马节话米整空元况今集温传土许步群广石记需段研界拉林律叫且究观越织装影算低持音众书布复容儿须际商非验连断深难近矿千周委素技备半办青省列习响约支般史感劳便团往酸历市克何除消构府称太准精值号率族维划选标写存候毛亲快效斯院查江型眼王按格养易置派层片始却专状育厂京识适属圆包火住调满县局照参红细引听该铁价严";
//    iU16 * Str = (iU16 *)"的一是在不了有和人这中大为上个国我以要他时来用们生到作地于出就分对成会可主发年动同工也能下过子说产种面而方后多定行学法所民得经十三之进着等部度家电力里如水化高自二理起小物现实加量都两体制机当使点从业本去把性好应开它合还因由其些然前外天政四日那社义事平形相全表间样与关各重新线内数正心反你明看原又么利比或但质气第向道命此变条只没结解问意建月公无系军很情者最立代想已通并提直题党程展五果料象员革位入常文总次品式活设及管特件长求老头基资边流路级少图山统接知较将组见计别她手角期根论运农指几九区强放决西被干做必战先回则任取据处队南给色光门即保治北造百规热领七海口东导器压志世金增争济阶油思术极交受联什认六共权收证改清己美再采转更单风切打白教速花带安场身车例真务具万每目至达走积示议声报斗完类八离华名确才科张信马节话米整空元况今集温传土许步群广石记需段研界拉林律叫且究观越织装影算低持音众书布复容儿须际商非验连断深难近矿千周委素技备半办青省列习响约支般史感劳便团往酸历市克何除消构府称太准精值号率族维划选标写存候毛亲快效斯院查江型眼王按格养易置派层片始却专状育厂京识适属圆包火住调满县局照参红细引听该铁价严";
iU16 * Str = (iU16 *)"  1967年,里奇参与的第一个项目是multics,\
这是一个操作系统项目,被设计在大型计算机主机上运行,\
但是由于整个目标过于庞大,糅合了太多的特性,multics的性能很低,\
最终以失败而告终.multics项目的开发者之一肯.汤普逊则继续为\
ge-5开发软件,并最终编写了一个新操作系统项目,\
能够支持同时的多用户操作,也就是后来的unix.";
//   iCHAR * Str = "6.strlen的结果要在运行的时候才能计算出来,是用来计算字符串的长度,不是类型占内存的大小.";
//    iU16 * Str = (iU16 *)"里奇参与的第一个项目!!!!!!!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}~";
//    iU16 * Str = (iU16 *)"里奇参与的第一个项目!!!!!!!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}~";
//    iU16 * Str = (iU16 *)"Designed to give you the finest document-formatting tools, Word 2010 also helps you easily organize and write your documents more efficiently, and stay within reach so you can capture your best ideas whenever and wherever they occur.";
    iU8 i;	

if(FLAG_FIRSTRUN)
{
//    i51KitG2FontSetUse(i51KitStdResOpen(L"Test\\i51KitG2.re"), 1);
    FLAG_FIRSTRUN = iFALSE;
}
//    i51KitG2FontCreat((void *)Str, L"Fonts\\", L"SmallFive.i51F",L"Fonts\\", L"Creat.i51F");
//    i51AdeMmiUpdateScreen();
//    return ;
    DrawRect.X = 10;
    DrawRect.Y = 20;
    DrawRect.Width = 120;
    DrawRect.Height= 220;

    SrcRect.X = 10;
    SrcRect.Y = 220;
    SrcRect.Width= 120;
    SrcRect.Height= 220;
	
#if 0
if(VP1==iNULL)
{
    VP1 = i51KitG2VPCreate(4, &DrawRect);
    i51KitG2VPSetActivity(VP1);
    DrawRect.Y = 20;
    DrawRect.Height= 55;
    i51KitG2FontDraw(TypeheadHandle,0,(void *)Str, 0, &DrawRect, 0,2,2, 0XF000, 0XFF00, i51KITG2_FONT_UNDERLINE|i51KITG2_FONT_BKCOLOR);
    DrawRect.Y = 80;
    DrawRect.Height= 75;
    i51KitG2FontDraw(TypeheadHandle,1,(void *)Str, 0, &DrawRect, 0,2,2, 0X0F00, 0XFF00, i51KITG2_FONT_UNDERLINE|i51KITG2_FONT_BKCOLOR);
    DrawRect.Y = 160;
    DrawRect.Height= 85;
    i51KitG2FontDraw(TypeheadHandle,2,(void *)Str, 0, &DrawRect, 0,2,2, 0X00F0, 0XFF00, i51KITG2_FONT_UNDERLINE|i51KITG2_FONT_BKCOLOR);
    DrawRect.Y = 250;
    i51KitG2FontDraw(TypeheadHandle,2,(void *)Str, 0, &DrawRect, 0,4,4, 0X00F0, 0XFF00, i51KITG2_FONT_UNDERLINE|i51KITG2_FONT_BKCOLOR);
}


    DrawRect.Y = 20;
//    i51KitG2FontDraw((void *)Str, 66, &DrawRect, 0, Color+500, 0XFF00, i51KITG2_FONT_UNDERLINE|i51KITG2_FONT_BKCOLOR);
    DrawRect.Y = 40;
//    i51KitG2FontDraw( TypeheadHandle , 0,(void *)Str, 0, &DrawRect, 2,2,0, Color+500, 0XFF00, i51KITG2_FONT_UNDERLINE|i51KITG2_FONT_BKCOLOR);
    memset ( LcdBuffer , 0XFFFF , 240*320*2 ) ;
    i51KitG2VPRender(VP1, iNULL);
i51KitG2VPSetCoord(VP1,X,Y);
X+= 10;
Y+= 10;
if(X>220) X =0;
if(Y>300) Y =0;
#endif
i51KitG2VPDisable();
RectTemp = &SrcRect;
if(OldTime==0||(i51AdeOsGetTick()-OldTime>=10))
{
	OldTime = i51AdeOsGetTick();
	switch(State)
	{
		case 0:
			Y -= 1;
			if( Y<-180 )
			{
				Y = 20;

				X = 130;	
				State = 1;
			}
			break;
		case 1:	
			X -= 1;
			if( X<-110 )
			{
				Y = -180;

				X = 10;	
				State = 2;
			}
			break;
		case 2:	
			Y += 1;
			if( Y>240 )
			{
				Y = 20;

				X = -110;	
				State = 3;
			}
			break;
		case 3:	
			X += 1;
			if( X>110 )
			{
				X = 110;
				Y = 240;
				State = 5;
			}
			break;
		case 4:
			RectTemp = iNULL;
			X += 1;
			if( X>110 )
			{
				Y =240;

				X = 10;	
				State = 0;
			}
			break;
		case 5:
			X--;
			Y--;
			if(X<-110)
			{
				X = -110;
				Y = -180;
				State = 6;
			}
			break;
		case 6:
			X++;
			Y++;
			if(X>110)
			{
				X = -110;
				Y = 240;
				State = 7;
			}
			break;
		case 7:
			X++;
			Y--;
			if(X>110)
			{
				X = 110;
				Y = -180;
				State = 8;
			}
			break;
		case 8:
			X--;
			Y++;
			if(X<10)
			{
				X = 0;
				State = 4;
			}
			break;
		default : break;	
			
	}
SrcRect.X = X;
SrcRect.Y = Y;
DrawRect.X = 0;
DrawRect.Y = 0;
DrawRect.Width = 120;
DrawRect.Height = 160;
i51KitG2CleanScreen(i51KitG2COLOR(255,0,0),&DrawRect);
DrawRect.X = 120;
i51KitG2CleanScreen(i51KitG2COLOR(0,255,0),&DrawRect);
DrawRect.X = 0;
DrawRect.Y = 160;
i51KitG2CleanScreen(31,&DrawRect);
DrawRect.X = 120;
i51KitG2CleanScreen(i51KitG2COLOR(0,0,240),&DrawRect);

i51KitG2CleanScreen(30,0);

//i51KitG2CleanScreen(0X0,0);
//i51KitG2FontDraw(TypeheadHandle,0,(void *)Str, 0, &DrawRect,RectTemp, i51KITG2_FONT_WORDS_ROTATE_0,0,0, 0XFF00, 31, i51KITG2_FONT_UNDERLINE|i51KITG2_FONT_BKCOLOR);
//i51KitG2FontDraw(TypeheadHandle,0,(void *)Str, 0, &DrawRect,RectTemp, i51KITG2_FONT_WORDS_ROTATE_0,0,0, 0XFF00, 31, i51KITG2_FONT_UNDERLINE|i51KITG2_FONT_BKCOLOR);
i51KitG2Update();

}
#endif	
}


void TestDrawTypehead( void )
{
    iU8 Typehead[224] = 
    {
    /*--  文字:  深  --*/
    /*--  宋体11;  此字体下对应的点阵为：宽x高=14x14   --*/
    /*--  宽度不是8的倍数，现调整为：宽度x高度=16x14  --*/
    0x8F,0xF8,0x68,0x08,0x29,0x48,0x82,0x20,0x44,0x90,0x00,0x80,0x2F,0xF8,0x41,0xC0,
    0x42,0xA0,0x84,0x90,0x98,0x88,0x80,0x80,0x80,0x80,0x00,0x00,
    
    /*--  文字:  圳  --*/
    /*--  宋体11;  此字体下对应的点阵为：宽x高=14x14   --*/
    /*--  宽度不是8的倍数，现调整为：宽度x高度=16x14  --*/
    0x24,0x10,0x24,0x90,0x24,0x90,0x24,0x90,0xFC,0x90,0x24,0x90,0x24,0x90,0x24,0x90,
    0x3C,0x90,0xE4,0x90,0x08,0x90,0x08,0x10,0x10,0x10,0x00,0x00,
    
    /*--  文字:  豆  --*/
    /*--  宋体11;  此字体下对应的点阵为：宽x高=14x14   --*/
    /*--  宽度不是8的倍数，现调整为：宽度x高度=16x14  --*/
    0x7F,0xF0,0x00,0x00,0x00,0x00,0x3F,0xE0,0x20,0x20,0x20,0x20,0x20,0x20,0x3F,0xE0,
    0x00,0x00,0x10,0x40,0x08,0x80,0x08,0x80,0xFF,0xF8,0x00,0x00,
    
    /*--  文字:  游  --*/
    /*--  宋体11;  此字体下对应的点阵为：宽x高=14x14   --*/
    /*--  宽度不是8的倍数，现调整为：宽度x高度=16x14  --*/
    0x48,0x40,0x24,0x78,0x1E,0x80,0x89,0x00,0x48,0xF8,0x0E,0x10,0x1A,0x20,0x2B,0xF8,
    0xCA,0x20,0x4A,0x20,0x52,0x20,0x52,0xA0,0x64,0x40,0x00,0x00,
    
    /*--  文字:  网  --*/
    /*--  宋体11;  此字体下对应的点阵为：宽x高=14x14   --*/
    /*--  宽度不是8的倍数，现调整为：宽度x高度=16x14  --*/
    0x00,0x00,0x7F,0xF0,0x42,0x30,0x63,0x50,0x55,0x50,0x59,0x50,0x48,0x90,0x55,0x50,
    0x55,0x30,0x65,0x10,0x62,0x10,0x44,0x50,0x40,0x20,0x00,0x00,
    
    /*--  文字:  络  --*/
    /*--  宋体11;  此字体下对应的点阵为：宽x高=14x14   --*/
    /*--  宽度不是8的倍数，现调整为：宽度x高度=16x14  --*/
    0x21,0x00,0x41,0xF0,0x42,0x20,0x95,0x20,0xF1,0x40,0x20,0x80,0x41,0x60,0x96,0x18,
    0xE3,0xF0,0x02,0x10,0x32,0x10,0xC3,0xF0,0x02,0x10,0x00,0x00,
    
    /*--  文字:  科  --*/
    /*--  宋体11;  此字体下对应的点阵为：宽x高=14x14   --*/
    /*--  宽度不是8的倍数，现调整为：宽度x高度=16x14  --*/
    0x1C,0x20,0xF1,0x20,0x10,0xA0,0x10,0x20,0xFD,0x20,0x10,0xA0,0x38,0x38,0x57,0xE0,
    0x50,0x20,0x90,0x20,0x10,0x20,0x10,0x20,0x10,0x20,0x00,0x00,
    
    /*--  文字:  技  --*/
    /*--  宋体11;  此字体下对应的点阵为：宽x高=14x14   --*/
    /*--  宽度不是8的倍数，现调整为：宽度x高度=16x14  --*/
    0x20,0x80,0x20,0x80,0x27,0xF8,0xF8,0x80,0x20,0x80,0x27,0xF0,0x32,0x10,0x62,0x20,
    0xA1,0x40,0x20,0x80,0x21,0x40,0xA2,0x20,0x4C,0x18,0x00,0x00,
    };

iU8 NumberTypehead[]=
{
/*--  文字:  0  --*/
/*--  新宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
0x00,0x00,0x70,0x88,0x88,0x88,0x88,0x88,0x88,0x70,0x00,0x00,

/*--  文字:  1  --*/
/*--  新宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
0x00,0x00,0x20,0x60,0x20,0x20,0x20,0x20,0x20,0x70,0x00,0x00,

/*--  文字:  2  --*/
/*--  新宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
0x00,0x00,0x70,0x88,0x88,0x10,0x20,0x40,0x80,0xF8,0x00,0x00,

/*--  文字:  3  --*/
/*--  新宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
0x00,0x00,0x70,0x88,0x08,0x30,0x08,0x08,0x88,0x70,0x00,0x00,

/*--  文字:  4  --*/
/*--  新宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
0x00,0x00,0x10,0x30,0x50,0x50,0x90,0x78,0x10,0x18,0x00,0x00,

/*--  文字:  5  --*/
/*--  新宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
0x00,0x00,0xF8,0x80,0x80,0xF0,0x08,0x08,0x88,0x70,0x00,0x00,

/*--  文字:  6  --*/
/*--  新宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
0x00,0x00,0x70,0x90,0x80,0xF0,0x88,0x88,0x88,0x70,0x00,0x00,

/*--  文字:  7  --*/
/*--  新宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
0x00,0x00,0xF8,0x90,0x10,0x20,0x20,0x20,0x20,0x20,0x00,0x00,

/*--  文字:  8  --*/
/*--  新宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
0x00,0x00,0x70,0x88,0x88,0x70,0x88,0x88,0x88,0x70,0x00,0x00,

/*--  文字:  9  --*/
/*--  新宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
0x00,0x00,0x70,0x88,0x88,0x88,0x78,0x08,0x48,0x70,0x00,0x00,

};

iU8 CutFruit_logo[] = {
	/*--  文字:  A  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x20,0x20,0x30,0x50,0x50,0x78,0x48,0xCC,0x00,0x00,

	/*--  文字:     --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

	/*--  文字:  s  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x78,0x40,0x30,0x08,0x78,0x00,0x00,

	/*--  文字:  i  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x20,0x00,0x00,0x60,0x20,0x20,0x20,0x70,0x00,0x00,

	/*--  文字:  m  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0xF0,0xA8,0xA8,0xA8,0xA8,0x00,0x00,

	/*--  文字:  p  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0xF0,0x48,0x48,0x48,0x70,0x40,0xE0,

	/*--  文字:  l  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0xE0,0x20,0x20,0x20,0x20,0x20,0x20,0xF8,0x00,0x00,

	/*--  文字:  e  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x30,0x48,0x78,0x40,0x38,0x00,0x00,

	/*--  文字:     --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

	/*--  文字:  c  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x38,0x48,0x40,0x40,0x38,0x00,0x00,

	/*--  文字:  a  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x30,0x48,0x38,0x48,0x3C,0x00,0x00,

	/*--  文字:  s  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x78,0x40,0x30,0x08,0x78,0x00,0x00,

	/*--  文字:  u  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0xD8,0x48,0x48,0x48,0x3C,0x00,0x00,

	/*--  文字:  a  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x30,0x48,0x38,0x48,0x3C,0x00,0x00,

	/*--  文字:  l  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0xE0,0x20,0x20,0x20,0x20,0x20,0x20,0xF8,0x00,0x00,

	/*--  文字:     --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

	/*--  文字:  g  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x3C,0x48,0x30,0x40,0x78,0x44,0x38,

	/*--  文字:  a  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x30,0x48,0x38,0x48,0x3C,0x00,0x00,

	/*--  文字:  m  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0xF0,0xA8,0xA8,0xA8,0xA8,0x00,0x00,

	/*--  文字:  e  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x30,0x48,0x78,0x40,0x38,0x00,0x00,

	/*--  文字:  .  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x40,0x00,0x00,

	/*--  文字:     --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

	/*--  文字:  T  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0xF8,0xA8,0x20,0x20,0x20,0x20,0x20,0x70,0x00,0x00,

	/*--  文字:  h  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0xC0,0x40,0x40,0x70,0x48,0x48,0x48,0xEC,0x00,0x00,

	/*--  文字:  e  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x30,0x48,0x78,0x40,0x38,0x00,0x00,

	/*--  文字:  r  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0xD8,0x60,0x40,0x40,0xE0,0x00,0x00,

	/*--  文字:  e  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x30,0x48,0x78,0x40,0x38,0x00,0x00,

	/*--  文字:     --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

	/*--  文字:  i  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x20,0x00,0x00,0x60,0x20,0x20,0x20,0x70,0x00,0x00,

	/*--  文字:  s  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x78,0x40,0x30,0x08,0x78,0x00,0x00,

	/*--  文字:     --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

	/*--  文字:  o  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x30,0x48,0x48,0x48,0x30,0x00,0x00,

	/*--  文字:  n  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0xF0,0x48,0x48,0x48,0xEC,0x00,0x00,

	/*--  文字:  l  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0xE0,0x20,0x20,0x20,0x20,0x20,0x20,0xF8,0x00,0x00,

	/*--  文字:  y  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0xEC,0x48,0x50,0x30,0x20,0x20,0xC0,

	/*--  文字:     --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

	/*--  文字:  o  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x30,0x48,0x48,0x48,0x30,0x00,0x00,

	/*--  文字:  n  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0xF0,0x48,0x48,0x48,0xEC,0x00,0x00,

	/*--  文字:  e  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x30,0x48,0x78,0x40,0x38,0x00,0x00,

	/*--  文字:     --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

	/*--  文字:  p  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0xF0,0x48,0x48,0x48,0x70,0x40,0xE0,

	/*--  文字:  u  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0xD8,0x48,0x48,0x48,0x3C,0x00,0x00,

	/*--  文字:  r  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0xD8,0x60,0x40,0x40,0xE0,0x00,0x00,

	/*--  文字:  p  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0xF0,0x48,0x48,0x48,0x70,0x40,0xE0,

	/*--  文字:  o  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x30,0x48,0x48,0x48,0x30,0x00,0x00,

	/*--  文字:  s  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x78,0x40,0x30,0x08,0x78,0x00,0x00,

	/*--  文字:  e  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x30,0x48,0x78,0x40,0x38,0x00,0x00,

	/*--  文字:     --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

	/*--  文字:  -  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0xF8,0x00,0x00,0x00,0x00,0x00,0x00,

	/*--  文字:  -  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0xF8,0x00,0x00,0x00,0x00,0x00,0x00,

	/*--  文字:     --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

	/*--  文字:  t  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x20,0x20,0x70,0x20,0x20,0x20,0x18,0x00,0x00,

	/*--  文字:  o  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x30,0x48,0x48,0x48,0x30,0x00,0x00,

	/*--  文字:     --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

	/*--  文字:  c  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x38,0x48,0x40,0x40,0x38,0x00,0x00,

	/*--  文字:  u  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0xD8,0x48,0x48,0x48,0x3C,0x00,0x00,

	/*--  文字:  t  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x20,0x20,0x70,0x20,0x20,0x20,0x18,0x00,0x00,

	/*--  文字:     --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

	/*--  文字:  f  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x1C,0x20,0x20,0x78,0x20,0x20,0x20,0x78,0x00,0x00,

	/*--  文字:  r  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0xD8,0x60,0x40,0x40,0xE0,0x00,0x00,

	/*--  文字:  u  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0xD8,0x48,0x48,0x48,0x3C,0x00,0x00,

	/*--  文字:  i  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x20,0x00,0x00,0x60,0x20,0x20,0x20,0x70,0x00,0x00,

	/*--  文字:  t  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x20,0x20,0x70,0x20,0x20,0x20,0x18,0x00,0x00,

	/*--  文字:  s  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x78,0x40,0x30,0x08,0x78,0x00,0x00,

	/*--  文字:  !  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x20,0x20,0x20,0x20,0x20,0x20,0x00,0x20,0x00,0x00,

	/*--  文字:     --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

	/*--  文字:  T  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0xF8,0xA8,0x20,0x20,0x20,0x20,0x20,0x70,0x00,0x00,

	/*--  文字:  h  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0xC0,0x40,0x40,0x70,0x48,0x48,0x48,0xEC,0x00,0x00,

	/*--  文字:  e  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x30,0x48,0x78,0x40,0x38,0x00,0x00,

	/*--  文字:     --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

	/*--  文字:  s  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x78,0x40,0x30,0x08,0x78,0x00,0x00,

	/*--  文字:  c  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x38,0x48,0x40,0x40,0x38,0x00,0x00,

	/*--  文字:  r  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0xD8,0x60,0x40,0x40,0xE0,0x00,0x00,

	/*--  文字:  e  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x30,0x48,0x78,0x40,0x38,0x00,0x00,

	/*--  文字:  e  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x30,0x48,0x78,0x40,0x38,0x00,0x00,

	/*--  文字:  n  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0xF0,0x48,0x48,0x48,0xEC,0x00,0x00,

	/*--  文字:     --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

	/*--  文字:  w  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0xA8,0xA8,0x70,0x50,0x50,0x00,0x00,

	/*--  文字:  i  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x20,0x00,0x00,0x60,0x20,0x20,0x20,0x70,0x00,0x00,

	/*--  文字:  l  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0xE0,0x20,0x20,0x20,0x20,0x20,0x20,0xF8,0x00,0x00,

	/*--  文字:  l  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0xE0,0x20,0x20,0x20,0x20,0x20,0x20,0xF8,0x00,0x00,

	/*--  文字:     --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

	/*--  文字:  c  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x38,0x48,0x40,0x40,0x38,0x00,0x00,

	/*--  文字:  o  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x30,0x48,0x48,0x48,0x30,0x00,0x00,

	/*--  文字:  n  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0xF0,0x48,0x48,0x48,0xEC,0x00,0x00,

	/*--  文字:  t  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x20,0x20,0x70,0x20,0x20,0x20,0x18,0x00,0x00,

	/*--  文字:  i  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x20,0x00,0x00,0x60,0x20,0x20,0x20,0x70,0x00,0x00,

	/*--  文字:  n  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0xF0,0x48,0x48,0x48,0xEC,0x00,0x00,

	/*--  文字:  u  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0xD8,0x48,0x48,0x48,0x3C,0x00,0x00,

	/*--  文字:  e  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x30,0x48,0x78,0x40,0x38,0x00,0x00,

	/*--  文字:     --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

	/*--  文字:  t  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x20,0x20,0x70,0x20,0x20,0x20,0x18,0x00,0x00,

	/*--  文字:  o  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x30,0x48,0x48,0x48,0x30,0x00,0x00,

	/*--  文字:     --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

	/*--  文字:  j  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x10,0x00,0x00,0x30,0x10,0x10,0x10,0x10,0x10,0xE0,

	/*--  文字:  u  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0xD8,0x48,0x48,0x48,0x3C,0x00,0x00,

	/*--  文字:  m  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0xF0,0xA8,0xA8,0xA8,0xA8,0x00,0x00,

	/*--  文字:  p  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0xF0,0x48,0x48,0x48,0x70,0x40,0xE0,

	/*--  文字:     --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

	/*--  文字:  o  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x30,0x48,0x48,0x48,0x30,0x00,0x00,

	/*--  文字:  u  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0xD8,0x48,0x48,0x48,0x3C,0x00,0x00,

	/*--  文字:  t  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x20,0x20,0x70,0x20,0x20,0x20,0x18,0x00,0x00,

	/*--  文字:     --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

	/*--  文字:  d  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x18,0x08,0x08,0x38,0x48,0x48,0x48,0x3C,0x00,0x00,

	/*--  文字:  i  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x20,0x00,0x00,0x60,0x20,0x20,0x20,0x70,0x00,0x00,

	/*--  文字:  f  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x1C,0x20,0x20,0x78,0x20,0x20,0x20,0x78,0x00,0x00,

	/*--  文字:  f  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x1C,0x20,0x20,0x78,0x20,0x20,0x20,0x78,0x00,0x00,

	/*--  文字:  e  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x30,0x48,0x78,0x40,0x38,0x00,0x00,

	/*--  文字:  r  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0xD8,0x60,0x40,0x40,0xE0,0x00,0x00,

	/*--  文字:  e  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x30,0x48,0x78,0x40,0x38,0x00,0x00,

	/*--  文字:  n  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0xF0,0x48,0x48,0x48,0xEC,0x00,0x00,

	/*--  文字:  t  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x20,0x20,0x70,0x20,0x20,0x20,0x18,0x00,0x00,

	/*--  文字:  s  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x78,0x40,0x30,0x08,0x78,0x00,0x00,

	/*--  文字:     --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

	/*--  文字:  f  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x1C,0x20,0x20,0x78,0x20,0x20,0x20,0x78,0x00,0x00,

	/*--  文字:  r  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0xD8,0x60,0x40,0x40,0xE0,0x00,0x00,

	/*--  文字:  u  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0xD8,0x48,0x48,0x48,0x3C,0x00,0x00,

	/*--  文字:  i  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x20,0x00,0x00,0x60,0x20,0x20,0x20,0x70,0x00,0x00,

	/*--  文字:  t  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x20,0x20,0x70,0x20,0x20,0x20,0x18,0x00,0x00,

	/*--  文字:  s  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x78,0x40,0x30,0x08,0x78,0x00,0x00,

	/*--  文字:  .  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x40,0x00,0x00,

	/*--  文字:     --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

	/*--  文字:  B  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0xF0,0x48,0x48,0x70,0x48,0x48,0x48,0xF0,0x00,0x00,

	/*--  文字:  e  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x30,0x48,0x78,0x40,0x38,0x00,0x00,

	/*--  文字:  f  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x1C,0x20,0x20,0x78,0x20,0x20,0x20,0x78,0x00,0x00,

	/*--  文字:  o  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x30,0x48,0x48,0x48,0x30,0x00,0x00,

	/*--  文字:  r  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0xD8,0x60,0x40,0x40,0xE0,0x00,0x00,

	/*--  文字:  e  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x30,0x48,0x78,0x40,0x38,0x00,0x00,

	/*--  文字:     --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

	/*--  文字:  t  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x20,0x20,0x70,0x20,0x20,0x20,0x18,0x00,0x00,

	/*--  文字:  h  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0xC0,0x40,0x40,0x70,0x48,0x48,0x48,0xEC,0x00,0x00,

	/*--  文字:  e  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x30,0x48,0x78,0x40,0x38,0x00,0x00,

	/*--  文字:     --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

	/*--  文字:  f  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x1C,0x20,0x20,0x78,0x20,0x20,0x20,0x78,0x00,0x00,

	/*--  文字:  r  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0xD8,0x60,0x40,0x40,0xE0,0x00,0x00,

	/*--  文字:  u  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0xD8,0x48,0x48,0x48,0x3C,0x00,0x00,

	/*--  文字:  i  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x20,0x00,0x00,0x60,0x20,0x20,0x20,0x70,0x00,0x00,

	/*--  文字:  t  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x20,0x20,0x70,0x20,0x20,0x20,0x18,0x00,0x00,

	/*--  文字:  s  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x78,0x40,0x30,0x08,0x78,0x00,0x00,

	/*--  文字:     --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

	/*--  文字:  d  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x18,0x08,0x08,0x38,0x48,0x48,0x48,0x3C,0x00,0x00,

	/*--  文字:  r  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0xD8,0x60,0x40,0x40,0xE0,0x00,0x00,

	/*--  文字:  o  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x30,0x48,0x48,0x48,0x30,0x00,0x00,

	/*--  文字:  p  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0xF0,0x48,0x48,0x48,0x70,0x40,0xE0,

	/*--  文字:  p  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0xF0,0x48,0x48,0x48,0x70,0x40,0xE0,

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

	/*--  文字:     --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

	/*--  文字:  o  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x30,0x48,0x48,0x48,0x30,0x00,0x00,

	/*--  文字:  f  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x1C,0x20,0x20,0x78,0x20,0x20,0x20,0x78,0x00,0x00,

	/*--  文字:  f  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x1C,0x20,0x20,0x78,0x20,0x20,0x20,0x78,0x00,0x00,

	/*--  文字:  ,  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x40,0x40,0x80,

	/*--  文字:     --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

	/*--  文字:  Y  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0xD8,0x50,0x50,0x20,0x20,0x20,0x20,0x70,0x00,0x00,

	/*--  文字:  o  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x30,0x48,0x48,0x48,0x30,0x00,0x00,

	/*--  文字:  u  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0xD8,0x48,0x48,0x48,0x3C,0x00,0x00,

	/*--  文字:     --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

	/*--  文字:  m  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0xF0,0xA8,0xA8,0xA8,0xA8,0x00,0x00,

	/*--  文字:  u  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0xD8,0x48,0x48,0x48,0x3C,0x00,0x00,

	/*--  文字:  s  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x78,0x40,0x30,0x08,0x78,0x00,0x00,

	/*--  文字:  t  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x20,0x20,0x70,0x20,0x20,0x20,0x18,0x00,0x00,

	/*--  文字:     --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

	/*--  文字:  f  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x1C,0x20,0x20,0x78,0x20,0x20,0x20,0x78,0x00,0x00,

	/*--  文字:  u  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0xD8,0x48,0x48,0x48,0x3C,0x00,0x00,

	/*--  文字:  l  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0xE0,0x20,0x20,0x20,0x20,0x20,0x20,0xF8,0x00,0x00,

	/*--  文字:  l  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0xE0,0x20,0x20,0x20,0x20,0x20,0x20,0xF8,0x00,0x00,

	/*--  文字:     --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

	/*--  文字:  c  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x38,0x48,0x40,0x40,0x38,0x00,0x00,

	/*--  文字:  u  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0xD8,0x48,0x48,0x48,0x3C,0x00,0x00,

	/*--  文字:  t  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x20,0x20,0x70,0x20,0x20,0x20,0x18,0x00,0x00,

	/*--  文字:     --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

	/*--  文字:  o  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x30,0x48,0x48,0x48,0x30,0x00,0x00,

	/*--  文字:  f  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x1C,0x20,0x20,0x78,0x20,0x20,0x20,0x78,0x00,0x00,

	/*--  文字:  f  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x1C,0x20,0x20,0x78,0x20,0x20,0x20,0x78,0x00,0x00,

	/*--  文字:     --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

	/*--  文字:  .  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x40,0x00,0x00,

	/*--  文字:     --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

	/*--  文字:  Y  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0xD8,0x50,0x50,0x20,0x20,0x20,0x20,0x70,0x00,0x00,

	/*--  文字:  o  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x30,0x48,0x48,0x48,0x30,0x00,0x00,

	/*--  文字:  u  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0xD8,0x48,0x48,0x48,0x3C,0x00,0x00,

	/*--  文字:     --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

	/*--  文字:  d  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x18,0x08,0x08,0x38,0x48,0x48,0x48,0x3C,0x00,0x00,

	/*--  文字:  o  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x30,0x48,0x48,0x48,0x30,0x00,0x00,

	/*--  文字:  n  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0xF0,0x48,0x48,0x48,0xEC,0x00,0x00,

	/*--  文字:  '  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x40,0x40,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

	/*--  文字:  t  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x20,0x20,0x70,0x20,0x20,0x20,0x18,0x00,0x00,

	/*--  文字:     --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

	/*--  文字:  c  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x38,0x48,0x40,0x40,0x38,0x00,0x00,

	/*--  文字:  u  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0xD8,0x48,0x48,0x48,0x3C,0x00,0x00,

	/*--  文字:  t  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x20,0x20,0x70,0x20,0x20,0x20,0x18,0x00,0x00,

	/*--  文字:     --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

	/*--  文字:  t  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x20,0x20,0x70,0x20,0x20,0x20,0x18,0x00,0x00,

	/*--  文字:  h  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0xC0,0x40,0x40,0x70,0x48,0x48,0x48,0xEC,0x00,0x00,

	/*--  文字:  e  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x30,0x48,0x78,0x40,0x38,0x00,0x00,

	/*--  文字:     --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

	/*--  文字:  b  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0xC0,0x40,0x40,0x70,0x48,0x48,0x48,0x70,0x00,0x00,

	/*--  文字:  o  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x30,0x48,0x48,0x48,0x30,0x00,0x00,

	/*--  文字:  m  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0xF0,0xA8,0xA8,0xA8,0xA8,0x00,0x00,

	/*--  文字:  b  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0xC0,0x40,0x40,0x70,0x48,0x48,0x48,0x70,0x00,0x00,

	/*--  文字:  !  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x20,0x20,0x20,0x20,0x20,0x20,0x00,0x20,0x00,0x00,

	/*--  文字:     --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

	/*--  文字:  O  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x70,0x88,0x88,0x88,0x88,0x88,0x88,0x70,0x00,0x00,

	/*--  文字:  t  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x20,0x20,0x70,0x20,0x20,0x20,0x18,0x00,0x00,

	/*--  文字:  h  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0xC0,0x40,0x40,0x70,0x48,0x48,0x48,0xEC,0x00,0x00,

	/*--  文字:  e  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x30,0x48,0x78,0x40,0x38,0x00,0x00,

	/*--  文字:  r  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0xD8,0x60,0x40,0x40,0xE0,0x00,0x00,

	/*--  文字:  w  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0xA8,0xA8,0x70,0x50,0x50,0x00,0x00,

	/*--  文字:  i  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x20,0x00,0x00,0x60,0x20,0x20,0x20,0x70,0x00,0x00,

	/*--  文字:  s  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x78,0x40,0x30,0x08,0x78,0x00,0x00,

	/*--  文字:  e  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x30,0x48,0x78,0x40,0x38,0x00,0x00,

	/*--  文字:  ,  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x40,0x40,0x80,

	/*--  文字:     --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

	/*--  文字:  G  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x38,0x48,0x80,0x80,0x9C,0x88,0x48,0x30,0x00,0x00,

	/*--  文字:  a  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x30,0x48,0x38,0x48,0x3C,0x00,0x00,

	/*--  文字:  m  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0xF0,0xA8,0xA8,0xA8,0xA8,0x00,0x00,

	/*--  文字:  e  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x30,0x48,0x78,0x40,0x38,0x00,0x00,

	/*--  文字:     --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

	/*--  文字:  o  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x30,0x48,0x48,0x48,0x30,0x00,0x00,

	/*--  文字:  v  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0xEC,0x48,0x50,0x30,0x20,0x00,0x00,

	/*--  文字:  e  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x30,0x48,0x78,0x40,0x38,0x00,0x00,

	/*--  文字:  r  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0xD8,0x60,0x40,0x40,0xE0,0x00,0x00,

	/*--  文字:  .  --*/
	/*--  宋体9;  此字体下对应的点阵为：宽x高=6x12   --*/
	/*--  宽度不是8的倍数，现调整为：宽度x高度=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x40,0x00,0x00,
};
iU8 * Str = (iU8*)"A simple casual game. There is only one purpose -- to cut fruits! The screen will continue to jump out differents fruits. Before the fruits dropping off, You must full cut off . You don't cut the bomb! Otherwise, Game over.";

//    i51KitG2FontDrawEx1( NumberTypehead, 10, 6, 12, 0 ,0XFF00, 0, 100);
//    i51KitG2FontDrawEx1( Str,CutFruit_logo, 0, 6, 12, 0, 0, 0, 0, 0XFF00, iNULL);
    i51KitG2FontDrawEx2( Str,CutFruit_logo, 0, 6, 12, 0, 0, 0, 0, 0XFF00, iNULL);
    i51AdeMmiUpdateScreen();

}

void TestDrawi51Image( void )
{
    iU32 i51Image;
    iRECT SrcRect, DesRect;	
    iU32 W = 0, H = 0;	
    static iS32 Angle = 0;

    iU8 * TestArr[9];// = {pvz_warning_Hex};
    static	iS32 ImageId = 0; 
    SrcRect.X = 0;
    SrcRect.Y = 184;
    SrcRect.Width = 96;
    SrcRect.Height = 184;

    DesRect.X = 100;
    DesRect.Y = 60;
    DesRect.Width = 96;
    DesRect.Height = 184;

//    if(FLAG_FIRSTRUN)    TestInit();
//    i51KitG2DrawImage(CurrentImageContainer,prizenum,&DesRect,iNULL);

	ImageId++;ImageId = 0;
    i51KitG2GetImageAttrEx((iU32)TestArr[(ImageId/50)%9],&W,&H);
    i51Image = i51KitG2ImageCreate( (iU32)TestArr[(ImageId/50)%9] );
//    i51KitG2DrawImageEX(i51Image, &SrcRect, &DesRect);
    i51KitG2DrawImageEx(i51Image, iNULL, iNULL);
    i51KitG2DrawImageEx(i51Image, &SrcRect, &DesRect);
	//i51KitG2MirrorImageEx(i51Image, iNULL, iNULL);
//	i51KitG2FlipImageEx(i51Image, iNULL, iNULL);
	//i51KitG2RotateImageEx(i51Image, iNULL, iNULL, -120, 140, 120, 160, 0, Angle);
	Angle++;
	Angle %=360;
	i51KitG2ImageDestory(i51Image);
    i51AdeMmiUpdateScreen();
}

void TestFillImage ( void )
{
    iU16 * Path = L"i51KitG2\\i51IMIC";
    iRECT SrcRect, SrcRect1, DesRect, DesRect1;
	static iHANDLE CurrentVpActivity1 = iNULL;
static iS32 X =0, Y = 0;

if( FLAG_FIRSTRUN )
{
    CurrentImageContainer =  i51KitG2OpenImageContainer(Path);
    SrcRect.X = 0;   
    SrcRect.Y = 0;   
    SrcRect.Width= 240;   
    SrcRect.Height= 320;   
    i51KitG2VPEnable();
    CurrentVpActivity1 = i51KitG2VPCreate(1,&SrcRect);
    memset ( LcdBuffer , 0XFFFF , 240*320*2 ) ;
    FLAG_FIRSTRUN = iFALSE;
    i51KitG2VPSetActivity(CurrentVpActivity1);
    i51KitG2ImageFillRect( CurrentImageContainer,WALL, iNULL, iNULL);
}  
    memset ( LcdBuffer , 0XFFFF , 240*320*2 ) ;
i51KitG2VPRender(CurrentVpActivity1,iNULL);
X++;
Y++;
if(X>=ScreenWidth) X = 0;
if(Y>=ScreenHeight) Y = 0;
i51KitG2VPSetCoord(CurrentVpActivity1,X,Y);
//    i51KitG2ImageFillRect( CurrentImageContainer,W4ALL, iNULL, iNULL);
    i51AdeMmiUpdateScreen  () ;

}
void TestFillImage1 ( void )
{
    int i;
	static int nloop = 0;

    iU16 * Path = L"i51KitG2\\i51IMIC";
    iRECT SrcRect, SrcRect1, DesRect, DesRect1;

	static iHANDLE vps[4] = {iNULL};

static iS32 X =0, Y = 0;

if( FLAG_FIRSTRUN )
{
    CurrentImageContainer =  i51KitG2OpenImageContainer(Path);
    SrcRect.X = 0;   
    SrcRect.Y = 0;   
    SrcRect.Width= 240;   
    SrcRect.Height= 320;   
    memset ( LcdBuffer , 0XFFFF , 240*320*2 ) ;
    FLAG_FIRSTRUN = iFALSE;
}  
if (nloop++ % 100 ==0)
{
    SrcRect.X = 0;   
    SrcRect.Y = 0;   
    SrcRect.Width= 240;   
    SrcRect.Height= 320;   
    for (i = 0; i < 4; ++i)
    {
        if (vps[i])
			i51KitG2VPDestroy(vps[i]);
        vps[i] = i51KitG2VPCreate(10,&SrcRect);
    }
    for (i = 0; i < 4; ++i)
    {
        iRECT DesRect={0,0,20,20};
        iRECT SrcRect={0,0,20,20};
        i51KitG2VPSetActivity(vps[i]);
        i51KitG2ImageFillRect( CurrentImageContainer,1, &DesRect, &SrcRect);
    }
}
    memset ( LcdBuffer , 0XFFFF , 240*320*2 ) ;

    for (i = 0; i < 4; ++i)
    {
        i51KitG2VPSetCoord(vps[i],20*i,20*i);
        i51KitG2VPRender(vps[i],iNULL);
    }
    i51AdeMmiUpdateScreen  () ;

}

void TestScreenZoom( void )
{
i51KitG2CleanScreen(0XAAAA,iNULL);
i51KitG2CleanScreen(0XAAAA,iNULL);
i51KitG2Update();
return;

	if(FLAG_FIRSTRUN)    TestInit();
	
	memset ( LcdBuffer , 0XFF , 240*320*2 ) ;

	if(DecodeComplete)
	{
		i51KitG2DrawImage(CurrentImageContainer,BG,iNULL,iNULL);
		i51KitG2ScreenZoom(iNULL, iNULL, ZoomRateX,ZoomRateY);
		i51KitG2Update();
	}
}

void TestUpdataContainer( void )
{
	static iU32 Temp = 0 ;
	iU32 Temp1 = 0;
	iHANDLE  ImageContainer = iNULL;
	iFILE ImageFile = 0;
	iU16 FileName[10][40] =
	{
		L"i51KitG2\\test1.i51G",L"i51KitG2\\test2.i51G",L"i51KitG2\\test3.i51G",
		L"i51KitG2\\test4.i51G",L"i51KitG2\\test5.i51G",L"i51KitG2\\test6.i51G",
		L"i51KitG2\\test7.i51G",L"i51KitG2\\test8.i51G",L"i51KitG2\\test9.i51G"
	};
	iS32 ImageDataLength = 0;
	iU32 ReadDataLength = 0;
	iU8 *ImageDataBuff = iNULL;
	iFILE ReFile = iNULL;
	iRECT CleanRect;
	iHANDLE  TempContainer = iNULL;
	
	if(Flag)
	{
//		Flag = iFALSE;
		Temp ++;
		ImageContainer = i51KitG2OpenImageContainer(L"i51KitG2\\NewMic");
//		ImageFile = i51AdeOsFileOpen((FileName[(Temp/30)%9]), i51_OS_FL_WRITE| i51_OS_FL_ALWAYS_OPEN, __FILE__ , __LINE__ );
//		i51AdeOsFileGetSize(ImageFile, &ImageDataLength);
//		i51AdeOsFileSetSeek(ImageFile, 0, i51_OS_FL_SEEK_HEAD);
//		ImageDataBuff = i51AdeOsMalloc(ImageDataLength, __FILE__ , __LINE__ );
//		i51AdeOsFileRead(ImageFile, ImageDataBuff, ImageDataLength, 0);
		ReFile = i51KitStdResOpen(L"i51KitG2\\i51KitG2.re");
		i51AdeOsFileGetSize(ReFile, &ImageDataLength);
		ImageDataBuff = i51AdeOsMalloc(ImageDataLength, __FILE__ , __LINE__ );
		i51AdeOsFileSetSeek(ReFile, 0, i51_OS_FL_SEEK_HEAD);
		i51AdeOsFileRead(ReFile, ImageDataBuff, ImageDataLength, &ReadDataLength);
			
//		i51KitG2UpdataContainerFromImage( ImageContainer, (void *)ImageFile, i51KitG2_MESSAGE_TYPE_FILE, 0);
//		i51KitG2UpdataContainerFromImage( ImageContainer, ImageDataBuff, i51KitG2_MESSAGE_TYPE_MEMORY, 0);
		i51KitG2UpdataContainerFromRe( ImageContainer, (void *)ReFile,(Temp/30)%9, i51KitG2_MESSAGE_TYPE_FILE, 0);
//		i51KitG2UpdataContainerFromRe( ImageContainer, (void *)ImageDataBuff,Temp, i51KitG2_MESSAGE_TYPE_MEMORY, 0);

		i51AdeOsFree(ImageDataBuff);
		ImageDataBuff = iNULL;

//		TempContainer = i51KitG2OpenImageContainer(L"i51KitG2\\i51IMIC");
//		i51KitG2GetContainerAttr(TempContainer,0,0,0,0);
//		i51KitG2CloseImageContainer(TempContainer);

		i51KitStdResClose(ReFile);
		ReFile = iNULL;
		
//		Temp = (Temp+1)%5;
//		Temp1 =  i51KitStdResGetContentEntry(ReFile,Temp);

		i51KitG2DrawImage(ImageContainer,0,iNULL,iNULL);
//		i51AdeOsFileClose(ImageFile);
		i51KitG2DestroyImageContainer(ImageContainer,L"i51KitG2\\NewMic");

		i51KitG2Update();
	}
}
#endif

static iBOOL i51KitG2Pause( void )
{
	// 图片索引表地址
	i51ImageIndex * ImageIndexTable = iNULL;
	// 图片容器地址入口
	i51ImageContainer * ImageContainerEntry = iNULL;
	iS32 Temp = 0;
	iS32 MemCopyNum = 0;
	iU32 * ImageID = iNULL;
	iU32 * MemCopyCount = iNULL;
	iU32 TempID = 0;
	iU16 AllImageNum = 0;
	iS32 i,j,k;

//iLog("1");
	if(TimeIdZlibDecodeImage)
	{
		i51AdeOsTimerStop( TimeIdZlibDecodeImage );       
		TimeIdZlibDecodeImage = iNULL;
	}

//iLog("2");
	ImageID = i51AdeOsMalloc((sizeof(iU32))*300, __FILE__ , __LINE__ );
	if( iNULL==ImageID )
	{
		iLog( "KIT - G2 : pause malloc");
		goto ERROR;
	}

//iLog("3");
	MemCopyCount = i51AdeOsMalloc((sizeof(iU32))*300, __FILE__ , __LINE__ );
	if( iNULL==MemCopyCount )
	{
		iLog( "KIT - G2 : pause malloc mem");
		goto ERROR;
	}

//iLog("4");
	for( Temp=0;Temp<MAX_ContainerManage_NUM; Temp++ )
	{
		if( ContainerManage[Temp].Flag )
		{
//iLog("Temp:%d",Temp);

			ImageContainerEntry = ContainerManage[Temp].ContainerHandle;
			if( iNULL==ImageContainerEntry )
			{
				iLog( "KIT - G2 : pause %X", Temp);
				continue;
			}
			ImageIndexTable = ImageContainerEntry->ImageIndexTable;
			AllImageNum = ImageContainerEntry->ImageMaxId;
			MemCopyNum = 0;
			for( TempID=0; TempID<AllImageNum; TempID++ )
			{
				if( ImageIndexTable[TempID].FlagImageExist==0) continue;
				if( ImageIndexTable[TempID].FlagCopyRAM )
				{
					ImageID[MemCopyNum] = TempID;
					MemCopyCount[MemCopyNum] = ImageIndexTable[TempID].SetMemoryCopyCount;
					MemCopyNum++;
					if( MemCopyNum>=300 )
					{
						iLog( "KIT - G2 : pause MemCopyNum %X", MemCopyNum);
						goto ERROR;
					}
				}
			}

			if(MemCopyNum>0)
			{
				for( i=0; i<MemCopyNum; i++ )
				{
					k = i;
					for( j=i+1; j<MemCopyNum; j++ )
					{
						if(MemCopyCount[j]<MemCopyCount[k])
						{
							k = j;
						}
					}
					
					if( k!=i)
					{
						j = MemCopyCount[i];
						MemCopyCount[i] = MemCopyCount[k];
						MemCopyCount[k] = j;
					
						j = ImageID[i];
						ImageID[i] = ImageID[k];
						ImageID[k] = j;
					
					}
				}
			
				ContainerManage[Temp].ImageID = i51AdeOsMalloc( (sizeof(iU32))*MemCopyNum, __FILE__, __LINE__ );
				if( iNULL==ContainerManage[Temp].ImageID )
				{
					iLog( "KIT - G2 : pause malloc ImagID");
					goto ERROR;
				}

				ContainerManage[Temp].MemCopyCount= i51AdeOsMalloc( (sizeof(iU32))*MemCopyNum, __FILE__, __LINE__ );
				if( iNULL==ContainerManage[Temp].MemCopyCount )
				{
					iLog( "KIT - G2 : pause malloc count");
					goto ERROR;
				}

				i51AdeStdMemcpy16(ContainerManage[Temp].ImageID, ImageID, (sizeof(iU32))*MemCopyNum);
				i51AdeStdMemcpy16(ContainerManage[Temp].MemCopyCount, MemCopyCount, (sizeof(iU32))*MemCopyNum);

			}
			else
			{
				ContainerManage[Temp].ImageID = iNULL;
				ContainerManage[Temp].MemCopyCount = iNULL;
			}
			
			ContainerManage[Temp].MemoryCopyImageNum = MemCopyNum;		
			i51KitG2UnsetImageMemoryCopy(ContainerManage[Temp].ContainerHandle, 0, 0);

		}
	}

//iLog("5");

	i51AdeOsFree(MemCopyCount);
	i51AdeOsFree(ImageID);

	MemCopyCount = iNULL;
	ImageID = iNULL;
//iLog("6");

	return iTRUE;

ERROR:
	if(MemCopyCount) i51AdeOsFree(MemCopyCount);
	if(ImageID) i51AdeOsFree(ImageID);
	MemCopyCount = iNULL;
	ImageID = iNULL;

	if( ContainerManage[Temp].ImageID ) i51AdeOsFree(ContainerManage[Temp].ImageID);
	if( ContainerManage[Temp].MemCopyCount ) i51AdeOsFree(ContainerManage[Temp].MemCopyCount);
	ContainerManage[Temp].ImageID = iNULL;
	ContainerManage[Temp].MemCopyCount = iNULL;

	return iFALSE;

}

static iBOOL i51KitG2Resume( void )
{

	iS32 Temp = 0;

	if(FlagDecodeImage)
	{
		TimeIdZlibDecodeImage = i51AdeOsTimerCreate( 1,1);
		i51AdeOsTimerStart( TimeIdZlibDecodeImage, ZlibDecodeAllImage, 30);
	}

	RESUNE_MEMORY_COPY = iTRUE;

	for( Temp=0;Temp<MAX_ContainerManage_NUM; Temp++ )
	{
		if( ContainerManage[Temp].Flag && ContainerManage[Temp].ImageID )
		{
			RESUNE_MEMORY_COPY = iTRUE;
			CurrContainerManageNum = Temp;
			i51KitG2SetImageMemoryCopy(
				ContainerManage[Temp].ContainerHandle, 
				ContainerManage[Temp].MemoryCopyImageNum, 
				ContainerManage[Temp].ImageID);
			if( ContainerManage[Temp].ImageID )
			{
				i51AdeOsFree( ContainerManage[Temp].ImageID );
			}
			ContainerManage[Temp].ImageID  = iNULL;

			if( ContainerManage[Temp].MemCopyCount )
			{
				i51AdeOsFree( ContainerManage[Temp].MemCopyCount);
			}
			ContainerManage[Temp].MemCopyCount= iNULL;
			
			ContainerManage[Temp].MemoryCopyImageNum = 0;
		}
	}
	
	RESUNE_MEMORY_COPY = iFALSE;

	return iTRUE;
}

static iBOOL i51KitG2Exit( void )
{
	iS32 Temp = 0;
	
	for( Temp=0;Temp<MAX_ContainerManage_NUM; Temp++ )
	{
		if( ContainerManage[Temp].Flag )
		{
			if( ContainerManage[Temp].ImageID )
			{
				i51AdeOsFree( ContainerManage[Temp].ImageID );
			}
			ContainerManage[Temp].ImageID  = iNULL;

			if( ContainerManage[Temp].MemCopyCount )
			{
				i51AdeOsFree( ContainerManage[Temp].MemCopyCount);
			}
			ContainerManage[Temp].MemCopyCount= iNULL;

//			ContainerManage[Temp].Flag = iFALSE;
//			ContainerManage[Temp].ContainerHandle = 0;
		}
	}

	if(TimeIdZlibDecodeImage)
	{
		i51AdeOsTimerStop( TimeIdZlibDecodeImage );       
		TimeIdZlibDecodeImage = iNULL;
	}

	if( FlagDecodeImage && ZlibImageContainer)
	{
		i51KitG2CloseImageContainer(ZlibImageContainer);
		ZlibImageContainer = iNULL;
	}

	i51KitG2Release();
	i51KIT_DYNAMIC_KIT_UNLOAD(i51KitStd);

	return iTRUE;

}

iKIT iBOOL i51KitMain ( iS32 aid , iU32 message , void* parameter )
{

    //    TODO :    消息循环
#if DEBUG_I51KITG2  
    iU32 TestType = 15;
#endif

    switch ( message )
    {

#if DEBUG_I51KITG2
	case i51_MSG_PADUP:
        case i51_MSG_KEYUP :
			Flag = iTRUE;
//		ZoomRate = ((iU32)parameter)*25;
		if ( UseCach)
		{
			UseCach = iFALSE;
//			i51KitG2FontCachEnable(TypeheadHandle, 0);
//	             i51KitG2SetImageMemoryCopy(CurrentImageContainer,0,iNULL);
		}
		else
		{
			UseCach = iTRUE;
//	             i51KitG2UnsetImageMemoryCopy(CurrentImageContainer,0,iNULL);
//			i51KitG2FontCachDisable(TypeheadHandle, 0);
		}
             break;
#endif			 
        case i51_MSG_PAINT :

#if DEBUG_I51KITG2
              switch( TestType){
	            case 1:
	              TestDrawLine();
	              break;
	            case 2:
	              TestDrawPolygon();
	              break;
	            case 3:
	              TestDrawArc();
	              break;
	            case 4 :
	              TestDarwCircle();
	              break;
	            case 5:
	              TestFillPolygon();
	              break;
	            case 6 :
	              TestFillArc();
	              break;
	            case 7 :
	              TestFillCircle();
	              break;
	            case 8 :
	              TestCreateImageContainer();
	              break;
	            case 9:
	              TestOpenImageContainer();
	              break;
	            case 10 :
	              TestCloseImageContainer();
	              break;
	           case 11 :
	              TestDestroyImageContainer();
	              break;
	          case 12 :
	              TestDrawImage();
	               break;
	          case 13 :
	              TestRotateImage();
	               break;
	          case 14 :
	              TestVP();
	               break;	
              case 15:
			  TestImage();
			  break;
		case 17:
			TestFontDraw();
			break;
		case 18:
			TestDrawTypehead();
			break;
		case 19:
			TestDrawi51Image();
			break;
		case 20:
			TestFillImage();
			break;
		case 21 :
			TestScreenZoom();
			break;
		case 22:
			TestUpdataContainer();
			break;
	       default :
	              break;
	      }
#endif
             break ;
	case i51_MSG_RUN:
	{
		break;
	}
        case i51_MSG_INIT :
        {
  
		i51KIT_STATIC_KIT_SET(Adapter,parameter);
		i51KIT_STATIC_KIT_ASSERT(Adapter);

//iLog("load Adapter");
             //	加载i51KitStd动态库
             i51KIT_DYNAMIC_KIT_LOAD(i51KitStd,L"PKiGAPI\\i51KitStd.kit");

        
             if ( iNULL == i51KIT_DYNAMIC_KIT_PROTOTYPE(i51KitStd) ) 
             {
                 return 0 ;
             }

//iLog("load std");
            if( iNULL == i51KitG2Create() ) return 0;
//iLog("i51KitG2Create");

		//i51KernelSetCache(L"i51KitG2");
					
#if DEBUG_I51KITG2
		i51AdeOsScreenAlwaysLight(1);
//		TypeheadHandle = i51KitStdResOpen(L"Fonts\\Typehead.re");
		TypeheadHandle = i51KitStdResOpen(L"Fonts\\fzlthjw_14.re");
		i51KitG2FontCachEnable(TypeheadHandle, 0);
		//    i51KitG2VPEnable();
		//    i51KitG2SetAlpha(150);
//		    memset ( LcdBuffer , 0 , 240*320*2 ) ;
i51KitG2CleanScreen(0, iNULL);
			i51KitG2Update();
		iLog( "Init OK");
		//i51KernelSetCache(L"i51KitG2");
#endif
        }
iLog(" KIT - G2 : init OK ");
        break ;

        case i51_MSG_PAUSE :
//iLog("暂停之前剩余内存 : %d", i51AdeOsGetTotallMemoryLeft());
iLog(" KIT - G2 : pause start ");
		i51KitG2Pause();
iLog(" KIT - G2 : pause end ");
//iLog("暂停之后剩余内存: %d", i51AdeOsGetTotallMemoryLeft());
            break ;

        case i51_MSG_RESUME:
//iLog("恢复之前剩余内存: %d", i51AdeOsGetTotallMemoryLeft());
iLog(" KIT - G2 : resume start ");
		i51KitG2Resume();
iLog(" KIT - G2 : resume end ");
//iLog("恢复之后剩余内存: %d", i51AdeOsGetTotallMemoryLeft());
            break;
            
        case i51_MSG_EXIT :
#if DEBUG_I51KITG2
	i51KitG2FontCachDisable(TypeheadHandle, 0);
	TypeheadHandle = iNULL;

	i51KitG2CloseImageContainer(CurrentImageContainer);
	CurrentImageContainer = iNULL;
	
	i51KitG2VPDestroy(VP_Bee);
	i51KitG2VPDestroy(VP_BK);
	i51KitG2VPDestroy(VP_Road);
	i51KitG2VPDestroy(VP_Snidget);
	VP_Bee = iNULL;
	VP_BK = iNULL;
	VP_Road = iNULL;
	VP_Snidget = iNULL;
#endif
//iLog("G2 exit");
iLog(" KIT - G2 : exit start ");
i51KitG2Exit();
iLog(" KIT - G2 : exit end ");
        break ;
    }

    return iTRUE ;

}


iKIT iU32* i51KitPort ()
{

    //    TODO : Initialize API Port for external layer

    i51KIT_DYNAMIC_KIT_MAPING_BEGIN(i51KitG2)
    
    i51KIT_DYNAMIC_KIT_MAPING(i51KitG2CleanScreen)

    i51KIT_DYNAMIC_KIT_MAPING(i51KitG2DrawPoint)
    i51KIT_DYNAMIC_KIT_MAPING(i51KitG2DrawLine)
    i51KIT_DYNAMIC_KIT_MAPING(i51KitG2DrawPolygon)
    i51KIT_DYNAMIC_KIT_MAPING(i51KitG2DrawArc)
    i51KIT_DYNAMIC_KIT_MAPING(i51KitG2DrawCircle)
    
    i51KIT_DYNAMIC_KIT_MAPING(i51KitG2FillPolygon)
    i51KIT_DYNAMIC_KIT_MAPING(i51KitG2FillArc)
    i51KIT_DYNAMIC_KIT_MAPING(i51KitG2FillCircle)
    
    i51KIT_DYNAMIC_KIT_MAPING(i51KitG2CreateImageContainer)
    i51KIT_DYNAMIC_KIT_MAPING(i51KitG2CloseImageContainer)
    i51KIT_DYNAMIC_KIT_MAPING(i51KitG2CheckImageContainer)
    i51KIT_DYNAMIC_KIT_MAPING(i51KitG2OpenImageContainer)
    i51KIT_DYNAMIC_KIT_MAPING(i51KitG2DestroyImageContainer)
    i51KIT_DYNAMIC_KIT_MAPING(i51KitG2SetContainerContentVersion)
    i51KIT_DYNAMIC_KIT_MAPING(i51KitG2GetContainerContentVersion)
    
    i51KIT_DYNAMIC_KIT_MAPING(i51KitG2SetImageMemoryCopy)
    i51KIT_DYNAMIC_KIT_MAPING(i51KitG2UnsetImageMemoryCopy)
    i51KIT_DYNAMIC_KIT_MAPING(i51KitG2GetImageAttr)
    i51KIT_DYNAMIC_KIT_MAPING(i51KitG2DrawImage)
    i51KIT_DYNAMIC_KIT_MAPING(i51KitG2MirrorImage)
    i51KIT_DYNAMIC_KIT_MAPING(i51KitG2FlipImage)
    i51KIT_DYNAMIC_KIT_MAPING(i51KitG2RotateImage)
    i51KIT_DYNAMIC_KIT_MAPING(i51KitG2ImageFillRect)

    i51KIT_DYNAMIC_KIT_MAPING(i51KitG2FontCachEnable )
    i51KIT_DYNAMIC_KIT_MAPING(i51KitG2FontCachDisable)
    i51KIT_DYNAMIC_KIT_MAPING(i51KitG2FontGetDemension )
    i51KIT_DYNAMIC_KIT_MAPING(i51KitG2FontDraw )

    i51KIT_DYNAMIC_KIT_MAPING(i51KitG2VPEnable)  
    i51KIT_DYNAMIC_KIT_MAPING(i51KitG2VPDisable)  
    i51KIT_DYNAMIC_KIT_MAPING(i51KitG2VPCreate)  
    i51KIT_DYNAMIC_KIT_MAPING(i51KitG2VPDestroy)  
    i51KIT_DYNAMIC_KIT_MAPING(i51KitG2VPSetCoord)  
    i51KIT_DYNAMIC_KIT_MAPING(i51KitG2VPSetActivity)  
    i51KIT_DYNAMIC_KIT_MAPING(i51KitG2VPGetActivity)  
    i51KIT_DYNAMIC_KIT_MAPING(i51KitG2VPClean)  
    i51KIT_DYNAMIC_KIT_MAPING(i51KitG2VPGetAtrribute)  
    i51KIT_DYNAMIC_KIT_MAPING(i51KitG2VPiSFull)  
    i51KIT_DYNAMIC_KIT_MAPING(i51KitG2VPRender)  
    
    i51KIT_DYNAMIC_KIT_MAPING(i51KitG2Update)  
    i51KIT_DYNAMIC_KIT_MAPING(i51KitG2SetAlpha)
    i51KIT_DYNAMIC_KIT_MAPING(i51KitG2ScreenZoom)

    i51KIT_DYNAMIC_KIT_MAPING(i51KitG2UpdataContainerFromImage )
    i51KIT_DYNAMIC_KIT_MAPING(i51KitG2UpdataContainerFromRe )
    i51KIT_DYNAMIC_KIT_MAPING(i51KitG2ContainerImageIdExist)
    i51KIT_DYNAMIC_KIT_MAPING(i51KitG2GetContainerAttr)
    i51KIT_DYNAMIC_KIT_MAPING(i51KitG2ContainerDeleteImage)

    // start add by otto 2012.6.11
    i51KIT_DYNAMIC_KIT_MAPING(i51KitG2StopDecode)
    // end add by otto 2012.6.11

   i51KIT_DYNAMIC_KIT_MAPING_END

    return i51KIT_DYNAMIC_KIT_GET_MAPING(i51KitG2) ;

}




