#include <iostream>
#include <cmath>
#include <fstream>
#include <vector>
using namespace std;
#define LEARN_RATE 0.001
#define ITERATION 2000
#define DIMENSION 20

typedef struct Data{
	double x[DIMENSION+1];
	int y;
}Data;

void readFile(vector<Data> &data, fstream &fs){
	Data temp;
	while(!fs.eof()){
		temp.x[0] = 1;
		for(int i = 1; i < DIMENSION+1; i++)
			fs >> temp.x[i];
		fs >> temp.y;
		data.push_back(temp);
	}
	fs.close();
}
int sign(double x){
	if(x > 0) return 1;
	else return -1;
}
double sigmoid(double x){
	return 1.0 / (1.0 + exp(-x));
}

double vectorMul(double *a, double *b){
	double temp = 0;
	for(int i = 0; i < DIMENSION+1; i++){
		temp += a[i] * b[i];
	}
	return temp;
}

void BGD(double *w, vector<Data> data){
	double gradient[DIMENSION+1] = {0.0};
	for(int i = 0; i < data.size(); i++){
		double temp = sigmoid(-1 * vectorMul(w,data[i].x) * (double)data[i].y);
		for(int j = 0; j < DIMENSION+1; j++){
			gradient[j] += -1.0 * temp * data[i].x[j] * data[i].y; 
		}
	}
	for(int i = 0; i < DIMENSION+1; i++)
		w[i] = w[i] - LEARN_RATE*(gradient[i]/data.size());
}

void logisticRegression(double *w, vector<Data> data){
	for(int i = 0; i < ITERATION; i++){
		BGD(w, data);
	}
}
double evaluate(double *w, vector<Data> data){
	double error = 0;
	for(int i = 0; i < data.size(); i++){
		if(sign(vectorMul(w,data[i].x)) != data[i].y)
			error++;
	}
	return error/data.size();
}
int main(){
	vector<Data> train_data, test_data;
	fstream train_fs("hw3_train.dat"), test_fs("hw3_test.dat");
	if(train_fs.is_open() && test_fs.is_open()){
		readFile(train_data, train_fs);
		readFile(test_data, test_fs);
	}
	else{
		cout << "The file is not exist" << endl;
	}
	double w[DIMENSION+1];
	for(int i = 0; i < DIMENSION+1; i++)
		w[i] = 0;
	logisticRegression(w,train_data);
	cout << "Eout:" << evaluate(w,test_data) << endl;
}
