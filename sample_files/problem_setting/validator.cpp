#include <algorithm>
#include <cassert>
#include <cstdio>
#include <regex.h>
#include <string>
#include <vector>

void readSpace() { assert(getchar() == ' '); }
void readNewLine() { assert(getchar() == '\n'); }
void readEOF() { assert(getchar() == EOF); }

std::string readToken(char min_char = 0, char max_char = 127) {
  static constexpr size_t MAX_TOKEN_SIZE = 1e7;
  std::string token;
  int c = getchar();
  assert(!isspace(c));
  while (!isspace(c) && c != EOF && token.size() < MAX_TOKEN_SIZE) {
    assert(min_char <= c && c <= max_char);
    token.push_back(char(c));
    c = getchar();
  }
  ungetc(c, stdin);
  return token;
}

// We need a regex because of how lax stoll and stold are.
namespace regex_detail {
regex_t compile(const char *pattern) {
  regex_t re;
  assert(regcomp(&re, pattern, REG_EXTENDED | REG_NOSUB) == 0);
  return re;
}
bool match(regex_t re, const std::string &text) {
  return regexec(&re, text.c_str(), 0, NULL, 0) == 0;
}
} // namespace regex_detail

long long readInt(long long lo, long long hi) {
  static regex_t re = regex_detail::compile("^0|-?[1-9][0-9]*$");
  std::string token = readToken();
  assert(regex_detail::match(re, token));

  long long parsedInt = stoll(token);
  assert(lo <= parsedInt && parsedInt <= hi);
  return parsedInt;
}

template <typename T>
std::vector<T> readIntArray(size_t N, long long lo, long long hi) {
  std::vector<T> arr;
  arr.reserve(N);
  for (size_t i = 0; i < N; i++) {
    arr.push_back(readInt(lo, hi));
    if (i != N - 1) {
      readSpace();
    }
  }
  readNewLine();
  return arr;
}

long double readFloat(long double min, long double max,
                      long double eps = 1e-9) {
  static regex_t re = regex_detail::compile("^-?(0|[1-9][0-9])(\\.[0-9]+)?$");
  std::string token = readToken();
  assert(regex_detail::match(re, token));
  long double parsedDouble = stold(token);
  assert(min - eps <= parsedDouble && parsedDouble <= max + eps);
  return parsedDouble;
}
