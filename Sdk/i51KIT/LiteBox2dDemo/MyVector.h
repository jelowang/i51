



#ifndef __MY_VECTOR_H__
#define __MY_VECTOR_H__

#include "i51KitAde.h"

template<class _Ty>
class MyVector
{
private:
    _Ty *pArr;
    int iSize;

public:
    MyVector(void)
    {
        pArr = NULL;
        iSize = 0;
    }
    ~MyVector(void)
    {
        if (pArr != NULL)
//delete [] pArr;
            i51AdeOsFree( pArr);
//		pArr = iNULL;
    }


    void push_back(const _Ty &_Val);
    void clear(void);
    int size(void);
    _Ty& operator [](int iPos);
};



template<class _Ty>
void MyVector<_Ty>::push_back(const _Ty &_Val)
{
    _Ty *pOrgArr = pArr;
//    pArr = new _Ty[iSize + 1];

//iLog("Start push_back,iSize=%d",iSize);
	pArr = (_Ty* ) i51AdeOsMalloc (sizeof(_Ty)*(iSize + 1) ,__FILE__,__LINE__) ;
//iLog("i51AdeOsMalloc");
    if (pArr != NULL)
    {
        int i;
        for ( i = 0; i < iSize; i++ )
            pArr[i] = pOrgArr[i];
        pArr[iSize] = _Val;
        iSize++;
        if (pOrgArr != NULL)
//delete [] pOrgArr;
i51AdeOsFree ( pOrgArr );
//pOrgArr = iNULL;
    }
    else
    {
        pArr = pOrgArr;
    }
}

template<class _Ty>
void MyVector<_Ty>::clear(void)
{
    if (pArr != NULL)
//delete [] pArr;
i51AdeOsFree(pArr);
    pArr = NULL;
    iSize = 0;
}

template<class _Ty>
int MyVector<_Ty>::size(void)
{
    return iSize;
}

template<class _Ty>
_Ty& MyVector<_Ty>::operator [](int iPos)
{
//    assert(iPos >= 0 && iPos < iSize);
    return pArr[iPos];
}


#endif /* __MY_VECTOR_H__ */


