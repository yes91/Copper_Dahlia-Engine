require 'lua.SceneMenu'

class 'SceneMap' (IGameState)

  function SceneMap:__init()
      IGameState.__init(self)
	  self.tex = Cache:get_tex("res/Evil.png")
	  self.sprite = Sprite()
	  self.sprite:set_tex(self.tex:get(), false)

	  --self:setSelf(self)
  end

  function SceneMap:getID()
      return 12
  end

  function SceneMap:handleEvents(e, game)
      if e.type == Event.KeyReleased and e.key.code == 4 then
	      game:enterState(0)
	  end
  end

  function SceneMap:update(w, delta, game)
      
  end

  function SceneMap:render(w)
      w:draw(self.sprite, Default)
  end

  function SceneMap:enter(game)
      
  end

  function SceneMap:leave(game)
      
  end

  function SceneMap:__finalize()
      print("SceneMap died!")
  end

--Register State
G:addState(SceneMap())