#include "i51List.h"
#if 0
typedef struct tagList
{
	struct tagNode *pHead;
	int sum;
}List;

typedef struct tagNode
{
	iWCHAR *pwszCaption;
	int index;
	struct tagNode *pPre;
	struct tagNode *pNext;
}Node;
#endif


int i51ListGetSum( List *pList)
{
	if (pList)
	{
		return pList->sum;
	}
	else
	{
		return 0;
	}
	
}

iBOOL i51ListInit( List *pList)
{
	if (pList)
	{
		pList->sum = 0;
		pList->pHead = iNULL;
		return iTRUE;
	}
	else
	{
		return iFALSE;
	}
}
void i51ListPrint( List *pList)
{   
	Node *pCurNode;
	static iCHAR strTemp[256];
	if (pList)
	{

		//SLOG("-------------------- List Nodes sum = %d --------------------",pList->sum);
		pCurNode = pList->pHead;
		while (pCurNode) 
		{  
			i51AdeStdUnicode2Ascii(pCurNode->wszCaption,strTemp);
			//SLOG("NO.%d = %s",pCurNode->index,strTemp);
			SLOG("%s",strTemp);
			if (!pCurNode->pNext)
			{
				break;
			}
			else
			{
				pCurNode = pCurNode->pNext;
			}
		}
	}
}

iBOOL i51ListInsertNode(List *pList,Node *pNode)
{   
	Node *pAddNode,*pCurNode,*pPreNode;
	int tempIndex;
    if (!pList || !pNode)
    {
         return iFALSE;
    }
	else
	{
		pAddNode = i51AdeOsMalloc(sizeof(Node),__FILE__,__LINE__);
		if (!pAddNode)
		{
			return iFALSE;
		}
		else
		{
			memcpy(pAddNode,pNode,sizeof(Node));
		}
        if (!pList->pHead)
        {
			pAddNode->index = 0;
            pList->pHead = pAddNode;
			pList->sum = 1;
			//pList->pHead->pNext = iNULL;
			//pList->pHead->pPre = iNULL;
			pAddNode->pNext = iNULL;
			pAddNode->pPre = iNULL;
			return iTRUE;
        }
		else
		{
			pCurNode = pList->pHead;
			pPreNode = iNULL;
			tempIndex = 1;
            while (pCurNode->pNext)
            {
				pPreNode = pCurNode;
				pCurNode = pCurNode->pNext;
				tempIndex++;
            }
			pAddNode->index = tempIndex;
			pCurNode->pNext = pAddNode;
			pList->sum++;
			pAddNode->pNext = iNULL;
			pAddNode->pPre = pPreNode;
			return iTRUE;
		}
	}
}

iBOOL i51ListInsertNodeSort(List *pList,Node *pNode)
{   
	Node *pAddNode,*pCurNode,*pPreNode,*pTempNode;
	static iCHAR strAdd[256],strCur[256];
	int tempIndex;
	iBOOL addInArry = iFALSE;
	if (!pList || !pNode)
	{
		return iFALSE;
	}
	else
	{
		pAddNode = i51AdeOsMalloc(sizeof(Node),__FILE__,__LINE__);
		if (!pAddNode)
		{
			return iFALSE;
		}
		else
		{
			memcpy(pAddNode,pNode,sizeof(Node));
		}
		if (!pList->pHead)
		{
			pAddNode->index = 0;
			pList->pHead = pAddNode;
			pList->sum = 1;
			pAddNode->pNext = iNULL;
			pAddNode->pPre = iNULL;
			
			return iTRUE;
		}
		else
		{

			pCurNode = pList->pHead;
			pPreNode = iNULL;
			tempIndex = 1;
			i51AdeStdUnicode2Ascii(pAddNode->wszCaption,strAdd);
			i51AdeStdUnicode2Ascii(pCurNode->wszCaption,strCur);
			if (i51PublicStrCmp(strCur,strAdd) == STR_CMP_BIGER)
			{
				pList->pHead = pAddNode;
				pAddNode->pPre = iNULL;
				pAddNode->pNext = pCurNode;
				pList->sum++;
				return iTRUE;
			}
			else
			{
				while(pCurNode->pNext)
				{
					i51AdeStdUnicode2Ascii(pCurNode->pNext->wszCaption,strCur);
					if (i51PublicStrCmp(strCur,strAdd) == STR_CMP_BIGER)
					{
						pTempNode = pCurNode->pNext;
						pCurNode->pNext = pAddNode;
						pAddNode->pPre = pCurNode;
						pAddNode->pNext = pTempNode;
                        pTempNode->pPre = pAddNode;
						pList->sum++;
						return iTRUE;
					}
					pCurNode = pCurNode->pNext;
				}
			}

			if (!addInArry)
			{
				pList->sum++;
				pCurNode->pNext = pAddNode;
				pAddNode->pPre = pCurNode;
				pAddNode->pNext = iNULL;
				return iTRUE;
			}
		}
	}
}
iBOOL i51ListDelNodeByNode(List *pList, List *pNode)
{

}

