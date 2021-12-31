#include"head.h"

string keyword[8] = { "int","if","then","else","while","do","write","for"};//定义关键字

unordered_map<string, int> symbolTable;//单词类别及符号管理表

//vector<pair<string, string>> lexAyTable;//定义表//改成结构体
vector<pair<string, string>> varTable;//定义变量表

int label = 100;//四元式首行号

//定义四元式表结构体
struct quaternion
{
	int qualabel;  //四元式行号
	string op;     //操作符
	string src1;   //操作数1
	string src2;   //操作数2
	string result; //操作结果
};
vector<quaternion> qua;

//词法分析结果存入该结构体
struct laTable
{
	string wordType;  //单词的类型
	string wordValue; //单词的值
	string wordLine;  //单词出现的行数:用于报错
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

/***************************************  词法分析  ***********************************/
/*
词法分析错误类型：
1.输入不合法字符或单词（单词符号表未定义单词）
2.标识符ID输入错误（例如数字开头）
*/
int lexFlag = 0;//定义词法分析正确flag=0，错误=1
string lexWrongMes;//用于存放词法分析错误点信息
//初始化定义文法符号表
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
	cout << "单词类别" << "            单词值" << "              行号" << endl;
	for (vector<laTable>::iterator it = lexAyTable.begin(); it != lexAyTable.end(); it++)
	{
		cout << setiosflags(ios::left) << setw(20) << it->wordType
			<< setiosflags(ios::left) << setw(20) << it->wordValue
			<< setiosflags(ios::left) << setw(20) << it->wordLine << endl;
	}
	cout << endl;
	if (lexFlag == 0)
	{
		cout << "词法分析结果正确!" << endl;
	}
	else if (lexFlag == 1)
	{
		cout << "词法分析结果错误!" << endl;
		cout << endl;
		cout << lexWrongMes << endl;
	}
	cout << endl;
}

void printVectxt(vector<laTable>& lexAyTable)
{
	char filename[] = "lexAnalysis.txt"; // 此处写入文件名
	ofstream fout(filename);
	fout << "单词类别" << "  单词值" << "    行号" << endl;
	for (vector<laTable>::iterator it = lexAyTable.begin(); it != lexAyTable.end(); it++)
	{
		fout << setiosflags(ios::left) << setw(10) << it->wordType
			<< setiosflags(ios::left) << setw(10) << it->wordValue
			<< setiosflags(ios::left) << setw(10) << it->wordLine << endl;
	}
	fout << endl;
	if (lexFlag == 0)
	{
		fout << "词法分析结果正确!" << endl;
	}
	else if (lexFlag == 1)
	{
		fout << "词法分析结果错误!" << endl;
		fout << endl;
		fout << lexWrongMes << endl;
	}
}

void printlexErrorVectxt(vector<laTable>& lexAyTable)
{
	char filename[] = "lexError.txt"; // 此处写入文件名
	ofstream fout(filename);
	fout << "单词类别" << "  单词值" << "    行号" << endl;
	for (vector<laTable>::iterator it = lexAyTable.begin(); it != lexAyTable.end(); it++)
	{
		fout << setiosflags(ios::left) << setw(10) << it->wordType
			<< setiosflags(ios::left) << setw(10) << it->wordValue
			<< setiosflags(ios::left) << setw(10) << it->wordLine << endl;
	}
	fout << endl;
	if (lexFlag == 0)
	{
		fout << "词法分析结果正确!" << endl;
	}
	else if (lexFlag == 1)
	{
		fout << "词法分析结果错误!" << endl;
		fout << endl;
		fout << lexWrongMes << endl;
	}
}

//数字转换为字符串
string str(int i)
{
	string res;
	stringstream ss;               //定义流ss
	ss << i;                      //将数字a转化成流ss
	ss >> res;                    //将流ss转化成字符串
	return res;
}

//字符串转数字
int str_to_int(string res)
{
	int a;
	stringstream ss;
	ss << res;
	ss >> a;
	return a;
}

//判断是否是单个字母
int isLetter(char letter)
{
	if (letter >= 'a' && letter <= 'z' || letter >= 'A' && letter <= 'Z')
	{
		return 1;
	}
	return 0;
}

