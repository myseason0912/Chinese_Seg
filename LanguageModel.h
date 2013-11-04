#pragma once
#include <map>
#include "TrieTree.h"
using namespace std;

#define N_GRAM 2
struct ws_compare{
	bool operator()(wstring a,wstring b) const{
		return a.compare(b) < 0 ;
	}
};

class LanguageModel
{
public:
	LanguageModel(void);
	~LanguageModel(void);
	/*
	训练语言模型
	@corpus_path:in 语料路径
	@outpath:in 语言模型输出路径
	@return:0成功，1失败
	*/
	int Train(char* corpus_path,char* outpath);

	/*
	加载语言模型
	@lm_path:in 语言模型路径
	@return:0成功，1失败
	*/
	int load(char* lm_path);

	/*
	根据key获得词的概率
	@key:in 词
	@return:若词出现则返回词的概率，否则返回0
	*/
	float get_prob(wstring key);
private:
	map<wchar_t,int> _letter_freq;
	map<wstring,int,ws_compare> _word_freq;
	map<wstring,float,ws_compare> _word_prob;

};
