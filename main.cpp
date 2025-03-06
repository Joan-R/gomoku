/*
** EPITECH PROJECT, 2024
** B-AIA-500-MAR-5-1-gomoku-nattan.cochet
** File description:
** main
*/

#include <array>
#include <cstdint>
#include <functional>
#include <iostream>
#include <string>
#include <string_view>
#include <unordered_map>
#include "board.hpp"
#include "minimax.hpp"

namespace
{

    bool SHOULD_STOP = false;

    int STATUS = 0;

    Board board;

    enum log_type : std::uint8_t
    {
        UNKNOWN,
        ERROR,
        MESSAGE,
        DEBUG,
    };

    const std::unordered_map<log_type, std::string_view> LOG_TYPE_MAPPINGS{
        {UNKNOWN, "UNKNOWN"},
        {ERROR, "ERROR"},
        {MESSAGE, "MESSAGE"},
        {DEBUG, "DEBUG"},
    };

    auto send_log(log_type type, const std::string_view &msg) -> void
    {
        std::cout << LOG_TYPE_MAPPINGS.at(type) << " " << msg << std::endl;
    }

    auto handle_about([[maybe_unused]] std::string &cmd) -> void
    {
        constexpr std::string_view bot_name = "Bot Gomoku";
        std::cout << "name=\"" << bot_name << "\", version=\"0.42\", country=\"France\", author=\"Joan Ruiz\"" << std::endl;
    }

    auto handle_start(std::string &cmd) -> void
    {
        if (STATUS != 0) {
            send_log(ERROR, "GAME ALREADY STARTED");
            return;
        }
        std::vector<std::string> words;
        size_t pos = 0;
        std::string token;

        while ((pos = cmd.find(' ')) != std::string::npos)
        {
            token = cmd.substr(0, pos);
            words.push_back(token);
            cmd.erase(0, pos + 1);
        }
        words.push_back(cmd);
        if (words.size() != 2) {
            send_log(ERROR, "WRONG START DATA");
            return;
        }
        int dimension;
        try {
            dimension = stoi(words[1]);
        } catch (std::exception &e) {
            std::cout << "error converting" << std::endl;
            return;
        }
        if (dimension < 5) {
            send_log(ERROR, "WRONG DIMENSION");
            return;
        }
        board.setBoardDimension(dimension);
        STATUS = 1;
        std::cout << "OK" << std::endl;
    }

    auto handle_end([[maybe_unused]] std::string &cmd) -> void
    {
        SHOULD_STOP = true;
    }

    auto handle_info(std::string &cmd) -> void
    {
        (void)cmd;
        return;
    }

    auto handle_begin(std::string &cmd) -> void
    {
        (void)cmd;
        if (STATUS == 0) {
            send_log(ERROR, "GAME NOT STARTED YET");
            return;
        }
        int dimension_half = board.getDimension() / 2;
        std::vector<std::vector<int>> myBoard = board.getBoard();
        myBoard[dimension_half][dimension_half] = 1;
        board.setBoard(myBoard);
        std::cout << dimension_half << "," << dimension_half << std::endl;
    }

    auto handle_turn(std::string &cmd) -> void
    {
        if (STATUS == 0) {
            send_log(ERROR, "GAME NOT STARTED YET");
            return;
        }
        std::vector<std::string> command;
        size_t pos = 0;
        std::string token;

        pos = cmd.find('\r');
        if (pos != std::string::npos)
            cmd.erase(pos);
        while ((pos = cmd.find(' ')) != std::string::npos) {
            token = cmd.substr(0, pos);
            command.push_back(token);
            cmd.erase(0, pos + 1);
        }
        command.push_back(cmd);
        if (command.size() != 2) {
            send_log(ERROR, "Too few or too many arguments.");
            return;
        }
        pos = 0;
        std::vector<std::string> args;
        while ((pos = command[1].find(',')) != std::string::npos){
            token = command[1].substr(0, pos);
            args.push_back(token);
            command[1].erase(0, pos + 1);
        }
        args.push_back(command[1]);
        if (args.size() != 2) {
            send_log(ERROR, "Too few or too many arguments.");
            return;
        }
        if (is_digits(args[0]) == false || is_digits(args[1]) == false) {
            send_log(ERROR, "Arguments aren't number.");
            return;
        }
        int posx = std::stoi(args[0]), posy = std::stoi(args[1]);
        int boardDimension = board.getDimension();
        if (posx >= boardDimension || posy >= boardDimension || posx < 0 || posy < 0) {
            send_log(ERROR, "Position is over the board.");
            return;
        }
        if (board.setPosition(posx, posy, 2) == false) {
            send_log(ERROR, "Position isn't free.");
            return;
        }
        Minimax minimax;
        std::tuple<int, int> result = minimax.bot_play(board.getBoard(), boardDimension);
        minimax.~Minimax();
        int ligne = std::get<0>(result), colonne = std::get<1>(result);
        if (board.setPosition(ligne, colonne, 1) == false) {
            send_log(ERROR, "Error with my algo");
            return;
        }
        board.displayMap();
        std::cout << ligne << "," << colonne << std::endl;
    }

