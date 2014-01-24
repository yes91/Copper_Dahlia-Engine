class 'SceneMenu' (IGameState)

  function SceneMenu:__init()
      IGameState.__init(self)
      self.back = Cache:get_tex("res/TitleDesert.png")
	  self.font = Cache:get_font("sansation.ttf")
	  self.title = Cache:get_tex("res/Title.png")
	  self.planet = Cache:get_tex("res/Planet.png")

	  self.win = Window(300, 200)
	  self.win:setPosition(1280/2 - 150, 720 - 200 - 50)

	  self.text = Text()
	  self.text:setFont(self.font:get())

	  self.background = Sprite()
	  self.background:set_tex(self.back:get(), false)

	  self.title_layer = Sprite()
	  self.title_layer:set_tex(self.title:get(), false)
	  self.title_layer:setPosition(-20, 0)

	  self.planet_layer = Sprite()
	  self.planet_layer:set_tex(self.planet:get(), false)
	  self.planet_layer:setPosition(550 - 20, 20)

	  self.sandstorm = ParallaxPlane("res/Sandstorm.png", Vector2f(1280, 720), Vector2f(8 * 60, 8 * 60))
	  self.sandstorm_top = ParallaxPlane("res/Sandstorm_top.png", Vector2f(1280, 512), Vector2f(9 * 60, 0))
	  self.bgm = Music()
	  self.bgm:openFromFile("res/Audio/BGM/Overland_Blues.ogg")
	  self.bgm:setLoop(true)

	  --self:setSelf(self)
  end

  function SceneMenu:getID()
      return 3
  end

  function SceneMenu:handleEvents(e, game)
      if e.type == Event.KeyReleased and e.key.code == 4 then
		game:enterState(0)
	  end
  end

  function SceneMenu:update(w, delta, game)
      self.text:setString(sf_String("FPS: " .. math.floor(1/delta:asSeconds())))
	  self.sandstorm:update(delta)
	  self.sandstorm_top:update(delta)
  end

  function SceneMenu:render(w)
      w:draw(self.background, Default)
	  w:draw(self.sandstorm, Default)
	  w:draw(self.sandstorm_top, Default)
	  w:draw(self.planet_layer, Default)
	  w:draw(self.title_layer, Default)
	  w:draw(self.win, Default)
	  w:draw(self.text, Default)
  end

  function SceneMenu:enter(game)
      self.bgm:play()
  end

  function SceneMenu:leave(game)
      self.bgm:stop()
  end

  function SceneMenu:__finalize()
      print("SceneMenu destroyed")
  end

--Register State
--G:addState(SceneMenu())
--Enter as initial state
G:enterState(SceneMenu())