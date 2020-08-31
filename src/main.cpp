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

//Prints helpful coordinates
void printh (int i) {
  rlutil::setColor(3);
  cout << i%10 << " ";
  rlutil::resetColor();
}

//Prints normal points
void printc (int i, int j) {
  if(field[i][j].flagged) cout << "F";
  else if (!field[i][j].visible) cout << "*";
  else if (field[i][j].value == 9) cout << "B";
  else cout << field[i][j].value;
  cout << " ";
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

int main () {
  srand (time(NULL));
  rlutil::saveDefaultColor();

  int bombs, flags;


  //Menu
  while (true) {
    rlutil::cls();
    cout << "Minesweeper++" << endl;
    cout << "Enter difficulty (beginner/intermediate/expert): ";
    string dif;
    cin >> dif;

    if (dif == "b" || dif == "beginner") {
      height = 9;
      width = 9;
      bombs = 10;
      break;
    }

    else if(dif == "i" || dif == "intermediate") {
      height = 16;
      width = 16;
      bombs = 40;
      break;
    }

    else if(dif == "e" || dif == "expert") {
      height = 24;
      width = 24;
      bombs = 99;
      break;
    }
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
      int c = 0;
      if(field[i+1][j+1].value == 9) c++;
      if(field[i+1][j].value == 9) c++;
      if(field[i+1][j-1].value == 9) c++;
      if(field[i][j+1].value == 9) c++;
      if(field[i][j-1].value == 9) c++;
      if(field[i-1][j+1].value == 9) c++;
      if(field[i-1][j].value == 9) c++;
      if(field[i-1][j-1].value == 9) c++;
      field[i][j].value = c;
    }
  }

  //Game loop
  while (true) {
    rlutil::cls();
    bool gameOver = false;
    bool gameWon = true;

    //Field Rendering
    for(int i=0; i<height; i++) {
      if (i == 0) {
        cout << "M ";
        for(int j=0; j<width; j++) printh(j+1);
        cout << endl;
      }

      printh(i+1);

      for (int j=0; j<width; j++) {
        printc(i,j);
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

    string cmd;
    cout << endl << "$ ";
    cin >> cmd;

    if (cmd == "plot") {
      int x, y;
      cin >> x;
      cin >> y;
      x--;
      y--;
      if (field[y][x].flagged) continue;
      //For already uncovered points, all adjacant points are uncovered
      //To be added: safety feature if N amount of flags havent been placed adjacently yet
      if (field[y][x].visible){
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

    else if (cmd == "flag") {
      int x, y;
      cin >> x;
      cin >> y;
      if(field[y-1][x-1].visible) continue;
      if(!field[y-1][x-1].flagged) {
        if(flags == 0) continue;
        field[y-1][x-1].flagged = true;
        flags--;
      }
      else {
        field[y-1][x-1].flagged = false;
        flags++;
      }
    }

    else if (cmd == "quit") {
      return 0;
    }

  }

  return 0;
}
