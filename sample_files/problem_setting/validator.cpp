#include <cassert>
#include <cstdio>
#include <iostream>
#include <regex.h>
#include <string>
#include <vector>

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

std::string readLine(char min_char = 0, char max_char = 127) {
  static constexpr size_t MAX_LINE_SIZE = 1e7;
  std::string line;
  int c = std::cin.get();
  while (c != '\n') {
    assert(line.size() < MAX_LINE_SIZE);
    assert(c != EOF);
    assert(min_char <= c && c <= max_char);
    line.push_back(char(c));
    c = std::cin.get();
  }
  return line;
}

long long readInt(long long lo, long long hi) {
  static regex_t re = regex_helpers::compile("^(0|-?[1-9][0-9]*)$");
  std::string token = readToken();
  assert(regex_helpers::match(re, token));

  long long parsedInt = stoll(token); // May throw.
  assert(lo <= parsedInt && parsedInt <= hi);
  return parsedInt;
}

long double readFloat(long double min, long double max,
                      long double eps = 1e-9) {
  static regex_t re = regex_helpers::compile("^-?(0|[1-9][0-9])(\\.[0-9]+)?$");
  std::string token = readToken();
  assert(regex_helpers::match(re, token));
  long double parsedDouble = stold(token); // May throw.
  assert(min - eps <= parsedDouble && parsedDouble <= max + eps);
  return parsedDouble;
}

template <typename T>
std::vector<T> readIntArray(size_t N, long long lo, long long hi) {
  std::vector<T> arr;
  arr.reserve(N);
  for (size_t i = 0; i < N; i++) {
    if (i) {
      readSpace();
    }
    arr.push_back(readInt(lo, hi));
  }
  readNewLine();
  return arr;
}
