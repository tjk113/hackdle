#pragma once

#include <string>
#include <array>
#include <map>

class Hackdle {
public:
	enum Error {
		GuessTooLong
	};
	enum class LetterResult {
		CorrectPosition,
		WrongPosition,
		Absent
	};
private:
	std::map<std::string, std::array<LetterResult,5>> guesses;
	std::string correct_answer;
	bool complete;
public:
	Hackdle(std::string correct_answer);
	void print();
	std::array<LetterResult,5> guess(std::string guess);
	bool is_complete();
};