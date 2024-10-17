#include <iostream>
#include <mutex>
#include <thread>
#include <unistd.h>
#include <atomic>
#include <vector>

using namespace std;

mutex potEmpty;
mutex potFull;
mutex mut;
atomic<int> serving(7);

void putServingsInPot(unsigned M){
    serving += M;

}

void getServingFromPot(){
    serving--;
}

void savage(unsigned id){
    while(true){

        if(serving <= 0){
            potEmpty.unlock();
            potFull.lock();
        }

        cout << "Savage n°" << id << " Takes a meal" << endl;
        getServingFromPot();
        sleep(2);
    }
}

void cook() {
    while(true){
        potEmpty.lock();
        cout << "Pot is empty reloading..." << endl;
        sleep(2);
        cout << "Pot reloaded" << endl;
        putServingsInPot(7);
        potFull.unlock();
    }
}


int main()
{
    potEmpty.lock();
    potFull.lock();

    thread firstCook(cook);

    vector<thread> v_thread;

    for(unsigned i = 0; i < 100; ++i){
        v_thread.push_back(thread(savage,i));
    }

    for(auto& thread : v_thread){
        thread.join();
    }



    return 0;
}
