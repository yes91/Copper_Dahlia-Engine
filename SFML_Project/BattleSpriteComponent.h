#include "ComponentBase.h"
#include "MultiResCache.h"
#include "AnimatedSprite.h"

class BattleSpriteComponent : ComponentBase<BattleSpriteComponent>
{
public:

	BattleSpriteComponent(GameObject* owner, std::string image)
		:ComponentBase(owner)
	{
		this->image = MultiResCache::getInstance().GetHandle<sf::Texture>(image);

		
	}

private:
	
	Resource<sf::Texture>::type image;
};