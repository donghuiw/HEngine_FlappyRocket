#pragma once

#include <random>

class Random
{
public:
	static void Init()
	{
		s_RandomEngine.seed(std::random_device()());		// ��Ӳ����Դ��ʼ������
	}

	static float Float()
	{
		return s_Distribution(s_RandomEngine);				// ֱ������ [0,1) ֮��ĸ�����
	}

	static float Float(float min, float max)
	{
		std::uniform_real_distribution<float> distribution(min, max);
		return distribution(s_RandomEngine);
	}

private:
	static std::mt19937 s_RandomEngine;		// α���������
	static std::uniform_real_distribution<float>  s_Distribution;
};