#pragma once

#include <string>
#include <array>

class Hackdle {
private:
	std::string correct_answer;
	int num_guesses;
public:
	enum Error {
		GuessTooLong
	};
	enum class LetterResult {
		CorrectPosition,
		WrongPosition,
		Absent
	};
	Hackdle(std::string correct_answer);
	std::array<LetterResult,5> guess(std::string guess);
};