#include <iostream>
#include <vector>
#include <string>

using namespace std;

int main() {
  string s;
  while (cin >> s) {
    if (s.find(':') != std::string::npos) {//[s.size() - 1] == ':') {
      cout << s.substr(0, s.size() - 1) << " ";
    }
    if (s.find("[mov.avg]") != std::string::npos) {
      int pos = s.find("[mov.avg]");
      cout << s.substr(0, pos) << endl;
    }
  }
}
