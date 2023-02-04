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


void print_stats(vector<double> V, int numObservations)
{
	/*
		1. a function to find the sum of a numeric vector
		2. a function to find the mean of a numeric vector
		3. a function to find the median of a numeric vector
		4. a function to find the range of a numeric vector

	*/

	//Get the sum
	//double V_sum = accumulate(V.begin(), V.end(), 0);
	//2923 11190

	double V_sum = 0;

	for (int i = 1; i < numObservations; i++)
	{
		V_sum += V[i];
	}

	//Get the mean of the vector
	double V_mean = (V_sum) / (V.size());

	//Get the median
	//The middle element would be numObservations-1/2
	//The -1 is there becaus the first line isnt a value
	
	
	//FIRST SORT THE VECTOR/
	sort(V.begin(), V.end());
	//initilize the 'middle' variable

	double V_median = 0;

	if (numObservations - 1 % 2 == 0)
	{
		//its even meaning the median is the avg of the two middle numbers
		int mid1 = (numObservations - 1) / 2;
		int mid2 = mid1 + 1;
		//the exact middle would be the lower of the two middle numbers

		V_median = (V[mid1] + V[mid2]) / 2;
	}
	else
	{
		int median_index = ((numObservations - 1) / 2) + 1;
		V_median = V[median_index];
	}


	//a function to find the range of a numeric vector
	//since the vector is already sorted we can do this

	double V_Max = V[numObservations - 1];
	double V_Min = V[1]; //because first value is names

	double V_range = V_Max - V_Min;

	std::cout << "The sum of the numeric vector is " << V_sum << endl;
	std::cout << "The mean of the numeric vector is " << V_mean << endl;
	std::cout << "The median of the numeric vector is " << V_median << endl;
	std::cout << "The range of the numeric vector is " << V_range << endl;

}


double covar(vector<double> rm, vector<double> medv, int numObservations)
{
	//Copied from my print_stats section, cause we need mean
	double rm_sum = 0;

	for (int i = 1; i < numObservations; i++)
	{
		rm_sum += rm[i];
	}

	double rm_mean = (rm_sum) / (rm.size());

	double medv_sum = 0;

	for (int i = 1; i < numObservations; i++)
	{
		medv_sum += medv[i];
	}

	double medv_mean = (medv_sum) / (medv.size());

	double covariance = 0;
	double temp = 0;

	for (int i = 1; i < numObservations; i++)
	{
		temp = ((rm[i] - rm_mean)*(medv[i] - medv_mean))/(numObservations - 1);

		covariance += temp; //gotta add for each iteration to n

		temp = 0; //restart
	}



	return covariance;
}



//NEED COV TO GET COR
double cor(vector<double> rm, vector<double> medv, double cov, int numObservations)
{
	//Copied from my print_stats section, cause we need mean
	double rm_sum = 0;

	for (int i = 1; i < numObservations; i++)
	{
		rm_sum += rm[i];
	}

	double rm_mean = (rm_sum) / (rm.size());

	double medv_sum = 0;

	for (int i = 1; i < numObservations; i++)
	{
		medv_sum += medv[i];
	}

	double medv_mean = (medv_sum) / (medv.size());


	//im not sure if it gets squared before or after sigma...
	//little sigma of rm
	double std_rm = 0;
	double temp_rm;
	for (int i = 1; i < numObservations; i++)
	{
		temp_rm = rm[i] - rm_mean;
		temp_rm = pow(temp_rm, 2);
		std_rm += temp_rm;
	}
	std_rm = (std_rm / numObservations);
	std_rm = sqrt(std_rm);

	//little sigma of medv
	double std_medv = 0;
	double temp_medv;
	for (int i = 1; i < numObservations; i++)
	{
		temp_medv = medv[i] - medv_mean;
		temp_medv = pow(temp_medv, 2);
		std_medv += temp_medv;
	}
	std_medv = (std_medv / numObservations);
	std_medv = sqrt(std_medv);



	double cor = (cov)/(std_rm * std_medv);

	return cor;
}


int main(int argc, char**) {


	ifstream inFS;
	string line;
	string rm_in, medv_in;
	const int MAX_LEN = 1000;
	vector<double> rm(MAX_LEN);
	vector<double> medv(MAX_LEN);

	std::cout << "Opening file Boston.csv." << endl;

	inFS.open("Boston.csv");
	if (!inFS.is_open())
	{
		std::cout << "Could not open file Boston.csv." << endl;
		return 1; //1 is the indicator
	}

	//inFS stream can now be used like the cin stream
	//The Boston.csv should contain two doubles

	std::cout << "Reading line 1" << endl;

	int numObservations = 0;
	while (inFS.good()) 
	{
		getline(inFS, rm_in, ',');
		getline(inFS, medv_in, '\n');

		//this makes it so that I dont get an abort thing from stof
		if (numObservations > 0)
		{
			rm.at(numObservations) = stof(rm_in);
			medv.at(numObservations) = stof(medv_in);
		}


		numObservations++;
	}

	rm.resize(numObservations);
	medv.resize(numObservations);

	std::cout << "new length: " << rm.size() << endl;

	std::cout << "Closing Boston.csv file" << endl;
	inFS.close();

	std::cout << "Num of Records : " << numObservations << endl;
	
	
	//Part I'm pretty sure I have to do
	
	std::cout << "\nStats for rm" << endl;
	print_stats(rm, numObservations);

	std::cout << "\nStats for medv" << endl;
	print_stats(medv, numObservations);

	std::cout << "\nCovariance = ";
	double cov = covar(rm, medv, numObservations);
	cout << cov << endl;

	std::cout << "\nCorrelation = " << cor(rm, medv, cov, numObservations) << endl;

	std::cout << "\n**Program End**";

	return 0;


}