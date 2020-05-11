#define name mysort
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <assert.h>
#include <iomanip>
#include <vector>
#include <sstream>
#include <algorithm>

using namespace std;

template < class T > void BubbleSort(vector<T> &a) {
	assert(a.size()>0);
	for (size_t i = a.size() - 1; i > 0; i--)
	for (size_t j = 0; j < i; j++)
	if (a[j] > a[j+1])
	swap(a[j],a[j+1]);
	for (size_t i=0; i<a.size()-1; i++) assert(a[i]<=a[i+1]); // is it really sorted?
}

template<typename T> void InsertionSort(vector<T> &a) {
	for(size_t j = 1; j < a.size(); j++) { 
		T key = a.at(j);
		int i = j - 1;
		while(i >= 0 && key < a.at(i)) {
			a.at(i + 1) = a.at(i);
			i--;
		}
		a[i + 1] = key;
	}
}

template < class T > void MergeSort(vector <T> &a){
    vector<double> tempVector;
    int lengthInput = a.size();
    int lengthHelp;
    int leftSite;
    int rightSite;
    int mid;
    int positionRight = 1;
    int positionLeft = 1;
    
    for(lengthHelp = 1; lengthHelp < lengthInput; lengthHelp *= 2){
        for(leftSite = 0; leftSite < lengthHelp - 1; leftSite += 2*lengthHelp){
            if(leftSite + 2*lengthHelp < lengthInput){rightSite =  leftSite + 2*lengthHelp - 1;}
            else{rightSite = lengthInput - 1;}
            
            if(leftSite + 2*lengthHelp > lengthInput){
                if(leftSite + lengthHelp -1 > rightSite){mid = leftSite;}
                else{mid = leftSite + lengthHelp -1;}
            }
        }
        for(int i = 0; i < ((rightSite-leftSite)-1); i++){
            if(positionLeft <= mid && (positionRight > rightSite || a.at(i) <= a.at(positionRight))){
                tempVector.at(i) = a.at(positionLeft);
                positionLeft++;
            }
            else{
                tempVector.at(i) = a.at(positionRight);
                positionRight++;
            }
        }
        for(int i = 0; i < rightSite-leftSite + 1; i++){a.at(leftSite + i) = tempVector.at(i);}
    }
    
}


vector<double> GenereateNumbers(string selection, size_t n) {
	vector<double> numbers(n, 17);
	if(selection != "constant") {
		numbers[0] = 0;
		if(selection == "random") {
			cout << setiosflags(ios::fixed) << setprecision(12);
			srand(time(nullptr));
			for(size_t i = 0; i < n; i++) {
				numbers[i] = ((double) rand() / (RAND_MAX));	
			}
		} else if(selection == "up") {
			for(size_t i = 1; i < n; i++) {
				numbers[i] = numbers[i-1] + 1;
			}
		} else if(selection == "down") {
			for(size_t i = 1; i < n; i++) {
				numbers[i] = numbers[i-1] - 1;
			}
		} else {
			throw "Second Parameter must be chosen from random|up|down|constant";
		}
	}
	return numbers;
}

template < class T> bool IsSortedAndNothingIsLost(vector <T> &Before, vector <T> &After){
	unsigned int beforeLength = Before.size();
	int counter = 0;
	
	if(beforeLength != After.size()){return false;}
	
	for(unsigned int i = 0; i <= beforeLength-1;i++){
		counter++;
		for(unsigned int j = 0; j <= beforeLength-1; j++){
			// if(After.at(i) == After.at(j)){counter++;
			// }
			
			if(After.at(i) == Before.at(j)){
				counter--;
				break;
			}
			if(j == beforeLength-1){return false;}
		}		
		if(counter != 0){return false;}
		if(i < beforeLength - 1){
			if(After.at(i) > After.at(i+1)){return false;}
		}
	}
	return true;
}

int main(int argc, char *argv[]) {
	int count;	// Optionen und Daten
	char* Input = nullptr;
	bool CalculateTime = false; 
	bool Compare = false;
	bool HideOutput = true;
	bool IsCorrectlySorted = false;
	int SortType = 0;	// SortType. 0 = Bubble, 1 = Insertion, 2 = Merge
	
	try{
		if(argc < 3) throw "usage: mysort n Direction [ -o ] [ -c ] [ -t ] [ -m | -i ]";
		if (!(istringstream(argv[1]) >> dec >> count) || count <= 0){
			throw "First Parameter n must be positive integer.";
		} 
		Input = argv[2];
		for (int i = 3; i < argc; i++) { // parse options
			if (!string(argv[i]).compare("-i")) {
				if(SortType) throw "use only one of [ -m | -i ]";
				SortType = 1;
				continue;
			} else if (!string(argv[i]).compare("-m")) {
				if(SortType) throw "use only one of [ -m | -i ]";
				SortType = 2;
				continue;
			} else if (!string(argv[i]).compare("-t")) {
				CalculateTime = true;
				continue;
			} else if (!string(argv[i]).compare("-c")) {
				Compare = true;
				continue;
			} else if (!string(argv[i]).compare("-o")) {
				HideOutput = false;
				continue;
			} else {
				throw "Wrong Switch, use only one of -o|-c|-t|-m|-i";
			}
		}
		vector<double> NumbersToSort = GenereateNumbers(Input, count); 
		vector<double> OriginalNumbers = NumbersToSort;
		
		double MeasuredTime = double(clock());
		if (SortType == 0) {
			cout << "Using BubbleSort" << endl;
			BubbleSort(NumbersToSort);
		} else if(SortType == 1) {
			cout << "Using InsertionSort" << endl;
			InsertionSort(NumbersToSort);
		} else {
			cout << "Using MergeSort" << endl;
			MergeSort(NumbersToSort);
		}
		MeasuredTime = (double(clock()) - MeasuredTime) / CLOCKS_PER_SEC;
		
		if(Compare) {
			IsCorrectlySorted = IsSortedAndNothingIsLost(OriginalNumbers, NumbersToSort);
			if(IsCorrectlySorted) {
				cout << "Success!" << endl;
			} else {
				throw "Check failed."; 
			}
			
		}
		if(!HideOutput) { // Ausgabe der Ergebnisse
			for(size_t i = 0; i < NumbersToSort.size(); i++) { 
				cout << NumbersToSort[i] << "\t" << OriginalNumbers[i] << endl;
			}
		}
		if(CalculateTime) {
			cout << "--- It took " << resetiosflags(ios::fixed);
			if(MeasuredTime < 0.001) cout << "< 0.001"; 
			else cout << MeasuredTime;
			cout << " seconds to compute ---" << endl;
		}
		
	} catch (const char *Reason) {
		cerr << Reason << endl; // Handle Exception
		exit(1);
	} catch(const out_of_range& Oor) {
		cerr << Oor.what() 	<< endl;
		exit(1);
	} catch(...) {
		cerr << "unbehandelte Ausnahme!" << endl;
		exit(1);
	}
	return 0;
}
