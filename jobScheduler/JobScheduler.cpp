#include <iostream>
#include <string>
#include <list>
#include <fstream>
#include <iostream>
#include <sstream>

//Classes To be Used
class Process {
    public:
        int pid;
        int arrivalTime;
        int cpuBursts;
        int priority;
        int completionTime;
        int firstTimeInQ;
        bool operator == (const Process& p) const { return this->pid == p.pid && this->arrivalTime == p.arrivalTime && this->cpuBursts == p.cpuBursts && this->priority == p .priority; }
        bool operator != (const Process& p) const { return !operator==(p); }

    Process(int pid, int at, int cpu, int pri) {
        this->pid = pid;
        this->arrivalTime = at;
        this->cpuBursts = cpu;
        this->priority = pri;
    }

    int getPID() {
        return pid;
    }
};

class queue {
    public:
        std::string name;
        std::list<Process> processList;



    queue(std::string n) {
        name = n;
    }

    int addProcess(Process p) {
            processList.push_back(p);
            return 0;
    }

    int removeProcess(Process p) {

        return 0;
    };

    int showList() {
        if (processList.empty()) {
            std::cout << this->name << " is empty\n";
            return 0;
        }
        for (auto p : processList ){
                printf("%d ", p.pid);
        }
        return 0;
    }
};

class scheduler {
    public:
        std::string type;
        std::string file;
        queue newProcess = queue("New");
        queue ready = queue("Ready");
        queue run = queue("Run");
        queue wait = queue("Wait");
        queue complete = queue("Complete");

    scheduler(std::string t, std::string jobsFile) {
        //Valid types are 
        //fsfs : First Come First Serve
        //sjf : Shortest Job First
        //rr : Round Robin
        //
        //f is the file to read the jobs
        this->type = t;
        this->file = jobsFile;
        
}

    int readFileContents() {
        std::string line;
        std::string word;
        std::ifstream fileObj(file);

        if (fileObj.is_open()) {
            std::getline(fileObj, line);
            while (getline(fileObj, line)) {
                
                std::stringstream str(line);

                getline(str, word, ',');
                int pid = stoi(word);

                getline(str, word, ',');
                int arr = stoi(word);

                getline(str, word, ',');
                int burst = stoi(word);

                getline(str, word);
                int prior = stoi(word);


                Process p = Process(pid, arr, burst, prior);
                
                //std::cout <<  "\n" << p.cpuBursts << "\n";

                newProcess.addProcess(p);
                //std::cout << "\n\n";

            }
            fileObj.close();

        }
        return 0;
    }

    int schedulingSimulation() {
        if (this->type == "fcfs") {
            int runtime = getRuntime();
            //This is the time loop
            for (int t = 0; t <= runtime; t++) {
                //This loops through the list and checks arrival times to move them to the next queue
                //Moves newProcess to Ready Queue
                //
                for (auto it = this->newProcess.processList.begin(); it != this->newProcess.processList.end();) {
                    Process p = *it;
                    if (p.arrivalTime == t) {
                        this->ready.addProcess(p);
                        it = this->newProcess.processList.erase(it);
                    } else {
                        ++it;
                    }
                }

                //This selects a job and moves it to the run queue
                int pToMove = 1000;
                for (auto it = this->ready.processList.begin(); it != this->ready.processList.end();) {
                    Process p = *it;
                    
                    int tempArrivalTime = 1000;
                    
                    if (pToMove < p.arrivalTime) {
                        pToMove = p.arrivalTime;
                    } else {
                        ++it;
                    }

                    
                }
                std::cout << pToMove << " P To Move\n";

                //Actually move pToMove to the Run queue
                for (auto it = this->ready.processList.begin(); it != this->ready.processList.end();) {
                        if (pToMove == (*it).arrivalTime) {
                            this->run.addProcess(*it);
                            it = this->ready.processList.erase(it);
                        }
                        ++it;

                }
                

            }
            return 0;
        } 
        else if (this->type == "sjf") {

        }   
        else if (this->type == "rr") {

        } 
        else {
            std::cout << "The Type of scheduler is invalid or not implemented\n\n";
        }


        return 0;
    };

    int getRuntime() {
        int sum = 0;
        for (auto p : this->newProcess.processList ){
        std::cout << p.cpuBursts << "\n";
        sum += p.cpuBursts;
        }
        return sum;
    };

};

//Main Program
int main() {    
    scheduler fcfs = scheduler("fcfs", "./jobs.txt");
    fcfs.readFileContents();
    fcfs.schedulingSimulation();

    std::cout << "New Process Queue: " << fcfs.newProcess.showList() << "\n";
    std::cout << "Ready Queue: " << fcfs.ready.showList() << "\n";
    std::cout << "Run Queue: " << fcfs.run.showList() << "\n";

    return 0;
}