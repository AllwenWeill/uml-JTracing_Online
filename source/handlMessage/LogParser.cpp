#include "LogParser.h"
LogParser::LogParser(){

}

LogParser::~LogParser() {

}

void LogParser::addnote(string parserStr) {
	parserNotes.push_back(parserStr);
}