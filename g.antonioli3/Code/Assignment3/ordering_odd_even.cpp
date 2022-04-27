//
// Created by giaco on 23/03/2022.
//
#include <iostream>
#include <vector>
#include "utimer.cpp"
#include <thread>
#include <utility>
#include <future>

using namespace std;
std::condition_variable cv;
std::mutex cv_m;
std::condition_variable cv1;
std::mutex cv_m1;

std::atomic<int> test(0);
std::atomic<int> swap_count(0);
std::atomic<int> stop(0);
std::atomic<int> i(0);


int odd_orderer_new(vector<int> &array, int start, int end, int n, int pardegree) {

    while(stop==0) {
        std::cout << "EVEN" << endl;

        for (int i = start; i <= end; i += 2) {
            if (i + 1 < n && array[i] > array[i + 1]) {
                swap(array[i], array[i + 1]);
                swap_count++;
            }
        }
        {
            std::lock_guard <std::mutex> lk(cv_m1);
            test += 1;
           std::cerr << "Modifying test"<<test<<"\n";

        } //std::cout << "SVEGLIO MAIN" << endl;
        cv1.notify_all();
        {
            std::unique_lock <std::mutex> lk(cv_m);

           // std::cerr << "Locking cv_m \n";

            //std::cerr << "TEST WAITIMG: " << test;
            //std::cout << "DORMO THREAD" << endl;
            cv.wait(lk, [] { return i == 1; });
            //std::cerr << "...finished waiting on cv_m\n";

        }
        //std::this_thread::sleep_for(std::chrono::seconds(1));


        std::cout << "ODD" << endl;
        for (int i = start + 1; i <= end - 1; i += 2) {
            if (i + 1 < n && array[i] > array[i + 1]) {
                swap(array[i], array[i + 1]);
                swap_count++;
            }
        }

        {
            std::lock_guard <std::mutex> lk(cv_m1);
            test += 1;
           // std::cerr << "Modifying test\n";
           // std::cout << "SVEGLIO MAIN" << endl;

        }
        cv1.notify_all();
        {
            std::unique_lock <std::mutex> lk(cv_m);
           // std::cerr << "Locking cv_m \n";
           // std::cout << "DORMO THREAD" << endl;

            cv.wait(lk, [] { return i == 0; });

            std::cerr << "\t\t\ti= "<<i<<"\n";
            //std::cerr <<"\t\t\t "<<stop<<std::endl;
        }
    }
/*    {
        std::unique_lock <std::mutex> lk(cv_m);
        cv.wait(lk, [] { return i == 0; });
    }*/
    return 0;
}


int main(int argc, char *argv[]) {

    if (argc != 5) {
        cout << "Usage is: " << argv[0] << " array-len max-random seed pardegree" << endl;
        return (0);

    }
    if (atoi(argv[4]) % 2 == 1) {
        cout << "Use a multiple of 2 as pardegree" << endl;
        return (0);
    }

    int n = atoi(argv[1]);
    int max = atoi(argv[2]);
    int seed = atoi(argv[3]);
    int pardegree = atoi(argv[4]);

    vector<int> array(n);
    srand(seed);


    vector <pair<int, int>> chunks(pardegree);
    int chunk_size = n / (pardegree);

    for (int i = 0; i < pardegree; i++) {
        if (i != (pardegree - 1)) {
            chunks[i] = make_pair(i * chunk_size, (i + 1) * chunk_size - 1);

        } else {

            chunks[i] = make_pair(i * chunk_size, n - 1);

        }

    }
    cout << "chunks" << endl;
    for (int i = 0; i < pardegree; i++)
        cout << "chunk " << i << " is " << chunks[i].first << "," << chunks[i].second << endl;
    cout << endl;
    for (int i = 0; i < n; i++) {
        array[i] = rand() % max;
    }

    auto body = [&](int j, int n, int pardegree) {

        odd_orderer_new(array, chunks[j].first, chunks[j].second, n, pardegree);
        //still_active=still_active-1;
        //cout << chunks[j].first<<"   "<<chunks[j].second<<endl;
        return;
    };


    vector < thread * > tids(pardegree);
    {
        utimer tim("parallel async_all");
        for (int i = 0; i < pardegree; i++) {
            tids[i] = new thread(body, i, n, pardegree);
        }
        while (stop == 0) {
            //  std::this_thread::sleep_for(std::chrono::seconds(2));

            {
                std::unique_lock <std::mutex> lk(cv_m1);
               // std::cerr << "Locking on cv_m1 \n";
                // i=0;
                std::cout << "MAIN DORMO" << endl;
                cv1.wait(lk, [pardegree] { return test == pardegree; });
                std::cout << "MAIN SVEGLIO" << endl;
                //test=0;
                i = 0;
                test = 0;
                //std::cerr << "...finished waiting cv_m1\n";
                //std::cout << "IN SIG SWAP " << swap_count << std::endl;

            }
            {
                std::lock_guard <std::mutex> lk(cv_m);
                i = 1;
               // std::cerr << "Modifying i\n";

            }
            std::cout << "SVEGLIO THREAD" << endl;
            cv.notify_all();




            {
                std::unique_lock <std::mutex> lk(cv_m1);
                //std::cerr << "Locking on cv_m1 \n";
                // i=0;
                std::cout << "MAIN DORMO" << endl;
                cv1.wait(lk, [pardegree] { return test == pardegree; });
                std::cout << "MAIN SVEGLIO" << endl;
                //test=0;
                i = 0;
                test = 0;
                //std::cerr << "...finished waiting cv_m1\n";
               // std::cout << "IN SIG SWAP " << swap_count << std::endl;

            }
/*            {
                std::lock_guard <std::mutex> lk(cv_m);
                i = 1;
                //std::cerr << "Modifying i\n";

            }
            cv.notify_all();*/
            {
                std::lock_guard <std::mutex> lk(cv_m);

             //   std:cerr<<"TEST "<<test<<endl;
                i = 0;

                if (swap_count == 0) {
                    stop = 1;
                } else {
                    swap_count = 0;
                }
                std::cerr << "\t\t\t\t\t\t\tStop "<<stop<<endl;


            }
            std::cout << "SVEGLIO THREAD" << endl;
            cv.notify_all();
        }
        for (int i = 0; i < pardegree; i++)
            tids[i]->join();
    }

}