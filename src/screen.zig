//
//
//

const ui = @import("ui.zig");
const entities = @import("entities.zig");
const melodie = @import("melodie.zig");
const raylib = @import("raylib");
const std = @import("std");
const ArrayList = std.ArrayList;

pub const Level = struct {
    player: *entities.Player,
    map: Map,
    beat: melodie.Beat,
};

pub const Map = struct {
    tiles: [][]entities.Tile,

    pub fn draw(self: *Map) void {
        var x = 0;
        var y = 0;
        for (self.tiles) |column| {
            for (column) |tile| {
                switch (tile) {
                    entities.Tile.start => raylib.drawRectangle(x, y, 100, 100, raylib.Color.green),
                    entities.Tile.log => raylib.drawRectangle(x, y, 100, 100, raylib.Color.brown),
                    entities.Tile.water => raylib.drawRectangle(x, y, 100, 100, raylib.Color.sky_blue),
                    entities.Tile.end => raylib.drawRectangle(x, y, 100, 100, raylib.Color.red),
                    else => {},
                }
                y += 100;
            }
            x += 100;
        }
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
