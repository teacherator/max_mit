#include <iostream>
#include <fstream>

#include <random>

#include <vector>
#include <unordered_set>

#include <ranges>
#include <algorithm>

#include "word.hh"

#define MAX_GUESSES 6
#define WORD_LENGTH 1
#define WORDS_FILE "../words.txt"

int
validateGuess(std::string s, int properLength, std::unordered_set<char> guessedLetters) {
	if(s.length() != properLength) {
		return 1;
	}
	for(char c : s) {
		if(!std::isalpha(c)) {
			return 2;
		}
	}
	if(guessedLetters.contains(s[0])) {
		return 3;
	}

	return 0;
}

int
main() {
	std::vector<std::string> words;
	std::ifstream wordsFile(WORDS_FILE);

	if(!wordsFile) {
		std::clog << "No " << WORDS_FILE << " file found! Exiting." << std::endl;
		return -1;
	}

	std::string line;
	while(std::getline(wordsFile, line)) {
		words.push_back(line);
	}

	std::random_device rd;
	std::uniform_int_distribution<int> range(0, words.size());

	bool wantsToPlay = true;
	while(wantsToPlay) {
		Word w(words[range(rd)]);
		std::string workingString(w.getLength(), '_');

		std::unordered_set<char> guessedLetters;

		int guessesLeft	= MAX_GUESSES;
		bool guessed	= false;
		while(!guessed and guessesLeft > 0) {
			std::string guessString;
			char guess;

			std::cout << workingString << "  (" << guessesLeft << " " << ((guessesLeft != 1) ? "guesses" : "guess") << " remaining.)" << std::endl;

			bool guessIsValid = false;
			while(!guessIsValid) {
				std::getline(std::cin, guessString);

				// convert to lowercase
				guessString = guessString
				            | std::ranges::views::transform([](char c){ return std::tolower(c); })
				            | std::ranges::to<std::string>();

				switch(validateGuess(guessString, WORD_LENGTH, guessedLetters)) {
					case 1: {
						std::cout << "Your can only guess single characters!" << std::endl;
						break;
					}
					case 2: {
						std::cout << "You can only guess letters!" << std::endl;
						break;
					}
					case 3: {
						std::cout << "You already guessed that letter!" << std::endl;
						break;
					}
					default: {
						guessIsValid = true;
						break;
					}
				}
			}

			guess = guessString[0];
			std::set<int> indicesOfGuess = w.indicesOf(guess);

			if(!indicesOfGuess.empty()) {
				for(int i : w.indicesOf(guess)) {
					workingString[i] = guess;
				}
			}
			else {
				guessesLeft--;
			}

			guessedLetters.insert(guess);

			guessed = (w.getString() == workingString);
		}

		if(guessed) {
			std::cout << workingString << std::endl;
			std::cout << "Congrats. You got it!" << std::endl;
		}
		else {
			std::cout << "The word was \"" << w.getString() << ".\" Better luck next time!" << std::endl;
		}

		std::cout << "Want to play again? (Type \"yes\" if you do.):" << std::endl;

		std::string playAgainInput;
		std::getline(std::cin, playAgainInput);

		// convert to lowercase
		playAgainInput = playAgainInput
		               | std::ranges::views::transform([](char c) { return std::tolower(c); })
		               | std::ranges::to<std::string>();

		wantsToPlay = (playAgainInput == "yes");
	}
}
