//	g++ assign2bwt562.cpp
//	./a.out
//
// User input is required to run the program. Follow the instructions given to operate program. 
// First build with a list of numbers (or insert a few numbers), then perform operations on the heap
// Use 'q' to quit

#include <iostream>
#include <string>
#include <math.h>

using namespace std;

// The minmax heap class. Follows the minmax properties where all even depth elemts are minimums, but thier grandchildren will be larger than they are. 
// And the odd depths are maximums, but its grandchildren will be smaller than they are
class minMaxHeap {
public:
	int* heap;
	int numElements;
	int maxSize;

	minMaxHeap(int mSize) {
		heap = new int[mSize];
		for (int i = 0; i < mSize; i++) {
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

	//Swaps the postions of two elements in the heap
	void swap(int pos1, int pos2) {
		int temp = heap[pos1];
		heap[pos1] = heap[pos2];
		heap[pos2] = temp;
	}

	//Prints the heap out
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

	//From an input array, builds the heap array by perfroming many insertions on the heap
	void buildHeap(int list[], int size) {
		for (int i = 0; i < size; i++) {
			insertHeap(list[i]);
		}
	}

	//Returns the Min(root)
	int findMin() {
		if (numElements == 0) {
			cout << "Heap empty" << endl;
			return NULL;
		}
		return heap[1];
	}

	//Returns the max. Searches through the second level of elements
	int findMax() {
		if (numElements == 0) {
			cout << "Heap empty" << endl;
			return NULL;
		}
		if (numElements == 1) {
			return heap[1];
		}
		int toCheck = getLeftChild(1);
		int numChecked = 0;
		int largest = 0;
		int largestPos;
		while ((toCheck <= numElements) && (numChecked < 2)) {
			if (heap[toCheck] > largest) {
				largest = heap[toCheck];
				largestPos = toCheck;
			}
			toCheck++;
			numChecked++;
		}
		return heap[largestPos];
	}

	//Creates a hole to insert the new element into, then performs swaps so that the minma heap properties are not broken
	void insertHeap(int value) {
		if ((numElements + 1) > maxSize) {
			cout << "Heap has reached max size, cannot insert" << endl;
			return;
		}
		int hole;
		bool isEvendepth;
		int pos = 1;
		if (heap[pos] == NULL) {
			heap[pos] = value;
			numElements++;
			return;
		}
		hole = numElements + 1;
		heap[hole] = value;
		numElements++;
		isEvendepth = isDepthEven(hole);

		//bubble the hole up if the new element is greater or less than its parent on a even or odd depth
		if (isEvendepth) {
			if (value > heap[getParent(hole)]) {
				swap(hole, getParent(hole));
				hole = getParent(hole);
			}
		}
		if (!isEvendepth) {
			if (value < heap[getParent(hole)]) {
				swap(hole, getParent(hole));
				hole = getParent(hole);
			}
		}
		checkGrandParents(hole); //Verify that it did not break the proerpty beween granparent and child
		return;
	}

	//Returns true if the position called is on an even depth
	bool isDepthEven(int pos) {
		bool isEven;
		int numParents = 0;
		while (pos != 1) {
			pos = pos / 2;
			numParents++;
		}
		if (numParents % 2 == 0) {
			return true;
		}
		else {
			return false;
		}
	}

	//Compared a newly inserted node against its grandparent, performs a swap if needed.
	void checkGrandParents(int pos) {
		bool isEven = isDepthEven(pos);
		bool checkGrandParents = true;
		while (checkGrandParents) {
			if (isEven) {
				if (getParent(getParent(pos)) >= 1) { //Check if there is a grandparent
					if (heap[pos] < heap[getParent(getParent(pos))]) { //If swap should be made
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
				if (getParent(getParent(pos)) >= 1) {
					if (pos > getParent(getParent(pos))) {
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

	//Deletes the min and bubbles up a number to fill the hole
	void deleteMin() {
		int lastNum = heap[numElements];
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
		// Case if there are more than 3 elements in the heap
		int toCheck = getLeftChild(getLeftChild(1));
		int numChecked = 0;
		int smallest = 9999;
		int smallestPos;
		while ((toCheck <= numElements) && (numChecked < 4)) {// Find the smallest element in the second min level
			if (heap[toCheck] < smallest) {
				smallest = heap[toCheck];
				smallestPos = toCheck;
			}
			toCheck++;
			numChecked++;
		}
		heap[1] = heap[smallestPos]; //replace the root with the smallest

		//replace the hole with the last number
		heap[smallestPos] = heap[numElements];
		numElements--;

		trickleDown(smallestPos); // Fix the heap by trickling down the new element
	}

	void deleteMax() {
		int lastNum = heap[numElements];
		if (numElements == 0) {
			cout << "nothing in heap" << endl;
			return;
		}
		if (numElements == 1) {
			heap[1] = NULL;
			numElements--;
			return;
		}

		//Locate the max element on the second level
		int toCheck = getLeftChild(1);
		int numChecked = 0;
		int largest = 0;
		int largestPos;
		while ((toCheck <= numElements) && (numChecked < 2)) {
			if (heap[toCheck] > largest) {
				largest = heap[toCheck];
				largestPos = toCheck;
			}
			toCheck++;
			numChecked++;
		}
		heap[largestPos] = heap[numElements]; //Replace the hole with the last element in the array
		numElements--;

		trickleDown(largestPos);	//Fix heap
	}

	// Trickles a newly placed element down the heap to maintain heap properties
	// Checks if the hole 
	void trickleDown(int hole) {
		bool isEven = isDepthEven(hole);
		bool swap1 = false;
		bool swap2 = false;
		int nextCheck = getLeftChild(hole);
		if (isEven) {
			// Check left node, on even level so swap if hole is greater than its child
			if (nextCheck > numElements) { 
				return;	//If this position doesnt exist
			}
			if (heap[hole] > heap[nextCheck]) {
				swap1 = true;
			}
			// Check right node
			nextCheck = getRightChild(hole);
			if (nextCheck > numElements) {
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

	cout << "Welcome! Please input the maximum # of elements in the heap: ";
	cin >> maxHeapSize;
	cout << endl;
	cout << "The max size of heap is " << maxHeapSize << endl;
	minMaxHeap *inputHeap = new minMaxHeap(maxHeapSize);
	while (!quit) {
		cout << "Actions: (b)-Build heap (i)-insert (fmax)-find max (fmin)-find min (dmax)-delete max (dmin)-delete min (q)-end program" << endl;
		cin >> choice;
		if (choice == "b") {
			cout << "Please enter a list of numbers seperated by spaces, followed by a 's'" << endl;
			cout << "Ex. 15 30 2 7 10 5 9 6 18 s" << endl;
			cin >> choice;
			while (choice != "s") {
				toInsert = 0;
				for (int j = 0; j < choice.length(); j++) {
					toInsert += (int)(choice.at(j) - 48) * pow(10, choice.length()-j-1);
				}
				inputHeap->insertHeap(toInsert);
				cin >> choice;
			}
		}
		if (choice == "i") {
			cout << "Please enter a number" << endl;
			cin >> choice;
			toInsert = 0;
			for (int j = 0; j < choice.length(); j++) {
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
