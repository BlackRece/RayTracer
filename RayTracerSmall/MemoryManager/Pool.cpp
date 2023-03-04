#include "Pool.h"

int Pool::s_nDefaultItemCount = 10;

void Pool::init(size_t nSize)
{
	m_nSize = nSize;
	m_nFree = m_nSize - sizeof(Pool);
	m_nUsed = m_nSize - m_nFree;
	
	m_pNext = nullptr;
	m_pPrev = nullptr;
	
	m_pFrame = nullptr;
		
	std::cout
		<< "Pool created size: "
		<< std::dec << m_nSize << std::endl;
}

int Pool::walk()
{
	int nCount = 0;
	int nBytes = 0;

	std::cout
		<< "\n\n address\t\t| bytes"
		<< "\n--------\t\t|------\n";

	Frame* pFrame = m_pFrame;
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
		<< "\nFramed item count = "
		<< std::dec << nCount
		<< std::endl;

	std::cout
		<< "\n-------------"
		<< "\nPool Info:"
		<< "\nPool Size: " << m_nSize
		<< "\n-------------"
		<< "\nBytes Free: " << m_nFree
		<< "\nBytes Used: " << m_nUsed
		<< "\n-------------"
		<< std::endl;

	return nBytes + m_nSize;
}

Frame* Pool::addFrame(size_t nSize)
{
	Frame* pLastFrame = getLastFrame();
	Frame* pFrame;

	if (pLastFrame != nullptr)
	{
		pFrame = (Frame*)((char*)pLastFrame + pLastFrame->fullSize());
		pFrame->init(nSize);

		pLastFrame->m_pNext = pFrame;
		pFrame->m_pPrev = pLastFrame;
	}
	else
	{
		pFrame = (Frame*)((char*)this + sizeof(Pool));
		pFrame->init(nSize);

		m_pFrame = pFrame;
	}

	pFrame->m_pPool = this;

	subBytes(pFrame->fullSize());
	
	return pFrame;
}

void Pool::clear()
{
	if (m_pFrame == nullptr)
	{
		std::cout << "Pool is empty" << std::endl;
		return;
	}

	std::cout
		<< "\Clearing pool: "
		<< "\n\n address\t\t| bytes"
		<< "\n--------\t\t|------\n";

	Frame* pFrame = m_pFrame;
	while (pFrame != nullptr)
	{
		pFrame->m_pHeader->validate();
		pFrame->m_pFooter->validate();

		std::cout
			<< std::hex << pFrame << "\t| "
			<< std::dec << pFrame->fullSize()
			<< std::endl;

		Frame* pNext = pFrame->m_pNext;

		delFrame(pFrame);

		pFrame = pNext;
	}
}

void Pool::delFrame(Frame* pFrame)
{
	if (pFrame == nullptr)
		return;

	if (pFrame->m_pPrev != nullptr)
		pFrame->m_pPrev->m_pNext = pFrame->m_pNext;
	else
		m_pFrame = pFrame->m_pNext;

	if (pFrame->m_pNext != nullptr)
		pFrame->m_pNext->m_pPrev = pFrame->m_pPrev;

	addBytes(pFrame->fullSize());
	pFrame = nullptr;
}