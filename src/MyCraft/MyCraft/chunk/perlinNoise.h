/*
* 噪声，参考：
* https://mrl.nyu.edu/~perlin/noise/
* http://flafla2.github.io/2014/08/09/perlinnoise.html
*/

#pragma once

#include <random>

class PerlinNoise
{
private:
    unsigned char p[512];

    // 使用fade函数插值
    double fade(double t);

    // 线性插值
    double lerp(double t, double a, double b);

    // 根据hash选取梯度向量，并返回与距离向量(x, y, z)的点积
    double grad(unsigned char hash, double x, double y, double z);

public:
    PerlinNoise();
    PerlinNoise(unsigned int seed);

    double noise(double x, double y);
    double noise(double x, double y, double z);
    double octaveNoise(double x, double y, int octaves, double persistence);

    // 根据seed生成p数组
    void reseed(unsigned int seed);
};

