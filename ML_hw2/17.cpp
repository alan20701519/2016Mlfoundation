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
using namespace std;

typedef struct Data{
	double x;
	int y;
}Data;
typedef struct Hypothesis{
	int s;
	double theta;
}Hypothesis;

bool Compare(Data x, Data y){
	return x.x < y.x;
}
int Sign(double x){
	if(x <= 0) return -1;
	else return 1;
}

void getTrainingData(vector<Data> &data){
	Data temp;
	for(int i = 0; i < DATASIZE; i++){
		temp.x = rand()%20000/(double)10000 - 1;
		temp.y = Sign(temp.x);
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
double getErrorRate(vector<Data> data, int s, double theta){
	double error = 0;
	for(int i = 0; i < DATASIZE; i++){
		if(data[i].y != s * Sign(data[i].x - theta))
			error++;
	}
	return error/(double)DATASIZE;
}
double getBestE_in(vector<Data> data, Hypothesis &h){
	Hypothesis temp;
	double minE_in = 1, theta = 0;
	for(int i = 0; i < DATASIZE+1; i++){
		if(i == 0) theta = (-1 + data[0].x) / 2;
		else if(i == DATASIZE) theta = (1 + data[DATASIZE-1].x) / 2;
		else theta = (data[i].x + data[i-1].x) / 2;
		temp.theta = theta;
		if(minE_in > getErrorRate(data, 1 ,theta)){
			temp.s = 1;	
			minE_in = getErrorRate(data, 1 ,theta); 
			h = temp;
		}
		else if(minE_in > getErrorRate(data, -1, theta)){
			temp.s = -1;
			minE_in = getErrorRate(data, -1 ,theta);
			h = temp;
		}
	}
	return minE_in;
}
double getE_out(Hypothesis h){
	return 0.5 + 0.3 * double(h.s) * (double)(fabs(h.theta) - 1.0);
}

int main(){
	srand((unsigned)time(0));
	fstream outputE_in("E_in.txt", ios::out), outputE_out("E_out.txt", ios::out);
	double E_in = 0, E_out = 0, totalE_in = 0, totalE_out = 0;
	for(int i = 0; i < ITERATION; i++){
		vector<Data> data;
		Hypothesis bestH;
		getTrainingData(data);
		getNoise(data);
		sort(data.begin(), data.end(), Compare);
		E_in = getBestE_in(data, bestH);
		E_out = getE_out(bestH);
		cout << "Iteration:" << i << ", E_in = " << E_in << ", E_out = " << E_out << endl;
		outputE_in << E_in << endl;
		outputE_out << E_out << endl;
		totalE_in += E_in;
		totalE_out += E_out;
	}
	cout << "Averate E_in = " << totalE_in/ITERATION << endl;
	cout << "Averate E_out = " << totalE_out/ITERATION << endl;

	return 0;
}
