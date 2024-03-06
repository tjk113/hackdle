#include <string>
#include <array>

#include "TerminalColor.hpp"
#include "Hackdle.hpp"

Hackdle::Hackdle(std::string correct_answer) {
	this->correct_answer = correct_answer;
	this->complete = false;
}

std::array<Hackdle::LetterResult,5> Hackdle::guess(std::string guess) {
	if (guess.length() > 5)
		throw Hackdle::Error::GuessTooLong;
	std::array<LetterResult,5> letter_results;

	// TODO: get LetterResult for each letter

	this->guesses.insert({guess, letter_results});
}

bool Hackdle::is_complete() {
	return this->complete;
}

void Hackdle::print() {
	for (const auto& guess : this->guesses) {
		for (int i = 0; i < guess.first.length(); i++) {
			TerminalColor::Color color = TerminalColor::Black;
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
			TerminalColor::print(guess.first, color, (color == TerminalColor::Black ? true : false));
		}
	}
}