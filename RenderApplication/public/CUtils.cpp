#include "CUtils.h"
#include <stdarg.h>
#include <fstream>
#include <sstream>
#include <chrono>
#include <ctime>
#include <Windows.h>

void CUtils::PrintLog(const char* file, const char* func, int line, const char* fmt, ...)
{
    // 格式化用户消息
    va_list args;
    va_start(args, fmt);
    fprintf(stdout, "[%s] %s:%s(%d)", GetCurrentDateTime().c_str(), file, func, line);
    vfprintf(stdout, fmt, args);
    fprintf(stdout, "\n");
    va_end(args);
}

std::string CUtils::GetCurrentDateTime()
{
    // 1. 获取当前系统时间点
    auto now = std::chrono::system_clock::now();

    // 2. 转换为 time_t (秒级时间戳)
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);

    // 3. 转换为本地时间结构体 (std::tm)
    std::tm bt{};

#if defined(_MSC_VER) // Windows (MSVC)
    localtime_s(&bt, &now_c);
#else // Linux / Unix / macOS (GCC/Clang)
    localtime_r(&now_c, &bt);
#endif

    // 4. 格式化为字符串
    char buf[64];
    // %Y-%m-%d %H:%M:%S 对应：年-月-日 时:分:秒
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &bt);

    return std::string(buf);
}

std::string CUtils::GetPathParentPath(const std::string& strPath)
{
    std::string strPathTemp = strPath;
    size_t nPos = strPathTemp.find_last_of("/\\");
    if (strPathTemp.npos != nPos) {
        strPathTemp = strPathTemp.substr(0, nPos);
    }
    return strPathTemp;
}

std::string CUtils::GetPathFileName(const std::string& strPath)
{
    std::string strName = strPath;
    size_t nPos = strPath.find_last_of("\\/");
    if (strPath.npos != nPos) {
        strName = strPath.substr(nPos + 1);
    }
    return strName;
}

std::string CUtils::GetSolutionPathDir()
{
    return (std::string(SOLUTION_DIR_PATH) + "\\");
}

std::string CUtils::GetProjectPathDir()
{
    return (std::string(PROJECT_DIR_PATH) + "\\");
}

std::string CUtils::GetRootPathDir()
{
    return GetSolutionPathDir();
}

std::string CUtils::GetResourcePathDir()
{
    return GetRootPathDir() + "resources/";
}

std::string CUtils::GetModelPathDir()
{
    return GetResourcePathDir() + "objects/";
}

std::string CUtils::GetImagePathDir()
{
    return GetResourcePathDir() + "image/";
}

std::string CUtils::GetShaderPathDir()
{
    return GetResourcePathDir() + "shader/";
}

std::string CUtils::GetImagePathFile(const std::string& strName)
{
    return GetImagePathDir() + strName;
}

std::string CUtils::GetModelPathFile(const std::string& strName)
{
    return GetModelPathDir() + strName;
}

std::string CUtils::GetShaderPathFile(const std::string& strName)
{
    return GetShaderPathDir() + strName;
}

std::string CUtils::GetGraphvizPathDir()
{
    return GetProjectPathDir() + "Graphviz/";
}

std::string CUtils::GetGraphvizTemplatePathFile(const std::string& strName)
{
    return GetGraphvizPathDir() + strName;
}

bool CUtils::DirExists(const char* path)
{
    DWORD ftyp = GetFileAttributesA(path);
    if (ftyp == INVALID_FILE_ATTRIBUTES)
        return false;  //something is wrong with your path!

    if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
        return true;   // this is a directory!

    return false;    // this is not a directory!
}

bool CUtils::FileExists(const char* path)
{
    DWORD dwAttrib = GetFileAttributesA(path);
    // 如果函数失败或路径无效
    if (dwAttrib == INVALID_FILE_ATTRIBUTES) {
        return false;
    }
    // 检查是否是有效的文件系统对象（不是无效句柄等）
    // 这里我们只关心文件是否存在，不区分文件和目录
    return true;
}

std::string CUtils::RendFile(const char* path)
{
    std::ifstream file(path);

    // 2. 异常检查：判断文件流状态是否正常
    if (!file.is_open()) {
        PRINTLOG("Fail to open file(%s)", path);
        return "";
    }

    // 3. 将文件缓冲区读入字符串流（最快且保留所有原始字符，包括换行符）
    std::stringstream buffer;
    buffer << file.rdbuf();

    return buffer.str();
}

CUtils::S_RECT CUtils::GetScreenSize()
{
    S_RECT Rect;
    Rect.width = GetSystemMetrics(SM_CXSCREEN);
    Rect.height = GetSystemMetrics(SM_CYSCREEN);
    return Rect;
}

std::string CUtils::UintToString(unsigned int value)
{
    char szBuf[20] = { 0 };
    sprintf(szBuf, "%u", value);
    return szBuf;
}

std::string CUtils::IntToString(int value)
{
    char szBuf[20] = { 0 };
    sprintf(szBuf, "%d", value);
    return szBuf;
}

std::string CUtils::FloatToString(float value)
{
    char szBuf[50] = { 0 };
    sprintf(szBuf, "%f", value);
    return szBuf;
}