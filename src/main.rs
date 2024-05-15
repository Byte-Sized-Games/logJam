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
use ui::*;
// ---

struct GameState {
    /// @brief scene-specific logic callstack. runs after master callstack on each priority level
    logic_stack: Vec<Vec<bool>>,
    /// @brief scene-specific draw callstack. runs after master callstack on each priority level
    draw_stack: Vec<Vec<bool>>,
    /// @brief vector of elements drawn on first state call via a draw_stack item
    init_scene: Vec<*const dyn Ui>
}


mod runtime {
    use crate::GameState;


    /// @brief master logic callstack. not recommended in most cases, try pushing to a scene-specific callstack to avoid transition hell
    static mut LOGIC_STACK : Vec<Vec<bool>> = vec![];
    /// @brief master draw callstack. not recommended in most cases, try pushing to a scene-specific callstack to avoid transition hell
    static mut DRAW_STACK : Vec<Vec<bool>> = vec![];

    /// @brief currently rendering GameState
    // TODO: implement init scene
    static mut CURRENT_STATE : GameState = GameState{
        logic_stack: vec![],
        draw_stack: vec![],
        init_scene: vec![],
    };

}
fn load_state() {

}

// #[macroquad::main("logjam")]

// Constants
const WINDOW_HEIGHT: u16 = 800;
const WINDOW_WIDTH: u16 = 800;
// ---

#[macroquad::main(window_conf)]

async fn main() {
    let button = button::Button::new(50.0, 100.0, "Play".to_string(), DARKBLUE, WHITE);
    let title = text_object::TextObject::new(30.0, 40.0, "logger".to_string(), 45.0, WHITE);
    let title_screen = Menu {
        elements: vec![Box::new(&button), Box::new(&title)],
    };
    loop {
        clear_background(SKYBLUE);

        title_screen.render();

        // Get button clicked
        if button.interacted() {
            println!("Pressed!");
        } else {
            println!("Not pressed!");
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
