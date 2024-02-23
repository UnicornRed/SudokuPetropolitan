#include <iostream>
#include <algorithm>
#include <thread>
#include <mutex>
#include <stdexcept>
#include "sudoku.h"
#include "cmath"

void Sudoku::Preparing()
{
    sqrtN = static_cast<int>(std::sqrt(static_cast<double>(n)));

    if (sqrtN * sqrtN != n)
        throw new std::invalid_argument("N must be the square of an integer.");

    field.resize(n, std::vector<int>(n));
    hypotheses.resize(n, std::vector<std::set<int>>(n));

    std::set<int> option;

    for (size_t i{1}; i <= n; ++i)
        option.insert(i);

    rows.resize(n);
    columns.resize(n);
    blocks.resize(n);

    std::for_each(hypotheses.begin(), hypotheses.end(), [&option](std::vector<std::set<int>>& v)
    {
        std::for_each(v.begin(), v.end(), [&option](std::set<int>& s)
        {
            s.insert(option.begin(), option.end());
        });
    });
}

void Sudoku::SetNumber(int i, int j, int num)
{
    field[i][j] = num;

    if (num)
    {
        --emptyCells;
        hypotheses[i][j].clear();

        for (size_t t{}; t < n; ++t)
        {
            hypotheses[i][t].erase(num);
            hypotheses[t][j].erase(num);
            hypotheses[i / sqrtN * sqrtN + t / sqrtN][j / sqrtN * sqrtN + t % sqrtN].erase(num);
        }
    }

    flagSetNumb = true;
}

Sudoku::Sudoku(size_t _n) : emptyCells(_n * _n), n(_n)
{
    Preparing();
}

Sudoku::Sudoku(size_t _n, std::vector<std::vector<int>> _field) : emptyCells(_n * _n), n(_n)
{
    Preparing();

    if (_field.size() != n || (_field.size() != 0 && _field[0].size() != n))
        throw new std::invalid_argument("The field must be of size n by n.");

    std::set<int> option;

    for (size_t i{1}; i <= n; ++i)
        option.insert(i);

    for (size_t i{}; i < n; ++i)
        for (size_t j{}; j < n; ++j)
            SetNumber(i, j, _field[i][j]);
}

std::recursive_mutex mtx;

int Sudoku::Solve()
{
    flagSetNumb = true;

    while (emptyCells > 0 && flagSetNumb)
    {
        flagSetNumb = false;

        for (size_t i{}; i < n; ++i)
        {
            for (size_t j{}; j < n; ++j)
            {
                if (field[i][j] != 0)
                    continue;

                if (hypotheses[i][j].size() == 0)
                    return 0;

                if (hypotheses[i][j].size() == 1)
                    SetNumber(i, j, *hypotheses[i][j].begin());

                blocks.assign(n, 0);
                rows.assign(n, 0);
                columns.assign(n, 0);

                for (size_t t{}; t < n; ++t)
                {
                    std::for_each(hypotheses[i][t].begin(), hypotheses[i][t].end(), [this](int a)
                    {
                        ++rows[a - 1];
                    });
                    std::for_each(hypotheses[t][j].begin(), hypotheses[t][j].end(), [this](int a)
                    {
                        ++columns[a - 1];
                    });
                    std::for_each(hypotheses[i / sqrtN * sqrtN + t / sqrtN][j / sqrtN * sqrtN + t % sqrtN].begin(),
                                  hypotheses[i / sqrtN * sqrtN + t / sqrtN][j / sqrtN * sqrtN + t % sqrtN].end(), [this](int a)
                    {
                        ++blocks[a - 1];
                    });
                }

                for (int it : hypotheses[i][j])
                    if (rows[it - 1] == 1 || columns[it - 1] == 1 || blocks[it - 1] == 1)
                    {
                        SetNumber(i, j, it);
                        break;
                    }
            }
        }

        if (!flagSetNumb)
        {
            size_t iMin, jMin, minSize = n;

            for (size_t i{}; i < n; ++i)
                for (size_t j{}; j < n; ++j)
                    if (!field[i][j] && hypotheses[i][j].size() <= minSize)
                    {
                        iMin = i;
                        jMin = j;
                        minSize = hypotheses[i][j].size();
                    }

            for (int it : hypotheses[iMin][jMin])
            {
                Sudoku help(*this);

                help.SetNumber(iMin, jMin, it);

                if (help.Solve())
                {
                    *this = help;
                    return 1;
                }
            }

            return 0;
        }
    }

    return 1;
}

std::vector<int>& Sudoku::operator[](size_t i)
{
    if (i >= n)
        throw new std::out_of_range("Going out of bounds.");

    return field[i];
}

const std::vector<int>& Sudoku::operator[](size_t i) const
{
    if (i >= n)
        throw new std::out_of_range("Going out of bounds.");

    return field[i];
}

std::ostream& operator<<(std::ostream& out, const Sudoku& s)
{
    for (size_t i{}; i < s.n; ++i)
    {
        for (size_t j{}; j < s.n; ++j)
            out << s[i][j] << " ";

        out << "\n";
    }

    return out;
}

std::istream& operator>>(std::istream& in, Sudoku& s)
{
    int num;

    for (size_t i{}; i < s.n; ++i)
        for (size_t j{}; j < s.n; ++j)
        {
            in >> num;
            s.SetNumber(i, j, num);
        }

    return in;
}