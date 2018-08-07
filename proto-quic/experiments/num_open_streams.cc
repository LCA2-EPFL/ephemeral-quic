#include <iostream>
#include <string>

using namespace std;

int main() {
  string s;
  while (cin >> s) {
    if (s.find("GetNumOpenOutgoingStreams") != std::string::npos) {
      cout << s.substr(26) << endl;
    }
  }
}
