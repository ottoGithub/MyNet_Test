#ifndef _COMMON_LOG_H_
#define _COMMON_LOG_H_

#ifdef _WIN32
#define FILENAME(x) strrchr(x,'\\')?strrchr(x,'\\')+1:x
#elif defined(__GNUC__)
#define FILENAME(x) strrchr(x,'/')?strrchr(x,'/')+1:x
#endif

#define LOG_ERROR(...) {\
	fprintf(stderr, "%s: Line %d;\t", FILENAME(__FILE__), __LINE__);\
	fprintf(stderr, __VA_ARGS__); \
	fprintf(stderr,"\n"); \
}

#define LOG_INFO(...) {\
	fprintf(stdout, "%s: Line %d;\t", FILENAME(__FILE__), __LINE__);\
	fprintf(stdout, __VA_ARGS__); \
	fprintf(stdout,"\n"); \
}
#endif