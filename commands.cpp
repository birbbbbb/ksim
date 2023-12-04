#include <iostream>
#include "processes.h"
using namespace std;

// Add a process to the new state
bool Processes::add(string id) {

	if(id == ""){
		cout << "No process name specified" << endl;
		return false;
	}

	// Check if the process already exists
	for (int i = 0; i < new_size; i++) {
		if (newP[i].id == id) {
			cout << "Process \"" << id << "\" already exists." << endl;
			return false;
		}
	}
	for (int i = 0; i < ready_size; i++) {
		if (ready[i].id == id) {
			cout << "Process \"" << id << "\" already exists." << endl;
			return false;
		}
	}
	for(int i = 0; i < blocked_size; i++){
		if(blocked[i].id == id){
			cout << "Process \"" << id << "\" already exists." << endl;
			return false;
		}
	}
	for (int i = 0; i < running_size; i++) {
		if (running[i].id == id) {
			cout << "Process \"" << id << "\" already exists." << endl;
			return false;
		}
	}
	for(int i = 0; i < exit_size; i++){
		if(exited[i].id == id){
			cout << "Process \"" << id << "\" already exists." << endl;
			return false;
		}
	}

	// Add the process to the new state
	process* temp = new process[new_size + 1];
	for(int i = 0; i < new_size; i++) {
		temp[i] = newP[i];
	}
	temp[new_size].id = id;
	temp[new_size].state = "new";
	temp[new_size].ioDevice = -1;
	temp[new_size].waitTime = 0;
	new_size++;
	delete[] newP;
	newP = temp;

	return true;
}

void Processes::query(string id) {
	bool found = false;

	if (id == "all" || id == "") {
		  found = true;
		  bool printed = false;
		  if(new_size > 0){
				printed = true;
				cout << "New:" << endl;
				printProcesses(newP, new_size);
		  }
		  if(ready_size > 0){
				printed = true;
				cout << "Ready:" << endl;
				printProcesses(ready, ready_size);
		  }
		  if(blocked_size > 0){
				printed = true;
				cout << "Blocked:" << endl;
				printProcesses(blocked, blocked_size);
		  }
		  if(running_size > 0){
				printed = true;
				cout << "Running:" << endl;
				printProcesses(running, running_size);
		  }
		  if(exit_size > 0){
				printed = true;
				cout << "Exited:" << endl;
				printProcesses(exited, exit_size);
		  }

		  if(!printed){
				cout << "No processes in system" << endl;
		  }
		  
	} else {
		  found = printProcessById(newP, new_size, id);
		  found = found || printProcessById(ready, ready_size, id);
		  found = found || printProcessById(blocked, blocked_size, id);
		  found = found || printProcessById(running, running_size, id);
		  found = found || printProcessById(exited, exit_size, id);
	 }

	if (!found) {
		cout << "No such process" << endl;
	}
}

void Processes::ioEvent(int ioDeviceNum) {
	bool found = false;

	if (ioDeviceNum >= 0 && ioDeviceNum <= 3) {
		for (int i = 0; i < blocked_size; i++) {
			if (blocked[i].ioDevice == ioDeviceNum) {
					 //set found and add ticks
					 found = true;
					 addTicks(1);

					 //move the process from blocked to ready
					 process *tempReady = new process[ready_size + 1];\
					 for(int i = 0; i < ready_size; i++){
						  tempReady[i] = ready[i];
					 }
					 tempReady[ready_size] = blocked[i];
					 tempReady[ready_size].state = "Ready";
					 tempReady[ready_size].ioDevice = -1;
					 tempReady[ready_size].waitTime = 0;
					 ready_size++;
					 delete[] ready; //clear ready
					 ready = tempReady;

					 // remove the process from blocked
					 process *tempBlocked = new process[blocked_size - 1];
					 for (int j = 0; j < i; j++) {
						  tempBlocked[j] = blocked[j];
					 }
					 for (int j = i + 1; j < blocked_size; j++) {
						  tempBlocked[j - 1] = blocked[j];
					 }
					 blocked_size--;
					 delete[] blocked; // clear blocked
					 blocked = tempBlocked;
				i--; //decrement i to account for the removed process
				cout << "\tProcess \"" << ready[ready_size - 1].id << "\" moved from Blocked (ioDev=" << ioDeviceNum << ") to Ready" << endl;
			}
		}
	} else {
		cout << "No such I/O device" << endl;
	}

	if (!found) {
		cout << "No processes waiting on device " << ioDeviceNum << endl;
	}
}

