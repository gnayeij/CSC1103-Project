#include "include/raylib.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define SCREEN_WIDTH 900
#define SCREEN_HEIGHT 1000
#define BOARD_SIZE 3
#define CELL_SIZE (SCREEN_WIDTH / BOARD_SIZE)
#define BUTTON_GAP 40
#define BUTTON_WIDTH 600
#define BUTTON_HEIGHT 80
#define BUTTON_FONT_SIZE 40
#define SCOREBOARD_Y (CELL_SIZE * BOARD_SIZE + 20)


// game states
typedef enum {
    LOBBY,
    DIFFICULTY_SELECT,
    GAME_TWO_PLAYER,
    GAME_VS_AI
} GameState;

// score structure
typedef struct {
    int playerX;
    int playerO;
    int draws;
} Score;

// AI difficulty levels
typedef enum {
    EASY = 1,
    MEDIUM = 2,
    HARD = 3
} AIDifficulty;

// game mode button structure
typedef struct {
    Rectangle bounds;
    const char *text;
    bool isHovered;
} Button;

// difficulty button structure
typedef struct {
    Button easy;
    Button medium;
    Button hard;
} DifficultyButtons;

// function prototypes
void initializeBoard(char board[BOARD_SIZE][BOARD_SIZE]);
void drawBoard(char board[BOARD_SIZE][BOARD_SIZE], Vector2 mousePos);
bool makeMove(char board[BOARD_SIZE][BOARD_SIZE], int row, int col, char player);
bool checkWin(char board[BOARD_SIZE][BOARD_SIZE], char player);
bool isBoardFull(char board[BOARD_SIZE][BOARD_SIZE]);
void resetGame(char board[BOARD_SIZE][BOARD_SIZE], char *currentPlayer, bool *gameWon);
void drawLobby(Button *twoPlayerButton, Button *onePlayerButton);
bool handleButton(Button *button);
void drawScoreboard(Score score, GameState gameState);
void updateScore(Score *score, bool gameWon, char currentPlayer);

void drawDifficultySelect(DifficultyButtons *buttons);

void aiMove(char board[BOARD_SIZE][BOARD_SIZE], char ai, int difficulty);
void makeRandomMove(char board[BOARD_SIZE][BOARD_SIZE], char player);
int minimax(char board[BOARD_SIZE][BOARD_SIZE], int depth, bool isMaximizing, char player, char ai);

// initialize difficulty selection buttons
DifficultyButtons initDifficultyButtons() {
    DifficultyButtons buttons;
    float startY = SCREEN_HEIGHT/2 - BUTTON_HEIGHT*2;
    
    buttons.easy = (Button){
        .bounds = (Rectangle){SCREEN_WIDTH/2 - BUTTON_WIDTH/2, startY, BUTTON_WIDTH, BUTTON_HEIGHT},
        .text = "Easy",
        .isHovered = false
    };
    
    buttons.medium = (Button){
        .bounds = (Rectangle){SCREEN_WIDTH/2 - BUTTON_WIDTH/2, startY + BUTTON_HEIGHT + BUTTON_GAP, BUTTON_WIDTH, BUTTON_HEIGHT},
        .text = "Medium",
        .isHovered = false
    };
    
    buttons.hard = (Button){
        .bounds = (Rectangle){SCREEN_WIDTH/2 - BUTTON_WIDTH/2, startY + (BUTTON_HEIGHT + BUTTON_GAP)*2, BUTTON_WIDTH, BUTTON_HEIGHT},
        .text = "Hard",
        .isHovered = false
    };
    
    return buttons;
}