//判断是否是单个数字
int isDigit(char digit)
{
	if (digit >= '0' && digit <= '9')
	{
		return 1;
	}
	return 0;
}

//判断是否是保留字
int isKeyword(string kw)
{
	for (int i = 0;i < 8;i++)
	{
		if (keyword[i] == kw)
			return 1; //在保留字数组中找到
	}	
	return 0; //在保留字数组中未找到
}

//判断是否为标识符 ：<字母>| <字母>（<字母>| <数字字符>)*
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

//判断是否为十进制数字 ：0|(1|2|3|4|5|6|7|8|9)(0|1|2|3|4|5|6|7|8|9)*
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

//利用表:根据助记符找到整数码
int findToken(string key)
{
	unordered_map<string, int>::iterator it = symbolTable.find(key);
	if (it == symbolTable.end())//查找失败
	{
		return -1;
	}
	else
	{
		return it->second;
	}
}

//单个单词词法分析函数
void lexical(string text, int lineNum)
{
	int token;
	string mnemonic;//助记符
	if (isKeyword(text)) //保留字
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
	{//如果无法直接找到，则肯定不是+、- 、* 、/ 、> 、< 、>= 、<= 、== 、！=、=、(){}、;，输入错误
		string info = "第";
		//lexAyTable.push_back(pair<string, string>("ERROR", text));
		laTable templa;
		templa.wordLine = str(lineNum);
		templa.wordType = "ERROR!";
		templa.wordValue = text;
		lexAyTable.push_back(laTable(templa));
		lexFlag = 1;
		info += str(lineNum);
		info += "行代码出现错误!";
		lexWrongMes += (info + "\n");
	}
	else //其余合法符号
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

//有穷自动机函数
void lexAnalysis(const char* filename)
{
	int lineNum = 0;
	fstream file;
	string currentLine;
	string tuple;
	string result = "";
	file.open(filename, ios_base::in | ios_base::out);
	if (!file.is_open()) {
		cout << "无法打开文件!" << endl;
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
				if (currentLine[i] == ' ') //如果是空格，继续读下一个
					continue;
				else
				{
					temp += currentLine[i];
					next += currentLine[i + 1];
					if (isLetter(currentLine[i]))// 当前扫描为字母
					{   //如果下一个是符号或空白或本行结尾，则将该字母输出
						if (findToken(next) >= 9 && findToken(next) <= 27 ||
							currentLine[i + 1] == '\0' || currentLine[i + 1] == ' ' ||
							currentLine[i + 1] == '!')
						{
							lexical(temp, lineNum);
							temp = "";
							next = "";
						}
						else // 否则继续匹配之后的作连接
							next = "";
					}
					else if (isDigit(currentLine[i]))// 当前扫描为数字
					{   //如果下一个是符号或空白，直接输出
						if (findToken(next) >= 9 && findToken(next) <= 27 ||
							currentLine[i + 1] == '\0' || currentLine[i + 1] == ' ')
						{
							lexical(temp, lineNum);
							temp = "";
							next = "";
						}
						else // 否则继续匹配之后的作连接
							next = "";
					}
					else if (findToken(temp) >= 9 && findToken(temp) <= 27)//如果是符号
					{   //如果是组合符号
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
							i++;//currentLine走了2个字符
						}
						else //如果是单个符号
						{
							lexical(temp, lineNum);
							temp = "";
							next = "";
						}
					}
					else//当前扫描为未知符号
					{   //如果下一个是符号或空白，直接输出
						if (currentLine[i + 1] == '\0' || currentLine[i + 1] == ' ')
						{
							lexical(temp, lineNum);
							temp = "";
							next = "";
						}
						else
						{
							// 否则继续匹配之后的作连接
							next = "";
						}
					}
				}
			}
		}
	}
	if (lexFlag == 0)
	{
		printVec(lexAyTable);//控制台打印结果
		printVectxt(lexAyTable);//输出结果到文件
	}
	else
	{
		printVec(lexAyTable);//控制台打印结果
		printlexErrorVectxt(lexAyTable);//输出结果到文件
	}
}

