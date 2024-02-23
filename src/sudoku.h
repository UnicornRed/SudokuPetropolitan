#pragma once

#include <set>
#include <vector>

class Sudoku
{
private:
    int sqrtN, emptyCells;
    bool flagSetNumb;
    size_t n;
    std::vector<std::vector<int>> field;
    std::vector<int> rows;
    std::vector<int> columns;
    std::vector<int> blocks;
    std::vector<std::vector<std::set<int>>> hypotheses;

    void Preparing();

    void SetNumber(int i, int j, int num);
public:
    Sudoku(size_t _n);

    Sudoku(size_t _n, std::vector<std::vector<int>> _field);

    int Solve();

    void PrintHypotheses(std::ostream& out);

    std::vector<int>& operator[](size_t i);

    const std::vector<int>& operator[](size_t i) const;

    friend std::ostream& operator<<(std::ostream& out, const Sudoku& s);
    friend std::istream& operator>>(std::istream& in, Sudoku& s);
};

std::ostream& operator<<(std::ostream& out, const Sudoku& s);
std::istream& operator>>(std::istream& in, Sudoku& s);