void drawDifficultySelect(DifficultyButtons *buttons) {
    // draw title
    const char* title = "Select Difficulty";
    DrawText(title, SCREEN_WIDTH/2 - MeasureText(title, 50)/2, 100, 50, BLACK);
    
    // draw each button
    Color buttonColor;
    
    // easy button
    buttonColor = buttons->easy.isHovered ? (Color){ 124, 163, 124, 255} : (Color){ 136, 231, 136, 255};
    DrawRectangleRec(buttons->easy.bounds, buttonColor);
    DrawText(buttons->easy.text, 
            buttons->easy.bounds.x + (buttons->easy.bounds.width - MeasureText(buttons->easy.text, BUTTON_FONT_SIZE))/2,
            buttons->easy.bounds.y + (buttons->easy.bounds.height - BUTTON_FONT_SIZE)/2,
            BUTTON_FONT_SIZE, WHITE);
            
    // medium button
    buttonColor = buttons->medium.isHovered ? (Color){219, 164, 0, 255} : (Color){255,206,27, 255};
    DrawRectangleRec(buttons->medium.bounds, buttonColor);
    DrawText(buttons->medium.text,
            buttons->medium.bounds.x + (buttons->medium.bounds.width - MeasureText(buttons->medium.text, BUTTON_FONT_SIZE))/2,
            buttons->medium.bounds.y + (buttons->medium.bounds.height - BUTTON_FONT_SIZE)/2,
            BUTTON_FONT_SIZE, WHITE);
            
    // hard button
    buttonColor = buttons->hard.isHovered ? (Color){155, 19, 19, 255} : (Color){205,28,24,255};
    DrawRectangleRec(buttons->hard.bounds, buttonColor);
    DrawText(buttons->hard.text,
            buttons->hard.bounds.x + (buttons->hard.bounds.width - MeasureText(buttons->hard.text, BUTTON_FONT_SIZE))/2,
            buttons->hard.bounds.y + (buttons->hard.bounds.height - BUTTON_FONT_SIZE)/2,
            BUTTON_FONT_SIZE, WHITE);
            
    // Draw return instruction
    DrawText("Press 'E' to return to menu", 10, SCREEN_HEIGHT - 30, 20, DARKGRAY);
}

