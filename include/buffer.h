#include <string>
#include <vector>

struct LinkedLine {
  LinkedLine *previousLine;
  LinkedLine *nextLine;
  int length;
  int row;
  int maxLength;
  std::string line;
};

class Buffer {
private:
  int point;
  LinkedLine *topLine;
  void shiftLeft();
  void shiftRight();
  void updateRows();
  bool overstrike;
  LinkedLine* makeNewLine();
public:
  LinkedLine *currentLine;
  LinkedLine *infoLine;
  Buffer();
  LinkedLine* getTopLine();
  void newLine();
  void newLoadLine();
  void createBuffer();
  void setPointA(int location, LinkedLine *line);
  void setPointR(int count);
  int getPoint();
  void insert(std::string insertString);
  void insert(char ch);
  void deleteW(int count);
  int searchF(std::string matchString);
  void toggleOverstrike();
};
