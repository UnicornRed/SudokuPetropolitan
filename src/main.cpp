#include <iostream>
#include "sudoku.h"

int main()
{
    int n;

    std::cin >> n;

    try
    {
        Sudoku mySudoku(n);

        std::cin >> mySudoku;

        if (!mySudoku.Solve())
        {
            std::cout << "Invalid field\n";
            return 0;
        }

        std::cout << mySudoku;     
    }
    catch(const std::exception* e)
    {
        std::cout << "Invalid field\n";
    }

    return 0;
}