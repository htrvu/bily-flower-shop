#pragma once
#include <string>
#include "../CONSTANTS.h"
using namespace std;

// JSON Modern for C++
#include "json/json.hpp"
using json = nlohmann::json;

// SHA256 encoding
// References: http://www.zedwood.com/article/cpp-sha256-function
#include "sha256/sha256.h"

wstring utf16(const string& utf8);
string utf8(const wstring& utf16);

json readJSON(const char* direct);
void writeJSON(const json& json, const char* direct);

wstring vndPrice(unsigned long long price);
string zeroPrefix(unsigned int number);

vector<string> getTokens(const string& data, const string& separator);
string getMainContent(wstring& data);
