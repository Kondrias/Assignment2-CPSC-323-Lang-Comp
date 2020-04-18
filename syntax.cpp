#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <string.h>
#include <map>
#include <stack>
using namespace std;

//readings of files
ifstream myFile;
ofstream outFile;
//char arrays to store identifiers
char keywords[20][10] = {"int", "float", "bool", "true", "false", "if", "else", "then", "endif",
"while", "whileend", "do", "doend","for", "forend", "input", "output", "and", "or", "not"};
char separators[] = "'(){}[],:;";
char operators[] = {'*', '+', '-', '=', '/', '>', '<', '%'};
//check if theres separator
bool separation =false;
//check if theres operator
bool operation =false;
//index used in containsNum
int curIndex = 0;
//states to print
bool sep=false;
bool op=false;
bool ident=false;
bool num=false;
string tokensLex[100];
int countTokens=0;
int printTokenCount=0;
//get chars
char ch;
//store chars here to identify their type
char buffer[20];
//buffer index
int x = 0;
//check if comment
bool comments=false;
//!block comment! detection
char detection[2] = {'\0'};
//used in detecting count
int y = 0;
//contains keyword check
bool containsKey(char buffer[]){
	for (int i = 0; i < 20; ++i)
		if (strcmp(keywords[i], buffer) == 0)
	return true;
	return false;
}
//contains separator check
bool containsSep(char ch) {
	for (int i = 0; i < 10; ++i){
		if (ch == separators[i])
			return true;
		}
			return false;
	}
//contains operator check
bool containsOp(char ch){
	for (int i = 0; i < 10; ++i){
		if (ch == operators[i])
		return true;
	}
	return false;
}
//contains integer check
bool containsNum(char buffer[]){
	for (int i = 0; i <= (curIndex-1); i++){
		if (!isdigit(buffer[i]))
			return false;
			continue;
		}
	return true;
}
//terminal symbols identification parenthesis, signs..
enum Terminals{
	//(
	leftParen,
	//)
	rightParen,
	//id
	terminalId,
	//+
	addition,
	//-
	subtraction,
	//*
	multiplication,
	// /
	division,
	//=
	terminalEqual,
	//epsilon
	eps,
	//;
	terminator,
	//token inputted is invalid
	invalid,

	//non-terminals
	//Assign S
	nonTs,
	//Expression E
	nonTe,
	//Expression Prime Q
	nonTq,
	//Term T
	nonTt,
	//Term Prime R
	nonTr,
	//FActor F
	nonTf,
	endFile
};
//outputting
// will also add count of tokens and lexemes return
Terminals print(){
	if (ident){
	 	ident = false;
		string temp = buffer;
		tokensLex[countTokens] ="Token:Identifier				Lexeme:"+ temp;
		countTokens++;
		return terminalId;
	}
	if (num){
		num = false;
		string temp = buffer;
		tokensLex[countTokens] ="Token:Number					Lexeme:"+ temp;
		countTokens++;
		return terminalId;
	}
	//printing of separators
	if (sep){
		sep = false;
		separation = false;
		if (ch == ';'||ch == '('||ch == ')'){
			string temp (1,ch);
			tokensLex[countTokens] ="Token:Separator					Lexeme:"+ temp;
			countTokens++;
			if (ch == ';'){
				return terminator;
			 }
			 else if (ch == '('){
 				return leftParen;
 			}
 			else if (ch == ')'){
 				return rightParen;
 			}
		}
	}
	//printing of operators
	if (op){ // single operator detected, print it
		op = false;
		operation = false;
		if (ch == '+'|| ch == '-'|| ch == '*'|| ch == '/'|| ch == '='){
			string temp(1, ch);
			tokensLex[countTokens] ="Token:Operator					Lexeme:"+ temp;
			countTokens++;
			if (ch == '+'){
				 return addition;
			  }
			else if (ch == '-'){
				 return subtraction;
			  }
			else if (ch == '*'){
				 return multiplication;
			  }
			else if (ch == '/'){
				 return division;
			  }
			else if (ch == '='){
				 return terminalEqual;
			}
		}
	}
	return invalid;
}
//lexical analyzer
Terminals lexical(){
	//check for comment first if not then identify chars
	if (ch == '!'){
		detection[y] = ch;
		y++;
		if (detection[1] == ch){
			comments = false;
			y = 0;
			detection[0] = '\0';
			detection[1] = '\0';
		}
		else
			comments = true;
	  	return invalid;
	}
  	if (!comments){
	//not a comment then
	 if (containsSep(ch)){
		 separation = true;
		 sep = true;
		}
	 if (containsOp(ch)){
		 operation = true;
		 op = true;
		}
	//check if char is alphanumeric	. $
	if (isalnum(ch) || ch == '.' || ch == '$'){
		 buffer[x++] = ch;
		 curIndex++;
		}
		else if ((ch == '\n' || ch == ' ' ||separation||operation||myFile.eof())&&(x != 0)){
			buffer[x] = '\0';
			x = 0;
			if (containsKey(buffer)){
				cout << "Keyword 			= 			" << buffer << endl;
			}
			else if (containsNum(buffer))
				num = true;
			else{
				//check if doesnt start with number
				if (!isdigit(buffer[0]))
					ident = true;
			}
			//resetting
			curIndex = 0;
		}
		return print();
	}
}
Terminals lex(string n){
	if (n=="+"){
	    return addition;
	  }
		else if (n=="-"){
			return subtraction;
		}
		else if (n=="*"){
			return multiplication;
		}
		else if (n=="/"){
			return division;
		}
		else if (n=="="){
			return terminalEqual;
		}
		else if (n==";"){
			return terminator;
		}
		else if (n=="("){
			return leftParen;
		}
		else if (n==")"){
			return rightParen;
		}
		else if (n=="id"){
			return terminalId;
		}
		else return invalid;
	}
