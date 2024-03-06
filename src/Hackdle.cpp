#include <algorithm>
#include <iostream>
#include <cstring>
#include <string>
#include <format>
#include <array>

#include "TerminalColor.hpp"
#include "Hackdle.hpp"

Hackdle::Hackdle(std::string correct_answer) {
	std::transform(correct_answer.begin(), correct_answer.end(), correct_answer.begin(), ::toupper);
	this->correct_answer = correct_answer;
	this->complete = false;
}

bool Hackdle::is_valid_character(char c){
	return c <= 90 && c >= 65;
}

std::array<Hackdle::LetterResult,5> Hackdle::guess(std::string guess) {
	if (guess.length() > 5)
		throw Hackdle::Error::GuessTooLong;
	std::array<LetterResult,5> letter_results = {};

	// TODO: get LetterResult for each letter
	for(int i = 0; i < guess.length(); i++){

		if(!is_valid_character(guess[i])){
			throw Hackdle::Error::InvalidCharacter;
		}
		if(guess[i] == correct_answer[i]){
			letter_results[i] = LetterResult::CorrectPosition;
		}
		else if(correct_answer.find(guess[i]) != std::string::npos){
			letter_results[i] = LetterResult::WrongPosition;
		}
		else {
			letter_results[i] = LetterResult::Absent;
		}
	}

	this->guesses.insert({guess, letter_results});

	return letter_results;
}

bool Hackdle::is_complete() {
	return this->complete;
}

//TODO clear screen 
void Hackdle::print() {
	for (const auto& guess : this->guesses) {
		TerminalColor::Color color = TerminalColor::Black;
		for (int i = 0; i < guess.first.length(); i++) {
			switch (guess.second[i]) {
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
			TerminalColor::print(std::format(" {} ", guess.first[i]), color, (color == TerminalColor::Black ? true : false));
		}
		std::cout << std::endl;
	}
}