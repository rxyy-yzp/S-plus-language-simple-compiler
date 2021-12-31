#include"head.h"

string keyword[8] = { "int","if","then","else","while","do","write","for"};//����ؼ���

unordered_map<string, int> symbolTable;//������𼰷��Ź����

//vector<pair<string, string>> lexAyTable;//�����//�ĳɽṹ��
vector<pair<string, string>> varTable;//���������

int label = 100;//��Ԫʽ���к�

//������Ԫʽ��ṹ��
struct quaternion
{
	int qualabel;  //��Ԫʽ�к�
	string op;     //������
	string src1;   //������1
	string src2;   //������2
	string result; //�������
};
vector<quaternion> qua;

//�ʷ������������ýṹ��
struct laTable
{
	string wordType;  //���ʵ�����
	string wordValue; //���ʵ�ֵ
	string wordLine;  //���ʳ��ֵ�����:���ڱ���
};
vector<laTable> lexAyTable;

void Init_Symboltable();
//void printVec(vector<pair<string, string>>& lexAyTable);
//void PrintVectxt(vector<pair<string, string>>& lexAyTable);
void printVec(vector<laTable>& lexAyTable);
void printVectxt(vector<laTable>& lexAyTable);
void printqua(vector<quaternion>& qua);
void printquatxt(vector<quaternion>& qua);
string str(int i);
int isLetter(char letter);
int isDigit(char digit);
int isID(string id);
int isNum(string num);
int isKeyword(string kw);
int findToken(string key);
void lexical(string text, int lineNum);
void lexAnalysis(const char* filename);
string getsym();
int findVar(string var);
int findVarValue(string var);
void program();//P
void declare();//D
void body();//S
void interval();//L
void semicolon();//L_
string bool_or();//B
string bool_and();//T_
string comparison();//F_
string add_sub();//E
string mul_div();//T
string expression();//F
int gramAnalysis();
int findkey(string key);
void interpretation();

/***************************************  �ʷ�����  ***********************************/
/*
�ʷ������������ͣ�
1.���벻�Ϸ��ַ��򵥴ʣ����ʷ��ű�δ���嵥�ʣ�
2.��ʶ��ID��������������ֿ�ͷ��
*/
int lexFlag = 0;//����ʷ�������ȷflag=0������=1
string lexWrongMes;//���ڴ�Ŵʷ������������Ϣ
//��ʼ�������ķ����ű�
void Init_Symboltable()
{
	symbolTable.insert(pair<string, int>("int", 1));
	symbolTable.insert(pair<string, int>("if", 2));
	symbolTable.insert(pair<string, int>("then", 3));
	symbolTable.insert(pair<string, int>("else", 4));
	symbolTable.insert(pair<string, int>("while", 5));
	symbolTable.insert(pair<string, int>("do", 6));
	symbolTable.insert(pair<string, int>("write", 7));
	symbolTable.insert(pair<string, int>("for", 8));
	symbolTable.insert(pair<string, int>("ID", 9));
	symbolTable.insert(pair<string, int>("NUM", 10));
	symbolTable.insert(pair<string, int>("+", 11));
	symbolTable.insert(pair<string, int>("-", 12));
	symbolTable.insert(pair<string, int>("*", 13));
	symbolTable.insert(pair<string, int>("/", 14));
	symbolTable.insert(pair<string, int>("&", 15));
	symbolTable.insert(pair<string, int>("|", 16));
	symbolTable.insert(pair<string, int>("<", 17));
	symbolTable.insert(pair<string, int>(">", 17));
	symbolTable.insert(pair<string, int>("<=", 17));
	symbolTable.insert(pair<string, int>(">=", 17));
	symbolTable.insert(pair<string, int>("!=", 17));
	symbolTable.insert(pair<string, int>("==", 17));
	symbolTable.insert(pair<string, int>("=", 18));
	symbolTable.insert(pair<string, int>("+=", 19));
	symbolTable.insert(pair<string, int>("-=", 20));
	symbolTable.insert(pair<string, int>("*=", 21));
	symbolTable.insert(pair<string, int>("/=", 22));
	symbolTable.insert(pair<string, int>(";", 23));
	symbolTable.insert(pair<string, int>("{", 24));
	symbolTable.insert(pair<string, int>("}", 25));
	symbolTable.insert(pair<string, int>("(", 26));
	symbolTable.insert(pair<string, int>(")", 27));
	symbolTable.insert(pair<string, int>("#", 0));
}

void printVec(vector<laTable>& lexAyTable)
{
	cout << "�������" << "            ����ֵ" << "              �к�" << endl;
	for (vector<laTable>::iterator it = lexAyTable.begin(); it != lexAyTable.end(); it++)
	{
		cout << setiosflags(ios::left) << setw(20) << it->wordType
			<< setiosflags(ios::left) << setw(20) << it->wordValue
			<< setiosflags(ios::left) << setw(20) << it->wordLine << endl;
	}
	cout << endl;
	if (lexFlag == 0)
	{
		cout << "�ʷ����������ȷ!" << endl;
	}
	else if (lexFlag == 1)
	{
		cout << "�ʷ������������!" << endl;
		cout << endl;
		cout << lexWrongMes << endl;
	}
	cout << endl;
}

void printVectxt(vector<laTable>& lexAyTable)
{
	char filename[] = "lexAnalysis.txt"; // �˴�д���ļ���
	ofstream fout(filename);
	fout << "�������" << "  ����ֵ" << "    �к�" << endl;
	for (vector<laTable>::iterator it = lexAyTable.begin(); it != lexAyTable.end(); it++)
	{
		fout << setiosflags(ios::left) << setw(10) << it->wordType
			<< setiosflags(ios::left) << setw(10) << it->wordValue
			<< setiosflags(ios::left) << setw(10) << it->wordLine << endl;
	}
	fout << endl;
	if (lexFlag == 0)
	{
		fout << "�ʷ����������ȷ!" << endl;
	}
	else if (lexFlag == 1)
	{
		fout << "�ʷ������������!" << endl;
		fout << endl;
		fout << lexWrongMes << endl;
	}
}

void printlexErrorVectxt(vector<laTable>& lexAyTable)
{
	char filename[] = "lexError.txt"; // �˴�д���ļ���
	ofstream fout(filename);
	fout << "�������" << "  ����ֵ" << "    �к�" << endl;
	for (vector<laTable>::iterator it = lexAyTable.begin(); it != lexAyTable.end(); it++)
	{
		fout << setiosflags(ios::left) << setw(10) << it->wordType
			<< setiosflags(ios::left) << setw(10) << it->wordValue
			<< setiosflags(ios::left) << setw(10) << it->wordLine << endl;
	}
	fout << endl;
	if (lexFlag == 0)
	{
		fout << "�ʷ����������ȷ!" << endl;
	}
	else if (lexFlag == 1)
	{
		fout << "�ʷ������������!" << endl;
		fout << endl;
		fout << lexWrongMes << endl;
	}
}

//����ת��Ϊ�ַ���
string str(int i)
{
	string res;
	stringstream ss;               //������ss
	ss << i;                      //������aת������ss
	ss >> res;                    //����ssת�����ַ���
	return res;
}

//�ַ���ת����
int str_to_int(string res)
{
	int a;
	stringstream ss;
	ss << res;
	ss >> a;
	return a;
}

//�ж��Ƿ��ǵ�����ĸ
int isLetter(char letter)
{
	if (letter >= 'a' && letter <= 'z' || letter >= 'A' && letter <= 'Z')
	{
		return 1;
	}
	return 0;
}

