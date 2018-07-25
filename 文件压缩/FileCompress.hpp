#define _CRT_SECURE_NO_WARNINGS
#pragma once
#include "heap.hpp"
#include "huffman.hpp"
#include <string>
#include <iostream>
using namespace std;

struct CharInfo
{
	CharInfo()
	:_count(0)
	{}

	bool operator>(CharInfo& right)
	{
		return _count > right._count;
	}

	bool operator<(CharInfo& right)
	{
		return _count < right._count;
	}

	bool operator==(CharInfo& right)
	{
		return _count == right._count;
	}

	bool operator!=(CharInfo& right)
	{
		return _count != right._count;
	}

	CharInfo operator+(CharInfo& right)
	{
		CharInfo temp;
		temp._count = _count + right._count;
		return temp;
	}

	char _ch;
	long long _count;
	string _strcode;
};

class Compress
{
public:
	Compress(const char* filepath)
	{
		for (int i = 0; i < 256; i++)
		{
			_fileInfo[i]._ch = i;
			_fileInfo[i]._count = 0;
		}

		//1、读取文件获得每个字符出现的次数
		GetCharInfo(filepath);

		//2、以每个字符出现的次数作为权值，创建Huffman树
		HuffmanTree<CharInfo> hf(_fileInfo, 256, CharInfo());

		//获取Huffman树的根
		HuffmanTreeNode<CharInfo>* Root = hf.GetRoot();

		//3、通过Huffman树获取每个字符对应的编码
		GetHuffmanCode(Root);

		//4、遍历源文件，使用每个字符的编码重新改写源文件
		CompressFile(filepath);
	}

	


private:
	//获取文件信息
	void GetCharInfo(const char* filepath)
	{
		FILE* pfIn = fopen(filepath, "r");

		if (pfIn == NULL)
			cout << "找不到该文件" << endl;

		size_t charSize = 0;
		char str[1024] = { 0 };
		while (true)
		{
			charSize = fread(str, 1, 1024, pfIn);
			if (0 == charSize)
				break;

			for (size_t i = 0; i < charSize; i++)
			{
				_fileInfo[str[i]]._count++;
			}
		}

		fclose(pfIn);
		
	}

	//反转字符串
	void myreverse(char* str)
	{
		char* first = str;
		char* last = str+strlen(str);
		while ((first != last) && (first != --last))
			swap(*first++, *last);
	}
	//获取编码
	void GetHuffmanCode(HuffmanTreeNode<CharInfo>* pRoot)
	{
		if (pRoot != NULL)
		{
			GetHuffmanCode(pRoot->_pLeft);
			GetHuffmanCode(pRoot->_pRight);

			//走到叶子结点的位置
			if (NULL == pRoot->_pLeft&&NULL == pRoot->_pRight)
			{
				HuffmanTreeNode<CharInfo>* parent = pRoot->_parent;
				CharInfo& NodeCode = _fileInfo[pRoot->_weight._ch];
				while (parent)
				{
					if (pRoot == parent->_pLeft)
						NodeCode._strcode += '0';
					else
						NodeCode._strcode += '1';
					pRoot = parent;
					parent = pRoot->_parent;
				}
				//将字符串反转
				myreverse((char*)NodeCode._strcode.c_str());
			}
		}
	}

	//文件压缩
	void CompressFile(const char* filepath)
	{
		FILE* pfIn = fopen(filepath, "r");
		if (pfIn == NULL)
			cout << "找不到该文件" << endl;

		FILE* pfOut = fopen("2.txt", "w");
		char c = '\n';

		//获取源文件的后缀名并将其写入压缩文件中
		string s = filepath;
		char* suffix;
		int pos = s.find_last_of(filepath, '.');

		if (pos >= 0)
			suffix = (char*)s.c_str() + (s.size()-pos);
		else
			cout << "未知文件名" << endl;

		fputs(suffix,pfOut);
		fputc((int)c, pfOut);

		//获取头部信息:有效字符行数+有效字符
		int lineCount = 0;
		string InfoBuf;
		char countBuf[32] = { 0 };
		for (int i = 0; i < 256; i++)
		{
			//出现次数不为0
			if (0 != _fileInfo[i]._count)
			{
				lineCount++;
				InfoBuf += _fileInfo[i]._ch;
				InfoBuf += ',';
				itoa((int)_fileInfo[i]._count, countBuf, 10);
				InfoBuf += countBuf;
				InfoBuf += '\n';
			}
		}

		//写入头部信息
		char line = lineCount + '0';
		fputc(line, pfOut);
		fputc((int)c, pfOut);
		fputs(InfoBuf.c_str(), pfOut);

		//写入压缩信息
		size_t readSize = 0;
		char str[1024] = { 0 };
		size_t strSize = 0;
		char ch = 0;
		int bit = 0;
		while (true)
		{
			readSize = fread(str, 1, 1024, pfIn);
			if (0 == readSize)//源文件已读完
				break;

			for (size_t i = 0; i < readSize; i++)
			{
				string _strCode = _fileInfo[str[i]]._strcode;
				char* strCode =(char*) _strCode.c_str();
				for (size_t j = 0; j < _strCode.size(); j++)
				{
					ch <<= 1;
					bit++;

					if ('1' == strCode[j])
						ch |= 1;
					if (8 == bit)
					{
						str[strSize++] = ch;
						bit = 0;
					}

					if (1024 == strSize)
					{
						fwrite(str, 1, 1024, pfOut);
						strSize = 0;
					}
				}	
			}
		}
		//文件读完了，但是没有满1024个，故未写入输出文件中
		if (strSize)
		{
			fwrite(str, 1, strSize, pfOut);
		}

		fclose(pfIn);
		fclose(pfOut);
	}

private:
	CharInfo _fileInfo[256];
};

