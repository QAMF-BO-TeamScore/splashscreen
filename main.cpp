#include "colorconverter.h"
#include "splashscreenmessage.h"
#include <SFML/Graphics.hpp>
#include <cpprest/http_listener.h>
#include <boost/lockfree/queue.hpp>
#include <iostream>

using namespace std;

namespace web { namespace http {
namespace listener = experimental::listener;
}}

int main()
{
	const size_t WINDOW_WIDTH{1280};
	const size_t WINDOW_HEIGHT{720};
	const size_t FONT_SIZE{100};

	const string defImagePath{"media/image2.png"};

	boost::lockfree::queue<SplashScreen::Message> queue{100};

	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Splashscreen");
	window.setPosition(sf::Vector2i{1280,0});

	sf::RectangleShape shape(sf::Vector2f{WINDOW_WIDTH, WINDOW_HEIGHT});
	sf::Sprite image{};
	sf::Font font;
	font.loadFromFile("media/font_rev.ttf");
	sf::Text text{sf::String("ciao"), font, FONT_SIZE};
	text.setPosition(sf::Vector2f{0, WINDOW_HEIGHT / 2});

	shape.setFillColor(sf::Color::Green);

	web::http::uri_builder path("http://0.0.0.0:6543");
	path.append_path("splashscreen");
	auto setupPath = path.to_string();

	web::http::listener::http_listener splashscreen(setupPath);
	splashscreen.support(web::http::methods::POST, [&queue] (web::http::http_request request) {
		using namespace SplashScreen;

		auto bodyTask = request.extract_string(true);

		auto message = Parser::parse(bodyTask.get());
		cout << "incoming message:\n" << message << endl;

		queue.push(message);

		request.reply(web::http::status_codes::OK);
	});

	splashscreen.open().wait();

	sf::Texture texture;
	sf::Texture::bind(&texture);

	while (window.isOpen())
	{
		SplashScreen::Message message;

		if (queue.pop(message))
		{
			if (message.isColorValid)
				shape.setFillColor(sf::Color(message.color));

			if (message.isImagePathValid)
			{
				texture.loadFromFile(string{begin(message.imagePath), end(message.imagePath)});
				image = sf::Sprite{texture};
			}

			if (message.isMessageValid)
				text.setString(sf::String(string{begin(message.message), end(message.message)}));
		}

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
				window.close();
		}

		window.clear();
		window.draw(shape);
		window.draw(image);
		window.draw(text);

		window.display();
	}

	return 0;
}
