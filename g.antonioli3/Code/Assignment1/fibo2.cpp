#include <iostream>
#include <vector>
#include "utimer.cpp"
#include <thread>
#include <utility>
#include <future>

using namespace std;

int fibonacci(int n) {
  if(n<2)
    return 1;
  else
    return (fibonacci(n-1) + fibonacci(n-2));
}

int count_time(int n){
  START(time);
  int result = fibonacci(n);
  STOP(time,elapsed);
  return elapsed;
}





int main(int argc, char * argv[]) {
  
  if(argc == 4) {
    cout << "Usage is: " << argv[0] << " v-len max-v seed trials" << endl;
    return(0);
  }

  int n    = atoi(argv[1]);
  int max  = atoi(argv[2]);
  int seed = atoi(argv[3]);
  int trials = atoi(argv[4]);


  vector<int> f(n), rseq(n), rpar(n);
  vector<shared_future<int>> rpar_aysnc(n);  // necessario perchè altrimenti il secondo get fallisce ...
  vector<size_t> elap_par(trials);
  vector<shared_future<size_t>> async_par(trials);
  srand(seed);
  
  for(int i=0; i<n; i++) {
    f[i] = rand() % max;
  }
  cout<<"Baseline example: "<<endl;
  {
    utimer tim("sequential");

    for(int i=0; i<n; i++)
      rseq[i] = fibonacci(f[i]);
  }

  //////////////////////////////////////////

  {

    //utimer tim("parallel for static");
    auto pardegree = thread::hardware_concurrency();
    std::cout<<"Par degree is: "<<pardegree<<endl;

    vector<pair<int,int>> chunks(pardegree);
    int chunk_size = n / pardegree;
    for(int i=0; i<pardegree; i++) {
      if(i!=(pardegree-1))
        chunks[i] = make_pair(i*chunk_size, (i+1)*chunk_size-1);
      else
        chunks[i] = make_pair(i*chunk_size, n-1);
    }

  /*  for(int i=0; i<pardegree; i++)
      cout << "chunk " << i << " is " << chunks[i].first << "," << chunks[i].second << endl;*/

    auto body = [&] (int j) {
        for(int i=chunks[j].first; i<=chunks[j].second; i++)
          rpar[i] = count_time(f[i]);
        return;
    };

    vector<thread*> tids(pardegree);
    START(paroutertime);
    for(int i=0; i<pardegree; i++)
      tids[i] = new thread(body, i);
    for(int i=0; i<pardegree; i++)
      tids[i]->join();
    STOP(paroutertime,outertime);
    size_t parInner=0;
    for(int i=0; i<n; i++)
    parInner+=rpar[i];
  }

  {
    utimer tim("parallel async_all");

    for(int i=0; i<n; i++)
      rpar_aysnc[i] = async(launch::async, fibonacci, f[i]);
    // asycn launched, now wait results
    for(int i=0; i<n; i++)
      try {
        rpar_aysnc[i].get();
      } catch (const std::exception& e) {
	      cout << "Got " << e.what() << " i= " << i << endl; 
      }
  }
  
  {
    utimer("Checking");
    bool same = true;
    for(int i=0; i<n; i++) {
      try {
	if(rseq[i] != rpar_aysnc[i].get()) {
	  same = false;
	  break;
	}
      }  catch (const std::exception& e) {
	cout << "Got " << e.what() << " i= " << i << endl; 
      }

    }

    cout << "Sequential and parallel results are "
	 << (same ? "the same" : "different") << endl;
  }
  
  return(0);
}
    

							  
