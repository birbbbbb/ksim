struct process{
	std::string id;
	std::string state;
	int ioDevice;
	int waitTime;
};


class Processes {
	private:
		int ticks;
		
		process* newP;
		process* ready;
		process* blocked;
		process* running;
		process* exited;
		int new_size;
		int ready_size;
		int blocked_size;
		int running_size;
		int exit_size;

		std::string pop(process *arr, int size);
		void push(process *arr, int size, std::string id, std::string state, int ioDevice, int waitTime);
		void printProcesses(process* p, int size);
		bool printProcessById(process* p, int size, std::string id);
	public:
		// processes.cpp
		Processes();
		~Processes();
		void addTicks(int num);
		int getTicks();

		// commands.cpp
		bool add(std::string id);
		void query(std::string id);
		void ioEvent(int ioDeviceNum);
		void wait(int ioDeviceNum);
		bool release();
		void step();
};
