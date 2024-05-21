//
// created on 02/04/24 by Michael Ward
//

use serde::Deserialize;

pub mod player;

pub trait Entity {
    fn render(&self);
}

#[derive(Deserialize)]

pub enum Tile {
    Water,
    Log,
}
