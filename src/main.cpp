#include <iostream> // Input & Output
#include <stdlib.h> // RNG
#include <time.h> // Time for generating seed
#include "rlutil.h" // Terminal coloring

using namespace std;

class Field {
  public:
    int value;
    bool visible = false;
    bool flagged = false;
};

int height, width;
Field field[24][24];

//Prints normal points
void printc (int i, int j, bool cursor) {
  if (cursor) {
    rlutil::setBackgroundColor(15);
    rlutil::setColor(0);
  }
  else if (!field[i][j].visible) rlutil::setColor(7);
  else if (field[i][j].value == 1) rlutil::setColor(1);
  else if (field[i][j].value == 2) rlutil::setColor(2);
  else if (field[i][j].value == 3) rlutil::setColor(12);
  else if (field[i][j].value == 4) rlutil::setColor(13);
  else if (field[i][j].value == 5) rlutil::setColor(4);
  else if (field[i][j].value == 6) rlutil::setColor(11);
  else if (field[i][j].value == 7) rlutil::setColor(14);
  else if (field[i][j].value == 8) rlutil::setColor(8);

  if(field[i][j].flagged) cout << "F";
  else if (!field[i][j].visible) cout << "#";
  else if (field[i][j].value == 9) cout << "B";
  else if (field[i][j].value == 0) cout << " ";
  else cout << field[i][j].value;
  rlutil::resetColor();
  cout << " ";
}

int countBombs (int x, int y) {
  int c = 0;
  if(y < height-1 && x < height-1 && field[y+1][x+1].value == 9) c++;
  if(y < height-1 && field[y+1][x].value == 9) c++;
  if(y < height-1 && x > 0 && field[y+1][x-1].value == 9) c++;
  if(x < height-1 && field[y][x+1].value == 9) c++;
  if(x > 0 && field[y][x-1].value == 9) c++;
  if(y > 0 && x < height-1 &&field[y-1][x+1].value == 9) c++;
  if(y > 0 && field[y-1][x].value == 9) c++;
  if(y > 0 && x > 0 && field[y-1][x-1].value == 9) c++;
  return c;
}

int countFlags (int x, int y) {
  int c = 0;
  if(y < height-1 && x < height-1 && field[y+1][x+1].flagged) c++;
  if(y < height-1 && field[y+1][x].flagged) c++;
  if(y < height-1 && x > 0 && field[y+1][x-1].flagged) c++;
  if(x < height-1 && field[y][x+1].flagged) c++;
  if(x > 0 && field[y][x-1].flagged) c++;
  if(y > 0 && x < height-1 &&field[y-1][x+1].flagged) c++;
  if(y > 0 && field[y-1][x].flagged) c++;
  if(y > 0 && x > 0 && field[y-1][x-1].flagged) c++;
  return c;
}

//Uncovering algorithm
void uncover (int x, int y) {
  field[y][x].visible = true;

  if(field[y][x].value == 0) {
    if(!field[y+1][x+1].visible && x < width - 1 && y < height -1) uncover(x+1, y+1);
    if(!field[y][x+1].visible && x < width - 1) uncover (x+1, y);
    if(!field[y-1][x+1].visible && x < width -1 && y > 0) uncover(x+1, y-1);
    if(!field[y+1][x].visible && y < height - 1) uncover(x, y+1);
    if(!field[y-1][x].visible && y > 0) uncover(x, y-1);
    if(!field[y+1][x-1].visible && x > 0 && y < height - 1) uncover(x-1, y+1);
    if(!field[y][x-1].visible && x > 0) uncover(x-1, y);
    if(!field[y-1][x-1].visible && x > 0 && y > 0) uncover(x-1, y-1);
  }
}

int menu (string title, const char *options[], int n) {
  int x = 0;
  while (true) {
    rlutil::cls();
    cout << title << endl;

    for (int i = 0; i < n; ++i) {
      if (x==i) {
        rlutil::setColor(2);
        cout << "> ";
        rlutil::resetColor();
      }
      else cout << "  ";
      cout << options[i] << endl;
    }

    int key = rlutil::getkey();

    if (key == 1) return x;

    else if (key == 0) exit(0);

    else if (key == 14) {
      if (x==0) continue;
      x--;
    }

    else if (key == 15) {
      if (x==n) continue;
      x++;
    }
  }
}

