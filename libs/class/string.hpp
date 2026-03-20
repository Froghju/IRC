#pragma once
#include <string>
#include <iostream>

class string {

private:
    std::string _value;
    //char *_save;

public:
    string();
    string(const char* &src);
    string(std::string &src);
    string(const string &src);
    string& operator=(const string &src);
    ~string();

    string operator+(const string &src);
    string operator+(const char* &src);
    string operator+(const std::string &src);
    
    string& operator+=(const string &src);
    string& operator+=(const char* &src);
    string& operator+=(const std::string &src);
    
    char *str();
    char *&setStr(void);
    const std::string c_str(void) const;
    std::string getStr(void) const;
    size_t size(void) const;
    size_t len(void) const;
};

std::ostream& operator<<(std::ostream &o, string const &src);