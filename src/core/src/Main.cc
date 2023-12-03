#include "FileReader.h"
#include "Parser.h"
#include "ThreadPool.hpp"
#include <future>
#include <iostream>
#include <string>
#include <vector>

using std::cerr;
using std::cin;
using std::cout;
using std::endl;
using std::future;
using std::string;
using std::vector;

int main(int argc, char *argv[]) {
  if (argc < 2) {
    cerr << "Usage" << argv[0] << " <file1> [file2] [file3] [file4]" << endl;
  }
  string service;
  string word;
  vector<string> filenames;
  for (int i = 1; i < argc; ++i) {
    filenames.push_back(argv[i]);
  }
  cout << "选择您要的服务:词频查询or关键词查询(1or2)" << endl;
  cin >> service;
  if (service == "1") {
    cout << "请输入单词:" << endl;
    cin >> word;

    ThreadPool pool(4);
    vector<future<void>> results;
    FrequencyParser parser(word);
    for (const auto &filename : filenames) {
      results.emplace_back(pool.enqueue([&parser, &filename]() {
        FileReader reader;
        reader.readFile(filename, parser);
      }));
    }

    for (auto &result : results) {
      result.get();
    }
    std::cout << "The word '" << word << "' appears " << parser.getCount()
              << " times in total." << std::endl;
  }
}