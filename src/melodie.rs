//
// created by Michael Ward on 27/05/24
//

use macroquad::prelude::get_time;

pub struct Track {
    pub bpm: f64,
    pub name: String,
    pub clock: f64,
    pub time: f64,
}

impl Track {
    pub fn tick(&mut self) {
        self.clock = get_time();
    }
    pub fn beat(&mut self) -> bool {
        if self.clock < (self.time + (60.0 / self.bpm)) + 0.75
            && self.clock > (self.time + (60.0 / self.bpm)) - 0.75
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