//�ж��Ƿ��ǵ�������
int isDigit(char digit)
{
	if (digit >= '0' && digit <= '9')
	{
		return 1;
	}
	return 0;
}

//�ж��Ƿ��Ǳ�����
int isKeyword(string kw)
{
	for (int i = 0;i < 8;i++)
	{
		if (keyword[i] == kw)
			return 1; //�ڱ������������ҵ�
	}	
	return 0; //�ڱ�����������δ�ҵ�
}

//�ж��Ƿ�Ϊ��ʶ�� ��<��ĸ>| <��ĸ>��<��ĸ>| <�����ַ�>)*
int isID(string id)
{
	if (isLetter(id[0]))
	{
		for (int i = 1;i < id.length();i++)
		{
			if (!isLetter(id[i]) && !isDigit(id[i]))
			{
				return 0;
			}
		}
		return 1;
	}
	return 0;
}

//�ж��Ƿ�Ϊʮ�������� ��0|(1|2|3|4|5|6|7|8|9)(0|1|2|3|4|5|6|7|8|9)*
int isNum(string num)
{
	if (isDigit(num[0]) && num[0] == '0' && num.length() != 1)
	{
		return 0;
	}
	if (isDigit(num[0]))
	{
		for (int i = 1;i < num.length();i++)
		{
			if (!isDigit(num[i]))
			{
				return 0;
			}
		}
		return 1;
	}
	return 0;
}

//���ñ�:�������Ƿ��ҵ�������
int findToken(string key)
{
	unordered_map<string, int>::iterator it = symbolTable.find(key);
	if (it == symbolTable.end())//����ʧ��
	{
		return -1;
	}
	else
	{
		return it->second;
	}
}

//�������ʴʷ���������
void lexical(string text, int lineNum)
{
	int token;
	string mnemonic;//���Ƿ�
	if (isKeyword(text)) //������
	{
		token = findToken(text);
		mnemonic = text;
		laTable templa;
		templa.wordLine = str(lineNum);
		templa.wordType = mnemonic;
		templa.wordValue = text;
		lexAyTable.push_back(laTable(templa));
	}
	else if (isID(text)) 
	{
		token = findToken("ID");
		mnemonic = "ID";
		laTable templa;
		templa.wordLine = str(lineNum);
		templa.wordType = mnemonic;
		templa.wordValue = text;
		lexAyTable.push_back(laTable(templa));
	}
	else if (isNum(text)) 
	{
		token = findToken("NUM");
		mnemonic = "NUM";
		laTable templa;
		templa.wordLine = str(lineNum);
		templa.wordType = mnemonic;
		templa.wordValue = text;
		lexAyTable.push_back(laTable(templa));
	}
	else if (findToken(text) == -1) 
	{//����޷�ֱ���ҵ�����϶�����+��- ��* ��/ ��> ��< ��>= ��<= ��== ����=��=��(){}��;���������
		string info = "��";
		//lexAyTable.push_back(pair<string, string>("ERROR", text));
		laTable templa;
		templa.wordLine = str(lineNum);
		templa.wordType = "ERROR!";
		templa.wordValue = text;
		lexAyTable.push_back(laTable(templa));
		lexFlag = 1;
		info += str(lineNum);
		info += "�д�����ִ���!";
		lexWrongMes += (info + "\n");
	}
	else //����Ϸ�����
	{
		token = findToken(text);
		if (token == 17)
		{
			mnemonic = "relop";
			laTable templa;
			templa.wordLine = str(lineNum);
			templa.wordType = mnemonic;
			templa.wordValue = text;
			lexAyTable.push_back(laTable(templa));
		}
		else
		{
			mnemonic = text;
			laTable templa;
			templa.wordLine = str(lineNum);
			templa.wordType = mnemonic;
			templa.wordValue = text;
			lexAyTable.push_back(laTable(templa));
		}
	}
}

//�����Զ�������
void lexAnalysis(const char* filename)
{
	int lineNum = 0;
	fstream file;
	string currentLine;
	string tuple;
	string result = "";
	file.open(filename, ios_base::in | ios_base::out);
	if (!file.is_open()) {
		cout << "�޷����ļ�!" << endl;
	}
	else
	{
		while (getline(file, currentLine))
		{
			lineNum++;
			string temp = "";
			string next = "";
			string str = "";
			for (int i = 0; i < currentLine.length(); i++)
			{
				if (currentLine[i] == ' ') //����ǿո񣬼�������һ��
					continue;
				else
				{
					temp += currentLine[i];
					next += currentLine[i + 1];
					if (isLetter(currentLine[i]))// ��ǰɨ��Ϊ��ĸ
					{   //�����һ���Ƿ��Ż�հ׻��н�β���򽫸���ĸ���
						if (findToken(next) >= 9 && findToken(next) <= 27 ||
							currentLine[i + 1] == '\0' || currentLine[i + 1] == ' ' ||
							currentLine[i + 1] == '!')
						{
							lexical(temp, lineNum);
							temp = "";
							next = "";
						}
						else // �������ƥ��֮���������
							next = "";
					}
					else if (isDigit(currentLine[i]))// ��ǰɨ��Ϊ����
					{   //�����һ���Ƿ��Ż�հף�ֱ�����
						if (findToken(next) >= 9 && findToken(next) <= 27 ||
							currentLine[i + 1] == '\0' || currentLine[i + 1] == ' ')
						{
							lexical(temp, lineNum);
							temp = "";
							next = "";
						}
						else // �������ƥ��֮���������
							next = "";
					}
					else if (findToken(temp) >= 9 && findToken(temp) <= 27)//����Ƿ���
					{   //�������Ϸ���
						if ((currentLine[i] == '<' && currentLine[i + 1] == '=') || 
							(currentLine[i] == '>' && currentLine[i + 1] == '=') ||
							(currentLine[i] == '=' && currentLine[i + 1] == '=') ||
							(currentLine[i] == '!' && currentLine[i + 1] == '=') ||
							(currentLine[i] == '+' && currentLine[i + 1] == '=') ||
							(currentLine[i] == '-' && currentLine[i + 1] == '=') ||
							(currentLine[i] == '*' && currentLine[i + 1] == '=') ||
							(currentLine[i] == '/' && currentLine[i + 1] == '='))
						{
							temp += next;
							lexical(temp, lineNum);
							temp = "";
							next = "";
							i++;//currentLine����2���ַ�
						}
						else //����ǵ�������
						{
							lexical(temp, lineNum);
							temp = "";
							next = "";
						}
					}
					else//��ǰɨ��Ϊδ֪����
					{   //�����һ���Ƿ��Ż�հף�ֱ�����
						if (currentLine[i + 1] == '\0' || currentLine[i + 1] == ' ')
						{
							lexical(temp, lineNum);
							temp = "";
							next = "";
						}
						else
						{
							// �������ƥ��֮���������
							next = "";
						}
					}
				}
			}
		}
	}
	if (lexFlag == 0)
	{
		printVec(lexAyTable);//����̨��ӡ���
		printVectxt(lexAyTable);//���������ļ�
	}
	else
	{
		printVec(lexAyTable);//����̨��ӡ���
		printlexErrorVectxt(lexAyTable);//���������ļ�
	}
}

