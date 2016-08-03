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

using offset_t = uint64_t;

template<class ArrayType, size_t size>
size_t ArraySize(ArrayType(&)[size]) { return size; }

const char SegmentFileExtension[] = ".kyf";