#pragma once

#include <iosfwd>
#include <array>
#include <cstdint>

namespace SplashScreen {

struct Message {
	bool isImagePathValid;
	bool isMessageValid;
	bool isColorValid;

	std::array<char, 512> imagePath;
	std::array<char, 512> message;
	std::uint32_t color;
};

class Parser
{
public:
	static Message parse(const std::string& messageToParse);

private:
	static Message createMessage();
	static void parseField(const std::string& row, Message& msg);
	static void assignField(const std::string& field, const std::string value, Message& msg);
};

std::ostream& operator << (std::ostream& os, const SplashScreen::Message& msg);

} // SplashScreen

