#pragma once

#include <string>

#include <unordered_map>
#include <set>

#include <ranges>
#include <algorithm>

class Word {
	private:
		std::string w; // word
		std::unordered_map<char, std::set<int>> letterIndexMap;
		int l; // length

	public:
		Word(std::string w);
		std::string getString();
		int getLength();
		std::set<char> getLetters();
		std::set<int> indicesOf(char c);
};
