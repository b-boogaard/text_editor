#include "buffer.h"
#include "string_find.h"

#include <curses.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

Buffer::Buffer() {
  
  LinkedLine *newLine;
  newLine = new LinkedLine;
  newLine->previousLine = NULL;
  newLine->nextLine = NULL;
  newLine->length = 0;
  newLine->row = 0;
  newLine->maxLength = 80;
  newLine->line = "";
  this->currentLine = newLine;
 
  point = 0;
  infoLine = NULL;
  topLine = NULL;
  overstrike = false;
  
}

LinkedLine* Buffer::makeNewLine() {
  LinkedLine *newLine;
  newLine = new LinkedLine;
  newLine->previousLine = NULL;
  newLine->nextLine = NULL;
  newLine->length = 0;
  newLine->row = this->currentLine->row + 1;
  newLine->maxLength = 80;
  newLine->line = "";

  return newLine;
}

void Buffer::createBuffer() {
  // create a buffer
  this->topLine = currentLine;
  this->infoLine = makeNewLine();
  this->infoLine->row = 24;
}

void Buffer::setPointA(int location, LinkedLine *line) {
  if (line != NULL) this->currentLine = line;
  (location >= 0) ? this->point = location : this->point = 0;
  
  move(this->currentLine->row, location);
}

void Buffer::setPointR(int count) {
	this->point += count;
}

int Buffer::getPoint() {
	return this->point;
}

void Buffer::insert(std::string insertString) {
  // Insert string
  int stringLength = insertString.length();
  if (stringLength > 0 && this->currentLine->length == 0) {
    this->currentLine->line = insertString;
    this->currentLine->length = stringLength;
  } else if (stringLength > 0) {
    for(int i = stringLength - 1; i >= 0; --i) {
      if (!this->overstrike) {
      this->currentLine->line.insert(this->point, 1, insertString.at(i));
      //this->setPointR(1);
    } else {
      this->currentLine->line.replace(this->point, 1, 1, insertString.at(i));
      //this->setPointR(1);
    }
      this->currentLine->length++;
    }
  }
}

void Buffer::insert(char ch) {
  if (this->point < this->currentLine->maxLength) 
    {
      if (!this->overstrike) {
        this->currentLine->line.insert(this->point, 1, ch);
        this->setPointR(1);
      } else if (this->overstrike) {
        this->currentLine->line.replace(this->point, 1, 1, ch);
        this->setPointR(1);
      }
    } else if (this->currentLine->line.length() >= (unsigned int)(this->currentLine->maxLength - 1)) {
      this->currentLine->line.replace(this->point, 1, 1, ch);
    }
  printw(currentLine->line.c_str());
  refresh();
}

void Buffer::deleteW(int count) {
  if (count == -1) {
    if (this->point > 0) 
      {
        this->currentLine->line.erase((this->point - 1), 1);
        this->point += count;
       } else if (this->point == 0 && this->currentLine->row == 0) { // Do nothing
       } else if (this->point == 0) {
          string str = this->currentLine->line;

          // Update links
          LinkedLine *temp = this->currentLine->nextLine;
          this->currentLine = this->currentLine->previousLine;
          this->currentLine->nextLine = temp->nextLine;
          delete temp;

          // update new current line and modify point
          this->point = this->currentLine->line.length();
          this->insert(str);
          updateRows();
          //this->currentLine->line.insert(this->currentLine->line.length(), str.c_str());
       }
  } else if (count == 1) {

    // if we delete at the end of a line, roll up the next line
    if (((unsigned int)this->point - this->currentLine->line.length()) <= 0) {
      // Get string
      string str = this->currentLine->nextLine->line;
      
      // Update links
      LinkedLine *temp = this->currentLine->nextLine;
      this->currentLine->nextLine = temp->nextLine;
      delete temp;

      // update line and rows
      this->insert(str);
      updateRows();
    } else {
      this->currentLine->line.erase(this->point, 1);
    }
  }
}

int Buffer::searchF(std::string matchString) {
  this->currentLine = this->topLine;
  int loc = 0;
  while (this->currentLine != NULL) {
    StringMatcher sm(this->currentLine->line, matchString);
    loc = sm.find_match();
    if (loc > 0) return loc + matchString.length();
    this->currentLine = this->currentLine->nextLine;
  }
  return 0; // no matches

}

void Buffer::newLoadLine() {
  LinkedLine *newLine = this->makeNewLine();
  newLine->previousLine = this->currentLine;
  this->currentLine->nextLine = newLine;
  this->currentLine = newLine;
}

void Buffer::updateRows() {
  LinkedLine *placeHolder = this->currentLine;
  int row = this->currentLine->row;
  this->currentLine = this->currentLine->nextLine;
  while (this->currentLine) {
    this->currentLine->row = ++row;
    this->currentLine = this->currentLine->nextLine;
  }
  this->currentLine = placeHolder;
}

void Buffer::newLine() {
  LinkedLine *newLine = this->makeNewLine();

   if ((unsigned int)this->point < this->currentLine->line.length()) {
    int length = (this->currentLine->line.length() - (unsigned int)this->point);
    string str = this->currentLine->line.substr(this->point, length);
    this->currentLine->line.erase(this->point, length);
    newLine->line = str;
    newLine->nextLine = this->currentLine->nextLine;
    //updateRows();
  }
  
  // Update pointers
   newLine->previousLine = this->currentLine;
   this->currentLine->nextLine = newLine;
   this->currentLine = newLine;
   updateRows();
}

LinkedLine* Buffer::getTopLine() {
  return this->topLine;
}

void Buffer::toggleOverstrike() {
  if (this->overstrike) { 
    this->overstrike = false;
   // printw("Overstrike off");
  } else {
   this->overstrike = true;
   //printw("Overstring on");
 }
}
