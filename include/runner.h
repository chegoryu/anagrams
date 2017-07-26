#ifndef __RUNNER_H__
#define __RUNNER_H__

#ifdef __cplusplus
extern "C" {
#endif
    void initBrute(const char *fileName);
    void delBrute();
    void brute(const char *st,
               int length,
               int minLen,
               int maxLen,
               int minWordLen,
			   int maxWordLen,
			   int (*callback)(const char*, int, int*, void*),
               void *user,
			   int acceptBadTail,
			   int canSame,
               int isRandom,
               int bruteTime);
#ifdef __cplusplus
}
#endif

#endif // __RUNNER_H__
