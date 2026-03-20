#include "../libs/class/string.hpp"

/*---- CONSTRUCTORS/DECONSTRUCTOR ----*/
string::string() {}
string::string(const char* &src) {
    _value = src;
}
string::string(std::string &src) {
    _value = src;
}
string::string(const string &src) {
    *this = src;
}
string::~string() {
    /*if (_save != NULL)
        free(_save);*/
}
string& string::operator=(const string &src) {
    if (_value != src._value)
        _value = src._value;
    /*if (_save)
        free(_save);
    _save = src._save;*/

    return (*this);
}

/*-----------------------------------*/
/*------------ OPERATORS ------------*/
string string::operator+(const string &src) {
    _value += src._value;
    return (*this);
}
string string::operator+(const char* &src) {
    _value += src;
    return (*this);
}
string string::operator+(const std::string &src) {
    _value += src;
    return (*this);
}
string& string::operator+=(const string &src) {
    _value += src._value;
    return (*this);
}
string& string::operator+=(const char* &src) {
    _value += src;
    return (*this);
}
string& string::operator+=(const std::string &src) {
    _value += src;
    return (*this);
}

/*-----------------------------------*/
/*--------- GETTER/SETTER -----------*/
char *string::str() {
    return ((char*)_value.c_str());
}
const std::string string::c_str(void) const {
    return (_value.c_str());
}
std::string string::getStr(void) const {
    return _value;
}
size_t string::size(void) const {
    return (_value.size());
}
size_t string::len(void) const {
    return (_value.length());
}

std::ostream& operator<<(std::ostream &o, const string &src) {
    o << src.getStr();
    return o;
}