//
// created on 02/04/24 by Michael Ward
//

use super::runtime::Call;
use macroquad::prelude::*;

pub trait Entity {
    fn render(&self);
}

pub struct Player {
    pub x: f32,
    pub y: f32,
    pub texture: Texture2D,
    pub colour: Color,
}

impl Player {
    pub fn new(x: f32, y: f32, texture: Texture2D, colour: Color) -> Self {
        Player {
            x,
            y,
            texture,
            colour,
        }
    }

    pub fn listen(&mut self) {
        // Vertical movement
        if is_key_down(KeyCode::W) || is_key_down(KeyCode::Up) {
            self.y -= 2.0;
        } else if is_key_down(KeyCode::S) || is_key_down(KeyCode::Down) {
            self.y += 2.0;
        }
        // Horizontal movement
        if is_key_down(KeyCode::A) || is_key_down(KeyCode::Left) {
            self.x -= 2.0;
        } else if is_key_down(KeyCode::D) || is_key_down(KeyCode::Right) {
            self.x += 2.0;
        }
    }
}

impl Entity for Player {
    fn render(&self) {
        draw_texture(&self.texture, self.x, self.y, self.colour);
    }
}

impl Call for Player {
    fn call(&mut self) {
        self.listen();
        self.render();
    }
}
