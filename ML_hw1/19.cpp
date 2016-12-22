#include <iostream>
#include <fstream>
#include <cstdio>
#include <vector>
#include <algorithm>
#define DIMENSION 5
#define TIMES 2000
#define ITERATION 100
using namespace std;
struct Data{
	double x[DIMENSION];
	int y;
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
double ErrorRate(double *w, vector<Data> t){
	double count = 0;
	for(int i = 0; i < t.size(); i++){
		if(Sign(t[i].x, w) != t[i].y){
			count++;
		}
	}
	return count/t.size();
}
int main(){
	vector<Data> trainingdata, testingdata;
	fstream input_training("hw1_18_train.dat.txt", ios::in);
	fstream input_testing("hw1_18_test.dat.txt", ios::in);
	Data temp;
	while(!input_training.eof()){
		temp.x[0] = 1;
		for(int i = 1; i < DIMENSION; i++){
			input_training >> temp.x[i];
		}
		input_training >> temp.y;
		trainingdata.push_back(temp);
	}
	input_training.close();
	while(!input_testing.eof()){
		temp.x[0] = 1;
		for(int i = 1; i < DIMENSION; i++){
			input_testing >> temp.x[i];
		}
		input_testing >> temp.y;
		testingdata.push_back(temp);
	}
	input_testing.close();
	double total_err = 0;
	fstream output("result_19.txt", ios:: out);
	for(int t = 0; t < TIMES; t++) { //for run the PLA 2000 times
		random_shuffle(trainingdata.begin(), trainingdata.end());
		double w[DIMENSION] = {0.0, 0.0, 0.0, 0.0, 0.0}, p[DIMENSION] = {0, 0, 0, 0, 0};
		int step = 0, index = 0;
		while(step != ITERATION){
			int sign = Sign(trainingdata[index].x, w);
			if(sign != trainingdata[index].y){
				for(int i = 0; i < DIMENSION; i++){
					w[i] += trainingdata[index].y * trainingdata[index].x[i];	
				}
				if(ErrorRate(w, trainingdata) < ErrorRate(p, trainingdata)){ //update the weight
					for(int i = 0; i < DIMENSION; i++){
						p[i] = w[i];
					}
				}
				step++;
			}
			index++;
			index %= trainingdata.size();
		}
		double testing_err = ErrorRate(p, testingdata);
		cout << "Iteration:" << t << " ,err_rate: " << testing_err << endl;
		total_err += testing_err;
		output << testing_err << endl;
	}
	output.close();
	cout << "The average error rate:" << total_err/TIMES << endl;

}

