//
// Created by giaco on 23/03/2022.
//

#include <iostream>
#include <condition_variable>
#include <thread>
#include <chrono>
#include <atomic>

std::condition_variable cv;
std::mutex cv_m;
std::condition_variable cv1;
std::mutex cv_m1;

std::atomic<int> test(0);
std::atomic<int> swap(0);
std::atomic<int> stop(0);

// This mutex is used for three purposes:
// 1) to synchronize accesses to i
// 2) to synchronize accesses to std::cerr
// 3) for the condition variable cv
int i = 0;

void waits() {
    while (stop==0) {
        swap += 1;
        {
            std::unique_lock <std::mutex> lk(cv_m);
            //std::cerr << "Locking cv_m \n";


            cv.wait(lk, [] { return i == 1; });
            //std::cerr << "...finished waiting on cv_m\n";

        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
        {
            std::lock_guard <std::mutex> lk(cv_m1);
            test += 1;
            //std::cerr << "Modifying test\n";

        }
        cv1.notify_all();
        {
            std::unique_lock <std::mutex> lk(cv_m);
            //std::cerr << "Locking cv_m \n";


            cv.wait(lk, [] { return i == 0; });
            //std::cerr << "...finished waiting on cv_m\n";

        }

    }
    /*  {
          std::unique_lock <std::mutex> lk(cv_m);
          std::cerr << "Waiting... \n";



          cv.wait(lk, [] { return i == 1; });
          std::cerr << "...finished waiting. i == 1\n";

      }
  */
}

void signals() {

    //std::this_thread::sleep_for(std::chrono::seconds(0));


/*  std::this_thread::sleep_for(std::chrono::seconds(2));
  {
      std::lock_guard<std::mutex> lk(cv_m);
      i=1;
      std::cerr << "Notifying...\n";

  }
  cv.notify_all();*/

}

int main() {
    std::thread t1(waits), t2(waits), t3(waits), t4(waits);
    while (stop==0) {
        std::this_thread::sleep_for(std::chrono::seconds(2));

        {
            std::lock_guard <std::mutex> lk(cv_m);
            i = 1;
            //std::cerr << "Modifying i\n";

        }
        cv.notify_all();

        {
            std::unique_lock <std::mutex> lk(cv_m1);
            //std::cerr << "Locking on cv_m1 \n";
            // i=0;

            cv1.wait(lk, [] { return test == 4; });
            //test=0;
            i = 0;
            test = 0;
            //std::cerr << "...finished waiting cv_m1\n";
            std::cout << "IN SIG SWAP " << swap << std::endl;

        }
        {
            std::lock_guard <std::mutex> lk(cv_m);
            i = 0;
            //std::cerr << "Modifying i\n";
            if(swap>19){
                stop=1;
            }

        }
        cv.notify_all();
    }

    t1.join();
    t2.join();
    t3.join();
    t4.join();
    std::cout << swap << std::endl;
}