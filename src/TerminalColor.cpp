#include <iostream>
#include <string>

#include "TerminalColor.hpp"

void TerminalColor::print(std::string string, TerminalColor::Color background, bool bright_background, TerminalColor::Color foreground) {
	// See here for color code information: https://chrisyeh96.github.io/2020/03/28/terminal-colors.html
	std::string color_sequence = "\033["
								 + std::to_string(90 + foreground)
								 + ';'
								 + std::to_string((bright_background ? 100 : 40) + background)
								 + 'm';
	static std::string reset_sequence = "\033[0m";
	std::cout << color_sequence << string << reset_sequence;
}

void TerminalColor::clear() {
	std::cout << "\033[2J\033[2H\033[1A";
}