/*****************************  �﷨�����������������Ԫʽ����  ***************************/
/*
�﷨��������������������ͣ�
1.���ź��治��Ϊ0
2.�������ķ�ʹ�ó���while��for��if������ʹ�õȣ�
3.��ֵ���㲻�Ϸ�
4.����δ����ֱ��ʹ��
5.����δ��ֱֵ������
*/
/*
��Ԫʽ��ʽ���壺
[��������op�� ,������1��src1�� ,������2��src2�� ,���������result�� ]
*/
//����̨��ӡ��Ԫʽ
void printqua(vector<quaternion>& qua)
{
	cout << "�����Ԫʽ:" << endl;
	for (vector<quaternion>::iterator it = qua.begin(); it != qua.end(); it++)
	{
		cout << setiosflags(ios::left) << setw(4) << it->qualabel
			<< "[" << setiosflags(ios::left) << setw(6) << it->op
			<< "," << setiosflags(ios::left) << setw(6) << it->src1
			<< "," << setiosflags(ios::left) << setw(6) << it->src2
			<< "," << setiosflags(ios::left) << setw(6) << it->result << "]" << endl;
	}
	cout << endl;
}

//��ӡ��Ԫʽ���ļ�
void printquatxt(vector<quaternion>& qua)
{
	char filename[] = "quaternion.txt"; // �˴�д���ļ���
	ofstream fout(filename);
	fout << "�����Ԫʽ:" << endl;
	for (vector<quaternion>::iterator it = qua.begin(); it != qua.end(); it++)
	{
		fout << setiosflags(ios::left) << setw(4) << it->qualabel
			<< "[" << setiosflags(ios::left) << setw(6) << it->op
			<< "," << setiosflags(ios::left) << setw(6) << it->src1
			<< "," << setiosflags(ios::left) << setw(6) << it->src2
			<< "," << setiosflags(ios::left) << setw(6) << it->result << "]" << endl;
	}
	fout << endl;
}

int vecnum = 0;//����vector������ָ��λ�ã�ȫ�ֱ�����
string sym;
string getsym()
{
	sym = lexAyTable.at(vecnum).wordType;
	vecnum++;
	return sym;
}

int tempnum = 1;
string newtemp()//������ʱ����
{
	string nt;
	nt = "t" + str(tempnum);
	tempnum++;
	return nt;
}

//Ѱ�ұ��������Ƿ���ڸñ���
int findVar(string var)
{
	for (vector<pair<string, string>>::iterator it = varTable.begin(); it != varTable.end(); it++)
	{
		if (it->first == var)
		{
			return -1;//�ҵ�
		}
	}
	return 1;//δ�ҵ�
}

//Ѱ�ұ������иñ����Ƿ�ֵ
int findVarValue(string var)
{
	for (vector<pair<string, string>>::iterator it = varTable.begin(); it != varTable.end(); it++)
	{
		if (it->first == var)
		{
			if (it->second == "NV")
			{
				return -1;//�ñ���δ��ֵ
			}
		}
		
	}
	return 1;//�ñ����Ѹ�ֵ
}

void program()
{
	cout << "ִ��program()����" << endl;
	if (sym == "{")
	{
		sym = getsym();
		declare();
		body();
		if (sym!="}") 
		{ 
			cout << "��" << lexAyTable.at(vecnum - 1).wordLine << "�д���! ȱ�ٷ��� }" << endl;
			exit(0); 
		}
		else
			sym = getsym();
	}
	else
	{
		cout << "��" << lexAyTable.at(vecnum - 1).wordLine << "�д���!";
		cout << "�Ƿ�����: " << sym << endl;
		exit(0);
	}
}

void declare()
{
	cout << "ִ��declare()����" << endl;
	if (sym == "int")
	{
		sym = getsym();
		if (sym == "ID")
		{
			string var = lexAyTable.at(vecnum - 1).wordValue;
			varTable.push_back(pair<string,string>(var,"NV"));//���������
			quaternion tempqua;//�������������Ԫʽ
			tempqua.qualabel = label;
			label++;
			tempqua.op = "ann";
			tempqua.src1 = "int";
			tempqua.src2 = "_";
			tempqua.result = var;
			qua.push_back(quaternion(tempqua));
			sym = getsym();
			if (sym == ";")
			{
				sym = getsym();
				while (sym == "int")
				{
					sym = getsym();
					if (sym == "ID")
					{
						string var = lexAyTable.at(vecnum - 1).wordValue;
						//����Ƿ��ض���
						if (findVar(var) == -1)
						{
							cout << "���ڱ���" << var << "�ض���" << endl;
							exit(0);
						}
						varTable.push_back(pair<string, string>(var, "NV"));//NV����δ��ֵ״̬
						quaternion tempqua;
						tempqua.qualabel = label;
						label++;
						tempqua.op = "ann";
						tempqua.src1 = "int";
						tempqua.src2 = "_";
						tempqua.result = var;
						qua.push_back(quaternion(tempqua));

						sym = getsym();
						if (sym == ";")
							sym = getsym();
						else
						{
							cout << "��" << lexAyTable.at(vecnum - 1).wordLine << "�д���! ȱ�ٷ��� ;" << endl;
							exit(0);
						}
					}
				}
			}
			else
			{
				cout << "��" << lexAyTable.at(vecnum - 1).wordLine << "�д���! ȱ�ٷ��� ;" << endl;
				exit(0);
			}
		}
		else
		{
			cout << "��" << lexAyTable.at(vecnum - 1).wordLine << "�д���! ȱ�ٱ��� ID" << endl;
			exit(0);
		}
	}
	else
	{
		cout << "��" << lexAyTable.at(vecnum - 1).wordLine << "�д���!";
		cout << "�Ƿ�����: " << sym << endl;
		exit(0);
	}
}

