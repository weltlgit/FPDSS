#include "Parser.h"

FrequencyParser::FrequencyParser(const string &word) : targetWord(word) {}

void FrequencyParser::parserline(const string &line) {
  size_t pos = line.find(this->targetWord);
  if ((pos == 0 || !isalpha(line[pos - 1])) &&
      (pos + targetWord.length() == line.length() ||
       !isalpha(line[pos + targetWord.length()]))) {
    count++;
  }
}

size_t FrequencyParser::getCount() { return this->count; }

KeywordParser::KeywordParser(const string &word) : keyword(word) {}

void KeywordParser::parserline(const string &line) {}

const vector<string> KeywordParser::getContextLines() {}
