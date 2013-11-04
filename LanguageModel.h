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
	ѵ������ģ��
	@corpus_path:in ����·��
	@outpath:in ����ģ�����·��
	@return:0�ɹ���1ʧ��
	*/
	int Train(char* corpus_path,char* outpath);

	/*
	��������ģ��
	@lm_path:in ����ģ��·��
	@return:0�ɹ���1ʧ��
	*/
	int load(char* lm_path);

	/*
	����key��ôʵĸ���
	@key:in ��
	@return:���ʳ����򷵻شʵĸ��ʣ����򷵻�0
	*/
	float get_prob(wstring key);
private:
	map<wchar_t,int> _letter_freq;
	map<wstring,int,ws_compare> _word_freq;
	map<wstring,float,ws_compare> _word_prob;

};
