#include "splashscreenmessage.h"
#include "colorconverter.h"
#include <string>
#include <regex>
#include <iomanip>
#include <cstdint>

using namespace std;

namespace SplashScreen {

Message Parser::parse(const string& messageToParse)
{
	static std::regex re{"&"};

	std::sregex_token_iterator it{std::begin(messageToParse), std::end(messageToParse), re, -1};
	std::sregex_token_iterator end{};

	auto message = createMessage();

	for(;it != end; ++it)
		parseField(*it, message);

	return message;
}

Message Parser::createMessage()
{
	Message msg;
	msg.imagePath.fill('\0');
	msg.message.fill('\0');
	msg.color = 0;

	msg.isImagePathValid = false;
	msg.isMessageValid = false;
	msg.isColorValid = false;

	return msg;
}

void SplashScreen::Parser::parseField(const std::string& row, Message& msg)
{
	auto separator = row.find('=');

	auto field = row.substr(0, separator);
	auto value = row.substr(separator + 1);

	assignField(field, value, msg);
}

void Parser::assignField(const string& field, const string value, Message& msg)
{
	static const std::string IMAGE{"image"};
	static const std::string COLOR{"color"};
	static const std::string MESSAGE{"message"};

	if (field == IMAGE)
	{
		std::copy(std::begin(value), std::end(value), std::begin(msg.imagePath));
		msg.isImagePathValid = true;
	}

	if (field == MESSAGE)
	{
		std::copy(std::begin(value), std::end(value), std::begin(msg.message));
		msg.isMessageValid = true;
	}

	if (field == COLOR)
	{
		msg.color = toIntColor(value);
		msg.isColorValid = true;
	}
}

std::ostream& operator << (std::ostream& os, const SplashScreen::Message& msg)
{
	if (msg.isImagePathValid)
		os << "image path: " << msg.imagePath.data() << '\n';

	if (msg.isMessageValid)
		os << "message: " << msg.message.data() << '\n';

	if (msg.isColorValid)
		os << "color: "
		   << std::hex << std::setfill('0') << std::setw(8)
		   << msg.color
		   << std::setfill(' ') << std::dec << '\n';
	return os;
}

} // SplashScreen

