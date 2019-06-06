#include<iostream>
#include<fstream>
#include<vector>

//for json
#include<string>
#include<utility>
#include <sstream>

#define QUOTES FUNC_QUOTES()
std::string FUNC_QUOTES () { return std::string("\""); }


std::string OFFSET(int  count){ return std::string (count, ' ');} 

#define GETVAL(val)\
	std::stringstream _ss;\
	_ss << val;

#define GETNAME(var) \
	json_str += OFFSET(_offset) + QUOTES + #var + QUOTES;\
	json_str += ": ";
	
#define PUTVAL(value) json_str += (value); 
	
#define PUTVAL_Q(value) json_str += QUOTES; json_str += (value); json_str += QUOTES;
	
void CLEARLAST(int count, std::string& str) 
{
	if(str.size()>2)
		str.resize(str.size()-count);
}

#define JSON_START \
	std::string getJson(int _offset = 0){\
	std::string json_str = "{\n";
	
#define JSON_END \
	CLEARLAST(2, json_str);\
	json_str+="\n" + OFFSET(_offset) + "}\n";\
	return json_str;}
	
template <class T>
bool CHECK_FOR_QUOTES(T& var)
{
	return (typeid(var) == typeid(std::string)||typeid(var) == typeid(char)||typeid(var) == typeid(char*)) + (typeid(var) == typeid(const std::string)||typeid(var) == typeid(const char)||typeid(var) == typeid(const char*));
}

#define ADDVAR(var) \
	{\
	GETNAME(var)\
	GETVAL(var)\
	bool var_quotes = CHECK_FOR_QUOTES(var);\
	if(var_quotes){PUTVAL_Q(_ss.str())}else{PUTVAL(_ss.str())}\
	json_str += ",\n";}

#define ADDARR(arr) \
	{\
	GETNAME(arr)\
	json_str += OFFSET(_offset) + "[";\
	bool var_quotes = CHECK_FOR_QUOTES(arr[0]);\
	for(int i = 0; i < sizeof(arr)/sizeof(arr[0]); i++){\
	std::stringstream _ss;\
	_ss << arr[i]; \
	if(var_quotes){PUTVAL_Q(_ss.str())}else{PUTVAL(_ss.str())}\
	json_str += ", ";}\
	CLEARLAST(2, json_str);\
	json_str += "],\n";}
	
#define ADDCLASS(obj)\
	{\
	GETNAME(obj)\
	json_str += obj.getJson(_offset + 4 + std::string(#obj).length());\
	CLEARLAST(1, json_str);\
	json_str += ",\n";}
	
//template<template <class> class ContainerT, class ValueT>
template<typename ValueT, template <typename U, typename =std::allocator<U>> class ContainerT>
void writeToJsonFromContaiter(ContainerT<ValueT>& arr, std::string& json)
{
	using namespace std;
	json+="[";
	ContainerT<ValueT>::iterator iter = arr.begin();
	ValueT tmp;
	bool var_quotes = CHECK_FOR_QUOTES(tmp);
	for ( ; iter != arr.end(); ++iter) 
	{
		std::stringstream _ss;
		_ss << *iter;
		if(var_quotes)
		{
			json += QUOTES;
			json += _ss.str();
			json += QUOTES;	
		}
		else
			json += _ss.str();
		json +=  + ", ";
		_ss.clear();
	}
	if(json.size()>2)
		json.resize(json.size()-2);
	json += "],\n";
}
#define ADDCONTAINER(arr)\
	{GETNAME(arr)\
	writeToJsonFromContaiter(arr,json_str);}\
	
struct D
{
int a1;
int b1;
 D(){a1 =5 ; b1=4;}
JSON_START
	ADDVAR(a1)
	ADDVAR(b1)
JSON_END
};

struct C
{
int a1;
int b1;
D d;
 C(){a1 =5 ; b1=4;}
JSON_START
	ADDVAR(a1)
	ADDVAR(b1)
	ADDCLASS(d)
JSON_END
};
	
struct B
{
int a1;
int b1;
C c;
 B(){a1 =5 ; b1=4;}
JSON_START
	ADDVAR(a1)
	ADDVAR(b1)
	ADDCLASS(c)
JSON_END
};

	
struct A
{
int ml;
double hl;
const char  *str;
int aray[3];
char caray[4];
std::string saray[3];
std::string strpp;
std::vector<int> veci;
std::vector<char> vecc;

B ba;
 
JSON_START
	ADDVAR(ml)
	ADDVAR(hl)
	ADDVAR(str)
	ADDVAR(strpp)
	ADDARR(aray)
	ADDARR(caray)
	ADDARR(saray)
	ADDCLASS(ba)
	ADDCONTAINER(veci)
	ADDCONTAINER(vecc)
JSON_END


};


#include<string.h>
void test()
{
	using namespace std;
	A obj;
	obj.ml =3;
	obj.hl=19.19;
	obj.str = "C string";
	obj.strpp = "c++ string";
	obj.aray[0] = 1;
	obj.aray[1] = 3;
	obj.aray[2] = 2;
	obj.saray[0] = "123";
	obj.saray[1] = "456";
	obj.saray[2] = "789";
	obj.veci = std::vector<int>(3,3);
	obj.vecc = std::vector<char >(3,'Y');	
	strcpy(obj.caray, "abdA"); 
	cout<< obj.getJson();
	
}



int main () 
{
	test();
	getchar();
	getchar();
}