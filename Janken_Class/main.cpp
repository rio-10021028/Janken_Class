#include <iostream>
#include "janken.h"
using namespace std;

enum input
{
	YES,
	NO
};

int main()
{
	int input;
	bool inputFlag = true;

	while (true)
	{
		jankenSimurator();

		cout << "--------------------------\n\n";

		// 繰り返すか確認
		while (inputFlag)
		{
			cout << "yes > 0, no > 1" << endl
				<< "もう一度？ > " << flush;
			cin >> input;

			switch (input)
			{
			case YES:
				inputFlag = false;
				cout << "---------------------------\n";
				break;
			case NO:
				cout << "またね！" << endl;
				return 0;
			default:
				cout << "0, 1 で入力してください\n\n";
			}
		}

		inputFlag = true;
	}
}