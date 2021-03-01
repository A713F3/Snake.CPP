#include <iostream>
#include <windows.h>
#include <conio.h>

#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77

HANDLE hConsole;

const short int height = 20;
const short int width = 20;

bool game;

short int dir; //0:not moving 1:up 2:right 3:down 4:left

short int snakeX, snakeY, foodX, foodY;

short int score;

short int tailX[100], tailY[100];
short int nTail;

void cls()
    {
        COORD cursorPosition;	
        cursorPosition.X = 0;	
        cursorPosition.Y = 0;	
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursorPosition);
    }

void hidecursor(bool show)
{
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;   
    if (show){
        info.dwSize = 100;
        info.bVisible = FALSE;
    } else {  
        info.dwSize = 1;     
        info.bVisible = TRUE;
    }
    SetConsoleCursorInfo(consoleHandle, &info);
}

void init(){
    system("MODE CON COLS=45 LINES=25");

    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    game = true;

    dir = 0;

    score = 0;

    snakeX = width / 2;
    snakeY = height / 2;

    foodX = rand() % (width - 4) + 2;
    foodY = rand() % (height - 4) + 2;

    std::fill_n(tailX, 100, 0);
    std::fill_n(tailY, 100, 0);
    nTail = 0;

}

void draw(){
    cls();
    for (int y = 0; y < height + 2; y++){

        if (y == 0 || y == height + 1){ //Top and Bottom Wall
            SetConsoleTextAttribute(hConsole, BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | 
                                                FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            std::cout << std::string(width*2 + 5, '#') << std::endl;
            SetConsoleTextAttribute(hConsole, 0);

        } else {
            SetConsoleTextAttribute(hConsole, BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | 
                                                FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            std::cout << "##"; //Left Side Wall
            SetConsoleTextAttribute(hConsole, 0);

            for (int x = 0; x < width; x++){
            
                if (snakeX == x && snakeY == y){
                    SetConsoleTextAttribute(hConsole, BACKGROUND_BLUE | FOREGROUND_BLUE);
                    std::cout << "##"; //Player
                    SetConsoleTextAttribute(hConsole, 0);
                    
                } else if (foodX == x && foodY == y){ //Food
                    SetConsoleTextAttribute(hConsole, BACKGROUND_RED | FOREGROUND_RED);
                    std::cout << "##";
                    SetConsoleTextAttribute(hConsole, 0);
                } else {
                    bool printT = false;
                    for (int k = 0; k < nTail; k++){
                        
                        if (tailX[k] == x && tailY[k] == y && tailX[k] != 0 && tailY[k] != 0){ //Tails
                            SetConsoleTextAttribute(hConsole, BACKGROUND_BLUE | FOREGROUND_BLUE);
                            std::cout << "##";
                            SetConsoleTextAttribute(hConsole, 0);

                            printT = true;
                        }                         
                    }
                    if (! printT) std::cout << "  ";
                }
        }
        SetConsoleTextAttribute(hConsole, BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | 
                                                FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        std::cout << "###" << std::endl; // Right Side Wall
        //SetConsoleTextAttribute(hConsole, 0);
        }
    }
    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
    std::cout << std::endl << std::string(19, ' ') << "Score: " << score << std::endl; 
    SetConsoleTextAttribute(hConsole, 0);
}

void input(){
    if (_kbhit()){
        char key = _getch();
        if (key == 'w' || key == KEY_UP) dir = 1;
        if (key == 'd' || key == KEY_RIGHT) dir = 2;
        if (key == 's' || key == KEY_DOWN) dir = 3;
        if (key == 'a' || key == KEY_LEFT) dir = 4;
    }
}

void move(){
    int prevX = tailX[0];
    int prevY = tailY[0];
    int prev2X, prev2Y;
    tailX[0] = snakeX;
    tailY[0] = snakeY;

    for (int i = 1; i < nTail; i++){
        prev2X = tailX[i];
        prev2Y = tailY[i];
        tailX[i] = prevX;
        tailY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }


    switch(dir){
        case 0:
            break;
        case 1:
            snakeY --;
            break;
        case 2:
            snakeX ++;
            break;
        case 3:
            snakeY ++;
            break;
        case 4:
            snakeX --;
            break;
    }
}

void update(){
    if (snakeY == 1 || snakeY == height || snakeX == 0 || snakeX == width - 1) game = false;

    if (snakeX == foodX && snakeY == foodY){
        score ++;

        foodX = rand() % (width - 4) + 2;
        foodY = rand() % (height - 4) + 2;

        nTail ++;
    }
    for (int i = 0; i < nTail; i++){
        if (tailX[i] == snakeX && tailY[i] == snakeY) game = false;
    }
}

int main(){
    init();

    while (game){
        Sleep(100);
        hidecursor(true);
        input();
        move();
        update();
        draw();
    }

    Sleep(600);

    system("cls");

    hidecursor(false);

    SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
    std::cout << std::endl << std::string(17, ' ') << "Game Over!!" << std::endl;

    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
    std::cout << std::endl << std::string(18, ' ') << "Score: " << score << std::endl;

    SetConsoleTextAttribute(hConsole, 0);

    std::cin.get();

    return 0;
}