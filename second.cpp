//	g++ assign2bwt562.cpp
//	./a.out
//
//
//when executing user input is needed
//	follow onscreen instruction to build, insert, find min / max and delete min / max
//	there is a sample input given if you enter build
//		enter 'q' to end program from user input mode

//#include "stdafx.h"
#include <iostream>
#include <string>
#include <math.h>

using namespace std;

//for any node, X, at even depth, the element stored at X is smaller than the parent but larger than the grandparent(where this makes sense),
//for any node X at odd depth, the element stored at X is larger than the parent but smaller than the grandparent.
class minMaxHeap {
public:
	int* heap;
	int numElements;
	int maxSize;

	minMaxHeap(int mSize) {
		heap = new int[mSize + 2];
		maxSize = mSize;
		for (int i = 0; i < maxSize + 2; i++) {
			heap[i] = NULL;
		}
		numElements = 0;
	}
	~minMaxHeap() {
		delete heap;
		heap = NULL;
	}

	int getParent(int pos) { 
		return pos / 2;
	}
	int getLeftChild(int pos) { 
		return (2 * pos);
	}
	int getRightChild(int pos)
	{
		return (2 * pos) + 1;
	}
	void swap(int pos1, int pos2) {
		//cout << "pos " << pos1 << pos2 << endl;
		//cout << "swapping " << heap[pos1] << heap[pos2] << endl;
		int temp = heap[pos1];
		//cout << "temp is " << temp << endl;
		heap[pos1] = heap[pos2];
		//cout << "new frist is " << heap[pos1] << endl;
		heap[pos2] = temp;
		//print();
	}
	void print() {
		if (numElements == 0) {
			return;
		}
		for (int i = 1; i <= numElements; i++) {
			cout << heap[i] << " ";
		}
		cout << endl;
		return;
	}
	void buildHeap(int list[], int size) {
		for (int i = 0; i < size; i++) {
			//cout << list[i] << " ";
		}
		//cout << endl;
		for (int i = 0; i < size; i++) {
			insertHeap(list[i]);
			//print();
			//cout << endl;
		}
	}

	int findMin() {
		if (numElements == 0) {
			cout << "nothing in heap" << endl;
			return NULL;
		}
		return heap[1];
	}

	int findMax() {
		if (numElements == 0) {
			cout << "nothing in heap" << endl;
			return NULL;
		}
		if (numElements == 1) {
			return heap[1];
		}
		int toCheck = getLeftChild(1);
		int numChecked = 0;
		int largest = 0;
		int largestPos;
		while ((toCheck <= numElements) && (numChecked < 2)) {//Go through all 2 elements in the frist max level, find the largest
			if (heap[toCheck] > largest) {
				largest = heap[toCheck];
				largestPos = toCheck;
			}
			toCheck++;
			numChecked++;
		}
		return heap[largestPos];
	}

	//Finds the last positoin in the array to insert to, then makes swaps in the array so no min-max properties are broken
	void insertHeap(int value) {
		//cout << "Inserting: " << value << endl;
		//cout << "New size is: " << numElements + 1 << " max is: " << maxSize << endl;
		if ((numElements + 1) > maxSize) {
			cout << "Heap has reached max size, cannot insert" << endl;
			return;
		}
		int hole;
		bool isEvendepth;
		int pos = 1;
		//If there is no elemts do a direct insert
		if (heap[pos] == NULL) {
			heap[pos] = value;
			numElements++;
			//cout << "Inserted " << value << " as the root" << endl;
			//cout << "Num elements: " << numElements << endl << endl;
			return;
		}
		hole = numElements + 1;
		heap[hole] = value;
		numElements++;
		//cout << "The hole is at " << hole << endl;
		isEvendepth = isDepthEven(hole);
		//cout << "isEven? " << isEvendepth << endl;

		if (isEvendepth) {
			//cout << "is even" << endl;
			if (value > heap[getParent(hole)]) {
				//cout << "swapping" << heap[hole] << " and " << heap[getParent(hole)] << endl;
				swap(hole, getParent(hole));
				hole = getParent(hole);
				//This is now on an odd(max) level, check against grandparent now
			}
			else {
				//cout << "is in the right level" << endl;
			}
		}
		if (!isEvendepth) {
			//cout << "is odd" << endl;
			if (value < heap[getParent(hole)]) {
				//cout << "swapping" << heap[hole] << " and " << heap[getParent(hole)] << endl;
				swap(hole, getParent(hole));
				hole = getParent(hole);
				//This is now on an even(min) level, check against grandparent now
			}
			else {
				//cout << "is in the right level" << endl;
			}
		}
		checkGrandParents(hole);
		//cout << "Num elements: " << numElements << endl << endl;
		return;
	}

