#pragma once
class HealthBar
{
public:
	HealthBar(int *, int *);
	~HealthBar(void);

	int *HPmax;
	int *HPcurrent;

};