



#ifndef __MY_MAP_H__
#define __MY_MAP_H__
#include "i51KitAde.h"
#include "i51KitStd.h"
#include "i51KitG2.h"

template<class _Ty>
class MyLess
{
public:
    // functor for operator<
    bool operator()(const _Ty& _Left, const _Ty& _Right) const
    {
        return (_Left < _Right);
    }
};

template<class _Ty1, class _Ty2>
class MyPair
{
public:
    typedef _Ty1 first_type;
    typedef _Ty2 second_type;
public:
    MyPair(void)
        : first(_Ty1()), second(_Ty2())
    {	// construct from defaults
    }

    MyPair(const _Ty1& _Val1, const _Ty2& _Val2)
        : first(_Val1), second(_Val2)
    {	// construct from specified values
    }

    template<class _Other1, class _Other2>
        MyPair(const MyPair<_Other1, _Other2>& _Right)
        : first(_Right.first), second(_Right.second)
    {	// construct from compatible pair
    }

public:
    _Ty1 first;     // the first stored value
    _Ty2 second;    // the second stored value
};


template<class _Kty, class _Ty>
class MyMap
{
public:
    typedef typename MyPair<_Kty, _Ty> MyMapPair;

public:
    struct _Node
    {
        _Node(const MyMapPair& Myval, _Node *Prev = NULL, _Node *Next = NULL)
        {
            _Prev = Prev;
            _Next = Next;
            _Myval = Myval;
        }
        _Node *_Prev;
        _Node *_Next;
        MyMapPair _Myval;
    };

    class iterator
    {
    public:
        _Node *_Ptr;

    public:
        iterator(_Node *Ptr = NULL)
            : _Ptr(Ptr)
        {
        }
        iterator(iterator& _Other)
            :_Ptr(_Other._Ptr)
        {
        }
        iterator& operator = (iterator& _Other)
        {
            _Ptr = _Other._Ptr;
            return *this;
        }

        bool operator == (const iterator& _Other) const
        {
            return (_Ptr == _Other._Ptr);
        }
        bool operator != (const iterator& _Other) const
        {
            return (_Ptr != _Other._Ptr);
        }

        iterator& operator ++()
        {
            _Ptr = _Ptr->_Next; 
            return *this; 
        }
        iterator& operator --()
        {
            _Ptr = _Ptr->_Prev;
            return *this;
        }

        MyMapPair& operator *()
        {
            return _Ptr->_Myval;
        }
        MyMapPair* operator ->()
        {
            return &(_Ptr->_Myval);
        }
    };

public:
    MyMap() {}
    ~MyMap()
    {
        clear();
    }

    iterator& begin()
    {
        return head;
    }
    iterator& end()
    {
        return last;
    }

public:
    bool insert(const MyMapPair& Pair);
    bool erase(const _Kty& Key);
    void clear(void);
    iterator& find(const _Kty& Key);

private:
    iterator head, last, temp;
};

template<class _Kty, class _Ty>
bool MyMap<_Kty, _Ty>::insert(const MyMapPair& Pair)
{
//    _Node *pNode = new _Node(Pair);
//i51AdeOsLog(0,"MyMap<_Kty, _Ty>::insert");
	_Node *pNode = (_Node* ) i51AdeOsMalloc (sizeof(_Node),__FILE__,__LINE__) ;
	pNode->_Prev = NULL;
	pNode->_Next = NULL;
	pNode->_Myval = Pair;

    if (pNode == NULL)
        return false;

    if (head == last)
    {
        head._Ptr = pNode;
        return true;
    }

    _Node *pCurr = head._Ptr;
    _Node *pLast = NULL;
  	pNode->_Next=head._Ptr;
  	head._Ptr->_Prev = pNode;
  	head._Ptr = pNode;
  	return true;

    while (pCurr != NULL)
    {
        // Pair.first < pCurr->_Myval.first
       if ( (Pair.first) < (pCurr->_Myval.first) )
//        if (0)
        {
            pNode->_Prev = pCurr->_Prev;
            pNode->_Next = pCurr;
            pCurr->_Prev = pNode;

            if (pNode->_Prev != NULL)
                pNode->_Prev->_Next = pNode;
            else
                head._Ptr = pNode;
            return true;
        }
        // Equal
       else if ( !((pCurr->_Myval.first) < (Pair.first)) )
//        else if ( 0)
        {
delete pNode;
i51AdeOsFree( pNode);
pNode = NULL;
            return false;
        }

        pLast = pCurr;
//pCurr->_Myval.first;
//Pair.first;
	pCurr = pCurr->_Next;
    }

   pNode->_Prev = pLast;
	pNode->_Next =  NULL;
   pLast->_Next = pNode;
// 	pNode->_Next=head._Ptr;
// 	head._Ptr->_Prev = pNode;
// 	head._Ptr = pNode;
    return true;
}

template<class _Kty, class _Ty>
bool MyMap<_Kty, _Ty>::erase(const _Kty& Key)
{
    _Node *pCurr = head._Ptr;
	while (pCurr != NULL)
	{
		// Equal
		if ( !(pCurr->_Myval.first < Key) && !(Key < pCurr->_Myval.first) )
		{
			if (pCurr->_Prev != NULL)
				pCurr->_Prev->_Next = pCurr->_Next;
			else
				head._Ptr = pCurr->_Next;
			if (pCurr->_Next != NULL)
				pCurr->_Next->_Prev = pCurr->_Prev;

//delete pCurr;
i51AdeOsFree( pCurr);
			return true;
		}
		pCurr = pCurr->_Next;
	}

    return false;
}

template<class _Kty, class _Ty>
void MyMap<_Kty, _Ty>::clear(void)
{
    _Node *pCurr = head._Ptr;
    while (pCurr != NULL)
    {
        _Node *pDel = pCurr;
        pCurr = pCurr->_Next;
// delete pDel;
i51AdeOsFree( pDel);
    }
    head._Ptr = NULL;
}

template<class _Kty, class _Ty>
typename MyMap<_Kty, _Ty>::iterator& MyMap<_Kty, _Ty>::find(const _Kty& Key)
{
    _Node *pCurr = head._Ptr;
	while (pCurr != NULL)
	{
		// Equal
		if ( !(pCurr->_Myval.first < Key) && !(Key < pCurr->_Myval.first) )
		{
			temp._Ptr = pCurr;
			return temp;
		}
		pCurr = pCurr->_Next;
	}

    return last;
}

#endif /* __MY_MAP_H__ */


