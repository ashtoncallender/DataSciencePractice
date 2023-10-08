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
        int tempCpuBursts;
        bool operator == (const Process& p) const { return this->pid == p.pid && this->arrivalTime == p.arrivalTime && this->cpuBursts == p.cpuBursts && this->priority == p .priority; }
        bool operator != (const Process& p) const { return !operator==(p); }

    //Default Pseudo Null constructor
    Process() {
        this->pid = 0;
        this->arrivalTime = 0;
        this->cpuBursts = 0;
        this->priority = 0;
        this->firstTimeInQ = -1;
    }

    Process(int pid, int at, int cpu, int pri) {
        this->pid = pid;
        this->arrivalTime = at;
        this->cpuBursts = cpu;
        this->priority = pri;
        this->tempCpuBursts = cpu;
    }

    int getPID() {
        return pid;
    }
};

class Queue {
    public:
        std::string name;
        Process processList[25];
        int position = 0;

    Queue(std::string n) {
        this->name = n;
        for (int i = 0; i < 25; i++) {
            //The negative one is used to signify it is a placeholder process from initalization
            this->processList[i] = Process(-1,0,0,0);
        }
        this->position = 0;
    }

    int addProcess(Process p) {
            this->processList[position] = p;
            this->position += 1;
            std::cout << "Process added id " << p.pid << "\n";
            return 0;
    };

    int removeProcess(Process p) {

        return 0;
    };

    int showList() {
        
        for (int i = 0; i < 25; i++) {
            if(this->processList[i].pid != -1) {
                std::cout << (this->processList[i]).pid << " ";
            }

        }
        return 0;
    };

};

class scheduler {
    public:
        std::string type;
        std::string file;
        Queue newProcess = Queue("New");
        Queue ready = Queue("Ready");
        Queue run = Queue("Run");
        Queue wait = Queue("Wait");
        Queue complete = Queue("Complete");

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
                //Process * pToAdd = &p;
                //std::cout <<  "\n" << p.cpuBursts << "\n";

