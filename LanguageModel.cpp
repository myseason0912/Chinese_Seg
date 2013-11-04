#include "StdAfx.h"
#include "LanguageModel.h"
#include "Global.h"
#include <math.h>

LanguageModel::LanguageModel(void)
{
}

LanguageModel::~LanguageModel(void)
{
}

int LanguageModel::Train(char* corpus_path,char *outpath)
{
	int ret = SUCCESS;
	long file_length =readFileLength(corpus_path);
	char* buff = (char*)calloc(file_length+1,sizeof(char));
	if (buff == NULL)
	{
		printf("calloc memory for corpus file FAIL.\n");
		ret = FAIL;
		return ret;
	}
	ret = readFileContent(corpus_path,buff,file_length);
	if(ret == FAIL)
	{
		return ret;
	}
	char** lines = (char**)calloc(MAX_CORPUS_LINES,sizeof(char*));
	wchar_t* w_line = (wchar_t*)calloc(MAX_LINE_LENGTH,sizeof(wchar_t));
	if (lines == NULL || w_line == NULL)
	{
		printf("calloc memory for corpus FAIL.\n");
		ret = FAIL;
		return ret;
	}
	int line_count = 0;
	ret = splitChar(buff,lines,"\r\n",MAX_CORPUS_LINES,line_count);
	if (ret == FAIL)
	{
		printf("split char FAIL.\n");
		return ret;
	}
	int i = 0;
	int j = 0;
	int line_length = 0;
	/*统计单个字出现的次数*/
	/*统计bigram各个词对出现的次数*/
	wstring word;
	word.resize(N_GRAM);
	for (i = 0;i < line_count;i++)
	{
		w_line[0] = '\0';
		ret = CharToWchar(lines[i],w_line,MAX_LINE_LENGTH);
		if (ret != -1)
		{
			line_length = wcslen(w_line);
			for (j = 0;j < line_length;j++)
			{
				map<wchar_t,int>::iterator l_iter = _letter_freq.find(w_line[j]);
				if (l_iter != _letter_freq.end())
				{
					l_iter->second++;
				}
				else
				{
					_letter_freq.insert(make_pair(w_line[j],1));
				}
				if (j != line_length + 1 - N_GRAM)
				{
					for (int k = 0;k < N_GRAM;k++)
					{
						word[k] = w_line[j+k];
					}
					map<wstring,int,ws_compare>::iterator w_iter = _word_freq.find(word);
					if (w_iter != _word_freq.end())
					{
						w_iter->second++;
					}
					else
					{
						_word_freq.insert(make_pair(word,1));
					}
				}
			}
		}
	}
	
	/*计算条件概率*/
	FILE* out_file = fopen(outpath,"wb");
	char enter[] = "\r\n";
	int enter_len = strlen(enter);
	char prob_s[WORD_LEN_MAX];
	char c_word[WORD_LEN_MAX];
	for (map<wstring,int,ws_compare>::iterator iter1 = _word_freq.begin();iter1 != _word_freq.end();++iter1)
	{
		map<wchar_t,int>::iterator l_iter = _letter_freq.find(iter1->first[1]);
		if (l_iter != _letter_freq.end())
		{
			double prob = log((double)iter1->second / l_iter->second);
			c_word[0] = '\0';
			ret = WcharToChar(iter1->first.c_str(),c_word,WORD_LEN_MAX);
			if(ret != -1)
			{
				sprintf_s(prob_s,"%.6lf",prob);
				fwrite(c_word,strlen(c_word),sizeof(char),out_file);
				fwrite(prob_s,strlen(prob_s),sizeof(char),out_file);
				fwrite(enter,enter_len,sizeof(char),out_file);
			}
		}
	}
	if (buff != NULL)
	{
		free(buff);
		buff = NULL;
	}
	if (lines != NULL)
	{
		free(lines);
		lines = NULL;
	}
	if (w_line != NULL)
	{
		free(w_line);
		w_line = NULL;
	}

	return SUCCESS;
}


int LanguageModel::load(char* lm_path)
{
	int ret = SUCCESS;
	long file_length =readFileLength(lm_path);
	char* buff = (char*)calloc(file_length+1,sizeof(char));
	if (buff == NULL)
	{
		printf("calloc memory for lm file FAIL.\n");
		ret = FAIL;
		return ret;
	}
	ret = readFileContent(lm_path,buff,file_length);
	if(ret == FAIL)
	{
		return ret;
	}
	char** lines = (char**)calloc(MAX_LM_LINES,sizeof(char*));
	wchar_t* w_line = (wchar_t*)calloc(MAX_LINE_LENGTH,sizeof(wchar_t));
	if (lines == NULL || w_line == NULL)
	{
		printf("calloc memory for lm FAIL.\n");
		ret = FAIL;
		return ret;
	}
	int line_count = 0;
	ret = splitChar(buff,lines,"\r\n",MAX_LM_LINES,line_count);
	if (ret == FAIL)
	{
		printf("split char FAIL.\n");
		return ret;
	}
	int i = 0;
	int j = 0;
	wstring word;
	word.resize(N_GRAM);
	char freq[WORD_LEN_MAX];
	wchar_t* old = w_line;
	for (i = 0;i < line_count;i++)
	{
		w_line = old;
		w_line[0] = '\0';
		ret = CharToWchar(lines[i],w_line,MAX_LINE_LENGTH);
		if (ret != -1)
		{
			for (j = 0;j < N_GRAM;j++)
			{
				word[j] = w_line[j];
			}
			w_line = w_line + N_GRAM;

			ret = WcharToChar(w_line,freq,WORD_LEN_MAX);
			if (ret != -1)
			{
				float prob = 0;
				prob = atof(freq);
				_word_prob.insert(make_pair(word,prob));
			}
		}
	}
	if (buff != NULL)
	{
		free(buff);
		buff = NULL;
	}
	if (lines != NULL)
	{
		free(lines);
		lines = NULL;
	}
	if (old != NULL)
	{
		free(old);
		old = NULL;
	}

	return SUCCESS;
}

float LanguageModel::get_prob(wstring key)
{
	map<wstring,float,ws_compare>::iterator iter1 = _word_prob.find(key);
	if (iter1 != _word_prob.end())
	{
		return iter1->second;
	}
	else
	{
		return 0;
	}
}