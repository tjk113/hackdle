#include <string>
#include <array>

#include "Hackdle.hpp"

Hackdle::Hackdle(std::string correct_answer) {
	this->correct_answer = correct_answer;
}

std::array<Hackdle::LetterResult,5> Hackdle::guess(std::string guess) {
	if (guess.length() > 5)
		throw Hackdle::Error::GuessTooLong;

	this->num_guesses++;
}