#include "perlinNoise.h"
#include <random>
#include <algorithm> 

PerlinNoise::PerlinNoise()
{
    reseed(0);
}

PerlinNoise::PerlinNoise(unsigned int seed)
{
    reseed(seed);
}

double PerlinNoise::noise(double x, double y)
{
    return noise(x, y, 0.0);
}

double PerlinNoise::noise(double x, double y, double z)
{
    // 输入点所在的单位立方体的位置
    int xi = (int)floor(x) & 255;
    int yi = (int)floor(y) & 255;
    int zi = (int)floor(z) & 255;

    // 输入在单位立方体内的位置
    double xf = x - floor(x);
    double yf = y - floor(y);
    double zf = z - floor(z);

    // 计算出线性插值过程中要用到的参数
    double u = fade(xf);
    double v = fade(yf);
    double w = fade(zf);

    // 得到hash值
    int A = p[xi] + yi, AA = p[A] + zi, AB = p[A + 1] + zi;
    int B = p[xi + 1] + yi, BA = p[B] + zi, BB = p[B + 1] + zi;

    // 线性插值
    double x1, x2, x3, x4;
    x1 = lerp(u, grad(p[AA], xf, yf, zf), grad(p[BA], xf - 1, yf, zf));
    x2 = lerp(u, grad(p[AB], xf, yf - 1, zf), grad(p[BB], xf - 1, yf - 1, zf));

    x3 = lerp(u, grad(p[AA + 1], xf, yf, zf - 1), grad(p[BA + 1], xf - 1, yf, zf - 1));
    x4 = lerp(u, grad(p[AB + 1], xf, yf - 1, zf - 1), grad(p[BB + 1], xf - 1, yf - 1, zf - 1));

    return lerp(w, lerp(v, x1, x2), lerp(v, x3, x4));
}

double PerlinNoise::octaveNoise(double x, double y, int octaves, double persistence)
{
    double total = 0;
    double frequency = 1;
    double amplitude = 1;
    double maxValue = 0;

    for (int i = 0; i < octaves; i++) {
        total += noise(x * frequency, y * frequency) * amplitude;

        frequency *= 2;
        amplitude *= persistence;
        maxValue += amplitude;
    }

    return (1 + total / maxValue) / 2;
}

// 根据seed生成p数组
void PerlinNoise::reseed(unsigned int seed)
{
    for (int i = 0; i < 256; i++) {
        p[i] = i;
    }

    std::shuffle(std::begin(p), std::begin(p) + 256, 
        std::default_random_engine(seed));

    for (int i = 0; i < 256; i++) {
        p[i + 256] = p[i];
    }
}

// 使用fade函数插值
double PerlinNoise::fade(double t)
{
    return t * t * t * (t * (t * 6 - 15) + 10);
}

// 线性插值
double PerlinNoise::lerp(double t, double a, double b)
{
    return a + t * (b - a);
}

double PerlinNoise::grad(unsigned char hash, double x, double y, double z)
{
    switch (hash & 0xf) {
    case 0x0: return  x + y;
    case 0x1: return -x + y;
    case 0x2: return  x - y;
    case 0x3: return -x - y;
    case 0x4: return  x + z;
    case 0x5: return -x + z;
    case 0x6: return  x - z;
    case 0x7: return -x - z;
    case 0x8: return  y + z;
    case 0x9: return -y + z;
    case 0xA: return  y - z;
    case 0xB: return -y - z;
    case 0xC: return  y + x;
    case 0xD: return -y + z;
    case 0xE: return  y - x;
    case 0xF: return -y - z;
    }
}