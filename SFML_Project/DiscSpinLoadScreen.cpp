#include "DiscSpinLoadScreen.h"

#include "MultiResCache.h"
#include "StateBasedGame.h"
#include "Util.h"

DiscSpinLoadScreen::DiscSpinLoadScreen(StateBasedGame& game)
{
	int width = game.getWinState().mode.width;
	int height = game.getWinState().mode.height;

	screen.setPrimitiveType(sf::Quads);
	screen.resize(1 * 4);

	sf::Vertex* quad = &screen[0];
	util::position(quad, sf::IntRect(0, height/2, width, height));
	quad[0].color = sf::Color::Black;
	quad[1].color = sf::Color::Black;
	quad[2].color = sf::Color(235, 115, 40);
	quad[3].color = sf::Color(235, 115, 40);

	font = MultiResCache::getInstance().GetHandle<sf::Font>("sansation.ttf");
	spinner = MultiResCache::getInstance().GetHandle<sf::Texture>("res/Loading_CD.png");
	spinner->get().setSmooth(true);

	cd_sprite.setTexture(spinner->get());
	float spinner_width = spinner->get().getSize().x;
	float spinner_height = spinner->get().getSize().y;
	cd_sprite.setOrigin(spinner_width/2.f, spinner_height/2.f);
	cd_sprite.setPosition((int)((width * 0.845f) - spinner_width/2.f), (int)((height * 0.92f)));

	text.setFont(font->get());
	text.setString("Loading...");
	text.setPosition((int)(width * 0.85f), (int)(height * 0.9f));
}

void DiscSpinLoadScreen::render(sf::RenderWindow& g)
{
	g.draw(screen);
	g.draw(text);
	g.draw(cd_sprite);
}

void DiscSpinLoadScreen::update(float dt)
{
	cd_sprite.rotate(360.f * 80.f * dt); //4800 rpm (standard Audio CD rate)
}
