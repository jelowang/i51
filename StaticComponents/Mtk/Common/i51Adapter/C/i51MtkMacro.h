/*
可以从下面选择、拷贝到i51Scripts.h中，来修改平台定义
#define MTK_6225_0812

#define MTK_6253_09A
#define MTK_6253_09B
#define MTK_6253_10A_W10
#define MTK_6253_10A_W11
#define MTK_6252_10A_W10
#define MTK_6252_10A_W11

#define MTK_6235_08B
#define MTK_6235_09A
#define MTK_6235_09B
#define MTK_6235_10A
#define MTK_6235_10A_W10
#define MTK_6235_10A_W11
#define MTK_6236_10A_W10

#define MTK_6236_11A_1112
#define MTK_6252_11B_1132
#define MTK_6250_11B_1132
#define MTK_6255_11B_1144
#define MTK_6255_11B_1212
#define MTK_6250_11B_1224
#define MTK_6255_11B_1224
*/

#define  VS_07B 100
#define  VS_0812 200
#define  VS_08B 201
#define  VS_09A 300
#define  VS_09B 301
#define  VS_W10 400
#define  VS_W11 401
#define  VS_11A_1112 500
#define  VS_11B_1132 501
#define  VS_11B_1212 502
#define  VS_11B_1224 510


#if defined(MTK_6225_0812) || defined(MTK_6225_0816) || defined(MTK_6225_GEMINI_0812)
    #define  i51_ADE_CNF_PLATFORM   "MTK_6225_0812"
    #define  I51_MTK_VS VS_0812
    #define  __MANUAL_CACHE__
#elif defined(MTK_6253_09A) || defined(MTK_6253_09A_0936) || defined(MTK_6253_09A_0952) || defined(MTK_6253_09A_1012)
    #define  i51_ADE_CNF_PLATFORM   "MTK_6253_09A"  
    #define  I51_MTK_VS VS_09A
    #define  __MANUAL_CACHE__
#elif defined(MTK_6253_09B) || defined(MTK_6253_09B_1012)
    #define  i51_ADE_CNF_PLATFORM   "MTK_6253_09B"   
    #define  I51_MTK_VS VS_09B
    #define  __MANUAL_CACHE__
#elif  defined(MTK_6253_10A_W10)
    #define  i51_ADE_CNF_PLATFORM   "MTK_6253_10A_W10"
    #define  I51_MTK_VS VS_W10
//    #define  __MANUAL_CACHE__
#elif  defined(MTK_6253_10A_W11)
    #define  i51_ADE_CNF_PLATFORM   "MTK_6253_10A_W11"
    #define  I51_MTK_VS VS_W11
//    #define  __MANUAL_CACHE__
#elif  defined(MTK_6252_10A_W10)
    #define  i51_ADE_CNF_PLATFORM   "MTK_6252_10A_W10"
    #define  I51_MTK_VS VS_W10
//    #define  __MANUAL_CACHE__
#elif  defined(MTK_6252_10A_W11)
    #define  i51_ADE_CNF_PLATFORM   "MTK_6252_10A_W11"
    #define  I51_MTK_VS VS_W11
//    #define  __MANUAL_CACHE__
#elif defined(MTK_6235_08B)  || defined(MTK_6235_08B_0912)
    #define  i51_ADE_CNF_PLATFORM   "MTK_6235_08B"
    #define  I51_MTK_VS VS_08B
#elif defined(MTK_6235_09A)  || defined(MTK_6235_09A_0912)
    #define  i51_ADE_CNF_PLATFORM   "MTK_6235_09A"
    #define  I51_MTK_VS VS_09A
#elif defined(MTK_6235_09B) || defined(MTK_6235_0932)
    #define  i51_ADE_CNF_PLATFORM   "MTK_6235_09B"
    #define  I51_MTK_VS VS_09B
#elif  defined(MTK_6235_10A_W10) || defined(MTK_6235_10A)
    #define  i51_ADE_CNF_PLATFORM   "MTK_6235_10A_W10"
    #define  I51_MTK_VS VS_W10
#elif  defined(MTK_6235_10A_W11)
    #define  i51_ADE_CNF_PLATFORM   "MTK_6235_10A_W11"
    #define  I51_MTK_VS VS_W11
#elif  defined(MTK_6236_10A_W10)
    #define  i51_ADE_CNF_PLATFORM   "MTK_6236_10A_W10"
    #define  I51_MTK_VS VS_W10
#elif  defined(MTK_6236_11A_1112)
    #define  i51_ADE_CNF_PLATFORM   "MTK_6236_11A_1112"
    #define  I51_MTK_VS VS_11A_1112
#elif  defined(MTK_6252_11B_1132) || defined(MTK_6250_11B_1132)
    #define  i51_ADE_CNF_PLATFORM   "MTK_6252_11B_1132"
    #define  I51_MTK_VS VS_11B_1132
#elif  defined(MTK_6255_11B_1144)
    #define  i51_ADE_CNF_PLATFORM   "MTK_6255_11B_1144"
    #define  I51_MTK_VS VS_11B_1132
#elif  defined(MTK_6255_11B_1212)
    #define  i51_ADE_CNF_PLATFORM   "MTK_6255_11B_1212"
    #define  I51_MTK_VS VS_11B_1212
#elif  defined(MTK_6250_11B_1224) || defined(MTK_6255_11B_1224)
    #define  i51_ADE_CNF_PLATFORM   "MTK_6250_11B_1224"
    #define  I51_MTK_VS VS_11B_1224
#else
#error "Please select one MTK for building!"
#endif

#ifdef WIN32
#ifdef __MANUAL_CACHE__
#undef __MANUAL_CACHE__
#endif
#endif
