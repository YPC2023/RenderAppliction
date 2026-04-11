#include "ColorGenerator.h"


const ColorGenerator& ColorGenerator::GetInstance()
{
    static ColorGenerator Instance(0.8, 0.8, 0.0);

    return Instance;
}

// 构造函数
    // sat: 饱和度 (0-1)，推荐 0.7-0.9
    // val: 明度 (0-1)，推荐 0.6-0.9
    // offset: 起始色相偏移
ColorGenerator::ColorGenerator(double sat, double val, double offset)
    : hueOffset(offset), saturation(sat), value(val) 
{
}

// 根据类型生成颜色 - 使用黄金角法（最大区分度）
ColorGenerator::RGBColor ColorGenerator::GenerateByType(int64_t type) const
{
    // 使用黄金角原理：每次增加 137.5度（360 * (1 - 1/黄金比例)）
    // 这样可以保证颜色在色环上均匀分布
    double hue = fmod(type * (1.0 / GOLDEN_RATIO) + hueOffset, 1.0);

    // 确保色相在 [0, 1) 范围内
    if (hue < 0) hue += 1.0;

    return HsvToRgb(hue, saturation, value);
}

// 备选方法：使用哈希函数混合，避免规律性
ColorGenerator::RGBColor ColorGenerator::GenerateByTypeHash(int64_t type) const
{
    // 简单但有效的哈希函数
    uint64_t hash = type;
    hash = (hash ^ 61) ^ (hash >> 16);
    hash = hash + (hash << 3);
    hash = hash ^ (hash >> 4);
    hash = hash * 0x27d4eb2d;
    hash = hash ^ (hash >> 15);

    // 使用哈希值的高位作为色相
    double hue = (hash & 0xFFFFFFFF) / 4294967296.0;

    return HsvToRgb(hue, saturation, value);
}

// 针对小范围类型（如 0-20）的优化版本
ColorGenerator::RGBColor ColorGenerator::GenerateForSmallRange(int64_t type) const
{
    // 使用固定偏移，避免相邻类型颜色相似
    double hue = fmod(type * 0.168 + hueOffset, 1.0);
    // 交替使用不同的饱和度，增加区分度
    double sat = saturation + (type % 3 - 1) * 0.1;
    sat = std::max(0.5, std::min(0.95, sat));

    return HsvToRgb(hue, sat, value);
}

// HSV 转 RGB
ColorGenerator::RGBColor ColorGenerator::HsvToRgb(double h, double s, double v) const 
{
    double r, g, b;

    int i = (int)(h * 6);
    double f = h * 6 - i;
    double p = v * (1 - s);
    double q = v * (1 - f * s);
    double t = v * (1 - (1 - f) * s);

    switch (i % 6) {
    case 0: r = v; g = t; b = p; break;
    case 1: r = q; g = v; b = p; break;
    case 2: r = p; g = v; b = t; break;
    case 3: r = p; g = q; b = v; break;
    case 4: r = t; g = p; b = v; break;
    default: r = v; g = p; b = q; break;
    }

    return RGBColor{
        (uint8_t)(r * 255),
        (uint8_t)(g * 255),
        (uint8_t)(b * 255)
    };
}