#include "i51Widget.h"

void i51WidgetScrollListView(ListView *lv,SCROLL_DIRT dir,iS16 offset)
{
	if (offset == 0 || !lv)
	{
		return ;
	}
	if (dir == SCROLL_DIRT_VERTICAL_UP)
	{
	    lv->yRel -= offset;
		if (lv->yRel <= lv->rcCtr.Y)
		{
			lv->yRel = 0;
		}
	}
	else if (dir == SCROLL_DIRT_VERTICAL_DOWN)
	{
		lv->yRel += offset;
		if (lv->yRel >= lv->rcCtr.Y+lv->rcCtr.Height)
		{
			lv->yRel = lv->rcCtr.Y+lv->rcCtr.Height;
		}
	}
}
#if 1
void i51WidgetScrollDrawOneItem(ListView *lv,Node *pCurNode)
{
	iRECT tempRc,tagRc,lapRc,txtRc,iconRc;
	if (pCurNode)
	{
		memcpy(&tempRc,&pCurNode->rc,sizeof(iRECT));
		memcpy(&txtRc,&pCurNode->rc,sizeof(iRECT));
		memcpy(&iconRc,&pCurNode->rc,sizeof(iRECT));
		iconRc.Width = ICON_W;
		iconRc.Height = ICON_W;
		iconRc.X += pCurNode->rc.Width/15;
		iconRc.Y += (pCurNode->rc.Height-iconRc.Height)/2;

		txtRc.X += iconRc.X+iconRc.Width+pCurNode->rc.Width/15;
		txtRc.Width -= (iconRc.X+iconRc.Width);

		i51FrameShiftXY(&tempRc,0,-lv->yRel);
		i51FrameShiftXY(&iconRc,0,-lv->yRel);
		i51FrameShiftXY(&txtRc,0,-lv->yRel);
		i51FrameGetOverlapArea(&tempRc,&lv->rcCtr,&lapRc);
		//i51KitG2CleanScreenEx(pCurNode->icIdle,&tempRc);
		if (pCurNode->curState == ITEM_STATE_IDLE)
		{
			i51KitG2CleanScreenEx(pCurNode->icIdleCompleten,&lapRc);
		}
		else if (pCurNode->curState == ITEM_STATE_DOWN)
		{
			i51KitG2CleanScreenEx(pCurNode->icDown,&lapRc);
		}
		i51FrameGetOverlapArea(&iconRc,&lv->rcCtr,&lapRc);
		//if (pCurNode->type == ITEM_TYPE_APP)
		{
			i51KitG2CleanScreenEx(pCurNode->icon,&lapRc);
		}
		
		if (pCurNode->bCompleten)
		{
			i51PublicDrawSysWStr(pCurNode->wszCaption,&txtRc,pCurNode->txtCompleten,DRAW_STYLE_H_LEFT,DRAW_STYLE_V_MID);
		}
		else
		{
			i51PublicDrawSysWStr(pCurNode->wszCaption,&txtRc,pCurNode->txtUnCompleten,DRAW_STYLE_H_LEFT,DRAW_STYLE_V_MID);
		}
		
	}

}

