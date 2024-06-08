//
//
//

const ui = @import("ui.zig");
const Scene = @import("main.zig").Scenes;
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
    path: [:0]const u8,
    player: entities.Player,
    map: Map,
    tileSet: raylib.Texture2D,
    itemSet: raylib.Texture2D,
    monsterSet: raylib.Texture2D,
    beat: melodie.Beat,
    song: raylib.Sound,
    score: f32,
    moves: u32,
    complete: bool,
    winScreen: raylib.Texture2D,

    /// Batch update all items in a level
    pub fn tick(self: *Level, allocator: std.mem.Allocator) !Scene {
        // Did the player beat the level?
        if (self.complete) {
            const returnButton = ui.Button{
                .x = 250,
                .y = 550,
                .bg = raylib.Color.green,
                .fg = raylib.Color.white,
                .width = 175,
                .height = 40,
                .content = "Main Menu?",
            };
            // Draw Victory Screen
            raylib.clearBackground(raylib.Color.black);
            self.winScreen.draw(0, 0, raylib.Color.white);
            // Victory text!
            raylib.drawText("Victory!", 50, 250, 60, raylib.Color.gold);
            raylib.drawText(
                raylib.textFormat("Moves: %d / %d", .{ self.moves, self.map.par }),
                50,
                315,
                60,
                raylib.Color.orange,
            );
            raylib.drawText(
                raylib.textFormat("Beat Score: %.1f", .{self.score}),
                50,
                375,
                60,
                raylib.Color.pink,
            );
            // Compute final score
            const ratio: f32 = @floatFromInt(self.moves / self.map.par);
            var finalScore: f32 = self.score / ratio;
            finalScore = @min(9999, finalScore);
            raylib.drawText(raylib.textFormat("Final Score: %.0f", .{finalScore}), 50, 430, 60, raylib.Color.lime);
            returnButton.render();

            // Reset level and return to main menu
            if (returnButton.clicked()) {
                // Reset level values
                self.complete = false;
                self.player.armed = false;
                self.score = 0;
                self.moves = 0;
                // Stop the music
                raylib.stopSound(self.song);
                // Reset level & enemies
                self.map = (try Map.parse(self.path, allocator)).value;

                // Load main menu
                return Scene.MainMenu;
            }
        }
        // Update the game otherwise!
        else {
            // Music
            if (!raylib.isSoundPlaying(self.song)) {
                raylib.playSound(self.song);
            }

            // Tick variables
            self.beat.counter += 1;
            const accuracy = self.player.listen(&self.beat, self);
            self.player.checkTile(&self.map, self);
            if (accuracy == -1.0) {
                self.score -= 0.01;
            } else if (accuracy < 0.35) {
                self.score -= 100;
            } else {
                self.score += accuracy * 100;
            }

            // Render content
            self.map.draw(self.*);
            self.player.render();

            if (self.beat.onBeat() >= 0.75) {
                raylib.drawCircle(320, 550, 15.0, raylib.Color.red);
            }

            // Render Score
            raylib.drawText(raylib.textFormat("Score: %.1f", .{self.score}), 350, 50, 25, raylib.Color.white);
        }
        return Scene.Level;
    }
};

pub const Map = struct {
    tiles: [][]entities.Tile, // Backdrop
    dungeon: [][]entities.Dungeon, // Game entities
    par: u32, // Try to keep your moves under this!

    /// Draws the map!
    /// Cycles through given arrays and draws tiles from the tilemap
    pub fn draw(self: *Map, level: Level) void {
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
    pub fn parse(path: [:0]const u8, allocator: std.mem.Allocator) !std.json.Parsed(Map) {
        // Load level path
        var levelPath = try allocator.alloc(u8, path.len + "/level.json".len);
        defer allocator.free(levelPath);
        std.mem.copyForwards(u8, levelPath[0..], path);
        std.mem.copyForwards(u8, levelPath[path.len..], "/level.json");
        // Load file to text
        const file = try std.fs.cwd().readFileAlloc(allocator, levelPath, 2048);
        defer allocator.free(file);
        // Attempt to parse the data
        const parsedData = try std.json.parseFromSlice(Map, allocator, file, .{ .allocate = .alloc_always });

        return parsedData;
    }
};

pub const Menu = struct {
    elements: ArrayList(ui.Element),
    background: raylib.Texture2D,

    pub fn draw(self: Menu) void {
        self.background.draw(0, 0, raylib.Color.white);
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
