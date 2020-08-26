#include <iostream>
#include <string>
#include "History.h"
#include "globals.h"

using namespace std;

History::History(int nRows, int nCols)
{
    m_rows = nRows;
    m_cols = nCols;
    for (int i = 0; i < m_rows; i++)
        for (int j = 0; j < m_cols; j++)
             m_grid[i][j] = '.';
}

bool History::record(int r, int c)
{
    if (r > m_rows || c > m_cols)
        return false;
    char hist = m_grid[r - 1][c - 1];
    if (hist == '.')
        m_grid[r - 1][c - 1] = 'A';
    else if (hist >= 'A' && hist <= 'Y')
        m_grid[r - 1][c - 1]++;
    return true;
}

void History::display() const
{
    clearScreen();
    for (int i = 0; i < m_rows; i++)
    {
        for (int j = 0; j < m_cols; j++)
            cout << m_grid[i][j];
        cout << endl;
    }
    cout << endl;
}

