//
// created on 02/04/24 by Michael Ward
//

use macroquad::color::WHITE;
use macroquad::texture::load_texture;

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
    pub async fn new(path: &str) -> Self {
        Level {
            player: player::Player::new(
                0.0,
                0.0,
                load_texture("assets/miku.png").await.unwrap(),
                WHITE,
            ),
            map: map::parse_config(path),
        }
    }
}

impl Call for Level {
    fn call_mut(&mut self) {
        self.map.call();
        self.player.call_mut();
    }
}
