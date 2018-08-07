#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
  int message_id, delay;
  char ch;
  long long total_delay = 0;
  int max_delay = 0;
  int count_all = 0;
  int count_valid = 0;
  int count_late = 0;
  int count_out_of_order = 0; // -> old & late
  int count_latest_but_late = 0;
  int max_message_id = -1;
  vector<int> delays;
  while (cin >> message_id >> ch >> delay) {
    count_all++;
    total_delay += delay;
    delays.push_back(delay);
    if (delay <= 100000 && message_id > max_message_id) {
      count_valid++;
    } else {
      if (delay > 100000) {
        count_late++;
      }
      if (message_id < max_message_id) {
        count_out_of_order++;
      }
      if (message_id >= max_message_id && delay > 100000) {
        count_latest_but_late++;
      }
    }
    max_message_id = max(max_message_id, message_id);
  }
  cout << "All messages received: " << count_all << ", " << count_all / 360.0 << "%" << endl;
  cout << "Late messages received: " << count_late << ", " << count_late / 360.0 << "%" << endl;
  cout << "Out-of-order messages received: " << count_out_of_order << ", " << count_out_of_order / 360.0 << "%" << endl;
  cout << "Latest but late: " << count_latest_but_late << ", " << count_latest_but_late / 360.0 << "%" << endl; 
  cout << "Valid messages received: " << count_valid << ", " << count_valid / 360.0 << "%" << endl;

  double mean_delay = (double)total_delay / count_all;
  sort(delays.begin(), delays.end());
  cout << "Mean delay(ms): " << mean_delay / 1000.0 <<  endl;
  cout << "Median delay(ms): " << delays[count_all/2] / 1000.0 << endl;
  cout << "90-th percentile(ms): " << delays[count_all * 9 / 10] << " " << delays[count_all * 9 / 10] / 1000.0 << endl;
  cout << "99-th percentile(ms): " << delays[count_all * 99/ 100] << " " << delays[count_all * 99 / 100] / 1000.0 << endl;
  cout << "Max delay(ms): " << delays.back() / 1000.0 << endl;
  return 0;
}
