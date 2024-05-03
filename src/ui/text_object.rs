//
// created on 02/04/24 by Michael Ward
//

use macroquad::prelude::*;

use super::Ui;

pub struct TextObject {
    x: f32,
    y: f32,
    content: String,
    font_size: f32,
    colour: Color,
}

impl TextObject {
    pub fn new(x: f32, y: f32, content: String, font_size: f32, colour: Color) -> TextObject {
        TextObject {
            x,
            y,
            content,
            font_size,
            colour,
        }
    }
}

impl Ui for TextObject {
    fn render(&self) {
        draw_text(&self.content, self.x, self.y, self.font_size, self.colour);
    }
}
