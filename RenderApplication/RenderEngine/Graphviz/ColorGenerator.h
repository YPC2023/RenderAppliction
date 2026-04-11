#pragma once
#include <string>

class ColorGenerator
{
public:
    // RGB颜色结构体
    struct RGBColor 
    {
        unsigned char r;
        unsigned char g;
        unsigned char b;

        // 转换为HTML颜色代码
        std::string toHex() const 
        {
            char hex[8];
            sprintf(hex, "#%02X%02X%02X", r, g, b);
            return std::string(hex);
        }

        // 转换为CSS rgb格式
        std::string toRGB() const 
        {
            char rgb[32];
            sprintf(rgb, "rgb(%d, %d, %d)", r, g, b);
            return std::string(rgb);
        }
    };
public:
    static const ColorGenerator& GetInstance();
private:
    ColorGenerator(double sat = 0.8, double val = 0.8, double offset = 0.0);
public:
    // 根据类型生成颜色 - 使用黄金角法（最大区分度）
    RGBColor GenerateByType(int64_t type) const;
    // 备选方法：使用哈希函数混合，避免规律性
    RGBColor GenerateByTypeHash(int64_t type) const;
    // 针对小范围类型（如 0-20）的优化版本
    RGBColor GenerateForSmallRange(int64_t type) const;
private:
    // HSV 转 RGB
    RGBColor HsvToRgb(double h, double s, double v) const;

private:
    const double GOLDEN_RATIO = 1.618033988749895;
    double hueOffset;      // 初始偏移
    double saturation;     // 饱和度 (0.7-0.9 保证鲜艳)
    double value;          // 明度 (0.6-0.9 保证亮度)
};