int main () {
  srand (time(NULL));
  rlutil::saveDefaultColor();
  rlutil::hidecursor();

  int bombs, flags;

  //Menu
  const char *opts[3];
  opts[0] = "New Game";
  opts[1] = "Credits";
  opts[2] = "Exit";
  int opt = menu("Minesweeper++", opts, 3);

  if (opt == 2) return 0;
  else if (opt == 1) {
    rlutil::cls();
    cout << "Minesweeper++" << endl;
    cout << "Created by Arisien (https://github.com/Arisien)" << endl;
    cout << "Thanks to tapio for rlutil (https://github.com/tapio/rlutil)" << endl;
    return 0;
  }

  //Difficulty menu
  const char *difs[3];
  difs[0] = "Beginner";
  difs[1] = "Intermediate";
  difs[2] = "Expert";
  int dif = menu("New Game", difs, 3);

  if (dif==0) { //beginner
    height = 9;
    width = 9;
    bombs = 10;
  }

  else if(dif==1) { //intermediate
    height = 16;
    width = 16;
    bombs = 40;
  }

  else if(dif==2) { //expert
    height = 24;
    width = 24;
    bombs = 99;
  }

  flags = bombs;

  //Bomb Generation
  for (int i=0; i<bombs; i++) {
    int x,y;
    do {
      y = rand() % height;
      x = rand() % width;
    } while (field[y][x].value == 9);
    field[y][x].value = 9;
  }

  //Adjacant bomb counting
  for(int i=0; i<height; i++){
    for (int j = 0; j < width; j++) {
      if(field[i][j].value == 9) continue;
      field[i][j].value = countBombs(j, i);
    }
  }

  //Game loop
  int x = 0;
  int y = 0;
  while (true) {
    rlutil::cls();
    bool gameOver = false;
    bool gameWon = true;

    //Field Rendering
    for(int i=0; i<height; i++) {
      for (int j=0; j<width; j++) {
        if(i == y && x == j) printc(i,j, true);
        else printc(i, j, false);
        if(!field[i][j].visible && !field[i][j].flagged) gameWon = false;
        if(field[i][j].visible && field[i][j].value == 9) gameOver = true;
      }
      cout << endl;
    }

    if(gameOver){
      cout << "Game Over!";
      return 0;
    }
    else if (gameWon) {
      cout << "You Win!";
      return 0;
    }

    cout << endl << "Flags: " << flags << endl;

    int key = rlutil::getkey();

    if (key == 0) return 0;

    else if (key == 14) {
      if (y == 0) continue;
      y--;
    }

    else if (key == 15) {
      if (y == height-1) continue;
      y++;
    }

    else if (key == 16) {
      if (x == 0) continue;
      x--;
    }

    else if (key == 17) {
      if (x == width-1) continue;
      x++;
    }

    else if (key == 1 || key == 32) {
      if (field[y][x].flagged) continue;
      if (field[y][x].visible){
        if(countFlags(x, y) != field[y][x].value) continue;
        if(!field[y+1][x+1].visible && !field[y+1][x+1].flagged) uncover(x+1, y+1);
        if(!field[y][x+1].visible && !field[y][x+1].flagged) uncover(x+1, y);
        if(!field[y-1][x+1].visible && !field[y-1][x+1].flagged) uncover(x+1, y-1);
        if(!field[y+1][x].visible && !field[y+1][x].flagged) uncover(x, y+1);
        if(!field[y-1][x].visible && !field[y-1][x].flagged) uncover(x, y-1);
        if(!field[y+1][x-1].visible && !field[y+1][x-1].flagged) uncover(x-1, y+1);
        if(!field[y][x-1].visible && !field[y][x-1].flagged) uncover(x-1, y);
        if(!field[y-1][x-1].visible && !field[y-1][x-1].flagged) uncover(x-1, y-1);
      }
      else uncover(x, y);
    }

    else if (key == 102 || key == 70) {
      if(field[y][x].visible) continue;
      if(!field[y][x].flagged) {
        if(flags == 0) continue;
        field[y][x].flagged = true;
        flags--;
      }
      else {
        field[y][x].flagged = false;
        flags++;
      }
    }

  }

  return 0;
}
