#pragma once
#include <SimpleMath.h>
#include <fstream>

class DEBUG
{
public:
	DEBUG(void);
	~DEBUG(void);
	void Print(char c[]);
	std::ofstream& operator<<(char c[]);
	std::ofstream& operator<<(int i);
	std::ofstream& operator<<(float f);
	std::ofstream& operator<<(bool b);
	std::ofstream& operator<<(DirectX::SimpleMath::Vector3 v);
	void Endl(int nr);
};

static DEBUG debug;


