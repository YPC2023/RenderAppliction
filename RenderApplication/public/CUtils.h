#pragma once

#include <istream>
#include <cassert>

typedef unsigned int	uint;
typedef unsigned short	ushort;
typedef unsigned char	uchar;
typedef unsigned __int64 uint64;

#define PRINTLOG(fmt, ...)      CUtils::PrintLog(__FILE__, __FUNCTION__, __LINE__, fmt, ##__VA_ARGS__);
#define ASSERT(condition, ...) assert((condition) && ("" __VA_ARGS__))

class CUtils
{
public:
	typedef struct _S_RECT
	{
		int width = 0;
		int height = 0;
	}S_RECT;
public:
	static void PrintLog(const char* file, const char* func, int line, const char* fmt, ...);
	static std::string GetPathParentPath(const std::string& strPath);
	static std::string GetSolutionPathDir();
	static std::string GetProjectPathDir();
	static std::string GetRootPathDir();
	static std::string GetResourcePathDir();
	static std::string GetModelPathDir();
	static std::string GetImagePathDir();
	static std::string GetShaderPathDir();
	static std::string GetImagePathFile(const std::string& strName);
	static std::string GetModelPathFile(const std::string& strName);
	static std::string GetShaderPathFile(const std::string& strName);
	static std::string GetGraphvizPathDir();
	static std::string GetGraphvizTemplatePathFile(const std::string& strName);
	static bool DirExists(const char* path);
	static bool FileExists(const char* path);
	static std::string RendFile(const char* path);
	static S_RECT GetScreenSize();
	static std::string UintToString(unsigned int value);
	static std::string IntToString(int value);
};

