#ifdef __I51_Explorer__
void i51PortEntryExplorer ()
{
	extern const unsigned char* i51SapGeti51Explorer () ;
	extern const int i51SapGeti51ExplorerSize () ;	

	if ( !i51PortCheckStorage ()) return ;
	if (!i51CheckThirdCpRunning())return ;
	if (!i51CheckKernelInitSucceed ()) return ;

	i51KernelRunStaticApp ( L"i51Explorer" , (unsigned char* )i51SapGeti51Explorer () , (unsigned int )i51SapGeti51ExplorerSize () , 0 , 0 , 1 ) ;	
	
}
#endif
/*
#ifdef __I51_Appstore__
void i51PortEntryAppstore ()
{
	extern const unsigned char* i51SapGetBootMenuSize () ;
	extern const int i51SapAppstoreParameter () ;	

	if (!i51PortCheckStorage ()) return ;
	if (!i51CheckKernelInitSucceed ()) return ;
	#ifdef __I51_SIMPLE_INSTALL__
	i51SimpleLunchApp (L"Appstore") ;
	#else
	i51KernelRunStaticApp ( L"i51SapBooter" , (unsigned char* )i51SapGetBootMenu () , (unsigned int )i51SapGetBootMenuSize () , i51SapAppstoreParameter(), 0 , 1 ) ;	
	#endif
}
#endif
*/

#ifdef __I51_Portal__
void i51PortEntryPortal ()
{
#if defined ( __I51_Appstore__ )
	extern const unsigned char* i51SapAppstoreParameter () ;	
	extern const int i51SapGetBootMenuSize () ;

	if (!i51PortCheckStorage ()) return ;
	if (!i51CheckThirdCpRunning())return ;
	if (!i51CheckKernelInitSucceed ()) return ;
	#ifdef __I51_SIMPLE_INSTALL__
	i51SimpleLunchApp (L"Appstore") ;
	#else
	i51KernelRunStaticApp ( L"i51SapBooter" , (unsigned char* )i51SapGetBootMenu () , (unsigned int )i51SapGetBootMenuSize () , i51SapAppstoreParameter(), 0 , 1 ) ;	
	#endif
#elif defined ( __I51_PortalKE__ )
	extern const unsigned char* i51SapPortalKEParameter () ;
	extern const int i51SapGetBootMenuSize () ;	

	if (!i51PortCheckStorage ()) return ;
	if (!i51CheckThirdCpRunning())return ;
	if (!i51CheckKernelInitSucceed ()) return ;
	#ifdef __I51_SIMPLE_INSTALL__
	i51SimpleLunchApp (L"PortalKE") ;
	#else
	i51KernelRunStaticApp ( L"i51SapBooter" , (unsigned char* )i51SapGetBootMenu () , (unsigned int )i51SapGetBootMenuSize () , i51SapPortalKEParameter(), 0 , 1 ) ;	
	#endif
#else
	extern const unsigned char* i51SapPortalParameter () ;
	extern const int i51SapGetBootMenuSize () ;	

	if (!i51PortCheckStorage ()) return ;
	if (!i51CheckThirdCpRunning())return ;
	if (!i51CheckKernelInitSucceed ()) return ;
	#ifdef __I51_SIMPLE_INSTALL__
	i51SimpleLunchApp (L"Portal") ;
	#else
	i51KernelRunStaticApp ( L"i51SapBooter" , (unsigned char* )i51SapGetBootMenu () , (unsigned int )i51SapGetBootMenuSize () , i51SapPortalParameter(), 0 , 1 ) ;	
	#endif
#endif
}
#endif

#ifdef	__I51_fighter__
void i51PortEntryFighter ()
{
	extern const unsigned char* i51SapfighterParameter () ;
	extern const int i51SapGetBootMenuSize () ;	

	if (!i51PortCheckStorage ()) return ;
	if (!i51CheckThirdCpRunning())return ;
	if (!i51CheckKernelInitSucceed ()) return ;

	#ifdef __I51_SIMPLE_INSTALL__
	i51SimpleLunchApp (L"fighter") ;
	#else		
	i51KernelRunStaticApp ( L"i51SapBooter" , (unsigned char* )i51SapGetBootMenu () , (unsigned int )i51SapGetBootMenuSize () , i51SapfighterParameter(), 0 , 1 ) ;	
	#endif
}
#endif

