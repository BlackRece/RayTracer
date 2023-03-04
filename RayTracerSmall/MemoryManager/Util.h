#pragma once
#include <iostream>
#include "Structures.h"

class Util
{
public:
	static char*	allocFrameBytes(size_t nSize);
	
	static Frame*	getParentFrame(void* pPtr) { return (Frame*)((char*)pPtr - sizeof(Frame) - sizeof(Header)); }
		
	static Str		getValidTag(Str* sTag);
};

