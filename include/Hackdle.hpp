#pragma once

#include <string>
#include <vector>
#include <tuple>
#include <array>
#include <map>

class Hackdle {
public:
	enum class Error {
		GuessTooLong,
		GuessTooSort,
		InvalidCharacter,
		NotInWordList
	};
	enum class LetterResult {
		CorrectPosition,
		WrongPosition,
		Absent
	};
	static constexpr int wordlist_length = 14855;
	static const std::string wordlist[wordlist_length];
private:
	std::vector<std::tuple<std::string, std::array<LetterResult,5>>> guesses;
	std::string correct_answer;
	bool complete;
	bool is_valid_character(char c);
public:
	Hackdle(std::string correct_answer);
	void print();
	bool guess(std::string guess);
	bool is_complete();
};