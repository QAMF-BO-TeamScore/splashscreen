#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>
#include <SFML/Graphics/Color.hpp>

inline sf::Color toSFMLColor(const std::string& color)
{
	using namespace std::literals::string_literals;

	static const std::unordered_map<std::string, sf::Color> COLOR_MAP =
	{
		{"black"s, sf::Color::Black},
		{"white"s, sf::Color::White},
		{"red"s, sf::Color::Red},
		{"green"s, sf::Color::Green},
		{"blue"s, sf::Color::Blue},
		{"yellow"s, sf::Color::Yellow},
		{"magenta"s, sf::Color::Magenta},
		{"cyan"s, sf::Color::Cyan},
	};

	auto colIt = COLOR_MAP.find(color);
	if (colIt != std::end(COLOR_MAP))
		return colIt->second;

	throw std::invalid_argument{"Unknown color"};
}

inline int toIntColor(const std::string& color)
{
	using namespace std::literals::string_literals;

	static const std::unordered_map<std::string, uint32_t> COLOR_MAP =
	{
		{"black"s, 0x000000FFu},
		{"white"s, 0xFFFFFFFFu},
		{"red"s, 0xFF0000FFu},
		{"green"s, 0x00FF00FF},
		{"blue"s, 0x0000FFFF},
		{"yellow"s, 0xFFFF00FF},
		{"magenta"s, 0xFF00FFFF},
		{"cyan"s, 0x00FFFFFF},
	};

	auto colIt = COLOR_MAP.find(color);
	if (colIt != std::end(COLOR_MAP))
		return colIt->second;

	throw std::invalid_argument{"Unknown color"};
}
