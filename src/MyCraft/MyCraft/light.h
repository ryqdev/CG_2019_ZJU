#pragma once
#include "GL/glew.h"
#include<stdio.h>
#include "game.h"

//不知道参数是什么的话看pointlight的注释

class Light {
protected:
	GLenum mLightIdentifier;
	Light();
public:
	void SetAmbientColor(float r, float g, float b, float a);
	void SetDiffuseColor(float r, float g, float b, float a);
	void SetSpecularColor(float r, float g, float b, float a);
	void Enable();
};

class DirectionLight :public Light {
public:
	DirectionLight(GLenum light);
	void SetPosition(float x, float y, float z);
};

class PointLight :public Light {
public:
	PointLight(GLenum light);//接受什么光源
	void SetPosition(float x, float y, float z);//光源位置
	void SetConstAttenuation(float v);//常数衰减系数
	void SetLinearAttenuation(float v);//线性衰减函数
	void SetQuadricAttenuation(float v);//平方衰减函数
};

class SpotLight :public PointLight {
public:
	SpotLight(GLenum light);
	void SetDirection(float x, float y, float z);
	void SetExponent(float v);
	void SetCutoff(float v);
};

