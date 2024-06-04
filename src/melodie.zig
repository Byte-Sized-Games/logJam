pub const Beat = struct {
    bpm: u32,
    counter: u32,
    audio: void, // TODO

    pub fn onBeat(self: Beat) bool {
        if (self.counter % (60 / self.bpm) == 0) {
            return true;
        }
        if (self.counter >= 60) {
            self.counter = 0;
        }
        return false;
    }
};
