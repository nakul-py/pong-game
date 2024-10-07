#include <iostream>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h> // Add this for fcntl
#include <cstdlib>
#include <ctime>

using namespace std;

const int WIDTH = 40;
const int HEIGHT = 20;

enum Direction { STOP = 0, LEFT, RIGHT, UPLEFT, UPRIGHT, DOWNLEFT, DOWNRIGHT };

class Ball {
public:
    int x, y;
    int startX, startY;
    Direction dir;

    Ball(int posX, int posY) {
        startX = posX;
        startY = posY;
        x = posX;
        y = posY;
        dir = STOP;
    }

    void Reset() {
        x = startX;
        y = startY;
        dir = STOP;
    }

    void ChangeDirection(Direction d) {
        dir = d;
    }

    void RandomDirection() {
        dir = (Direction)((rand() % 6) + 1); 
    }

    void Move() {
        switch (dir) {
        case STOP:
            break;
        case LEFT:
            x--;
            break;
        case RIGHT:
            x++;
            break;
        case UPLEFT:
            x--;
            y--;
            break;
        case UPRIGHT:
            x++;
            y--;
            break;
        case DOWNLEFT:
            x--;
            y++;
            break;
        case DOWNRIGHT:
            x++;
            y++;
            break;
        }
    }
};

class Paddle {
public:
    int x, y;
    int startX, startY;

    Paddle(int posX, int posY) {
        startX = posX;
        startY = posY;
        x = posX;
        y = posY;
    }

    void Reset() {
        x = startX;
        y = startY;
    }

    void MoveUp() {
        if (y > 0)
            y--;
    }

    void MoveDown() {
        if (y + 3 < HEIGHT)
            y++;
    }
};

void nonblockingInput() {
    termios term;
    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

int _kbhit() {
    termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);  
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);  

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if (ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }

    return 0;
}

char _getch() {
    char buf = 0;
    struct termios old = {0};
    if (tcgetattr(STDIN_FILENO, &old) < 0)
        perror("tcsetattr()");
    old.c_lflag &= ~ICANON;
    old.c_lflag &= ~ECHO;
    old.c_cc[VMIN] = 1;
    old.c_cc[VTIME] = 0;
    if (tcsetattr(STDIN_FILENO, TCSANOW, &old) < 0)
        perror("tcsetattr ICANON");
    if (read(STDIN_FILENO, &buf, 1) < 0)
        perror("read()");
    old.c_lflag |= ICANON;
    old.c_lflag |= ECHO;
    if (tcsetattr(STDIN_FILENO, TCSADRAIN, &old) < 0)
        perror("tcsetattr ~ICANON");
    return buf;
}

class GameManager {
private:
    int score1, score2;
    bool quit;
    Ball *ball;
    Paddle *player1, *player2;

public:
    GameManager() {
        quit = false;
        score1 = 0;
        score2 = 0;
        ball = new Ball(WIDTH / 2, HEIGHT / 2);
        player1 = new Paddle(1, HEIGHT / 2 - 2);
        player2 = new Paddle(WIDTH - 2, HEIGHT / 2 - 2);
        ball->RandomDirection();
    }

    ~GameManager() {
        delete ball;
        delete player1;
        delete player2;
    }

    void Draw() {
        system("clear"); 
        for (int i = 0; i < WIDTH + 2; i++) cout << "#";
        cout << endl;

        for (int i = 0; i < HEIGHT; i++) {
            for (int j = 0; j < WIDTH; j++) {
                if (j == 0) cout << "#";

                if (ball->x == j && ball->y == i) cout << "O";
                else if (player1->x == j && (player1->y == i || player1->y + 1 == i || player1->y + 2 == i)) cout << "|";
                else if (player2->x == j && (player2->y == i || player2->y + 1 == i || player2->y + 2 == i)) cout << "|";
                else cout << " ";

                if (j == WIDTH - 1) cout << "#";
            }
            cout << endl;
        }

        for (int i = 0; i < WIDTH + 2; i++) cout << "#";
        cout << endl;

        cout << "Player 1: " << score1 << "  Player 2: " << score2 << endl;
    }

    void Input() {
        if (_kbhit()) {
            char current = _getch();

            if (current == 'w') player1->MoveUp();
            if (current == 's') player1->MoveDown();
            if (current == 'i') player2->MoveUp();
            if (current == 'k') player2->MoveDown();

            if (current == 'q') quit = true;

            if (ball->dir == STOP) ball->RandomDirection();
        }
    }

    void Logic() {
        ball->Move();

        if (ball->x == player1->x + 1) {
            if (ball->y >= player1->y && ball->y <= player1->y + 2) ball->ChangeDirection((Direction)((rand() % 3) + 4));
        }
        if (ball->x == player2->x - 1) {
            if (ball->y >= player2->y && ball->y <= player2->y + 2) ball->ChangeDirection((Direction)((rand() % 3) + 1));
        }

        if (ball->y == 0) ball->ChangeDirection((ball->dir == UPRIGHT) ? DOWNRIGHT : DOWNLEFT);
        if (ball->y == HEIGHT - 1) ball->ChangeDirection((ball->dir == DOWNRIGHT) ? UPRIGHT : UPLEFT);

        if (ball->x == 0) { score2++; ball->Reset(); }
        if (ball->x == WIDTH - 1) { score1++; ball->Reset(); }
    }

    void Run() {
        nonblockingInput();
        while (!quit) {
            Draw();
            Input();
            Logic();
            usleep(); 
        }
    }
};

int main() {
    GameManager game;
    game.Run();
    return 0;
}
