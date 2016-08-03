#include "stdafx.h"
#include "Log.h"

Log::Log(std::string name, size_t segmentSize)
	: m_name(name)
	, m_segmentSize(segmentSize)
{
	m_segments.insert(std::make_pair(0, LogSegment(0, m_segmentSize)));
	m_segments.at(0).Initialise();
}

const LogSegment* Log::FindSegment(offset_t offset) const
{
	auto& it = m_segments.lower_bound(offset);
	if (it == m_segments.end())
	{
		return nullptr;
	}

	return &it->second;
}

const LogSegment* Log::GetSegmentAfter(const LogSegment& segment) const
{
	auto& it = m_segments.find(segment.GetStartOffset());
	assert(it != m_segments.end());

	auto& nextIt = std::next(it);
	if (nextIt == m_segments.end())
	{
		return nullptr;
	}

	return &nextIt->second;
}

void Log::Commit(uint8_t* key, uint32_t keyLen, uint8_t* value, uint32_t valueLen)
{
	auto& it = m_segments.rbegin();
	auto& segment = it->second;
	m_head++;

	if (!segment.WriteMessage(m_head, key, keyLen, value, valueLen))
	{
		LogSegment newSegment(m_head, m_segmentSize);
		newSegment.Initialise();
		newSegment.WriteMessage(m_head, key, keyLen, value, valueLen);
		m_segments.insert(std::make_pair(m_head, newSegment));
	}
}