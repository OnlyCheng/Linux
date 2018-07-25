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

		//1����ȡ�ļ����ÿ���ַ����ֵĴ���
		GetCharInfo(filepath);

		//2����ÿ���ַ����ֵĴ�����ΪȨֵ������Huffman��
		HuffmanTree<CharInfo> hf(_fileInfo, 256, CharInfo());

		//��ȡHuffman���ĸ�
		HuffmanTreeNode<CharInfo>* Root = hf.GetRoot();

		//3��ͨ��Huffman����ȡÿ���ַ���Ӧ�ı���
		GetHuffmanCode(Root);

		//4������Դ�ļ���ʹ��ÿ���ַ��ı������¸�дԴ�ļ�
		CompressFile(filepath);
	}

	


private:
	//��ȡ�ļ���Ϣ
	void GetCharInfo(const char* filepath)
	{
		FILE* pfIn = fopen(filepath, "r");

		if (pfIn == NULL)
			cout << "�Ҳ������ļ�" << endl;

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

	//��ת�ַ���
	void myreverse(char* str)
	{
		char* first = str;
		char* last = str+strlen(str);
		while ((first != last) && (first != --last))
			swap(*first++, *last);
	}
	//��ȡ����
	void GetHuffmanCode(HuffmanTreeNode<CharInfo>* pRoot)
	{
		if (pRoot != NULL)
		{
			GetHuffmanCode(pRoot->_pLeft);
			GetHuffmanCode(pRoot->_pRight);

			//�ߵ�Ҷ�ӽ���λ��
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
				//���ַ�����ת
				myreverse((char*)NodeCode._strcode.c_str());
			}
		}
	}

	//�ļ�ѹ��
	void CompressFile(const char* filepath)
	{
		FILE* pfIn = fopen(filepath, "r");
		if (pfIn == NULL)
			cout << "�Ҳ������ļ�" << endl;

		FILE* pfOut = fopen("2.txt", "w");
		char c = '\n';

		//��ȡԴ�ļ��ĺ�׺��������д��ѹ���ļ���
		string s = filepath;
		char* suffix;
		int pos = s.find_last_of(filepath, '.');

		if (pos >= 0)
			suffix = (char*)s.c_str() + (s.size()-pos);
		else
			cout << "δ֪�ļ���" << endl;

		fputs(suffix,pfOut);
		fputc((int)c, pfOut);

		//��ȡͷ����Ϣ:��Ч�ַ�����+��Ч�ַ�
		int lineCount = 0;
		string InfoBuf;
		char countBuf[32] = { 0 };
		for (int i = 0; i < 256; i++)
		{
			//���ִ�����Ϊ0
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

		//д��ͷ����Ϣ
		char line = lineCount + '0';
		fputc(line, pfOut);
		fputc((int)c, pfOut);
		fputs(InfoBuf.c_str(), pfOut);

		//д��ѹ����Ϣ
		size_t readSize = 0;
		char str[1024] = { 0 };
		size_t strSize = 0;
		char ch = 0;
		int bit = 0;
		while (true)
		{
			readSize = fread(str, 1, 1024, pfIn);
			if (0 == readSize)//Դ�ļ��Ѷ���
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
		//�ļ������ˣ�����û����1024������δд������ļ���
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