/*****************************  语法分析、语义分析、四元式生成  ***************************/
/*
语法分析及语义分析错误类型：
1.除号后面不能为0
2.不按照文法使用程序（while、for、if语句错误使用等）
3.赋值运算不合法
4.变量未定义直接使用
5.变量未赋值直接运算
*/
/*
四元式格式定义：
[操作符（op） ,操作数1（src1） ,操作数2（src2） ,操作结果（result） ]
*/
//控制台打印四元式
void printqua(vector<quaternion>& qua)
{
	cout << "输出四元式:" << endl;
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

//打印四元式到文件
void printquatxt(vector<quaternion>& qua)
{
	char filename[] = "quaternion.txt"; // 此处写入文件名
	ofstream fout(filename);
	fout << "输出四元式:" << endl;
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

int vecnum = 0;//访问vector容器的指针位置（全局变量）
string sym;
string getsym()
{
	sym = lexAyTable.at(vecnum).wordType;
	vecnum++;
	return sym;
}

int tempnum = 1;
string newtemp()//生成临时变量
{
	string nt;
	nt = "t" + str(tempnum);
	tempnum++;
	return nt;
}

//寻找变量表中是否存在该变量
int findVar(string var)
{
	for (vector<pair<string, string>>::iterator it = varTable.begin(); it != varTable.end(); it++)
	{
		if (it->first == var)
		{
			return -1;//找到
		}
	}
	return 1;//未找到
}

//寻找变量表中该变量是否赋值
int findVarValue(string var)
{
	for (vector<pair<string, string>>::iterator it = varTable.begin(); it != varTable.end(); it++)
	{
		if (it->first == var)
		{
			if (it->second == "NV")
			{
				return -1;//该变量未赋值
			}
		}
		
	}
	return 1;//该变量已赋值
}

void program()
{
	cout << "执行program()函数" << endl;
	if (sym == "{")
	{
		sym = getsym();
		declare();
		body();
		if (sym!="}") 
		{ 
			cout << "第" << lexAyTable.at(vecnum - 1).wordLine << "行错误! 缺少符号 }" << endl;
			exit(0); 
		}
		else
			sym = getsym();
	}
	else
	{
		cout << "第" << lexAyTable.at(vecnum - 1).wordLine << "行错误!";
		cout << "非法符号: " << sym << endl;
		exit(0);
	}
}

void declare()
{
	cout << "执行declare()函数" << endl;
	if (sym == "int")
	{
		sym = getsym();
		if (sym == "ID")
		{
			string var = lexAyTable.at(vecnum - 1).wordValue;
			varTable.push_back(pair<string,string>(var,"NV"));//插入变量表
			quaternion tempqua;//生成声明语句四元式
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
						//检查是否重定义
						if (findVar(var) == -1)
						{
							cout << "存在变量" << var << "重定义" << endl;
							exit(0);
						}
						varTable.push_back(pair<string, string>(var, "NV"));//NV代表未赋值状态
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
							cout << "第" << lexAyTable.at(vecnum - 1).wordLine << "行错误! 缺少符号 ;" << endl;
							exit(0);
						}
					}
				}
			}
			else
			{
				cout << "第" << lexAyTable.at(vecnum - 1).wordLine << "行错误! 缺少符号 ;" << endl;
				exit(0);
			}
		}
		else
		{
			cout << "第" << lexAyTable.at(vecnum - 1).wordLine << "行错误! 缺少变量 ID" << endl;
			exit(0);
		}
	}
	else
	{
		cout << "第" << lexAyTable.at(vecnum - 1).wordLine << "行错误!";
		cout << "非法符号: " << sym << endl;
		exit(0);
	}
}

