//
// Created by giaco on 30/06/2022.
//

#ifndef SPM_SHARED_QUEUE_H
#define SPM_SHARED_QUEUE_H
#include <queue>
#include <condition_variable>

using namespace std;

template<typename QTYPE> //Create my own shared queue
class sharedQueue {

private:
    std::queue <QTYPE> q;       //Queue
    std::mutex m;               //Mutex to protect the queue
    std::condition_variable c;  //Condition variable to notify queue usage

public:
    QTYPE get_task();

    void add_task(QTYPE element);

};


#endif //SPM_SHARED_QUEUE_H
