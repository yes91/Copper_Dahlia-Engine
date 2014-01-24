#include "EditorState.h"

#include "StateBasedGame.h"

#define THEME_CONFIG_FILE "../References/TGUI/widgets/Black.conf"

EditorState::EditorState(sf::RenderWindow& window)
	:map("res/data/map")
{
	gui.setWindow(window);
	gui.setGlobalFont("../References/TGUI/fonts/DejaVuSans.ttf");
}

EditorState::~EditorState()
{

}

int EditorState::getID()
{
	return 10;
}

void EditorState::init( StateBasedGame& game )
{
	
	camera = sf::View(sf::FloatRect(0.0f, 0.0f, 1280.0f, 720.0f));

	camera.setCenter(1280.0f/2.f, 720.0f/2.f);

	map.Load("testmap2.tmx", dispatch);

	tgui::Scrollbar::Ptr v_scroll_bar(gui, "VerticalBar");
	v_scroll_bar->load(THEME_CONFIG_FILE);
	v_scroll_bar->setVerticalScroll(true);
	v_scroll_bar->setLowValue(6);
	v_scroll_bar->setMaximum(map.GetMapSize().y/64 + 6);
	v_scroll_bar->setSize(v_scroll_bar->getSize().x, 695.f);
	v_scroll_bar->setPosition(1275.f - v_scroll_bar->getSize().x, 0.f);
	v_scroll_bar->bindCallback([&](){ camera.setCenter(camera.getCenter().x, gui.get<tgui::Scrollbar>("VerticalBar")->getValue() * 64); }, tgui::Scrollbar::ValueChanged);

	tgui::Scrollbar::Ptr h_scroll_bar(gui, "HorizontalBar");
	h_scroll_bar->load(THEME_CONFIG_FILE);
	h_scroll_bar->setVerticalScroll(false);
	h_scroll_bar->setLowValue(6);
	h_scroll_bar->setMaximum(map.GetMapSize().x/64 + 6);
	h_scroll_bar->setSize(1255.f, h_scroll_bar->getSize().y);
	h_scroll_bar->setPosition(0.f, 715.f - h_scroll_bar->getSize().y);
	h_scroll_bar->bindCallback([&](){ camera.setCenter(gui.get<tgui::Scrollbar>("HorizontalBar")->getValue() * 64, camera.getCenter().y); }, tgui::Scrollbar::ValueChanged);

	tgui::ChildWindow::Ptr right_click(gui, "RightClick");
	right_click->load(THEME_CONFIG_FILE);
	right_click->setSize(200.f, 200.f);
	right_click->bindCallback([&](){ gui.get<tgui::ChildWindow>("RightClick")->hide(); }, tgui::ChildWindow::Closed);
	right_click->hide();
	
	tgui::Button::Ptr add_object(gui);
	gui.remove(add_object);
	add_object->load(THEME_CONFIG_FILE);
	add_object->setText("New GameObject");
	add_object->setPosition(0.f, 0.f);
	add_object->scale(0.75f, 0.75f);
	add_object->bindCallback
		(
	[&]()
	{ 
		auto obj = std::make_shared<GameObject>(dispatch); 
		
		obj->setPosition(gui.get<tgui::ChildWindow>("RightClick")->getPosition() + sf::Vector2f(getCamRect(camera).left, getCamRect(camera).top)); 
		game_objects.push_back(obj); 
	}, 
		tgui::Button::LeftMouseClicked
		);
	right_click->add(add_object, "AddObject");

	tgui::ChildWindow::Ptr c_win(gui, "ObjectWindow");
	c_win->load(THEME_CONFIG_FILE);
	c_win->setSize(300.f, 300.f);
	c_win->bindCallback([&](){ gui.get<tgui::ChildWindow>("ObjectWindow")->hide(); },tgui::ChildWindow::Closed);
	c_win->hide();


	initialized = true;
}

void EditorState::handleEvents( sf::Event event, StateBasedGame& game )
{
	gui.handleEvent(event);

	if(event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right)
	{
		gui.get<tgui::ChildWindow>("RightClick")->setPosition(event.mouseButton.x, event.mouseButton.y);
		gui.get<tgui::ChildWindow>("RightClick")->show();
	}
}

sf::FloatRect EditorState::getCamRect(sf::View camera)
{
	return sf::FloatRect(camera.getCenter().x - camera.getSize().x/2.f, camera.getCenter().y - camera.getSize().y/2.f, camera.getSize().x, camera.getSize().y);
}

void EditorState::update( sf::RenderWindow& container, float dt, StateBasedGame& game )
{
	if(sf::Mouse::isButtonPressed(sf::Mouse::Middle))
	{
		sf::Vector2f mouse_delta = static_cast<sf::Vector2f>(previous - sf::Mouse::getPosition(container));
		camera.move(mouse_delta);
	}

	previous = sf::Mouse::getPosition(container);

	sf::Vector2f mouse_world = container.mapPixelToCoords(sf::Mouse::getPosition(container), camera);

	sf::FloatRect cursor(mouse_world.x, mouse_world.y, 10.f, 10.f);

	map.UpdateQuadTree(getCamRect(camera));

	auto objs = map.QueryQuadTree(cursor);

	for(std::shared_ptr<GameObject>& o : objs)
	{
		if(o->get<IPhysicsComponent>().getGlobalBounds().contains(mouse_world))
		{
			if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				gui.get<tgui::ChildWindow>("ObjectWindow")->setTitle(o->get<tmx::MapObjectComponent>().GetType());
				gui.get<tgui::ChildWindow>("ObjectWindow")->show();
				gui.get<tgui::ChildWindow>("ObjectWindow")->setPosition(static_cast<sf::Vector2f>(sf::Mouse::getPosition(container)));
			}
		}
	}
}

void EditorState::render( sf::RenderWindow& g, float alpha )
{
	g.setView(camera);
	map.Draw(g);

#ifdef DEBUG
	if(Debug::OVERLAY_ON)
	{
		g.setView(camera);
		map.Draw(g, tmx::MapLayer::Debug);
	}
#endif // DEBUG

	sf::RectangleShape rect(sf::Vector2f(30.f, 30.f));
	for(auto& game_obj : game_objects)
	{
		rect.setPosition(game_obj->getPosition());
		rect.setFillColor(sf::Color::Cyan);
		g.draw(rect);
	}

	gui.draw(true);
}

void EditorState::enter( StateBasedGame& game )
{

}

void EditorState::leave( StateBasedGame& game )
{

}