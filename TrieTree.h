#pragma once
#include<vector>
#include<map>
using namespace std;

/*单个词最大长度*/
#define WORD_LEN_MAX 128

struct Trie{
	bool is_word;
	wchar_t word;
	map<wchar_t,int> word_index;
	vector<Trie*> son;
	Trie()
	{
		is_word = false;
		word = '\0';
	}
};

class TrieTree
{
public:
	TrieTree(void);
	~TrieTree(void);
	int travel();
	int del();
	int del(Trie* curr);
	int travel(Trie* curr);
	int find(wchar_t* str,int len);
	void printWord(wchar_t* str);
	int insert(wchar_t* str,int len);
private:
	 Trie* root;
};
