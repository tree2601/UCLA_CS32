using namespace std;
#include<iostream>
#include"History.h"
#include"globals.h"



History::History(int nRows, int nCols) {
	Rows = nRows;
	Cols = nCols;

	for (int i = 0; i <nRows; i++) {
		for (int j = 0; j < nCols; j++) {
			graph[i][j] = '.';
		}
	}
}


bool History::record(int r, int c) {
	if (r>0&&r <= Rows && c>0&&c <=Cols) {
		if (graph[r - 1][c - 1] >= 'A'&&graph[r - 1][c - 1] < 'Z') {
			graph[r - 1][c - 1]++;
		}
		else {
			graph[r - 1][c - 1] = 'A';
		}
			
		return true;
	}
		return false;
}

void History::display() const {
	void clearScreen();

	for (int k = 0; k < Rows; k++) {
		for (int g = 0; g < Cols; g++) {
			cout << graph[k][g];
		}
		cout << endl;
	}
	cout << endl;
}