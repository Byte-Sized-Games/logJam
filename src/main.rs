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
use ui::prelude::*;
// ---

// Constants
const WINDOW_HEIGHT: u16 = 800;
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

    let mut master_state = runtime::Scene {
        function_stack: vec![],
        // draw_stack: vec![],
    };
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
