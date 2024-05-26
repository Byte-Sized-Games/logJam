//
// created on 02/04/24 by Michael Ward
//

use macroquad::color::WHITE;
use macroquad::texture::Texture2D;

use crate::RunCode;

// Namespaces
use super::entities::*;
use super::ui;
use super::Call;
// ---

pub mod map;

/// Bundled Collection of UI elements
pub struct Menu<'m> {
    pub elements: Vec<Box<dyn ui::Ui + 'm>>,
}

impl Call for Menu<'_> {
    /// Render elements contained in menu
    fn call(&self) {
        for element in &self.elements {
            element.render();
        }
    }
}

/// Bundled collection of game elements.
/// Used to map out level layout
pub struct Level {
    player: player::Player,
    map: map::Map,
}

impl Level {
    pub async fn new(path: &str, player_texture: &[u8]) -> Self {
        Level {
            player: player::Player::new(
                Texture2D::from_file_with_format(player_texture, None),
                WHITE,
            ),
            map: map::parse_config(path),
        }
    }
}

impl Call for Level {
    fn call_mut(&mut self) -> RunCode {
        // Update values
        self.player.call_mut();
        // Draw
        self.map.call();
        self.player.call();

        return map::check_tile(&self.map.tiles[self.player.position.0][self.player.position.1], &mut self.player);
    }
}
