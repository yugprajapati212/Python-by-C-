#include <bits/stdc++.h>
#include <conio.h>
#include <windows.h>



using namespace std;

enum Direction { STOP = 0, LEFT, RIGHT, UP, DOWN };
void red () {
  printf("\033[1;31m");
}

void yellow() {
  printf("\033[1;33m");
}
void green(){
    printf("\033[0;32m");
    }
void cyan() {
  printf("\033[0;36m");
}
void reset () {
  printf("\033[0m");
}
void clearScreen() {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        COORD cursorPosition = {0, 0};
        SetConsoleCursorPosition(hConsole, cursorPosition);
    }

void printGameOver() {
    red();
    cout << "  * * *     *     *     *   * * *      * * *  *   *  * * *   * * * " << endl;
    cout << "  *        * *    *   *   *          *   *  *   *  *       *   *" << endl;
    cout << "  *  *  * * *   * * * *   * * *      *   *  *   *  * * *   * *" << endl;
    cout << "  *   *   *   *   *  *  *   *          *   *   * *   *       * *" << endl;
    cout << "  * * *   *   *   *     *   * * *      * * *    *    * * *   *   *" << endl;
    cout << "                                                    " << endl;
    reset();
}
class SnakeGame {
private:
    bool gameOver;
    const int width;
    const int height;
    int x, y, foodX, foodY, score;
    int speed; // Adjustable game speed
    Direction dir;
    vector<pair<int, int>> snake; // Snake body
    vector<pair<int, int>> obstacles; // Obstacles

public:
    SnakeGame(int w, int h, int spd) : width(w), height(h), gameOver(false), score(0), speed(spd), dir(STOP) {
        resetGame();
    }

    void resetGame() {
        gameOver = false;
        score = 0;
        dir = STOP;
        x = width / 2;
        y = height / 2;
        snake.clear();
        snake.push_back({x, y});
        foodX = rand() % width;
        foodY = rand() % height;
        obstacles.clear();
        system("cls");
        generateObstacles();
    }

    void generateObstacles() {
        // Generate random obstacles
        for (int i = 0; i < width / 5; i++) {
            int ox = rand() % width;
            int oy = rand() % height;
            if ((ox != x || oy != y) && (ox != foodX || oy != foodY)) {
                obstacles.push_back({ox, oy});
            }
        }
    }

    void draw() {
        //system("cls");
        clearScreen();
        // Draw top border
        for (int i = 0; i < width + 2; i++){ 
        cyan();
        cout << "#";
        reset();
        }
        cout << endl;

        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                if (j == 0){ cyan(); cout << "#";reset();}
                if (i == y && j == x){
                    green();
                    cout << "O"; // Snake head
                    reset();
                }
                else if (i == foodY && j == foodX){
                    yellow();
                    cout << "F"; // Food
                    reset();
                }
                else {
                    bool isBody = false;
                    for (auto part : snake) {
                        if (part.first == j && part.second == i) {
                            green();
                            cout << "o"; // Snake body
                            reset();
                            isBody = true;
                            break;
                        }
                    }
                    bool isObstacle = false;
                    for (auto obstacle : obstacles) {
                        if (obstacle.first == j && obstacle.second == i) {
                            red();
                            cout << "X"; // Obstacle
                            reset();
                            isObstacle = true;
                            break;
                        }
                    }
                    if (!isBody && !isObstacle) cout << " ";
                }
                if (j == width - 1){ 
                    cyan();
                    cout << "#";
                    reset();
                    }
            }
            cout << endl;
        }

        // Draw bottom border
        for (int i = 0; i < width + 2; i++){cyan(); cout << "#"; reset();}
        cout << endl;

        cout << "Score: " << score << endl;
    }

    void input() {
        if (_kbhit()) {
            Direction olddir=dir;
            switch (_getch()) {
                case 'a': dir = LEFT; break;
                case 'd': dir = RIGHT; break;
                case 'w': dir = UP; break;
                case 's': dir = DOWN; break;
                case 'x': gameOver = true; break;
                case 75: dir = LEFT; break;
                case 77: dir = RIGHT; break;
                case 72: dir = UP; break;
                case 80: dir = DOWN; break;
                //case 'x': gameOver = true; break;
            }

        if(olddir == UP && dir == DOWN) dir = olddir;
        else if(olddir == DOWN && dir == UP) dir = olddir;
        else if(olddir == LEFT && dir == RIGHT) dir = olddir;
        else if(olddir == RIGHT && dir == LEFT) dir = olddir;

        }
    }

    void logic() {
        // Update tail
        snake.insert(snake.begin(), {x, y});
        if (x == foodX && y == foodY) {
            score += 10;
            foodX = rand() % width;
            foodY = rand() % height;
            Beep(500,400);
        } else {
            snake.pop_back();
        }

        switch (dir) {
            case LEFT: x--; break;
            case RIGHT: x++; break;
            case UP: y--; break;
            case DOWN: y++; break;
            default: break;
        }

        // Check collisions with boundaries
        if (x < 0 || x >= width || y < 0 || y >= height)
            gameOver = true;

        // Check collisions with itself
        for (int i = 1; i < snake.size(); i++) {
            if (snake[i].first == x && snake[i].second == y) {
                gameOver = true;
                break;
            }
        }

        // Check collisions with obstacles
        for (auto obstacle : obstacles) {
            if (obstacle.first == x && obstacle.second == y) {
                gameOver = true;
                break;
            }
        }
    }

    void run() {
        int maxx=0;
        while (true) {
            while (!gameOver) {
                draw();
                input();
                logic();
                Sleep(speed); // Adjustable game speed
            }

            int maxx = max(maxx,score);
            printGameOver();
            Beep(440,1500);
            cout<<"Your score: " << score << endl;
            cout<<"Maximum Score: "<<maxx<<endl;
            char choice;
            cout << "Press 'R' to restart or any other key to exit: ";
            cin>>choice;
            if (choice == 'R' || choice == 'r') {
                resetGame();    
            } else {
                break;
            }
        }
    }
};


int main() {
    srand(static_cast<unsigned>(time(0)));
    int gridWidth, gridHeight, gameSpeed;
    char pref;
    cout<<" Enter length and width of the frame: (For standard size enter \"S\")"<<endl;
    cin>>pref;

    // Customizable grid size and speed
    if(pref=='s'||pref=='S'){
        gridHeight=40;
        gridWidth=80;
    }
    else{
    cout << "Enter grid width: ";
    cin >> gridWidth;
    cout << "Enter grid height: ";
    cin >> gridHeight;
    }
    char difficulty;
    cout << "Enter Difficulty level:(\"E\" for Easy, \"M\" for Medium, \"H\" for Hard): "<<endl;
    cin >> difficulty;
    if(difficulty == 'H'|| difficulty=='h'){
        gameSpeed=1;
    }
    else if(difficulty=='M'||difficulty=='m'){
        gameSpeed=50;
    }
    else if(difficulty=='E'||difficulty=='e'){
        gameSpeed=80;
    }

    SnakeGame game(gridWidth, gridHeight, gameSpeed);
    game.run();
    return 0;
}