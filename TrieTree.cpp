#include "StdAfx.h"
#include "TrieTree.h"
#include <stdlib.h>
#include <iostream>
#include "Global.h"

TrieTree::TrieTree(void)
{
	root = new Trie();
}

TrieTree::~TrieTree(void)
{
	del();
}

int TrieTree::insert(wchar_t* str,int len)
{
	if (str == NULL || str[0] == '\0')
	{
		return -1;
	}
	Trie* curr = root;
	Trie* new_node = NULL;
	for (int i = 0;i < len;i++)
	{
		map<wchar_t,int>::iterator word_iter = curr->word_index.find(str[i]); 
		if (word_iter == curr->word_index.end())
		{
			new_node = new Trie();
			new_node->word = str[i];
			curr->son.push_back(new_node);
			curr->word_index.insert(make_pair(str[i],curr->son.size() - 1));
			curr = curr->son[curr->son.size() - 1];
		}
		else
		{
			curr = curr->son[word_iter->second];
		}
	}
	curr->is_word = true;
	return 0;
}

int TrieTree::travel()
{
	return travel(root);
}

int TrieTree::del()
{
	return del(root);
}

int TrieTree::travel(Trie* curr)
{
	static wchar_t word[WORD_LEN_MAX];
	static int pos = 0;
	if (curr == NULL)
	{
		return 0;
	}
	if (curr->is_word)
	{
		printWord(word);
		word[pos] = '\0';
	}
	for (int i = 0;i < curr->son.size();i++)
	{
		word[pos++]=curr->son[i]->word;
		travel(curr->son[i]);
		pos--;
	}
	return 0;
}

int TrieTree::del(Trie* curr)
{
	if (curr == NULL)
	{
		return 0;
	}
	for (int i = 0;i < curr->son.size();i++)
	{
		del(curr->son[i]);
	}
	delete curr;
	curr = NULL;
	return 0;
}

int TrieTree::find(wchar_t* str,int len)
{
	if (str == NULL || str[0] == '\0')
	{
		return FAIL;
	}
	Trie* curr = root;
	for (int i = 0;i < len;i++)
	{
		map<wchar_t,int>::iterator word_iter = curr->word_index.find(str[i]);
		if(word_iter == curr->word_index.end())
		{
			return FAIL;
		}
		else
		{
			curr = curr->son[word_iter->second];
		}
	}
	return SUCCESS;

}

void TrieTree::printWord(wchar_t* str)
{
	std::wcout.imbue(std::locale("chs"));
	wcout<<str<<endl;
}
