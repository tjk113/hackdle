#pragma once

#include <algorithm>
#include <cstring>
#include <string>
#include <vector>
#include <tuple>
#include <array>
#include <map>

class Hackdle {
public:
	enum class Error {
		GuessTooLong,
		GuessTooShort,
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
	static std::array<std::tuple<char, bool>, 26> letters_guessed;
	std::vector<std::tuple<std::string, std::array<LetterResult,5>>> guesses;
	std::string correct_answer;
	bool complete;
	bool is_valid_character(char c);
	bool is_in_word_list(std::string guess);
public:
	Hackdle(std::string correct_answer);
	void print();
	void guess(std::string guess);
	bool is_complete();
	std::string get_correct_answer();
};