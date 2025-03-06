/*
** EPITECH PROJECT, 2024
** B-AIA-500-MAR-5-1-gomoku-nattan.cochet
** File description:
** minimax
*/

#include "minimax.hpp"

Minimax::Minimax(/* args */)
{
    meilleur_score = -5000;
    meilleur_coup = {0, 0};
}

Minimax::~Minimax()
{
}

int Minimax::check_end(std::vector<std::vector<int>> &board, int size, int i, int j, int me)
{
    std::array<int, NUM_DIRECTIONS> dir_i = {-1, -1, 0, 1, 1, 1, 0, -1};
    std::array<int, NUM_DIRECTIONS> dir_j = {0, -1, -1, -1, 0, 1, 1, 1};
    std::array<int, NUM_DIRECTIONS> tokens = {1, 1, 1, 1, 1, 1, 1, 1};
    int block = 0;

    for (int k = 1; k <= 5; k++) {
        for (int d = 0; d < NUM_DIRECTIONS; d++) {
            if (tokens[d] == -1) {
                block++;
                continue;
            }
            int di = i + k * dir_i[d];
            int dj = j + k * dir_j[d];
            if (di < 0 || di >= size || dj < 0 || dj >= size) {
                tokens[d] = -1;
                block++;
                continue;
            }
            if (board[di][dj] != me)
                tokens[d] = -1;
            else
                tokens[d] += 1;
            if (tokens[d] >= 5)
                return (1);
        }
        if (block == 8)
            return 0;
        block = 0;
    }
    return (0);
}

int Minimax::end_game(std::vector<std::vector<int>> &board, int size, std::unordered_set<std::pair<int, int>> &myactions, std::unordered_set<std::pair<int, int>> &ennemyactions)
{
    for (std::unordered_set<std::pair<int, int>>::iterator it = myactions.begin(); it != myactions.end(); it++) {
        int i = it->first;
        int j = it->second;
        if (check_end(board, size, i, j, 1) == 1) {
            return (1);
        }
    }
    for (std::unordered_set<std::pair<int, int>>::iterator it = ennemyactions.begin(); it != ennemyactions.end(); it++) {
        int i = it->first;
        int j = it->second;
        if (check_end(board, size, i, j, 2) == 1)
            return (2);
    }
    return (0);
}

std::pair<int, int> Minimax::bot_play(std::vector<std::vector<int>> board, int size)
{
    std::unordered_set<std::pair<int, int>> area;
    area_game(board, size, 0, area);
    std::unordered_set<std::pair<int, int>> newArea;
    std::unordered_set<std::pair<int, int>> myactions;
    get_actions(board, size, 1, myactions);
    std::unordered_set<std::pair<int, int>> ennemyactions;
    get_actions(board, size, 2, ennemyactions);
    std::vector<std::tuple<int, int, int>> sorted_moves;
    int i = 0;
    int j = 0;
    int result = 0;
    int depth = 3;
    for (std::unordered_set<std::pair<int, int>>::iterator it = myactions.begin(); it != myactions.end();) {
        i = it->first;
        j = it->second;
        result = all_checks(board, size, i, j, 2, 1);
        if (result == -1) {
            it = myactions.erase(it);
        } else {
            it++;
        }
    }
    for (std::unordered_set<std::pair<int, int>>::iterator it = ennemyactions.begin(); it != ennemyactions.end();) {
        i = it->first;
        j = it->second;
        result = all_checks(board, size, i, j, 1, 2);
        if (result == -1) {
            it = ennemyactions.erase(it);
        } else {
            it++;
        }
    }
    _start_time = std::chrono::steady_clock::now();
    sorted_moves = evaluate_and_sort_moves(area, board, size, myactions, ennemyactions, true);
    for (std::vector<std::tuple<int, int, int>>::iterator it = sorted_moves.begin(); it != sorted_moves.end(); it++) {

        auto current_time = std::chrono::steady_clock::now();
        auto elapsed_time = std::chrono::duration_cast<std::chrono::seconds>(current_time - _start_time).count();

        if (elapsed_time >= 4)
            break;
        if (elapsed_time >= 3.5)
            depth = 2;
        i = std::get<0>(*it);
        j = std::get<1>(*it);
        board[i][j] = 1;
        newArea = add_position(area, board, i, j, size, 0);
        newArea.erase({i, j});
        myactions.insert({i, j});
        int score = algoMinimax(newArea, board, depth, false, size, -5000, 5000, myactions, ennemyactions);
        //std::cout << "pos = (" << i << "," << j << ")" << std::endl;
        //std::cout << "result = " << score << std::endl;
        newArea.clear();
        myactions.erase({i, j});
        board[i][j] = 0;
        //print(score, meilleur_score)
        if (score == 100000)
            return meilleur_coup;
        if (std::get<0>(meilleur_coup) == -1 && std::get<1>(meilleur_coup) == -1)
            meilleur_coup = {i, j};
        if (score > meilleur_score) {
            meilleur_score = score;
            meilleur_coup = {i, j};
        }
    }
    return (meilleur_coup);
}