int main(void) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Tic Tac Toe");
    SetTargetFPS(60);

    // initialize game state
    GameState currentState = LOBBY;
    
    // initialize AI difficulty
    AIDifficulty aiDifficulty = EASY;

    // initialize difficulty selection buttons
    DifficultyButtons difficultyButtons = initDifficultyButtons();

    // initialize scores
    Score score = {
        .playerX = 0,
        .playerO = 0,
        .draws = 0
    };

    // initialize button
    Button twoPlayerButton = {
        .bounds = (Rectangle){
            SCREEN_WIDTH/2 - BUTTON_WIDTH/2, 
            SCREEN_HEIGHT/2 - BUTTON_HEIGHT - 40/2, 
            BUTTON_WIDTH, 
            BUTTON_HEIGHT},
        .text = "2 Player Mode",
        .isHovered = false
    };

    Button onePlayerButton = {
        .bounds = (Rectangle){
            SCREEN_WIDTH/2 - BUTTON_WIDTH/2, 
            SCREEN_HEIGHT/2 + 40/2, 
            BUTTON_WIDTH, 
            BUTTON_HEIGHT
        },
        .text = "1 Player Mode (VS AI)",
        .isHovered = false
    };

    // game variables
    char board[BOARD_SIZE][BOARD_SIZE];
    char currentPlayer = 'X';
    bool gameWon = false;
    bool gameOver = false;

    initializeBoard(board);

    while (!WindowShouldClose()) {
        Vector2 mousePos = GetMousePosition();
        // update
        switch (currentState) {
            case LOBBY:
                twoPlayerButton.isHovered = CheckCollisionPointRec(GetMousePosition(), twoPlayerButton.bounds);
                onePlayerButton.isHovered = CheckCollisionPointRec(GetMousePosition(), onePlayerButton.bounds);

                if (handleButton(&twoPlayerButton)) {
                    currentState = GAME_TWO_PLAYER;
                    resetGame(board, &currentPlayer, &gameWon);
                    gameOver = false;

                    // reset score every new game session
                    score.playerX = 0;
                    score.playerO = 0;
                    score.draws = 0;
                }

                if (handleButton(&onePlayerButton)) {
                    currentState = DIFFICULTY_SELECT;
                }
                break;
            case DIFFICULTY_SELECT:
                // Update button hover states
                difficultyButtons.easy.isHovered = CheckCollisionPointRec(mousePos, difficultyButtons.easy.bounds);
                difficultyButtons.medium.isHovered = CheckCollisionPointRec(mousePos, difficultyButtons.medium.bounds);
                difficultyButtons.hard.isHovered = CheckCollisionPointRec(mousePos, difficultyButtons.hard.bounds);
                
                // Handle button clicks
                if (handleButton(&difficultyButtons.easy)) {
                    aiDifficulty = EASY;
                    currentState = GAME_VS_AI;
                    resetGame(board, &currentPlayer, &gameWon);
                    gameOver = false;
                    score.playerX = 0;
                    score.playerO = 0;
                    score.draws = 0;
                }
                else if (handleButton(&difficultyButtons.medium)) {
                    aiDifficulty = MEDIUM;
                    currentState = GAME_VS_AI;
                    resetGame(board, &currentPlayer, &gameWon);
                    gameOver = false;
                    score.playerX = 0;
                    score.playerO = 0;
                    score.draws = 0;
                }
                else if (handleButton(&difficultyButtons.hard)) {
                    aiDifficulty = HARD;
                    currentState = GAME_VS_AI;
                    resetGame(board, &currentPlayer, &gameWon);
                    gameOver = false;
                    score.playerX = 0;
                    score.playerO = 0;
                    score.draws = 0;
                }
                
                if (IsKeyPressed(KEY_E)) {
                    currentState = LOBBY;
                }
                break;

            case GAME_TWO_PLAYER:
                // check for restart
                if (gameOver && IsKeyPressed(KEY_SPACE)) {
                    resetGame(board, &currentPlayer, &gameWon);
                    gameOver = false;
                }
                
                // back to lobby
                if (IsKeyPressed(KEY_E)) {
                    currentState = LOBBY;
                    continue;
                }

                // game logic
                if (!gameOver) {
                    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                        // Vector2 mousePos = GetMousePosition();
                        int row = mousePos.y / CELL_SIZE;
                        int col = mousePos.x / CELL_SIZE;

                        if (makeMove(board, row, col, currentPlayer)) {
                            gameWon = checkWin(board, currentPlayer);
                            if (!gameWon) {
                                currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
                            }
                            if (gameWon || isBoardFull(board)) {
                                gameOver = true;
                                // update score
                                updateScore(&score, gameWon, currentPlayer);
                            }
                        }
                    }
                }
                break;
            
            case GAME_VS_AI:
                // check for restart
                if (gameOver && IsKeyPressed(KEY_SPACE)) {
                    resetGame(board, &currentPlayer, &gameWon);
                    gameOver = false;
                }
                
                // back to lobby
                if (IsKeyPressed(KEY_E)) {
                    currentState = LOBBY;
                    continue;
                }

                if (!gameOver) {
                    if (currentPlayer == 'X') {  // player's turn
                        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                            int row = mousePos.y / CELL_SIZE;
                            int col = mousePos.x / CELL_SIZE;

                            if (makeMove(board, row, col, currentPlayer)) {
                                gameWon = checkWin(board, currentPlayer);
                                if (!gameWon) {
                                    currentPlayer = 'O';  // switch to AI's turn
                                }
                                if (gameWon || isBoardFull(board)) {
                                    gameOver = true;
                                    updateScore(&score, gameWon, currentPlayer);
                                }
                            }
                        }
                    } else {  // AI's turn
                        static float aiDelay = 0.5f;  // half second delay
                        static float aiTimer = 0.0f;
                        
                        aiTimer += GetFrameTime();
                        if (aiTimer >= aiDelay) {
                            aiMove(board, 'O', aiDifficulty);
                            gameWon = checkWin(board, currentPlayer);
                            if (gameWon || isBoardFull(board)) {
                                gameOver = true;
                                updateScore(&score, gameWon, currentPlayer);
                            }
                            if (!gameWon) {
                                currentPlayer = 'X';  // switch back to player's turn
                            }
                            aiTimer = 0.0f;  // reset timer
                        }
                    }
                }
                break;
        }

        // draw canvas
        BeginDrawing();
        Color softBeige = {237, 232, 208, 255};
        ClearBackground(softBeige);

        // define common messages outside switch to prevent redefinition error
        const char* returnMessage = "Press E to return to Lobby";
        const char* restartMsg = "Press SPACEBAR to restart";

        switch (currentState) {
            case LOBBY:
                drawLobby(&twoPlayerButton, &onePlayerButton);
                break;

            case DIFFICULTY_SELECT:
                drawDifficultySelect(&difficultyButtons);
                break;

            case GAME_TWO_PLAYER:
                drawBoard(board, mousePos);
                drawScoreboard(score, currentState);

                // draw game status
                if (gameOver) {
                    if (gameWon) {
                        const char* message = TextFormat("Player %c wins!", currentPlayer);
                        DrawText(message, SCREEN_WIDTH/2 - MeasureText(message, 40)/2, SCREEN_HEIGHT/2 - 20, 40, GREEN);
                    } else {
                        const char* message = "It's a draw!";
                        DrawText(message, SCREEN_WIDTH/2 - MeasureText(message, 40)/2, SCREEN_HEIGHT/2 - 20, 40, BLACK);
                    }
                    DrawText(restartMsg, SCREEN_WIDTH/2 - MeasureText(restartMsg, 20)/2, SCREEN_HEIGHT/2 + 30, 20, DARKGRAY);
                } else {
                    const char* message = TextFormat("Player %c's turn", currentPlayer);
                    DrawText(message, SCREEN_WIDTH/2 - MeasureText(message, 50)/2, 20, 50, DARKGRAY);

                }
                
                // draw return to lobby message
                DrawText(returnMessage, SCREEN_WIDTH/2 - MeasureText(returnMessage, 20)/2, SCREEN_HEIGHT-20, 20, DARKGRAY);
                break;
            case GAME_VS_AI:
                drawBoard(board, mousePos);
                drawScoreboard(score, currentState);

                // draw game status
                if (gameOver) {
                    if (gameWon) {
                        const char* message = (currentPlayer == 'X') ? "You win!" : "AI wins!";
                        DrawText(message, SCREEN_WIDTH/2 - MeasureText(message, 40)/2, SCREEN_HEIGHT/2 - 20, 40, GREEN);
                    } else {
                        const char* message = "It's a draw!";
                        DrawText(message, SCREEN_WIDTH/2 - MeasureText(message, 40)/2, SCREEN_HEIGHT/2 - 20, 40, BLACK);
                    }
                    DrawText(restartMsg, SCREEN_WIDTH/2 - MeasureText(restartMsg, 20)/2, SCREEN_HEIGHT/2 + 30, 20, DARKGRAY);
                } else {
                    const char* message;
                    if (currentPlayer == 'X') {
                        message = "Your turn";
                    } else {
                        message = "AI is thinking...";
                    }
                    DrawText(message, SCREEN_WIDTH/2 - MeasureText(message, 50)/2, 20, 50, DARKGRAY);
                }
                
                // draw return to lobby message
                DrawText(returnMessage, SCREEN_WIDTH/2 - MeasureText(returnMessage, 20)/2, SCREEN_HEIGHT-20, 20, DARKGRAY);
                break;
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}

