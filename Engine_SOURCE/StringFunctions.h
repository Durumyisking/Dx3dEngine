#pragma once
#include <string>
#include <iostream>
#include <algorithm>

#define MsgBoxAssert(Text) 	MessageBeep(0); \
MessageBoxA(nullptr, Text, "Error", MB_OK); \
assert(false); 



void GetLastErrorPrint();

void ToUpper(std::string& text);
std::string ToUpperReturn(const std::string& text);

void AnsiToUniCode(const std::string& text, std::wstring& out);
std::wstring AnsiToUniCodeReturn(const std::string& text);
void UniCodeToUTF8(const std::wstring& text, std::string& out);
void AnsiToUTF8(const std::string& text, std::string& out);
std::string AnsiToUTF8Return(const std::string& text);
std::wstring StringToWStringReturn(const std::string& text);

bool UniCodeToAnsi(const std::wstring& uniCode, std::string& ansiString);
bool UTF8ToUniCode(const std::string& UTF8, std::wstring& uniCode);

std::string UniCodeToAnsiReturn(const std::wstring& uniCode);
std::wstring UTF8ToUniCodeReturn(const std::string& UTF8);
std::string UTF8ToAnsiReturn(const std::string& UTF8);

std::string Replace(const std::string& text, const std::string_view& oldText, const std::string_view& changeText);

std::wstring ConvertToW_String(const char* str);
std::string ConvertToString(const wchar_t* str);