void body()
{
	cout << "ִ��body()����" << endl;
	int whiletruelabel;
	int whilefalselabel;
	string tempq;
	if (sym =="while")
	{
		sym = getsym();
		if (sym == "(")
		{
			sym = getsym();
			int whilelabel = label;
			tempq = bool_or();
			//��jnzѹ��
			quaternion tempqua;
			tempqua.qualabel = label;
			whiletruelabel = label;
			label++;
			tempqua.op = "jnz";
			tempqua.src1 = tempq;
			tempqua.src2 = "_";
			tempqua.result = "";
			qua.push_back(quaternion(tempqua));
			//��jmpѹ��
			tempqua.qualabel = label;
			whilefalselabel = label;
			label++;
			tempqua.op = "jmp";
			tempqua.src1 = "_";
			tempqua.src2 = "_";
			tempqua.result = "";
			qua.push_back(quaternion(tempqua));
			if (sym == ")")
			{
				sym = getsym();
				if (sym == "do")
				{
					sym = getsym();
					//���������
					qua.at(whiletruelabel - 100).result = str(label);
					body();
					tempqua.qualabel = label;
					label++;
					tempqua.op = "jmp";
					tempqua.src1 = "_";
					tempqua.src2 = "_";
					tempqua.result = str(whilelabel);
					qua.push_back(quaternion(tempqua));
					qua.at(whilefalselabel - 100).result = str(label);
				}
				else
				{
					cout << "��" << lexAyTable.at(vecnum - 1).wordLine << "�д���! ȱ�ٹؼ��� do" << endl;
					exit(0);
				}
			}
			else
			{
				cout << "��" << lexAyTable.at(vecnum - 1).wordLine << "�д���! ȱ�ٷ��� )" << endl;
				exit(0);
			}
		}
		else
		{
			cout << "��" << lexAyTable.at(vecnum - 1).wordLine << "�д���! ȱ�ٷ��� (" << endl;
			exit(0);
		}
	}
	else if (sym == "{")
	{
		sym = getsym();
		interval();
		if (sym != "}")
		{
			cout << "��" << lexAyTable.at(vecnum - 1).wordLine << "�д���! ȱ�ٷ��� }" << endl;
			exit(0);
		}
		else
		{
			sym = getsym();
		}
	}
	else if (sym == "ID")
	{
		//�ж�Ҫ��ֵ�ı����Ƿ���
		string var = lexAyTable.at(vecnum - 1).wordValue;
		if (findVar(var) == 1)//δ�ҵ�
		{
			cout << "����" << var << "δ�����ʹ��!" << endl;
			exit(0);
		}

		sym = getsym();
		if (sym == "=")
		{
			sym = getsym();
			string tempx;
			tempx = add_sub();
			quaternion tempqua;
			tempqua.qualabel = label;
			label++;
			tempqua.op = "=";
			tempqua.src1 = tempx;
			tempqua.src2 = "_";
			tempqua.result = var;
			qua.push_back(quaternion(tempqua));
			for (vector<pair<string, string>>::iterator it = varTable.begin(); it != varTable.end(); it++)
			{
				if (it->first == var)
				{
					it->second = tempx;//�ҵ�
				}
			}
		}
		else if (sym == "+=")
		{
			string var = lexAyTable.at(vecnum - 2).wordValue;
			if (findVarValue(var) == -1)//δ��ֵ
			{
				cout << "����" << var << "δ��ֵ��ʹ��!" << endl;
				exit(0);
			}
			sym = getsym();
			string tempx;
			tempx = add_sub();
			
			quaternion tempqua;
			tempqua.qualabel = label;
			label++;
			string t;
			t = newtemp();
			tempqua.op = "+";
			tempqua.src1 = var;
			tempqua.src2 = tempx;
			tempqua.result = t;
			qua.push_back(quaternion(tempqua));
			tempqua.qualabel = label;
			label++;
			tempqua.op = "=";
			tempqua.src1 = t;
			tempqua.src2 = "_";
			tempqua.result = var;
			qua.push_back(quaternion(tempqua));
			for (vector<pair<string, string>>::iterator it = varTable.begin(); it != varTable.end(); it++)
			{
				if (it->first == var)
				{
					it->second = tempx;//�ҵ�
				}
			}

		}
		else if (sym == "-=")
		{
			string var = lexAyTable.at(vecnum - 2).wordValue;
			if (findVarValue(var) == -1)//δ��ֵ
			{
				cout << "����" << var << "δ��ֵ��ʹ��!" << endl;
				exit(0);
			}
			sym = getsym();
			string tempx;
			tempx = add_sub();
			quaternion tempqua;
			tempqua.qualabel = label;
			label++;
			string t;
			t = newtemp();
			tempqua.op = "-";
			tempqua.src1 = var;
			tempqua.src2 = tempx;
			tempqua.result = t;
			qua.push_back(quaternion(tempqua));
			tempqua.qualabel = label;
			label++;
			tempqua.op = "=";
			tempqua.src1 = t;
			tempqua.src2 = "_";
			tempqua.result = var;
			qua.push_back(quaternion(tempqua));
			for (vector<pair<string, string>>::iterator it = varTable.begin(); it != varTable.end(); it++)
			{
				if (it->first == var)
				{
					it->second = tempx;//�ҵ�
				}
			}
		}
		else if (sym == "*=")
		{
			string var = lexAyTable.at(vecnum - 2).wordValue;
			if (findVarValue(var) == -1)//δ��ֵ
			{
				cout << "����" << var << "δ��ֵ��ʹ��!" << endl;
				exit(0);
			}
			sym = getsym();
			string tempx;
			tempx = add_sub();
			quaternion tempqua;
			tempqua.qualabel = label;
			label++;
			string t;
			t = newtemp();
			tempqua.op = "*";
			tempqua.src1 = var;
			tempqua.src2 = tempx;
			tempqua.result = t;
			qua.push_back(quaternion(tempqua));
			tempqua.qualabel = label;
			label++;
			tempqua.op = "=";
			tempqua.src1 = t;
			tempqua.src2 = "_";
			tempqua.result = var;
			qua.push_back(quaternion(tempqua));
			for (vector<pair<string, string>>::iterator it = varTable.begin(); it != varTable.end(); it++)
			{
				if (it->first == var)
				{
					it->second = tempx;//�ҵ�
				}
			}
		}
		else if (sym == "/=")
		{
			string var = lexAyTable.at(vecnum - 2).wordValue;
			if (findVarValue(var) == -1)//δ��ֵ
			{
				cout << "����" << var << "δ��ֵ��ʹ��!" << endl;
				exit(0);
			}
			sym = getsym();
			string tempx;
			tempx = add_sub();
			quaternion tempqua;
			tempqua.qualabel = label;
			label++;
			string t;
			t = newtemp();
			tempqua.op = "/";
			tempqua.src1 = var;
			tempqua.src2 = tempx;
			tempqua.result = t;
			qua.push_back(quaternion(tempqua));
			tempqua.qualabel = label;
			label++;
			tempqua.op = "=";
			tempqua.src1 = t;
			tempqua.src2 = "_";
			tempqua.result = var;
			qua.push_back(quaternion(tempqua));
			for (vector<pair<string, string>>::iterator it = varTable.begin(); it != varTable.end(); it++)
			{
				if (it->first == var)
				{
						it->second = tempx;//�ҵ�
				}				
			}
		}
		else
		{
			cout << "��" << lexAyTable.at(vecnum - 1).wordLine << 
				"�д���! ȱ�ٷ��� = �� += �� -= �� *= �� /=" << endl;
			exit(0);
		}
	}
	else if (sym == "if")
	{
		string tempq;
		sym = getsym();
		if (sym == "(")
		{
			sym = getsym();
			tempq = bool_or();
			quaternion tempqua;
			tempqua.qualabel = label;
			int truelabel = label;
			label++;
			tempqua.op = "jnz";
			tempqua.src1 = tempq;
			tempqua.src2 = "_";
			tempqua.result = "";
			qua.push_back(quaternion(tempqua));//��jnzѹ��
			tempqua.qualabel = label;
			int falselabel = label;
			label++;
			tempqua.op = "jmp";
			tempqua.src1 = "_";
			tempqua.src2 = "_";
			tempqua.result = "";
			qua.push_back(quaternion(tempqua));//��jmpѹ��	
			if (sym == ")")
			{
				sym = getsym();
				if (sym == "then")
				{
					sym = getsym();					
					qua.at(truelabel - 100).result = str(label);//���������
					body();
					if (sym != "else")
					{
						qua.at(falselabel - 100).result = str(label);
					}
					if (sym == "else")
					{
						tempqua.qualabel = label;
						truelabel = label;
						label++;
						tempqua.op = "jmp";
						tempqua.src1 = "_";
						tempqua.src2 = "_";
						tempqua.result = "";
						qua.push_back(quaternion(tempqua));
						qua.at(falselabel - 100).result = str(label);
						sym = getsym();
						body();
						qua.at(truelabel - 100).result = str(label);
					}
				}
				else
				{
					cout << "��" << lexAyTable.at(vecnum - 1).wordLine << "�д���! ȱ�ٹؼ��� then" << endl;
					exit(0);
				}
			}
			else
			{
				cout << "��" << lexAyTable.at(vecnum - 1).wordLine << "�д���! ȱ�ٷ��� )" << endl;
				exit(0);
			}
		}
		else
		{
			cout << "��" << lexAyTable.at(vecnum - 1).wordLine << "�д���! ȱ�ٷ��� (" << endl;
			exit(0);
		}
	}
	else if (sym == "write")
	{
		sym = getsym();
		if (sym == "ID")
		{
			string outstring = lexAyTable.at(vecnum - 1).wordValue;
			if (findVar(outstring) == 1)//δ�ҵ�
			{
				cout << "����" << outstring << "δ�����ʹ��!" << endl;
				exit(0);
			}
			if (findVarValue(outstring) == -1)//δ��ֵ
			{
				cout << "����" << outstring << "δ��ֵ��ʹ��!" << endl;
				exit(0);
			}
			sym = getsym();
			quaternion tempqua;
			tempqua.qualabel = label;
			label++;
			tempqua.op = "out";
			tempqua.src1 = outstring;
			tempqua.src2 = "_";
			tempqua.result = "_";
			qua.push_back(quaternion(tempqua));
		}
		else if (sym == "NUM")
		{
			string outstring = lexAyTable.at(vecnum - 1).wordValue;
			sym = getsym();
			quaternion tempqua;
			tempqua.qualabel = label;
			label++;
			tempqua.op = "out";
			tempqua.src1 = outstring;
			tempqua.src2 = "_";
			tempqua.result = "_";
			qua.push_back(quaternion(tempqua));
		}
		else
		{
			cout << "��" << lexAyTable.at(vecnum - 1).wordLine << "�д���! ȱ�� ID �� NUM" << endl;
			exit(0);
		}
	}
	else if (sym == "for")
	{
		sym = getsym();
		if (sym == "(")
		{
			sym = getsym();
			if (sym == "ID")
			{
				//�ж�Ҫ��ֵ�ı����Ƿ���
				string var = lexAyTable.at(vecnum - 1).wordValue;
				if (findVar(var) == 1)//δ�ҵ�
				{
					cout << "����" << var << "δ�����ʹ��!" << endl;
					exit(0);
				}
				sym = getsym();
				if (sym == "=")
				{
					sym = getsym();
					string tempx;
					tempx = add_sub();
					quaternion tempqua;
					tempqua.qualabel = label;
					label++;
					tempqua.op = "=";
					tempqua.src1 = tempx;
					tempqua.src2 = "_";
					tempqua.result = var;
					qua.push_back(quaternion(tempqua));
					for (vector<pair<string, string>>::iterator it = varTable.begin(); it != varTable.end(); it++)
					{
						if (it->first == var)
						{
							it->second = tempx;//�ҵ�
						}
					}
					if (sym == ";")
					{
						int fortruelabel;
						int forfalselabel;
						sym = getsym();
						int forlabel = label;
						tempq = bool_or();
						//��jnzѹ��
						quaternion tempqua;
						tempqua.qualabel = label;
						fortruelabel = label;//��¼jnz��Ԫʽ������
						label++;
						tempqua.op = "jnz";
						tempqua.src1 = tempq;
						tempqua.src2 = "_";
						tempqua.result = "";
						qua.push_back(quaternion(tempqua));
						//��jmpѹ��
						tempqua.qualabel = label;
						forfalselabel = label;//��¼jmp��Ԫʽ������
						label++;
						tempqua.op = "jmp";
						tempqua.src1 = "_";
						tempqua.src2 = "_";
						tempqua.result = "";
						qua.push_back(quaternion(tempqua));
						if (sym == ";")
						{
							sym = getsym();
							if (sym == "ID")
							{
								//�ж�Ҫ��ֵ�ı����Ƿ���
								string var = lexAyTable.at(vecnum - 1).wordValue;
								if (findVar(var) == 1)//δ�ҵ�
								{
									cout << "����" << var << "δ�����ʹ��!" << endl;
									exit(0);
								}
								sym = getsym();
								if (sym == "=")
								{
									sym = getsym();
									int pluslabel = label;
									string tempx;
									tempx = add_sub();
									quaternion tempqua;
									tempqua.qualabel = label;
									label++;
									tempqua.op = "=";
									tempqua.src1 = tempx;
									tempqua.src2 = "_";
									tempqua.result = var;
									qua.push_back(quaternion(tempqua));
									for (vector<pair<string, string>>::iterator it = varTable.begin(); it != varTable.end(); it++)
									{
										if (it->first == var)
										{
											it->second = tempx;//�ҵ�
										}
									}
									tempqua.qualabel = label;
									label++;
									tempqua.op = "jmp";
									tempqua.src1 = "_";
									tempqua.src2 = "_";
									tempqua.result = str(forlabel);
									qua.push_back(quaternion(tempqua));
									if (sym == ")")
									{
										sym = getsym();
										if (sym == "do")
										{
											sym = getsym();
											//���������
											qua.at(fortruelabel - 100).result = str(label);
											body();
											quaternion tempqua;
											tempqua.qualabel = label;
											fortruelabel = label;//��¼jnz��Ԫʽ������
											label++;
											tempqua.op = "jmp";
											tempqua.src1 = "_";
											tempqua.src2 = "_";
											tempqua.result = str(pluslabel);
											qua.push_back(quaternion(tempqua));
											qua.at(forfalselabel - 100).result = str(label);
										}
										else
										{
											cout << "��" << lexAyTable.at(vecnum - 1).wordLine << "�д���! ȱ�ٱ����� do" << endl;
											exit(0);
										}
									}
									else
									{
										cout << "��" << lexAyTable.at(vecnum - 1).wordLine << "�д���! ȱ�ٷ��� )" << endl;
										exit(0);
									}
								}
								else
								{
									cout << "��" << lexAyTable.at(vecnum - 1).wordLine << "�д���! ȱ�ٷ��� =" << endl;
									exit(0);
								}
							}
						}
						else
						{
							cout << "��" << lexAyTable.at(vecnum - 1).wordLine << "�д���! ȱ�ٷ��� ;" << endl;
							exit(0);
						}
					}
					else
					{
						cout << "��" << lexAyTable.at(vecnum - 1).wordLine << "�д���! ȱ�ٷ��� ;" << endl;
						exit(0);
					}
				}
			}
		}
	}
	else
	{
		cout << "��" << lexAyTable.at(vecnum - 1).wordLine << "�д���!";
		cout << "�Ƿ�����: " << sym << endl;
		exit(0);
	}
}

