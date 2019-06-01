#ifndef LOG_H
#define LOG_H

#define log(...) logm(__VA_ARGS__, NULL)

void logm(int level, const char *str, ...);

#endif
