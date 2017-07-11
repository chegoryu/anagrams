#include "runner.h"
#include "engine.h"

bool is_init = false;
Dict<33> *dict = nullptr;

void initBrute(const char *fileName)
{
	if (!is_init)
	{
		is_init = true;
		
		dict = new Dict<33>;
		dict->createDict(fileName);
	}
}

void delBrute()
{
	is_init = false;
	delete dict;
	dict = nullptr;
}

void brute(const char *st,
		   int length,
		   int minLen,
		   int maxLen,
		   int (*callback)(const char*, int, int*),
		   int canSame,
		   int isRandom,
		   int bruteTime)
{
	assert(is_init);
	std::vector<char> vst(length);
	std::vector<std::pair<char, int>> arr;
	
	for (int i = 0; i < length; ++i)
	{
		vst[i] = st[i];
	}

	std::sort(vst.begin(), vst.end());
	arr.push_back({vst[0], 1});
	for (int i = 1; i < length; ++i)
	{
		if (arr.back().first == vst[i])
		{
			++arr.back().second;
		}
		else
		{
			arr.push_back({vst[i], 1});
		}
	}

	Finder<33> *finder = new Finder<33>(dict,
									    arr,
									    minLen,
									    maxLen,
									    callback,
									    canSame,
									    isRandom,
									    bruteTime);
	
	finder->startFind();
	delete finder;

	arr.resize(0);
	arr.shrink_to_fit();
	vst.resize(0);
	vst.shrink_to_fit();
}
