#pragma once

#include "word.hh"

#include <vector>
#include <algorithm>
#include <iterator>

class SecretWord: public Word {
	private:
		std::set<char> getSharedLettersWith(Word guess);
	public:
		SecretWord(std::string w);
		std::set<int> getGreensWith(Word guess);
		std::set<int> getYellowsWith(Word guess);
};
