//
// created by Michael Ward on 01/05/24
//

// Declare modules
mod entities;
mod screen;
mod ui;
// ---

use macroquad::prelude::*;
use ui::*;

#[macroquad::main("logjam")]
async fn main() {
    let button = button::Button::new(50.0, 100.0, "Play".to_string(), DARKBLUE, WHITE);
    let title = text_object::TextObject::new(30.0, 40.0, "logger".to_string(), 45.0, WHITE);
    loop {
        clear_background(SKYBLUE);

        title.render();
        button.render();

        // Get button clicked
        if button.interacted() {
            println!("Pressed!");
        } else {
            println!("Not pressed!");
        }

        next_frame().await
    }
}
