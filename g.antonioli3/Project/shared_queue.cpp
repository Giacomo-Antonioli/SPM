//
// Created by giaco on 30/06/2022.
//

#include <stdlib.h>
#include <vector>
#include <functional>
#include <iostream>
#include <vector>
#include <functional>
#include <thread>
#include <utility>
#include <queue>
#include <condition_variable>

template<typename QTYPE> //Create my own shared queue

class sharedQueue {

private:
    std::queue <QTYPE> q;       //Queue
    std::mutex m;               //Mutex to protect the queue
    std::condition_variable c;  //Condition variable to notify queue usage

public:
    QTYPE get_element() {
        std::unique_lock <std::mutex> l(m);
        while (q.empty())
            c.wait(l);
        QTYPE task = q.front();
        q.pop();
        return task;
    }

    void add_element(QTYPE element) {
        std::unique_lock <std::mutex> l(m);
        q.push(element);
        c.notify_all();
    }

};
