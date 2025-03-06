/*
** EPITECH PROJECT, 2024
** B-AIA-500-MAR-5-1-gomoku-nattan.cochet
** File description:
** minimax
*/

#include <tuple>
#include <vector>
#include <iostream>
#include <string>
#include <array>
#include <chrono>
#include <unordered_set>
#include <algorithm>
#include <unordered_set>
#include <utility>


#ifndef MINIMAX_HPP_
    #define MINIMAX_HPP_

enum Directions {
    N,
    NE,
    E,
    SE,
    S,
    SW,
    W,
    NW,
    NUM_DIRECTIONS
};

template <>
    struct std::hash<std::pair<int, int>> {
        std::size_t operator()(const std::pair<int, int>& key) const {
            auto hash1 = std::hash<int>()(std::get<0>(key));
            auto hash2 = std::hash<int>()(std::get<1>(key));
            return hash1 ^ (hash2 << 1);
        }
    };

class Minimax
{
    public:
        Minimax();
        ~Minimax();
        std::pair<int, int> bot_play(std::vector<std::vector<int>> board, int size);
    private:
        int meilleur_score;
        std::chrono::steady_clock::time_point _start_time;
        std::pair<int, int> meilleur_coup;
        void get_actions(std::vector<std::vector<int>> &board, int size, int player, std::unordered_set<std::pair<int, int>> &actions);
        void area_game(std::vector<std::vector<int>> &board, int size, int incase, std::unordered_set<std::pair<int, int>> &result);
        std::unordered_set<std::pair<int, int>> add_position(std::unordered_set<std::pair<int, int>> &area, std::vector<std::vector<int>> &board, int i, int j, int size, int incase);
        int algoMinimax(std::unordered_set<std::pair<int, int>> &area, std::vector<std::vector<int>> &board, int depth, bool player, int size, int alpha, int beta, std::unordered_set<std::pair<int, int>> &myactions, std::unordered_set<std::pair<int, int>> &ennemyactions);
        int estimation(std::vector<std::vector<int>> &board, int size, std::unordered_set<std::pair<int, int>> &myactions, std::unordered_set<std::pair<int, int>> &ennemyactions);
        int all_checks(std::vector<std::vector<int>> &board, int size, int i, int j, int ennemy, int me);
        int end_game(std::vector<std::vector<int>> &board, int size, std::unordered_set<std::pair<int, int>> &myactions, std::unordered_set<std::pair<int, int>> &ennemyactions);
        int check_end(std::vector<std::vector<int>> &board, int size, int i, int j, int ennemy);
        std::vector<std::tuple<int, int, int>> evaluate_and_sort_moves(std::unordered_set<std::pair<int, int>> &area, std::vector<std::vector<int>> &board, int size, std::unordered_set<std::pair<int, int>> &myactions, std::unordered_set<std::pair<int, int>> &ennemyactions, bool player);
};
bool is_digits(std::string& str);

#endif /* !MINIMAX_HPP_ */
