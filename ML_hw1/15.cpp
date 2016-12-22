#include <iostream>
#include <fstream>
#include <cstdio>
#include <vector>
#define DIMENSION 5
using namespace std;
int n;
struct TrainingData{
	double x[DIMENSION];
	int y;
	int updateNum;
};
int Sign(double *x, double *w){
	double temp = 0;
	for(int i = 0; i < DIMENSION; i++){
		temp += x[i] * w[i];
	}
	if(temp > 0){
		return 1;
	}
	else{
		return -1;
	}
}
int main(){
	vector<TrainingData> trainingdata;
	ifstream input("hw1_15_train.dat.txt", ios::in);
	TrainingData temp;
	while(!input.eof()){
		temp.x[0] = 1;
		for(int i = 1; i < DIMENSION; i++){
			input >> temp.x[i];
		}
		input >> temp.y;
		temp.updateNum = 0;
		trainingdata.push_back(temp);
	}
	n = trainingdata.size();
	input.close();
	double w[DIMENSION] = {0, 0, 0, 0, 0};
	int step = 0, check = 0, index = 0;
	while(check != n){
		int sign = Sign(trainingdata[index].x, w);
		if(sign != trainingdata[index].y){
			for(int i = 0; i < DIMENSION; i++){
				w[i] += trainingdata[index].y * trainingdata[index].x[i];
			}
			step++;
			check = 0;
			trainingdata[index].updateNum++;
		}
		else{
			check++;
		}
		index++;
		index %= n;
	}
	cout << "step: " << step << endl;
	int maxUpdate = 0;
	for(int i = 1; i < n; i++){
		if(trainingdata[i].updateNum > trainingdata[maxUpdate].updateNum){
			maxUpdate = i;
		}
	}
	cout << "The most number of updates: " << maxUpdate << endl;
}

