//
// created by Michael Ward on 01/05/24
//

// Declare modules
mod entities;
mod screen;
mod ui;
// ---

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
    let button = Button::new(
        50.0,
        100.0,
        "Play".to_string(),
        RunAction::LoadScene(1),
        DARKBLUE,
        WHITE,
    );
    let title = TextObject::new(30.0, 40.0, "logger".to_string(), 45.0, WHITE);
    let title_screen: Menu = Menu {
        elements: vec![Box::new(&button), Box::new(&title)],
    };

    // Initialise Test level
    let mut main_level = Level::new("test_cfg.toml").await;

    // Initialise Title screen

    // Initialise Scenes & Screens
    let mut master_state = Game::init();
    master_state.scene_stack.push(Box::new(&title_screen));
    master_state.scene_stack.push(Box::new(&mut main_level));
    master_state.push_fn(0, check_esc);

    loop {
        clear_background(LIME);

        let output = master_state.tick();

        if let RunAction::Quit = output {
            break;
        }

        if button.interacted() {
            master_state.active_scene = 1;
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
    Action(RunAction),
    Err(&'static str),
}

#[derive(Clone, Copy)]
enum RunAction {
    NextScene,
    LoadScene(usize),
    PrevScene,
    Quit,
    None,
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
struct Game<'s> {
    function_stack: Vec<Vec<fn() -> RunCode>>, // Used to run/handle game logic
    entity_stack: Vec<Box<dyn Call + 's>>,     // Used to handle entities
    scene_stack: Vec<Box<dyn Call + 's>>,      // Game Scenes
    active_scene: usize,                       // Tracker for active scene
}

impl<'s> Game<'s> {
    /// Constructor.
    /// Initializes with empty entity stack & one function stack
    fn init() -> Self {
        Game {
            function_stack: vec![vec![]],
            entity_stack: vec![],
            scene_stack: vec![],
            active_scene: 0,
        }
    }
    /// Calls all function & entity logic
    fn tick(&mut self) -> RunAction {
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
                    RunCode::Action(act) => match act {
                        RunAction::Quit => return act,
                        RunAction::LoadScene(scene) => self.active_scene = scene,
                        RunAction::None => (),
                        RunAction::NextScene => self.active_scene += 1,
                        RunAction::PrevScene => self.active_scene -= 1,
                    },
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
        // Scene logic
        self.scene_stack[self.active_scene].call();
        self.scene_stack[self.active_scene].call_mut();

        RunAction::None
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
    fn new_function_stack(&mut self) {
        self.function_stack.push(vec![]);
    }
}

impl Call for Game<'_> {
    fn call_mut(&mut self) {
        self.tick();
    }
}

// Grid Tools

fn draw_grid() -> RunCode {
    let mut x = 0.0;
    let mut y = 0.0;

    while x < screen_width() {
        draw_rectangle(x - 1.0, y, 2.0, screen_height(), WHITE);
        x += 100.0;
    }
    x = 0.0;

    while y < screen_height() {
        draw_rectangle(x, y - 1.0, screen_width(), 2.0, WHITE);
        y += 100.0;
    }

    RunCode::Ok
}

// Utilities

/// Basic checker for pressing escape key
/// Used to quit game for the moment, may become used for a pause menu in the future
fn check_esc() -> RunCode {
    if is_key_down(KeyCode::Escape) {
        return RunCode::Action(RunAction::LoadScene(0));
    }
    RunCode::Ok
}
