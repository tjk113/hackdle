#pragma once

#include <string>

namespace TerminalColor {
	enum Color {
		Black,
		Red,
		Green,
		Yellow,
		Blue,
		Magenta,
		Cyan,
		White
	};
	void print(std::string string, TerminalColor::Color background, bool bright_background = false, TerminalColor::Color foreground = TerminalColor::Color::White);
};