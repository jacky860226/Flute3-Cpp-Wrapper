#pragma once
#include <memory>
#include <ostream>
#include <vector>

namespace Flute {

class FluteWrapper {
  static std::unique_ptr<FluteWrapper> UniqueFluteWrapper;
  FluteWrapper();

public:
  struct Branch {
    int X;
    int Y;
    int Neighbor;
    Branch(int X, int Y, int Neighbor) : X(X), Y(Y), Neighbor(Neighbor) {}
  };

  struct Tree {
    const std::vector<Branch> Branches;
    const int Length;
    const int Degree;
    Tree(std::vector<Branch> &&Branches, int Length, int Degree)
        : Branches(std::move(Branches)), Length(Length), Degree(Degree) {}
    Tree(Tree &&O)
        : Branches(std::move(O.Branches)), Length(O.Length), Degree(O.Degree) {}
    void print(std::ostream &Out) const;
    void plot(std::ostream &Out) const;
    void writeSVG(std::ostream &Out) const;
  };
  static FluteWrapper *getInstance();
  ~FluteWrapper();
  Tree runFlute(std::vector<int> &X, std::vector<int> &Y);
  int runFlute_wl(std::vector<int> &X, std::vector<int> &Y);
};

} // namespace Flute