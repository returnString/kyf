#pragma once

#include "LogFwd.h"

class LogCursor
{
public:
	LogCursor(const Log& log, offset_t start);
	bool Next(uint8_t*& key, uint32_t& keyLen, uint8_t*& value, uint32_t& valueLen);
	offset_t GetOffset() { return m_currentOffset; }

private:
	const Log& m_log;
	ReadOnlyLogSegment m_currentSegment;
	offset_t m_currentOffset;
	size_t m_currentSegmentPos;
};