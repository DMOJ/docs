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
constexpr int PARTIAL = 7;
} // namespace CheckerCodes

void assertWA(bool);

// Implementation of the tricky whitespace logic for standard checkers.
namespace standard_whitespace_detail {

enum WhitespaceFlag { NONE = 0, SPACE = 1, NEWLINE = 2, ALL = 3 };

WhitespaceFlag current_flag = ALL; // At checker start, consume all whitespace.

void pokeFlag(WhitespaceFlag flag) {
  if (current_flag != NONE && (current_flag != NEWLINE || flag != ALL)) {
    throw std::runtime_error("Never call two whitespace methods in a row, "
                             "except for readNewLine() followed by readEOF().");
  }
  current_flag = flag;
}

enum ConsumeResult {
  NO_WHITESPACE,
  NO_LINES,
  LINES,
};

ConsumeResult consumeWhitespace() {
  int c = std::cin.get();
  ConsumeResult result = NO_WHITESPACE;
  while (isspace(c) && c != EOF) {
    if (result == NO_WHITESPACE) {
      result = NO_LINES;
    }
    if (c == '\r' || c == '\n') {
      result = LINES;
    }
    c = std::cin.get();
  }
  std::cin.unget();
  current_flag = NONE;
  return result;
}

void preReadToken() {
  switch (current_flag) {
  case NONE:
    throw std::runtime_error(
        "Must not call readInt (or readToken, or readFloat) twice in a row!");
  case SPACE:
    assertWA(consumeWhitespace() == NO_LINES);
    break;
  case NEWLINE:
    assertWA(consumeWhitespace() == LINES);
    break;
  case ALL:
    consumeWhitespace();
    break;
  }
}
} // namespace standard_whitespace_detail

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

void assertWA(bool condition) {
  if (!condition) {
    exitWA();
  }
}

void readSpace() {
  standard_whitespace_detail::pokeFlag(standard_whitespace_detail::SPACE);
}

void readNewLine() {
  standard_whitespace_detail::pokeFlag(standard_whitespace_detail::NEWLINE);
}

void readEOF() {
  standard_whitespace_detail::pokeFlag(standard_whitespace_detail::ALL);
  standard_whitespace_detail::consumeWhitespace();
  assertWA(std::cin.get() == EOF);
}

std::string readToken(char min_char = 0, char max_char = 127) {
  standard_whitespace_detail::preReadToken();
  static constexpr size_t MAX_TOKEN_SIZE = 1e7;
  std::string token;
  int c = std::cin.get();
  assertWA(!isspace(c));
  while (!isspace(c) && c != EOF) {
    assertWA(token.size() < MAX_TOKEN_SIZE);
    assertWA(min_char <= c && c <= max_char);
    token.push_back(char(c));
    c = std::cin.get();
  }
  std::cin.unget();
  return token;
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
  static regex_t re = regex_helpers::compile("^-?(0|[1-9][0-9])(\\.[0-9]+)?$");
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
