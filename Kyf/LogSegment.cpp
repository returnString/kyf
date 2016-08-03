#include "stdafx.h"
#include "LogSegment.h"

LogSegment::LogSegment(offset_t start, size_t bufferSize)
	: m_startOffset(start)
	, m_fileHandle(INVALID_HANDLE_VALUE)
	, m_mapHandle(INVALID_HANDLE_VALUE)
	, m_writePos(0)
	, m_buffer(nullptr)
	, m_bufferSize(bufferSize)
{
}

#define WINCALL(...) \
	{ \
		__VA_ARGS__; \
		DWORD lastError = GetLastError(); \
		if (lastError != ERROR_SUCCESS) \
		{ \
			LPTSTR message = nullptr; \
			FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS, \
				nullptr, lastError, LANG_NEUTRAL, (LPTSTR)&message, 0, nullptr); \
			std::wcerr << "Windows error occurred: " << message \
									<< "Executing: " << #__VA_ARGS__ << std::endl \
									<<  __FILE__  << ":" << __LINE__ << std::endl; \
			std::terminate(); \
		} \
	}

void LogSegment::Initialise()
{
	std::wostringstream nameStream;
	nameStream << m_startOffset << SegmentFileExtension;
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
		assert(fileSize.QuadPart == m_bufferSize);
	}
	else
	{
		WINCALL(m_fileHandle = CreateFile(namePtr, access, sharing, 0, CREATE_NEW, attributes, 0));
		LARGE_INTEGER newSize;
		newSize.QuadPart = m_bufferSize;
		WINCALL(SetFilePointerEx(m_fileHandle, newSize, nullptr, FILE_BEGIN));
		WINCALL(SetEndOfFile(m_fileHandle));
	}

	WINCALL(m_mapHandle = CreateFileMapping(m_fileHandle, nullptr, PAGE_READWRITE, 0, 0, nullptr));
	WINCALL(m_buffer = (uint8_t*)MapViewOfFile(m_mapHandle, FILE_MAP_ALL_ACCESS, 0, 0, 0));
}

bool LogSegment::WriteMessage(offset_t offset, uint8_t* key, uint32_t keyLen, uint8_t* value, uint32_t valueLen)
{
	size_t messageLen = sizeof(offset_t)
		+ (sizeof(uint32_t) * 3) // message length, key length, value length
		+ keyLen
		+ valueLen;

	assert(messageLen < UINT32_MAX);

	if ((m_writePos + messageLen) >= m_bufferSize)
	{
		return false;
	}

	WriteData(offset);
	WriteData((uint32_t)messageLen);
	WriteBinary(key, keyLen);
	WriteBinary(value, valueLen);
	return true;
}

void LogSegment::WriteBinary(uint8_t* data, uint32_t len)
{
	WriteData(len);
	memcpy(m_buffer + m_writePos, data, len);
	m_writePos += len;
}

bool LogSegment::ReadMessage(offset_t& offset, size_t& pos, uint8_t *& key, uint32_t & keyLen, uint8_t *& value, uint32_t & valueLen) const
{
	if (pos >= m_writePos)
	{
		return false;
	}

	ReadData(pos, offset);
	uint32_t messageLen;
	ReadData(pos, messageLen);
	ReadBinary(pos, key, keyLen);
	ReadBinary(pos, value, valueLen);
	return true;
}

void LogSegment::ReadBinary(size_t& pos, uint8_t*& data, uint32_t& len) const
{
	ReadData(pos, len);
	data = m_buffer + pos;
	pos += len;
}