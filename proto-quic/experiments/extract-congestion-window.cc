#include <iostream>
#include <string>

// Filter congestion window values from the client output
// Example usage: ./extract-congestion-window < ~/Desktop/lca2/delay_client_server.txt

int main() {
  std::string s;
  int congestion_window;
  while (std::cin >> s) {
    if (s == "CongestionWindowAfterAck:" || s == "current_congestion_window:") {
      std::cin >> congestion_window;
      std::cout << congestion_window << std::endl;
    }
  }
}
