#pragma once
#include "TrieTree.h"
#include "LanguageModel.h"

class TokenSeg
{
public:
	TokenSeg(void);
	~TokenSeg(void);

	/*
	初始化
	*/
	int init(char* dict_path,char* lm_path);


	/*
	加载词典到Trie
	@path:in 词典路径
	@return: -1 dest长度不够;0成功
	*/
	int loadDict(char* word_path,bool freq);

	/*
	分词。使用最短路径分词
	@sour:in 待分词字符串
	@len:in 待切分的字符串长度
	@dest:out 切分后的结果
	@max_count:in 切分后最多的词个数
	@return: -1 dest长度不够;0成功
	*/
	int tokenize(wchar_t* sour,int len,wchar_t** dest,int& total_count,int max_count);



	/*
	具体分词函数,获得全分词
	@sour:in 待分词字符串
	@len:in 待切分的字符串长度
	@begin:in 开始处理的偏移量
	@dest:out 切分后的结果
	@total_count:out 全切分后的词个数
	@max_count:in 切分后最多的词个数
	@done:in 已经处理好的偏移量
	@return: -1 dest长度不够;0成功
	*/
	int tokenize(wchar_t* sour,int len,int begin,float** dest,bool* done);

	/*
	遍历分词结果获得最短路径，使用概率来区分路径一样的切分结果
	@result:in 全分词结果
	@len:in result大小
	@index:in 当前遍历的哪一行
	@freq:in,out 各个位置分词的最大概率
	@token_count:in,out 各个位置分词的最小词个数
	@done:in,out 各个位置是否遍历过
	@token_result:out 分词序列
	@return:返回分词个数
	*/
	int traverse(float** result,int len,int index,float *freq,int* token_count,bool* done,int* token_result);


private:
	TrieTree _word_trie;
	TrieTree _stop_trie;
	LanguageModel _language_model;

};
