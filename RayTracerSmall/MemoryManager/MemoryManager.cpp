#include "MemoryManager.h"

Heap* MemoryManager::s_pRootHeap = nullptr;
bool MemoryManager::m_bDebugMode = true;
bool MemoryManager::m_bPoolingMode = true;

void* operator new(size_t nSize)
{
	if (!MemoryManager::isDebugEnabled())
		return malloc(nSize);
	
	std::cout
		<< "global operator new called ... \n";
	
	void* pStartMemBlock = MemoryManager::isPoolingEnable()
		? MemoryManager::allocToPool(nSize, nullptr)
		: MemoryManager::allocToHeap(nSize, nullptr);

	return pStartMemBlock;
}

void* operator new (size_t nSize, char pHeapTag[])
{
	if (!MemoryManager::isDebugEnabled())
		return malloc(nSize);

	Str sTag; sTag.set(pHeapTag);

	std::cout
		<< "global operator new called ..."
		<< "\nwith size: " << nSize
		<< "\nfor heap: " << sTag.str
		<< std::endl;

	void* pStartMemBlock = MemoryManager::isPoolingEnable()
		? MemoryManager::allocToPool(nSize, &sTag)
		: MemoryManager::allocToHeap(nSize, &sTag);

	return pStartMemBlock;
}

void operator delete(void* pMem)
{
	if (!MemoryManager::isDebugEnabled())
	{
		free(pMem);
		return;
	}

	std::cout 
		<< "global operator delete called \n"
		<< "for address : " << std::hex << pMem
		<< std::endl;
		
	Frame* pFrame = Util::getParentFrame(pMem);
	size_t nBytes = pFrame->fullSize();

	pFrame->m_pHeader->validate();
	pFrame->m_pFooter->validate();

	if (pFrame->m_pPool != nullptr)
		pFrame->m_pPool->delFrame(pFrame);
	else
		pFrame->m_pHeap->freeFrame(pFrame);

	std::cout
		<< "\nBytes freed: " << std::dec << nBytes << std::endl;
}

void MemoryManager::init(bool bDebugMobe)
{
	MemoryManager::m_bDebugMode = bDebugMobe;

	if (!isDebugEnabled())
	{
		std::cout
			<< "Memory Manager not available in release mode.\n";

		return;
	}

	if (s_pRootHeap != nullptr)
		return;
	
	s_pRootHeap = newHeap(nullptr);
}

Heap* MemoryManager::addHeap(char sHeapTag[])
{
	Str sTag; sTag.set(sHeapTag);

	Heap* pTargetHeap = findHeap(&sTag);
	if (pTargetHeap != nullptr)
		return pTargetHeap;
	
	pTargetHeap = newHeap(&sTag);

	Heap* pLastHeap = s_pRootHeap->getLast();
		
	pTargetHeap->setPrevHeap(pLastHeap);
	pLastHeap->setNextHeap(pTargetHeap);
	
	return pTargetHeap;
}

Heap* MemoryManager::newHeap(Str* sTag)
{
	size_t nHeapSize = sizeof(Heap);
	
	char* pMem = Util::allocFrameBytes(nHeapSize);

	Frame* pFrame = (Frame*)pMem;
	pFrame->init(nHeapSize);
	
	Str sValidTag = Util::getValidTag(sTag);

	Heap* pHeap = (Heap*)pFrame->m_pData;
	Str* sHeapTag = pHeap->init(&sValidTag);

	std::cout
		<< std::endl << sHeapTag->str
		<< " heap created" << std::hex
		<< "\naddress : " << pHeap
		<< "\nheap header : " << pFrame->m_pHeader
		<< "\nheap footer : " << pFrame->m_pFooter
		<< std::endl;

	return pHeap;
}

Heap* MemoryManager::findHeap(Str* sHeapTag)
{
	std::cout
		<< "MemoryManager - finding \"" << sHeapTag->str
		<< "\" Heap from heap list...\n";

	if(s_pRootHeap != nullptr)
		return s_pRootHeap->findHeap(*sHeapTag);
	
	return nullptr;
}

Heap* MemoryManager::getHeap(Str* sHeapTag)
{
	Heap* pHeap = findHeap(sHeapTag);
	
	if (pHeap == nullptr)
		pHeap = s_pRootHeap;
	
	return pHeap;
}

void MemoryManager::freeHeap(Str* heapTag)
{
	Heap* pHeap = findHeap(heapTag);
	if (pHeap == nullptr)
		return;
	
	pHeap->clear();

	Heap* pPrev = pHeap->getPrev();
	Heap* pNext = pHeap->getNext();
	if (pPrev != nullptr)
		pPrev->setNextHeap(pNext);
	else
		s_pRootHeap = pNext;
	
	if (pNext != nullptr)
		pNext->setPrevHeap(pPrev);
		
	Frame* pFrame = Util::getParentFrame((void*)pHeap);
	free(pFrame);
}

void MemoryManager::checkHeaps()
{
	Heap* pHeap = s_pRootHeap;

	while (pHeap != nullptr)
	{
		pHeap->walk();
		pHeap = pHeap->getNext();
	}
}

void MemoryManager::clearHeaps()
{
	Heap* pHeap = (Heap*)s_pRootHeap;

	while (pHeap != nullptr)
	{
		pHeap->clear();
		pHeap = pHeap->getNext();
	}
}

void MemoryManager::clearHeap(Str* heapTag)
{
	Heap* pHeap = findHeap(heapTag);
	
	if(pHeap != nullptr)
		pHeap->clear();
}

void* MemoryManager::allocFrame(size_t nSize, Str* sHeapTag)
{
	char* pAddr = Util::allocFrameBytes(nSize);

	Str sTag = Util::getValidTag(sHeapTag);
	Heap* pHeap = getHeap(&sTag);
	
	Frame* pFrame = (Frame*)pAddr;
	pFrame->init(nSize);
	pHeap->addFrame(pFrame);
	
	std::cout
		<< "\non selected heap: " << sTag.str
		<< "\naddress selected: "
		<< std::hex << (void*)pAddr
		<< "\nheader address: "
		<< std::hex << pFrame->m_pHeader
		<< "\n  data address: "
		<< std::hex << pFrame->m_pData
		<< "\nfooter address: "
		<< std::hex << pFrame->m_pFooter
		<< std::endl;

	return pFrame->m_pData;
}

void* MemoryManager::allocToHeap(size_t nSize, Str* sHeapTag)
{
	return MemoryManager::allocFrame(nSize, sHeapTag);
}

void* MemoryManager::allocToPool(size_t nSize, Str* sHeapTag)
{
	Str sTag = Util::getValidTag(sHeapTag);

	Heap* pHeap = getHeap(&sTag);

	std::cout
		<< "MemoryManager - allocating "
		<< std::dec << nSize
		<< " bytes to pool on heap "
		<< pHeap->getTag()->str << std::endl;
	
	Pool* pPool = pHeap->getPoolWithBytesFree(nSize);
	if (pPool == nullptr)
		pPool = newPool(nSize, pHeap);
	
	Frame* pFrame = pPool->addFrame(nSize);
	pFrame->m_pHeap = pHeap;
			
	return pFrame->m_pData;
}

Pool* MemoryManager::newPool(size_t nSize, Heap* pHeap)
{
	size_t nPoolSize = Pool::calcPoolSize(nSize);

	Pool* pPool = (Pool*)allocFrame(nPoolSize, pHeap->getTag());
	pPool->init(nPoolSize);

	pHeap->addPool(pPool);

	return pPool;
}