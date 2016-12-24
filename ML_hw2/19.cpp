#include <iostream>
#include <vector>
#include <ctime>
#include <algorithm>
#include <cstdio>
#include <cmath>
#include <fstream>
#define DATASIZE 10
#define ITERATION 5000
#define NOISERATE 0.2
#define DIMENSION 9
using namespace std;

typedef struct Data{
	double x[DIMENSION];
	int y;
}Data;
typedef struct OneDimensionData{
	double x;
	int y;
}OneDimensionData;
typedef struct Hypothesis{
	int s;
	double theta;
}Hypothesis;

bool Compare(OneDimensionData x, OneDimensionData y){
	return x.x < y.x;
}
int Sign(double x){
	if(x <= 0) return -1;
	else return 1;
}

void getData(vector<Data> &data, const char *s){
	Data temp;
	fstream fp(s, ios::in);
	while(!fp.eof()){
		for(int i = 0; i < DIMENSION; i++) 
			fp >> temp.x[i];
		fp >> temp.y;
		data.push_back(temp);
	}
}

void getNoise(vector<Data> &data){
	for(int i = 0; i < DATASIZE; i++){
		double num = rand()%10000/(double)10000;
		if(num < NOISERATE){
			data[i].y *= -1;
		}
	}
}
double getErrorRate(vector<OneDimensionData> data, int s, double theta){
	double error = 0;
	for(int i = 0; i < data.size(); i++){
		if(data[i].y != s * Sign(data[i].x - theta))
			error++;
	}
	return error/(double)data.size();
}
double getBestE_in(vector<Data> data, Hypothesis &h, int &bestDim){
	double minE_in = 1, theta = 0;
	for(int d = 0; d < DIMENSION; d++){
		OneDimensionData temp;
		vector<OneDimensionData> one_data;
		for(int i = 0; i < data.size(); i++){
			temp.x = data[i].x[d];
			temp.y = data[i].y;
			one_data.push_back(temp);
		}
		sort(one_data.begin(), one_data.end(), Compare);

		for(int i = 0; i < data.size(); i++){
			if(i == 0) theta = (-11 + one_data[0].x) / 2;
			else if(i == DATASIZE) theta = (11 + one_data[DATASIZE-1].x) / 2;
			else theta = (one_data[i].x + one_data[i-1].x) / 2;
			if(minE_in > getErrorRate(one_data, 1 ,theta)){
				minE_in = getErrorRate(one_data, 1 ,theta); 
				h.s = 1;
				h.theta = theta;
				bestDim = d;
			}
			else if(minE_in > getErrorRate(one_data, -1, theta)){
				minE_in = getErrorRate(one_data, -1 ,theta);
				h.s = -1;
				h.theta = theta;
				bestDim = d;
			}
		}
	}
	return minE_in;
}
double getE_out(vector<Data> data, Hypothesis h, int d){
	double error = 0;
	for(int i = 0; i < data.size(); i++){
		if(data[i].y != h.s*Sign(data[i].x[d] - h.theta))
			error++;
	}
	return error/(double)data.size();
}

int main(){
	srand((unsigned)time(0));
	fstream outputE_in("E_in.txt", ios::out), outputE_out("E_out.txt", ios::out);
	double E_in = 0, E_out = 0;
	Hypothesis bestH;
	int bestDim = 0;
	vector<Data> train_data, test_data;
	getData(train_data, "hw2_train.dat");
	getData(test_data, "hw2_test.dat");
	E_in = getBestE_in(train_data, bestH, bestDim);
	E_out = getE_out(test_data, bestH, bestDim);
	cout << "Min E_in = " << E_in << endl;
	cout << "Min E_out = " << E_out << endl;

	return 0;
}
