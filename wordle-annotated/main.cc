#include <iostream>
#include <fstream>

#include <random>

#include <vector>
#include <unordered_set>

#include <ranges>
#include <algorithm>

#include "word.hh"
#include "secret_word.hh"

#define MAX_GUESSES 6
#define WORD_LENGTH 5

#define ANSWERS_FILE "../answers.txt"
#define VALID_GUESSES_FILE "../valid_guesses.txt"

/* the validateGuess function takes three parameters:
 * 1. the guess to validate (s)
 * 2. the length any guess should be (properLength)
 * 3. an unordered_set of valid guesses (validGuesses)
 *
 * the function checks that the guess is:
 * a) the proper length (ref: 1)
 * b) all letters (ref: 2)
 * c) and in the list of valid guesses (ref: 3)
 *
 * it returns a reference code for each of these issues instead of printing to the user to keep all printing in the main loop
 * if the function finds no issues, it returns a reference code of 0 */

int
validateGuess(std::string s, int properLength, std::unordered_set<std::string> validGuesses) {
	if(s.length() != properLength) {
		return 1;
	}
	for(char c : s) {
		if(!std::isalpha(c)) {
			return 2;
		}
	}
	if(!validGuesses.contains(s)) {
		return 3;
	}

	return 0;
}

/* the highlight function takes three parameters:
 * 1. the string to format (s)
 * 2. a set containing the yellow positions (yellows)
 * 3. a set containing the green positions (greens)
 *
 * to maintain the indices while formatting, s is broken up by character into a vector of strings (stringByCharacter).
 * the function then prepends the correct color code and appends a reset code to each string and
 * finally, it combines each value of that vector back into a string and returns it */

std::string
highlight(std::string s, std::set<int> yellows, std::set<int> greens) {
	std::vector<std::string> stringByCharacter;

	// breaking s into a vector, we can ensure the given indices always point to the correct character
	for(int i = 0; i < s.length(); i++) {
		stringByCharacter.push_back(s.substr(i, 1));
	}

	for(int i : yellows) {
		stringByCharacter[i] = "\x1B[7;33m" + stringByCharacter[i] + "\x1B[27;39m";
	}

	for(int i : greens) {
		stringByCharacter[i] = "\x1B[7;32m" + stringByCharacter[i] + "\x1B[27;39m";
	}

	std::string highlighted = stringByCharacter
	                        | std::ranges::views::join
	                        | std::ranges::to<std::string>();

	return highlighted;
}

int
main() {
	// there are more valid guesses than actual answers in wordle, so each is kept in a separate file

	// create a vector to store the answers
	// vectors are indexed, so they're easier to pick a random value from
	std::vector<std::string> answers;

	// open answers.txt and check if the file exists
	std::ifstream answersFile(ANSWERS_FILE);
	if(!answersFile) {
		std::clog << "No " << ANSWERS_FILE << "  file found! Exiting." << std::endl;
		return -1;
	}

	// add each word in answers.txt to the vector 'answers'
	std::string line;
	while(std::getline(answersFile, line)) {
		answers.push_back(line);
	}

	// create an unordered set to store the valid guesses
	// it is much faster to check if a word is in an unordered set than a vector
	std::unordered_set<std::string> validGuesses;

	// open valid_guesses.txt and check if the file exists
	std::ifstream validGuessesFile(VALID_GUESSES_FILE);
	if(!validGuessesFile) {
		std::clog << "No " << VALID_GUESSES_FILE << " file found! Exiting." << std::endl;
		return -1;
	}

	// add each word in valid_guesses.txt to the unordered set 'validGuesses'
	line.clear();
	while(std::getline(validGuessesFile, line)) {
		validGuesses.insert(line);
	}

	/* set up the random number generator to pick indices within the bounds of the vector 'answers'
	 * each answer has a corresponding index, so by picking an index we're really picking a word */
	std::random_device rd;
	std::uniform_int_distribution<int> range(0, answers.size());

	/* the game's loop will only run if the user wants to play
	 * we keep track of that using the below variable
	 * we can assume they want to play because they just opened the program */
	bool wantsToPlay = true;
	while(wantsToPlay) {
		// make sure the user knows how to play the game
		std::cout << "You have " << MAX_GUESSES << " chances to guess a " << WORD_LENGTH << "-letter word." << std::endl;

		// assign a random word as the secret word
		SecretWord sw(answers[range(rd)]);

		// set up the guess counter and keep track of whether the secret word has been guessed
		int guessesLeft	= MAX_GUESSES;
		bool guessed	= false;
		while(!guessed and guessesLeft > 0) { // main game loop
			std::string guessString; // set up a buffer string to store the user's inputted guess

			// get a guess and check if it's valid
			bool guessIsValid = false;
			while(!guessIsValid) {
				// get a guess
				std::getline(std::cin, guessString);

				// convert to lowercase
				guessString = guessString
				            | std::ranges::views::transform([](char c){ return std::tolower(c); })
				            | std::ranges::to<std::string>();

				// check if its valid
				switch(validateGuess(guessString, WORD_LENGTH, validGuesses)) {
					case 1: { // it's not the right length
						std::cout << "Your word must be exactly " << WORD_LENGTH << " characters!" << std::endl;
						break;
					}
					case 2: { // it's got non-letters
						std::cout << "Your guess can only contain letters!" << std::endl;
						break;
					}
					case 3: { // it isn't in the valid guess word list
						std::cout << "Your guess is not in the word list!" << std::endl;
						break;
					}
					default: {
						guessIsValid = true;
						break;
					}
				}
			}

			/* since we know the guess is valid, we can make it a 'Word'.
			 * 'Word's have special functions to identify the greens and yellows */
			Word guess(guessString);

			// get the positions of the yellows (right letter, wrong position) and greens (right letter, right position)
			std::set<int> yellows = sw.getYellowsWith(guess);
			std::set<int> greens = sw.getGreensWith(guess);

			// highlight the guess with the positions acquired above
			std::string highlighted = highlight(guess.getString(), yellows, greens);

			// output the highlighted guess
			std::cout << "\033[1A\x1b[2K\033[1m" << highlighted << "\033[0m" << std::endl;

			// check if the word was guessed and remove 1 from the number of guesses left
			guessed = (sw.getString() == guess.getString());
			guessesLeft--;
		}

		// the program only exits the above loop if the word is guessed or the user runs out of guesses

		if(guessed) { // congratulate the user if they got the word
			std::cout << "Congrats. You got it!" << std::endl;
		}
		else { // tell the user what the word was if they didn't get it
			std::cout << "The word was \"" << sw.getString() << ".\" Better luck next time!" << std::endl;
		}

		// ask the user if they want to play again
		std::cout << "Want to play again? (Type \"yes\" if you do.):" << std::endl;

		std::string playAgainInput;
		std::getline(std::cin, playAgainInput);

		// convert to lowercase
		playAgainInput = playAgainInput
		               | std::ranges::views::transform([](char c) { return std::tolower(c); })
		               | std::ranges::to<std::string>();

		wantsToPlay = (playAgainInput == "yes");

		if(wantsToPlay) {
			std::cout << "Great. ";
		}
		else {
			std::cout << "Thanks for playing. See you next time!" << std::endl;
		}
	}
}
