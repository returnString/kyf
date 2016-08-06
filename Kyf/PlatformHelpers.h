#pragma once

#if WINDOWS
#define WINCALL(...) \
	{ \
		__VA_ARGS__; \
		DWORD lastError = GetLastError(); \
		if (lastError != ERROR_SUCCESS) \
		{ \
			LPTSTR message = nullptr; \
			FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS, \
				nullptr, lastError, LANG_NEUTRAL, (LPTSTR)&message, 0, nullptr); \
			std::wcerr << "Platform error occurred: " << message \
									<< "Executing: " << #__VA_ARGS__ << std::endl \
									<<  __FILE__  << ":" << __LINE__ << std::endl; \
			std::terminate(); \
		} \
	}
#elif LINUX
	#define LINUXCALL(...) \
	{ \
		__VA_ARGS__; \
		if (errno) \
		{ \
			std::cerr << "Platform error occurred: " << strerror(errno) << std::endl \
				<< "Executing: " << #__VA_ARGS__ << std::endl \
				<< __FILE__ << ":" << __LINE__ << std::endl; \
			std::terminate(); \
		} \
	}
#endif