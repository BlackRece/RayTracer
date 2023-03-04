#include "Heap.h"

Str* Heap::init(Str* sHeapTag)
{
	m_nByteCount = 0;
	m_nBytesAllocated = 0;
	m_nBytesDeallocated = 0;

	m_pNext = nullptr;
	m_pPrev = nullptr;

	m_pRootPool = nullptr;
	m_pRootFrame = nullptr;

	if (sHeapTag != nullptr)
		m_sTag.set(sHeapTag->str);


	std::cout << std::endl << m_sTag.str << " heap initialised.\n";

	return &m_sTag;
}

void Heap::addBytes(size_t byteCount) 
{
	size_t nByteAmount = (size_t)std::abs((int)byteCount);
	m_nBytesAllocated += nByteAmount;
	m_nByteCount += nByteAmount;
}

void Heap::subBytes(size_t byteCount) 
{
	size_t nByteAmount = (size_t)std::abs((int)byteCount);
	m_nBytesDeallocated += nByteAmount;
	m_nByteCount -= nByteAmount;
}

size_t Heap::getBytes()
{
	// TODO: resulting tally is incorrect. bytes removed instead of added or removed without setting bytes added
	size_t nTally = m_nBytesAllocated - m_nBytesDeallocated;
	if (nTally != m_nByteCount)
	{
		std::cout
			<< std::dec
			<< "\nByte Tally;"
			<< "\n-------------"
			<< "\nexpected =\t" << m_nByteCount
			<< "\ntallied =\t" << nTally
			<< std::endl;

		throw "Error: allocated bytes have not been accounted for!";
	}

	return nTally;
}

void Heap::walk()
{
	std::cout
		<< "\nWalking the heap..."
		<< "\nHeap tag: " << m_sTag.str << std::endl;

	int nCount = 0;
	int nBytes = 0;
	int nPoolBytes = 0;

	if (m_pRootPool != nullptr)
		nPoolBytes = walkPools();

	if (m_pRootFrame == nullptr)
		return;

	std::cout
		<< "\nHeap Items..."
		<< "\n\n address\t\t| bytes"
		<< "\n--------\t\t|------\n";

	Frame* pFrame = m_pRootFrame;
	while (pFrame != nullptr)
	{
		pFrame->m_pHeader->validate();
		pFrame->m_pFooter->validate();
		
		nCount++;
		nBytes += (int)pFrame->fullSize();

		std::cout
			<< std::hex << pFrame << "\t| "
			<< std::dec << pFrame->fullSize()
			<< std::endl;

		pFrame = pFrame->m_pNext;
	}

	std::cout
		<< "\nByteCount = "
		<< std::dec << nBytes
		<< "\nvariable count = "
		<< std::dec << nCount
		<< std::endl;

	std::cout
		<< "\n-------------"
		<< "\nHeap Info:"
		<< "\nBytes Allocated:\t" << m_nBytesAllocated
		<< "\nBytes Deallocated:\t" << m_nBytesDeallocated
		<< "\n-------------"
		<< "\nByte Tally:\t\t" << getBytes()
		<< std::endl;
}

int Heap::walkPools()
{
	std::cout
		<< "\nWalking all pools of "
		<< m_sTag.str << " heap.\n\n";

	Pool* pPool = m_pRootPool;
	int nPoolCount = 1;
	int nPoolBytes = 0;
	while (pPool != nullptr)
	{
		std::cout
			<< "\nWalking Pool #" << nPoolCount;

		nPoolBytes += pPool->walk();
		pPool = pPool->getNext();

		nPoolCount++;
	}

	return nPoolBytes;
}

void Heap::clear()
{
	std::cout
		<< "\Clearing heap: " << m_sTag.str << std::endl;

	if (m_pRootPool != nullptr)
	{
		m_pRootPool->clear();
		freeFrame(Util::getParentFrame(m_pRootPool));
		m_pRootPool = nullptr;
	}

	if (m_pRootFrame == nullptr)
	{
		std::cout << "Heap is empty" << std::endl;
		return;
	}

	std::cout
		<< "\Clearing heap: " << m_sTag.str
		<< "\n\n address\t\t| bytes"
		<< "\n--------\t\t|------\n";

	Frame* pFrame = m_pRootFrame;
	while (pFrame != nullptr)
	{
		pFrame->m_pHeader->validate();
		pFrame->m_pFooter->validate();
		
		std::cout
			<< std::hex << pFrame << "\t| "
			<< std::dec << pFrame->fullSize()
			<< std::endl;

		Frame* pNext = pFrame->m_pNext;
		
		freeFrame(pFrame);
		
		pFrame = pNext;
	}

	m_pRootFrame = nullptr;
}

void Heap::freeFrame(Frame* pFrame)
{
	Frame* pPrev = pFrame->m_pPrev;
	Frame* pNext = pFrame->m_pNext;
	
	if (pPrev != nullptr)
		pPrev->m_pNext = pNext;
	else
		m_pRootFrame = pNext;
	
	if (pNext != nullptr)
		pNext->m_pPrev = pPrev;
	
	subBytes(pFrame->fullSize());
	free(pFrame);
	pFrame = nullptr;
}

void Heap::freePool(Pool* pPool)
{
	Pool* pPrev = pPool->getPrev();
	Pool* pNext = pPool->getNext();

	if (pPrev != nullptr)
		pPrev->setNext(pNext);
	else
		m_pRootPool = pNext;

	if (pNext != nullptr)
		pNext->setPrev(pPrev);

	Frame* pFrame = Util::getParentFrame((void*)pPool);
	subBytes(pFrame->fullSize());
	free(pFrame);
}

void Heap::addHeap(Heap* pHeap)
{
	setNextHeap(pHeap);
	pHeap->setPrevHeap(this);
}

Heap* Heap::findHeap(Str sTag)
{
	if (hasTag(sTag))
		return this;
	
	if (m_pNext != nullptr)
		return m_pNext->findHeap(sTag);
	
	return nullptr;
}

Pool* Heap::getPoolWithBytesFree(size_t nSize)
{
	Pool* pPool = m_pRootPool;

	while (pPool != nullptr)
	{
		if (pPool->hasBytesFree(nSize))
			return pPool;

		pPool = pPool->getNext();
	}

	return pPool;
}

void Heap::addPool(Pool* pPool)
{
	addBytes(Util::getParentFrame((void*)pPool)->fullSize());
	
	if (m_pRootPool == nullptr)
	{
		m_pRootPool = pPool;
		return;
	}

	Pool* pLast = m_pRootPool->getLast();

	pLast->setNext(pPool);
	pPool->setPrev(pLast);
}

void Heap::addFrame(Frame* pFrame)
{
	pFrame->m_pHeap = this;
	addBytes(pFrame->fullSize());
	
	if (m_pRootFrame == nullptr)
	{
		m_pRootFrame = pFrame;
		return;
	}
	
	Frame* pLast = m_pRootFrame->getLast();
	
	pLast->m_pNext = pFrame;
	pFrame->m_pPrev = pLast;
}