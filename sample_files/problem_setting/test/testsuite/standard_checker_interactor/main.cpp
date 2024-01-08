#include "standard_checker_interactor.cpp"

int main() {
  int N = readInt(1, 10);
  readSpace();
  int M = readInt(1, 10);
  readNewLine();

  for (int i = 0; i < M; i++) {
    int a = readInt(1, N);
    readSpace();
    int b = readInt(1, N);
    readSpace();
    int c = readInt(-10, 10);
    readNewLine();
  }
  readEOF();
}
