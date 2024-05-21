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

    let mut main_level = Level::new("test_cfg.toml").await;

    let mut master_state = Scene::init();

    master_state.push_mut(&mut main_level);
    master_state.push_mut(&mut player);
    master_state.push(&title_screen);
    master_state.push_fn(0, test_hello);

    loop {
        clear_background(LIME);

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

enum RunCode {
    Ok,
    Err(&'static str),
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
    pub function_stack: Vec<Vec<fn() -> RunCode>>, // Used to run/handle game logic
    pub entity_stack: Vec<Box<dyn Call + 's>>,     // Used to handle entities
}

impl<'s> Scene<'s> {
    /// Constructor.
    /// Initializes with empty entity stack & one function stack
    fn init() -> Self {
        Scene {
            function_stack: vec![vec![]],
            entity_stack: vec![],
        }
    }
    /// Calls all function & entity logic
    fn tick(&mut self) {
        // Function Logic
        let mut i = 0;
        while i < self.function_stack.len() {
            // Roll through stacks
            let mut x = 0;
            while x < self.function_stack[i].len() {
                // Roll through functions
                let val = self.function_stack[i][x]();
                match val {
                    RunCode::Err(msg) => {
                        panic!("Function #{x} failed on Stack #{i} with message: {msg}")
                    }
                    _ => (),
                }
                x += 1;
            }
            i += 1;
        }
        // Entity Logic
        for entity in &mut self.entity_stack {
            entity.call_mut(); // Mutate stuff
            entity.call(); // Use stuff
        }
    }

    /// Push value as mutable reference
    /// For player, maps & other gameObjects
    fn push_mut<T: Call>(&mut self, item: &'s mut T) {
        self.entity_stack.push(Box::new(item));
    }

    /// Push value as reference
    /// For UI and the like
    fn push<T: Call>(&mut self, item: &'s T) {
        self.entity_stack.push(Box::new(item));
    }

    /// Push function to the function stack.
    /// Requires stack to exist
    fn push_fn(&mut self, stack: usize, value: fn() -> RunCode) {
        self.function_stack[stack].push(value);
    }

    /// Creates new function stack.
    fn new_stack(&mut self) {
        self.function_stack.push(vec![]);
    }
}

fn test_hello() -> RunCode {
    println!("Hello, World!");
    RunCode::Ok
}
