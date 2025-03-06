/*
** EPITECH PROJECT, 2024
** B-AIA-500-MAR-5-1-gomoku-nattan.cochet
** File description:
** board
*/

#include "board.hpp"

Board::Board()
{
}

std::vector<std::vector<int>> Board::getBoard(void) const
{
    return _board;
}

void Board::displayMap(void)
{
    for (size_t i = 0; i != _board.size(); i++)
    {
        for (size_t y = 0; y < _board[i].size(); y++)
            std::cout << _board[i][y] << " ";
        std::cout << std::endl;
    }
}

int Board::getDimension(void)
{
    return _dimension;
}

void Board::setBoardDimension(int x)
{
    _board.resize(x, std::vector<int>(x, 0));
    _dimension = x;
}

void Board::setBoard(std::vector<std::vector<int>> newBoard) {
    _board = newBoard;
}

bool Board::setPosition(int x, int y, int player)
{
    if (_board[x][y] == 0) {
        _board[x][y] = player;
        return (true);
    }
    return (false);
}

Board::Board(int x)
{
    _board.resize(x, std::vector<int>(x, 0));
}

Board::~Board()
{
}