#include <algorithm>
#include <iostream>
#include <cstring>
#include <string>
#include <format>
#include <tuple>
#include <array>
#include <span>

#include "TerminalColor.hpp"
#include "Hackdle.hpp"

#define MT std::make_tuple
std::array<std::tuple<char, bool>, 26> Hackdle::letters_guessed = {
	MT('a', false), MT('b', false), MT('c', false), MT('d', false), MT('e', false), MT('f', false),
	MT('g', false), MT('h', false), MT('i', false), MT('j', false), MT('k', false), MT('l', false),
	MT('m', false), MT('n', false), MT('o', false), MT('p', false), MT('q', false), MT('r', false),
	MT('s', false), MT('t', false), MT('u', false), MT('v', false), MT('w', false), MT('x', false),
	MT('y', false), MT('z', false)
};
#undef MT

//Randomly pick the word that needs to be guessed
Hackdle::Hackdle(std::string correct_answer) {
	//Make all input lower case for comparisons
	std::transform(correct_answer.begin(), correct_answer.end(), correct_answer.begin(), ::tolower);
	this->correct_answer = correct_answer;
	this->complete = false;
}

//Check to make sure user is inputting a letter
bool Hackdle::is_valid_character(char c){
	return c >= 97 && c <= 122;
}

//Check to see if the word guessed is a possible answer
bool Hackdle::is_in_word_list(std::string guess){
	//iterate through wordlist
	for(int i = 0; i < Hackdle::wordlist_length; i++){
		//If it's in wordlist return true
		if(guess == this->wordlist[i]){
			return true;
		}
	}
	return false;
}

void Hackdle::guess(std::string guess) {
	//Error handling wooo
	if (guess.length() > 5) {
		if (this->guesses.size() < 4)
			std::cout << std::format("\033[{}B", 4 - this->guesses.size());
		throw Hackdle::Error::GuessTooLong;
	}
	if(guess.length() < 5){
		if (this->guesses.size() < 4)
			std::cout << std::format("\033[{}B", 4 - this->guesses.size());
		throw Hackdle::Error::GuessTooShort;
	}

	//Initialize letter results array (With respect to memory)
	std::array<LetterResult, 5> letter_results = {};
	std::string check_for_multiple_instances = correct_answer;
	char temp = ' ';
	int pos = 0;

	for(int i = 0; i < guess.length(); i++){
		//Throw error if input is not valid
		if(!is_valid_character(guess[i])){
			if (this->guesses.size() < 4)
				std::cout << std::format("\033[{}B", 4 - this->guesses.size());
			throw Hackdle::Error::InvalidCharacter;
		}
		//Check if the letter is correct and in the correct position
		if(guess[i] == correct_answer[i]){
			letter_results[i] = LetterResult::CorrectPosition;
			check_for_multiple_instances[i] = ' ';
		}
		//Check if letter is in word but wrong position
		else if(check_for_multiple_instances.find(guess[i]) != std::string::npos) {
			letter_results[i] = LetterResult::WrongPosition;
			for(int j = 0; j < check_for_multiple_instances.size(); j++){
				if(guess[i] == check_for_multiple_instances[j]){
					temp = check_for_multiple_instances[j];
					pos = j;
					check_for_multiple_instances[j] = ' ';
				}
			}
		}
		//Check if letter is completely absent
		else {
			letter_results[i] = LetterResult::Absent;
		}
	}
	if(temp != ' '){
		check_for_multiple_instances[pos] = temp;
	}

	if(!is_in_word_list(guess)){
		if (this->guesses.size() < 4)
			std::cout << std::format("\033[{}B", 4 - this->guesses.size());
		throw Hackdle::Error::NotInWordList;
	}

	this->guesses.emplace_back(std::make_tuple(guess, letter_results));

	// Update `letters_guessed`
	for (int i = 0; i < 26; i++) {
		char c = std::get<0>(this->letters_guessed[i]);
		/* Only mark a letter as guessed if it's in
		the guess and not in the correct answer */
		if (guess.find(c) != std::string::npos && this->correct_answer.find(c) == std::string::npos)
			std::get<1>(this->letters_guessed[i]) = true;
	}

	if (guess == this->correct_answer)
		this->complete = true;
}

bool Hackdle::is_complete() {
	return this->complete;
}

//Print out guesses with correct coloring
void Hackdle::print() {
	std::cout << "Guesses:             Letters:" << std::endl;
	int letters_guessed_start = 0;
	for (int i = 0; i < this->guesses.size(); i++) {
		const auto& guess = this->guesses[i];
		TerminalColor::Color color = TerminalColor::Black;
		for (int i = 0; i < std::get<0>(guess).length(); i++) {
			switch (std::get<1>(guess)[i]) {
				case LetterResult::CorrectPosition:
					color = TerminalColor::Green;
					break;
				case LetterResult::WrongPosition:
					color = TerminalColor::Yellow;
					break;
				case LetterResult::Absent:
					color = TerminalColor::Black;
					break;
			}
			// Print word guess line
			TerminalColor::print(std::format(" {} ", std::get<0>(guess)[i]), color,
								 (color == TerminalColor::Black ? true : false));
		}
		// Print letters guessed
		std::cout << "      ";
		if (letters_guessed_start <= 24) {
			// Get a slice of 6 chars (or of 2 if we're printing the last 2 letters)
			std::span slice(this->letters_guessed.begin() + letters_guessed_start, letters_guessed_start < 24 ? 6 : 2);
			for (const auto& letter : slice) {
				bool letter_was_guessed = std::get<1>(letter);
				TerminalColor::print(std::format(" {} ", std::get<0>(letter)), TerminalColor::Black,
					(letter_was_guessed ? false : true), (letter_was_guessed ? TerminalColor::Black : TerminalColor::White));
			}
			letters_guessed_start += 6;
		}
		if (i < this->guesses.size() - 1)
			std::cout << std::endl;
	}
	// Print the remaining lines of `letters_guessed` if needed
	for (; letters_guessed_start <= 24; letters_guessed_start += 6) {
		std::cout << std::endl << "                     ";
		std::span slice(this->letters_guessed.begin() + letters_guessed_start, letters_guessed_start < 24 ? 6 : 2);
		for (const auto& letter : slice) {
			bool letter_was_guessed = std::get<1>(letter);
			TerminalColor::print(std::format(" {} ", std::get<0>(letter)), TerminalColor::Black,
				(letter_was_guessed ? false : true), (letter_was_guessed ? TerminalColor::Black : TerminalColor::White));
		}
	}
	std::cout << "\r";
	// Move cursor back up to after the last guess
	if (this->guesses.size() < 4)
		std::cout << std::format("\033[{}A", 4 - this->guesses.size());
	if (this->guesses.size() > 4)
		std::cout << std::format("\033[{}B", 1);
}

std::string Hackdle::get_correct_answer() {
	return this->correct_answer;
}