void body()
{
	cout << "执行body()函数" << endl;
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
			//将jnz压入
			quaternion tempqua;
			tempqua.qualabel = label;
			whiletruelabel = label;
			label++;
			tempqua.op = "jnz";
			tempqua.src1 = tempq;
			tempqua.src2 = "_";
			tempqua.result = "";
			qua.push_back(quaternion(tempqua));
			//将jmp压入
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
					//存入真出口
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
					cout << "第" << lexAyTable.at(vecnum - 1).wordLine << "行错误! 缺少关键字 do" << endl;
					exit(0);
				}
			}
			else
			{
				cout << "第" << lexAyTable.at(vecnum - 1).wordLine << "行错误! 缺少符号 )" << endl;
				exit(0);
			}
		}
		else
		{
			cout << "第" << lexAyTable.at(vecnum - 1).wordLine << "行错误! 缺少符号 (" << endl;
			exit(0);
		}
	}
	else if (sym == "{")
	{
		sym = getsym();
		interval();
		if (sym != "}")
		{
			cout << "第" << lexAyTable.at(vecnum - 1).wordLine << "行错误! 缺少符号 }" << endl;
			exit(0);
		}
		else
		{
			sym = getsym();
		}
	}
	else if (sym == "ID")
	{
		//判断要赋值的变量是否定义
		string var = lexAyTable.at(vecnum - 1).wordValue;
		if (findVar(var) == 1)//未找到
		{
			cout << "变量" << var << "未定义就使用!" << endl;
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
					it->second = tempx;//找到
				}
			}
		}
		else if (sym == "+=")
		{
			string var = lexAyTable.at(vecnum - 2).wordValue;
			if (findVarValue(var) == -1)//未赋值
			{
				cout << "变量" << var << "未赋值就使用!" << endl;
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
					it->second = tempx;//找到
				}
			}

		}
		else if (sym == "-=")
		{
			string var = lexAyTable.at(vecnum - 2).wordValue;
			if (findVarValue(var) == -1)//未赋值
			{
				cout << "变量" << var << "未赋值就使用!" << endl;
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
					it->second = tempx;//找到
				}
			}
		}
		else if (sym == "*=")
		{
			string var = lexAyTable.at(vecnum - 2).wordValue;
			if (findVarValue(var) == -1)//未赋值
			{
				cout << "变量" << var << "未赋值就使用!" << endl;
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
					it->second = tempx;//找到
				}
			}
		}
		else if (sym == "/=")
		{
			string var = lexAyTable.at(vecnum - 2).wordValue;
			if (findVarValue(var) == -1)//未赋值
			{
				cout << "变量" << var << "未赋值就使用!" << endl;
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
						it->second = tempx;//找到
				}				
			}
		}
		else
		{
			cout << "第" << lexAyTable.at(vecnum - 1).wordLine << 
				"行错误! 缺少符号 = 或 += 或 -= 或 *= 或 /=" << endl;
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
			qua.push_back(quaternion(tempqua));//将jnz压入
			tempqua.qualabel = label;
			int falselabel = label;
			label++;
			tempqua.op = "jmp";
			tempqua.src1 = "_";
			tempqua.src2 = "_";
			tempqua.result = "";
			qua.push_back(quaternion(tempqua));//将jmp压入	
			if (sym == ")")
			{
				sym = getsym();
				if (sym == "then")
				{
					sym = getsym();					
					qua.at(truelabel - 100).result = str(label);//存入真出口
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
					cout << "第" << lexAyTable.at(vecnum - 1).wordLine << "行错误! 缺少关键字 then" << endl;
					exit(0);
				}
			}
			else
			{
				cout << "第" << lexAyTable.at(vecnum - 1).wordLine << "行错误! 缺少符号 )" << endl;
				exit(0);
			}
		}
		else
		{
			cout << "第" << lexAyTable.at(vecnum - 1).wordLine << "行错误! 缺少符号 (" << endl;
			exit(0);
		}
	}
	else if (sym == "write")
	{
		sym = getsym();
		if (sym == "ID")
		{
			string outstring = lexAyTable.at(vecnum - 1).wordValue;
			if (findVar(outstring) == 1)//未找到
			{
				cout << "变量" << outstring << "未定义就使用!" << endl;
				exit(0);
			}
			if (findVarValue(outstring) == -1)//未赋值
			{
				cout << "变量" << outstring << "未赋值就使用!" << endl;
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
			cout << "第" << lexAyTable.at(vecnum - 1).wordLine << "行错误! 缺少 ID 或 NUM" << endl;
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
				//判断要赋值的变量是否定义
				string var = lexAyTable.at(vecnum - 1).wordValue;
				if (findVar(var) == 1)//未找到
				{
					cout << "变量" << var << "未定义就使用!" << endl;
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
							it->second = tempx;//找到
						}
					}
					if (sym == ";")
					{
						int fortruelabel;
						int forfalselabel;
						sym = getsym();
						int forlabel = label;
						tempq = bool_or();
						//将jnz压入
						quaternion tempqua;
						tempqua.qualabel = label;
						fortruelabel = label;//记录jnz四元式的行数
						label++;
						tempqua.op = "jnz";
						tempqua.src1 = tempq;
						tempqua.src2 = "_";
						tempqua.result = "";
						qua.push_back(quaternion(tempqua));
						//将jmp压入
						tempqua.qualabel = label;
						forfalselabel = label;//记录jmp四元式的行数
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
								//判断要赋值的变量是否定义
								string var = lexAyTable.at(vecnum - 1).wordValue;
								if (findVar(var) == 1)//未找到
								{
									cout << "变量" << var << "未定义就使用!" << endl;
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
											it->second = tempx;//找到
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
											//存入真出口
											qua.at(fortruelabel - 100).result = str(label);
											body();
											quaternion tempqua;
											tempqua.qualabel = label;
											fortruelabel = label;//记录jnz四元式的行数
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
											cout << "第" << lexAyTable.at(vecnum - 1).wordLine << "行错误! 缺少保留字 do" << endl;
											exit(0);
										}
									}
									else
									{
										cout << "第" << lexAyTable.at(vecnum - 1).wordLine << "行错误! 缺少符号 )" << endl;
										exit(0);
									}
								}
								else
								{
									cout << "第" << lexAyTable.at(vecnum - 1).wordLine << "行错误! 缺少符号 =" << endl;
									exit(0);
								}
							}
						}
						else
						{
							cout << "第" << lexAyTable.at(vecnum - 1).wordLine << "行错误! 缺少符号 ;" << endl;
							exit(0);
						}
					}
					else
					{
						cout << "第" << lexAyTable.at(vecnum - 1).wordLine << "行错误! 缺少符号 ;" << endl;
						exit(0);
					}
				}
			}
		}
	}
	else
	{
		cout << "第" << lexAyTable.at(vecnum - 1).wordLine << "行错误!";
		cout << "非法符号: " << sym << endl;
		exit(0);
	}
}

