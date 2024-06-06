//
//
//

const ui = @import("ui.zig");
const entities = @import("entities.zig");
const melodie = @import("melodie.zig");
const raylib = @import("raylib");
const std = @import("std");
const ArrayList = std.ArrayList;

// Types

pub const MapError = error{
    InvalidPath,
    InvalidAccess,
    FileNotFound,
};

// Structures

pub const Level = struct {
    player: entities.Player,
    map: Map,
    tileSet: raylib.Texture2D,
    beat: melodie.Beat,
    score: f32,

    pub fn tick(self: *Level) void {
        // Tick variables
        self.beat.counter += 1;
        self.score += self.player.listen(&self.beat) * 100;

        // Render content
        self.map.draw(self.tileSet);
        self.player.render();

        if (self.beat.onBeat() >= 0.75) {
            raylib.drawCircle(400, 750, 15.0, raylib.Color.red);
        }

        // Render Score

        raylib.drawText(raylib.textFormat("Score: %.1f", .{self.score}), 550, 50, 35, raylib.Color.white);
    }
};

pub const Map = struct {
    tiles: [][]entities.Tile,
    par: u32,

    pub fn draw(self: *Map, tileSet: raylib.Texture2D) void {
        var x: f32 = 0;
        var y: f32 = 0;
        for (self.tiles) |column| {
            for (column) |tile| {
                tileSet.drawPro(
                    raylib.Rectangle.init(0, 192, 32, 32),
                    raylib.Rectangle.init(0, 0, 100, 100),
                    raylib.Vector2.init(x, y),
                    0,
                    raylib.Color.white,
                );
                switch (tile) {
                    entities.Tile.start => tileSet.drawPro(
                        raylib.Rectangle.init(256, 512, 32, 32),
                        raylib.Rectangle.init(0, 0, 100, 100),
                        raylib.Vector2.init(x, y),
                        0,
                        raylib.Color.white,
                    ),
                    entities.Tile.log => tileSet.drawPro(
                        raylib.Rectangle.init(64, 192, 32, 32),
                        raylib.Rectangle.init(0, 0, 100, 100),
                        raylib.Vector2.init(x, y),
                        0,
                        raylib.Color.white,
                    ),
                    entities.Tile.water => tileSet.drawPro(
                        raylib.Rectangle.init(0, 96, 32, 32),
                        raylib.Rectangle.init(0, 0, 100, 100),
                        raylib.Vector2.init(x, y),
                        0,
                        raylib.Color.white,
                    ),
                    entities.Tile.end => tileSet.drawPro(
                        raylib.Rectangle.init(224, 512, 32, 32),
                        raylib.Rectangle.init(0, 0, 100, 100),
                        raylib.Vector2.init(x, y),
                        0,
                        raylib.Color.white,
                    ),
                }
                x -= 100;
            }
            y -= 100;
            x = 0;
        }
    }

    pub fn parse(path: [:0]const u8, allocator: std.mem.Allocator) !std.json.Parsed(Map) {
        var levelPath = try allocator.alloc(u8, path.len + "/level.json".len);
        defer allocator.free(levelPath);
        std.mem.copyForwards(u8, levelPath[0..], path);
        std.mem.copyForwards(u8, levelPath[path.len..], "/level.json");
        const file = try std.fs.cwd().readFileAlloc(allocator, levelPath, 2048);
        defer allocator.free(file);
        const parsedData = try std.json.parseFromSlice(Map, allocator, file, .{ .allocate = .alloc_always });

        return parsedData;
    }
};

pub const Menu = struct {
    elements: ArrayList(ui.Element),

    pub fn draw(self: Menu) void {
        for (self.elements.items) |element| {
            switch (element) {
                .button => |btn| {
                    btn.render();
                },
                .textBox => |txt| {
                    txt.render();
                },
            }
        }
    }

    pub fn deinit(self: *Menu) void {
        self.elements.deinit();
    }
};
