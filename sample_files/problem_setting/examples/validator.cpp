#include <cassert>
#include <cstdio>
#include <iostream>
#include <regex.h>
#include <string>

namespace regex_helpers {

regex_t compile(const char *pattern) {
  regex_t re;
  assert(regcomp(&re, pattern, REG_EXTENDED | REG_NOSUB) == 0);
  return re;
}

bool match(regex_t re, const std::string &text) {
  return regexec(&re, text.c_str(), 0, NULL, 0) == 0;
}

} // namespace regex_helpers

void readSpace() { assert(std::cin.get() == ' '); }
void readNewLine() { assert(std::cin.get() == '\n'); }
void readEOF() { assert(std::cin.get() == EOF); }

std::string readToken(char min_char = 0, char max_char = 127) {
  static constexpr size_t MAX_TOKEN_SIZE = 1e7;
  std::string token;
  int c = std::cin.get();
  assert(!isspace(c));
  while (!isspace(c) && c != EOF) {
    assert(token.size() < MAX_TOKEN_SIZE);
    assert(min_char <= c && c <= max_char);
    token.push_back(char(c));
    c = std::cin.get();
  }
  std::cin.unget();
  return token;
}

bool validateInt(const std::string &token) {
  if (token == "0") {
    return true;
  }
  auto i = 0u;
  if (i < token.size() && token[i] == '-') {
    ++i;
  }
  if (i >= token.size() || token[i] < '1' || '9' < token[i]) {
    return false;
  }
  for (i++; i < token.size(); i++) {
    if (token[i] < '0' || '9' < token[i]) {
      return false;
    }
  }
  return true;
}

long long readInt(long long lo, long long hi) {
  std::string token = readToken();
  assert(validateInt(token));

  long long parsedInt = stoll(token); // May throw.
  assert(lo <= parsedInt && parsedInt <= hi);
  return parsedInt;
}

// NOTE: `readLine(), readFloat(), and readIntArray()` were removed for brevity.

int main() {
  int N = readInt(1, 1e5);
  for (int i = 0; i < N; i++) {
    int A = readInt(-1e9, 1e9);
    readSpace();
    int B = readInt(-1e9, 1e9);
    readNewLine();

    std::cout << A + B << std::endl;
  }
  // A common mistake is forgetting to read EOF: be careful!
  readEOF();
}
