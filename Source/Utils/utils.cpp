#include "utils.h"

#include <fstream>
#include <iostream>
#include <vector>

// Read file .ttx to JSON object
json readJSON(const char* direct) {
  ifstream f(direct, ios::binary);

  if (!f.is_open()) {
    cout << "Error! Can't open json file";
    exit(-1);
  }

  f.seekg(0, ios::end);
  int size = f.tellg();
  f.seekg(0, ios::beg);

  string rawData(size + 1, '\0');
  f.read((char*)(&rawData[0]), size);
  f.close();

  json jsonData = json::parse(rawData);
  return jsonData;
}

// Write JSON object to file .txt
void writeJSON(const json& json, const char* direct) {
  ofstream f(direct);
  f << json;
  f.close();
}

// References:
// https://cppdeveloper.com/c-nang-cao/ham-convert-chuoi-ky-tu-tu-utf16-sang-utf8-su-dung-c-native-code/
string utf8(const wstring& utf16) {
  uint16_t* utf16Buf = (uint16_t*)utf16.c_str();
  string utf18 = "";
  uint16_t utf16CodePoint = 0;

  for (utf16Buf; *utf16Buf != 0; ++utf16Buf) {
    utf16CodePoint = *utf16Buf;

    if (utf16CodePoint <= 0x007f) {
      // using 1 byte: 0xxxxxxx
      utf18.append(1, static_cast<char>(utf16CodePoint));
    } else if (utf16CodePoint <= 0x07ff) {
      // 11 bits codepoint
      // using 2 bytes: 110xxxxx 10xxxxxx
      char byte1 = 0xc0 | ((utf16CodePoint >> 6) & 0x1f);
      char byte2 = 0x80 | (utf16CodePoint & 0x3f);
      utf18.append(1, byte1);
      utf18.append(1, byte2);
    } else {
      // 16 bits codepoint
      // using 3 bytes: 1110xxxx 10xxxxxx 10xxxxxx
      char byte1 = 0xe0 | ((utf16CodePoint >> 12) & 0x0f);
      char byte2 = 0x80 | ((utf16CodePoint >> 6) & 0x3f);
      char byte3 = 0x80 | (utf16CodePoint & 0x3f);
      utf18.append(1, byte1);
      utf18.append(1, byte2);
      utf18.append(1, byte3);
    }
  }

  return utf18;
}

// References:
// https://stackoverflow.com/questions/7153935/how-to-convert-utf-8-stdstring-to-utf-16-stdwstring/38383389
wstring utf16(const string& utf8) {
  vector<unsigned long> buffer;
  string errorMessage = "not a UTF-8 string";

  size_t i = 0;
  while (i < utf8.size()) {
    unsigned long uni;
    size_t todo;

    unsigned char ch = utf8[i++];
    if (ch <= 0x7F) {
      uni = ch;
      todo = 0;
    } else if (ch <= 0xBF) {
      throw logic_error(errorMessage);
    } else if (ch <= 0xDF) {
      uni = ch & 0x1F;
      todo = 1;
    } else if (ch <= 0xEF) {
      uni = ch & 0x0F;
      todo = 2;
    } else if (ch <= 0xF7) {
      uni = ch & 0x07;
      todo = 3;
    } else {
      throw logic_error(errorMessage);
    }

    for (size_t j = 0; j < todo; ++j) {
      if (i == utf8.size()) throw logic_error(errorMessage);
      unsigned char ch = utf8[i++];
      if (ch < 0x80 || ch > 0xBF) throw logic_error(errorMessage);
      uni <<= 6;
      uni += ch & 0x3F;
    }

    if (uni >= 0xD800 && uni <= 0xDFFF) throw logic_error(errorMessage);
    if (uni > 0x10FFFF) throw logic_error(errorMessage);
    buffer.push_back(uni);
  }

  wstring utf16;
  for (size_t i = 0; i < buffer.size(); ++i) {
    unsigned long uni = buffer[i];
    if (uni <= 0xFFFF) {
      utf16 += (wchar_t)uni;
    } else {
      uni -= 0x10000;
      utf16 += (wchar_t)((uni >> 10) + 0xD800);
      utf16 += (wchar_t)((uni & 0x3FF) + 0xDC00);
    }
  }

  return utf16;
}

