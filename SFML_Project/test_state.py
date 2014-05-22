from core import *
from sfml import *

import math

class TestState(IGameState):
    def __init__(self):
        IGameState.__init__(self)

    def get_id(self):
        return 22

    def init(self, game):
        print("init")
        self.font = Resources.instance().get_font("sansation.ttf")

        self.text = Text()
        self.text.set_font(self.font.get())
        self.text.set_color(Color(255, 255, 255))
        self.text.set_string("Hello World! Konnichiwa Sekai!")
        self.text.set_pos(300, 1280/2)

        self.rect = RectangleShape(Vector2f(50, 80))
        self.rect.set_fill(Color.cyan)

        self.get_dispatch().add_listener(KeyEvent, self.whoop)

        self.time = 0
        self.x = 0
        self.y = 0

        self.initialized = True

    def whoop(self, event):
        print("Hey now")

    def handle_events(self, event, game):
        if event.type == EventType.KeyReleased and event.key.code == Key.R:
            game.enter_state(0)
        if event.type == EventType.MouseButtonPressed and event.mouse_button.button == MouseButton.left:
            self.text.set_pos(event.mouse_button.x, event.mouse_button.y)
        if event.type == EventType.KeyReleased:
            self.get_dispatch().trigger_event(event.key)
            

    def update(self, container, dt, game):
        self.time += dt
        if Keyboard.key_pressed(Key.W):
            self.y -= 100 * dt
        if Keyboard.key_pressed(Key.S):
            self.y += 100 * dt
        if Keyboard.key_pressed(Key.A):
            self.x -= 100 * dt
        if Keyboard.key_pressed(Key.D):
            self.x += 100 * dt

        self.rect.set_size(Vector2f(50 * math.cos(self.time), 50 * math.sin(self.time)))

        self.rect.set_pos(self.x, 50 * math.sin( 8 * self.time) + self.y)

    def render(self, g, alpha):
        g.draw(self.rect)
        g.draw(self.text)

    def enter(self, game):
        print("enter")

    def leave(self, game):
        print("leave")

