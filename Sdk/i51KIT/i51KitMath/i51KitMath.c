
#include "i51KitAde.h"
#include "i51KitMath.h"

i51KIT_STATIC_KIT_DEF(Adapter);

iS16 i51KitMathAbsIS16(iS16 Num )
{
	// author : Otto
	// since : 2012.7.26
	// (C) 2012 , PKIG Tech. Co., Ltd.

	// iS16 i : 需要计算的数据
	// notes :  返回iS16数据的绝对值 

	return Num>0?Num:-Num;
	
}

iS32 i51KitMathAbsIS32(iS32 Num )
{
	// author : Otto
	// since : 2012.7.26
	// (C) 2012 , PKIG Tech. Co., Ltd.

	// iS32 i : 需要计算的数据
	// notes :  返回iS32数据的绝对值 

	return Num>0?Num:-Num;
	
}

iS32 i51KitMathPow(iS32 X, iS32 Y )
{
	// author : Otto
	// since : 2012.7.26
	// (C) 2012 , PKIG Tech. Co., Ltd.

	// iS32 X : 底数
	// iS32 Y : 指数
	// notes :  返回X的Y次方

	iS32 Data = 1;
	iS32 i = 0;
	
	for( i=0; i<Y; i++ )
	{
		Data *= X;
	}
	
	return Data;
	
}

iS32 i51KitMathPow10(iS32 P )
{
	// author : Otto
	// since : 2012.7.26
	// (C) 2012 , PKIG Tech. Co., Ltd.

	// iS32 P : 指数
	// notes :  返回10的P次方

	iS32 Data = 1;
	iS32 i = 0;
	
	for( i=0; i<P; i++ )
	{
		Data *= 10;
	}
	
	return Data;
	
}

iU32 i51KitMathSqrt( iU32 Num )
{
	// author : Otto
	// since : 2012.7.26
	// (C) 2012 , PKIG Tech. Co., Ltd.

	// iS32 Num : 代开方数
	// notes :  返回根号Num的整数部分

	iU32 i = 0;
	iU32 Start = 1;
	iU32 End = Num;
	iU32 Temp = 0;

	if( Num==0 || Num==1 ) return Num;
	if( Num>100000000 )
	{
		End = 65535;
		Start = 10000;
	}
	else if(Num>1000000)
	{
		End = 10000;
		Start = 1000;
			
	}
	else if( Num>10000 )
	{
		End = 1000;
		Start = 100;
	}
	else if( Num>100 )
	{
		End = 100;
		Start = 10;
	}
	
	i = (Start+End)/2;
	
	while(1)
	{
		Temp = i*i;
		if( Temp <Num )
		{
			Start = i;
		}
		else if( Temp>Num )
		{
			End = i;
		}
		else
		{
			break;
		}
		i = (Start+End)/2;
		if(i==Start) break;

	}
	
	return i;

}

iS32 SinData[91] = 
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


iS32 TanData[91] = 
{
	0,285,572,858,1145,1433,1722,2011,2302,2594,2888,
	3184,3482,3782,4084,4390,4698,5009,5323,5641,5963,
	6289,6619,6954,7294,7639,7991,8348,8711,9081,9459,
	9844,10237,10639,11051,11472,11903,12346,12800,13267,13747,
	14242,14752,15278,15821,16384,16966,17569,18196,18847,19525,
	20232,20970,21742,22550,23398,24290,25229,26219,27267,28377,
	29557,30813,32155,33592,35135,36799,38598,40551,42681,45014,
	47582,50424,53589,57137,61145,65712,70966,77080,84288,92918,
	103444,116578,133436,155883,187269,234302,312625,469176,938638,0X7FFFFFFF,
};

