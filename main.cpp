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
	const auto WIN_RECT = sf::IntRect{0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};

	const string defImagePath{"image2.png"};

	boost::lockfree::queue<SplashScreen::Message> queue{100};

	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Splashscreen");
	sf::RectangleShape shape(sf::Vector2f{WINDOW_WIDTH, WINDOW_HEIGHT});
	sf::Texture texture;

	texture.loadFromFile(defImagePath, WIN_RECT);
	sf::Sprite image{texture};

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
				texture.loadFromFile(string{begin(message.imagePath), end(message.imagePath)}, WIN_RECT);
				image = sf::Sprite{texture};
			}
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
		window.display();
	}

	return 0;
}
