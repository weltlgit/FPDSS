#include <string>
#include <vector>
#include <iostream>

using std::cout;
using std::endl;
using std::string;
using std::vector;

class Parser {
  virtual ~Parser(){};
  virtual void parserline(const string &line) = 0;
};

class FrequencyParser : public Parser {
private:
  string targetWord;
  size_t count;

public:
  FrequencyParser(const string &word);
  virtual void parserline(const string &line) override;
  size_t getCount();
  virtual ~FrequencyParser(){};
};

class KeywordParser : public Parser {
private:
  string keyword;
  vector<string> contextLines;
public:
    KeywordParser(const string &word);
    virtual void parserline(const string &line) override;
    const vector<string> getContextLines();
};