#include <iostream>
#include <vector>
#include <algorithm>
#include <string> 
#include <cstdlib>

using namespace std;

int main() {
  int message_id;
  double delay;
  char ch;
  long long total_delay = 0;
  int count_all = 0;
  vector<double> delays;
  string s;
  while (cin >> delay) {
    delays.push_back(delay);
    count_all++;
  }
  double mean_delay = (double)total_delay / count_all;
  sort(delays.begin(), delays.end());
  // cout << "Mean delay: " << mean_delay <<  endl;
  cout << count_all << endl;
  cout << "Median delay: " << delays[count_all/2] << endl;
  cout << "90-th percentile: " << delays[count_all * 9 / 10]<< endl;
  cout << "99-th percentile: " << delays[count_all * 99 / 100]<< endl;
  cout << "Max delay: " <<  delays.back() << endl;
  return 0;
}
