#include <iostream>
#include <sstream>
#include <windows.h>  //I tried to stick to the librariess taught by Sir waseem only
using namespace std;

const int r = 20;
const int c = 50;
char b[r][c];
int px = r - 2, py = c / 2, sc = 0, hp = 3, damage = 0, et = 0;
char pb = '|', b1 = '*', b2 = '+', b3 = '~';
char e1 = 'E', e2 = 'F', e3 = 'G', p = 'P';
bool g = false;
int layer = 1;

void clr() {
    for (int i = 0; i < r; i++)
        for (int j = 0; j < c; j++)
            b[i][j] = ' ';
    for (int i = 0; i < r; i++) {
        b[i][0] = '|';
        b[i][c - 1] = '|';
    }
    for (int j = 0; j < c; j++) {
        b[0][j] = '-';
        b[r - 1][j] = '-';
    }
}

void printBoard() {
    COORD pos = { 0, 0 };
    //sets position of baord 
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);

    std::stringstream frame;

//will represent score board
    frame << "+--------------------------------------------------+" << endl;
    frame << "|   SCORE: " << sc
         << "   LIVES: " << hp
         << "   DAMAGE: " << damage << "/5"
         << "   WAVE: " << layer << "   |" << endl;
    frame << "+--------------------------------------------------+" << endl;

    for (int i = 0; i < r; i++) {
        frame << "|";
        for (int j = 1; j < c - 1; j++) {
            frame << b[i][j];
        }
        frame << "|" << endl;
    }

    frame << "+--------------------------------------------------+" << endl;

    if (g) {
        string msg = ">>> GAME OVER <<<";
        int padding = (50 - msg.length()) / 2;
        frame << string(padding, ' ') << msg << endl;
    }

    cout << frame.str();
}


void movePlayerLeft() {
    if (py > 1) {
        b[px][py] = ' ';
        py--;
        b[px][py] = p;
    }
}

void movePlayerRight() {
    if (py < c - 2) {
        b[px][py] = ' ';
        py++;
        b[px][py] = p;
    }
}

void fire() {
    if (px > 1 && b[px - 1][py] == ' ')
        b[px - 1][py] = pb;
}

void moveFire(int) {
    for (int i = 1; i < r - 1; i++) {
        for (int j = 1; j < c - 1; j++) {
            if (b[i][j] == pb) {
                if (i - 1 <= 0) {
                    b[i][j] = ' ';
                    continue;
                }
                if (b[i - 1][j] == ' ') {
                    b[i - 1][j] = pb;
                } else if (b[i - 1][j] == e1 || b[i - 1][j] == e2 || b[i - 1][j] == e3) {
                    b[i - 1][j] = ' ';
                    sc += 10;
                }
                b[i][j] = ' ';
            }
        }
    }
}

void moveEnemy(char o, string d) {
    for (int i = 1; i < r - 1; i++) {
        for (int j = 1; j < c - 1; j++) {
            if (b[i][j] == o) {
                int ni = i, nj = j;
                if (d == "left" && j > 1) nj--;
                else if (d == "right" && j < c - 2) nj++;
                else if (d == "down" && i < r - 2) ni++;
                if (b[ni][nj] == ' ') {
                    b[ni][nj] = o;
                    b[i][j] = ' ';
                }
                return;
            }
        }
    }
}

void enemyFire(int timeStep) {
    if (timeStep % 20 != 0) return;
    for (int j = 1; j < c - 1; j++) {
        if (layer == 1 && b[1][j] == e1 && b[2][j] == ' ') b[2][j] = b1;
        else if (layer == 2 && b[1][j] == e2 && b[2][j] == ' ') b[2][j] = b2;
        else if (layer == 3 && b[1][j] == e3 && b[2][j] == ' ') b[2][j] = b3;
    }
}

void moveEnemyFire() {
    for (int i = r - 2; i >= 1; i--) {
        for (int j = 1; j < c - 1; j++) {
            if (b[i][j] == b1 || b[i][j] == b2 || b[i][j] == b3) {
                if (i + 1 >= r - 1) { b[i][j] = ' '; continue; }
                if (b[i + 1][j] == ' ') {
                    b[i + 1][j] = b[i][j];
                } else if (b[i + 1][j] == p) {
                    damage++;
                    if (damage >= 5) {
                        hp--;
                        damage = 0;
                    }
                    if (hp <= 0) g = true;
                    b[i + 1][j] = p;
                }
                b[i][j] = ' ';
            }
        }
    }
}

bool layerDead(int lay) {
    for (int i = 1; i < r - 1; i++)
        for (int j = 1; j < c - 1; j++)
            if ((lay == 1 && b[i][j] == e1) || (lay == 2 && b[i][j] == e2) || (lay == 3 && b[i][j] == e3))
                return false;
    return true;
}

void spawnLayer(int lay) {
    if (lay == 1) {
        b[1][10] = e1; b[1][30] = e1; b[1][40] = e1;
    } else if (lay == 2) {
        int pos[5] = { 5, 15, 25, 35, 45 };
        for (int i = 0; i < 5; i++) b[1][pos[i]] = e2;
    } else if (lay == 3) {
        int pos[7] = { 3, 10, 17, 25, 33, 40, 47 };
        for (int i = 0; i < 7; i++) b[1][pos[i]] = e3;
    }
}

int main() {
    clr();
    std::cout << "\033[?25l"; // this ASCII sequence hides carriage "|" in terminal
    b[px][py] = p;
    spawnLayer(layer);

    while (!g) {
        printBoard();

        if (GetAsyncKeyState(VK_LEFT) & 0x8000) movePlayerLeft();  
		 // Only major built in function used 
        if (GetAsyncKeyState(VK_RIGHT) & 0x8000) movePlayerRight();
        if (GetAsyncKeyState(VK_SPACE) & 0x8000) fire();
        //continously press space for better firing and long press for non stop firing

        moveFire(1);
        et++;
        enemyFire(et);
        moveEnemyFire();
        moveEnemy(e1, "right");
        moveEnemy(e2, "left");
        moveEnemy(e3, "right");

        if (layerDead(layer)) {
            layer++;
            if (layer > 3) layer = 1;
            clr();
            b[px][py] = p;
            spawnLayer(layer);
        }
          Sleep(55);
		 //this will make that disappearing effect go off but will also slow the game
}
    printBoard();
    system("pause");
    std::cout << "\033[?25h"; // this ASCII sequence shows carriage "|" in terminal
    return 0;
}
