// Chinese_Seg.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Global.h"
#include <iostream>
#include <string>
#include <fstream>

using namespace std;

TokenSeg g_token_seg;


int init()
{
	return g_token_seg.init(".\\dict\\word.dict",".\\model\\lm");
}

int test()
{
	int ret = 0;
	char path[] = ".\\test\\open_ini.txt";
	long file_length =readFileLength(path);
	char* buff = (char*)calloc(file_length+1,sizeof(char));
	if (buff == NULL)
	{
		printf("calloc memory for test file FAIL.\n");
		ret = -1;
		return ret;
	}
	ret = readFileContent(path,buff,file_length);
	if(ret == -1)
	{
		ret = -1;
		return ret;
	}
	char** lines = (char**)calloc(MAX_CORPUS_LINES,sizeof(char*));
	if (lines == NULL)
	{
		printf("calloc memory for dict FAIL.\n");
		ret = -1;
		return ret;
	}
	int line_count = 0;
	ret = splitChar(buff,lines,"\r\n",MAX_CORPUS_LINES,line_count);
	if (ret == -1)
	{
		printf("split char FAIL.\n");
		ret = -1;
		return ret;
	}
	wchar_t* word = (wchar_t*)calloc(MAX_LINE_LENGTH,sizeof(wchar_t));
	wchar_t** dest = (wchar_t**)calloc(MAX_LINE_LENGTH,sizeof(wchar_t*));
	char* c_word = (char*)calloc(WORD_LEN_MAX*2,sizeof(char));
	if (dest == NULL || word == NULL || c_word == NULL)
	{
		return -1;
	}
	int i = 0;
	for(i = 0;i < MAX_LINE_LENGTH;i++)
	{
		dest[i] = (wchar_t*)calloc(WORD_LEN_MAX,sizeof(wchar_t));
	}
	FILE* outfile = fopen(".\\test\\open_result.txt","wb");
	char enter[] = "\r\n";
	char blank[] = " ";
	int blank_len = strlen(blank);
	int enter_len = strlen(enter);
	for (i = 0;i < line_count;i++)
	{
		word[0] = '\0';
		ret = CharToWchar(lines[i],word,MAX_LINE_LENGTH);
		if (ret != -1)
		{
			int split_count = 0;
			g_token_seg.tokenize(word,wcslen(word),dest,split_count,MAX_LINE_LENGTH);

			for(int j = 0;j < split_count ;j++)
			{
				ret = WcharToChar(dest[j],c_word,WORD_LEN_MAX*2);
				if(ret != -1)
				{
					fwrite(c_word,strlen(c_word),sizeof(char),outfile);
					if (j < split_count - 1)
					{
						fwrite(blank,blank_len,sizeof(char),outfile);
					}
				}
			}
			fwrite(enter,enter_len,sizeof(char),outfile);
			
		}
		if (i % 1000 == 0)
		{
			printf("%d\n",i+1);
		}
	}

	fclose(outfile);

	if(buff != NULL)
	{
		free(buff);
		buff = NULL;
	}
	if (lines != NULL)
	{
		free(lines);
		lines = NULL;
	}
	for(i = 0;i < MAX_LINE_LENGTH;i++)
	{
		if (dest[i] != NULL)
		{
			free(dest[i]);
		}
	}
	if (dest != NULL)
	{
		free(dest);
	}
	if (word != NULL)
	{
		free(word);
	}
	if (c_word != NULL)
	{
		free(c_word);
	}
}

void trainLM()
{
	char corpus_path[] = ".\\data\\corpus";
	char out_lm[] = ".\\model\\lm";
	LanguageModel lm;
	lm.Train(corpus_path,out_lm);
}

void calculatePrecision()
{
	char seg_path[] = ".\\test\\open_result.txt";
	char answer_path[] = ".\\test\\open_seg.txt";
	char precision_path[] = ".\\test\\open_precision.txt";
	vector< vector<string> > seg;
	vector< vector<string> > answer;
	string line;
	ifstream seg_file(seg_path);
	if (!seg_file.is_open())
	{
		cout << "open seg file error" << endl;
		return ;
	}
	while (getline(seg_file,line))
	{
		vector<string> tokens = splitString(line," ");
		seg.push_back(tokens);
	}
	seg_file.close();

	ifstream answer_file(answer_path);
	if (!answer_file.is_open())
	{
		cout << "open answer file error " << endl;
		return;
	}

	while (getline(answer_file,line))
	{
		vector<string> tokens = splitString(line," ");
		answer.push_back(tokens);
	}
	answer_file.close();

	/*计算正确率和召回率*/
	if (seg.size() != answer.size())
	{
		cout << "sentence count not equal." << endl;
		return;
	}
	int total_count = 0;
	int seg_token_count = 0;
	int answer_token_count = 0;
	for (int i = 0;i < seg.size();i++)
	{
		int j = 0;
		int k = 0;
		seg_token_count += seg[i].size();
		answer_token_count += answer[i].size();
		bool shorter = false;
		while (j < seg[i].size() && k < answer[i].size())
		{
			if (seg[i][j] == answer[i][k])
			{
				j++;
				k++;
				total_count++;
			}
			else
			{
				if (seg[i][j][0] == answer[i][k][0])
				{
					/*第一个字符相同，但整体不同*/
					j++;
					k++;
				}
				else
				{
					
					if (shorter)
					{
						j++;	
					}
					else
					{
						k++;
					}
					if (j < seg[i].size() && k < answer[i].size())
					{
						if (seg[i][j].length() < answer[i][k].length())
						{
							shorter = true;
						}
					}
				}
			}
		}
	}

	ofstream precision(precision_path);
	if (precision.is_open())
	{
		precision  << "seg count : " << seg_token_count << ";answer count : " << answer_token_count 
					<< ";common count : " << total_count << endl;
		precision << "precision : " << (double)total_count / seg_token_count << endl;
		precision << "recall : " << (double)total_count / answer_token_count << endl;
	}
	precision.close();
}

int _tmain(int argc, _TCHAR* argv[])
{

	if (init() == -1)
	{
		return 0;
	}
	test();
	//trainLM();
	//calculatePrecision();
	return 0;
}

