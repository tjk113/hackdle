#include <algorithm>
#include <iostream>
#include <cstring>
#include <string>
#include <format>
#include <tuple>
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

bool Hackdle::guess(std::string guess) {
	if (guess.length() > 5)
		throw Hackdle::Error::GuessTooLong;

	std::array<LetterResult,5> letter_results = {};

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

	this->guesses.emplace_back(std::make_tuple(guess, letter_results));
}

bool Hackdle::is_complete() {
	return this->complete;
}

//TODO clear screen 
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