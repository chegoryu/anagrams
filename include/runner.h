#ifndef __RUNNER_H__
#define __RUNNER_H__

#include "engine.h"

void initBrute(const char *fileName);
void delBrute();
void brute(const char *st,
		   int length,
		   int minLen,
		   int maxLen,
		   int (*callback)(const char*, int, int*),
		   int canSame,
		   int isRandom,
		   int bruteTime);

#endif // __RUNNER_H__