void drawLobby(Button *twoPlayerButton, Button *onePlayerButton) {
    // draw title
    const char* title = "Tic Tac Toe";
    DrawText(title, SCREEN_WIDTH/2 - MeasureText(title, 50)/2, 100, 50, BLACK);

    // draw 2 players button
    Color twoplayer_buttonColor = twoPlayerButton->isHovered ? DARKGRAY : GRAY;
    DrawRectangleRec(twoPlayerButton->bounds, twoplayer_buttonColor);
    int textWidth = MeasureText(twoPlayerButton -> text, BUTTON_FONT_SIZE);
    int textX = twoPlayerButton -> bounds.x + (twoPlayerButton -> bounds.width - textWidth)/2;
    int textY = twoPlayerButton -> bounds.y + (twoPlayerButton -> bounds.height - BUTTON_FONT_SIZE)/2;
    DrawText(twoPlayerButton->text, textX, textY, BUTTON_FONT_SIZE, WHITE);

    // draw 1 player vs ai button
    Color oneplayer_buttonColor = onePlayerButton->isHovered ? DARKGRAY : GRAY;
    DrawRectangleRec(onePlayerButton->bounds, oneplayer_buttonColor);
    textWidth = MeasureText(onePlayerButton -> text, BUTTON_FONT_SIZE);
    textX = onePlayerButton -> bounds.x + (onePlayerButton -> bounds.width - textWidth)/2;
    textY = onePlayerButton -> bounds.y + (onePlayerButton -> bounds.height - BUTTON_FONT_SIZE)/2;
    DrawText(onePlayerButton->text, textX, textY, BUTTON_FONT_SIZE, WHITE);
    
}

