
#include "i51KitAde.h"
#include "i51KitStd.h"
#include "i51KitG2.h"
#include "i51KitG1.h"
#include "i51Particle.h"
#include "i51ParticleRes.h"

i51KIT_STATIC_KIT_DEF(Adapter);
i51KIT_DYNAMIC_KIT_DEF(i51KitStd);
i51KIT_DYNAMIC_KIT_DEF(i51KitG2);
i51KIT_DYNAMIC_KIT_DEF(i51KitG1);

static iHANDLE g_ParticleHandle = iNULL;
static iHANDLE g_ImageContainer = iNULL;
static iHANDLE g_PH[4] = {0};
static iU32 g_ImageId[2] = {alpha_blue_star5, alpha_blue_star7};
static iS16 g_Count = 3;
static iS16 g_ParticleIndex = 0;

static iU16 *g_ScreenBuf = iNULL;
static iU16 g_ScreenWidth= 0, g_ScreenHeight = 0;
static iS16 g_TouchFlag = 0;
static iRECT g_Rect = {0};


void i51KitG1Process(iU32 Step , iU32 Totall , iHANDLE Handle)
{

}

void i51KitG1Done( iHANDLE Handle)
{
	if( i51AdeOsFileIsExist(L"i51Particle\\PARTICLE") ){
		iLog("create imageContainer fail");
	}

	if (Handle){
		g_ImageContainer = Handle;

		i51KitG2SetImageMemoryCopy(g_ImageContainer,2,g_ImageId);

		//创建特效
		g_PH[0] = i51KitG1ParticleCreateEffect(G1_PARTICLE_JET, g_ImageContainer, g_ImageId[0], 5, 25, 0);
		g_PH[1] = i51KitG1ParticleCreateEffect(G1_PARTICLE_EXPLOSION, g_ImageContainer, g_ImageId[1], 7, 25, 0);
		g_PH[2] = i51KitG1ParticleCreateEffect(G1_PARTICLE_CYCLICDIFFUSE, g_ImageContainer, g_ImageId[1], 7, 25, 0);
		g_PH[3] = i51KitG1ParticleCreateEffect(G1_PARTICLE_AREADIFFUSE, g_ImageContainer, g_ImageId[1], 7, 25, 0);

		g_ParticleHandle = g_PH[0];
	}
}

iKIT iBOOL i51KitMain ( iS32 aid , iU32 message , void* parameter )
{

	//	TODO :	消息循环
	static iU32 sTime = 0;

	switch ( message )
	{
	case i51_MSG_INIT :
		{
			i51KIT_STATIC_KIT_SET(Adapter,parameter);
			i51KIT_STATIC_KIT_ASSERT(Adapter);

			i51KIT_DYNAMIC_KIT_LOAD(i51KitStd,L"PKiGAPI\\i51KitStd.kit");
			if ( iNULL == i51KIT_DYNAMIC_KIT_PROTOTYPE(i51KitStd) ){
				return 0;
			}

			//	加载i51KitG2动态库
			i51KIT_DYNAMIC_KIT_LOAD(i51KitG2,L"PKiGAPI\\i51KitG2.kit");
			i51AdeOsLog(0,"PKiGAPI\\i51KitG2.kit");
			if ( iNULL == i51KIT_DYNAMIC_KIT_PROTOTYPE(i51KitG2) ){
				return 0 ;
			}

			//	加载i51KitG1动态库
			i51KIT_DYNAMIC_KIT_LOAD(i51KitG1,L"PKiGAPI\\i51KitG1.kit");
			i51AdeOsLog(0,"PKiGAPI\\i51KitG1.kit");
			if ( iNULL == i51KIT_DYNAMIC_KIT_PROTOTYPE(i51KitG1) ){
				return 0 ;
			}


			{
				iU32 *ScreenContext=iNULL;
				if( i51AdeMmiGetScreenScale (&g_ScreenWidth , &g_ScreenHeight) == 0 ){
					return iFALSE;
				}

				ScreenContext =  i51AdeMmiGetPanel();
				if( iNULL == ScreenContext ) return iFALSE;
				g_ScreenBuf = (iU16 *)ScreenContext[1];
			}


			//加载粒子特效所需图片
			{
				iU16 *ResPath = L"i51Particle\\i51Particle.re";
				iU16 *ContainerPath = L"i51Particle\\PARTICLE";
				iFILE fileRes = iNULL;

				if ( iTRUE == i51AdeOsFileIsExist(ResPath) ){

					fileRes = i51KitStdResOpen(ResPath);
					if ( iNULL == fileRes ){
						iLog("i51KitStdResOpen open res fail");
						return iFALSE;
					}

					i51KitG2CreateImageContainer (fileRes , g_ImageId , 2, ContainerPath ,  i51KitG1Process , i51KitG1Done );
				}
			}

		}break ;
	case i51_MSG_PAINT :
		{
			i51AdeStdMemset16(g_ScreenBuf, 0, sizeof(iU16)*g_ScreenWidth*g_ScreenHeight);

			sTime = i51AdeOsGetTick();
			i51KitG1ParticleDraw(g_ParticleHandle);
			sTime = i51AdeOsGetTick() - sTime;
			iLog("i51KitG1DrawParticle use time is %d, Particle Id is %d", sTime, g_ParticleIndex);

			i51AdeMmiUpdateScreen();

		}break ;
	case i51_MSG_RUN :
		break ;
	case i51_MSG_PAUSE :
		break ;
	case i51_MSG_EXIT :
		{
			i51KitG1ParticleDeleteEffect(g_PH[0]);
			i51KitG1ParticleDeleteEffect(g_PH[1]);
			i51KitG1ParticleDeleteEffect(g_PH[2]);
			i51KitG1ParticleDeleteEffect(g_PH[3]);
		}
		break ;
	case i51_MSG_PADDOWN:
	case i51_MSG_PADUP:
	case i51_MSG_PADMOVE:
		{
			iS16 x = 0;
			iS16 y = 0;
			x = (((iU32)parameter)>>16)&0XFFFF;
			y = (((iU32)parameter))&0XFFFF;


			g_Rect.X  = g_ScreenWidth - 31;
			g_Rect.Y  = g_ScreenHeight - 31;	
			g_Rect.Width = 30;
			g_Rect.Height = 30;	
			if ( i51_MSG_PADDOWN == message ){
				if ( x>=g_Rect.X && x<g_ScreenWidth && 
					y >= g_Rect.Y && y<g_ScreenHeight){
						g_TouchFlag = 1;
				}
			}

			if ( i51_MSG_PADDOWN == message ){
				if ( x>=g_Rect.X && x<g_ScreenWidth && 
					y >= g_Rect.Y && y<g_ScreenHeight && 1 == g_TouchFlag){

					g_ParticleIndex++;
					g_ParticleIndex = g_ParticleIndex%4;

					g_ParticleHandle=g_PH[g_ParticleIndex];
					if ( 0 == g_ParticleIndex ){
						g_Count = 3;							
					}else if ( 2 == g_ParticleIndex ){
						g_Count = 8;
					}else{
						g_Count = 25;
					}
				}
			}

			g_TouchFlag = 0;

			sTime = i51AdeOsGetTick();
			i51KitG1ParticleAdd(g_ParticleHandle, g_Count, x, y);
			sTime = i51AdeOsGetTick() - sTime;
			iLog("i51KitG1AddParticle use time is %d, Particle Id is %d", sTime, g_ParticleIndex);
		}break;
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
