#pragma once

#include <iostream>

#include "Util.h"
#include "Heap.h"

void* operator new(size_t nSize);
void* operator new(size_t size, char pHeapTag[]);

void operator delete(void* ptr);

class MemoryManager
{
public:
	static bool			isDebugEnabled() { return m_bDebugMode; };
	static bool			isPoolingEnable() { return m_bPoolingMode; };

	static void			init(bool bDebugMobe = true);
	static void			enablePooling(bool bPoolingMode) { m_bPoolingMode = bPoolingMode; }

	static void*		allocFrame(size_t nSize, Str* sHeapTag);
	static void*		allocToHeap(size_t nSize, Str* sHeapTag);
	static void*		allocToPool(size_t nSize, Str* sHeapTag);

	static Heap*		addHeap(char heapTag[] = nullptr);
	static void			freeHeap(Str* heapTag);
	static void			checkHeaps();
	static void			clearHeaps();
	static void			clearHeap(Str* heapTag);

	static void			addHeader(Header* pNewHeader, Str* sHeapTag);
	static void			delItem(Header* pHeader);

private:
						MemoryManager() {}
						~MemoryManager() {}

	static Heap*		newHeap(Str* sHeapTag);
	static Heap*		findHeap(Str* sHeapTag);
	static Heap*		getHeap(Str* sHeapTag);
	
	static Pool*		newPool(size_t nSize, Heap* pHeap);

	static bool			m_bDebugMode;
	static bool			m_bPoolingMode;
	static Heap*		s_pRootHeap;
};
