#include <vector>
#include <iostream>

using namespace std;

int main() {
  double timestamp, delay;
  int count_all, count_in_time;
  int message_id;
  int count_out_of_order = 0;
  int highest_message_id = -1;
  vector<double> delays;
  while (cin >> timestamp >> delay >> count_all >> count_in_time >> message_id) {
    delays.push_back(delay);
    if (message_id <= highest_message_id) {
      count_out_of_order++;
    } else {
      highest_message_id = message_id;
    }
  }
  int count_late = count_all - count_in_time;
  int count_valid = count_in_time - count_out_of_order;
  cout << "All messages received: " << count_all  << ", " << count_all / 360.0 << "%" << endl;
  cout << "Out-of-order messages received: " << count_out_of_order << ", " << count_out_of_order / 360.0 << "%" << endl;
  cout << "Late messages received: " << count_late << ", " << count_late / 360.0 << "%" << endl;
  cout << "Valid messages received: " << count_valid << ", " << count_valid / 360.0 << "%" << endl;
  cout << "Median delay(ms): " << delays[count_all/2] << endl;
  cout << "90-th percentile(ms): " << delays[count_all * 9 / 10]<< endl;
  cout << "99-th percentile(ms): " << delays[count_all * 99 / 100]<< endl;
  cout << "Max delay(ms): " <<  delays.back() << endl;
}
