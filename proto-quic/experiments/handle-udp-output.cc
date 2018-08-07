#include <iostream>

using namespace std;

int main() {
  double timestamp, delay;
  int count_all, count_non_outdated, message_id;
  int count_old_messages = 0;
  int highest_message_id = -1;
  while (cin >> timestamp >> delay >> count_all >> count_non_outdated >> message_id) {
    if (message_id <= highest_message_id) {
      count_old_messages++;
    } else {
      highest_message_id = message_id;
    }
  }
  cout << "Count of all messages received: " << count_all << endl;
  cout << "Count of valid messages received: " << count_non_outdated - count_old_messages << endl;
  cout << "Count of outdated messages: " << count_all - count_non_outdated << endl;
  cout << "Count of old messages: " << count_old_messages << endl;
}
