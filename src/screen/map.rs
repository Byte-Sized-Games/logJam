use crate::{entities::Tile, Call};
use macroquad::{
    color::{BEIGE, BROWN, SKYBLUE},
    shapes::{draw_rectangle, draw_rectangle_lines},
    window::{screen_height, screen_width},
};
use serde::Deserialize;
use std::fs;

#[derive(Deserialize)]
pub struct Map {
    pub name: String,
    pub hi_score: u32,
    pub width: u32,
    pub height: u32,
    pub tiles: Vec<Vec<Tile>>,
}

impl Map {
    pub fn new(name: &str, hi_score: u32, width: u32, height: u32) -> Self {
        Map {
            name: name.to_owned(),
            hi_score,
            width,
            height,
            tiles: vec![vec![]],
        }
    }
    pub fn render(&self) {
        let mut x = screen_width() - self.width as f32;
        let mut y = screen_height() - self.height as f32;

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
            y = screen_height() - self.height as f32;
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
