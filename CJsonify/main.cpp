#include<iostream>
#include<fstream>
#include<vector>


#include<string.h>
#include<fstream>
#include<windows.h>

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


//*******************	PARTIAL FUNCTION`S SPECIALIZATION	********************

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


//************************	HELP FUNCTIONS	*******************************************

#define QUOTES std::string("\"")

std::string OFFSET(int  count) {
	return std::string (count, ' ');
} 

void CLEARLAST(std::string& str, int count) {
	if(str.size()>2) {
		str.resize(str.size()-count);
	}
}

template<class T>
bool CHECK_FOR_QUOTES(T& var) {
	bool ret = (typeid(var) == typeid(std::string)||typeid(var) == typeid(char)||typeid(var) == typeid(char*));
	ret += (typeid(var) == typeid(const std::string)||typeid(var) == typeid(const char)||typeid(var) == typeid(const char*));
	return ret;
}

#define GETVAL(val)\
	std::stringstream _ss;\
	_ss << val;

#define GETNAME(var) \
	json_str += OFFSET(_offset) + QUOTES + #var + QUOTES;\
	json_str += ": ";

	
#define PUTVAL(value) json_str += (value); 
#define PUTVAL_Q(value) json_str += QUOTES; json_str += (value); json_str += QUOTES;


//************************	HELP FUNCTIONS	*******************************************

#define JSON_START \
	std::string getJson(int _offset = 0){return mainJsonFunc(NULLp, _offset);} \
	std::string readJson(std::string& json_source) {return mainJsonFunc(&json_source);}\
	std::string mainJsonFunc(std::string* source, int _offset = 0){\
	bool output = false; if(source == NULLp){output = true;}\
	std::string json_str = "{\n";
	

#define JSON_END \
	if(output){\
		CLEARLAST(json_str ,2);\
		json_str+="\n" + OFFSET(_offset) + "}\n";\
		return json_str;\
		} else { return std::string(" "); }\
	}
	
	
//************************	WRITE TYPE FUNCTIONS	*******************************************

#define addVar_  bool, addVar, (t var,std::string& json_str, int _offset) 
declFUNC( (bool valid ,class t), addVar_)
	return false;
endFUNC

partialFUNC( (class t),(true, t), addVar_ ) 
	GETVAL(var)
	bool var_quotes = CHECK_FOR_QUOTES(var);
	if(var_quotes) {
		PUTVAL_Q(_ss.str())
	} else { 
		PUTVAL(_ss.str())
	}
	json_str += ",\n";
	return true;
endFUNC

#define addArray_ bool, addArray, (t &arr,std::string& json_str, int _offset)
declFUNC( (bool valid ,class t),  addArray_)
	return false;
endFUNC

partialFUNC( (class t),(true, t), addArray_ ) 
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
	return true;
endFUNC

#define addClass_ bool, addClass, (t &obj,std::string& json_str, int _offset, int var_len)
declFUNC( (bool valid ,class t),  addClass_)
	return false;
endFUNC

partialFUNC( (class t),(true, t), addClass_ ) 
	json_str += obj.getJson(_offset + 4 + var_len);
	CLEARLAST(json_str, 1);
	json_str += ",\n";
	return true;
endFUNC

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


//************************	TYPE CHECK FUNCTIONS	*******************************************

template<class>
struct sfinae_true : std::true_type{};

namespace detail{
	using namespace std;
  template<class T>
  static auto test_getJson(int)
      -> sfinae_true< decltype(declval<T>().getJson())>;
  template<class>
  static auto test_getJson(long) -> std::false_type;
} // detail::

template<class T>
struct has_getJson : decltype(detail::test_getJson<T>(0)){};


//************************	MAIN JSON FUNCTIONS 	*******************************************

#define defaultARGS json_str, offset
#define wrapCall2(funcName,typecheck,args) if(funcName<is_same<T, typecheck>::value, T>()(field, EXPAND args) == true) return;
#define wrapCall(funcName,typecheck,args) if(funcName<typecheck<T>::value, T>()(field, EXPAND args) == true) return;
#define LENGHT(var) std::char_traits<char>::length(var)

template<class T>
void WriteIntoJson(T& field, std::string &json_str, int offset, std::string field_name)
{
	using namespace std;
	json_str += OFFSET(offset) + QUOTES + field_name + QUOTES;
	json_str += ": ";
	
	wrapCall2(addVar,  string, 	        (defaultARGS))
	wrapCall(addClass, has_getJson,	    (defaultARGS, field_name.length()))
	wrapCall(addArray, is_array,	    (defaultARGS))
	wrapCall(addVar,   is_fundamental,  (defaultARGS))
	wrapCall(addVar,   is_pointer,      (defaultARGS))
}

#define ADDFIELD(field)\
	if(output) {\
		WriteIntoJson(field, json_str, _offset, #field);\
	}

#define ADDCONTAINER(arr)\
	{GETNAME(arr)\
	writeToJsonFromContaiter(arr,json_str);}
	
//************************	PUBLUC JSON INTERFACE	*******************************************
/*

1. In the end of your class declaration add next statement
	1.1	JSON_START
	1.2 JSON_END
	
2. Between these statements(1.1 and 1.2) add wanted fields
List of avaible fields type
	2.1 ADDFIELD(field_name)
		use this for any C/C++ types, arrays, yours custom class, and std::string
		if yours another class has CJsonify, it can be used as supported member for ADDFIELD, otherwise it ignoring this field 
	2.2	ADDCONTAINER(field_name)
		use it for STL containers (except std::string)

3. template of usage Cjsonify
class A {
	int a1;
	std::vector<int> a2;
JSON_START
	ADDFIELD(a1) 
	ADDCONTAINER(a2)
JSON_END
}

*/
	
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
	ADDFIELD(c)
JSON_END
};

	
struct A
{
int ml;
double hl;
 char  *str;
char caray[4];
int aray[3];
std::string saray[4];
std::string strpp;
std::vector<int> veci;
std::vector<char> vecc;

B ba;
 
JSON_START
	ADDFIELD(ml)
	ADDFIELD(hl)
	ADDFIELD(str) 
	ADDFIELD(strpp)
	ADDFIELD(aray)
	ADDFIELD(caray)
	ADDFIELD(saray)
	ADDFIELD(ba)
	ADDCONTAINER(veci)
	ADDCONTAINER(vecc)
JSON_END


};


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
	strcpy(obj.caray, "abAs");  
	std::cout<<"get json \n\r";
	//ofstream ff("testt.txt"); 
	try
	{ 
	//ff << obj.getJson(); 
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

int main () 
{
	using namespace std;
	std::ofstream ff("tesstt.txt");
	ff << "sdsds";
	std::cout<<"start test \n\r";
	test(); 
	system("pause");
}
