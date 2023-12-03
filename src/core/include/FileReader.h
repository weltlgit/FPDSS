#include <string>
#include <sstream>
#include <fstream>
#include "Parser.h"

using std::string;
using std::istringstream;
using std::ifstream;

class FileReader {
    public:
    void readFile(const string& filename,Parser& parser){
        ifstream ifsFile(filename);
        string line;
        while(getline(ifsFile,line)){
            parser.parserline(line);
        }
    }
}