#pragma once

#include <random>

class Random
{
public:
	static void Init()
	{
		s_RandomEngine.seed(std::random_device()());		// 用硬件熵源初始化种子
	}

	static float Float()
	{
		return s_Distribution(s_RandomEngine);				// 直接生成 [0,1) 之间的浮点数
	}

	static float Float(float min, float max)
	{
		std::uniform_real_distribution<float> distribution(min, max);
		return distribution(s_RandomEngine);
	}

private:
	static std::mt19937 s_RandomEngine;		// 伪随机数引擎
	static std::uniform_real_distribution<float>  s_Distribution;
};