#pragma once

#include <string>
#include <regex>

struct SplashScreenMessage
{
	SplashScreenMessage(const std::string& message)
	{
		static std::regex re{"&"};

		std::sregex_token_iterator it{std::begin(message), std::end(message), re, -1};
		std::sregex_token_iterator end{};

		for(;it != end; ++it)
		{
			parseField(*it);
		}
	}

	std::string imagePath{"."};
	std::string message{""};
	std::string color{"blue"};

	struct SplashScreenMessage
	{
		SplashScreenMessage(const std::string& message)
		{
			static std::regex re{"&"};

			std::sregex_token_iterator it{std::begin(message), std::end(message), re, -1};
			std::sregex_token_iterator end{};

			for(;it != end; ++it)
			{
				parseField(*it);
			}
		}

		std::string imagePath{"."};
		std::string message{""};
		std::string color{"blue"};

		friend std::ostream& operator << (std::ostream& os, const SplashScreenMessage& msg)
		{
			os << "image path: " << msg.imagePath << '\n'
			   << "message: " << msg.message << '\n'
			   << "color: " << msg.color << '\n';
			return os;
		}
	private:
		void parseField(const std::string& row)
		{
			auto separator = row.find('=');

			auto field = row.substr(0, separator);
			auto value = row.substr(separator + 1);

			assignField(field, value);
		}

		void assignField(const std::string& field, const std::string value)
		{
			static const std::string IMAGE{"image"};
			static const std::string COLOR{"color"};
			static const std::string MESSAGE{"message"};

			if (field == IMAGE) imagePath = value;
			if (field == COLOR) color = value;
			if (field == MESSAGE) message = value;
		}
	};

	friend std::ostream& operator << (std::ostream& os, const SplashScreenMessage& msg)
	{
		os << "image path: " << msg.imagePath << '\n'
		   << "message: " << msg.message << '\n'
		   << "color: " << msg.color << '\n';
		return os;
	}

private:
	void parseField(const std::string& row)
	{
		auto separator = row.find('=');

		auto field = row.substr(0, separator);
		auto value = row.substr(separator + 1);

		assignField(field, value);
	}

	void assignField(const std::string& field, const std::string value)
	{
		static const std::string IMAGE{"image"};
		static const std::string COLOR{"color"};
		static const std::string MESSAGE{"message"};

		if (field == IMAGE) imagePath = value;
		if (field == COLOR) color = value;
		if (field == MESSAGE) message = value;
	}
};
