#include <iostream>
#include <mutex>
#include <thread>
#include <unistd.h>
#include <atomic>

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
        mut.lock();

        if(serving == 0){
            potEmpty.unlock();
            potFull.lock();
        }

        cout << "Savage n°" << id << " Takes a meal" << endl;
        getServingFromPot();
        mut.unlock();
        sleep(rand() % 10);
    }
}

void cook() {
    while(true){
        potEmpty.lock();
        cout << "Pot is empty reloading..." << endl;
        sleep(5);
        cout << "Pot reloaded" << endl;
        putServingsInPot(7);
    }
}


int main()
{
    potEmpty.lock();
    potFull.lock();

    thread firstCook(cook);
    thread firstSavage(savage,1);
    thread secondSavage(savage,2);
    thread thirdSavage(savage, 3);

    firstCook.join();
    secondSavage.join();
    thirdSavage.join();

    return 0;
}