#ifdef	__I51_FruitShot__
void i51PortEntryFruitShot ()
{
	extern const unsigned char* i51SapFruitShotParameter () ;
	extern const int i51SapGetBootMenuSize () ;	

	if (!i51PortCheckStorage ()) return ;
	if (!i51CheckThirdCpRunning())return ;
	if (!i51CheckKernelInitSucceed ()) return ;
	#ifdef __I51_SIMPLE_INSTALL__
	i51SimpleLunchApp (L"FruitShot") ;
	#else				
	i51KernelRunStaticApp ( L"i51SapBooter" , (unsigned char* )i51SapGetBootMenu () , (unsigned int )i51SapGetBootMenuSize () , i51SapFruitShotParameter(), 0 , 1 ) ;	
	#endif
}
#endif

#ifdef	__I51_Convoy__
void i51PortEntryConvoy()
{
	extern const unsigned char* i51SapConvoyParameter () ;
	extern const int i51SapGetBootMenuSize () ;	

	if (!i51PortCheckStorage ()) return ;
	if (!i51CheckThirdCpRunning())return ;
	if (!i51CheckKernelInitSucceed ()) return ;
	#ifdef __I51_SIMPLE_INSTALL__
	i51SimpleLunchApp (L"Convoy") ;
	#else				
	i51KernelRunStaticApp ( L"i51SapBooter" , (unsigned char* )i51SapGetBootMenu () , (unsigned int )i51SapGetBootMenuSize () , i51SapConvoyParameter(), 0 , 1 ) ;	
	#endif
}
#endif

#ifdef	__I51_Snidget__
void i51PortEntrySnidget()
{
	extern const unsigned char* i51SapSnidgetParameter () ;
	extern const int i51SapGetBootMenuSize () ;	

	if (!i51PortCheckStorage ()) return ;
	if (!i51CheckThirdCpRunning())return ;
	if (!i51CheckKernelInitSucceed ()) return ;
	
	#ifdef __I51_SIMPLE_INSTALL__
	i51SimpleLunchApp (L"Snidget") ;
	#else						
	i51KernelRunStaticApp ( L"i51SapBooter" , (unsigned char* )i51SapGetBootMenu () , (unsigned int )i51SapGetBootMenuSize () , i51SapSnidgetParameter(), 0 , 1 ) ;	
	#endif
	
}
#endif

#ifdef __I51_CutFruit__
void i51PortEntryCutFruit()
{

	extern const unsigned char* i51SapCutFruitParameter () ;
	extern const int i51SapGetBootMenuSize () ;	

	if (!i51PortCheckStorage ()) return ;
	if (!i51CheckThirdCpRunning())return ;
	if (!i51CheckKernelInitSucceed ()) return ;

	#ifdef __I51_SIMPLE_INSTALL__
	i51SimpleLunchApp (L"CutFruit") ;
	#else				
	i51KernelRunStaticApp ( L"i51SapBooter" , (unsigned char* )i51SapGetBootMenu () , (unsigned int )i51SapGetBootMenuSize () , i51SapCutFruitParameter(), 0 , 1 ) ;	
	#endif
	
}
#endif


#ifdef	__I51_SavageAdventure__
void i51PortEntrySavageAdventure()
{

	extern const unsigned char* i51SapSavageAdventureParameter () ;
	extern const int i51SapGetBootMenuSize () ;	

	if (!i51PortCheckStorage ()) return ;
	if (!i51CheckThirdCpRunning())return ;
	if (!i51CheckKernelInitSucceed ()) return ;

	#ifdef __I51_SIMPLE_INSTALL__
	i51SimpleLunchApp (L"SavageAdventure") ;
	#else		
	i51KernelRunStaticApp ( L"i51SapBooter" , (unsigned char* )i51SapGetBootMenu () , (unsigned int )i51SapGetBootMenuSize () , i51SapSavageAdventureParameter(), 0 , 1 ) ;	
	#endif
}
#endif


#ifdef	__I51_VexedJewels__
void i51PortEntryVexedJewels()
{

	extern const unsigned char* i51SapVexedJewelsParameter () ;
	extern const int i51SapGetBootMenuSize () ;	

	if (!i51PortCheckStorage ()) return ;
	if (!i51CheckThirdCpRunning())return ;
	if (!i51CheckKernelInitSucceed ()) return ;

	#ifdef __I51_SIMPLE_INSTALL__
	i51SimpleLunchApp (L"VexedJewels") ;
	#else			
	i51KernelRunStaticApp ( L"i51SapBooter" , (unsigned char* )i51SapGetBootMenu () , (unsigned int )i51SapGetBootMenuSize () , i51SapVexedJewelsParameter(), 0 , 1 ) ;	
	#endif
}
#endif