void interval()//L→SL_
{
	cout << "执行interval()函数" << endl;
	if (sym == "if" || sym == "while" || sym == "{" || sym == "ID" 
		|| sym == "write" || sym == "for")// if (sym属于first(S))
	{
		body();
		semicolon();
	}
	else
	{
		cout << "第" << lexAyTable.at(vecnum - 1).wordLine << "行错误!";
		cout << "非法符号: " << sym << endl;
		exit(0);
	}
}

void semicolon()//L_ →; L |~
{
	cout << "执行semicolon()函数" << endl;
	if (sym == ";")
	{
		sym = getsym();
		interval();
	}
	else if (sym != "}")    /* if (sym不属于follow(L_)) */
	{
		cout << "第" << lexAyTable.at(vecnum - 1).wordLine << "行错误!";
		cout << "非法符号: " << sym << endl;
		exit(0);
	}
}

string bool_or()//B→T_
{
	cout << "执行bool_or()函数" << endl;
	if (sym == "ID" || sym == "NUM")// if (sym属于first(T_))
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
		cout << "第" << lexAyTable.at(vecnum - 1).wordLine << "行错误!";
		cout << "非法符号: " << sym << endl;
		exit(0);
	}
}

string bool_and()//T_→F_
{
	cout << "执行bool_and()函数" << endl;
	if (sym == "ID" || sym == "NUM")// if (sym属于first(F_))
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
		cout << "第" << lexAyTable.at(vecnum - 1).wordLine << "行错误!";
		cout << "非法符号: " << sym << endl;
		exit(0);
	}
}

