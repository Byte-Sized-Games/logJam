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

// Constants
const WINDOW_HEIGHT: u32 = 800;
const WINDOW_WIDTH: u32 = 800;
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