iS32 i51KitMathSin( iS32 Angle)
{
	// author : Otto
	// since : 2012.7.26
	// (C) 2012 , PKIG Tech. Co., Ltd.

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

iS32 i51KitMathCos( iS32 Angle)
{
	// author : Otto
	// since : 2012.7.26
	// (C) 2012 , PKIG Tech. Co., Ltd.

	//iS32 Angle: 需要计算的角度值
	// notes : 返回角度Angle的余玄放大16384倍(相当于左移14位)的值

	iS32 ActualAngle = 0;
	iS32 Ret = 0;

	Angle+=90;

	ActualAngle = Angle%360;
	if( ActualAngle<0) ActualAngle = 360+ActualAngle;

	if(ActualAngle<=90) Ret = SinData[ActualAngle];
	else if ( ActualAngle<=180) Ret = SinData[180-ActualAngle];
	else if ( ActualAngle<=270) Ret = -SinData[ActualAngle-180];
	else Ret = -SinData[360-ActualAngle];

	return Ret;
	
}

iS32 i51KitMathTan( iS32 Angle)
{
	// author : Otto
	// since : 2012.7.26
	// (C) 2012 , PKIG Tech. Co., Ltd.

	//iS32 Angle: 需要计算的角度值
	// notes : 返回角度Angle的正切放大16384倍(相当于左移14位)的值

	iS32 ActualAngle = 0;
	iS32 Ret = 0;

	ActualAngle = Angle%180;
	if( ActualAngle<0 ) ActualAngle += 180;
	
	if( ActualAngle>90 )
	{
		ActualAngle = 180 - ActualAngle;
		Ret = -TanData[ActualAngle];
	}
	else
	{
		Ret = TanData[ActualAngle];
	}

	return Ret;
	
}

iS32 i51KitMathCot( iS32 Angle)
{
	// author : Otto
	// since : 2012.7.26
	// (C) 2012 , PKIG Tech. Co., Ltd.

	//iS32 Angle: 需要计算的角度值
	// tan(90°-α)=cotα
	// notes : 返回角度Angle的正切放大16384倍(相当于左移14位)的值

	iS32 ActualAngle = 0;
	iS32 Ret = 0;

	Angle = 90 -Angle;
	
	ActualAngle = Angle%180;
	if( ActualAngle<0 ) ActualAngle += 180;

	if( ActualAngle>90 )
	{
		ActualAngle = 180 - ActualAngle;
		Ret = -TanData[ActualAngle];
	}
	else
	{
		Ret = TanData[ActualAngle];
	}

	return Ret;
	
}

void i51KitMathTest( void )
{
	int i = 0;
	static iCHAR Flag = iTRUE, count = 0;;

	if( Flag)
	{
		for( i =0; i<360; i++ )
		{
			iS32 X = i;
//			X = rand() - 15000;
			iLog("X:%6d,%6d,%6d,%6d,%6d,%8d",
				X,i51KitMathSqrt(i51KitMathAbsIS32(X)),i51KitMathSin(X),i51KitMathCos(X),
				i51KitMathTan(X),i51KitMathCot(X));
		}
		count++;
		count=20;
		if( count>=10 ) Flag = iFALSE;
	}
}

iKIT iBOOL i51KitMain ( iS32 aid , iU32 message , void* parameter )
{

	//	TODO :	消息循环

	switch ( message )
	{
		
		case i51_MSG_PAINT:
			i51KitMathTest();
			break;
		case i51_MSG_INIT :
		{
	
			i51KIT_STATIC_KIT_SET(Adapter,parameter);
			i51KIT_STATIC_KIT_ASSERT(Adapter);
	
		}
		break ;
		
		case i51_MSG_RUN :
		break ;

		case i51_MSG_PAUSE :
		break ;

		case i51_MSG_EXIT :
		break ;
		

	}

	

	return iTRUE ;

}

iKIT iU32* i51KitPort ()
{

	//	TODO : Initialize API Port for external layer
	
	//	i51KIT_DYNAMIC_KIT_MAPING_BEGIN(i51KitMath)
	//	i51KIT_DYNAMIC_KIT_MAPING_END 

	//	return i51KIT_DYNAMIC_KIT_GET_MAPING(i51KitMath) ;

	return 0 ;

}
