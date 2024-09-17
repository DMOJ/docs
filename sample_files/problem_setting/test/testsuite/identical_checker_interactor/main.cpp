#include "identical_checker_interactor.cpp"

int main() {
  int N = readInt(1, 10);
  readNewLine();
  float f = readFloat(-10, 10);
  readNewLine();
  readEOF();

  printf("%0.2f\n", f * N);
}