                newProcess.addProcess(p);
                //std::cout << "\n\n";

            }
            fileObj.close();

        }
        return 0;
    };


    void runStats() {
        float tPut, turnaround, response;

        turnaround = 0.0f;
        response = 0.0f;
        int runtime = getRuntime();
        
        for (int i = 0; i < 25; i++) {
            if(complete.processList[i].pid != -1 || complete.processList[i].pid != 0) {
                turnaround = turnaround + (complete.processList[i].completionTime - complete.processList[i].arrivalTime);
                response = response + (complete.processList[i].firstTimeInQ - complete.processList[i].arrivalTime);
            }
        }

        turnaround = turnaround / 7;
        response = response / 7;
        tPut = float(7)/float(runtime);

        std::cout << "Run Stats\n";
        std::cout << "Throughput = " << tPut << "\n";
        std::cout << "Turnaround Time = " << turnaround << "\n";
        std::cout << "Response Time = " << response << "\n";
    };

    void schedulingSimulation() {
        Process pToMove;
        //This is how we choose the algorithim for each type of scheduling algorithim
        //In an ideal world I think this is when you would use Classes and interfaces
        //But lets be honest we just need to show it works
        if (this->type == "fcfs") {
            int runtime = getRuntime();
            std::cout << "Runtime: " << runtime << "\n";
            //This is the time loop
            for (int t = 0; t <= runtime; t++) {

                //This loops through the list and checks arrival times to move them to the next queue
                //Moves newProcess to Ready Queue
                //It runs everytime and there is probably a better way to do this, but I have been working on this for too long
                
                for (int i = 0; i < 25; i++) {
                    if(this->newProcess.processList[i].arrivalTime == t) {
                        this->ready.processList[ready.position] = this->newProcess.processList[i];
                        newProcess.processList[i] = Process(-1,0,0,0);
                        ready.position++;
                    }
                }

                //This selects a job and moves it to the run queue
                //I moved the for loop inside so it wont run every single time
                //Trying to be somewhat more efficent 
                if (this->run.processList[0].pid = -1) {
                    for (int i = 0; i < 25; i++) {
                        if(this->ready.processList[i].pid != -1) {
                            this->run.processList[0] = this->ready.processList[i];
                            this->ready.processList[i] = Process(-1,0,0,0);
                            this->run.processList[0].firstTimeInQ = t;
                            break;
                        }
                    }
                }
                
                int timeNeeded;
                //Actually run process
                
                timeNeeded = t - run.processList[0].cpuBursts - complete.processList[complete.position - 1].completionTime;

                //Remove process from run Queue and to Completed
                if (timeNeeded == 0) {
                    this->run.processList[0].completionTime = t;
                    this->complete.processList[complete.position] = this->run.processList[0];
                    this->run.processList[0] = Process(-1,0,0,0);
                    this->complete.position++;
                }

                //This was added for debugging, took a lot longer than you'd think
                //std::cout << "Coomplete Position: " << complete.position <<  " " << complete.processList[complete.position - 1].completionTime << "\n";
                //std::cout << "Current Time: " << t << "\n";
                

            }
            
        }
        
        else if (this->type == "sjf") {

            int runtime = getRuntime();
            std::cout << "Runtime: " << runtime << "\n";
            //This is the time loop
            for (int t = 0; t <= runtime; t++) {

                //This loops through the list and checks arrival times to move them to the next queue
                //Moves newProcess to Ready Queue
                //It runs everytime and there is probably a better way to do this, but I have been working on this for too long
                
                for (int i = 0; i < 25; i++) {
                    if(this->newProcess.processList[i].arrivalTime == t) {
                        this->ready.processList[ready.position] = this->newProcess.processList[i];
                        newProcess.processList[i] = Process(-1,0,0,0);
                        ready.position++;
                    }
                }

                //This selects a job and moves it to the run queue
                //I moved the for loop inside so it wont run every single time
                //Trying to be somewhat more efficent 
                int pTaken = 0;
                

                pToMove = Process(0,1000,1000,1000);
                
               
                //Why it never loads process 101 into the queue I have no idea
                
                if (this->run.processList[0].pid == -1) {
                    for (int i = 0; i < 25; i++) {
                        if(this->ready.processList[i].pid != -1) {
                            if(this->ready.processList[i].cpuBursts < pToMove.cpuBursts) {
                                pToMove = this->ready.processList[i];
                                pTaken = i;
                                this->run.processList[0] = pToMove;
                                
                            }
                        }
                    }
                }
                if (run.processList[0].firstTimeInQ == -1) {
                    this->ready.processList[pTaken] = Process(-1,0,0,0);
                    this->run.processList[0].firstTimeInQ = t - run.processList[0].cpuBursts;
                }
                
                
                int timeNeeded;
                //Actually run process
                if(t > 6) {
                    timeNeeded = t - this->run.processList[0].cpuBursts - this->complete.processList[complete.position - 1].completionTime;
                }
                else {
                    timeNeeded = t - this->run.processList[0].cpuBursts;
                }
                

                //Remove process from run Queue and to Completed
                if (timeNeeded == 0) {
                    this->run.processList[0].completionTime = t;
                    this->complete.processList[complete.position] = this->run.processList[0];
                    this->run.processList[0] = Process(-1,0,0,0);
                    this->complete.position++;
                    
                }

                //This was added for debugging, took a lot longer than you'd think
                //std::cout << "Complete Position: " << complete.position <<  " " << complete.processList[complete.position - 1].completionTime << "\n";
                //std::cout << "Current Time: " << t << "\n";
                //std::cout <<"Current Run Queue: " << run.processList[0].pid << "\n";
       
            }

        }   
        else if (this->type == "rr") {
            int runtime = getRuntime();
            int timeQ = 10;
            int pos = 0;

            for (int t = 0; t <= runtime; t++) {
                //Move all processes to ready Queue as time marches on
                for (int i = 0; i < 25; i++) {
                    if(this->newProcess.processList[i].arrivalTime == t && ready.position < 25) {
                        this->ready.processList[ready.position] = this->newProcess.processList[i];
                        this->newProcess.processList[i] = Process(-1,0,0,0);
                        this->ready.position++;      
                    }
                }

                //Select a process to move to run Queue
                for (int i = 0; i < 25; i++) {
                    if(ready.processList[i].pid != -1) {
                        this->run.processList[0] = ready.processList[i];
                        this->run.processList[0].firstTimeInQ = t;
                        break;
                    }
                }


                
                
                this->run.processList[0].tempCpuBursts = run.processList[0].tempCpuBursts - 1;
                timeQ = timeQ - 1;
                if (this->run.processList[0].tempCpuBursts <= 0) {
                    this->run.processList[0].completionTime = t;
                    this->complete.processList[complete.position] = run.processList[0];
                    this->run.processList[0] = Process(-1,0,0,0);
                    this->complete.position++;
                    timeQ = 10;
                }
                else if (timeQ == 0) {
                    this->ready.processList[this->ready.position - 1] = this->run.processList[0];
                    this->run.processList[0] = Process(-1,0,0,0);
                    timeQ = 10;
                }
                
            }
            
            
        } 
        else {
            std::cout << "The Type of scheduler is invalid or not implemented\n\n";
        }
        //End of scheduler 
        
    };

    int getRuntime() {
        int sum = 0;
        for (int i = 0; i < 25; i++) {
            if(this->newProcess.processList[i].pid != -1) {
                sum += this->newProcess.processList[i].cpuBursts;
            }
        }
        return sum;
    };

    void showProcessStats() {
        std::cout << "\n\nProcess Stats: " << this->type <<"\n\n";
        for (int i = 0; i < this->complete.position; i++) {
            //Alot of formatting
            if (this->complete.processList[i].pid != -1) {
            std::cout << "ProcessID    Arrival Time    cpuBurst    Priority    Completed Time    First Time In Queue\n";
            std::cout << "   " << this->complete.processList[i].pid << "            ";
            std::cout << this->complete.processList[i].arrivalTime << "              ";
            std::cout << this->complete.processList[i].cpuBursts << "           ";
            std::cout << this->complete.processList[i].priority << "            ";
            std::cout << this->complete.processList[i].completionTime << "                   ";
            std::cout << this->complete.processList[i].firstTimeInQ << "\n";
            }
        }
    };

};


