
#ifndef THE_i51KitG1_H
#define THE_i51KitG1_H

#include "i51.h"

//粒子系统模式
typedef enum enumPARTICLE_MODE
{
	G1_PARTICLE_JET			  = 1, //粒子喷射
	G1_PARTICLE_EXPLOSION     = 2, //爆炸
	G1_PARTICLE_CYCLICDIFFUSE = 3, //环状扩散
	G1_PARTICLE_AREADIFFUSE	  = 4, //区域扩散
}G1_PARTICLE_MODE;

//#define RELEASE_i51KitG1

i51KIT_DYNAMIC_KIT_DEF_BEGIN

i51KIT_DYNAMIC_KIT_DEF_PORT(i51KitG1BladeDraw)
i51KIT_DYNAMIC_KIT_DEF_PORT(i51KitG1BladeClearTrack)
i51KIT_DYNAMIC_KIT_DEF_PORT(i51KitG1BladeSetTrack)
i51KIT_DYNAMIC_KIT_DEF_PORT(i51KitG1BladeSetColor)

i51KIT_DYNAMIC_KIT_DEF_PORT(i51KitG1ParticleCreateEffect)
i51KIT_DYNAMIC_KIT_DEF_PORT(i51KitG1ParticleStopEffect)
i51KIT_DYNAMIC_KIT_DEF_PORT(i51KitG1ParticleResumeEffect)
i51KIT_DYNAMIC_KIT_DEF_PORT(i51KitG1ParticleDeleteEffect)
i51KIT_DYNAMIC_KIT_DEF_PORT(i51KitG1ParticleSetSpeed)
i51KIT_DYNAMIC_KIT_DEF_PORT(i51KitG1ParticleSetDiffuseAngle)
i51KIT_DYNAMIC_KIT_DEF_PORT(i51KitG1ParticleSetActionRadius)
i51KIT_DYNAMIC_KIT_DEF_PORT(i51KitG1ParticleSetAccel)
i51KIT_DYNAMIC_KIT_DEF_PORT(i51KitG1ParticleSetImage)
i51KIT_DYNAMIC_KIT_DEF_PORT(i51KitG1ParticleSetTime)
i51KIT_DYNAMIC_KIT_DEF_PORT(i51KitG1ParticleAdd)
i51KIT_DYNAMIC_KIT_DEF_PORT(i51KitG1ParticleDraw)
i51KIT_DYNAMIC_KIT_DEF_PORT(i51KitG1ParticleClear)

i51KIT_DYNAMIC_KIT_DEF_END

i51KIT_DYNAMIC_KIT_DEF_TYPE(iBOOL, i51KitG1BladeDraw, (void))
i51KIT_DYNAMIC_KIT_DEF_TYPE(iBOOL, i51KitG1BladeClearTrack, (void))
i51KIT_DYNAMIC_KIT_DEF_TYPE(iBOOL, i51KitG1BladeSetTrack, (iS16 x, iS16 y, iS32 type))
i51KIT_DYNAMIC_KIT_DEF_TYPE(iBOOL, i51KitG1BladeSetColor, (iS16 FillColor, iS16 BorderColor))


