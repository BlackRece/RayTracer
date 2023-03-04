#pragma once
#include <exception>
#include <cstring>

#define HEAD_VALUE 0xDEADC0DE
#define FOOT_VALUE 0xDEADBEEF

#define TAG_LENGTH 30
#define DEFAULT_TAG "Default"

class Heap;
class Pool;

struct Str
{
	char str[TAG_LENGTH];
	int len;

	bool isEmpty() { return len == 0; }
	bool cmp(Str src) { return len == src.len && strcmp(str, src.str) == 0; }
	void set(char string[])
	{
		if (string == nullptr)
			return;

		strcpy_s(str, string);
		len = (int)strlen(string);
	}
};

struct Footer
{
	int m_iCheckValue;

	void init() { m_iCheckValue = FOOT_VALUE; }
	bool validate()
	{
		//FOOT_VALUE = -559038737
		if (m_iCheckValue != FOOT_VALUE)
			throw "invalid footer";

		return true;
	}
};

struct Header
{
	int		m_iCheckValue;
	
	void init()	{ m_iCheckValue = HEAD_VALUE; }
	bool validate() 
	{
		//HEAD_VALUE = -559038242
		if (m_iCheckValue != HEAD_VALUE)
		{
			throw "invalid header";
			return false;
		}
		
		return true;
	}
};

struct Frame
{
	size_t		m_nSize;

	Heap*		m_pHeap;
	Pool*		m_pPool;
	
	Header*		m_pHeader;
	Footer*		m_pFooter;
	void*		m_pData;
	
	Frame*		m_pNext;
	Frame*		m_pPrev;

	Frame*		getLast() { return m_pNext == nullptr ? this : m_pNext->getLast(); }
	size_t		fullSize() { return sizeof(Frame) + sizeof(Header) + m_nSize + sizeof(Footer); }

	void init(size_t nSize)
	{
		m_nSize = nSize;
		
		m_pHeader = (Header*)((char*)this + sizeof(Frame));
		m_pHeader->init();

		m_pData = (void*)((char*)m_pHeader + sizeof(Header));
		
		m_pFooter = (Footer*)((char*)m_pData + nSize);
		m_pFooter->init();
		
		m_pNext = nullptr;
		m_pPrev = nullptr;

		m_pHeap = nullptr;
		m_pPool = nullptr;
	}
};