void Minimax::get_actions(std::vector<std::vector<int>> &board, int size, int player, std::unordered_set<std::pair<int, int>> &actions)
{
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (board[i][j] == player)
                actions.insert({i, j});
        }
    }
}

std::unordered_set<std::pair<int, int>> Minimax::add_position(std::unordered_set<std::pair<int, int>> &area, std::vector<std::vector<int>> &board, int i, int j, int size, int incase)
{
    std::unordered_set<std::pair<int, int>> result = area;
    if (i > 0 && board[i - 1][j] == incase)
            result.insert({i - 1, j});
    if (i > 0 && j > 0 && board[i - 1][j - 1] == incase)
            result.insert({i - 1, j - 1});
    if (j > 0 && board[i][j - 1] == incase)
            result.insert({i, j - 1});
    if (i < size - 1 && j > 0 && board[i + 1][j - 1] == incase)
            result.insert({i + 1, j - 1});
    if (i < size - 1 && board[i + 1][j] == incase)
            result.insert({i + 1, j});
    if (i < size - 1 && j < size - 1 && board[i + 1][j + 1] == incase)
            result.insert({i + 1, j + 1});
    if (j < size - 1 && board[i][j + 1] == incase)
            result.insert({i, j + 1});
    if (i > 0 && j < size - 1 && board[i - 1][j + 1] == incase)
            result.insert({i - 1, j + 1});
    return (result);
}

//#check profiler
void Minimax::area_game(std::vector<std::vector<int>> &board, int size, int incase, std::unordered_set<std::pair<int, int>> &result)
{
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (board[i][j] != incase)
                continue;
            if (i > 0 && board[i - 1][j] != incase) {
                result.insert({i, j});
                continue;
            }
            if (i > 0 && j > 0 && board[i - 1][j - 1] != incase) {
                result.insert({i, j});
                continue;
            }
            if (j > 0 && board[i][j - 1] != incase) {
                result.insert({i, j});
                continue;
            }
            if ((i + 1) < size && j > 0 && board[i + 1][j - 1] != incase) {
                result.insert({i, j});
                continue;
            }
            if ((i + 1) < size && board[i + 1][j] != incase) {
                result.insert({i, j});
                continue;
            }
            if ((i + 1) < size && (j + 1) < size && board[i + 1][j + 1] != incase) {
                result.insert({i, j});
                continue;
            }
            if ((j + 1) < size && board[i][j + 1] != incase) {
                result.insert({i, j});
                continue;
            }
            if (i > 0 && (j + 1) < size && board[i - 1][j + 1] != incase) {
                result.insert({i, j});
                continue;
            }
        }
    }
}

int Minimax::all_checks(std::vector<std::vector<int>> &board, int size, int i, int j, int ennemy, int me)
{
    int alltokens = 0;
    int block = 0;
    std::array<int, NUM_DIRECTIONS> dir_i = {-1, -1, 0, 1, 1, 1, 0, -1};
    std::array<int, NUM_DIRECTIONS> dir_j = {0, -1, -1, -1, 0, 1, 1, 1};
    std::array<int, NUM_DIRECTIONS> tokens = {0, 0, 0, 0, 0, 0, 0, 0};

    for (int k = 1; k <= 5; k++) {
        for (int d = 0; d < NUM_DIRECTIONS; d++) {
            if (tokens[d] == -1) {
                block++;
                continue;
            }
            int di = i + k * dir_i[d];
            int dj = j + k * dir_j[d];
            if (di < 0 || di >= size || dj < 0 || dj >= size) {
                tokens[d] = -1;
                block++;
                continue;
            }
            if (board[di][dj] == ennemy)
                tokens[d] = -1;
            if (board[di][dj] == me)
                tokens[d] += 1;
        }
        if (block == 8)
            break;
        block = 0;
    }
    if (block == 8)
        return (-1);
    for (std::array<int, NUM_DIRECTIONS>::iterator it = tokens.begin(); it != tokens.end(); it++) {
        if (*it != -1)
            alltokens += (*it + 1) * (*it + 1);
    }
    return (alltokens);
}

int Minimax::estimation(std::vector<std::vector<int>> &board, int size, std::unordered_set<std::pair<int, int>> &myactions, std::unordered_set<std::pair<int, int>> &ennemyactions)
{
    int resultPlayer = 0;
    int resultEnnemie = 0;
    for (std::unordered_set<std::pair<int, int>>::iterator it = myactions.begin(); it != myactions.end(); it++) {
        int i = it->first;
        int j = it->second;
        resultPlayer += all_checks(board, size, i, j, 2, 1);
    }
    for (std::unordered_set<std::pair<int, int>>::iterator it = ennemyactions.begin(); it != ennemyactions.end(); it++) {
        int i = it->first;
        int j = it->second;
        resultEnnemie += all_checks(board, size, i, j, 1, 2);
    }
    return resultPlayer - resultEnnemie;
    /*if (resultEnnemie > resultPlayer)
        return (resultEnnemie * -1);
    if (resultEnnemie < resultPlayer)
        return (resultPlayer);
    return (0);*/
}