#endif
void i51WidgetScrollDrawOneItemBaseLapRect(ListView *lv,Node *pCurNode,iRECT *pTagRc)
{
	iRECT tempRc,tagRc,lapRc,txtRc,iconRc;
	iS32 Point[2][2];
	iU32 tCount1,tCount2;
	if (pCurNode)
	{ 
		if (i51FrameGetOverlapArea(pTagRc,&pCurNode->rc,&tempRc))
		{
            //tCount1 = i51AdeOsGetTick();
			memcpy(&tempRc,&pCurNode->rc,sizeof(iRECT));
			memcpy(&txtRc,&pCurNode->rc,sizeof(iRECT));
			memcpy(&iconRc,&pCurNode->rc,sizeof(iRECT));
			iconRc.Width = ICON_W;
			iconRc.Height = ICON_W;
			iconRc.X += pCurNode->rc.Width/15;
			iconRc.Y += (pCurNode->rc.Height-iconRc.Height)/2;

			txtRc.X += iconRc.X+iconRc.Width+pCurNode->rc.Width/15;
			txtRc.Width -= (iconRc.X+iconRc.Width);

			i51FrameShiftXY(&tempRc,0,-lv->yRel);
			i51FrameShiftXY(&iconRc,0,-lv->yRel);
			i51FrameShiftXY(&txtRc,0,-lv->yRel);
			i51FrameGetOverlapArea(&tempRc,&lv->rcCtr,&lapRc);
			if (pCurNode->curState == ITEM_STATE_IDLE)
			{
				i51KitG2CleanScreenEx(pCurNode->icIdleCompleten,&lapRc);
			}
			else if (pCurNode->curState == ITEM_STATE_DOWN)
			{
				i51KitG2CleanScreenEx(pCurNode->icDown,&lapRc);
			}
			i51FrameGetOverlapArea(&iconRc,&lv->rcCtr,&lapRc);
			//i51KitG2CleanScreenEx(pCurNode->icon,&lapRc);
			//if (pCurNode->type == ITEM_TYPE_APP)
			{
				i51KitG2CleanScreenEx(pCurNode->icon,&lapRc);
			}
			if (pCurNode->bCompleten)
			{
				i51PublicDrawSysWStr(pCurNode->wszCaption,&txtRc,pCurNode->txtCompleten,DRAW_STYLE_H_LEFT,DRAW_STYLE_V_MID);
			}
			else
			{
				i51PublicDrawSysWStr(pCurNode->wszCaption,&txtRc,pCurNode->txtUnCompleten,DRAW_STYLE_H_LEFT,DRAW_STYLE_V_MID);
			}
			//tCount2 = i51AdeOsGetTick();
			//SLOG("tCount = %d",(tCount2 - tCount1));
#if 0
			Point[0][0] = tempRc.X;
			Point[0][1] = tempRc.Y+tempRc.Height-1;
			Point[1][0] = tempRc.X+tempRc.Width;
			Point[1][1] = tempRc.Y+tempRc.Height-1;
			i51KitG2DrawLineEx(Point,COLOR_BLACK);
#endif
		}
	}

}