string comparison()//F_→ID relop ID | ID | NUM | NUM relop ID | ID relop NUM
{
	cout << "执行comparison()函数" << endl;
	if (sym == "ID")
	{
		string var = lexAyTable.at(vecnum - 1).wordValue;
		if (findVar(var) == 1)//未找到
		{
			cout << "变量" << var << "未定义就使用!" << endl;
			exit(0);
		}
		if (findVarValue(var) == -1)//未赋值
		{
			cout << "变量" << var << "未赋值就使用!" << endl;
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
				if (findVar(var) == 1)//未找到
				{
					cout << "变量" << var << "未定义就使用!" << endl;
					exit(0);
				}
				if (findVarValue(var) == -1)//未赋值
				{
					cout << "变量" << var << "未赋值就使用!" << endl;
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
				cout << "第" << lexAyTable.at(vecnum - 1).wordLine << "行错误! 缺少变量 ID 或 NUM" << endl;
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
				if (findVar(numvar) == 1)//未找到
				{
					cout << "变量" << numvar << "未定义就使用!" << endl;
					exit(0);
				}
				if (findVarValue(numvar) == -1)//未赋值
				{
					cout << "变量" << numvar << "未赋值就使用!" << endl;
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
				cout << "第" << lexAyTable.at(vecnum - 1).wordLine << "行错误! 缺少变量 ID" << endl;
				exit(0);
			}
		}
	}
	else
	{
		cout << "第" << lexAyTable.at(vecnum - 1).wordLine << "行错误!";
		cout << "非法符号: " << sym << endl;
		exit(0);
	}  //非法符号
}

string add_sub()//E→T{+T| -T}
{
	cout << "执行add_sub()函数" << endl;
	if (sym == "ID" || sym == "NUM" || sym == "(")// if (sym属于first(T))
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
		cout << "第" << lexAyTable.at(vecnum - 1).wordLine << "行错误!";
		cout << "非法符号: " << sym << endl;
		exit(0);
	}
}

string mul_div()//T→F{* F | /F }
{
	cout << "执行mul_div()函数" << endl;
	if (sym == "ID" || sym == "NUM" || sym == "(")// if (sym属于first(F))
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
					cout << "除法分母不可为0" << endl;
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
		cout << "第" << lexAyTable.at(vecnum - 1).wordLine << "行错误!";
		cout << "非法符号: " << sym << endl;
		exit(0);
	}
}

string expression()//F→ (E) | NUM | ID
{
	cout << "执行expression()函数" << endl;
	if (sym == "(")
	{
		sym = getsym();
		string tempq;
		tempq = add_sub();
		if (sym!=")")
		{
			cout << "第" << lexAyTable.at(vecnum - 1).wordLine << "行错误! 缺少符号 }" << endl;
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
		if (findVar(var) == 1)//未找到
		{
			cout << "变量" << var << "未定义就使用!" << endl;
			exit(0);
		}
		if (findVarValue(var) == -1)//未赋值
		{
			cout << "变量" << var << "未赋值就使用!" << endl;
			exit(0);
		}
		sym = getsym();
		return var;
	}
	else
	{
		cout << "第" << lexAyTable.at(vecnum - 1).wordLine << "行错误!";
		cout << "非法符号: " << sym << endl;
		exit(0);
	}
}

int gramAnalysis()
{
	cout << "执行gramAnalysis()函数" << endl;
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
		cout << "语法分析结果正确!" << endl;
		return 1;
	}
	else
	{
		cout << "main函数错误!" << endl;
		return -1;
	}
}

unordered_map<string, int> v;//执行变量表

int findkey(string key)
{
	unordered_map<string, int>::iterator it = v.find(key);
	if (it == v.end())//查找失败
	{
		return -1;
	}
	else
	{
		return it->second;
	}
}

/***************************************  执行程序  ***********************************/
/*利用迭代器指针循环运行所有四元式及其跳转，至到运行到FIN，输出运行结束后所有变量的值*/
string output;
//执行程序（完成变量值计算）（简易图灵机:循环，读到不同的四元式执行不同的操作，直到程序结束）
void interpretation()
{
	int max = 0;
	//找出一共生成了多少临时变量
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

	//临时变量全部初始化
	for (int i = 0;i < max;i++)
	{
		string intpreword = "t" + str(i + 1);
		int intprevalue = -1;
		v.insert(pair<string, int>(intpreword,intprevalue));
	}
	
	//执行
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
			char filename[] = "write.txt"; // 此处写入文件名
			ofstream fout(filename);
			fout << output << endl;
			cout << "程序执行完毕!" << endl;
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
				cout << "错误!  分母不能为0!";
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
	Init_Symboltable();//初始化总符号表
	lexAnalysis("text.txt");//词法分析 //词法分析错误样例:lexErrorText.txt
	if (lexFlag == 0)   //词法分析正确，否则不继续进行语法分析
	{
		gramAnalysis();//语法分析
		printqua(qua);
		printquatxt(qua);
		interpretation();
	}
	return 0;
}