std::vector<std::tuple<int, int, int>> Minimax::evaluate_and_sort_moves(std::unordered_set<std::pair<int, int>> &area, std::vector<std::vector<int>> &board, int size, std::unordered_set<std::pair<int, int>> &myactions, std::unordered_set<std::pair<int, int>> &ennemyactions, bool player)
{
    std::vector<std::tuple<int, int, int>> sorted_actions;

    for (std::unordered_set<std::pair<int, int>>::iterator it = area.begin(); it != area.end(); it++) {
        int i = it->first;
        int j = it->second;
        if (player == false) {
            board[i][j] = 2;
            ennemyactions.insert(*it);
        } else {
            board[i][j] = 1;
            myactions.insert(*it);
        }
        int estimation_score = 0;
        estimation_score = estimation(board, size, myactions, ennemyactions);
        sorted_actions.push_back({i, j, estimation_score});
        if (player == false)
            ennemyactions.erase(*it);
        else
            myactions.erase(*it);
        board[i][j] = 0;
    }
    // Sort moves in descending order of their scores
    if (player == false)
        std::sort(sorted_actions.begin(), sorted_actions.end(), [](const auto &a, const auto &b) { return std::get<2>(a) < std::get<2>(b); });
    else
        std::sort(sorted_actions.begin(), sorted_actions.end(), [](const auto &a, const auto &b) { return std::get<2>(a) > std::get<2>(b); });
    return sorted_actions;
}

int Minimax::algoMinimax(std::unordered_set<std::pair<int, int>> &area, std::vector<std::vector<int>> &board, int depth, bool player, int size, int alpha, int beta, std::unordered_set<std::pair<int, int>> &myactions, std::unordered_set<std::pair<int, int>> &ennemyactions)
{
    auto current_time = std::chrono::steady_clock::now();
    auto elapsed_time = std::chrono::duration_cast<std::chrono::seconds>(current_time - _start_time).count();
    if (elapsed_time >= 4.5) {
        return 100000;
    }
    int result_end = end_game(board, size, myactions, ennemyactions);
    if (result_end == 1)
        return 1000 * (depth + 1);
    if (result_end == 2)
        return -1000 * (depth + 1);
    if (depth == 0) {
        return (estimation(board, size, myactions, ennemyactions));
    }
    std::unordered_set<std::pair<int, int>> newArea;
    std::vector<std::tuple<int, int, int>> sorted_moves;

    if (player == false) {
        int meilleur_score = 5000;
        sorted_moves = evaluate_and_sort_moves(area, board, size, myactions, ennemyactions, player);
        for (std::vector<std::tuple<int, int, int>>::iterator it = sorted_moves.begin(); it != sorted_moves.end(); it++) {
            int i = std::get<0>(*it);
            int j = std::get<1>(*it);
            board[i][j] = 2;
            newArea = add_position(area, board, i, j, size, 0);
            newArea.erase({i, j});
            ennemyactions.insert({i, j});
            int score = algoMinimax(newArea, board, (depth - 1), true, size, alpha, beta, myactions, ennemyactions);
            board[i][j] = 0;
            newArea.clear();
            ennemyactions.erase({i, j});
            if (score == 100000)
                return 100000;
            if (score < meilleur_score)
                meilleur_score = score;
            if (score < beta)
                beta = score;
            if (beta <= alpha)
                break;
        }
        return (meilleur_score);
    } else {
        int meilleur_score = -5000;
        sorted_moves = evaluate_and_sort_moves(area, board, size, myactions, ennemyactions, player);
        for (std::vector<std::tuple<int, int, int>>::iterator it = sorted_moves.begin(); it != sorted_moves.end(); it++) {
            int i = std::get<0>(*it);
            int j = std::get<1>(*it);
            board[i][j] = 1;
            newArea = add_position(area, board, i, j, size, 0);
            newArea.erase({i, j});
            myactions.insert({i, j});
            int score = algoMinimax(newArea, board, (depth - 1), false, size, alpha, beta, myactions, ennemyactions);
            board[i][j] = 0;
            newArea.clear();
            myactions.erase({i, j});
            if (score == 100000)
                return 100000;
            if (score > meilleur_score)
                meilleur_score = score;
            if (score > alpha)
                alpha = score;
            if (beta <= alpha)
                break;
        }
        return (meilleur_score);
    }
    return 0;
}

bool is_digits(std::string& str)
{
    for (char ch : str) {
        int v = ch;
        if (!(v >= 48 && v <= 57))
            return (false);
    }
    return (true);
}