void i51WidgetScrollSetAllIdleState(ListView *lv)
{
	if (!lv)
	{
		return;
	}
	//if (i51PubicPtInRect(x,y,&(lv->rcCtr)))
	{
		Node *pCurNode;
		iRECT tempRc;
		pCurNode = lv->ls.pHead;
		while (pCurNode)
		{   
			pCurNode->curState = ITEM_STATE_IDLE;
			if (pCurNode->preState !=  pCurNode->curState)
			{
				i51WidgetScrollDrawOneItem(lv,pCurNode);
				pCurNode->preState = pCurNode->curState;
			}
			if(pCurNode->pNext)
			{
				pCurNode = pCurNode->pNext;
			}
			else
			{
				break;
			}
		}
	}
}
Node* i51WidgetScrollGetNodeInfo(ListView *lv,iS16 x,iS16 y)
{
	if (!lv)
	{
		return iNULL;
	}
	if (i51PubicPtInRect(x,y,&(lv->rcCtr)))
	{
		Node *pCurNode;
		iRECT tempRc;
		pCurNode = lv->ls.pHead;


		while (pCurNode)
		{   
			memcpy(&tempRc,&pCurNode->rc,sizeof(iRECT));
			i51FrameShiftXY(&tempRc,0,-lv->yRel);
			if (i51PubicPtInRect(x,y,&tempRc))
			{  
				return pCurNode;
			}
			if(pCurNode->pNext)
			{
				pCurNode = pCurNode->pNext;
			}
			else
			{
				break;
			}
		}
	}
	return iNULL;
}
void i51WidgetScrollSetItemActived(ListView *lv,iS16 x,iS16 y)
{   
	if (!lv)
	{
		return;
	}
	if (i51PubicPtInRect(x,y,&(lv->rcCtr)))
	{
		Node *pCurNode;
		iRECT tempRc;
		pCurNode = lv->ls.pHead;
		while (pCurNode)
		{   
			memcpy(&tempRc,&pCurNode->rc,sizeof(iRECT));
			i51FrameShiftXY(&tempRc,0,-lv->yRel);
			if (i51PubicPtInRect(x,y,&tempRc))
			{  
				pCurNode->curState = ITEM_STATE_DOWN;
				if (pCurNode->preState !=  pCurNode->curState)
				{
                    i51WidgetScrollDrawOneItem(lv,pCurNode);
					pCurNode->preState = pCurNode->curState;
				}
				break;
			}
			if(pCurNode->pNext)
			{
				pCurNode = pCurNode->pNext;
			}
			else
			{
				break;
			}
		}
   }
}
void i51WidgetScrollDrawBar(ListView *lv)
{
	iRECT tempRc;
	memcpy(&tempRc,&lv->bar.rc,sizeof(iRECT));
	tempRc.Y = lv->rcCtr.Y;
	tempRc.Height = lv->rcCtr.Height;
	i51KitG2CleanScreenEx(COLOR_WHITE,&tempRc);
	///////////
	if (lv->bBarVisible && lv->hAbs > lv->rcCtr.Height)
	{
		memcpy(&tempRc,&lv->bar.rc,sizeof(iRECT));
		tempRc.Y = lv->yRel*lv->rcCtr.Height/lv->hAbs+lv->rcCtr.Y;
		i51KitG2CleanScreenEx(lv->bar.icIdle,&tempRc);
	}
}
void i51WidgetScrollDraw(ListView *lv,iS16 dstX,iS16 dstY,iRECT *prc,SCROLL_REDRAW_LIMIT limit)//dstX,dstY,prc 都是相对lv的X,Y为原点坐标
{
	Node *pCurNode;
	iRECT tempRc,tagRc,lapRc,txtRc,iconRc;
	if (!lv || !prc)
	{
		return;
	}
	pCurNode = lv->ls.pHead;
    memcpy(&tagRc,prc,sizeof(iRECT));

	tagRc.X = dstX+lv->rcCtr.X;
	tagRc.Y = dstY+lv->rcCtr.Y+lv->yRel;

	while (pCurNode)
	{
		if (limit == SCROLL_REDRAW_LIMIT_NONE)
		{
			if ((pCurNode->rc.Y >= tagRc.Y && pCurNode->rc.Y <= tagRc.Y+tagRc.Height)
				||(pCurNode->rc.Y+pCurNode->rc.Height >= tagRc.Y && pCurNode->rc.Y+pCurNode->rc.Height <= tagRc.Y+tagRc.Height)
				||( pCurNode->rc.Y<= tagRc.Y && pCurNode->rc.Y+pCurNode->rc.Height >= tagRc.Y+tagRc.Height) )
			{
				i51WidgetScrollDrawOneItemBaseLapRect(lv,pCurNode,&tagRc);
			}
		}
		else if (limit == SCROLL_REDRAW_LIMIT_BOTTOM)
		{
			if(i51FrameGetOverlapArea(&tagRc,&pCurNode->rc,&tempRc))
			{
				i51WidgetScrollDrawOneItemBaseLapRect(lv,pCurNode,&tagRc);
			}
		}

		if(pCurNode->pNext)
		{
			pCurNode = pCurNode->pNext;
		}
		else
		{
			break;
		}
	}
	i51WidgetScrollDrawBar(lv);

}