#ifdef	__I51_X_Space__
void i51PortEntryX_Space()
{

	extern const unsigned char* i51SapX_SpaceParameter () ;
	extern const int i51SapGetBootMenuSize () ;	

	if (!i51PortCheckStorage ()) return ;
	if (!i51CheckThirdCpRunning())return ;
	if (!i51CheckKernelInitSucceed ()) return ;

	#ifdef __I51_SIMPLE_INSTALL__
	i51SimpleLunchApp (L"X_Space") ;
	#else			
	i51KernelRunStaticApp ( L"i51SapBooter" , (unsigned char* )i51SapGetBootMenu () , (unsigned int )i51SapGetBootMenuSize () , i51SapX_SpaceParameter(), 0 , 1 ) ;	
	#endif
}
#endif

#ifdef	__I51_DontRun__
void i51PortEntryDontRun()
{

	extern const unsigned char* i51SapDontRunParameter () ;
	extern const int i51SapGetBootMenuSize () ;	

	if (!i51PortCheckStorage ()) return ;
	if (!i51CheckThirdCpRunning())return ;
	if (!i51CheckKernelInitSucceed ()) return ;

	#ifdef __I51_SIMPLE_INSTALL__
	i51SimpleLunchApp (L"DontRun") ;
	#else				
	i51KernelRunStaticApp ( L"i51SapBooter" , (unsigned char* )i51SapGetBootMenu () , (unsigned int )i51SapGetBootMenuSize () , i51SapDontRunParameter(), 0 , 1 ) ;	
	#endif
}
#endif


#ifdef	__I51_Mjump__
void i51PortEntryMjump()
{

	extern const unsigned char* i51SapMjumpParameter () ;
	extern const int i51SapGetBootMenuSize () ;	

	if (!i51PortCheckStorage ()) return ;
	if (!i51CheckThirdCpRunning())return ;
	if (!i51CheckKernelInitSucceed ()) return ;

	#ifdef __I51_SIMPLE_INSTALL__
	i51SimpleLunchApp (L"Mjump") ;
	#else	
	i51KernelRunStaticApp ( L"i51SapBooter" , (unsigned char* )i51SapGetBootMenu () , (unsigned int )i51SapGetBootMenuSize () , i51SapMjumpParameter(), 0 , 1 ) ;	
	#endif
	
}
#endif

#ifdef	__I51_DDZ__
void i51PortEntryDDZ()
{

	extern const unsigned char* i51SapDDZParameter () ;
	extern const int i51SapGetBootMenuSize () ;	

	if (!i51PortCheckStorage ()) return ;
	if (!i51CheckThirdCpRunning())return ;
	if (!i51CheckKernelInitSucceed ()) return ;

	#ifdef __I51_SIMPLE_INSTALL__
	i51SimpleLunchApp (L"DDZ") ;
	#else			
	i51KernelRunStaticApp ( L"i51SapBooter" , (unsigned char* )i51SapGetBootMenu () , (unsigned int )i51SapGetBootMenuSize () , i51SapDDZParameter(), 0 , 1 ) ;	
	#endif
}
#endif

#ifdef	__I51_HundredStorey__
void i51PortEntryHundredStorey()
{

	extern const unsigned char* i51SapHundredStoreyParameter () ;
	extern const int i51SapGetBootMenuSize () ;	

	if (!i51PortCheckStorage ()) return ;
	if (!i51CheckThirdCpRunning())return ;
	if (!i51CheckKernelInitSucceed ()) return ;

	#ifdef __I51_SIMPLE_INSTALL__
	i51SimpleLunchApp (L"HundredStorey") ;
	#else			
	i51KernelRunStaticApp ( L"i51SapBooter" , (unsigned char* )i51SapGetBootMenu () , (unsigned int )i51SapGetBootMenuSize () , i51SapHundredStoreyParameter(), 0 , 1 ) ;	
	#endif
}
#endif

