#pragma once

#include "PlatformHelpers.h"

struct MemoryMappedFile
{
	MemoryMappedFile(std::string filename, size_t size);
	~MemoryMappedFile();

	uint8_t* buffer;
	size_t size;

private:
#if WINDOWS
	HANDLE m_fileHandle;
	HANDLE m_mapHandle;
#elif LINUX
	int m_fileHandle;
#endif
};