#ifndef __ENGINE_H__
#define __ENGINE_H__

#include <algorithm>
#include <vector>
#include <string>

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
		   int (*_callback)(const char*, int, int*),
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
	int (*callback)(const char*, int, int*);
	bool canSame;
	int isRandom;
	int bruteTime;
	int startTime;
	int iter;
	int haveLetters;
};

#include "engine.cpp"

#endif // __ENGINE_H__

