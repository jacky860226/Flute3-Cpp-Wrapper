#include "Flute3Wrapper.hpp"
#include "flute.h"
#include <cassert>
#include <limits>

namespace Flute {

void FluteWrapper::Tree::print(std::ostream &Out) const {
  int i;
  for (i = 0; i < Degree; ++i)
    Out << ' ' << i << ":  x=" << Branches.at(i).X << "  y=" << Branches.at(i).Y
        << "  e=" << Branches.at(i).Neighbor << '\n';
  for (; i < 2 * Degree - 2; ++i)
    Out << 's' << i << ":  x=" << Branches.at(i).X << "  y=" << Branches.at(i).Y
        << "  e=" << Branches.at(i).Neighbor << '\n';
  Out << '\n';
}
void FluteWrapper::Tree::plot(std::ostream &Out) const {
  for (auto &B : Branches) {
    Out << B.X << ' ' << B.Y << '\n';
    Out << Branches.at(B.Neighbor).X << ' ' << Branches.at(B.Neighbor).Y
        << "\n\n";
  }
}
void FluteWrapper::Tree::writeSVG(std::ostream &Out, double Scale) const {
  int X_min = std::numeric_limits<int>::max();
  int Y_min = std::numeric_limits<int>::max();
  int X_max = std::numeric_limits<int>::min();
  int Y_max = std::numeric_limits<int>::min();
  for (auto &B : Branches) {
    X_min = std::min(X_min, B.X);
    Y_min = std::min(Y_min, B.Y);
    X_max = std::max(X_max, B.X);
    Y_max = std::max(Y_max, B.Y);
  }

  int Dx = X_max - X_min;
  int Dy = Y_max - Y_min;

  const double StrokeWidth = 1;

  Out << "<svg xmlns=\"http://www.w3.org/2000/svg\" viewBox=\""
      << (X_min - 1) * Scale << ' ' << (Y_min - 1) * Scale << ' '
      << (Dx + 2) * Scale << ' ' << (Dy + 2) * Scale << "\""
      << " style=\"width:100%\" stroke=\"black\" stroke-width=\"" << StrokeWidth
      << "\">\n";
  for (auto &B : Branches) {
    Out << "<line x1=\"" << B.X * Scale << "\" y1=\"" << B.Y * Scale
        << "\" x2=\"" << Branches.at(B.Neighbor).X * Scale << "\" y2=\""
        << Branches.at(B.Neighbor).Y * Scale << "\"/>\n";
  }
  Out << "</svg>\n";
}

std::unique_ptr<FluteWrapper> FluteWrapper::UniqueFluteWrapper = nullptr;

FluteWrapper::FluteWrapper() { readLUT(); }

FluteWrapper::~FluteWrapper() { deleteLUT(); }

FluteWrapper *FluteWrapper::getInstance() {
  if (UniqueFluteWrapper == nullptr)
    UniqueFluteWrapper = std::unique_ptr<FluteWrapper>(new FluteWrapper());
  return UniqueFluteWrapper.get();
}

FluteWrapper::Tree FluteWrapper::runFlute(std::vector<int> &X,
                                          std::vector<int> &Y) {
  auto Size = X.size();
  assert(Size == Y.size() && Size > 0);
  auto FluteTree = flute(X.size(), X.data(), Y.data(), FLUTE_ACCURACY);
  std::vector<Branch> Branchs;
  auto BranchsSize = FluteTree.deg * 2 - 2;
  Branchs.reserve(BranchsSize);
  for (int i = 0; i < BranchsSize; ++i) {
    Branchs.emplace_back(FluteTree.branch[i].x, FluteTree.branch[i].y,
                         FluteTree.branch[i].n);
  }
  free_tree(FluteTree);
  return Tree(std::move(Branchs), FluteTree.length, FluteTree.deg);
}
int FluteWrapper::runFlute_wl(std::vector<int> &X, std::vector<int> &Y) {
  auto Size = X.size();
  assert(Size == Y.size() && Size > 0);
  return flute_wl(X.size(), X.data(), Y.data(), FLUTE_ACCURACY);
}

} // namespace Flute