#include "Flute3Wrapper.hpp"
#include <fstream>
#include <iostream>

int main() {
  auto Wrapper = Flute::FluteWrapper::getInstance();
  std::vector<int> X = {2000, 8000, 6000, 5000, 3000, 4000};
  std::vector<int> Y = {3000, 2000, 6000, 4000, 5000, 2000};

  auto FluteTree = Wrapper->runFlute(X, Y);

  std::cout << "FLUTE wirelength only = " << Wrapper->runFlute_wl(X, Y) << '\n';
  std::cout << "FLUTE wirelength = " << FluteTree.Length << '\n';
  std::cout << "FLUTE degree = " << FluteTree.Degree << '\n';
  std::cout << "FLUTE printtree\n";
  FluteTree.print(std::cout);
  std::cout << "FLUTE plottree\n";
  FluteTree.plot(std::cout);
  std::cout << "FLUTE writeSVG\n";
  std::ofstream Fout("test.svg");
  FluteTree.writeSVG(Fout);

  return 0;
}
