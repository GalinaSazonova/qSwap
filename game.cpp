#include "game.h"

Game::Game() :
    sizeG(6)
{
    QTime midnight(0,0,0);
    qsrand(midnight.secsTo(QTime::currentTime()));
    for (int i = 0; i < sizeG; ++i)
        for (int j = 0; j < sizeG; ++j)
            matrix[i][j] = qrand() % 6;
}

int Game::getPiece(int i, int j)
{
    return matrix[i][j];
}

bool Game::lookForMatches()
{
    //looking for horizntal i - row, j - col
    for (int i = 0; i < sizeG; i++)
    {
        for (int j = 0; j < sizeG - 2; j++)
        {
            QList<QPair<int,int>> temp = getMatchHoriz(i, j);
            if (temp.size() > 2)
            {
                matches.push_back(temp);
                j += temp.size() - 1;
            }
        }
    }

    //looking for vertical i - col, j - row
    for (int i = 0; i < sizeG; i++)
    {
        for (int j = 0; j < sizeG - 2; j++)
        {
            QList<QPair<int,int>> temp = getMatchVert(i, j);
            if (temp.size() > 2)
            {
                matches.push_back(temp);
                j += temp.size() - 1;
            }
        }
    }

    if (matches.isEmpty())
    {
        return false;
    }
    return true;
}

void Game::findAndRemoveMatches()
{
    for (int i = 0; i < matches.size(); i++)
    {
        int scoreTemp = (matches[i].length() - 1) * 50;
        for (int j = 0; j < matches[i].length(); j++)
        {
            score += scoreTemp;
            matrix[matches[i][j].first][matches[i][j].second] = -1;
            dropPieces(matches[i][j].first, matches[i][j].second);
        }        
    }
    for (int i = 0; i < matches.size(); i++)
    {
        matches[i].clear();
    }
    matches.clear();
    addNewPieces();
}

bool Game::isMovePossible()
{
    for (int col = 0; col < sizeG; col++)
    {
        for (int row = 0; row < sizeG; row++)
        {
            if (matchPattern(col, row, qMakePair(1,0), horizTwoInRow, twoInRowSize))
                return true;
            if (matchPattern(col, row, qMakePair(2,0), horizTwoThroughOne, twoThroughOneSize))
                return true;
            if (matchPattern(col, row, qMakePair(0,1), verticTwoInRow, twoInRowSize))
                return true;
            if (matchPattern(col, row, qMakePair(0,2), verticTwoThroughOne,twoThroughOneSize))
                return true;
        }
    }
    return false;
}

void Game::swapPieces(QPair<int, int> first, QPair<int, int> second)
{
    int temp = matrix[first.first][first.second];
    matrix[first.first][first.second] = matrix[second.first][second.second];
    matrix[second.first][second.second] = temp;
}


QString Game::scoreAtTheMoment()
{
    return QString::number(score);
}

void Game::getNullScore()
{
    score = 0;
}

void Game::mixButtons()
{
    while (!isMovePossible())
    {
        for (int i = 0; i < 25; i++)
        {
            int fp = qrand() % sizeG;
            int sp = qrand() % sizeG;
            int fp2 = qrand() % sizeG;
            int sp2 = qrand() % sizeG;

            swapPieces(qMakePair(fp, sp), qMakePair(fp2, sp2));
        }
    }
}


void Game::dropPieces(int col, int row)
{
    /*for (int i = row - 1; i >= 0; i--)
    {
        if (matrix[col][i] != -1)
        {
            matrix[col][i + 1] = matrix[col][i]; //Do we need to increase row???
            matrix[col][i] = -1;
        }
    }*/
    for (int i = col - 1; i >= 0; i--)
    {
        if (matrix[i][row] != -1)
        {
            matrix[i + 1][row] = matrix[i][row]; //Do we need to increase row???
            matrix[i][row] = -1;
        }
    }
}

void Game::addPiece(int col, int row)
{
    matrix[col][row] = qrand() % 6;
}

void Game::addNewPieces()
{
    for (int col = 0; col < sizeG; col++)
    {
        for (int row = 0; row < sizeG; row++)
        {
            if (matrix[col][row] == -1)
            {
                addPiece(col, row);
            }
        }
    }
}

bool Game::matchType(int col, int row, int type)
{
    if((col < 0) || (col > sizeG - 1) || (row < 0) || (row > sizeG - 1))
        return false;
    else
        return matrix[col][row] == type;
}

bool Game::matchPattern(int col, int row, QPair<int, int> needToMatch, QPair<int, int> possibilities[], int arraySize)
{
    int type = matrix[col][row];

    if (!matchType(col + needToMatch.first, row + needToMatch.second, type))
        return false;
    for (int i = 0; i < arraySize; i++)
    {
        if (matchType(col + possibilities[i].first, row + possibilities[i].second, type))
            return true;
    }
    return false;
}

QList<QPair<int, int>> Game::getMatchHoriz(int row, int col)
{
    QList<QPair<int,int>> temp;
    temp.append(qMakePair(col,row));
    for (int i = 1; i + col < sizeG; i++)
    {
        if(matrix[col][row] == matrix[i + col][row])
        {
           temp.append(qMakePair(i + col, row));
        }
        else
        {
            return temp;
        }
    }
    return temp;
}

QList<QPair<int, int> > Game::getMatchVert(int col, int row)
{
    QList<QPair<int,int>> temp;
    temp.append(qMakePair(col, row));
    for (int i = 1; i + row < sizeG; i++)
    {
        if(matrix[col][row] == matrix[col][row + i])
        {
           temp.append(qMakePair(col, row + i));
        }
        else
        {
            return temp;
        }
    }
    return temp;
}