#ifdef	__I51_RaceCar__
void i51PortEntryRaceCar()
{

	extern const unsigned char* i51SapRaceCarParameter () ;
	extern const int i51SapGetBootMenuSize () ;	

	if (!i51PortCheckStorage ()) return ;
	if (!i51CheckThirdCpRunning())return ;
	if (!i51CheckKernelInitSucceed ()) return ;

	#ifdef __I51_SIMPLE_INSTALL__
	i51SimpleLunchApp (L"RaceCar") ;
	#else				
	i51KernelRunStaticApp ( L"i51SapBooter" , (unsigned char* )i51SapGetBootMenu () , (unsigned int )i51SapGetBootMenuSize () , i51SapRaceCarParameter(), 0 , 1 ) ;	
	#endif
}
#endif

#ifdef	__I51_FishingMaster__
void i51PortEntryFishingMaster()
{

	extern const unsigned char* i51SapFishingMasterParameter () ;
	extern const int i51SapGetBootMenuSize () ;	

	if (!i51PortCheckStorage ()) return ;
	if (!i51CheckThirdCpRunning())return ;
	if (!i51CheckKernelInitSucceed ()) return ;

	#ifdef __I51_SIMPLE_INSTALL__
	i51SimpleLunchApp (L"FishingMaster") ;
	#else				
	i51KernelRunStaticApp ( L"i51SapBooter" , (unsigned char* )i51SapGetBootMenu () , (unsigned int )i51SapGetBootMenuSize () , i51SapFishingMasterParameter(), 0 , 1 ) ;	
	#endif
}
#endif

#ifdef	__I51_JigsawPuzzle__
void i51PortEntryJigsawPuzzle()
{

	extern const unsigned char* i51SapJigsawPuzzleParameter () ;
	extern const int i51SapGetBootMenuSize () ;	

	if (!i51PortCheckStorage ()) return ;
	if (!i51CheckThirdCpRunning())return ;
	if (!i51CheckKernelInitSucceed ()) return ;

	#ifdef __I51_SIMPLE_INSTALL__
	i51SimpleLunchApp (L"JigsawPuzzle") ;
	#else			
	i51KernelRunStaticApp ( L"i51SapBooter" , (unsigned char* )i51SapGetBootMenu () , (unsigned int )i51SapGetBootMenuSize () , i51SapJigsawPuzzleParameter(), 0 , 1 ) ;	
	#endif
}
#endif


#ifdef __I51_AngryBird__
	//	·ßÅ­µÄÐ¡Äñ
void i51PortEntryAngryBird()
{

	extern const unsigned char* i51SapAngryBirdParameter () ;
	extern const int i51SapGetBootMenuSize () ;	

	if (!i51PortCheckStorage ()) return ;
	if (!i51CheckThirdCpRunning())return ;
	if (!i51CheckKernelInitSucceed ()) return ;

	#ifdef __I51_SIMPLE_INSTALL__
	i51SimpleLunchApp (L"AngryBird") ;
	#else			
	i51KernelRunStaticApp ( L"i51SapBooter" , (unsigned char* )i51SapGetBootMenu () , (unsigned int )i51SapGetBootMenuSize () , i51SapAngryBirdParameter(), 0 , 1 ) ;	
	#endif
}
#endif

#ifdef __I51_GlassDestroyer__
	//	ÇÃ²£Á§
void i51PortEntryGlassDestroyer()
{

	extern const unsigned char* i51SapGlassDestroyerParameter () ;
	extern const int i51SapGetBootMenuSize () ;	

	if (!i51PortCheckStorage ()) return ;
	if (!i51CheckThirdCpRunning())return ;
	if (!i51CheckKernelInitSucceed ()) return ;

	#ifdef __I51_SIMPLE_INSTALL__
	i51SimpleLunchApp (L"GlassDestroyer") ;
	#else			
	i51KernelRunStaticApp ( L"i51SapBooter" , (unsigned char* )i51SapGetBootMenu () , (unsigned int )i51SapGetBootMenuSize () , i51SapGlassDestroyerParameter(), 0 , 1 ) ;	
	#endif
}

#endif

#ifdef __I51_PullSkirt__
	//	´µÈ¹×Ó
void i51PortEntryPullSkirt()
{

	extern const unsigned char* i51SapPullSkirtParameter () ;
	extern const int i51SapGetBootMenuSize () ;	

	if (!i51PortCheckStorage ()) return ;
	if (!i51CheckThirdCpRunning())return ;
	if (!i51CheckKernelInitSucceed ()) return ;

	#ifdef __I51_SIMPLE_INSTALL__
	i51SimpleLunchApp (L"PullSkirt") ;
	#else			
	i51KernelRunStaticApp ( L"i51SapBooter" , (unsigned char* )i51SapGetBootMenu () , (unsigned int )i51SapGetBootMenuSize () , i51SapPullSkirtParameter(), 0 , 1 ) ;	
	#endif
}

