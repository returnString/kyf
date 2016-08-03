#pragma once

class LogSegment
{
public:
	LogSegment(offset_t start, size_t segmentSize);
	void Initialise();
	bool WriteMessage(offset_t offset, uint8_t* key, uint32_t keyLen, uint8_t* value, uint32_t valueLen);
	bool ReadMessage(offset_t& offset, size_t& pos, uint8_t*& key, uint32_t& keyLe, uint8_t*& value, uint32_t& valueLen) const;
	offset_t GetStartOffset() const { return m_startOffset; }

private:
	void WriteBinary(uint8_t* data, uint32_t len);

	template<class T>
	void WriteData(T data)
	{
		memcpy(m_buffer + m_writePos, &data, sizeof(T));
		m_writePos += sizeof(T);
	}

	void ReadBinary(size_t& pos, uint8_t*& data, uint32_t& len) const;

	template<class T>
	void ReadData(size_t& pos, T& data) const
	{
		memcpy(&data, m_buffer + pos, sizeof(T));
		pos += sizeof(T);
	}

	HANDLE m_fileHandle;
	HANDLE m_mapHandle;
	uint8_t* m_buffer;
	size_t m_bufferSize;
	size_t m_writePos;
	offset_t m_startOffset;
};