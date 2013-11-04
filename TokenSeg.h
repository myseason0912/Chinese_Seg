#pragma once
#include "TrieTree.h"
#include "LanguageModel.h"

class TokenSeg
{
public:
	TokenSeg(void);
	~TokenSeg(void);

	/*
	��ʼ��
	*/
	int init(char* dict_path,char* lm_path);


	/*
	���شʵ䵽Trie
	@path:in �ʵ�·��
	@return: -1 dest���Ȳ���;0�ɹ�
	*/
	int loadDict(char* word_path,bool freq);

	/*
	�ִʡ�ʹ�����·���ִ�
	@sour:in ���ִ��ַ���
	@len:in ���зֵ��ַ�������
	@dest:out �зֺ�Ľ��
	@max_count:in �зֺ����Ĵʸ���
	@return: -1 dest���Ȳ���;0�ɹ�
	*/
	int tokenize(wchar_t* sour,int len,wchar_t** dest,int& total_count,int max_count);



	/*
	����ִʺ���,���ȫ�ִ�
	@sour:in ���ִ��ַ���
	@len:in ���зֵ��ַ�������
	@begin:in ��ʼ�����ƫ����
	@dest:out �зֺ�Ľ��
	@total_count:out ȫ�зֺ�Ĵʸ���
	@max_count:in �зֺ����Ĵʸ���
	@done:in �Ѿ�����õ�ƫ����
	@return: -1 dest���Ȳ���;0�ɹ�
	*/
	int tokenize(wchar_t* sour,int len,int begin,float** dest,bool* done);

	/*
	�����ִʽ��������·����ʹ�ø���������·��һ�����зֽ��
	@result:in ȫ�ִʽ��
	@len:in result��С
	@index:in ��ǰ��������һ��
	@freq:in,out ����λ�÷ִʵ�������
	@token_count:in,out ����λ�÷ִʵ���С�ʸ���
	@done:in,out ����λ���Ƿ������
	@token_result:out �ִ�����
	@return:���طִʸ���
	*/
	int traverse(float** result,int len,int index,float *freq,int* token_count,bool* done,int* token_result);


private:
	TrieTree _word_trie;
	TrieTree _stop_trie;
	LanguageModel _language_model;

};