#endif

#ifdef __I51_TOM__
	//	tomÃ¨
void i51PortEntryTOM()
{

	extern const unsigned char* i51SapTOMParameter () ;
	extern const int i51SapGetBootMenuSize () ;	

	if (!i51PortCheckStorage ()) return ;
	if (!i51CheckThirdCpRunning())return ;
	if (!i51CheckKernelInitSucceed ()) return ;

	#ifdef __I51_SIMPLE_INSTALL__
	i51SimpleLunchApp (L"TOM") ;
	#else			
	i51KernelRunStaticApp ( L"i51SapBooter" , (unsigned char* )i51SapGetBootMenu () , (unsigned int )i51SapGetBootMenuSize () , i51SapTOMParameter(), 0 , 1 ) ;	
	#endif
}

#endif

#ifdef __I51_NinJump__
	//	ÈÌÕßÌøÔ¾
void i51PortEntryNinJump()
{

	extern const unsigned char* i51SapNinJumpParameter () ;
	extern const int i51SapGetBootMenuSize () ;	

	if (!i51PortCheckStorage ()) return ;
	if (!i51CheckThirdCpRunning())return ;
	if (!i51CheckKernelInitSucceed ()) return ;

	#ifdef __I51_SIMPLE_INSTALL__
	i51SimpleLunchApp (L"NinJump") ;
	#else			
	i51KernelRunStaticApp ( L"i51SapBooter" , (unsigned char* )i51SapGetBootMenu () , (unsigned int )i51SapGetBootMenuSize () , i51SapNinJumpParameter(), 0 , 1 ) ;	
	#endif
}

#endif

#ifdef __I51_PlantsVsZombies__
	//	Ö²Îï´óÕ½½©Ê¬
void i51PortEntryPlantsVsZombies()
{

	extern const unsigned char* i51SapPlantsVsZombiesParameter () ;
	extern const int i51SapGetBootMenuSize () ;	 

	if (!i51PortCheckStorage ()) return ;
	if (!i51CheckThirdCpRunning())return ;
	if (!i51CheckKernelInitSucceed ()) return ;

	#ifdef __I51_SIMPLE_INSTALL__
	i51SimpleLunchApp (L"PlantsVsZombies") ;
	#else			
	i51KernelRunStaticApp ( L"i51SapBooter" , (unsigned char* )i51SapGetBootMenu () , (unsigned int )i51SapGetBootMenuSize () , i51SapPlantsVsZombiesParameter(), 0 , 1 ) ;	
	#endif
}
#endif

#ifdef __I51_VXin__
void i51PortEntryVXin()
{

	extern const unsigned char* i51SapVXinParameter () ;
	extern const int i51SapGetBootMenuSize () ;	 

	if (!i51PortCheckStorage ()) return ;
	if (!i51CheckThirdCpRunning())return ;
	if (!i51CheckKernelInitSucceed ()) return ;

	#ifdef __I51_SIMPLE_INSTALL__
	i51SimpleLunchApp (L"VXin") ;
	#else			
	i51KernelRunStaticApp ( L"i51SapBooter" , (unsigned char* )i51SapGetBootMenu () , (unsigned int )i51SapGetBootMenuSize () , i51SapVXinParameter(), 0 , 1 ) ;	
	#endif
}
#endif

#ifdef __I51_RipClothes__
void i51PortEntryRipClothes()
{

	extern const unsigned char* i51SapRipClothesParameter () ;
	extern const int i51SapGetBootMenuSize () ;	

	if (!i51PortCheckStorage ()) return ;
	if (!i51CheckThirdCpRunning())return ;
	if (!i51CheckKernelInitSucceed ()) return ;

	#ifdef __I51_SIMPLE_INSTALL__
	i51SimpleLunchApp (L"RipClothes") ;
	#else			
	i51KernelRunStaticApp ( L"i51SapBooter" , (unsigned char* )i51SapGetBootMenu () , (unsigned int )i51SapGetBootMenuSize () , i51SapRipClothesParameter(), 0 , 1 ) ;	
	#endif
}
#endif

