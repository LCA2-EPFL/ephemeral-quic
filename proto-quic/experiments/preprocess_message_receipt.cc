#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <fstream>

using namespace std;

int main(int argc, char *argv[]) {
  ifstream myfile;
  myfile.open(argv[1]);
  set<int> valid_ids;
  int id, delay;
  char ch;
  while (myfile >> id >> ch >> delay) {
    if (delay < 100000) {
      valid_ids.insert(id);
    }
  }
  myfile.close();
  long long start_time = -1;
  long long relative_timestamp = -1;
  string s;
  while (cin >> s) {
    if (s.find("sending_timestamp") != std::string::npos) {
      cin >> s;
      int pos = s.find(":");
      if (pos != -1) {
        string timestamp_str = s.substr(0, pos);
        long long timestamp = std::stoll(timestamp_str);
        if (start_time == -1) {
          start_time = timestamp;
        }
        relative_timestamp = timestamp - start_time;
       // cout << relative_timestamp << ",";
      }
    } else if (s.length() >= 3 && s.substr(0, 3) == "ID:") {
      string stream_id_str = s.substr(3);
      int stream_id = std::stoi(stream_id_str);
      int message_id = (stream_id - 5) / 2; // start from 0
      if (valid_ids.find(message_id) != valid_ids.end()) {
        cout << relative_timestamp << "," << message_id << endl;
      }
     // cout << message_id << endl;
    }
  }
}