	bool isDepthEven(int pos) {
		bool isEven;
		int numParents = 0;
		while (pos != 1) {
			pos = pos / 2;
			numParents++;
		}
		//cout << "Num parents of " << pos << " is " << numParents << endl;
		if (numParents % 2 == 0) {
			//cout << "returning true" << endl;
			return true;
		}
		else {
			//cout << "returning false" << endl;
			return false;
		}
	}
	//Checks a newly placed node against its granparents and swaps them if needed
	void checkGrandParents(int pos) {
		bool isEven = isDepthEven(pos);
		bool checkGrandParents = true;
		while (checkGrandParents) {
			if (isEven) {
				if (getParent(getParent(pos)) >= 1) { //If there are grandparents to check against
					if (heap[pos] < heap[getParent(getParent(pos))]) { //If the current pos is smaller than its grandparent
						//cout << "swapping grandparnets " << heap[pos] << " and " << heap[getParent(getParent(pos))] << endl;
						swap(pos, getParent(getParent(pos)));
						pos = getParent(getParent(pos));
					}
					else {
						checkGrandParents = false;
					}
				}
				else {
					checkGrandParents = false;
				}
			}
			if (!isEven) {
				cout << "checking gpa " << getParent(getParent(pos)) << endl;
				if (getParent(getParent(pos)) >= 1) {
					if (pos > getParent(getParent(pos))) {
						cout << "swapping grandparnets " << heap[pos] << " and " << heap[getParent(getParent(pos))] << endl;
						swap(pos, getParent(getParent(pos)));
						pos = getParent(getParent(pos));
					}
					else {
						checkGrandParents = false;
					}
				}
				else {
					checkGrandParents = false;
				}
			}
		}
	}

	void deleteMin() {
		int lastNum = heap[numElements];
		//cout << "The last num is " << lastNum << endl;
		if (numElements == 0) {
			cout << "nothing in heap" << endl;
			return;
		}
		if (numElements == 1) {
			heap[1] = NULL;
			numElements--;
			return;
		}
		if (numElements <= 3) {
			if (numElements == 2) {
				heap[1] = heap[2];
				heap[2] = NULL;
				numElements--;
			}
			int smallest = heap[2];
			int smallestPos = 2;
			int largest = heap[3];
			if (heap[3] < heap[2]) {
				smallest = heap[3];
				smallestPos = 3;
				largest = heap[2];
			}
			heap[1] = smallest;
			heap[2] = largest;
			heap[3] = NULL;
			numElements--;
			return;
		}
		int toCheck = getLeftChild(getLeftChild(1));
		int numChecked = 0;
		int smallest = 9999;
		int smallestPos;
		while ((toCheck <= numElements) && (numChecked < 4)) {//Go through all 4 elements in the frist min level, find the smallest
			if (heap[toCheck] < smallest) {
				smallest = heap[toCheck];
				smallestPos = toCheck;
			}
			toCheck++;
			numChecked++;
		}
		//cout << heap[smallestPos] << " is the smallest" << endl;
		//replace the root with the smallest
		heap[1] = heap[smallestPos];

		//replace the hole with the last number
		heap[smallestPos] = heap[numElements];
		numElements--;

		//check if this replacement broke heap rules
			//ONly need to check whats below the current placement. (weve checked the granparnts above for the frist insert of this num)
		trickleDown(smallestPos);
	}

	void deleteMax() {
		int lastNum = heap[numElements];
		//cout << "The last num is " << lastNum << endl;
		if (numElements == 0) {
			cout << "nothing in heap" << endl;
			return;
		}
		if (numElements == 1) {
			heap[1] = NULL;
			numElements--;
			return;
		}
		//find the greatest elemtent on first amx level
		int toCheck = getLeftChild(1);
		int numChecked = 0;
		int largest = 0;
		int largestPos;
		while ((toCheck <= numElements) && (numChecked < 2)) { //Go through all 2 elements in the frist max level, find the largest
			//cout << "checkung" << toCheck << endl;
			if (heap[toCheck] > largest) {
				largest = heap[toCheck];
				largestPos = toCheck;
			}
			toCheck++;
			numChecked++;
		}
		//cout << heap[largestPos] << " is the largest" << endl;
		//replace the largest with the last number
		heap[largestPos] = heap[numElements];
		numElements--;

		trickleDown(largestPos);
	}

