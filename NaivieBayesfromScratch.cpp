//Marcela Pantoja
//CS 4375
//2/4/2023
/*

I'm not sure if I was supposed to, but  I used the built
in sort function to sort the vector to get the median and range.

I didnt see any mention of not using functions on the homework so I hope its ok.

*/

#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <vector>
#include<numeric>
#include<algorithm>
#include <math.h>

using namespace std;

/*
Repeat the above but implement Naïve Bayes using predictors age, pclass, and sex to
predict survival on the Titanic data. Use the first 800 observations for train, the rest for test.
*/

double calc_age_lh(double v, double mean_v, double var_v)
{
	// run like this: calc_age_lh(6, 25.9, 138)

	double age_lh = (1 / sqrt(2 * 3.14 * var_v) * exp(-(pow((v - mean_v), 2))/(2* var_v)));
		
		//(1 / sqrt(2 * pi * var_v) * exp(-((v - mean_v) ^ 2) / (2 * var_v));
	

	return age_lh;

}

void calcu_raw_prob(int pclass, int sex, int age, double lh_pclass[2][3], double lh_sex[2][2], double apriori[2], double  age_var[2], double  age_mean[2])
{
	//So suposedly to calc the proboability of age I need a function

	double num_s = lh_pclass[1][pclass] * lh_sex[1][sex] * apriori[2] * calc_age_lh(age, age_mean[1], age_var[1]);

	double num_p = lh_pclass[0][pclass] * lh_sex[0][sex] * apriori[0] *
		calc_age_lh(age, age_mean[0], age_var[0]);

	double denominator = lh_pclass[1][pclass] * lh_sex[1][sex] *
		calc_age_lh(age, age_mean[1], age_var[1]) * apriori[1] +
		lh_pclass[0][pclass] * lh_sex[0][sex] *
		calc_age_lh(age, age_mean[0], age_var[0]) * apriori[0];

	double prob_survived = num_s / denominator;

	double prob_perished = num_p / denominator;

	cout << "\n\n\nProbability survived: " << prob_survived;

	cout << "\nProbability dead: " << prob_perished;
}