//switch statement for rule printing
void rulesPrint(int index){
	switch (index){
	case 1: cout << "	<Assig> -> id = <Expression>" << endl;
	break;
	case 2: cout << "	<Expression> -> <Term> <Expression Prime> " << endl;
  break;
	case 3: cout << "	<Expression Prime> -> + <Term> <Expression Prime> " << endl;
	break;
	case 4: cout << "	<Expression Prime> -> - <Term> <Expression Prime> " << endl;
	break;
	case 5: cout << "	<Expression Prime> -> eps" << endl;
	break;
	case 6: cout << "	<Term> -> <Factor> <Term Prime> " << endl;
	break;
	case 7: cout << "	<Term Prime> -> * <Factor> <Term Prime> " << endl;
	break;
	case 8: cout << "	<Term Prime> -> / <Factor> <Term Prime> " << endl;
	break;
	case 9: cout << "	<Term Prime> -> eps" << endl;
	break;
	case 10: cout << "	<Factor> -> (E)" << endl;
	break;
	case 11: cout << "	<Factor> -> id" << endl;
	break;
	default: cout << "There is no production Rule."<< endl;
	}
}
//conversion of symbols
string conversion(Terminals m){
	if (m == leftParen)
	return "(";
	else if (m == rightParen)
	return ")";
	else if (m == terminalId)
	return "id";
	else if (m == addition)
	return "+";
	else if (m == subtraction)
	return "-";
	else if (m == multiplication)
	return "*";
	else if (m == division)
	return "/";
	else if (m == terminalEqual)
	return "=";
	else if (m == terminator)
	return ";";
	else   return ".";
}
int main() {
	//parser table,terminal and nonterminals
	//stack for symbols to push
	map<Terminals,map<Terminals,int> >table;
	stack<Terminals> ss;
	Terminals a[100];
	int index = 0;
	for (int x = 0; x < 100; x++){
		a[x]=endFile;
	}
	string z;
	//buffer
	Terminals*m;
	bool printing = false;
	//start stack
	//pushing $ to stack
	ss.push(terminator);
	ss.push(nonTs);

	string filenameIn;
	cout <<"Enter file to read:" << endl;
	cin >> filenameIn;
	myFile.open(filenameIn);
	//error if could not open so exit
	if (!myFile.is_open()){
		cout << "Error\n";
		exit(0);
	}
	//implementation of lex,syntax
	while (!myFile.eof()){
		ch = myFile.get();
		Terminals temp = lexical();
		if (temp != invalid) {
			a[index] = temp;
			index++;
		}
	}
	m=a;
	//parsing table
	table[nonTs][terminalId]= 1;
	table[nonTe][terminalId]= 2;
	table[nonTe][leftParen]= 2;
	table[nonTq][addition]= 3;
	table[nonTq][subtraction]= 4;
	table[nonTq][rightParen]= 5;
	table[nonTq][terminator]= 5;
	table[nonTt][terminalId]= 6;
	table[nonTt][leftParen]= 6;
	table[nonTr][multiplication]= 7;
	table[nonTr][division]= 8;
	table[nonTr][addition]= 9;
	table[nonTr][subtraction]= 9;
	table[nonTr][rightParen]= 9;
	table[nonTr][terminator]= 9;
	table[nonTf][leftParen]= 10;
	table[nonTf][terminalId]= 11;

	int counter = 0;
	while(a[counter+1] != endFile){
		while (ss.size() > 0){
			z = conversion(*m);
			if (!printing){
				cout << tokensLex[printTokenCount] << endl;
				printing = true;
			}
			if (lex(z) == ss.top()){
				cout << endl;
				m++;
				counter++;
				printTokenCount++;
				printing = false;
				ss.pop();
			}
			else{
				rulesPrint(table[ss.top()][lex(z)]);
				switch (table[ss.top()][lex(z)]){
				//S->id=E
				case 1:
				ss.pop();
				ss.push(nonTe);
				ss.push(terminalEqual);
				ss.push(terminalId);
				break;
				//E->TQ
				case 2:
				ss.pop();
				ss.push(nonTq);
				ss.push(nonTt);
				break;
				//Q->+TQ
				case 3:
				ss.pop();
				ss.push(nonTq);
				ss.push(nonTt);
				ss.push(addition);
				break;
				//Q->-TQ
				case 4:
				ss.pop();
				ss.push(nonTq);
				ss.push(nonTt);
				ss.push(subtraction);
				break;
				//Q->eps
				case 5:
				ss.pop();
				break;
				//T->FR
				case 6:
				ss.pop();
				ss.push(nonTr);
				ss.push(nonTf);
				break;
				//R->*FR
				case 7:
				ss.pop();
				ss.push(nonTr);
				ss.push(nonTf);
				ss.push(multiplication);
				break;
				//R->/FR
				case 8:
				ss.pop();
				ss.push(nonTr);
				ss.push(nonTf);
				ss.push(division);
				break;
				//R->eps
				case 9:
				ss.pop();
				break;
				//F->(E)
				case 10:
				ss.pop();
				ss.push(rightParen);
				ss.push(nonTe);
				ss.push(leftParen);
				break;
				//F->ID
				case 11:
				ss.pop();
				ss.push(terminalId);
			  break;
				//syntatically error
				default:
				cout << "Syntax Error." << endl;
				system("pause");
				return 0;
				}
			}
		}
		ss.push(terminator);
		ss.push(nonTs);
	}
	cout << "Finish." <<endl;
	system("pause");
	return 0;
}
