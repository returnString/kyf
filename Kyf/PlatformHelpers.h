#pragma once

#ifdef _WIN32
#define WINCALL(...) \
	{ \
		__VA_ARGS__; \
		DWORD lastError = GetLastError(); \
		if (lastError != ERROR_SUCCESS) \
		{ \
			LPTSTR message = nullptr; \
			FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS, \
				nullptr, lastError, LANG_NEUTRAL, (LPTSTR)&message, 0, nullptr); \
			std::wcerr << "Windows error occurred: " << message \
									<< "Executing: " << #__VA_ARGS__ << std::endl \
									<<  __FILE__  << ":" << __LINE__ << std::endl; \
			std::terminate(); \
		} \
	}
#endif

#define UMIMPLEMENTED(thing) static_assert(false, "Not implemented for this platform: " thing)