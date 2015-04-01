#
#	Important :	This software is protected	by	Copyright and the	information
#					contained herein is confidential. Distribution,	reproduction
#					as	well as exploitation	and transmission of its	contents	is
#					not allowed	except if expressly permitted. Infringments
#					result in damage claims
#					Copyright Wavecom	S.A. (c)	$JustDate:$
#

# ****************************************************************************
# *  File	 :	$Workfile:$			$Revision:$
# *--------------------------------------------------------------------------*
# *  Scope	 :	  Description of the	librarie	to	generate							  *
# *																								  *
# *  Notes	 :	  to launch	the compilation of the current library,			  *
# *				  type from	output directory :										  *
# *						 make	-r	-f	..//gen.mak	TARGET=<target>					  *
# *																								  *
# *																								  *
# *																								  *
# *  Changes	 :																				  *
# *--------------------------------------------------------------------------*
# *  Version VSS	| Date			  | Author |	Modification					  *
# *--------------------------------------------------------------------------*
# *	$Log:$
#
# ****************************************************************************

#SCE_OPTIONS = -R -G -Cysc.cfg -N
#SCE_OPTIONS = -R -N

#------------------------------------------------------------------------------
#	Description	des variables propre	a la bibliothèque
#	Description	of	some variables	own to the librairie
#------------------------------------------------------------------------------
# Library module (lib) or Binary	module (bin)
PROCESS		= lib


#------------------------------------------------------------------------------
#	Liste	des fichiers sources	de	la	bibliothèque ou de l'éxecutable a générer
#	List of source	files	of	the library	or	executable to generate
#------------------------------------------------------------------------------

#Add files here

#header files
PATH_H += \
  $(PATH_srv)/inc\
  $(PATH_srv)/pub\
  $(PATH_SrvWrapper)/pub\
  $(PATH_widget)/pub\
  $(PATH_widget)/inc\
  $(PATH_mmicomn)/cfg\
  $(PATH_mmicomn)/inc\
  $(PATH_AppLauncher)/inc\
  $(PATH_CusDefine)/pub\
  $(PATH_model)/pub\
  $(PATH_model)/inc\
  $(PATH_build)


#i51Adapter source code               
REL_SRC_C_LIST += i51AdeMmi.c
REL_SRC_C_LIST += i51AdeOs.c        
REL_SRC_C_LIST += i51AdeSms.c
REL_SRC_C_LIST += i51AdeSoc.c    
REL_SRC_C_LIST += i51AdeStd.c   
REL_SRC_C_LIST += i51AdeCnf.c
                                
#i51Kernel source code               
REL_SRC_C_LIST += i51Kernel.c

#i51Sap source code
REL_SRC_C_LIST += i51Sap.c
                                
#i51SlApi source code             
REL_SRC_C_LIST += i51SlLoader.c   
REL_SRC_C_LIST += i51SlStruct.c    
REL_SRC_C_LIST += i51SlZlib.c     
REL_SRC_C_LIST += i51SlDl.c

#i51Port source code              
REL_SRC_C_LIST += i51Port.c
REL_SRC_C_LIST += i51CusGame.c
REL_SRC_C_LIST += i51ManagedApp.c






                                