void interval()//L��SL_
{
	cout << "ִ��interval()����" << endl;
	if (sym == "if" || sym == "while" || sym == "{" || sym == "ID" 
		|| sym == "write" || sym == "for")// if (sym����first(S))
	{
		body();
		semicolon();
	}
	else
	{
		cout << "��" << lexAyTable.at(vecnum - 1).wordLine << "�д���!";
		cout << "�Ƿ�����: " << sym << endl;
		exit(0);
	}
}

void semicolon()//L_ ��; L |~
{
	cout << "ִ��semicolon()����" << endl;
	if (sym == ";")
	{
		sym = getsym();
		interval();
	}
	else if (sym != "}")    /* if (sym������follow(L_)) */
	{
		cout << "��" << lexAyTable.at(vecnum - 1).wordLine << "�д���!";
		cout << "�Ƿ�����: " << sym << endl;
		exit(0);
	}
}

string bool_or()//B��T_
{
	cout << "ִ��bool_or()����" << endl;
	if (sym == "ID" || sym == "NUM")// if (sym����first(T_))
	{
		string tempq;
		tempq = bool_and();
		while (sym == "|")
		{
			sym = getsym();
			string tempr;
			tempr = bool_and();
			string t;
			t = newtemp();
			quaternion tempqua;
			tempqua.qualabel = label;
			label++;
			tempqua.op = "|";
			tempqua.src1 = tempq;
			tempqua.src2 = tempr;
			tempqua.result = t;
			qua.push_back(quaternion(tempqua));
			tempq = t;
		}
		return tempq;
	}
	else
	{
		cout << "��" << lexAyTable.at(vecnum - 1).wordLine << "�д���!";
		cout << "�Ƿ�����: " << sym << endl;
		exit(0);
	}
}

