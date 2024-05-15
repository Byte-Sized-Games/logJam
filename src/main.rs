//
// created by Michael Ward on 01/05/24
//

// Declare modules
mod entities;
mod runtime;
mod screen;
mod ui;
// ---

use entities::Player;
// Namespaces
use macroquad::prelude::*;
use screen::*;
use ui::*;
// ---

// Constants
const WINDOW_HEIGHT: u16 = 800;
const WINDOW_WIDTH: u16 = 800;
// ---

#[macroquad::main(window_conf)]

async fn main() {
    let mut master_state = runtime::Scene {
        function_stack: vec![],
    };

    let button = button::Button::new(50.0, 100.0, "Play".to_string(), DARKBLUE, WHITE);
    let title = text_object::TextObject::new(30.0, 40.0, "logger".to_string(), 45.0, WHITE);
    let title_screen = Menu {
        elements: vec![Box::new(button), Box::new(title)],
    };
    let player = Player::new(
        400.0,
        400.0,
        load_texture("assets/miku.png").await.unwrap(),
        WHITE,
    );

    master_state.function_stack.push(Box::new(title_screen));
    master_state.function_stack.push(Box::new(player));

    loop {
        clear_background(SKYBLUE);

        master_state.run();

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
