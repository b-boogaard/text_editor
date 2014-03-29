#include <string>

class StringMatcher
{
 private:
  std::string text;
  std::string pattern;
  int after_star_match;
  int after_star;
  int cur_match;
  int curtext_pos;
  int curpattern_pos;
  


  void inc_tp();
  void inc_t();
  void inc_p();
  void set_match();
  
 public:
  StringMatcher(std::string text_, std::string pattern_);
  
  int find_match();
};


