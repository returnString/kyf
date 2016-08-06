#pragma once

#ifdef _WIN32
#include <SDKDDKVer.h>
#include <windows.h>
#include <codecvt>
#define WINDOWS 1
#elif __linux
#define LINUX 1
#else
#error Unknown platform
#endif

#include <stdint.h>
#include <memory>
#include <map>
#include <unordered_set>
#include <assert.h>
#include <sstream>
#include <iostream>
#include <locale>
#include <cstring>

using offset_t = uint64_t;

namespace constants
{
	const char SegmentFileExtension[] = ".kyf";
}