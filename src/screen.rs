//
// created on 02/04/24 by Michael Ward
//

use macroquad::color::{RED, WHITE};
use macroquad::prelude::draw_circle;
use macroquad::texture::Texture2D;

use crate::melodie::Track;
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
    track: Track,
    active: bool,
}

impl Level {
    pub async fn new(path: &str, player_texture: &[u8], track: Track) -> Self {
        Level {
            player: player::Player::new(
                Texture2D::from_file_with_format(player_texture, None),
                WHITE,
            ),
            map: map::parse_config(path),
            track,
            active: false,
        }
    }
}

impl Call for Level {
    fn call_mut(&mut self) -> RunCode {
        // Draw
        self.map.render();
        self.player.call();

        // Update values
        self.track.tick();
        if self.track.beat() {
            println!("Beat!");
            draw_circle(400.0, 750.0, 24.0, RED);
            self.player.listen();
        }

        return map::check_tile(&mut self.map, &mut self.player);
    }
}
