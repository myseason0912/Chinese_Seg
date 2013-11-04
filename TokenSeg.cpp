#include "StdAfx.h"
#include "TokenSeg.h"
#include "Global.h"
#include <stdlib.h>
#include <string.h>

TokenSeg::TokenSeg(void)
{
}

TokenSeg::~TokenSeg(void)
{
}

int TokenSeg::init(char* dict_path,char* lm_path)
{
	int ret = SUCCESS;
	ret = loadDict(dict_path,false);
	if(ret == FAIL)
	{
		printf("load dict failed. program exit.\n");
		return ret;
	}

	ret = _language_model.load(lm_path);
	if(ret == FAIL)
	{
		printf("load lm failed. program exit.\n");
		return ret;
	}
	return ret;
}

int TokenSeg::loadDict(char* path,bool stop)
{
	int ret = SUCCESS;
	if (path == NULL || *path == '\0')
	{
		ret = FAIL;
		return ret;
	}
	long file_length =readFileLength(path);
	char* buff = (char*)calloc(file_length+1,sizeof(char));
	if (buff == NULL)
	{
		printf("calloc memory for dict FAIL.\n");
		ret = FAIL;
		return ret;
	}
	ret = readFileContent(path,buff,file_length);
	if(ret == FAIL)
	{
		return ret;
	}
	char** lines = (char**)calloc(MAX_DICT_LINES,sizeof(char*));

	if (lines == NULL)
	{
		printf("calloc memory for dict FAIL.\n");
		ret = FAIL;
		return ret;
	}

	int line_count = 0;
	ret = splitChar(buff,lines,"\r\n",MAX_DICT_LINES,line_count);
	if (ret == FAIL)
	{
		printf("split char FAIL.\n");
		return ret;
	}
	wchar_t word[WORD_LEN_MAX];
	for (int i = 0;i < line_count;i++)
	{
		if (stop)
		{
			ret = CharToWchar(lines[i],word,WORD_LEN_MAX);
			if (ret != FAIL)
			{
				_stop_trie.insert(word,wcslen(word));
			}
		}
		else
		{
			ret = CharToWchar(lines[i],word,WORD_LEN_MAX);
			if (ret != FAIL)
			{
				_word_trie.insert(word,wcslen(word));
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
	ret = SUCCESS;
	return ret;
}

int TokenSeg::tokenize(wchar_t* sour,int len,int begin,float** dest,bool* done)
{
	if(sour == NULL || len == 0)
	{
		return FAIL;
	}
	int i =0;
	wchar_t* temp = (wchar_t*)calloc(len+1,sizeof(wchar_t));
	if (temp == NULL)
	{
		return FAIL;
	}
	temp[0] = '\0';
	int start = begin;
	int end = begin + 1;
	wchar_t* sub_sour = sour;
	wchar_t* old = sour;
	wstring gram_word;
	gram_word.resize(N_GRAM);
	for(i = begin;i < len;i++)
	{
		end = i+1;
		wcsncpy(temp,sour+start,end-start);
		temp[end-start] = '\0';

		float prob = 0;
		int w_len = wcslen(temp);
		if(_word_trie.find(temp,w_len) == SUCCESS)
		{
			/*计算该词的概率*/
			for (int j = 0;j < w_len;j++)
			{	
				if (j != w_len + 1 - N_GRAM)
				{
					for (int k = 0;k < N_GRAM;k++)
					{
						gram_word[k] = temp[j + k];
					}
					prob += _language_model.get_prob(gram_word);
				}
			}

			/*作为单独一个词*/
			dest[start][end] = prob;
			begin = end;
			if (!done[begin])
			{
				tokenize(sour,len,begin,dest,done);
				done[begin] = true;
			}
		}

	}
	if (temp != NULL)
	{
		free(temp);
		temp = NULL;
	}
	return SUCCESS;
}

int TokenSeg::traverse(float** result,int len,int index,float *freq,int* token_count,bool* done,int* token_result)
{
	if (result == NULL || freq == NULL || token_count == NULL 
		|| done == NULL || token_result == NULL)
	{
		return FAIL;
	}
	int old = len;
	int cur = 0;
	int best_j = 0;
	float old_freq = 0;
	float cur_freq = 0;
	bool exist = false;
	int j = 0;
	for (j = 0;j < len;j++)
	{
		if (result[index][j] <= 0)
		{
			exist = true;
			if (!done[j])
			{
				traverse(result,len,j,freq,token_count,done,token_result);
			}
			cur = 1 + token_count[j];
			cur_freq = result[index][j] + freq[j];

			if (cur > old)
			{
				cur = old;
			}
			else
			{
				if (old == cur)
				{
					if (cur_freq > old_freq)
					{
						best_j = j;
						old_freq = cur_freq;
					}
					else
					{
						cur_freq = old_freq;
					}
				}
				else
				{
					old = cur;
					old_freq = cur_freq;
					best_j = j;
				}

			}
		}

	}
	if (!exist)
	{
		j = index + 1;
		if ( j < len)
		{
			if (!done[j])
			{
				traverse(result,len,j,freq,token_count,done,token_result);
			}
			cur = 1 + token_count[j];
			token_result[index] = j;
		}
		else
		{
			cur = 0;
			token_result[index] = j;
		}
	}
	else
	{
		token_result[index] = best_j;
	}
	done[index] = true;
	token_count[index] = cur;
	freq[index] = cur_freq;
	return SUCCESS;
}

int TokenSeg::tokenize(wchar_t* sour,int len,wchar_t** dest,int& total_count,int max_count)
{
	if(sour == NULL || len == 0 || dest == NULL || max_count == 0)
	{
		return FAIL;
	}
	len = len + 1;
	float** result = (float**)calloc(len,sizeof(float*));
	bool* done = (bool*)calloc(len,sizeof(bool));
	float* freq = (float*)calloc(len,sizeof(float));
	int* token_count = (int*)calloc(len,sizeof(int));
	int* token_result = (int*)calloc(len,sizeof(int));
	if(result == NULL || done == NULL || freq == NULL || token_count == NULL || token_result == NULL)
	{
		goto FAILs;
	}
	int i = 0;
	int j = 0;
	for (i = 0;i < len;i++)
	{
		result[i] = (float*)calloc(len,sizeof(float));
		done[i] = false;
	}
	for (i = 0;i < len;i++)
	{
		for(j = 0;j < len;j++)
		{
			result[i][j] = 1;
		}
	}
	/*全分词*/
	tokenize(sour,len-1,0,result,done);
	//printMatrix(result,len,len);

	/*遍历分词结果*/

	for (int i = 0;i < len;i++)
	{
		done[i] = false;
		token_count[i] = 0;
		freq[i] = 0;
	}
	traverse(result,len,0,freq,token_count,done,token_result);
	total_count = token_count[0];

	/*分词结果拷贝*/
	i = 0;
	j = 0;
	while(j < total_count)
	{
		dest[j][0] = '\0';
		wcsncat(dest[j],sour+i,token_result[i]-i);
		i = token_result[i];
		j++;
	}

FAILs:
	for (i = 0;i < len;i++)
	{
		if (result[i] != NULL)
		{
			free(result[i]);
			result[i] = NULL;
		}
	}
	if (result != NULL)
	{
		free(result);
	}
	if (token_result != NULL)
	{
		free(token_result);
	}
	if (freq != NULL)
	{
		free(freq);
	}
	if (token_count != NULL)
	{
		free(token_count);
	}
	if (done != NULL)
	{
		free(done);
	}
	return SUCCESS;
}