    auto handle_board(std::string &cmd) -> void
    {
        if (STATUS == 0) {
            send_log(ERROR, "GAME NOT STARTED YET");
            return;
        }
        std::string input;
        if (cmd == "DONE") {
            Minimax minimax;
            int boardDimension = board.getDimension();
            std::tuple<int, int> result = minimax.bot_play(board.getBoard(), boardDimension);
            minimax.~Minimax();
            int ligne = std::get<0>(result), colonne = std::get<1>(result);
            if (board.setPosition(ligne, colonne, 1) == false)
            {
                send_log(ERROR, "Error with my algo");
                return;
            }
            //board.displayMap();
            std::cout << ligne << "," << colonne << std::endl;
            return;
        }
        if (cmd == "BOARD") {
            std::getline(std::cin, input);
            handle_board(input);
        } else {
            std::vector<std::string> words;
            size_t pos = 0;
            std::string token;

            while ((pos = cmd.find(',')) != std::string::npos) {
                token = cmd.substr(0, pos);
                words.push_back(token);
                cmd.erase(0, pos + 1);
            }
            words.push_back(cmd);
            if (words.size() != 3) {
                send_log(ERROR, "Not good info sent");
                std::getline(std::cin, input);
                handle_board(input);
            }
            std::vector<std::vector<int>> myBoard = board.getBoard();
            int x;
            int y;
            int type;
            try {
                x = stoi(words[0]);
                y = stoi(words[1]);
                type = stoi(words[2]);
            } catch (std::exception &e) {
                send_log(ERROR, "Not good info sent");
                std::getline(std::cin, input);
                handle_board(input);
            }
            if (x > board.getDimension() || y > board.getDimension() || type < 0 || type > 2) {
                send_log(ERROR, "Not good info sent");
                std::getline(std::cin, input);
                handle_board(input);
            }
            myBoard[stoi(words[0])][stoi(words[1])] = stoi(words[2]);
            board.setBoard(myBoard);
            //board.displayMap();
            std::getline(std::cin, input);
            handle_board(input);
        }
    }

    struct CommandMapping
    {
        std::string cmd;
        std::function<void(std::string &)> func;
    };

    const std::array<CommandMapping, 7> COMMAND_MAPPINGS{{
        {
            "ABOUT",
            handle_about,
        },
        {
            "START",
            handle_start,
        },
        {
            "END",
            handle_end,
        },
        {
            "INFO",
            handle_info,
        },
        {
            "BEGIN",
            handle_begin,
        },
        {
            "TURN",
            handle_turn,
        },
        {
            "BOARD",
            handle_board,
        },
    }};

    auto handle_command(std::string &cmd) -> void
    {
        for (const auto &i : COMMAND_MAPPINGS)
        {
            if (cmd.rfind(i.cmd, 0) == 0)
            {
                i.func(cmd);
                return;
            }
        }
        send_log(UNKNOWN, "command is not implemented");
    }

} // namespace

auto main() -> int
{
    for (std::string line; !SHOULD_STOP && std::getline(std::cin, line);)
    {
        handle_command(line);
    }
    //std::string start = "START 20";
    //std::string turn = "TURN 10,10";
    //std::string turn2 = "TURN 11,10";
    //std::string turn3 = "TURN 12,10";
    //handle_start(start);
    //handle_turn(turn);
    //handle_turn(turn2);
    //handle_turn(turn3);
    return 0;
}