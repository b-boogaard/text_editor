#include "window.h"
#include <iostream>

using namespace std;

int main(int argc, char* argv[]) {
  Window *newWindow = new Window();

  newWindow->newBuffer();
  if (argc == 2) {
    newWindow->loadBuffer(argv[1]);
  } else {
    newWindow->loadBuffer();
  } 
  move(0,0);
  newWindow->displayBuffer();
  move(0,0);

  nonl();
  intrflush(stdscr, FALSE);
  keypad(stdscr, TRUE);
  
  int ch;
  bool running = true;

  while(running) {
    ch = getch();
    
    if (ch >= 32 && ch <= 126) {
      //isascii(ch);
      char character = ch;
      //cout << character;
      newWindow->updateBuffer(character);
    } else {

      switch(ch) {
      case KEY_LEFT:
	     newWindow->moveCursor('l');
	     break;
      case KEY_RIGHT:
	     newWindow->moveCursor('r');
       break;
      case KEY_UP:
        newWindow->moveCursor('u');
      	break;
      case KEY_DOWN:
        newWindow->moveCursor('d');
      	break;
      case 127: // delete character
        newWindow->deleteW(-1);
        break;
      case 13: // enter pressed
        if (!newWindow->searchMode) {
        newWindow->newLine();
      } else {
        newWindow->searchBuffer();
      }
        break;
      case 1: // ctrl-a
         newWindow->moveCursor('b');
         break;
      case 5: // ctrl-e
        newWindow->moveCursor('e');
        break;
      case 263: // ctrl-h
        newWindow->moveCursor('h');
        break;
      case 14: // ctrl-n
        newWindow->moveCursor('n');
        break;
      case 6: // ctrl-f
        newWindow->searchMode = true;
        newWindow->setSearchBuffer();
        break;
      case 27: // escape
        newWindow->exitSearch();
        break;
      case 20: // ctrl-t
        newWindow->setOverstrike();
        break;
      case 2: // ctrl-b
        newWindow->deleteW(1);
        break;
      default:
     // cout << ch;
	//printw("default happened\n");
	break;
      }
    }
    refresh();
  }
  
  return 0;
}
