// Brian van de Boogaard
// Algorithms - String matching with * and ?

#include "string_find.h"
#include <string>
#include <iostream>

using namespace std;

StringMatcher::StringMatcher(std::string text_, std::string pattern_)
  : text(text_), pattern(pattern_)
{
  after_star_match = 0;
  after_star = 0;
  cur_match = 0;
  curtext_pos = 0;
  curpattern_pos = 0;
}

void StringMatcher::inc_tp() {
  ++this->curtext_pos;
  ++this->curpattern_pos;
}

void StringMatcher::inc_t() {
  ++this->curtext_pos;
}

void StringMatcher::inc_p() {
  ++this->curpattern_pos;
}
void StringMatcher::set_match() {
  //  cout << "Set Match Called" << endl;
  if (this->cur_match == 0) {
    this->cur_match = this->curtext_pos;
    // cout << "In Set Match this->cur_match: " << this->cur_match << endl;
  }
}

int StringMatcher::find_match() {
 
  if (this->text.length() > 0 && 
      this->pattern.length() > 0 &&
      this->text.length() > (unsigned int)this->curtext_pos) {
 
    if ((unsigned int)this->curpattern_pos == this->pattern.length()) {
      return this->cur_match;    } else if (this->pattern.at(this->curpattern_pos) == '*') {
      //cout << "* Found" << endl;
      this->after_star_match = 0;
      this->set_match();
      this->after_star = 1;
      this->inc_p();
    } else if (this->pattern.at(this->curpattern_pos) == '?') {
      //cout << "? Found" << endl;
	this->set_match();
	this->inc_tp();
      } else if ((this->text.at(this->curtext_pos)) == (this->pattern.at(this->curpattern_pos)) && 
	  this->after_star == 1) {
      //cout << "Match with after_star found" << endl;
	this->after_star_match = 1;
	this->inc_t();
      } else if ((this->text.at(this->curtext_pos)) == (this->pattern.at(this->curpattern_pos))) {
      //cout << "Exact match found" << endl;
	this->set_match();
	this->after_star_match = 0;
	this->after_star = 0;
	this->inc_tp();
      } else if (this->text.at(this->curtext_pos) != this->pattern.at(this->curpattern_pos) && 
	  this->after_star == 1) {
      //cout << "Not match with after_star found" << endl;
	if (this->after_star_match == 1) {
	  this->after_star_match = 0;
	  this->after_star = 0;
	  this->inc_p();
	} else {
	  this->inc_t();
	}
      } else if (this->text.at(this->curtext_pos) != this->pattern.at(this->curpattern_pos)) {
      //cout << "Not match found" << endl;
	this->cur_match = 0;
	this->inc_t();
	this->curpattern_pos = 0;
      }
    this->find_match();
  } else if ((unsigned int)this->curtext_pos == this->text.length() &&
	     (unsigned int)this->curpattern_pos != this->pattern.length()) {
    this->cur_match = 0;
  }
  return this->cur_match;
}
