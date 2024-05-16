//
// created by Michael Ward on 01/05/24
//

// Declare modules
mod entities;
mod screen;
mod ui;
// ---

use entities::player::*;
// Namespaces
use macroquad::prelude::*;
use screen::*;
use ui::prelude::*;
// ---

// Constants

/// Screen Height.
const WINDOW_HEIGHT: u16 = 800;
/// Screen Width.
const WINDOW_WIDTH: u16 = 800;
// ---

#[macroquad::main(window_conf)]

async fn main() {
    let mut player = Player::new(
        400.0,
        400.0,
        load_texture("assets/miku.png").await.unwrap(),
        WHITE,
    );

    let button = Button::new(50.0, 100.0, "Play".to_string(), DARKBLUE, WHITE);
    let title = TextObject::new(30.0, 40.0, "logger".to_string(), 45.0, WHITE);
    let title_screen: Menu = Menu {
        elements: vec![Box::new(&button), Box::new(&title)],
    };

    let mut master_state = Scene::init();

    // Added in reverse order
    master_state.push_mut(&mut player);
    master_state.push(&title_screen);

    loop {
        clear_background(SKYBLUE);

        master_state.tick();

        if button.interacted() {
            println!("Interacted!");
        }

        if is_key_down(KeyCode::Escape) {
            break;
        }
        next_frame().await
    }
}

/// Function to initialise game window
fn window_conf() -> Conf {
    Conf {
        window_title: "logger".to_owned(),
        window_width: WINDOW_WIDTH as i32,
        window_height: WINDOW_HEIGHT as i32,
        ..Default::default()
    }
}

/// Callable logic or draw data to be run on the game loop
trait Call {
    /// Run data/logic that doesn't require modifying the related entity. Used for rendering.
    fn call(&self) {}

    /// Run entity logic & interaction, such as components or other game information.
    fn call_mut(&mut self) {}
}

impl<T: ?Sized + Call> Call for &'_ T {
    fn call(&self) {
        <T as Call>::call(&**self);
    }
}

impl<T: ?Sized + Call> Call for &'_ mut T {
    fn call_mut(&mut self) {
        <T as Call>::call_mut(&mut **self);
    }
    fn call(&self) {
        <T as Call>::call(&**self);
    }
}

/// Container for call objects.
/// Bundle and call all gameObjects at once
struct Scene<'s> {
    pub function_stack: Vec<Box<dyn Call + 's>>, // Used to run/handle game logic
}

impl<'s> Scene<'s> {
    /// Constructor.
    /// Initializes with empty function stack
    fn init() -> Self {
        Scene {
            function_stack: vec![],
        }
    }
    /// Call all function logic
    fn tick(&mut self) {
        for function in &mut self.function_stack {
            function.call_mut();
            function.call();
        }
    }

    /// Push value as mutable reference
    /// For player, maps & other gameObjects
    fn push_mut<T: Call>(&mut self, item: &'s mut T) {
        self.function_stack.push(Box::new(item));
    }

    /// Push value as reference
    /// For UI and the like
    fn push<T: Call>(&mut self, item: &'s T) {
        self.function_stack.push(Box::new(item));
    }
}