string bool_and()//T_��F_
{
	cout << "ִ��bool_and()����" << endl;
	if (sym == "ID" || sym == "NUM")// if (sym����first(F_))
	{
		string tempq;
		tempq = comparison();
		while (sym == "&")
		{
			sym = getsym();
			string tempr;
			tempr = comparison();
			string t;
			t = newtemp();
			quaternion tempqua;
			tempqua.qualabel = label;
			label++;
			tempqua.op = "&";
			tempqua.src1 = tempq;
			tempqua.src2 = tempr;
			tempqua.result = t;
			qua.push_back(quaternion(tempqua));
			tempq = t;
		}
		return tempq;
	}
	else
	{
		cout << "��" << lexAyTable.at(vecnum - 1).wordLine << "�д���!";
		cout << "�Ƿ�����: " << sym << endl;
		exit(0);
	}
}

string comparison()//F_��ID relop ID | ID | NUM | NUM relop ID | ID relop NUM
{
	cout << "ִ��comparison()����" << endl;
	if (sym == "ID")
	{
		string var = lexAyTable.at(vecnum - 1).wordValue;
		if (findVar(var) == 1)//δ�ҵ�
		{
			cout << "����" << var << "δ�����ʹ��!" << endl;
			exit(0);
		}
		if (findVarValue(var) == -1)//δ��ֵ
		{
			cout << "����" << var << "δ��ֵ��ʹ��!" << endl;
			exit(0);
		}
		string tempvar = var;
		sym = getsym();
		if (sym != "relop")
			return var;
		if (sym == "relop")
		{
			string temprop = lexAyTable.at(vecnum - 1).wordValue;
			sym = getsym();
			if (sym == "ID")
			{
				string var = lexAyTable.at(vecnum - 1).wordValue;
				if (findVar(var) == 1)//δ�ҵ�
				{
					cout << "����" << var << "δ�����ʹ��!" << endl;
					exit(0);
				}
				if (findVarValue(var) == -1)//δ��ֵ
				{
					cout << "����" << var << "δ��ֵ��ʹ��!" << endl;
					exit(0);
				}
				string t;
				t = newtemp();
				quaternion tempqua;
				tempqua.qualabel = label;
				label++;
				tempqua.op = temprop;
				tempqua.src1 = tempvar;
				tempqua.src2 = var;
				tempqua.result = t;
				qua.push_back(quaternion(tempqua));
				sym = getsym();
				return t;
			}
			else if (sym == "NUM")
			{
				string tnum = lexAyTable.at(vecnum - 1).wordValue;
				string t;
				t = newtemp();
				quaternion tempqua;
				tempqua.qualabel = label;
				label++;
				tempqua.op = temprop;
				tempqua.src1 = tempvar;
				tempqua.src2 = tnum;
				tempqua.result = t;
				qua.push_back(quaternion(tempqua));
				sym = getsym();
				return t;
			}
			else
			{
				cout << "��" << lexAyTable.at(vecnum - 1).wordLine << "�д���! ȱ�ٱ��� ID �� NUM" << endl;
				exit(0);
			}
		}
	}
	else if (sym == "NUM")
	{
		string relnum = lexAyTable.at(vecnum - 1).wordValue;
		string temprelnum = relnum;
		sym = getsym();
		if (sym != "relop")
		{
			return relnum;
		}
		if (sym == "relop")
		{
			string tempnumrop = lexAyTable.at(vecnum - 1).wordValue;
			sym = getsym();
			if (sym == "ID")
			{
				string numvar = lexAyTable.at(vecnum - 1).wordValue;
				if (findVar(numvar) == 1)//δ�ҵ�
				{
					cout << "����" << numvar << "δ�����ʹ��!" << endl;
					exit(0);
				}
				if (findVarValue(numvar) == -1)//δ��ֵ
				{
					cout << "����" << numvar << "δ��ֵ��ʹ��!" << endl;
					exit(0);
				}
				string t;
				t = newtemp();
				quaternion tempqua;
				tempqua.qualabel = label;
				label++;
				tempqua.op = tempnumrop;
				tempqua.src1 = temprelnum;
				tempqua.src2 = numvar;
				tempqua.result = t;
				qua.push_back(quaternion(tempqua));
				sym = getsym();
				return t;
			}
			else
			{
				cout << "��" << lexAyTable.at(vecnum - 1).wordLine << "�д���! ȱ�ٱ��� ID" << endl;
				exit(0);
			}
		}
	}
	else
	{
		cout << "��" << lexAyTable.at(vecnum - 1).wordLine << "�д���!";
		cout << "�Ƿ�����: " << sym << endl;
		exit(0);
	}  //�Ƿ�����
}

string add_sub()//E��T{+T| -T}
{
	cout << "ִ��add_sub()����" << endl;
	if (sym == "ID" || sym == "NUM" || sym == "(")// if (sym����first(T))
	{
		string tempq;
		tempq = mul_div();
		while (sym == "+" || sym == "-")
		{
			if (sym == "+")
			{
				sym = getsym();
				string tempr;
				tempr = mul_div();
				string t;
				t = newtemp();
				quaternion tempqua;
				tempqua.qualabel = label;
				label++;
				tempqua.op = "+";
				tempqua.src1 = tempq;
				tempqua.src2 = tempr;
				tempqua.result = t;
				qua.push_back(quaternion(tempqua));
				tempq = t;
			}
			else if (sym == "-")
			{
				sym = getsym();
				string tempr;
				tempr = mul_div();
				string t;
				t = newtemp();
				quaternion tempqua;
				tempqua.qualabel = label;
				label++;
				tempqua.op = "-";
				tempqua.src1 = tempq;
				tempqua.src2 = tempr;
				tempqua.result = t;
				qua.push_back(quaternion(tempqua));
				tempq = t;
			}
		}
		return tempq;
	}
	else
	{
		cout << "��" << lexAyTable.at(vecnum - 1).wordLine << "�д���!";
		cout << "�Ƿ�����: " << sym << endl;
		exit(0);
	}
}

