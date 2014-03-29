#include "window.h"

#include <iostream>
#include <fstream>
#include <list>
#include <string>
#include <sstream>
#include <curses.h>

using namespace std;

Window::Window() {
  //  bufferList = NULL; 
  currentBuffer = NULL;
  savedLine = NULL;
  savedPoint = 0;
  searchMode = false;
}

void Window::moveCursor(char ch) {
  int currentPoint = this->currentBuffer->getPoint();
  switch (ch) {
    case 'l':
      this->currentBuffer->setPointA(currentPoint - 1, this->currentBuffer->currentLine);
      break;
    case 'r':
      this->currentBuffer->setPointA(currentPoint + 1, this->currentBuffer->currentLine);
      break;
    case 'u':
      if (this->currentBuffer->currentLine->previousLine != NULL) this->currentBuffer->setPointA(currentPoint, this->currentBuffer->currentLine->previousLine);
      break;
    case 'd':
      if (this->currentBuffer->currentLine->nextLine != NULL) this->currentBuffer->setPointA(currentPoint, this->currentBuffer->currentLine->nextLine);
      break;
    case 'b':
      this->currentBuffer->setPointA(0, this->currentBuffer->currentLine);
      break;
    case 'e':
      this->currentBuffer->setPointA(this->currentBuffer->currentLine->line.length(), this->currentBuffer->currentLine);
      break;
    case 'h':
      this->currentBuffer->currentLine = this->currentBuffer->getTopLine();
      this->currentBuffer->setPointA(0, this->currentBuffer->currentLine);
      break;
    case 'n':
      while (this->currentBuffer->currentLine->nextLine->nextLine) {
        this->currentBuffer->currentLine = this->currentBuffer->currentLine->nextLine;
      }
      this->currentBuffer->setPointA(this->currentBuffer->currentLine->line.length(), this->currentBuffer->currentLine);
      break;
    }
    displayBuffer();
  }

void Window::updateBuffer(char ch) {
  this->currentBuffer->insert(ch);
  displayBuffer();
}

void Window::newBuffer() {
  //cout << "New buffer called";
	Buffer *newBuffer = new Buffer;
	this->bufferList.push_front(newBuffer);
	this->currentBuffer = newBuffer;
  //cout << "Made it base new Buffer";
	this->currentBuffer->createBuffer();
	this->initBuffer();
	//printw("Buffer Created\n");
	refresh();
}

void Window::deleteBuffer(Buffer *buffer) {
	if (buffer) delete buffer;
}

void Window::saveBuffer(std::string filename) {

}

void Window::initBuffer() {
  if (this->currentBuffer) {
    initscr();
    clear();
    noecho();
    cbreak();
    move(0,0);
  }	
}

// Need to rework this so it only displays from the current point onward
void Window::displayBuffer() {
  if (this->currentBuffer) {
    clear();
    LinkedLine *topLine = this->currentBuffer->getTopLine();
    while(topLine != NULL) {
      string str = topLine->line + "\n";
      printw(str.c_str());
      topLine = topLine->nextLine;
    }
  }

  // Setup and print column/row line
  if (!this->searchMode) {
   string col, row;
   stringstream out, out1;
   out << this->currentBuffer->getPoint() + 1;
   col = out.str();
   //cout << col;
   out1 << this->currentBuffer->currentLine->row + 1;
   row = out1.str();
   string bufferBase = "column: " + col + "   row: " + row;
   this->currentBuffer->infoLine->line = bufferBase;
   mvprintw(24, 0, this->currentBuffer->infoLine->line.c_str());
} else {
  mvprintw(24, 0,this->currentBuffer->infoLine->line.c_str());
  this->currentBuffer->setPointA(this->currentBuffer->infoLine->line.length(), this->currentBuffer->infoLine);
}

  refresh();
  move(this->currentBuffer->currentLine->row, this->currentBuffer->getPoint());
}

void Window::loadBuffer() {
  this->currentBuffer->currentLine = this->currentBuffer->getTopLine();
}

void Window::loadBuffer(std::string filename) {
	string fileLine = "";
	if (this->currentBuffer) {
	  ifstream inf(filename.c_str());
	  while (getline(inf, fileLine)) {
	    this->currentBuffer->insert(fileLine);
      this->currentBuffer->currentLine->length = fileLine.length();
	    this->currentBuffer->newLoadLine();
	  }
    this->currentBuffer->currentLine = this->currentBuffer->getTopLine();
	  //displayBuffer();
	}
}

void Window::deleteW(int direction) {
    if (direction == -1 || direction == 1) this->currentBuffer->deleteW(direction);
    displayBuffer();
  }

void Window::newLine() {
  this->currentBuffer->newLine();
  this->currentBuffer->setPointA(0, this->currentBuffer->currentLine);
  displayBuffer();
}

void Window::exitSearch() {
  this->searchMode = false;
  this->currentBuffer->setPointA(this->savedPoint, this->savedLine);
  displayBuffer();
}

void Window::searchBuffer() {
  this->searchMode = false;
  int found = this->currentBuffer->searchF(this->currentBuffer->infoLine->line.substr(15, this->currentBuffer->infoLine->line.length()));
  if (found > 0)  {
    this->currentBuffer->setPointA(found, this->currentBuffer->currentLine);
  } else {
    this->currentBuffer->setPointA(this->savedPoint, this->savedLine);
  }
  displayBuffer();
}

void Window::setSearchBuffer() {
  this->searchMode = true;
  this->savedLine = this->currentBuffer->currentLine;
  this->savedPoint = this->currentBuffer->getPoint();

  string searchString = "Insert String: ";
  this->currentBuffer->infoLine->line = searchString;
  this->currentBuffer->currentLine = this->currentBuffer->infoLine;
  displayBuffer();
}

void Window::setOverstrike() {
  this->currentBuffer->toggleOverstrike();
}