bool handleButton(Button *button) {
    return button->isHovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
}

// reset game function
void resetGame(char board[BOARD_SIZE][BOARD_SIZE], char *currentPlayer, bool *gameWon) {
    initializeBoard(board);
    *currentPlayer = 'X';
    *gameWon = false;
}

void initializeBoard(char board[BOARD_SIZE][BOARD_SIZE]) {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            board[i][j] = ' ';
        }
    }
}

void drawBoard(char board[BOARD_SIZE][BOARD_SIZE], Vector2 mousePos) {
    int hoverRow = mousePos.y / CELL_SIZE;
    int hoverCol = mousePos.x / CELL_SIZE;

    // draw cells for hover effects
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            Rectangle cellRect = { j * CELL_SIZE, i * CELL_SIZE, CELL_SIZE, CELL_SIZE };
            
            // hover effect
            if (i == hoverRow && j == hoverCol && board[i][j] == ' ') {
                DrawRectangleRec(cellRect, (Color){ 220, 220, 200, 255 }); // grid changes colour when hovering
            }
        }
    }

    // draw grid lines
    for (int i = 1; i < BOARD_SIZE; i++) {
        DrawLine(i * CELL_SIZE, 0, i * CELL_SIZE, SCREEN_HEIGHT, BLACK);
        DrawLine(0, i * CELL_SIZE, SCREEN_WIDTH, i * CELL_SIZE, BLACK);
    }

    // draw X's and O's
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (board[i][j] == 'X') {
                DrawText("X", j * CELL_SIZE + CELL_SIZE/3, i * CELL_SIZE + CELL_SIZE/4, CELL_SIZE/2, RED);
            } else if (board[i][j] == 'O') {
                DrawText("O", j * CELL_SIZE + CELL_SIZE/3, i * CELL_SIZE + CELL_SIZE/4, CELL_SIZE/2, BLUE);
            }
        }
    }
}

bool makeMove(char board[BOARD_SIZE][BOARD_SIZE], int row, int col, char player) {
    if (row < 0 || row >= BOARD_SIZE || col < 0 || col >= BOARD_SIZE || board[row][col] != ' ') {
        return false;
    }
    board[row][col] = player;
    return true;
}

bool checkWin(char board[BOARD_SIZE][BOARD_SIZE], char player) {
    for (int i = 0; i < BOARD_SIZE; i++) {
        if ((board[i][0] == player && board[i][1] == player && board[i][2] == player) ||
            (board[0][i] == player && board[1][i] == player && board[2][i] == player)) {
            return true;
        }
    }
    if ((board[0][0] == player && board[1][1] == player && board[2][2] == player) ||
        (board[0][2] == player && board[1][1] == player && board[2][0] == player)) {
        return true;
    }
    return false;
}

bool isBoardFull(char board[BOARD_SIZE][BOARD_SIZE]) {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (board[i][j] == ' ') {
                return false;
            }
        }
    }
    return true;
}

