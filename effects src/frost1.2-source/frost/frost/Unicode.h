
#ifndef __UNICODECOVERSIONS_H__
#define __UNICODECOVERSIONS_H__

#include <Windows.h>
#include <string>


std::string ConvertWideToUTF8(const std::wstring& string);
std::wstring ConvertUTF8ToWide(const std::string& string);

std::wstring ConvertToUTF16(std::string str, UINT codepage);
std::string ConvertFromUTF16(std::wstring wstr, UINT codepage, char defaultChar = '\0');

std::string UnicodeToCurrentAnsi(std::wstring wstr, char defaultChar = '\0');
std::wstring CurrentAnsiToUnicode(std::string str);


#endif //__UNICODECOVERSIONS_H__