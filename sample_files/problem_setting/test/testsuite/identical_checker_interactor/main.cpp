#include "identical_checker_interactor.cpp"
#include <iomanip>

int main() {
  int N = readInt(1, 10);
  readNewLine();
  float f = readFloat(-10, 10);
  readNewLine();
  readEOF();

  std::cout << std::fixed << std::setprecision(2) << f * N << std::endl;
}
