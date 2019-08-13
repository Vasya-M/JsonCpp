#include<iostream>
#include<fstream>
#include<vector>

//for json
#include<string>
#include<typeinfo>
#include<utility>
#include<type_traits>
#include <sstream>
#include<string.h>

#if __cplusplus >= 201103L
#define NULLp nullptr
#else
#define NULLp NULL
#endif


//*******************************************************************
#define EMPTY()
#define DEFER(id) id EMPTY()
#define OBSTRUCT(...) __VA_ARGS__ DEFER(EMPTY)()
#define EXPAND(...) __VA_ARGS__

#define declFUNCTION(templateLIst,ret, name,  funcArg)    \
	template <EXPAND templateLIst>\
	struct name\
	{\
	ret operator()funcArg{

#define declFUNC(templateLIst, ...)\
	declFUNCTION(templateLIst,__VA_ARGS__)

#define partialFUNCTION(templateLIst,tempArgs,ret, name, funcArg)\
	template <EXPAND templateLIst>\
	struct name <EXPAND tempArgs>\
	{\
	ret operator()funcArg{

#define partialFUNC(templateLIst, tempArgs, ...)\
	partialFUNCTION(templateLIst, tempArgs, __VA_ARGS__)
#define endFUNC }};
//*******************************************************************

#define QUOTES std::string("\"")

std::string OFFSET(int  count) {
	return std::string (count, ' ');
} 

#define GETVAL(val)\
	std::stringstream _ss;\
	_ss << val;

#define GETNAME(var) \
	json_str += OFFSET(_offset) + QUOTES + #var + QUOTES;\
	json_str += ": ";

#define VARIABLE_NAME(var) #var
	
#define PUTVAL(value) json_str += (value); 
#define PUTVAL_Q(value) json_str += QUOTES; json_str += (value); json_str += QUOTES;

#define JSON_START \
	std::string getJson(int _offset = 0){return mainJsonFunc(NULLp, _offset);} \
	std::string readJson(std::string& json_source) {return mainJsonFunc(&json_source);}\
	std::string mainJsonFunc(std::string* source, int _offset = 0){\
	bool output = false; if(source == NULLp){output = true;}\
	std::string json_str = "{\n";
	
//#define CLEARLAST(count) if(json_str.size()>2){json_str.resize(json_str.size()-count);}
void CLEARLAST(std::string& str, int count) {
	if(str.size()>2) {
		str.resize(str.size()-count);
	}
}
#define JSON_END \
	if(output){\
		CLEARLAST(json_str ,2);\
		json_str+="\n" + OFFSET(_offset) + "}\n";\
		return json_str;\
		} else { return std::string(" "); }\
	}
	
	
template<class T>
bool CHECK_FOR_QUOTES(T& var) {
	bool ret = (typeid(var) == typeid(std::string)||typeid(var) == typeid(char)||typeid(var) == typeid(char*));
	ret += (typeid(var) == typeid(const std::string)||typeid(var) == typeid(const char)||typeid(var) == typeid(const char*));
	return ret;
}

/*#define CHECK_FOR_QUOTES(var) (typeid(var) == typeid(std::string)||typeid(var) == typeid(char)||typeid(var) == typeid(char*))\
	+	(typeid(var) == typeid(const std::string)||typeid(var) == typeid(const char)||typeid(var) == typeid(const char*)); */

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
	CLEARLAST(json_str, 2);\
	json_str += "],\n";}

template<class t>
void addVar__(t var,std::string& json_str, int _offset) {
	GETVAL(var)
	
	bool var_quotes = CHECK_FOR_QUOTES(var);
	if(var_quotes) {
		PUTVAL_Q(_ss.str())
	} else { 
		PUTVAL(_ss.str())
	}
	json_str += ",\n";
}


#define addVar_  void, addVar, (t var,std::string& json_str, int _offset) 
declFUNC( (bool valid ,class t), addVar_)
std::cout <<"-------- received inv value ------ \n";
endFUNC

partialFUNC( (class t),(true, t), addVar_ ) 

	std::cout <<"-------- received value ------ "<<var<<std::endl;
	GETVAL(var)
	bool var_quotes = CHECK_FOR_QUOTES(var);
	if(var_quotes) {
		PUTVAL_Q(_ss.str())
	} else { 
		PUTVAL(_ss.str())
	}
	json_str += ",\n";
endFUNC


#define addArray_ void, addArray, (t &arr,std::string& json_str, int _offset)
declFUNC( (bool valid ,class t),  addArray_)

std::cout <<"-------- received inv arr value ------ \n";
endFUNC

partialFUNC( (class t),(true, t), addArray_ ) 
	std::cout <<"-------- received arr value ------ "<<arr[0]<<std::endl; 
	json_str += OFFSET(_offset) + "[";	
	bool var_quotes = CHECK_FOR_QUOTES(arr[0]);

	for(int i = 0; i < sizeof(arr)/sizeof(arr[0]); i++) {
		std::stringstream _ss;
		_ss << arr[i];
		 
		if(var_quotes) {
			PUTVAL_Q(_ss.str())
		} else { 
			PUTVAL(_ss.str())
		}
		json_str += ", ";
	}
	CLEARLAST(json_str, 2);
	json_str += "],\n";
endFUNC


#define addClass_ void, addClass, (t &obj,std::string& json_str,int var_len, int _offset)
declFUNC( (bool valid ,class t),  addClass_)
endFUNC

partialFUNC( (class t),(true, t), addClass_ ) 
	json_str += obj.getJson(_offset + 4 + var_len);
	CLEARLAST(json_str, 1);
	json_str += ",\n";
endFUNC


enum FIELD_TYPE {CLASS, ARRAY};

#define ADDFIELD(field)\
	if(output) {\
		GETNAME(field)\
			addClass<std::is_class<decltype(field)>::value, decltype(field)>()(field, json_str, std::char_traits<char>::length(#field),_offset);\
			addArray<std::is_array<decltype(field)>::value, decltype(field)>()(field, json_str, _offset);\
			addVar<std::is_fundamental<decltype(field)>::value,decltype(field)>()(field, json_str, _offset);\
			addVar<std::is_pointer<decltype(field)>::value,decltype(field)>()(field, json_str, _offset);\
			\
	}
	
#define ADDCLASS(obj)\
	{\
	GETNAME(obj)\
	json_str += obj.getJson(_offset + 4 + std::string(#obj).length());\
	CLEARLAST(json_str, 1);\
	json_str += ",\n";}
	

template<typename ValueT, template <typename U, typename = std::allocator<U>> class ContainerT>
void writeToJsonFromContaiter(ContainerT<ValueT>& arr, std::string& json) {
	using namespace std;
	json+="[";
	typename ContainerT<ValueT>::iterator iter = arr.begin();
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
	writeToJsonFromContaiter(arr,json_str);}
	
struct D
{
int a1;
int b1;
 D(){a1 =5 ; b1=4;}
JSON_START
	ADDFIELD(a1)
	ADDFIELD(b1)
JSON_END
};

struct C
{
int a1;
int b1;
D d;
 C(){a1 =5 ; b1=4;}
JSON_START
	ADDFIELD(a1)
	ADDFIELD(b1)
	ADDFIELD(d)
JSON_END
};
	
struct B
{
int a1;
int b1;
C c;
 B(){a1 =5 ; b1=4;}
JSON_START
	ADDFIELD(a1)
	ADDFIELD(b1)
	ADDCLASS(c)
JSON_END
};

	
struct A
{
int ml;
double hl;
 char  *str;
int aray[3];
char caray[4];
std::string saray[4];
std::string strpp;
std::vector<int> veci;
std::vector<char> vecc;

B ba;
 
JSON_START
	ADDFIELD(ml)
	ADDFIELD(hl)
	ADDFIELD(str) //------------
	ADDVAR(strpp)
	ADDFIELD(aray)
	ADDFIELD(caray)
	ADDFIELD(saray)
	ADDFIELD(ba)
	ADDCONTAINER(veci)
	ADDCONTAINER(vecc)
JSON_END


};


#include<string.h>
#include<fstream>
void test()
{
	using namespace std;
	A obj;
	obj.ml =3;
	obj.hl=19.19;
	obj.str = (char *)"C string";
	obj.strpp = "c++ string";
	obj.aray[0] = 1;
	obj.aray[1] = 2;
	obj.aray[2] = 3;
	obj.saray[0] = "000";
	obj.saray[1] = "123"; 
	obj.saray[2] = "456";
	obj.saray[3] = "789";
	obj.veci = std::vector<int>(3,3);
	obj.vecc = std::vector<char >(3,'Y');	
	strcpy(obj.caray, "abdA"); 
	std::cout<<"get json \n\r";
	ofstream ff("testt.txt");
	try
	{
	ff << obj.getJson();
	cout<< obj.getJson();
	}
	catch (...)
	{
		cout<<"cathced exception";
	}
	std::cout<<"end test \n";
	
//	 
//	 std::cout << std::boolalpha; 
// cout<<"\n "<< is_pointer<decltype (obj.aray)>::value;
// cout<<"\n "<<  is_lvalue_reference<decltype (obj.str)>::value;
// cout<<"\n "<< is_rvalue_reference<decltype (obj.str)>::value;
// cout<<"\n "<<  is_member_object_pointer<decltype (obj.str)>::value;
// cout<<"\n "<<  is_member_function_pointer<decltype (obj.str)>::value;
// cout<<"\n "<<  is_enum<decltype (obj.str)>::value;
//cout<<"\n "<<  is_union<decltype (obj.str)>::value;
//   cout<<"\n "<<is_class<decltype (obj.str)>::value;
//  cout<<"\n "<<is_function<decltype (obj.str)>::value;
	return;
}

 
#include<windows.h>
int main () 
{
	std::ofstream ff("tesstt.txt");
	ff << "sdsds";
	std::cout<<"start test \n\r";
	test(); 
	system("pause");
}
