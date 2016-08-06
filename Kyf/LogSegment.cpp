#include "stdafx.h"
#include "LogSegment.h"

LogSegment::LogSegment(offset_t start, size_t bufferSize)
	: m_startOffset(start)
	, m_writePos(0)
	, m_file(std::to_string(start), bufferSize)
{
}

bool LogSegment::WriteMessage(offset_t offset, uint8_t* key, uint32_t keyLen, uint8_t* value, uint32_t valueLen)
{
	size_t messageLen = sizeof(offset_t)
		+ (sizeof(uint32_t) * 3) // message length, key length, value length
		+ keyLen
		+ valueLen;

	assert(messageLen < UINT32_MAX);

	if ((m_writePos + messageLen) >= m_file.size)
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
	memcpy(m_file.buffer + m_writePos, data, len);
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
	data = m_file.buffer + pos;
	pos += len;
}