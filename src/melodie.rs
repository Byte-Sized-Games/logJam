//
// created by Michael Ward on 27/05/24
//

use crate::entities::player;
use macroquad::prelude::{get_frame_time, get_time};

pub struct Track {
    pub bpm: u32,
    pub name: String,
    pub clock: f64,
    pub time: f64,
}

impl Track {
    pub fn tick(&mut self) {
        self.clock = get_time();
    }
    pub fn beat(&mut self) -> bool {
        if self.clock < (self.time + (60.0 / self.bpm as f64)) + 1.0
            && self.clock > (self.time + (60.0 / self.bpm as f64)) - 1.0
        {
            self.time = get_time();
            return true;
        }
        false
    }
}

pub trait BeatObject {
    fn on_beat(t: Track) -> bool {
        if t.clock >= (t.time + 60.0) / t.bpm as f64 {
            return true;
        }
        false
    }
}
