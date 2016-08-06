#pragma once

#include "PlatformHelpers.h"

struct MemoryMappedFile
{
	MemoryMappedFile(std::string filename, size_t size);
	uint8_t* buffer;
	size_t size;

private:
#ifdef WIN32
	HANDLE m_fileHandle;
	HANDLE m_mapHandle;
#else
	UMIMPLEMENTED("Memory mapped files");
#endif
};