#include "StdAfx.h"
#include "Global.h"
#include <stdlib.h>
#include <string.h>
#include <windows.h>

long readFileLength(char* path)
{
	if (path == NULL || *path == '\0')
	{
		return FAIL;
	}
	FILE* infile = fopen(path,"r");
	long ret = SUCCESS;
	if (infile == NULL)
	{
		printf("open file %s FAIL.\n",path);
		ret = FAIL;
	}
	else
	{
		fseek(infile,0,SEEK_END);	
		ret = ftell(infile);
		fclose(infile);
		infile = NULL;
	}

	return ret;
}

int readFileContent(char* path,char* buff,long size)
{
	if (path == NULL || *path == '\0' || buff == NULL || size == 0)
	{
		return FAIL;
	}
	FILE* infile = fopen(path,"rb");

	int ret = SUCCESS;
	if (infile == NULL)
	{
		printf("open file %s FAIL.\n",path);
		ret = FAIL;
	}
	else
	{
		ret  = fread(buff,size,1,infile);
		fclose(infile);
		infile = NULL;
	}

	return ret;
}

int splitChar(char* source,char** text,char* gap,int max_count,int& split_count)
{

	if(source == NULL || text == NULL || gap == NULL || max_count == 0)
	{
		return FAIL;
	}
	int gap_len = strlen(gap);
	split_count = 0;
	if (gap_len == 0)
	{
		text[0] = source;
		split_count = 1;
	}
	else
	{
		char* old = source;
		char* next = strstr(old,gap);
		while(next)
		{
			if (split_count < max_count)
			{
				*next = '\0';
				text[split_count++] = old;
				old = next + strlen(gap);
				next = strstr(old,gap);
			}
			else
			{
				break;
			}
		}
		if (*old != '\0')
		{
			if (split_count < max_count)
			{
				text[split_count++] = old;
			}
		}
	}

	return split_count;
}

vector<string> splitString(const string& source,const string& delimit)
{
	vector<string> tokens;
	if (source.empty() || delimit.empty())
	{
		return tokens;
	}
	int index = string::npos;
	int cursor = 0;
	int d_len = delimit.length();
	while ((index = source.find(delimit,cursor) )!= string::npos)
	{
		if(index > cursor)
		{
			tokens.push_back(source.substr(cursor,index - cursor));
		}
		cursor = index + d_len;
	}
	if (cursor < source.length() - 1)
	{
		tokens.push_back(source.substr(cursor));
	}
	return tokens;
}

void printMatrix(int** sour,int rows,int cols)
{
	for (int i = 0;i < rows;i++)
	{
		printf("%3d ",i);
		for (int j = 0;j < cols;j++)
		{
			if (sour[i][j] > 0)
				printf("%d %d\t",j,sour[i][j]);
		}
		printf("\n");
	}
}

int CharToWchar(const char* source,wchar_t* dest,int max_len)
{   
	int len = MultiByteToWideChar(CP_ACP,0,source,strlen(source),NULL,0);  
	if (max_len < len + 1)
	{
		return FAIL;
	}
	MultiByteToWideChar(CP_ACP,0,source,strlen(source),dest,len);  
	dest[len]='\0';   
	return SUCCESS;
}  

int WcharToChar(const wchar_t* sour,char* dest,int max_len)  
{  
	int len= WideCharToMultiByte(CP_ACP,0,sour,wcslen(sour),NULL,0,NULL,NULL);  
	if (max_len < len + 1)
	{
		return FAIL;
	}
	WideCharToMultiByte(CP_ACP,0,sour,wcslen(sour),dest,len,NULL,NULL);  
	dest[len]='\0';  
	return SUCCESS;  
}  