iBOOL i51ListDelNodeByCaption(List *pList, iWCHAR *pwszCap)
{  
	Node *pCurNode,*pPreNode;
	if (!pList || !pwszCap )
	{
		return iFALSE;
	}
	else
	{   

		pCurNode = pList->pHead;
		pPreNode = iNULL;
		while(pCurNode)
		{
			if (!i51AdeStdWStrcmp(pCurNode->wszCaption,pwszCap))
			{
				if (pCurNode == pList->pHead)
				{
					pList->pHead = pCurNode->pNext;
					i51AdeOsFree((void *)pCurNode);
					pList->sum --;
					return iTRUE;
				}
				else
				{
                    if (pPreNode)
                    {
						pPreNode->pNext = pCurNode->pNext;
						i51AdeOsFree((void *)pCurNode);
						pList->sum -- ;
						/////
						while(pPreNode->pNext)
						{
							pPreNode->pNext->index--;
							pPreNode = pPreNode->pNext;
						}
						/////
						return iTRUE;
                    }
				}
			}
            if (!pCurNode->pNext)
            {
				break;
            }
			else
			{
				pPreNode = pCurNode;
				pCurNode = pCurNode->pNext;
			}
		}
	}
	return iFALSE;
}
void i51ListSwapNodes(List *pList,Node *pNodeA,Node *pNodeB)
{   
	Node *pNodeTemp;
	if(!pList || !pNodeA || !pNodeB)
	{
		return;
	}
#if 1
	pNodeTemp = (Node *)i51AdeOsMalloc(sizeof(Node),__FILE__,__LINE__);
	if (pNodeTemp)
	{
		memcpy(pNodeTemp,pNodeA,sizeof(Node));
		i51AdeStdWStrcpy(pNodeA->wszCaption,pNodeB->wszCaption);
		i51AdeStdWStrcpy(pNodeB->wszCaption,pNodeTemp->wszCaption);
// 		memcpy(pNodeA->wszCaption,pNodeB,sizeof(Node));
// 		memcpy(pNodeB,pNodeTemp,sizeof(Node));
		i51AdeOsFree((void*)pNodeTemp);
	}
#endif
}

void i51ListSort(List *pList)
{   
	Node *pCurNode;
	static iCHAR strTemp1[256],strTemp2[256];
	int oneSortCountMax,curSortIndex;
	if(!pList)
	{
		return;
	}
	oneSortCountMax = pList->sum;
	while(oneSortCountMax > 1)
	{
		pCurNode = pList->pHead;
		curSortIndex = 0;
		while (pCurNode && curSortIndex < oneSortCountMax)
		{
			if (pCurNode->pNext)
			{
				i51AdeStdUnicode2Ascii(pCurNode->wszCaption,strTemp1);
				i51AdeStdUnicode2Ascii(pCurNode->pNext->wszCaption,strTemp2);
				if (i51PublicStrCmp(strTemp1,strTemp2) == STR_CMP_BIGER)
				{
					//SLOG(" str1 = %s    str2  = %s> 0",strTemp1,strTemp2);
					i51ListSwapNodes(pList,pCurNode,pCurNode->pNext);
				}
				curSortIndex++;
			}
			else
			{
				break;
			}
			if (pCurNode->pNext)
			{
				pCurNode = pCurNode->pNext;

			}
			else
			{
				break;
			}
		}
		//SLOG("--------------------------------");
		oneSortCountMax -- ;
	}

}

iBOOL i51ListDelNodeByIndex(List *pList, int Index)
{

}

iBOOL i51ListDestroy(List *pList)
{
	iBOOL ret = iFALSE;
   if (!pList)
   {
	   return ret;
   }
   else
   {
	   Node *pCurNode,*pTempNode;
	   ret = iTRUE;
	   pCurNode = pList->pHead;
	   while (pCurNode)
	   {   
		   pTempNode = pCurNode;
		   if (pCurNode->pNext)
		   {
			   pCurNode = pCurNode->pNext;
			   i51AdeOsFree((void*)pTempNode);
		   }
		   else
		   {
			   i51AdeOsFree((void*)pTempNode);
			   return ret;
		   }
	   }
   }
}


iBOOL i51ListInsertNodeBeforeOneNode(List *pList,Node *pNode,Node *pBeNode)
{
	iBOOL ret = iFALSE;
	Node *pPreNode,*pAddNode;
	if (!pList || !pNode || !pBeNode)
	{
		return ret;
	}
	else
	{ 
		pAddNode = (Node *)i51AdeOsMalloc(sizeof(Node),__FILE__,__LINE__);
		if (pAddNode)
		{
			memcpy(pAddNode,pNode,sizeof(Node));
			pPreNode = pBeNode->pPre;
			if (pPreNode)
			{
				pPreNode->pNext = pAddNode;
				pAddNode->pNext = pBeNode;
				pBeNode->pPre = pAddNode;
				
			}
			else
			{
				pList->pHead = pAddNode;
				pAddNode->pPre = iNULL;
				pAddNode->pNext = pBeNode; 
			}
			pAddNode->index = 0;
			pList->sum++;
			ret = iTRUE;
			return ret;
		}

	}
	return ret;
}