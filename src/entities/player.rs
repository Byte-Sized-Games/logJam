//
// created on 16/05/24 by Michael Ward
//

use super::Entity;
use crate::Call;
use macroquad::prelude::*;

pub struct Player {
    pub position: (usize, usize),
    pub x: f32,
    pub y: f32,
    pub texture: Texture2D,
    pub colour: Color,
}

impl Player {
    pub fn new(texture: Texture2D, colour: Color) -> Self {
        Player {
            position: (0, 0),
            x: 50.0,
            y: 50.0,
            texture,
            colour,
        }
    }

    pub fn listen(&mut self) {
        // Special Movement
        if is_key_down(KeyCode::Space) {
            // Special Movement
            if is_key_pressed(KeyCode::W) || is_key_pressed(KeyCode::Up) {
                self.y -= 200.0;
                self.position.1 -= 2;
            } else if is_key_pressed(KeyCode::S) || is_key_pressed(KeyCode::Down) {
                self.y += 200.0;
                self.position.1 += 2;
            } else if is_key_pressed(KeyCode::A) || is_key_pressed(KeyCode::Left) {
                self.x -= 200.0;
                self.position.0 -= 2;
            } else if is_key_pressed(KeyCode::D) || is_key_pressed(KeyCode::Right) {
                self.x += 200.0;
                self.position.0 += 2;
            }
        }
        // Movement
        else if is_key_pressed(KeyCode::W) || is_key_pressed(KeyCode::Up) {
            self.y -= 100.0;
            self.position.1 -= 1;
        } else if is_key_pressed(KeyCode::S) || is_key_pressed(KeyCode::Down) {
            self.y += 100.0;
            self.position.1 += 1;
        } else if is_key_pressed(KeyCode::A) || is_key_pressed(KeyCode::Left) {
            self.x -= 100.0;
            self.position.0 -= 1;
        } else if is_key_pressed(KeyCode::D) || is_key_pressed(KeyCode::Right) {
            self.x += 100.0;
            self.position.0 += 1;
        }
    }
}

impl Entity for Player {
    fn render(&self) {
        draw_texture(
            &self.texture,
            self.x - (self.texture.width() / 2.0),
            self.y - (self.texture.height() / 2.0),
            self.colour,
        );
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
