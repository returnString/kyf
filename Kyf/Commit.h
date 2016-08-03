#pragma once

class Commit
{
public:
	Commit(uint8_t* contents, commit_size_t size);

private:
	std::unique_ptr<uint8_t> m_contents;
	commit_size_t m_size;
};