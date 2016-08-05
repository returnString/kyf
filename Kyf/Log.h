#pragma once

#include "LogFwd.h"

class Log
{
public:
	Log(std::string name, size_t segmentSize);
	const ReadOnlyLogSegment Log::FindSegment(offset_t offset) const;
	const ReadOnlyLogSegment Log::GetSegmentAfter(const LogSegment& segment) const;
	void Commit(uint8_t* key, uint32_t keyLen, uint8_t* value, uint32_t valueLen);

private:
	WritableLogSegment CreateNextSegment();

	std::string m_name;
	std::map<offset_t, WritableLogSegment> m_segments;
	offset_t m_head;
	size_t m_segmentSize;
};