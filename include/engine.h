#ifndef __ENGINE_H__
#define __ENGINE_H__

#include <algorithm>
#include <vector>
#include <string>
#include <random>
#include <iostream>
#include <fstream>
#include <assert.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>


template<int ALPH>
class Node
{
public:
	Node(bool _term = false);
	~Node();

	void setTerm(bool _term);
	bool getTerm();
	void setNext(int x, int _next);
	int getNext(int x);

private:
	int next[ALPH];
	bool term;
};

template<int ALPH>
class Dict
{
public:	
	Dict();
	~Dict();
	
	void createDict(const char *fileName);
	void add(const std::vector<char> &st);
	int nextNode(int v, int x);
	bool isTermNode(int v);
private:
	int newNode(bool term = false);
	
	std::vector<Node<ALPH>> nodes;
	int nodeNow;
};

template<int ALPH>
class Finder
{
public:
	Finder(Dict<ALPH> *_dict,
		   const std::vector<std::pair<char, int>> &_toFind,
		   int _minLen,
		   int _maxLen,
		   int (*_callback)(const char*, int, int*, void*),
		   void *_user,
		   bool _canSame = false,
		   int _isRandom = 0,
		   int _bruteTime = 10);	
	~Finder();

	void startFind();
private:
	bool findVertex(int v);
	bool checkTime();
	void convert_answer(char *&st,
				 		int &answerLength,
				 		int *&length);
	int returnAnswer();
	void addLetter(int letter);
	void delLetter(int letter);
	bool checkGo();
	bool checkUp();

	Dict<ALPH> *dict;
	std::vector<std::pair<char, int>> toFind;
	std::vector<int> now;
	std::vector<std::vector<char>> findNow;
	int minLen;
	int maxLen;
	int (*callback)(const char*, int, int*, void*);
	void *user;
	bool canSame;
	int isRandom;
	int bruteTime;
	int startTime;
	int iter;
	int haveLetters;
};

template<int ALPH>
Node<ALPH>::Node(bool _term)
{
	for (size_t i = 0; i < ALPH; ++i)
	{
		next[i] = -1;
	}
	term = _term;
}

template<int ALPH>
Node<ALPH>::~Node()
{
	return;
}

template<int ALPH>
void Node<ALPH>::setTerm(bool _term)
{
	term = _term;
}

template<int ALPH>
bool Node<ALPH>::getTerm()
{
	return term;
}

template<int ALPH>
void Node<ALPH>::setNext(int x, int _next)
{
	next[x] = _next;
}

template<int ALPH>
int Node<ALPH>::getNext(int x)
{
	return next[x];
}

template<int ALPH>
Dict<ALPH>::Dict()
{
	nodes.resize(2);
	nodeNow = 1;
}

template<int ALPH>
Dict<ALPH>::~Dict()
{
	nodes.resize(0);
	nodes.shrink_to_fit();
	
	nodeNow = 0;
}

template<int ALPH>
void Dict<ALPH>::createDict(const char *fileName)
{
	std::locale::global(std::locale("")); // I don't know why but it needed	
	std::wstring st;
	std::wifstream in(fileName);
	std::vector<char> now;

	std::cerr << "START BUILD" << std::endl;
	while (in >> st)
	{
		now.clear();
		now.resize(st.size());
		for (size_t i = 0; i < st.size(); ++i)
		{
			if (st[i] >= L'А' && st[i] <= L'Я')
			{
				st[i] = st[i] - L'А' + L'а';
			}
			
			assert(st[i] >= L'а' && st[i] <= L'я');
			now[i] = st[i] - L'а';
			assert(now[i] >= 0 && now[i] < ALPH);
		}

		add(now);
	}
	std::cerr << "END BUILD" << std::endl;
	std::cerr << nodeNow << std::endl;
	in.close();
}

template<int ALPH>
void Dict<ALPH>::add(const std::vector<char> &st)
{
	int now = 0;
	for (size_t i = 0; i < st.size(); ++i)
	{
		if (nextNode(now, (int)st[i]) == -1)
		{
			nodes[now].setNext((int)st[i], newNode(false));
		}
		now = nextNode(now, (int)st[i]);
	}

	nodes[now].setTerm(true);
}

template<int ALPH>
int Dict<ALPH>::nextNode(int v, int x)
{
	return nodes[v].getNext(x);
}

template<int ALPH>
bool Dict<ALPH>::isTermNode(int v)
{
	return nodes[v].getTerm();
}

template<int ALPH>
int Dict<ALPH>::newNode(bool term)
{
	if (nodeNow == (int)nodes.size())
	{
		nodes.resize(nodes.size() * 2);
	}
	
	nodes[nodeNow].setTerm(term);

	return nodeNow++;
}