	void trickleDown(int hole) {
		bool isEven = isDepthEven(hole);
		bool swap1 = false;
		bool swap2 = false;
		int nextCheck = getLeftChild(hole);
		if (isEven) {
			//We are on a even level so it should be smaller than its children, if not then switch
			if (nextCheck > numElements) {
				//weve checked the last element and can stop
				return;
			}
			if (heap[hole] > heap[nextCheck]) {
				swap1 = true;
			}
			//Checkng right node
			nextCheck = getRightChild(hole);
			if (nextCheck > numElements) {
				//weve checked the last element and can stop
				return;
			}
			if (heap[hole] > heap[nextCheck]) {
				swap2 = true;
			}
			int smallest;
			//if both children are smaller than the parent, swap with the smallest one
			if (swap1 && swap2) {
				if (heap[nextCheck - 1] < heap[nextCheck]) {
					swap(hole, nextCheck - 1);
					hole = nextCheck - 1;
				}
				else {
					swap(hole, nextCheck);
					hole = nextCheck;
				}
			}
			//if only one child is smaller, swap with it
			if (swap1) {
				swap(hole, nextCheck - 1);
				hole = nextCheck;
			}
			if (swap2) {
				swap(hole, nextCheck);
				hole = nextCheck;
			}
		}

		if (!isEven) {
			//We are on a odd level so it should be larger than its children, if not then switch
			if (nextCheck > numElements) {
				//weve checked the last element and can stop
				return;
			}
			if (heap[hole] < heap[nextCheck]) {
				swap1 = true;
			}
			//Checkng right node
			nextCheck = getRightChild(hole);
			if (nextCheck > numElements) {
				//weve checked the last element and can stop
				return;
			}
			if (heap[hole] < heap[nextCheck]) {
				swap2 = true;
			}
			int smallest;
			//if both children are larger than the parent, swap with the largest one
			if (swap1 && swap2) {
				if (heap[nextCheck - 1] < heap[nextCheck]) {
					swap(hole, nextCheck - 1);
					hole = nextCheck - 1;
				}
				else {
					swap(hole, nextCheck);
					hole = nextCheck;
				}
			}
			//if only one child is larger, swap with it
			if (swap1) {
				swap(hole, nextCheck - 1);
				hole = nextCheck - 1;
			}
			if (swap2) {
				swap(hole, nextCheck);
				hole = nextCheck;
			}
		}
	}
};


int main()
{
	bool quit = false;
	int maxHeapSize;
	string choice = "";
	int toInsert = 0;

	cout << "Welcome! please input the maximum # of elements in the heap: ";
	cin >> maxHeapSize;
	cout << endl;
	cout << "The max size of hap is " << maxHeapSize << endl;
	minMaxHeap *inputHeap = new minMaxHeap(maxHeapSize);
	while (!quit) {
		cout << "Actions: (b)-Build heap (i)-insert (fmax)-find max (fmin)-find min (dmax)-delete max (dmin)-delete min (q)-end program" << endl;
		cin >> choice;
		if (choice == "b") {
			cout << "Please enter a list of numbers seperated by spaces, followed by a 's'" << endl;
			cout << "Ex. 30 26 25 22 21 20 18 16 14 12 10 5 s" << endl;
			cin >> choice;
			while (choice != "s") {
				toInsert = 0;
				for (int j = 0; j < choice.length(); j++) {
					toInsert += (int)(choice.at(j) - 48) * pow(10, choice.length()-j-1);
				}
				//toInsert = atoi(choice.c_str());
				inputHeap->insertHeap(toInsert);
				cin >> choice;
			}
		}
		if (choice == "i") {
			cout << "Please enter a number" << endl;
			cin >> choice;
			toInsert = 0;
			for (int j = 0; j < choice.length(); j++) {
				//cout << "looking at " << ((int)choice.at(j) - 48) << " j is " << j << endl;
				//cout << "which is " << choice.at(j) - 48 << endl;
				//cout << "changing to " << ((int)choice.at(j) - 48) * pow(10, choice.length() - j-1) << " by multipying by " << pow(10, choice.length() - j-1) << endl;
				toInsert += (((int)choice.at(j) - 48) * pow(10, choice.length() - j-1));
			}
			inputHeap->insertHeap(toInsert);
		}
		if (choice == "fmax") {
			cout << "The maximum number is: " << inputHeap->findMax() << endl;
		}
		if (choice == "fmin") {
			cout << "The minimum number is: " << inputHeap->findMin() << endl;
		}
		if (choice == "dmax") {
			inputHeap->deleteMax();
			cout << "The maximum number has been deleted" << endl;
		}
		if (choice == "dmin") {
			inputHeap->deleteMin();
			cout << "The minimum number has been deleted" << endl;
		}
		if (choice == "q") {
			cout << "Goodbye." << endl;
			quit = true;
		}
		cout << endl;
		inputHeap->print();
		cout << endl;
	}
	inputHeap->findMax();
	delete inputHeap;
	return 0;
}

