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
        // Grab input
        let key_input = get_keys_down();

        // Jumping
        let move_speed = if key_input.contains(&KeyCode::Space) {
            2
        } else {
            1
        };

        // Horizontal Movement
        if key_input.contains(&KeyCode::A) || key_input.contains(&KeyCode::Left) {
            self.position.0 -= move_speed;
        } else if key_input.contains(&KeyCode::D) || key_input.contains(&KeyCode::Right) {
            self.position.0 += move_speed;
        }

        // Vertical Movement
        if key_input.contains(&KeyCode::W) || key_input.contains(&KeyCode::Up) {
            self.position.1 -= move_speed;
        } else if key_input.contains(&KeyCode::S) || key_input.contains(&KeyCode::Down) {
            self.position.1 += move_speed;
        }

        self.x = self.position.0 as f32 * 100.0 + 50.0;
        self.y = self.position.1 as f32 * 100.0 + 50.0;
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
    fn call_mut(&mut self) -> crate::RunCode {
        self.listen();
        crate::RunCode::Ok
    }
    fn call(&self) {
        self.render();
    }
}