template<int ALPH>
Finder<ALPH>::Finder(Dict<ALPH> *_dict,
					const std::vector<std::pair<char, int>> &_toFind,
					int _minLen,
					int _maxLen,
					int (*_callback)(const char*, int, int*, void*),
					void *_user,
					bool _canSame,
					int _isRandom,
					int _bruteTime)
{
	dict = _dict;
	
	haveLetters = 0;
	toFind.resize(_toFind.size());
	now.resize(_toFind.size());
	findNow.resize(1);
	findNow[0].resize(0);
	for (size_t i = 0; i < _toFind.size(); ++i)
	{
		toFind[i] = _toFind[i];
		haveLetters += toFind[i].second;
		now[i] = 0;
	}
	
	minLen = _minLen;
	maxLen = _maxLen;
	
	callback = _callback;
	user = _user;

	canSame = _canSame;
	isRandom = _isRandom;
	bruteTime = _bruteTime;
	
	iter = 0;
}

template<int ALPH>
Finder<ALPH>::~Finder()
{
	toFind.resize(0);
	toFind.shrink_to_fit();
	now.resize(0);
	now.shrink_to_fit();
	findNow.resize(0);
	findNow.shrink_to_fit();

	dict = nullptr;
	minLen = 0;
	maxLen = 0;
	callback = nullptr;
	user = nullptr;
	canSame = false;
	isRandom = false;
	bruteTime = 0;
	startTime = 0;
	iter = 0;
}

template<int ALPH>
void Finder<ALPH>::startFind()
{
	if (isRandom > 0)
	{
		std::mt19937 rnd;
		rnd.seed(isRandom);
		std::shuffle(toFind.begin(), toFind.end(), rnd);
	}
	
	startTime = clock();
	findVertex(0);
}

template<int ALPH>
bool Finder<ALPH>::findVertex(int v)
{
	++iter;
	if ((iter&255) == 0 && !checkTime())
	{
		return false;
	}
	
	if (haveLetters != 0 && v == 0 && (int)findNow.size() - 1 == maxLen)
	{
		return true;
	}

	if (haveLetters == 0 && v == 0 && (int)findNow.size() - 1 < minLen)
	{
		return true;
	}

	if (haveLetters == 0 && v == 0)
	{
		if (!returnAnswer())
		{
			return false;
		}
		return true;
	}
	
	if (dict->isTermNode(v) && checkUp())
	{
		findNow.push_back(std::vector<char>(0));
		findVertex(0);
		findNow.pop_back();
	}

	for (size_t i = 0; i < toFind.size(); ++i)
	{
		if (toFind[i].second > now[i])
		{
			addLetter(i);
			if (dict->nextNode(v, toFind[i].first) != -1 && checkGo())
			{
				if (!findVertex(dict->nextNode(v, toFind[i].first)))
				{
					return false;
				}
			}
			delLetter(i);
		}
	}

	return true;
}

template<int ALPH>
bool Finder<ALPH>::checkTime()
{
	return clock() - startTime <= CLOCKS_PER_SEC * bruteTime; 
}

template<int ALPH>
void Finder<ALPH>::convert_answer(char *&st,
								  int &answerLength,
								  int *&length)
{
	answerLength = (int)findNow.size() - 1;
	length = (int*)malloc(sizeof(int) * answerLength);

	int sz = 0;
	for (size_t i = 0; i < findNow.size() - 1; ++i)
	{
		length[i] = findNow[i].size();
		sz += findNow[i].size();
	}

	st = (char*)malloc(sizeof(char) * sz);

	int ptr = 0;
	for (size_t i = 0; i < findNow.size() - 1; ++i)
	{
		for (size_t j = 0; j < findNow[i].size(); ++j)
		{
			st[ptr++] = findNow[i][j];
		}
	}
	assert(ptr == sz);
}

template<int ALPH>
int Finder<ALPH>::returnAnswer()
{
	char *st;
	int answerLength;
	int *length;

	convert_answer(st, answerLength, length);
	int ans = callback(st, answerLength, length, user);
	free(st);
	free(length);

	return ans;
}

template<int ALPH>
void Finder<ALPH>::addLetter(int letter)
{
	--haveLetters;
	++now[letter];
	findNow.back().push_back(toFind[letter].first);
}

template<int ALPH>
void Finder<ALPH>::delLetter(int letter)
{
	++haveLetters;
	--now[letter];
	findNow.back().pop_back();
}

template<int ALPH>
bool Finder<ALPH>::checkGo()
{
	if (isRandom)
	{
		return true;
	}

	if (findNow.size() == 1)
	{
		return true;
	}
	return findNow.back() >= findNow[(int)findNow.size() - 2];
}

template<int ALPH>
bool Finder<ALPH>::checkUp()
{
	if (isRandom)
	{
		return true;
	}

	if (findNow.size() == 1)
	{
		return true;
	}

	if (canSame)
	{
		return findNow.back() >= findNow[(int)findNow.size() - 2];
	}
	else
	{
		return findNow.back() > findNow[(int)findNow.size() - 2];
	}
}


#endif // __ENGINE_H__

