#include "Util.h"

char* Util::allocFrameBytes(size_t nSize)
{
	size_t nFrame = sizeof(Frame);
	size_t nHead = sizeof(Header);
	size_t nFoot = sizeof(Footer);
	size_t nTotalSize = nSize + nHead + nFoot + nFrame;
	std::cout
		<< std::dec
		<< "frame size:\t" << nFrame << std::endl
		<< "header size:\t" << nHead << std::endl
		<< "requested size:\t" << nSize << std::endl
		<< "footer size:\t" << nFoot << std::endl
		<< "===========" << std::endl
		<< "total size:\t" << nTotalSize << std::endl;

	char* pResult = (char*)malloc(nTotalSize);
	std::cout
		<< "\nallocated at address: "
		<< std::hex << (void*)pResult
		<< std::endl << std::endl;

	return pResult;
}

Str Util::getValidTag(Str* tag)
{
	char sDefaultTag[] = DEFAULT_TAG;
	Str sTag; sTag.set(sDefaultTag);

	if (tag == nullptr)
		return sTag;

	if (tag->isEmpty())
		tag->set(sDefaultTag);

	return *tag;
}
