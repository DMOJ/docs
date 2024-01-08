#include <algorithm>

#include "validator.cpp"

// A validator that uses `readToken` and `readLine`.

int main() {
  std::string first = readToken('A', 'B');
  readSpace();
  std::string rest = readLine();
  assert(std::all_of(rest.begin(), rest.end(), [](char c) {
    return isspace(c) || 'a' <= c && c <= 'z';
  }));
  assert(rest.size() > 0 && rest.back() != '\n');
  std::cout << rest.front() + rest.back() << std::endl;

  std::string secondLine = readLine('a', 'z');
  std::cout << secondLine.size() << std::endl;

  readEOF();
}