//Main Program
int main() { 
    
    scheduler fcfs = scheduler("fcfs", "./jobs.txt");
    fcfs.readFileContents();
    fcfs.schedulingSimulation();

    std::cout << "New Process Queue: " << fcfs.newProcess.showList() << "\n";
    std::cout << "Ready Queue: " << fcfs.ready.showList() << "\n";
    std::cout << "Run Queue: " << fcfs.run.showList() << fcfs.run.processList[0].cpuBursts  << " " << fcfs.run.processList[0].arrivalTime << "\n";
    fcfs.showProcessStats();
    //fcfs.runStats();
    

    /*
    scheduler sjf = scheduler("sjf", "./jobs.txt");
    sjf.readFileContents();
    sjf.schedulingSimulation();
    std::cout << "New Process Queue: " << sjf.newProcess.showList() << "\n";
    std::cout << "Ready Queue: " << sjf.ready.showList() << "\n";
    std::cout << "Run Queue: " << sjf.run.showList() << sjf.run.processList[0].cpuBursts  << " " << sjf.run.processList[0].arrivalTime << "\n";
    sjf.showProcessStats();
    sjf.runStats();
    */

    /*
    scheduler rr = scheduler("rr", "./jobs.txt");
    rr.readFileContents();
    rr.schedulingSimulation();
    std::cout << "New Process Queue: " << rr.newProcess.showList() << "\n";
    std::cout << "Ready Queue: " << rr.ready.showList() << "\n";
    std::cout << "Run Queue: " << rr.run.showList() << " cpu bursts: " << rr.run.processList[0].cpuBursts  << "   arrival time: " << rr.run.processList[0].arrivalTime << "   temp cpu bursts: " << rr.run.processList[0].tempCpuBursts <<"\n";
    std::cout << "Complete Queue: " << rr.complete.showList() << "\n";
    rr.showProcessStats();
    rr.runStats();
    */

    return 0;
}