// score display for different game modes
void drawScoreboard(Score score, GameState gameState) {
    const int fontSize = 30;
    const int spacing = SCREEN_WIDTH / 3;
    const int baseY = SCOREBOARD_Y;
    
    if (gameState == GAME_TWO_PLAYER) {
        DrawText(TextFormat("X: %d", score.playerX), spacing/2 - 40, baseY, fontSize, RED);
        DrawText(TextFormat("Draw: %d", score.draws), SCREEN_WIDTH/2 - 50, baseY, fontSize, DARKGRAY);
        DrawText(TextFormat("O: %d", score.playerO), spacing * 2 + spacing/2 - 40, baseY, fontSize, BLUE);
    } else if (gameState == GAME_VS_AI) {
        DrawText(TextFormat("Player: %d", score.playerX), spacing/2 - 60, baseY, fontSize, RED);
        DrawText(TextFormat("Draw: %d", score.draws), SCREEN_WIDTH/2 - 50, baseY, fontSize, DARKGRAY);
        DrawText(TextFormat("AI: %d", score.playerO), spacing * 2 + spacing/2 - 40, baseY, fontSize, BLUE);
    }
}

void updateScore(Score *score, bool gameWon, char currentPlayer) {
    if (gameWon) {
        if (currentPlayer == 'X') {
            score->playerX++;
        } else {
            score->playerO++;
        }
    } else {
        score->draws++;
    }
}

void aiMove(char board[BOARD_SIZE][BOARD_SIZE], char ai, int difficulty) {
    // Easy difficulty: random move
    if (difficulty == EASY) {
        makeRandomMove(board, ai);
        return;
    }

    // Medium difficulty: 70% optimal move, 30% random
    if (difficulty == MEDIUM && (rand() % 10) < 3) {
        makeRandomMove(board, ai);
        return;
    }

    // Medium/Hard: Use minimax to find best move
    int bestScore = 1000;
    int bestMove[2] = {-1, -1};

    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (board[i][j] == ' ') {
                board[i][j] = ai;
                int score = minimax(board, 0, true, 'X', ai);
                board[i][j] = ' ';

                if (score < bestScore) {
                    bestScore = score;
                    bestMove[0] = i;
                    bestMove[1] = j;
                }
            }
        }
    }

    if (bestMove[0] != -1 && bestMove[1] != -1) {
        makeMove(board, bestMove[0], bestMove[1], ai);
    }
}

void makeRandomMove(char board[BOARD_SIZE][BOARD_SIZE], char player) {
    int available[BOARD_SIZE * BOARD_SIZE][2];
    int count = 0;

    // Find all empty positions
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (board[i][j] == ' ') {
                available[count][0] = i;
                available[count][1] = j;
                count++;
            }
        }
    }

    if (count > 0) {
        int randIndex = rand() % count;
        makeMove(board, available[randIndex][0], available[randIndex][1], player);
    }
}

int minimax(char board[BOARD_SIZE][BOARD_SIZE], int depth, bool isMaximizing, char player, char ai) {
    // Check terminal states
    if (checkWin(board, ai)) return depth - 10;
    if (checkWin(board, player)) return 10 - depth;
    if (isBoardFull(board)) return 0;

    if (isMaximizing) {
        int bestScore = -1000;
        for (int i = 0; i < BOARD_SIZE; i++) {
            for (int j = 0; j < BOARD_SIZE; j++) {
                if (board[i][j] == ' ') {
                    board[i][j] = player;
                    int score = minimax(board, depth + 1, false, player, ai);
                    board[i][j] = ' ';
                    bestScore = (score > bestScore) ? score : bestScore;
                }
            }
        }
        return bestScore;
    } else {
        int bestScore = 1000;
        for (int i = 0; i < BOARD_SIZE; i++) {
            for (int j = 0; j < BOARD_SIZE; j++) {
                if (board[i][j] == ' ') {
                    board[i][j] = ai;
                    int score = minimax(board, depth + 1, true, player, ai);
                    board[i][j] = ' ';
                    bestScore = (score < bestScore) ? score : bestScore;
                }
            }
        }
        return bestScore;
    }
}