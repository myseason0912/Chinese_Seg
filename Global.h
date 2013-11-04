#pragma once
#include "TokenSeg.h"
#include <vector>
#include <string>
using std::string;
using std::vector;
/*文件最大行数*/
#define MAX_DICT_LINES 100000
#define MAX_LM_LINES 300000
#define MAX_CORPUS_LINES 30000
/*单个句子最大长度*/
#define MAX_LINE_LENGTH 1024

#define FIELD_COUNT 2

#define  FAIL -1
#define  SUCCESS 0

extern TokenSeg g_token_seg;

/*
	读取文件长度
	@path: in 文件路径
	@return:0 成功;1失败
*/
long readFileLength(char* path);

/*
	读取指定长度的文件内容
	@path: in 文件路径
	@buff: out 读取的文件内容
	@size: in 读取的长度
	@return:0 成功;1失败
*/
int readFileContent(char* path,char* buff,long size);

/*
	将指定字符串按某个标识符分割
	@source: in 待分割的字符串
	@text: out 切分后的各个字符
	@gap: in 分隔符
	@max_count :in text的大小
	@split_count: out 切分出来的字符串个数
	@return:0 成功;1失败
*/
int splitChar(char* source,char** text,char* gap,int max_count,int& split_count);


/*
	将指定字符串按某个标识符分割
	@source: in 待分割的字符串
	@delimit:in 分隔符
	@return:切分后的字符串
*/
vector<string> splitString(const string& source,const string& delimit);


/*
	char转换为wchar_t
	@source: in 待转换的字符串
	@dest:out 转换后的字符串
	@max_len:in dest最大长度
	@return: -1 dest长度不够;0成功
*/
int CharToWchar(const char* source,wchar_t* dest,int max_len);


/*
	wchar_t转换为char
	@source: in 待转换的字符串
	@dest:out 转换后的字符串
	@max_len:in dest最大长度
	@return: -1 dest长度不够;0成功
*/
int WcharToChar(const wchar_t* sour,char* dest,int max_len) ;


/*
	打印二维数组
	@sour:in 二维数组
	@rows:in 第一维大小
	@cols:in 第二维大小
*/
void printMatrix(int** sour,int rows,int cols);