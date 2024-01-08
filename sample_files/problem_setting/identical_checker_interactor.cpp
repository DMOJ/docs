#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <regex.h>
#include <stdexcept>
#include <string>
#include <vector>

void errorHook();
void exitWA() {
  errorHook();
  std::exit(1);
}
void exitPE() {
  errorHook();
  std::exit(2);
}
void assertWA(bool condition) {
  if (!condition) {
    exitWA();
  }
}
void assertPE(bool condition) {
  if (!condition) {
    // Defined for `testlib` and `coci`.
    exitPE();
  }
}
void readSpace() { assertPE(getchar() == ' '); }
void readNewLine() { assertPE(getchar() == '\n'); }
void readEOF() { assertPE(getchar() == EOF); }
std::string readToken(char min_char = 0, char max_char = 127) {
  static constexpr size_t MAX_TOKEN_SIZE = 1e7;
  std::string token;
  int c = getchar();
  assertPE(!isspace(c));
  while (!isspace(c) && c != EOF && token.size() < MAX_TOKEN_SIZE) {
    assertWA(min_char <= c && c <= max_char);
    token.push_back(char(c));
    c = getchar();
  }
  ungetc(c, stdin);
  return token;
}
namespace regex_detail {
regex_t compile(const char *pattern) {
  regex_t re;
  if (regcomp(&re, pattern, REG_EXTENDED | REG_NOSUB) != 0) {
    throw std::runtime_error("Pattern failed to compile.");
  }
  return re;
}
bool match(regex_t re, const std::string &text) {
  return regexec(&re, text.c_str(), 0, NULL, 0) == 0;
}
} // namespace regex_detail
long long readInt(long long lo, long long hi) {
  // stoll is horribly insufficient, so we use a regex.
  static regex_t re = regex_detail::compile("^0|-?[1-9][0-9]*$");
  std::string token = readToken();
  assertWA(regex_detail::match(re, token));

  long long parsedInt;
  try {
    parsedInt = stoll(token);
  } catch (const std::invalid_argument &) {
    exitWA();
  } catch (const std::out_of_range &) {
    exitWA();
  }
  assertWA(lo <= parsedInt && parsedInt <= hi);
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
  // stold is horribly insufficient, so we use a regex.
  static regex_t re = regex_detail::compile("^-?(0|[1-9][0-9])(\\.[0-9]+)?$");
  std::string token = readToken();
  assertWA(regex_detail::match(re, token));
  long double parsedDouble;
  try {
    parsedDouble = stold(token);
  } catch (const std::invalid_argument &) {
    exitWA();
  } catch (const std::out_of_range &) {
    exitWA();
  }
  assertWA(min - eps <= parsedDouble && parsedDouble <= max + eps);
  return parsedDouble;
}
// This is a hook for when the reader functions error, including `assertWA`. Use
// it to do custom handling when an error happens, such as overriding WA with
// partials, or printing a flag like `-1` in interactors.
void errorHook() {}

// If this is a checker, use the following line in main(int argc, char** argv)
// to replace stdin with the user output. freopen(argv[2], "r", stdin);
