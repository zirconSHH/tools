#pragma once
#include <iostream>
#include <string>

using namespace std;

struct function_entry {
	string discription;
	void (*function)(void);
};

enum sensitivity_type
{
	mW = 1,
	Vrms = 2,
};

double sensitivity_Vrms_to_mW(double vrms, double impendence);

double sensitivity_mW_to_Vrms(double mw, double impendence);

double max_output_power(double standardPower, double standardImpendence, double outPutImpendence);

double max_output_loudness(double maxPower, double sensitivity);