#define _CRT_SECURE_NO_WARNINGS 1

#include<iostream>
#include<vector>
#include<string>
using namespace std;

class Maze
{
public:
	Maze(int _n, int _m, string _str)   //���캯������ʼ����Ա
		:n(2*_n+1)
		, m(2*_m+1)
		, str(_str)
	{
		Map.resize(n);               //����Ⱦ�����ʼ����ȫ0
		for (int i = 0; i < n; i++)
		{
			Map[i].resize(m, 0);
		}
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < m; j++)
			{
				if (i % 2 == 1 && j % 2 == 1)
					Map[i][j] = 1;
			}
		}
	}
	void Print()     //��ӡ��Ⱦ����
	{
		if (Insert() == false)        //���ú�����������Ⱦ���񣬽�ͨ·����
			return;
		for (int i = 0; i < n; i++)             //����ѭ����������Ⱦ����
		{
			for (int j = 0; j < m; j++)
			{
				if (Map[i][j] == 0)
					cout << "[W]" << " ";       //��ӡǽ��
				else if (Map[i][j] == 1)
					cout << "[R]" << " ";       //��ӡ·��
			}
			cout << endl;
		}
		cout << endl;
	}
private:
	bool Insert()        //������Ⱦ����
	{
		int x = -1, y = -1;      //�������������������һ��������Ϣ
		int x2 = -1, y2 = -1;     //������������������ڶ���������Ϣ
		int size = str.size();
		int i = 0;
		while (i<size)
		{
			if (str[i] >= '0'&&str[i] <= '9')
			{
				if (x == -1 && y == -1)
					x = 2 * (str[i] - '0') + 1;         //��ȡx��ֵ
				else if (y == -1)
					y = 2 * (str[i] - '0') + 1;          //��ȡy��ֵ
				else if (x2 == -1 && y2 == -1)
					x2 = 2 * (str[i] - '0') + 1;        //��ȡx2��ֵ
				else if (y2 == -1)
					y2 = 2 * (str[i] - '0') + 1;        //��ȡy2��ֵ
			}
			else if (str[i] == ';')         //��Ϊ';'ʱ������(x,y)(x2,y2)
			{
				x = -1, y = -1;
				x2 = -1, y2 = -1;
			}
			if (x != -1 && y != -1 && x2 != -1 && y2 != -1)    //�ж��Ƿ��������궼�Ѹ�ֵ
			{
				int min = 0;     
				int max = 0;
				if (x == x2)
				{
					min = y < y2 ? y : y2;             //ȡ��С��ֵ
					max = y>y2 ? y : y2;               //ȡ�ϴ��ֵ
					if (max - min != 2)            //�������겻��ͨ
					{
						cout << "Maze format error." << endl;
						return false;
					}
					Map[x][min + 1] = 1;
				}
				else if (y == y2)
				{
					min = x < x2 ? x : x2;             //ȡ��С��ֵ
					max = x>x2 ? x : x2;               //ȥ�ϴ��ֵ
					if (max - min != 2)           //�������겻��ͨ
					{
						cout << "Maze format error." << endl;
						return false;
					}
					Map[min + 1][y] = 1;
				}
				else                   //�������겻��ͨ
				{
					cout << "Maze format error." << endl;
					return false;
				}
			}
			i++;
		}
		return true;          //����ɹ�
	}
