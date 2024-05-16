//
// created on 02/04/24 by Michael Ward
//

use super::Call;
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
        if is_key_pressed(KeyCode::W) || is_key_pressed(KeyCode::Up) {
            self.y -= 50.0;
        } else if is_key_pressed(KeyCode::S) || is_key_pressed(KeyCode::Down) {
            self.y += 50.0;
        } else if is_key_pressed(KeyCode::A) || is_key_pressed(KeyCode::Left) {
            self.x -= 50.0;
        } else if is_key_pressed(KeyCode::D) || is_key_pressed(KeyCode::Right) {
            self.x += 50.0;
        }
    }
}

impl Entity for Player {
    fn render(&self) {
        draw_texture(&self.texture, self.x, self.y, self.colour);
    }
}

impl Call for Player {
    fn call_mut(&mut self) {
        self.listen();
    }
    fn call(&self) {
        self.render();
    }
}
