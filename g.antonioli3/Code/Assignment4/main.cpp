//
// Created by giaco on 27/04/2022.
//
#include <iostream>
#include <vector>
#include <functional>
#include "utimer.cpp"
#include <thread>
#include <utility>
#include <queue>
#include <condition_variable>

#define DELAY 10ms

template<typename QTYPE> //Create my own shared queue
class sharedQueue {

private:
    std::queue <QTYPE> q;       //Queue
    std::mutex m;               //Mutex to protect the queue
    std::condition_variable c;  //Condition variable to notify queue usage

public:
    QTYPE get_task() {
        std::unique_lock <std::mutex> l(m);
        while (q.empty())
            c.wait(l);
        QTYPE task = q.front();
        q.pop();
        return task;
    }

    void add_task(QTYPE element) {
        std::unique_lock <std::mutex> l(m);
        q.push(element);
        c.notify_one();
    }

};

sharedQueue<function < void()>> shared_q;


void active_delay(int msecs) {
    cout<<"OYOOYOY   "<<msecs<<endl;
    auto start = chrono::high_resolution_clock::now();
    auto end = false;
    while (!end) {

        auto elapsed = chrono::high_resolution_clock::now() - start;
        auto msec = chrono::duration_cast<chrono::microseconds>(elapsed).count();
        if (msec > msecs)
            end = true;
    }
    return;
}


class function_handler {

private:
    int number_of_functions;
public:
    function_handler(int number_of_functions)
    {
        this->number_of_functions=number_of_functions;
    }

    void generate_functions() {
        for (int i = 0; i < number_of_functions; i++) {
            cout<<"EMITTER# "<<i<<endl;
            auto f1 = std::bind(active_delay,rand() % 1000);
            shared_q.add_task(f1);
        }
    }

};


class ThreadPool{

private:
    vector<thread> threads_ids;
public:
    ThreadPool(int workers)
    this->thread_ids.push_back(thread())

};


int main(int argc, char *argv[]) {
    if (argc != 4) {
        cout << "Threadpool usage: [seed] [n] [nw]" << endl;
        return -1;
    }

    int seed = atoi(argv[1]);
    int n = atoi(argv[2]);
    int nw = atoi(argv[3]);

    srand(seed);

    function_handler emitter(n);
    emitter.generate_functions();
    for(int i=0;i<n;i++){
        auto f=shared_q.get_task();
        f();
    }
    return 0;
}
