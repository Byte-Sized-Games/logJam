//
// created by Michael Ward on 01/05/24
//

// Declare modules
mod entities;
mod ui;
// ---

use macroquad::prelude::*;
use ui::Ui;

#[macroquad::main("logjam")]
async fn main() {
    let button = ui::button::Button::new(50.0, 100.0, "Play".to_string(), DARKBLUE, WHITE);
    loop {
        clear_background(DARKGRAY);

        draw_text("IT WORKS!", 20.0, 40.0, 30.0, WHITE);
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
