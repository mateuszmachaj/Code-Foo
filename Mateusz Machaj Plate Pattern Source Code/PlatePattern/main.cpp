#include <iostream>
#include <math.h>

using namespace std;

int main()
{
	char choice = ' ';

	//main program loop, continues to loop untill user choices to quit
	while(choice != 'n' && choice != 'N')
	{
		//reads in only one character ignores the rest
		cout<<"Find plate pattern for a population? (Y/N): ";
		cin>>choice;
		cin.ignore(1000, '\n');

		if(choice == 'y' || choice == 'Y')
		{
			int population;
			cout<<"Enter population amount: ";
			cin>>population;
			cin.ignore(1000, '\n');//skips other character after the number if any

			//checks if the input was the right type
			while(!cin)
			{
				cout<<"Enter population amount: ";
				cin.clear();
				cin.ignore(1000, '\n');
				cin>>population;
				cin.ignore(1000, '\n');
			}

			if(population > 0)
			{
				double value = population;
				int size = 0;//size of the pattern list
				int countA = 0;//lenght of the first pattern
				int countB = 0;//lenght of the second pattern
				
				//generates the first pattern lenght
				while(value > 1)
				{
					value = value/26;
					countA++;
				}

				value = population;

				//generates the second pattern lenght
				while(value > 1)
				{
					value = value/10;
					countB++;
				}
				
				//just incase
				if(population == 1)
				{
					countA = countB = 1;
				}

				size = countA + countB + 2;

				//using the two pattern lenghts generates all possible patterns
				int (*pattern)[4] = new int[size][4]; 

				for(int i = 0; i < countA + 1; i++)
				{
					pattern[i][0] = countA - i;
					pattern[i][1] = i;
					pattern[i][2] = static_cast<int>(pow(26.0,static_cast<double>(countA - i))*pow(10.0,static_cast<double>(i)));
					pattern[i][3] = pattern[i][2] - population;
				}

				for(int i = 0; i < countB + 1; i++)
				{
					pattern[i + countA + 1][0] = i;
					pattern[i + countA + 1][1] = countB - i;
					pattern[i + countA + 1][2] = static_cast<int>(pow(10.0,static_cast<double>(countB - i))*pow(26.0,static_cast<double>(i)));
					pattern[i + countA + 1][3] = pattern[i + countA + 1][2] - population;
				}

				//chooses the pattern with the least amount of excess plates 
				int lowExcess[4];
				lowExcess[0] = pattern[0][0];
				lowExcess[1] = pattern[0][1];
				lowExcess[2] = pattern[0][2];
				lowExcess[3] = pattern[0][3];
				for(int i = 1; i < size; i++)
				{
					if(lowExcess[3] > pattern[i][3])
					{
						if(pattern[i][3] > -1)
						{
							lowExcess[0] = pattern[i][0];
							lowExcess[1] = pattern[i][1];
							lowExcess[2] = pattern[i][2];
							lowExcess[3] = pattern[i][3];
						}
					}
				}

				//prints out the best results
				cout<<endl;
				cout<<"Population: "<<population<<endl;
				cout<<endl;
				cout<<"Pattern: ";
				if(lowExcess[0] > 0)
				{
					if(lowExcess[0] == 1)
						cout<<"1 Letter ";
					else
						cout<<lowExcess[0]<<" Letters ";
				}
				if(lowExcess[1] > 0)
				{
					if(lowExcess[1] == 1)
						cout<<"1 Number";
					else
						cout<<lowExcess[1]<<" Numbers";
				}
				cout<<endl;
				cout<<endl;
				cout<<"Total Plates: "<<lowExcess[2]<<endl;
				cout<<endl;
				cout<<"Excess Plates: "<<lowExcess[3]<<endl;
				cout<<endl;
			}
		}
	}

	return 0;
}