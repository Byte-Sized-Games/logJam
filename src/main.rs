//
// created by Michael Ward on 01/05/24
//

use macroquad::prelude::*;

#[macroquad::main("logjam")]
async fn main() {
    loop {
        clear_background(DARKGRAY);

        draw_text("IT WORKS!", 20.0, 40.0, 30.0, WHITE);

        next_frame().await
    }
}