int main(int argc, char**)
{
	//Naivie Bayes

	ifstream inFS;
	string line;
	string num_in, pclass_in, survived_in, sex_in, age_in;
	const int MAX_LEN = 2000;
	vector<string> num(MAX_LEN);
	vector<double> pclass(MAX_LEN);
	vector<string> survived(MAX_LEN);
	vector<string> sex(MAX_LEN);
	vector<double> age(MAX_LEN);

	//this woul be my test double array
	int test[247][5];

	std::cout << "Opening file titanic_project.csv." << endl;

	inFS.open("titanic_project.csv");
	if (!inFS.is_open())
	{
		std::cout << "Could not open file titanic_project.csv." << endl;
		return 1; //1 is the indicator
	}

	//inFS stream can now be used like the cin stream
	//The Boston.csv should contain two doubles

	std::cout << "Reading line 1" << endl;

	int numObservations = 0;
	while (inFS.good())
	{
		getline(inFS, num_in, ',');
		getline(inFS, pclass_in, ',');
		getline(inFS, survived_in, ',');
		getline(inFS, sex_in, ',');
		getline(inFS, age_in, '\n');

		//this makes it so that I dont get an abort thing from stof
		if (numObservations > 0)
		{
			num.at(numObservations) = string(num_in);
			pclass.at(numObservations) = stof(pclass_in);
			survived.at(numObservations) = string(survived_in);
			sex.at(numObservations) = string(sex_in);
			age.at(numObservations) = stof(age_in);
		}


		numObservations++;
	}

	num.resize(numObservations);
	pclass.resize(numObservations);
	survived.resize(numObservations);
	sex.resize(numObservations);
	age.resize(numObservations);

	std::cout << "new length: " << num.size() << endl;

	std::cout << "Closing titanic_project.csv file" << endl;
	inFS.close();

	std::cout << "Num of Records : " << numObservations << endl << endl;




	double Totaldied = 0, Totalsurvived = 0;
	int c1s0 = 0, c2s0 = 0, c3s0 = 0, c1s1 = 0, c2s1 = 0, c3s1 = 0;
	int FS = 0, FD = 0, MS = 0, MD = 0;

	int count = 0;

	do
	{

		if (survived[count].compare("0"))
		{
			Totaldied++;

			if (pclass[count] == 1)
			{
				c1s0++;
			}
			if (pclass[count] == 2)
			{
				c2s0++;
			}
			if (pclass[count] == 3)
			{
				c3s0++;
			}

			if (sex[count] == "1")
			{
				FD++;
			}
			if (sex[count] == "2")
			{
				MD++;
			}

		}
		else if (survived[count].compare("1"))
		{
			Totalsurvived++;

			if (pclass[count] == 1)
			{
				c1s1++;
			}
			if (pclass[count] == 2)
			{
				c2s1++;
			}
			else if (pclass[count] == 3)
			{
				c3s1++;
			}

			if (sex[count] == "1")
			{
				FS++;
			}
			if (sex[count] == "2")
			{
				MS++;
			}
		}

		count++;
	} while (count < numObservations);

	double apriori[2];

	double tempo = (Totalsurvived / numObservations);
	apriori[0] = tempo;
	tempo = (Totaldied / numObservations);
	apriori[1] = tempo;


	cout << "\nPrior probability\nSurvived= " << apriori[0];
	cout << "\nDead = " << apriori[1];

	//Have total counts for survived and died

	//Conditional Probability for Discrete Data

	//.........................

	double  age_mean[2];
	double  age_var[2];

	//Getting the mean age for survived and died

	double V_sum = 0;
	double age_survived_mean = 0;
	double age_died_mean = 0;
	count = 0;


	do
	{
		if (survived[count].compare("0"))
		{
			age_died_mean += age[count];
		}
		else if (survived[count].compare("1"))
		{
			age_survived_mean += age[count];
		}

		count++;

	} while (count < numObservations);

	//Get the mean of the vector
	age_died_mean = age_died_mean / Totaldied;
	age_survived_mean = age_survived_mean / Totalsurvived;

	age_mean[0] = age_died_mean;
	age_mean[1] = age_survived_mean;

	cout << "\nCount Total Survived: " << Totalsurvived;
	cout << "\nCount Total dead: " << Totaldied << "\n";

	cout << "\n\nMean of age dead: " << age_died_mean;
	cout << "\nMean of age survived: " << age_survived_mean << endl;

	//get rhe variance of vector

	//survived
	double age_survived_var = 0;
	double age_died_var = 0;
	double temp = 0;

	count = 0;

	do
	{
		if (survived[count].compare("0"))
		{
			temp = pow((age[count] - age_died_mean), 2);// / (Totaldied - 1);

			age_died_var += temp; //gotta add for each iteration to n

			temp = 0; //restart
		}
		else if (survived[count].compare("1"))
		{
			temp = pow((age[count] - age_survived_mean), 2);// / (Totalsurvived - 1);

			age_survived_var += temp; //gotta add for each iteration to n

			temp = 0; //restart
		}

		count++;

	} while (count < numObservations);

	age_died_var = age_died_var / (Totaldied - 1);
	age_survived_var = age_survived_var / (Totalsurvived - 1);

	age_var[0] = age_died_var;
	age_var[1] = age_survived_var;

	cout << "\n\nVairence of age dead: " << age_died_var;
	cout << " \nVarience of age survived: " << age_survived_var << endl;




	double lh_pclass[2][3];

	lh_pclass[0][0] = c1s1 / Totalsurvived;
	lh_pclass[0][1] = c2s1 / Totalsurvived;
	lh_pclass[0][2] = c3s1 / Totalsurvived;
	lh_pclass[1][0] = c1s0 / Totaldied;
	lh_pclass[1][1] = c2s0 / Totaldied;
	lh_pclass[1][2] = c3s0 / Totaldied;

	cout << "\n\nLIKELIHOOD OF SURVIVAL BASED ON CLASS**\n";


	cout << lh_pclass[0][0] << " " << lh_pclass[0][1] << " " << lh_pclass[0][2] << "\n\n";
	cout << lh_pclass[1][0] << " " << lh_pclass[1][1] << " " << lh_pclass[1][2] << "\n\n";


	double lh_sex[2][2];

	lh_sex[0][0] = FS / Totalsurvived;
	lh_sex[0][1] = MS / Totalsurvived;
	lh_sex[1][0] = FD / Totaldied;
	lh_sex[1][1] = MD / Totaldied;

	cout << "\n\nLIKELIHOOD OF SURVIVAL BASED ON SEX**\n";

	cout << lh_sex[0][0] << " " << lh_sex[0][1] << "\n\n";
	cout << lh_sex[1][0] << " " << lh_sex[1][1] << "\n\n";



	//So suposedly to calc the proboability of age I need a function



	//void calcu_raw_prob(int pclass, int sex, int age, double lh_pclass[2][3], 
	//double lh_sex[2][2], double apriori[2], double  age_var[2], double  age_mean[2])
	cout << "\n**I've applied everything to the first 5 test observations**";

	for (int i = 800; i < 805; i++)
	{
	
		if (sex[i] == "0")
		{
			calcu_raw_prob(pclass.at(i), 0, age.at(i), lh_pclass, lh_sex, apriori, age_var, age_mean);
		}
		else if (sex[i] == "1")
		{
			calcu_raw_prob(pclass.at(i), 1, age.at(i), lh_pclass, lh_sex, apriori, age_var, age_mean);
		}
		
	}

	cout << "\n\n";
		//cout << calcu_raw_prob(test[i, 1], test[i, 3]), test[i, 4])

}
