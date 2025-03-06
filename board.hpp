/*
** EPITECH PROJECT, 2024
** B-AIA-500-MAR-5-1-gomoku-nattan.cochet
** File description:
** board
*/
#include <iostream>
#include <vector>

class Board
{
    private:
        std::vector<std::vector<int>> _board;
        int _dimension;
    public:
        void setBoardDimension(int x);
        std::vector<std::vector<int>> getBoard(void) const;
        int getDimension(void);
        void displayMap(void);
        void setBoard(std::vector<std::vector<int>>);
        bool setPosition(int x, int y, int player);
        Board();
        Board(int x);
        ~Board();
};