void i51WidgetMoveScrollRender(ListView *lv,SCROLL_DIRT dir,iS16 offSetTotal)//屏幕坐标不变（参考系），变化的是list的坐标
{

	iS16 offSetCur = 0;
	iRECT tempRcMove,tempRcRedraw;
	iS16 tempYRel = 0;
	if (offSetTotal == 0 || !lv)
	{
		return ;
	}
	if (lv->rcCtr.Height > lv->hAbs)
	{
		return;
	}
	tempYRel = lv->yRel;
	if (dir == SCROLL_DIRT_VERTICAL_DOWN)
	{ 
		offSetCur = offSetTotal>tempYRel?tempYRel:offSetTotal;
		if (offSetCur == 0)
		{
			return;
		}

		tempYRel -= offSetCur;
		memcpy(&tempRcMove,&lv->rcCtr,sizeof(iRECT));
		memcpy(&tempRcRedraw,&lv->rcCtr,sizeof(iRECT));
        
		tempRcMove.Height -=offSetCur;
		tempRcRedraw.Y = lv->rcCtr.Y-offSetCur;
		tempRcRedraw.Height = offSetCur;
		i51FrameMovePanel(&tempRcMove,lv->rcCtr.X,lv->rcCtr.Y+offSetCur);
		lv->yRel = tempYRel;
		i51WidgetScrollDraw(lv,0,0,&tempRcRedraw,SCROLL_REDRAW_LIMIT_BOTTOM);
		i51AdeMmiUpdateScreen();
	}
	else if (dir == SCROLL_DIRT_VERTICAL_UP)
	{
		//offSetCur = offSetTotal>(lv->rcCtr.Y+lv->hAbs -lv->rcCtr.Height- tempYRel)?(lv->rcCtr.Y+lv->hAbs -lv->rcCtr.Height- tempYRel):offSetTotal;
		offSetCur = offSetTotal>(lv->hAbs -lv->rcCtr.Height- tempYRel)?(lv->hAbs -lv->rcCtr.Height- tempYRel):offSetTotal;
		if (offSetCur == 0)
		{
			return;
		}

		tempYRel += offSetCur;
		memcpy(&tempRcMove,&lv->rcCtr,sizeof(iRECT));
		memcpy(&tempRcRedraw,&lv->rcCtr,sizeof(iRECT));
		tempRcRedraw.Y = lv->rcCtr.Height;
		tempRcRedraw.Height = offSetCur;
		tempRcMove.Y = lv->rcCtr.Y+offSetCur;
		tempRcMove.Width = lv->rcCtr.Width - lv->bar.rc.Width;
		i51FrameMovePanel(&tempRcMove,lv->rcCtr.X,lv->rcCtr.Y);

		lv->yRel = tempYRel;
		i51WidgetScrollDraw(lv,0,lv->rcCtr.Height-offSetCur,&tempRcRedraw,SCROLL_REDRAW_LIMIT_NONE);
		i51AdeMmiUpdateScreen();
	}

}

iBOOL i51WidgetCreateImageButton(ImageButton *pBtn,BTN_ID id,iWCHAR *pwszCaption,iU32 imgAD,iRECT *pimageRc,iRECT *ptouchRc,iCOLOR bgIcidle,iCOLOR bgIcDown)
{
	iBOOL ret = iFALSE;
	if (!pBtn || !pimageRc)
	{
		return ret;
	}
	memcpy(&pBtn->imgRc,pimageRc,sizeof(iRECT));
	if (ptouchRc)
	{
		memcpy(&pBtn->touchRc,ptouchRc,sizeof(iRECT));
	}
	else
	{
		memcpy(&pBtn->touchRc,pimageRc,sizeof(iRECT));
	}
    if (bgIcidle)
    {
		pBtn->bgIcidle = bgIcidle;
    }
    else
	{
		pBtn->bgIcidle = COLOR_BLACK;
	}

	if (bgIcDown)
	{
		pBtn->bgIcDown = bgIcDown;
	}
	else
	{
		pBtn->bgIcDown = COLOR_WHITE;
	}
    
	pBtn->imgAD = imgAD;
	pBtn->id = id;
    i51AdeStdWStrcpy(pBtn->wszCaption,pwszCaption);
	ret = iTRUE;
	return ret;

}

