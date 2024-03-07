#include <algorithm>
#include <iostream>
#include <cstring>
#include <string>
#include <format>
#include <tuple>
#include <array>

#include "TerminalColor.hpp"
#include "Hackdle.hpp"

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
	if (guess.length() > 5)
		throw Hackdle::Error::GuessTooLong;
	if(guess.length() < 5){
		throw Hackdle::Error::GuessTooShort;
	}

	//Initialize letter results array (With respect to memory)
	std::array<LetterResult, 5> letter_results = {};

	for(int i = 0; i < guess.length(); i++){
		//Throw error if input is not valid
		if(!is_valid_character(guess[i])){
			throw Hackdle::Error::InvalidCharacter;
		}
		//Check if the letter is correct and in the correct position
		if(guess[i] == correct_answer[i]){
			letter_results[i] = LetterResult::CorrectPosition;
		}
		//Check if letter is in word but wrong position
		else if(correct_answer.find(guess[i]) != std::string::npos){
			letter_results[i] = LetterResult::WrongPosition;
		}
		//Check if letter is completely absent
		else {
			letter_results[i] = LetterResult::Absent;
		}
	}

	if(!is_in_word_list(guess)){
		throw Hackdle::Error::NotInWordList;
	}

	this->guesses.emplace_back(std::make_tuple(guess, letter_results));

	if (guess == this->correct_answer)
		this->complete = true;
}

bool Hackdle::is_complete() {
	return this->complete;
}

//Print out guesses with correct coloring
void Hackdle::print() {
	for (const auto& guess : this->guesses) {
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
			TerminalColor::print(std::format(" {} ", std::get<0>(guess)[i]), color, (color == TerminalColor::Black ? true : false));
		}
		std::cout << std::endl;
	}
}