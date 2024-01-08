#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <numeric>
#include <regex.h>
#include <stdexcept>
#include <string>
#include <vector>

namespace CheckerCodes {
int AC = 0;
int WA = 1;
int PE = 2;
int PARTIAL = 7;
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
void readSpace() { assertPE(getchar() == ' '); }
void readNewLine() { assertPE(getchar() == '\n'); }
void readEOF() { assertPE(getchar() == EOF); }
std::string readToken(char min_char = 0, char max_char = 127) {
  static constexpr size_t MAX_TOKEN_SIZE = 1e7;
  std::string token;
  int c = getchar();
  assertPE(!isspace(c));
  while (!isspace(c) && c != EOF) {
    assertWA(token.size() < MAX_TOKEN_SIZE);
    assertWA(min_char <= c && c <= max_char);
    token.push_back(char(c));
    c = getchar();
  }
  ungetc(c, stdin);
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
void errorHook() {}

// readLine() and readFloat() removed for brevity.

int main(int argc, char **argv) {
  std::ifstream judge_input(argv[1]);
  freopen(argv[2], "r", stdin);
  std::ifstream judge_answer(argv[3]);

  int N, K;
  judge_input >> N >> K;

  // If any integer is greater than K, we give an immediate WA.
  std::vector<int> arr = readIntArray<int>(N, 0, K);
  // We can read EOF now, since we are done with the input. This makes it easier
  // to remember.
  readEOF();

  // Note that we must store the sum in a long long, since it may overflow a
  // 32-bit integer.
  long long sum = std::accumulate(arr.begin(), arr.end(), 0LL);

  assertWA(sum == K);

  // It turns out that the minimum product is always zero, since [0, 0, ..., K]
  // is always valid.
  // Thus, it suffices to check for a zero in the array.
  if (std::find(arr.begin(), arr.end(), 0) == arr.end()) {
    // No zero found. Give partial points:
    // Output to stderr for the coci contrib module to grant partial AC.
    fprintf(stderr, "partial 50/100\n");
    // Output to stdout to give user feedback.
    printf("50/100 points");
    return CheckerCodes::PARTIAL;
  }
  return CheckerCodes::AC;
}
