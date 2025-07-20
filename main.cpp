#include <iostream>
#include <string>
#include <sstream>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <map>
#include <vector>
using namespace std;

map<string, long long> varList; //변수값 저장하는 map
vector<long long> print_values; //출력할 값들 저장하는 벡터
vector<string> tokens; //입력된 문장을 토큰화하여 저장
string token; //현재 토큰
int token_index;
long long aexpr();

void error(){
    cerr << "Syntax Error!\n";
    exit(2);
}

void initVarList(){
    varList = map<string, long long>();
    print_values = vector<long long>();
}

void insert_print_value(long long value, int execute){
    if(!execute) return;
    print_values.push_back(value);
}

void execute_prints(){
    for(long long x : print_values)
        cerr << x << " ";
    if(print_values.size() != 0)
        cerr << "\n";
}

int find_var(string name){
	return varList.count(name);
}

//변수 선언
void insert_var(string name){
    if(find_var(name)) error();
    varList[name]=0;
}
//변수 값 할당
void updateVarList(string name, long long value, int execute){
    if(!find_var(name)) error();
    if(!execute) return;
    varList[name]=value;
}

long long get_var(string name){
    if(!find_var(name)) error();
    return varList[name];
}

void get_inst(){
    string inst;
    token_index=0;
    tokens = vector<string>();

    getline(cin, inst);
    if(inst.empty()) exit(1);

    istringstream iss(inst);
    string t;
    while(iss >> t)
        tokens.push_back(t);
	if(tokens.size()==1)
		error();
}

void nextToken(){
    if(token_index < tokens.size())
		token = tokens[token_index++];
	else
		token = "";
}

void check(string exp){
    if(token == exp) nextToken();
    else error();
}

bool isNumber(string& s){
    if(s.empty()) return false;
    for(char c : s){
        if(!isdigit(c)) return false;
    }
    return true;
}

long long number(){
    if(isdigit(token[0])){
        if(!isNumber(token)) error();
        long long num = stoll(token);
        nextToken();
        return num;
    }
    error();
    return 0;
}

string var(){
    if(token.empty() || token[0] < 'a' || token[0] > 'z')
        error();

    if(token == "print" || token == "integer")
        error();
	if(token == "while" || token == "do" || token == "if" || token=="else")
		error();
    for(char c : token){
        if(c < 'a' || c > 'z')
            error();
    }
    string v = token;
    nextToken();
    return v;
}

int type(){
    if(token == "integer")
        return 1;
    else
        return 0;
}

long long factor(){
    long long sign = 1;
    if(token == "-"){
        sign = -1;
        nextToken();
    }
    if(isdigit(token[0]))
        return sign * number();
    else if(token[0]>='a' && token[0]<='z')
        return sign * get_var(var());
    else if(token == "("){
        nextToken();
        long long value = aexpr();
        check(")");
        return sign * value;
    }
    error();
    return 0;
}

long long term(){
    long long value = factor();
    while(token == "*"){
        nextToken();
        value *= factor();
    }
    return value;
}

long long aexpr(){
    long long value = term();
    while(token == "+" || token == "-"){
        string op = token;
        nextToken();
        if(op == "+")
            value += term();
        else
            value -= term();
    }
    return value;
}

int relop(){
    string text = token;
    nextToken();
    int v = 0;
    if(text == "==")
        v=1;
    else if(text == "!=")
        v=2;
    else if(text == "<")
        v=3;
    else if(text == ">")
        v=4;
    return v;
}

int bexpr(){
    long long var1 = get_var(var());
    int op = relop();
    long long var2 = get_var(var());
    switch(op){
        case 1:
            return (var1 == var2);
        case 2:
            return (var1 != var2);
        case 3:
            return (var1 < var2);
        case 4:
            return (var1 > var2);
        default:
            error();
    }
    return 0;
}

int declaration(){
    string name;
    if(type()){
        nextToken();
        name = var();
        check(";");
        insert_var(name);
        return 1;
    }
    return 0;
}

void statement(int execute){
    if(token == "print"){
        nextToken();
        long long value = aexpr();
        check(";");
        insert_print_value(value, execute);
    }
    else if(token == "while"){
		int save_index = token_index;
		nextToken();
		check("(");
		int e = bexpr();
		if(execute==0) e=0;
		check(")");
		check("do");
		check("{");
		
		while(token != "}"){
			statement(e);
		}
		check("}");
		check(";");
		
		if(e){
			token = tokens[save_index-1];
			token_index=save_index;
			statement(e);
		}
	}else if(token == "if"){
		nextToken();
		check("(");
		int e = bexpr();
		int ce = e;
		if(execute==0) e = 0;
		check(")");
		check("{");
		while(token!="}"){
			statement(e);
		}
		check("}");
		check("else");
		check("{");
		if(ce==1) e=0;
		else e=1;
		if(execute==0) e = 0;
		while(token!="}"){
			statement(e);
		}
		check("}");
		check(";");
	}
    else if(token[0] >= 'a' && token[0] <= 'z'){
		string name = var();
        check("=");
        long long value = aexpr();
		check(";");
        updateVarList(name, value, execute);
    }
    else
        error();
}

void program(){
    int r=1;
    while(token_index < tokens.size()){
        if(r)
            r=declaration();
        else
            statement(1);
    }
    execute_prints();
}

int main(){
    cerr << ">> ";
    while(1){
        pid_t pid = fork();
        if(pid<0){
            perror("fork fail");
            return 0;
        }else if(pid==0){
            initVarList();
            get_inst();
            nextToken();
            program();
            cerr << ">> ";
            exit(0);
        }else{
            int status;
            wait(&status);
            if(WIFEXITED(status)){
                if(WEXITSTATUS(status)==1) break;
                else if(WEXITSTATUS(status)==2){
                    cerr << ">> ";
                }
            }
        }
    }
    return 0;
}

