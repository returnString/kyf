#include "stdafx.h"
#include "Log.h"
#include "LogCursor.h"

int main()
{
	Log log("test_log", 1024 * 5);

	size_t iterations = 1000;
	for (size_t i = 1; i <= iterations; i++)
	{
		bool isEven = i % 2 == 0;
		std::string key(isEven ? "even" : "odd");
		auto value = std::to_string(i);
		log.Commit((uint8_t*)key.c_str(), (uint32_t)key.size(), (uint8_t*)value.c_str(), (uint32_t)value.size());
		std::cout << "Wrote new entry " << i << std::endl;
	}

	LogCursor cursor(log, 0);

	uint8_t* key;
	uint8_t* value;
	uint32_t keyLen, valueLen;

	std::unordered_set<offset_t> offsets;
	while (cursor.Next(key, keyLen, value, valueLen))
	{
		offset_t currentOffset = cursor.GetOffset();
		auto insertion = offsets.insert(currentOffset);
		assert(insertion.second);

		std::string keyStr((char*)key, keyLen);
		std::string valueStr((char*)value, valueLen);
		;
		std::cout << "Read message back ['" << keyStr << "'] '" << valueStr << "' (" << currentOffset << ")" << std::endl;
	}

	std::cout << "Read " << offsets.size() << " messages back" << std::endl;
	assert(offsets.size() == iterations);

	std::string dummy;
	std::getline(std::cin, dummy);
	return 0;
}

