#include "StdAfx.h"
#include "HealthBar.h"


HealthBar::HealthBar(int *pointer1, int *pointer2)
{
	HPmax = pointer1;
	HPcurrent = pointer2;


}


HealthBar::~HealthBar(void)
{
}