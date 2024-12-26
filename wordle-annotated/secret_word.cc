#include "secret_word.hh"

// construct a SecretWord object
SecretWord::SecretWord(std::string w) : Word(w) {
	// since SecretWord inherits everything from Word, and the latter adds no new parameters, we don't need to do anything here other than call the parent constructor.
}

/* get a set of the letters which this SecretWord shares with another Word
 * since we have the letters in a set, we can simply take the intersection of the two */
std::set<char>
SecretWord::getSharedLettersWith(Word guess) {
	std::set<char> sharedLetters;
	std::ranges::set_intersection(this->getLetters(), guess.getLetters(), std::inserter(sharedLetters, sharedLetters.end()));

	return sharedLetters;
}

/* get a set of the positions where the given guess matches this SecretWord's characters in both letter and position
 * this algorithm is pretty simple. all we have to do is iterate through each letter and take the intersection of the sets of its positions in this SecretWord and the guess Word */
std::set<int>
SecretWord::getGreensWith(Word guess) {
	std::set<char> sharedLetters = this->getSharedLettersWith(guess);

	// for each letter, find shared positions and add them to our set
	std::set<int> greenIndices;
	for(char c : sharedLetters) {
		std::ranges::set_intersection(this->indicesOf(c), guess.indicesOf(c), std::inserter(greenIndices, greenIndices.end()));
	}

	return greenIndices;
}

/* get a set of the positions where the given guess's characters match characters in this SecretWord but are in the wrong position.
 * this algorithm is more complicated. it still iterates through every character, though. see below for more explanation. */
std::set<int>
SecretWord::getYellowsWith(Word guess) {
	std::set<char> sharedLetters = this->getSharedLettersWith(guess);

	std::set<int> yellowIndices;
	for(char c : sharedLetters) {
		/* wordle requires that the yellows per letter not exceed its unshared occurrences in the secret word
		 * so we need to:
		 * 1. take the difference of the indices of each shared letter in the guess with the secret word, finding all unshared occurrences of each shared letter in the guess
		 * 2. take the difference of the indices of each shared letter in the secret word with the guess, finding all unshared occurrences of each shared letter in the secret word
		 * 3. get the size of both differences, to ensure we meet the condition above
		 * 4. copy a certain amount of the values of the set obtained in (1) according to the minimum size of the two sets to the set keeping track of yellow indices
		 *
		 * for example:
		 * - our SecretWord is "aaabb"
		 * - you guess "bbbcc"
		 * - if we only did (1), we would highlight the all three 'b's, when we really only want to count the first two
		 * - by also doing (2) and (3), we can determine how many 'b's we should highlight */

		std::set<int> indicesInSecretWord	= this->indicesOf(c);
		std::set<int> indicesInGuess		= guess.indicesOf(c);

		std::set<int> notSharedWithSecretWord;
		std::set<int> notSharedWithGuess;

		std::ranges::set_difference(indicesInGuess, indicesInSecretWord, std::inserter(notSharedWithSecretWord, notSharedWithSecretWord.end()));
		std::ranges::set_difference(indicesInSecretWord, indicesInGuess, std::inserter(notSharedWithGuess, notSharedWithGuess.end()));

		int minOccurrencesOfC = std::min(notSharedWithSecretWord.size(), notSharedWithGuess.size());

		yellowIndices.insert(notSharedWithSecretWord.begin(), std::ranges::next(notSharedWithSecretWord.begin(), minOccurrencesOfC));
	}

	return yellowIndices;
}
