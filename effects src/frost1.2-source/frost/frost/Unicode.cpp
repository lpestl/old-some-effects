
#include <memory>
#include <codecvt>
#include "Unicode.h"


std::string ConvertWideToUTF8(const std::wstring& string)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>> convert;
	return convert.to_bytes(string);
}

std::wstring ConvertUTF8ToWide(const std::string& string)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>> convert;
	return convert.from_bytes(string);
}


std::wstring ConvertToUTF16(std::string str, UINT codepage)
{
	int wstrCharCount = MultiByteToWideChar(codepage, 0, str.c_str(), -1, nullptr, 0);
	if( wstrCharCount > 0 )
	{
		std::shared_ptr<wchar_t> buf( new wchar_t[wstrCharCount] );
		if( MultiByteToWideChar(codepage, 0, str.c_str(), -1, buf.get(), wstrCharCount) > 0)
			return buf.get();
	}
	return L"";
}

std::string ConvertFromUTF16(std::wstring wstr, UINT codepage, char defaultChar)
{
	int strBytesCount = WideCharToMultiByte(codepage, 0, wstr.c_str(), -1, NULL, 0, &defaultChar, NULL);
	if( strBytesCount > 0 )
	{
		std::shared_ptr<char> buf( new char[strBytesCount] );
		if( WideCharToMultiByte(codepage, 0, wstr.c_str(), -1, buf.get(), strBytesCount, &defaultChar, NULL) > 0 )
			return buf.get();
	}
	return "";
}

std::string UnicodeToCurrentAnsi(std::wstring wstr, char defaultChar)
{
	return ConvertFromUTF16(wstr, CP_ACP, defaultChar);
}

std::wstring CurrentAnsiToUnicode(std::string str)
{
	return ConvertToUTF16(str, CP_ACP);
}