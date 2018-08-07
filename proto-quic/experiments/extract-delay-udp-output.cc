#include <iostream>
#include <string>

using namespace std;

int main() {
  double timestamp;
  string delay;
  int count_all, count_non_outdated, message_id;
  while (cin >> timestamp >> delay >> count_all >> count_non_outdated >> message_id) {
    cout << delay << endl;
  }
}
