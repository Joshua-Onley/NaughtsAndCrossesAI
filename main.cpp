git #include <iostream>
#include <array>
#include <SFML/Graphics.hpp>
#include <vector>
#include <limits>

int playerTurn = 0;  // 0 for player, 1 for AI
std::string result;


std::array<std::array<char, 3>, 3> board = {{
    {' ', ' ', ' '},
    {' ', ' ', ' '},
    {' ', ' ', ' '}
}};


int evaluate_position(const std::array<std::array<char, 3>, 3>& board) {

    for (int i = 0; i < 3; ++i) {
        if (board[i][0] == board[i][1] && board[i][1] == board[i][2]) {
            if (board[i][0] == 'X') return -1;  // Player win
            if (board[i][0] == 'O') return 1;   // AI win
        }
        if (board[0][i] == board[1][i] && board[1][i] == board[2][i]) {
            if (board[0][i] == 'X') return -1;
            if (board[0][i] == 'O') return 1;
        }
    }

    if (board[0][0] == board[1][1] && board[1][1] == board[2][2]) {
        if (board[0][0] == 'X') return -1;
        if (board[0][0] == 'O') return 1;
    }
    if (board[0][2] == board[1][1] && board[1][1] == board[2][0]) {
        if (board[0][2] == 'X') return -1;
        if (board[0][2] == 'O') return 1;
    }

    return 0;
}

std::vector<std::array<int, 2>> gen_moves(const std::array<std::array<char, 3>, 3>& board) {
    std::vector<std::array<int, 2>> moves;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (board[i][j] == ' ') {
                moves.push_back({i, j});
            }
        }
    }
    return moves;
}

int minimax(std::array<std::array<char, 3>, 3>& board, bool maximizing_player) {
    int score = evaluate_position(board);
    if (score == 1 || score == -1 || gen_moves(board).empty()) {
        return score;
    }

    if (maximizing_player) {
        int maxEval = std::numeric_limits<int>::min();
        for (const auto& move : gen_moves(board)) {
            board[move[0]][move[1]] = 'O';
            int eval = minimax(board, false);
            board[move[0]][move[1]] = ' ';  // Undo move
            maxEval = std::max(maxEval, eval);
        }
        return maxEval;
    } else {
        int minEval = std::numeric_limits<int>::max();
        for (const auto& move : gen_moves(board)) {
            board[move[0]][move[1]] = 'X';
            int eval = minimax(board, true);
            board[move[0]][move[1]] = ' ';
            minEval = std::min(minEval, eval);
        }
        return minEval;
    }
}

void make_computer_move(std::array<std::array<char, 3>, 3>& board) {
    auto possibleMoves = gen_moves(board);
    if (possibleMoves.empty()) return;

    int bestEval = std::numeric_limits<int>::min();
    std::array<int, 2> bestMove = {-1, -1};

    for (const auto& move : possibleMoves) {
        board[move[0]][move[1]] = 'O';
        int eval = minimax(board, false);
        board[move[0]][move[1]] = ' ';

        if (eval > bestEval) {
            bestEval = eval;
            bestMove = move;
        }
    }


    if (bestMove[0] != -1 && bestMove[1] != -1) {
        board[bestMove[0]][bestMove[1]] = 'O';
    }
}


void handle_click(sf::RenderWindow& window) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    const int cellSize = 100;
    int row = mousePos.y / cellSize;
    int col = mousePos.x / cellSize;

    if (row >= 0 && row < 3 && col >= 0 && col < 3) {

        if (board[row][col] == ' ') {
            board[row][col] = 'X';
            playerTurn = 1;
        }
    }
}


void draw_board(sf::RenderWindow& window) {
    const int cellSize = 100;
    const sf::Color lineColor = sf::Color::Black;


    sf::RectangleShape verticalLine(sf::Vector2f(2.f, 3 * cellSize));
    verticalLine.setFillColor(lineColor);
    sf::RectangleShape horizontalLine(sf::Vector2f(3 * cellSize, 2.f));
    horizontalLine.setFillColor(lineColor);

    for (int i = 1; i < 3; ++i) {
        verticalLine.setPosition(i * cellSize, 0);
        window.draw(verticalLine);
        horizontalLine.setPosition(0, i * cellSize);
        window.draw(horizontalLine);
    }


    sf::Font font;
    if (!font.loadFromFile("/System/Library/Fonts/Supplemental/Arial.ttf")) {
        std::cerr << "Failed to load font!" << std::endl;
        return;
    }

    for (int row = 0; row < 3; ++row) {
        for (int col = 0; col < 3; ++col) {
            sf::Text cellText;
            cellText.setFont(font);
            cellText.setCharacterSize(72);
            cellText.setFillColor(sf::Color::Black);
            cellText.setString(board[row][col]);
            cellText.setPosition(col * cellSize + cellSize / 4, row * cellSize);
            window.draw(cellText);
        }
    }
}


bool gameOver(const std::array<std::array<char, 3>, 3>& board, std::string& result) {

    for (int i = 0; i < 3; ++i) {
        if (board[i][0] == board[i][1] && board[i][1] == board[i][2] && board[i][0] != ' ') {
            result = (board[i][0] == 'X') ? "Player wins!" : "AI wins!";
            return true;
        }
        if (board[0][i] == board[1][i] && board[1][i] == board[2][i] && board[0][i] != ' ') {
            result = (board[0][i] == 'X') ? "Player wins!" : "AI wins!";
            return true;
        }
    }

    if (board[0][0] == board[1][1] && board[1][1] == board[2][2] && board[0][0] != ' ') {
        result = (board[0][0] == 'X') ? "Player wins!" : "AI wins!";
        return true;
    }
    if (board[0][2] == board[1][1] && board[1][1] == board[2][0] && board[0][2] != ' ') {
        result = (board[0][2] == 'X') ? "Player wins!" : "AI wins!";
        return true;
    }

    bool isFull = true;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (board[i][j] == ' ') {
                isFull = false;
                break;
            }
        }
    }
    if (isFull) {
        result = "It's a tie!";
        return true;
    }
    return false;
}

void reset_game() {
    board = {{{' ', ' ', ' '}, {' ', ' ', ' '}, {' ', ' ', ' '}}};
    playerTurn = 0;
    result.clear();
}

int main() {
    sf::RenderWindow window(sf::VideoMode(300, 300), "Tic-Tac-Toe");
    std::string result = "";
    bool isGameOver = false;
    sf::Font font;
    sf::Clock clock;

    // Load font
    if (!font.loadFromFile("/System/Library/Fonts/Supplemental/Arial.ttf")) {
        std::cerr << "Failed to load font!" << std::endl;
        return -1;
    }

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (!isGameOver && event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left && playerTurn == 0)
                handle_click(window);
            else if (isGameOver && event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {

                reset_game();
                isGameOver = false;
            }
        }

        if (!isGameOver && playerTurn == 1) {
            make_computer_move(board);
            playerTurn = 0;
        }


        if (!isGameOver && gameOver(board, result)) {
            isGameOver = true;
            clock.restart();
        }

        window.clear(sf::Color::White);
        draw_board(window);

        if (isGameOver) {
            sf::Text gameOverText;
            gameOverText.setFont(font);
            gameOverText.setString(result);
            gameOverText.setCharacterSize(40);
            gameOverText.setFillColor(sf::Color::Red);
            gameOverText.setPosition(50, 130);

            window.draw(gameOverText);
        }

        window.display();
    }

    return 0;
}

