#include <list>
#include <string>
#include <curses.h>

#include "buffer.h"

class Window {
private:
  std::list<Buffer*> bufferList;
  Buffer *currentBuffer;
  LinkedLine *savedLine;
  int savedPoint;
public:
  bool searchMode;
  Window();
  void newBuffer();
  void moveCursor(char ch);
  void updateBuffer(char ch);
  void deleteBuffer(Buffer *buffer);
  void saveBuffer(std::string filename);
  void loadBuffer(std::string filename);
  void loadBuffer();
  void displayBuffer();
  void initBuffer();
  void deleteW(int direction);
  void newLine();
  void searchBuffer();
  void setSearchBuffer();
  void exitSearch();
  void setOverstrike();
};