string mul_div()//T��F{* F | /F }
{
	cout << "ִ��mul_div()����" << endl;
	if (sym == "ID" || sym == "NUM" || sym == "(")// if (sym����first(F))
	{
		string tempq;
		tempq = expression();
		while (sym == "*" || sym == "/")
		{
			if (sym == "*")
			{
				sym = getsym();
				string tempr;
				tempr = expression();
				string t;
				t = newtemp();
				quaternion tempqua;
				tempqua.qualabel = label;
				label++;
				tempqua.op = "*";
				tempqua.src1 = tempq;
				tempqua.src2 = tempr;
				tempqua.result = t;
				qua.push_back(quaternion(tempqua));
				tempq = t;
			}
			else if (sym == "/")
			{
				sym = getsym();
				string tempr;
				tempr = expression();
				if (tempr == "0")
				{
					cout << "������ĸ����Ϊ0" << endl;
					exit(0);
				}
				string t;
				t = newtemp();
				quaternion tempqua;
				tempqua.qualabel = label;
				label++;
				tempqua.op = "/";
				tempqua.src1 = tempq;
				tempqua.src2 = tempr;
				tempqua.result = t;
				qua.push_back(quaternion(tempqua));
				tempq = t;
			}
		}
		return tempq;
	}
	else
	{
		cout << "��" << lexAyTable.at(vecnum - 1).wordLine << "�д���!";
		cout << "�Ƿ�����: " << sym << endl;
		exit(0);
	}
}

string expression()//F�� (E) | NUM | ID
{
	cout << "ִ��expression()����" << endl;
	if (sym == "(")
	{
		sym = getsym();
		string tempq;
		tempq = add_sub();
		if (sym!=")")
		{
			cout << "��" << lexAyTable.at(vecnum - 1).wordLine << "�д���! ȱ�ٷ��� }" << endl;
			exit(0);
		}
		else
		{
			sym = getsym();
			return tempq;
		}
	}
	else if (sym == "NUM")
	{
		string num = lexAyTable.at(vecnum - 1).wordValue;
		sym = getsym();
		return num;
	}
	else if (sym == "ID")
	{
		string var = lexAyTable.at(vecnum - 1).wordValue;
		if (findVar(var) == 1)//δ�ҵ�
		{
			cout << "����" << var << "δ�����ʹ��!" << endl;
			exit(0);
		}
		if (findVarValue(var) == -1)//δ��ֵ
		{
			cout << "����" << var << "δ��ֵ��ʹ��!" << endl;
			exit(0);
		}
		sym = getsym();
		return var;
	}
	else
	{
		cout << "��" << lexAyTable.at(vecnum - 1).wordLine << "�д���!";
		cout << "�Ƿ�����: " << sym << endl;
		exit(0);
	}
}

int gramAnalysis()
{
	cout << "ִ��gramAnalysis()����" << endl;
	quaternion tempqua;
	tempqua.qualabel = label;
	label++;
	tempqua.op = "STA";
	tempqua.src1 = "_";
	tempqua.src2 = "_";
	tempqua.result = "_";
	qua.push_back(quaternion(tempqua));
	sym = getsym();
	program();
	if (sym == "#")
	{
		quaternion tempqua;
		tempqua.qualabel = label;
		label++;
		tempqua.op = "FIN";
		tempqua.src1 = "_";
		tempqua.src2 = "_";
		tempqua.result = "_";
		qua.push_back(quaternion(tempqua));
		cout << "�﷨���������ȷ!" << endl;
		return 1;
	}
	else
	{
		cout << "main��������!" << endl;
		return -1;
	}
}

unordered_map<string, int> v;//ִ�б�����

int findkey(string key)
{
	unordered_map<string, int>::iterator it = v.find(key);
	if (it == v.end())//����ʧ��
	{
		return -1;
	}
	else
	{
		return it->second;
	}
}

