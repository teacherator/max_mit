#include "word.hh"

Word::Word(std::string w) {
	this->w	= w;
	this->l	= w.length();

	// add the indices where each character occurs to letterIndexMap
	for(int i = 0; i < this->l; i++) {
		this->letterIndexMap[w[i]].insert(i);
	}
}

std::string
Word::getString() {
	return this->w;
}

int
Word::getLength() {
	return this->l;
}

std::set<char>
Word::getLetters() {
	std::set<char> letters = this->letterIndexMap
	                       | std::ranges::views::keys
	                       | std::ranges::to<std::set>();

	return letters;
}

std::set<int>
Word::indicesOf(char c) {
	return letterIndexMap[c];
}
