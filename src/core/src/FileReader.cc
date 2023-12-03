#include "FileReader.h"

void FileReader::readFile(const string& filename,Parser& parser){
    ifstream ifsfile(filename);
    istringstream issfile(ifsfile);
    string line;
    while(getline(issfile,line)){
        parser.parserline(line);
    }
}