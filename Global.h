#pragma once
#include "TokenSeg.h"
#include <vector>
#include <string>
using std::string;
using std::vector;
/*�ļ��������*/
#define MAX_DICT_LINES 100000
#define MAX_LM_LINES 300000
#define MAX_CORPUS_LINES 30000
/*����������󳤶�*/
#define MAX_LINE_LENGTH 1024

#define FIELD_COUNT 2

#define  FAIL -1
#define  SUCCESS 0

extern TokenSeg g_token_seg;

/*
	��ȡ�ļ�����
	@path: in �ļ�·��
	@return:0 �ɹ�;1ʧ��
*/
long readFileLength(char* path);

/*
	��ȡָ�����ȵ��ļ�����
	@path: in �ļ�·��
	@buff: out ��ȡ���ļ�����
	@size: in ��ȡ�ĳ���
	@return:0 �ɹ�;1ʧ��
*/
int readFileContent(char* path,char* buff,long size);

/*
	��ָ���ַ�����ĳ����ʶ���ָ�
	@source: in ���ָ���ַ���
	@text: out �зֺ�ĸ����ַ�
	@gap: in �ָ���
	@max_count :in text�Ĵ�С
	@split_count: out �зֳ������ַ�������
	@return:0 �ɹ�;1ʧ��
*/
int splitChar(char* source,char** text,char* gap,int max_count,int& split_count);


/*
	��ָ���ַ�����ĳ����ʶ���ָ�
	@source: in ���ָ���ַ���
	@delimit:in �ָ���
	@return:�зֺ���ַ���
*/
vector<string> splitString(const string& source,const string& delimit);


/*
	charת��Ϊwchar_t
	@source: in ��ת�����ַ���
	@dest:out ת������ַ���
	@max_len:in dest��󳤶�
	@return: -1 dest���Ȳ���;0�ɹ�
*/
int CharToWchar(const char* source,wchar_t* dest,int max_len);


/*
	wchar_tת��Ϊchar
	@source: in ��ת�����ַ���
	@dest:out ת������ַ���
	@max_len:in dest��󳤶�
	@return: -1 dest���Ȳ���;0�ɹ�
*/
int WcharToChar(const wchar_t* sour,char* dest,int max_len) ;


/*
	��ӡ��ά����
	@sour:in ��ά����
	@rows:in ��һά��С
	@cols:in �ڶ�ά��С
*/
void printMatrix(int** sour,int rows,int cols);