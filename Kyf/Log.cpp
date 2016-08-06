#include "stdafx.h"
#include "Log.h"
#include "LogSegment.h"

Log::Log(std::string name, size_t segmentSize)
	: m_name(name)
	, m_segmentSize(segmentSize)
	, m_head(0)
{
	CreateNextSegment();
}

const ReadOnlyLogSegment Log::FindSegment(offset_t offset) const
{
	auto& it = m_segments.lower_bound(offset);
	if (it == m_segments.end())
	{
		return nullptr;
	}

	return it->second;
}

const ReadOnlyLogSegment Log::GetSegmentAfter(const LogSegment& segment) const
{
	auto& it = m_segments.find(segment.GetStartOffset());
	assert(it != m_segments.end());

	auto& nextIt = std::next(it);
	if (nextIt == m_segments.end())
	{
		return nullptr;
	}

	return nextIt->second;
}

void Log::Commit(uint8_t* key, uint32_t keyLen, uint8_t* value, uint32_t valueLen)
{
	auto& it = m_segments.rbegin();
	m_head++;

	auto segment = it->second;
	if (!segment->WriteMessage(m_head, key, keyLen, value, valueLen))
	{
		segment = CreateNextSegment();
		segment->WriteMessage(m_head, key, keyLen, value, valueLen);
	}
}

WritableLogSegment Log::CreateNextSegment()
{
	auto segment = std::make_shared<LogSegment>(m_head, m_segmentSize);
	auto result = m_segments.insert(std::make_pair(m_head, segment));
	assert(result.second);
	return segment;
}