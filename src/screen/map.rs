use crate::{entities::Tile, Call};
use macroquad::{
    color::{BROWN, SKYBLUE},
    shapes::draw_rectangle,
};
use serde::Deserialize;
use std::fs;

#[derive(Deserialize)]
pub struct Map {
    pub name: String,
    pub hi_score: u32,
    pub tiles: Vec<Vec<Tile>>,
}

impl Map {
    pub fn render(&self) {
        let mut x = 0.0;
        let mut y = 0.0;

        for column in &self.tiles {
            for tile in column {
                // Draw Tiles
                match tile {
                    Tile::Log => draw_rectangle(x, y, 100.0, 100.0, BROWN),
                    Tile::Water => draw_rectangle(x, y, 100.0, 100.0, SKYBLUE),
                }
                y += 100.0;
            }
            x += 100.0;
            y = 0.0;
        }
    }
}

impl Call for Map {
    fn call(&self) {
        self.render();
    }
}

pub fn parse_config(path: &str) -> Map {
    let config = fs::read_to_string(path).expect("Couldn't read config");
    toml::from_str(&config).unwrap()
}

pub fn check_tile(tile: &Tile) {
    match tile {
        Tile::Log => (),
        Tile::Water => println!("wet :("),
    }
}
