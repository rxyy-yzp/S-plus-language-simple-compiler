#include"head.h"
#include"main.cpp"

extern vector<quaternion> qua;
vector<pair<string, int>> v;//ִ�б�����

void interpretation()
{
	for (vector<quaternion>::iterator it = qua.begin(); it != qua.end(); it++)
	{
		if (it->op == "STA" || it->op == "FIN")
		{
			continue;
		}
		else if (it->op == "ann")
		{

		}
	}
}