/***************************************  ִ�г���  ***********************************/
/*���õ�����ָ��ѭ������������Ԫʽ������ת���������е�FIN��������н��������б�����ֵ*/
string output;
//ִ�г�����ɱ���ֵ���㣩������ͼ���:ѭ����������ͬ����Ԫʽִ�в�ͬ�Ĳ�����ֱ�����������
void interpretation()
{
	int max = 0;
	//�ҳ�һ�������˶�����ʱ����
	for (vector<quaternion>::iterator it = qua.begin(); it != qua.end(); it++)
	{
		if (it->result[0] == 't')
		{
			string tempera = it->result;
			tempera.erase(0, 1);
			int temp=str_to_int(tempera);
			if (temp > max)
			{
				max = temp;
			}
		}
		
	}

	//��ʱ����ȫ����ʼ��
	for (int i = 0;i < max;i++)
	{
		string intpreword = "t" + str(i + 1);
		int intprevalue = -1;
		v.insert(pair<string, int>(intpreword,intprevalue));
	}
	
	//ִ��
	for (vector<quaternion>::iterator it = qua.begin(); it != qua.end(); )
	{
		/*for (unordered_map<string, int>::iterator itt = v.begin(); itt != v.end();itt++)
		{
			cout << itt->first << " " << itt->second << endl;
		}*/
		if (it->op == "STA")
		{
			it++;
			continue;
		}
		else if (it->op == "FIN")
		{
			for (unordered_map<string, int>::iterator it = v.begin();it != v.end();it++)
			{
				cout << it->first << ":" << it->second << endl;
			}
			char filename[] = "write.txt"; // �˴�д���ļ���
			ofstream fout(filename);
			fout << output << endl;
			cout << "����ִ�����!" << endl;
			exit(0);
		}
		else if (it->op == "ann")
		{
			string intpreword = it->result;
			int intprevalue = -1;
			v.insert(pair<string, int>(intpreword, intprevalue));
			it++;
		}
		else if (it->op == "=")
		{
			int varnum;
			if (it->src1[0] < '0' || it->src1[0] > '9')
			{
				varnum = findkey(it->src1);
			}
			else
			{
				varnum = str_to_int(it->src1);
			}
			unordered_map<string, int>::iterator itv = v.find(it->result);
			itv->second = varnum;
			it++;
		}
		else if (it->op == "+")
		{
			int varnum1, varnum2;
			if (it->src1[0] < '0' || it->src1[0] > '9')
			{
				varnum1 = findkey(it->src1);
			}
			else
			{
				varnum1 = str_to_int(it->src1);
			}
			if (it->src2[0] < '0' || it->src2[0] > '9')
			{
				varnum2 = findkey(it->src2);
			}
			else
			{
				varnum2 = str_to_int(it->src2);
			}
			unordered_map<string, int>::iterator itv = v.find(it->result);
			itv->second = varnum1 + varnum2;
			it++;
		}
		else if (it->op == "-")
		{
			int varnum1, varnum2;
			if (it->src1[0] < '0' || it->src1[0] > '9')
			{
				varnum1 = findkey(it->src1);
			}
			else
			{
				varnum1 = str_to_int(it->src1);
			}
			if (it->src2[0] < '0' || it->src2[0] > '9')
			{
				varnum2 = findkey(it->src2);
			}
			else
			{
				varnum2 = str_to_int(it->src2);
			}
			unordered_map<string, int>::iterator itv = v.find(it->result);
			itv->second = varnum1 - varnum2;
			it++;
		}
		else if (it->op == "*")
		{
			int varnum1, varnum2;
			if (it->src1[0] < '0' || it->src1[0] > '9')
			{
				varnum1 = findkey(it->src1);
			}
			else
			{
				varnum1 = str_to_int(it->src1);
			}
			if (it->src2[0] < '0' || it->src2[0] > '9')
			{
				varnum2 = findkey(it->src2);
			}
			else
			{
				varnum2 = str_to_int(it->src2);
			}
			unordered_map<string, int>::iterator itv = v.find(it->result);
			itv->second = varnum1 * varnum2;
			it++;
		}
		else if (it->op == "/")
		{
			int varnum1, varnum2;
			if (it->src1[0] < '0' || it->src1[0] > '9')
			{
				varnum1 = findkey(it->src1);
			}
			else
			{
				varnum1 = str_to_int(it->src1);
			}
			if (it->src2[0] < '0' || it->src2[0] > '9')
			{
				varnum2 = findkey(it->src2);
			}
			else
			{
				varnum2 = str_to_int(it->src2);
			}
			if (varnum2 == 0)
			{
				cout << "����!  ��ĸ����Ϊ0!";
				exit(0);
			}
			unordered_map<string, int>::iterator itv = v.find(it->result);
			itv->second = varnum1 / varnum2;
			it++;
		}
		else if (it->op == "&")
		{
			int varnum1, varnum2;
			if (it->src1[0] < '0' || it->src1[0] > '9')
			{
				varnum1 = findkey(it->src1);
			}
			else
			{
				varnum1 = str_to_int(it->src1);
			}
			if (it->src2[0] < '0' || it->src2[0] > '9')
			{
				varnum2 = findkey(it->src2);
			}
			else
			{
				varnum2 = str_to_int(it->src2);
			}
			unordered_map<string, int>::iterator itv = v.find(it->result);
			itv->second = varnum1 && varnum2;
			it++;
		}
		else if (it->op == "|")
		{
			int varnum1, varnum2;
			if (it->src1[0] < '0' || it->src1[0] > '9')
			{
				varnum1 = findkey(it->src1);
			}
			else
			{
				varnum1 = str_to_int(it->src1);
			}
			if (it->src2[0] < '0' || it->src2[0] > '9')
			{
				varnum2 = findkey(it->src2);
			}
			else
			{
				varnum2 = str_to_int(it->src2);
			}
			unordered_map<string, int>::iterator itv = v.find(it->result);
			itv->second = varnum1 || varnum2;
			it++;
		}
		else if (it->op == "==")
		{
			int varnum1, varnum2;
			if (it->src1[0] < '0' || it->src1[0] > '9')
			{
				varnum1 = findkey(it->src1);
			}
			else
			{
				varnum1 = str_to_int(it->src1);
			}
			if (it->src2[0] < '0' || it->src2[0] > '9')
			{
				varnum2 = findkey(it->src2);
			}
			else
			{
				varnum2 = str_to_int(it->src2);
			}
			unordered_map<string, int>::iterator itv = v.find(it->result);
			if (varnum1 == varnum2)
			{
				itv->second = 1;
			}
			else
			{
				itv->second = 0;
			}
			it++;
		}
		else if (it->op == "!=")
		{
			int varnum1, varnum2;
			if (it->src1[0] < '0' || it->src1[0] > '9')
			{
				varnum1 = findkey(it->src1);
			}
			else
			{
				varnum1 = str_to_int(it->src1);
			}
			if (it->src2[0] < '0' || it->src2[0] > '9')
			{
				varnum2 = findkey(it->src2);
			}
			else
			{
				varnum2 = str_to_int(it->src2);
			}
			unordered_map<string, int>::iterator itv = v.find(it->result);
			if (varnum1 != varnum2)
			{
				itv->second = 1;
			}
			else
			{
				itv->second = 0;
			}
			it++;
		}
		else if (it->op == ">")
		{
			int varnum1, varnum2;
			if (it->src1[0] < '0' || it->src1[0] > '9')
			{
				varnum1 = findkey(it->src1);
			}
			else
			{
				varnum1 = str_to_int(it->src1);
			}
			if (it->src2[0] < '0' || it->src2[0] > '9')
			{
				varnum2 = findkey(it->src2);
			}
			else
			{
				varnum2 = str_to_int(it->src2);
			}
			unordered_map<string, int>::iterator itv = v.find(it->result);
			if (varnum1 > varnum2)
			{
				itv->second = 1;
			}
			else
			{
				itv->second = 0;
			}
			it++;
		}
		else if (it->op == ">=")
		{
			int varnum1, varnum2;
			if (it->src1[0] < '0' || it->src1[0] > '9')
			{
				varnum1 = findkey(it->src1);
			}
			else
			{
				varnum1 = str_to_int(it->src1);
			}
			if (it->src2[0] < '0' || it->src2[0] > '9')
			{
				varnum2 = findkey(it->src2);
			}
			else
			{
				varnum2 = str_to_int(it->src2);
			}
			unordered_map<string, int>::iterator itv = v.find(it->result);
			if (varnum1 >= varnum2)
			{
				itv->second = 1;
			}
			else
			{
				itv->second = 0;
			}
			it++;
		}
		else if (it->op == "<")
		{
			int varnum1, varnum2;
			if (it->src1[0] < '0' || it->src1[0] > '9')
			{
				varnum1 = findkey(it->src1);
			}
			else
			{
				varnum1 = str_to_int(it->src1);
			}
			if (it->src2[0] < '0' || it->src2[0] > '9')
			{
				varnum2 = findkey(it->src2);
			}
			else
			{
				varnum2 = str_to_int(it->src2);
			}
			unordered_map<string, int>::iterator itv = v.find(it->result);
			if (varnum1 < varnum2)
			{
				itv->second = 1;
			}
			else
			{
				itv->second = 0;
			}
			it++;
		}
		else if (it->op == "<=")
		{
			int varnum1, varnum2;
			if (it->src1[0] < '0' || it->src1[0] > '9')
			{
				varnum1 = findkey(it->src1);
			}
			else
			{
				varnum1 = str_to_int(it->src1);
			}
			if (it->src2[0] < '0' || it->src2[0] > '9')
			{
				varnum2 = findkey(it->src2);
			}
			else
			{
				varnum2 = str_to_int(it->src2);
			}
			unordered_map<string, int>::iterator itv = v.find(it->result);
			if (varnum1 <= varnum2)
			{
				itv->second = 1;
			}
			else
			{
				itv->second = 0;
			}
			it++;
		}
		else if (it->op == "jmp")
		{
			int loop = str_to_int(it->result);
			for (vector<quaternion>::iterator gotoit = qua.begin(); gotoit != qua.end(); gotoit++)
			{
				
				if (gotoit->qualabel == loop)
				{
					it = gotoit;
					continue;
				}
			}
		}
		else if (it->op == "jnz")
		{
			int varnum;
			if (it->src1[0] < '0' || it->src1[0] > '9')
			{
				varnum = findkey(it->src1);
			}
			else
			{
				varnum = str_to_int(it->src1);
			}
			if (varnum != 0)
			{
				int loop = str_to_int(it->result);
				for (vector<quaternion>::iterator gotoit = qua.begin(); gotoit != qua.end(); gotoit++)
				{
					if (gotoit->qualabel == loop)
					{
						it = gotoit;
						continue;
					}
				}
			}
			else
			{
				it++;
			}
		}
		else if (it->op == "out")
		{
			int outint;
			if (it->src1[0] < '0' || it->src1[0] > '9')
			{
				outint = findkey(it->src1);
			}
			else
			{
				outint = str_to_int(it->src1);
			}
			string tempout = output + str(outint) + "\n";
			output = tempout;
			it++;
		}
	}
}

int main()
{
	Init_Symboltable();//��ʼ���ܷ��ű�
	lexAnalysis("text.txt");//�ʷ����� //�ʷ�������������:lexErrorText.txt
	if (lexFlag == 0)   //�ʷ�������ȷ�����򲻼��������﷨����
	{
		gramAnalysis();//�﷨����
		printqua(qua);
		printquatxt(qua);
		interpretation();
	}
	return 0;
}
