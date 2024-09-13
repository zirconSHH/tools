#include <iostream>
#include <cmath>
#include <vector>

#include "headphone_calculater.h"

using namespace std;

double sensitivity_Vrms_to_mW(double vrms, double impendence)
{
	return vrms + 10 * log10(impendence / 1000);
}

void convert_Vrms()
{
	double vrms, impendence;
	cout << "input sensitivity(Vrms), impendence(ohm)" << endl;
	cin >> vrms >> impendence;
	cout << sensitivity_Vrms_to_mW(vrms, impendence) << "(mW)" << endl;
}

double sensitivity_mW_to_Vrms(double mw, double impendence)
{
	return mw - 10 * log10(impendence / 1000);
}

void convert_mW()
{
	double mw, impendence;
	cout << "input sensitivity(mW), impendence(ohm)" << endl;
	cin >> mw >> impendence;
	cout << sensitivity_mW_to_Vrms(mw, impendence) << "(Vrms)" << endl;
}

double max_output_power(double standardPower, double standardImpendence, double outPutImpendence)
{
	return standardPower * standardImpendence / outPutImpendence;
}

double max_output_loudness(double maxPower, double sensitivity)
{
	return sensitivity + 10 * log10(maxPower);
}

void get_max_loudness()
{
	int type;
	cout << "choose sentivity type, 1:dB/mW, 2:dB/Vrms" << endl;
	cin >> type;

	double sensitivity, impendence;
	cout << "input sensitity, impendence(ohm) of the headphone" << endl;
	cin >> sensitivity >> impendence;
	if (type == 2)
	{
		sensitivity = sensitivity_Vrms_to_mW(sensitivity, impendence);
	}
	else if (type > 2)
	{
		cout << "undefined parameter" << endl;
		return;
	}

	double standardPower, standardImpendence;
	cout << "input power(mW), impendence(ohm) of the player" << endl;
	cin >> standardPower >> standardImpendence;

	cout << "max loudness is " << max_output_loudness(max_output_power(standardPower,
		standardImpendence, impendence), sensitivity) << endl;
}

vector<function_entry> function_list = {
	{"convert sensitivity Vrms -> mW", convert_Vrms},
	{"convert sensitivity mW -> Vrms", convert_mW},
	{"calculate max loudness of pair of headphone & player", get_max_loudness},
};

int main()
{
	while (true)
	{
		cout << "choose a function" << endl;
		for (int funcIdex = 0; funcIdex < function_list.size(); funcIdex++)
		{
			cout << funcIdex << " " << function_list[funcIdex].discription << endl;
		}

		int funNum;
		cin >> funNum;
		if (funNum > function_list.size() - 1)
		{
			cout << "quit" << endl;
			system("pause");
			return 0;
		}
		else
		{
			function_list[funNum].function();
		}
	}

	cout << "loudness " << max_output_loudness(max_output_power(165, 32, 8), 80.04) << " dB" << endl;
}