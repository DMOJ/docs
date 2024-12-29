#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <regex.h>
#include <stdexcept>
#include <string>
#include <vector>

namespace CheckerCodes {
constexpr int AC = 0;
constexpr int WA = 1;
constexpr int PE = 2;
constexpr int PARTIAL = 7;
} // namespace CheckerCodes

namespace regex_helpers {

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

} // namespace regex_helpers

void errorHook();

void exitWA() {
  errorHook();
  std::exit(CheckerCodes::WA);
}

void exitPE() {
  errorHook();
  std::exit(CheckerCodes::PE);
}

void assertWA(bool condition) {
  if (!condition) {
    exitWA();
  }
}

void assertPE(bool condition) {
  if (!condition) {
    exitPE();
  }
}

void readSpace() { assertPE(std::cin.get() == ' '); }
void readNewLine() { assertPE(std::cin.get() == '\n'); }
void readEOF() { assertPE(std::cin.get() == EOF); }

std::string readToken(char min_char = 0, char max_char = 127) {
  static constexpr size_t MAX_TOKEN_SIZE = 1e7;
  std::string token;
  int c = std::cin.get();
  assertPE(!isspace(c));
  while (!isspace(c) && c != EOF) {
    assertWA(token.size() < MAX_TOKEN_SIZE);
    assertWA(min_char <= c && c <= max_char);
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
    assertPE(c != EOF);
    assertWA(line.size() < MAX_LINE_SIZE);
    assertWA(min_char <= c && c <= max_char);
    line.push_back(char(c));
    c = std::cin.get();
  }
  return line;
}

long long readInt(long long lo, long long hi) {
  static regex_t re = regex_helpers::compile("^(0|-?[1-9][0-9]*)$");
  std::string token = readToken();
  assertWA(regex_helpers::match(re, token));

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

long double readFloat(long double min, long double max,
                      long double eps = 1e-9) {
  static regex_t re = regex_helpers::compile("^-?(0|[1-9][0-9]*)(\\.[0-9]+)?$");
  std::string token = readToken();
  assertWA(regex_helpers::match(re, token));
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

void errorHook() {}

// If this is a checker:
// int main(int argc, char **argv) {
//   std::ifstream judge_input(argv[1]);
//   std::ifstream submission_output(argv[2]);
//   std::cin.rdbuf(submission_output.rdbuf());
//   std::ifstream judge_answer(argv[3]);
// }

// If this is an interactor:
// int main(int argc, char **argv) {
//   std::ifstream judge_input(argv[1]);
//   std::ifstream judge_answer(argv[2]);
// }
