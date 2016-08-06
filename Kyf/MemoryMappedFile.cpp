#include "stdafx.h"
#include "MemoryMappedFile.h"

MemoryMappedFile::MemoryMappedFile(std::string filename, size_t size)
	: size(size)
#ifdef WIN32
	, m_fileHandle(INVALID_HANDLE_VALUE)
	, m_mapHandle(INVALID_HANDLE_VALUE)
#endif
{
#ifdef WIN32
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	std::wostringstream nameStream;
	nameStream << converter.from_bytes(filename);
	auto name = nameStream.str();
	auto namePtr = name.c_str();

	auto access = GENERIC_READ | GENERIC_WRITE;
	auto sharing = FILE_SHARE_READ | FILE_SHARE_WRITE;
	auto attributes = FILE_ATTRIBUTE_NORMAL;

	DWORD fileAttrs = GetFileAttributes(namePtr);
	auto exists = (fileAttrs != INVALID_FILE_ATTRIBUTES && !(fileAttrs & FILE_ATTRIBUTE_DIRECTORY));

	if (exists)
	{
		WINCALL(m_fileHandle = CreateFile(namePtr, access, sharing, 0, OPEN_EXISTING, attributes, 0));
		LARGE_INTEGER fileSize;
		WINCALL(GetFileSizeEx(m_fileHandle, &fileSize));
		assert(fileSize.QuadPart == size);
	}
	else
	{
		WINCALL(m_fileHandle = CreateFile(namePtr, access, sharing, 0, CREATE_NEW, attributes, 0));
		LARGE_INTEGER newSize;
		newSize.QuadPart = size;
		WINCALL(SetFilePointerEx(m_fileHandle, newSize, nullptr, FILE_BEGIN));
		WINCALL(SetEndOfFile(m_fileHandle));
	}

	WINCALL(m_mapHandle = CreateFileMapping(m_fileHandle, nullptr, PAGE_READWRITE, 0, 0, nullptr));
	WINCALL(buffer = (uint8_t*)MapViewOfFile(m_mapHandle, FILE_MAP_ALL_ACCESS, 0, 0, 0));
#else
	UMIMPLEMENTED("Memory mapped files");
#endif
}

MemoryMappedFile::~MemoryMappedFile()
{
#ifdef WIN32
	WINCALL(UnmapViewOfFile(buffer));
	WINCALL(CloseHandle(m_mapHandle));
	WINCALL(CloseHandle(m_fileHandle));
#else
	UMIMPLEMENTED("Memory mapped files");
#endif
}