void Processes::wait(int ioDeviceNum) {
	if (running_size == 0) {
		cout << "No process is currently in the Running state" << endl;
		return;
	}

	if (ioDeviceNum < 0 || ioDeviceNum > 3) {
		cout << "Operand <io-dev-num> is not valid" << endl;
		return;
	}

	 //add the running process to the blocked queue, remove from running queue
	 process* temp = new process[blocked_size + 1];
	 
	 for(int i = 0; i < blocked_size; i++){
		  temp[i] = blocked[i];
	 }
	 temp[blocked_size] = running[0];
	 temp[blocked_size].state = "Blocked";
	 temp[blocked_size].ioDevice = ioDeviceNum;
	 temp[blocked_size].waitTime = 1024;
	 blocked = temp;
	 delete[] running; //clear running

	blocked_size++;
	running_size--;
	 addTicks(1);
	cout << "\tProcess \"" << blocked[blocked_size - 1].id << "\" moved from Running to Blocked" << endl;
}

bool Processes::release(){
	if(running_size == 0){
		return false;
	}
	process* temp = new process[exit_size + 1];
	for(int i = 0; i < exit_size; i++){
		temp[i] = exited[i];
	}
	temp[exit_size] = running[0];
	temp[exit_size].state = "Exited";
	exited = temp;
	exit_size++;
	running_size--;

	cout << "\tProcess \"" << exited[exit_size - 1].id << "\" moved from Running to Exited" << endl;
	return true;
}

void Processes::step() {
	bool dispatch = false;

	// Remove processes in the Exit state
	for (int i = 0; i < exit_size; i++) {
		cout << "\tProcess \"" << exited[i].id << "\" is banished to the void." << endl;
	}
	exit_size = 0;

	// Advance at most 1 process from New to Ready
	if (new_size > 0 && &newP[0] != nullptr) {
		process* temp = new process[ready_size + 1];
		for (int i = 0; i < ready_size; i++) {
			temp[i] = ready[i];
		}
		temp[ready_size] = newP[0];
		//cout << "Advancing process from new to ready:" << endl;
		ready = temp;
		ready[ready_size].state = "Ready";
		cout << "\tProcess \"" << newP[0].id << "\" moved from New to Ready." << endl;
		ready_size++;
		for (int i = 1; i < new_size; i++) {
			newP[i - 1] = newP[i];
		}
		new_size--;
	}

	// If a process is in the Running state, move it to Ready
	if (running_size > 0) {
		  process *tempReady = new process[ready_size + 1];
		  for(int i = 0; i < ready_size; i++){
				tempReady[i] = ready[i];
		  }
		  tempReady[ready_size] = running[0];
		  tempReady[ready_size].state = "Ready";
		  ready = tempReady;
		  ready_size++;
		  delete[] running; //clear running
		  running_size--;
		cout << "\tProcess \"" << ready[ready_size - 1].id << "\" moved from Running to Ready." << endl;
	}

	// Dispatch a process from Ready to Running and remove it from Ready
	if (ready_size > 0 && running_size == 0	) {

		process* temp = new process[running_size + 1];
		for (int i = 0; i < running_size; i++) {
			temp[i] = running[i];
		}
		temp[running_size] = ready[0];
		temp[running_size].state = "Running";
		
		//cout << "Dispatching process from ready to running:" << endl;
		running = temp;
		dispatch = true;
		cout << "\tProcess \"" << ready[0].id << "\" moved from Ready to Running." << endl;
		running_size++;
		ready_size--;

		  //remove the process from ready
		  process *tempReady = new process[ready_size];
		  for(int i = 0; i < ready_size; i++){
				tempReady[i] = ready[i + 1];
		  }
		  delete[] ready; //clear ready
		  ready = tempReady;
	}

	// If a process went from ready to running, consume 256 ticks
	if (dispatch) {
		addTicks(256);
	}

	// Advance at most 1 process from each I/O device in Blocked to Ready
	for (int i = 0; i < blocked_size; i++) {
		if (blocked[i].waitTime <= 0) {
			//cout << "Advancing processes from blocked to ready:" << endl;
				process* tempReady = new process[ready_size + 1];
				for(int i = 0; i < ready_size; i++){
					 tempReady[i] = ready[i];
				}
			int ioDevice = blocked[i].ioDevice;
				tempReady[ready_size] = blocked[i];
				tempReady[ready_size].state = "Ready";
				tempReady[ready_size].ioDevice = -1;
				tempReady[ready_size].waitTime = 0;
				delete[] ready; //clear ready
				ready = tempReady;
				ready_size++;

			for (int j = i + 1; j < blocked_size; j++) {
				blocked[j - 1] = blocked[j];
			}
			blocked_size--;
			cout << "\tProcess \"" << blocked[i].id << "\" moved from Blocked (ioDev=" << ioDevice << ")" << endl;
		}
	}

}
