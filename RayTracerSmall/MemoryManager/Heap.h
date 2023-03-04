#pragma once
#include <cmath>

#include "Util.h"
#include "Pool.h"

class Heap
{
public:
	Str*				init(Str* sHeapTag);
	void				walk();
	int					walkPools();
	void				clear();
	
	void				addBytes(size_t byteCount);
	void				subBytes(size_t byteCount);
	size_t				getBytes();

	const bool			hasTag(Str sTag) { return m_sTag.cmp(sTag); }
	Str*				getTag() { return &m_sTag; }

	Pool*				getPoolWithBytesFree(size_t nSize);
	Pool*				getPoolWithSpace(size_t nSize);
	void				addPool(Pool* pPool);
	void				freePool(Pool* pPool);
	
	// linked list of heaps
	void				addHeap(Heap* pHeap);
	Heap*				findHeap(Str sTag);
	Heap*				getLast() { return m_pNext == nullptr ? this : m_pNext->getLast(); }
	Heap*				getNext() { return m_pNext; }
	void				setNextHeap(Heap* pNext) { m_pNext = pNext; }
	
	Heap*				getPrev() { return m_pPrev; }
	void				setPrevHeap(Heap* pPrev) { m_pPrev = pPrev; }
	
	void				addFrame(Frame* pFrame);
	void				freeFrame(Frame* pFrame);
		
private:	
						Heap() { ; }
						~Heap()	{ clear(); }
						
	Pool*				m_pRootPool;

	// heap stats
	Str					m_sTag;
	size_t				m_nByteCount;
	size_t				m_nBytesAllocated;
	size_t				m_nBytesDeallocated;
	
	// heap linked list
	Heap*				m_pNext;
	Heap*				m_pPrev;
	
	// linked list of frames
	Frame*				getLastFrame() { return m_pRootFrame != nullptr ? m_pRootFrame->getLast() : nullptr; }
	Frame*				m_pRootFrame;
};