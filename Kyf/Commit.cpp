#include "stdafx.h"
#include "Commit.h"

Commit::Commit(uint8_t* contents, commit_size_t size)
	: m_contents(contents)
	, m_size(size)
{
}