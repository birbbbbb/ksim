#include <iostream>
#include "processes.h"
using namespace std;

Processes::Processes() {
	ticks = 0;
	// array sizes
	new_size = 0;
	ready_size = 0;
	blocked_size = 0;
	running_size = 0;
	exit_size = 0;
	// arrays of processes
	newP = nullptr;
	ready = nullptr;
	blocked = nullptr;
	running = nullptr;
	exited = nullptr;
}

// Deconstructor, no memory leaks here
Processes::~Processes() {
	delete[] newP;
	delete[] ready;
	delete[] blocked;
	delete[] running;
	delete[] exited;
}

void Processes::printProcesses(process* arr, int size) {
	for (int i = 0; i < size; i++) {
		cout << "\tid: " << arr[i].id << "\n\tstate: " << arr[i].state;
		if(arr[i].ioDevice != -1){
				cout << "\n\tio-device: " << arr[i].ioDevice;
			}
		if(arr[i].waitTime != 0){
			cout << "\n\twait-time: " << arr[i].waitTime;
		}
		cout << endl << endl;
	}	
}

bool Processes::printProcessById(process* arr, int size, string id) {
	for (int i = 0; i < size; i++) {
		if (arr[i].id == id) {
			cout << "\tid: " << arr[i].id << "\n\tstate: " << arr[i].state;
			if(arr[i].ioDevice != -1){
				cout << "\n\tio-device: " << arr[i].ioDevice;
			}
			if(arr[i].waitTime != 0){
				cout << "\n\twait-time: " << arr[i].waitTime;
			}
			cout << endl << endl;
			return true;
		}
	}
	return false;
}


void Processes::addTicks(int num) {
	if(blocked_size > 0){
		for (int i = 0; i < blocked_size; i++) {
			blocked[i].waitTime -= num;
		}
	}

	ticks += num;
	if(ticks > 999999999){
		  ticks -= 999999999; //ticks will still be counted even if it exceeds 999999999
	 }
}

int Processes::getTicks() {
	return ticks;
}
