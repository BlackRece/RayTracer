#pragma once

#include "Util.h"

class Pool
{
public:
	static void				setDefaultObjectCount(int nCount) { s_nDefaultItemCount = nCount; }
	static size_t			calcPoolSize(size_t nSize) { return s_nDefaultItemCount * (nSize + sizeof(Frame)); }
	
	void					init(size_t nSize);
	bool					hasBytesFree(size_t nSize) { return m_nFree >= nSize + sizeof(Frame); }
	int						walk();
	void					clear();

	Pool*					getLast() { return m_pNext == nullptr ? this : m_pNext->getLast(); }
	Pool*					getNext() { return m_pNext; }
	Pool*					getPrev() { return m_pPrev; }
		
	Frame*					addFrame(size_t nSize);
	void					delFrame(Frame* pFrame);
	
	void					setNext(Pool* pPool) { m_pNext = pPool; }
	void					setPrev(Pool* pPool) { m_pPrev = pPool; }
	
private:
							Pool() { ; }
							~Pool() { ; }
													
	void					addBytes(size_t nSize) { m_nFree += nSize; m_nUsed -= nSize; }
	void					subBytes(size_t nSize) { m_nFree -= nSize; m_nUsed += nSize; }

	Frame*					getLastFrame() { return m_pFrame != nullptr ? m_pFrame->getLast() : nullptr; }
	Frame*					newFrame(size_t nSize);

	static int				s_nDefaultItemCount;
	
	Frame*					m_pFrame;

	size_t					m_nSize;
	size_t					m_nFree;
	size_t					m_nUsed;

	Pool*					m_pNext;
	Pool*					m_pPrev;
};