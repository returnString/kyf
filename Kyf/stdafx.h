#pragma once

#include <SDKDDKVer.h>
#include <stdint.h>
#include <memory>
#include <map>
#include <unordered_set>
#include <assert.h>
#include <windows.h>
#include <sstream>
#include <iostream>
#include <locale>
#include <codecvt>

using offset_t = uint64_t;

namespace constants
{
	const char SegmentFileExtension[] = ".kyf";
}