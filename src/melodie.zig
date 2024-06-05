const std = @import("std");

pub const Beat = struct {
    bpm: f32,
    counter: f32,
    name: [:0]const u8,

    pub fn onBeat(self: *Beat) f32 {
        // Get time of click
        const time = self.counter;
        if (self.counter >= 3600.0) {
            self.counter = 0.0;
        }
        // Calculate perfect beat
        const idealBeat: f32 = 3600.0 / self.bpm;

        // Calculate diff between move and beat
        const nearestBeat = std.math.floor(time / idealBeat) * idealBeat;
        const diff = @abs(time - nearestBeat);

        // Normalize the diff and invert
        const normalizedDiff = diff / idealBeat;
        const closeness: f32 = @min(1.0, 1.0 - normalizedDiff);

        return closeness;
    }

    pub fn parse(path: [:0]const u8, allocator: std.mem.Allocator) !std.json.Parsed(Beat) {
        var levelPath = try allocator.alloc(u8, path.len + "/sound.json".len);
        defer allocator.free(levelPath);
        std.mem.copyForwards(u8, levelPath[0..], path);
        std.mem.copyForwards(u8, levelPath[path.len..], "/sound.json");
        const file = try std.fs.cwd().readFileAlloc(allocator, levelPath, 2048);
        defer allocator.free(file);
        const parsedData = try std.json.parseFromSlice(Beat, allocator, file, .{ .allocate = .alloc_always });

        return parsedData;
    }
};
