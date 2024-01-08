#include <numeric>

#include "validator.cpp"

int main() {
  int N = readInt(1, 10);
  readSpace();
  int M = readInt(1, 100);
  readNewLine();

  auto arr = readIntArray<int>(N, 1, M);
  readEOF();

  std::cout << std::accumulate(arr.begin(), arr.end(), 0) << std::endl;
}