private:
	vector<vector<int>> Map;      //������Ⱦ����
	int n;                        //��Ⱦ���������
	int m;                        //��Ⱦ���������
	string str;                   //����������±���Ϣ
};
bool JudgeInput(string str,int n,int m)      //�ж�����������Ƿ���Ч
{
	int size = str.size();      //�����ַ��ĸ���
	int i = 0;
	int flag = 0;               //�����һ�������ж����Ƿ���֣���ֻ�ܳ���һ��
	while (i<size)
	{
		if (i == 0)
		{
			while (i<size&&str[i] == ' ')      //�����ײ��Ŀո�
				i++;
		}
		if (i<size&&str[i] >= '0'&&str[i] <= '9')       //�ж��Ƿ������֣������Ƿ񳬷�Χ
		{
			string temp;
			while (i < size&&str[i] >= '0'&&str[i] <= '9')    //��ȡ������ĸ
			{
				temp.push_back(str[i]);
				i++;
			}
			if (i < size && flag==0 && str[i] != ',')      //������ַ��޷�ת������   
			{
				cout << "Invalid number format." << endl;
				return false;
			}
			int num = atoi(temp.c_str());          //���ӷ���ת������������
			if (flag == 0&&(num < 0 || num >= n))
			{
				cout << "Number out of range." << endl;
				return false;
			}
			else if (flag == 1&&(num < 0 || num >= m))
			{
				cout << "Number out of range." << endl;
				return false;
			}
		}
		else if (str[i] == ','&&flag==0)       
		{
			i++;
			flag = 1;
		}
		else if (i < size&&str[i] == ' ' && (str[i + 1] >= '0'&&str[i + 1] <= '9'))    //�ж���������֮���һ���ո�
		{
			i++;
			flag = 0;    //һ�������ж���ϣ���falg��0
		}
		else if (i < size&&str[i] == ';' && (str[i + 1] >= '0'&&str[i + 1] <= '9'))   //�ж�һ�������ķֺ�
		{
			i++;
			flag = 0;    //һ�������ж���ϣ���falg��0
		}
		else
		{
			cout << "Invalid number format." << endl;
			return false;
		}
	}
	return true;     
}
bool JudgeN_M(string n_m, int* n, int* m)     //�ж�������к����Ƿ���Ч
{
	int size = n_m.size();      //��ȡ�ַ�������
	if (size == 0)         //�ַ���Ϊ��
	{
		cout << "Incorrect command format." << endl;
		return false;
	}
	int i = 0;         //��������
	int flag = 0;      //��ǵڼ������֣�0�ǵ�һ��
	while (i < size)
	{
		if (i == 0)
		{
			while (i < size&&n_m[i] == ' ')      //�����ײ��Ŀո�
				i++;
			if (i < size&&(n_m[i]<'0'||n_m[i]>'9'))    //�����ʽ����ȷ
			{
				cout << "Incorrect command format." << endl;
				return false;
			}
		}

		if (i<size&&n_m[i] >= '0'&&n_m[i] <= '9')     //��ȡ�����ַ�
		{
			string cur;               //����һ���ַ��������������ַ�
			while (i < size&&n_m[i] >= '0'&&n_m[i] <= '9')   //����һ�����ַ������
			{
				cur.push_back(n_m[i]);
				i++;
			}
			if (i < size&&n_m[i] != ' ')         //�ж���һ���ַ��Ƿ�ʱ�ո������ǣ���Ч������
			{
				cout << "Invalid number format." << endl;
					return false;
			}
			int num = atoi(cur.c_str());         //���ַ���ת��������
			if (num == 0)
			{
				cout << "Invalid number format." << endl;
				return false;
			}
			if (flag == 0)     //��ʾ��ǰ����Ϊ��һ������
			{
				*n = num;
				flag = 1;
			}
			else                  //Ϊ�ڶ�������
				*m = num;
		}
		else if (i<size&&n_m[i] == ' '&&n_m[i+1]!=' ')     //�����������еĿո���ֻ��һ���ո�
		{
			i++;
		}
		else                  //�����ʽ����ȷ
		{
			cout << "Incorrect command format." << endl;
			return false;
		}
	}
	return true;
}
int main()
{
	int n=0, m=0;
	string n_m;    //���ַ�����ʽ����n��m
	while (1)     //��·��������
	{
		getline(cin, n_m);     //��ȡ��һ���ַ�
		if (JudgeN_M(n_m, &n, &m) == false)       //�жϵ�һ�е������Ƿ���Ч
			continue;
		fflush(stdin);             //ˢ�����뻺����
		string str;
		getline(cin, str);          //��ȡ�ڶ��е������Ƿ���Ч
		if (JudgeInput(str, n, m) == false)        //�жϵڶ��е������Ƿ���Ч
			continue;
		Maze M(n, m, str);            //�����Թ�����
		M.Print();                    //���ó�Ա������ӡ��Ⱦ����
	}
	
	system("pause");
	return 0;
}