i51KIT_DYNAMIC_KIT_DEF_TYPE(iHANDLE,i51KitG1ParticleCreateEffect, (iS32 type, iHANDLE ImageHander, iU32 ImageId, iS16 ImageNum, iS16 MaxCount, iS16 LifeTime))
i51KIT_DYNAMIC_KIT_DEF_TYPE(iBOOL, i51KitG1ParticleStopEffect, (iHANDLE ParticleHandle))
i51KIT_DYNAMIC_KIT_DEF_TYPE(iBOOL, i51KitG1ParticleResumeEffect, (iHANDLE ParticleHandle))
i51KIT_DYNAMIC_KIT_DEF_TYPE(iBOOL, i51KitG1ParticleDeleteEffect, (iHANDLE ParticleHandle))
i51KIT_DYNAMIC_KIT_DEF_TYPE(iBOOL, i51KitG1ParticleSetSpeed, (iHANDLE ParticleHandle))
i51KIT_DYNAMIC_KIT_DEF_TYPE(iBOOL, i51KitG1ParticleSetDiffuseAngle, (iHANDLE ParticleHandle, iS16 angle, iS16 AngleVariance))
i51KIT_DYNAMIC_KIT_DEF_TYPE(iBOOL, i51KitG1ParticleSetActionRadius, (iHANDLE ParticleHandle, iS16 radius))
i51KIT_DYNAMIC_KIT_DEF_TYPE(iBOOL, i51KitG1ParticleSetAccel, (iHANDLE ParticleHandle, iS16 AccelX, iS16 AccelY))
i51KIT_DYNAMIC_KIT_DEF_TYPE(iBOOL, i51KitG1ParticleSetImage, (iHANDLE ParticleHandle, iHANDLE ImageHander, iS16 ImageId, iS16 ImageNum))
i51KIT_DYNAMIC_KIT_DEF_TYPE(iBOOL, i51KitG1ParticleSetTime, (iHANDLE ParticleHandle, iS16 LifeTime, iS16 DecayTime))
i51KIT_DYNAMIC_KIT_DEF_TYPE(iBOOL, i51KitG1ParticleAdd, (iHANDLE ParticleHandle, iS16 count, iS16 x, iS16 y))
i51KIT_DYNAMIC_KIT_DEF_TYPE(iBOOL, i51KitG1ParticleDraw, (iHANDLE ParticleHandle))
i51KIT_DYNAMIC_KIT_DEF_TYPE(iBOOL, i51KitG1ParticleClear, (iHANDLE ParticleHandle))

#ifdef RELEASE_i51KitG1

#define i51KitG1BladeDraw i51KIT_DYNAMIC_KIT_DEF_CALL(i51KitG1,i51KitG1BladeDraw)
#define i51KitG1BladeClearTrack i51KIT_DYNAMIC_KIT_DEF_CALL(i51KitG1,i51KitG1BladeClearTrack)
#define i51KitG1BladeSetTrack i51KIT_DYNAMIC_KIT_DEF_CALL(i51KitG1,i51KitG1BladeSetTrack)
#define i51KitG1BladeSetColor i51KIT_DYNAMIC_KIT_DEF_CALL(i51KitG1,i51KitG1BladeSetColor)

#define i51KitG1ParticleCreateEffect i51KIT_DYNAMIC_KIT_DEF_CALL(i51KitG1,i51KitG1ParticleCreateEffect)
#define i51KitG1ParticleStopEffect i51KIT_DYNAMIC_KIT_DEF_CALL(i51KitG1,i51KitG1ParticleStopEffect)
#define i51KitG1ParticleResumeEffect i51KIT_DYNAMIC_KIT_DEF_CALL(i51KitG1,i51KitG1ParticleResumeEffect)
#define i51KitG1ParticleDeleteEffect i51KIT_DYNAMIC_KIT_DEF_CALL(i51KitG1,i51KitG1ParticleDeleteEffect)
#define i51KitG1ParticleSetSpeed i51KIT_DYNAMIC_KIT_DEF_CALL(i51KitG1,i51KitG1ParticleSetSpeed)
#define i51KitG1ParticleSetDiffuseAngle i51KIT_DYNAMIC_KIT_DEF_CALL(i51KitG1,i51KitG1ParticleSetDiffuseAngle)
#define i51KitG1ParticleSetActionRadius i51KIT_DYNAMIC_KIT_DEF_CALL(i51KitG1,i51KitG1ParticleSetActionRadius)
#define i51KitG1ParticleSetAccel i51KIT_DYNAMIC_KIT_DEF_CALL(i51KitG1,i51KitG1ParticleSetAccel)
#define i51KitG1ParticleSetImage i51KIT_DYNAMIC_KIT_DEF_CALL(i51KitG1,i51KitG1ParticleSetImage)
#define i51KitG1ParticleSetTime i51KIT_DYNAMIC_KIT_DEF_CALL(i51KitG1,i51KitG1ParticleSetTime)
#define i51KitG1ParticleAdd i51KIT_DYNAMIC_KIT_DEF_CALL(i51KitG1,i51KitG1ParticleAdd)
#define i51KitG1ParticleDraw i51KIT_DYNAMIC_KIT_DEF_CALL(i51KitG1,i51KitG1ParticleDraw)
#define i51KitG1ParticleClear i51KIT_DYNAMIC_KIT_DEF_CALL(i51KitG1,i51KitG1ParticleClear)

#endif 

#endif

