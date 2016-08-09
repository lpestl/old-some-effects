
#ifndef __FILESYSTEM_H__
#define __FILESYSTEM_H__

#include <memory>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <iterator>
#include <direct.h>
#include <Windows.h>


class Directory
{
public:
	static std::string GetCurrentDir()
	{
		const int length = 512;
		char path[length];

		_getcwd(path, length * sizeof(char));
		return path;
	}

	static std::string LoadText(std::string path)
	{
		std::ifstream file(path);
		std::istreambuf_iterator<char> begin(file);
		std::istreambuf_iterator<char> end;
		return std::string( begin,end );
	}
};


class ProfileReader
{
public:
	ProfileReader(std::string file)
	{
		char buf[MAX_PATH];
		GetFullPathName(file.c_str(), MAX_PATH, buf, nullptr);
		m_fullPath = buf;
	}

	std::string GetString(std::string sectionHeader, std::string key, std::string def = "")
	{
		const int maxString = 512;
		char buffer[maxString];
		memset(buffer, 0, sizeof(buffer));

		int ret = GetPrivateProfileString(
			sectionHeader.c_str(),
			key.c_str(),
			def.c_str(),
			buffer,
			maxString,
			m_fullPath.c_str()
			);
		return buffer;
	}

	int GetInteger(std::string sectionHeader, std::string key, int def = 0)
	{
		std::string str = GetString(sectionHeader, key);
		if( str.empty() )
			return def;
		return atoi( str.c_str() );
	}

	float GetFloat(std::string sectionHeader, std::string key, float def = 0.0f)
	{
		std::string str = GetString(sectionHeader, key);
		if( str.empty() )
			return def;
		return (float)atof( str.c_str() );
	}

private:
	std::string m_fullPath;
};


class ProfileReaderW
{
public:
	ProfileReaderW(std::wstring file)
	{
		wchar_t buf[MAX_PATH];
		GetFullPathNameW(file.c_str(), MAX_PATH, buf, nullptr);
		m_fullPath = buf;
	}

	std::wstring GetString(std::wstring sectionHeader, std::wstring key, std::wstring def = L"")
	{
		const int maxString = 512;
		wchar_t buffer[maxString];
		memset(buffer, 0, sizeof(buffer));

		int ret = GetPrivateProfileStringW(
			sectionHeader.c_str(),
			key.c_str(),
			def.c_str(),
			buffer,
			maxString,
			m_fullPath.c_str()
			);
		return buffer;
	}

	int GetInteger(std::wstring sectionHeader, std::wstring key, int def = 0)
	{
		std::wstring str = GetString(sectionHeader, key);
		if( str.empty() )
			return def;
		return _wtoi( str.c_str() );
	}

	float GetFloat(std::wstring sectionHeader, std::wstring key, float def = 0.0f)
	{
		std::wstring str = GetString(sectionHeader, key);
		if( str.empty() )
			return def;
		return (float)_wtof( str.c_str() );
	}

private:
	std::wstring m_fullPath;
};


/*
struct ProfileRecord
{
	std::string key;
	std::vector<std::string> values;
};


struct ProfileSection
{
	typedef std::shared_ptr<ProfileRecord> RecordPtr;
	typedef std::map<std::string, RecordPtr> RecordSet;

	std::string header;
	RecordSet records;
};


class ProfileData
{
public:
	std::string GetString(std::string sectionHeader, std::string key, int valueIndex = 0, std::string def = "")
	{
		SectionSet::iterator s = sections.find( sectionHeader );
		if( s != sections.end() )
		{
			SectionPtr section = s->second;
			ProfileSection::RecordSet::iterator r = section->records.find( key );

			if( r != section->records.end() )
			{
				ProfileSection::RecordPtr record = r->second;
				if( valueIndex >= 0 && valueIndex < record->values.size() )
					return record->values[valueIndex];
			}
		}
		return def;
	}

private:
	typedef std::shared_ptr<ProfileSection> SectionPtr;
	typedef std::map<std::string, SectionPtr> SectionSet;
	SectionSet sections;
};
*/

#endif //__FILESYSTEM_H__