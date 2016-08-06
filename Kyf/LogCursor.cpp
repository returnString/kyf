#include "stdafx.h"
#include "LogCursor.h"
#include "Log.h"
#include "LogSegment.h"

LogCursor::LogCursor(const Log& log, offset_t start)
	: m_log(log)
	, m_currentOffset(start)
	, m_currentSegmentPos(0)
{
	m_currentSegment = log.FindSegment(start);
}

bool LogCursor::Next(uint8_t*& key, uint32_t& keyLen, uint8_t*& value, uint32_t& valueLen)
{
	if (!m_currentSegment)
	{
		return false;
	}

	if (m_currentSegment->ReadMessage(m_currentOffset, m_currentSegmentPos, key, keyLen, value, valueLen))
	{
		return true;
	}
	else
	{
		m_currentSegmentPos = 0;
		if ((m_currentSegment = m_log.GetSegmentAfter(*m_currentSegment)))
		{
			return Next(key, keyLen, value, valueLen);
		}
		else
		{
			return false;
		}
	}
}