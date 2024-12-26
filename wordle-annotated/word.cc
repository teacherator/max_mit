#include "word.hh"

// create a word object
Word::Word(std::string w) {
	this->w	= w; // since this is often necessary, its better to keep it as a string instead of constantly reconstructing it from letterIndexMap
	this->l	= w.length(); // also useful to cache

	/* by breaking a word into a map between its letters and their positions within it, it becomes much easier to check certain properties of the word
	 * the below adds the indices where each character occurs to letterIndexMap */
	for(int i = 0; i < this->l; i++) {
		this->letterIndexMap[w[i]].insert(i);
	}
}

// get the word as a string
std::string
Word::getString() {
	return this->w;
}

// get the length of the string
int
Word::getLength() {
	return this->l;
}

// get the letters of the word from the keys of letterIndexMap
std::set<char>
Word::getLetters() {
	std::set<char> letters = this->letterIndexMap
	                       | std::ranges::views::keys
	                       | std::ranges::to<std::set>();

	return letters;
}

// get the indices where a particular letter occurs within the word
std::set<int>
Word::indicesOf(char c) {
	return letterIndexMap[c];
}
