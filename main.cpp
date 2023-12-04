#include <iostream>
#include <iomanip>
#include "processes.h"
using namespace std;

Processes processes; // global processes

//help function
void help(){
	 cout << "help - Displays all the commands" << endl;
	 cout << "clear - Clears the screen" << endl;
	 cout << "add <process-name> - Add a new process" << endl;
	 cout << "exit - Exits the program" << endl;
	 cout << "io-event <io-dev-num> - Simulates the I/O Device <io-dev-num> is ready" << endl;
	 cout << "query <process-name> - Displays the status of <process-name>" << endl;
	 cout << "release - Moves the current running process to state exit" << endl;
	 cout << "step - Runs one clock cycle" << endl;
	 cout << "wait <io-dev-num> - Moves the current running process to the wait queue\n\tfor I/O device <io-dev-num>" << endl;
}

//parse function
void parse(string token, string opcode = ""){
	 if(token == "help")
	 {
		  help();
	 } else if (token == "clear"){
		  system("clear");
	 } else if (token == "add"){
		  if(processes.add(opcode)){
				cout << "Process " << opcode << " added" << endl;
				processes.addTicks(32);
		  }
	 } else if(token == "query"){
		  processes.query(opcode);
	 } else if(token == "step"){
		  processes.step();
	 } else if(token == "wait"){
		  if(opcode == ""){
				cout << "No I/O device specified" << endl;
		  } else {
				processes.wait(stoi(opcode));
		  }
	 } else if (token == "release"){
		  if(processes.release()){
				processes.addTicks(32);
		  } else {
				cout << "No process is currently running" << endl;
		  }
	 } else if (token == "io-event"){
		  processes.ioEvent(stoi(opcode));
	 } else {
		  cout << "Command Not Found!" << endl;
	 }
}

int main(){
	 
	 cout << endl;
	 
	 // Main loop
	 while(true){
		  string input;
		  string token = "";
		  string opcode = "";
		  bool space = false;
		  
		  // Outputs the prompt
		  cout << "ksim-" << setfill('0') << setw(9) << processes.getTicks() << "> ";

		  getline(cin, input);
		  // Tokenize the input   
		  for(int i = 0; i < input.length(); i++){
				if(input[i] == ' '){
					 space = true;
					 i++;
				}
				
				if(!space){
					 token += input[i];
				} else {
					 opcode += input[i];
				}
		  }
		  if(token == ""){
				token = input;
		  }

		  //Check for exit condition
		  if(token == "exit"){
				break;
		  }

		  parse(token, opcode);

	 }

	 return 0;
}