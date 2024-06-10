const entities = @import("entities.zig");
const Level = @import("screen.zig").Level;
const std = @import("std");
const raylib = @import("raylib");

pub const Map = struct {
    raw: std.json.Parsed(RawMap), // Raw map data. Never accessed
    val: RawMap, // Accessor for mutating

    pub fn reload(self: *Map, path: [:0]const u8, allocator: std.mem.Allocator) !void {
        self.raw = try RawMap.parse(path, allocator);
        self.val = self.raw.value;
    }

    pub fn init(r: std.json.Parsed(RawMap)) Map {
        var newMap: Map = undefined;

        newMap.raw = r;
        newMap.val = newMap.raw.value;

        return newMap;
    }

    pub fn deinit(self: *Map) void {
        self.map = null;
        self.raw.deinit();
        self.copy.deinit();
    }
};

pub const RawMap = struct {
    tiles: [][]entities.Tile, // Backdrop
    dungeon: [][]entities.Dungeon, // Game entities
    par: u32, // Try to keep your moves under this!

    /// Draws the map!
    /// Cycles through given arrays and draws tiles from the tilemap
    pub fn draw(self: *RawMap, level: Level) void {
        var x: f32 = 0;
        var y: f32 = 0;
        // Draw Background
        for (self.tiles) |column| {
            for (column) |tile| {
                level.tileSet.drawPro(
                    raylib.Rectangle.init(0, 192, 32, 32),
                    raylib.Rectangle.init(0, 0, 64, 64),
                    raylib.Vector2.init(x, y),
                    0,
                    raylib.Color.white,
                );
                switch (tile) {
                    entities.Tile.start => level.tileSet.drawPro(
                        raylib.Rectangle.init(256, 512, 32, 32),
                        raylib.Rectangle.init(0, 0, 64, 64),
                        raylib.Vector2.init(x, y),
                        0,
                        raylib.Color.white,
                    ),
                    entities.Tile.floor => level.tileSet.drawPro(
                        raylib.Rectangle.init(64, 192, 32, 32),
                        raylib.Rectangle.init(0, 0, 64, 64),
                        raylib.Vector2.init(x, y),
                        0,
                        raylib.Color.white,
                    ),
                    entities.Tile.wall => level.tileSet.drawPro(
                        raylib.Rectangle.init(0, 96, 32, 32),
                        raylib.Rectangle.init(0, 0, 64, 64),
                        raylib.Vector2.init(x, y),
                        0,
                        raylib.Color.white,
                    ),
                    entities.Tile.end => level.tileSet.drawPro(
                        raylib.Rectangle.init(224, 512, 32, 32),
                        raylib.Rectangle.init(0, 0, 64, 64),
                        raylib.Vector2.init(x, y),
                        0,
                        raylib.Color.white,
                    ),
                }
                x -= 64;
            }
            x = 0;
            y -= 64;
        }
        // Draw Item & Monsters
        y = 0;
        for (self.dungeon) |column| {
            for (column) |dgn| {
                switch (dgn) {
                    entities.Dungeon.empty => {},
                    entities.Dungeon.monster => level.monsterSet.drawPro(
                        raylib.Rectangle.init(32, 224, 32, 32),
                        raylib.Rectangle.init(0, 0, 64, 64),
                        raylib.Vector2.init(x, y),
                        0,
                        raylib.Color.white,
                    ),
                    entities.Dungeon.item => level.itemSet.drawPro(
                        raylib.Rectangle.init(128, 32, 32, 32),
                        raylib.Rectangle.init(0, 0, 64, 64),
                        raylib.Vector2.init(x, y),
                        0,
                        raylib.Color.white,
                    ),
                    entities.Dungeon.key => level.itemSet.drawPro(
                        raylib.Rectangle.init(32, 512, 32, 32),
                        raylib.Rectangle.init(0, 0, 64, 64),
                        raylib.Vector2.init(x, y),
                        0,
                        raylib.Color.white,
                    ),
                    entities.Dungeon.door => level.tileSet.drawPro(
                        raylib.Rectangle.init(64, 512, 32, 32),
                        raylib.Rectangle.init(0, 0, 64, 64),
                        raylib.Vector2.init(x, y),
                        0,
                        raylib.Color.white,
                    ),
                    entities.Dungeon.doorOpen => level.tileSet.drawPro(
                        raylib.Rectangle.init(96, 512, 32, 32),
                        raylib.Rectangle.init(0, 0, 64, 64),
                        raylib.Vector2.init(x, y),
                        0,
                        raylib.Color.white,
                    ),
                    // else => {},
                }
                x -= 64;
            }
            y -= 64;
            x = 0;
        }
    }

    /// Parses a map from a json file
    pub fn parse(path: [:0]const u8, allocator: std.mem.Allocator) !std.json.Parsed(RawMap) {
        // Load level path
        var levelPath = try allocator.alloc(u8, path.len + "/level.json".len);
        defer allocator.free(levelPath);
        std.mem.copyForwards(u8, levelPath[0..], path);
        std.mem.copyForwards(u8, levelPath[path.len..], "/level.json");
        // Load file to text
        const file = try std.fs.cwd().readFileAlloc(allocator, levelPath, 2048);
        defer allocator.free(file);
        // Attempt to parse the data
        const parsedData = try std.json.parseFromSlice(RawMap, allocator, file, .{ .allocate = .alloc_always });

        return parsedData;
    }
};
