#include "Component.h"
#include "GameObject.h"

Component::~Component()
{
	for(std::pair<ListenerIter, DelegateIter> d : delegates)
	{
		m_pOwner->removeListener(d);
	}
};