// Get the VND Money format
wstring vndPrice(unsigned long long price) {
  string priceStr = to_string(price);

  int len = priceStr.length();
  int count = 0;
  for (int i = len - 1; i > 0; i--) {
    count++;
    if (count < 3) continue;
    priceStr.insert(i, ".");
    count = 0;
  }

  wstring result = utf16(priceStr);
  result += L"đ";

  return result;
}

string zeroPrefix(unsigned int number) {
  string result = to_string(number);
  if (result.length() == 1) result = "0" + result;
  return result;
}

vector<string> getTokens(const string& data, const string& separator) {
  vector<string> tokens;

  int startPos = 0;
  int pos = data.find(separator);
  while (pos >= 0) {
      int length = pos - startPos;
      string word = data.substr(startPos, length);
      tokens.push_back(word);
      startPos = pos + 1;
      pos = data.find(separator, startPos);
  }

  int length = data.length() - startPos;
  string lastWord = data.substr(startPos, length);
  tokens.push_back(lastWord);

  return tokens;
}

// Get the main content of an utf16 string (return an utf8 string)
string getMainContent(wstring& data) {
  // Read the stopword list
  ifstream f(VNESE_STOPWORD_PATH);
  string s;
  vector<wstring> stopwords;
  while (!f.eof()) {
    getline(f, s);
    stopwords.push_back(utf16(s));
  }
  f.close();

  // Read the vietnamese wchar list
  f.open(VNESE_WCHAR_PATH);
  s = "";
  vector<wstring> vneseChars;
  while (!f.eof()) {
    getline(f, s);
    vneseChars.push_back(utf16(s));
  }
  f.close();

  // Change the data to lowercase
  wstring st = L" ";
  for (int i = 0; i < data.length(); i++) {
    if (data[i] >= 'A' && data[i] <= 'Z') {
      data[i] = data[i] + 32;
    }
    
    // Special characters
    if (data[i] == ',' || data[i] == '.' || data[i] == '?' || data[i] == '-' || data[i] == '\r' ||
        data[i] == '\n' || data[i] == ')' || data[i] == '(' || data[i] == '\\' || data[i] == '"' ||
        data[i] == '!') {
      data.replace(i, 1, st);
    }
  }

  // Remove stopwords in data
  for (int i = 0; i < stopwords.size(); i++) {
    int pos = data.find(stopwords[i]);
    while (
        (pos == 0 || (pos > 0 && data[pos - 1] == ' ')) &&
        (data[pos + stopwords[i].length()] == ' ' || data[pos + stopwords[i].length()] == '\n')) {
      data.erase(pos, stopwords[i].length() + 1);
      pos = data.find(stopwords[i], pos);
    }
  }

  // Remove "sign" of vietnamese words
  for (int i = 0; i < vneseChars.size(); i++) {
    wstring ws;
    if (i >= 0 && i <= 9)
      ws = L"y";
    else if (i >= 10 && i <= 31)
      ws = L"u";
    else if (i >= 32 && i <= 65)
      ws = L"o";
    else if (i >= 66 && i <= 75)
      ws = L"i";
    else if (i >= 76 && i <= 97)
      ws = L"e";
    else if (i >= 98 && i <= 99)
      ws = L"d";
    else if (i >= 100 && i <= 133)
      ws = L"a";

    int pos = data.find(vneseChars[i]);

    while (pos >= 0) {
      data.replace(pos, 1, ws);
      pos = data.find(vneseChars[i], pos + 1);
    }
  }

  // Trimming the data
  while (data[0] == ' ') data.erase(0, 1);
  int i = 0;
  while (i < data.length())
    if (data[i] == ' ' && data[i + 1] == ' ')
      data.erase(i, 1);
    else
      i++;

  return utf8(data);
}
