//
//
//

const ui = @import("ui.zig");
const Scene = @import("main.zig").Scenes;
const entities = @import("entities.zig");
const melodie = @import("melodie.zig");
const Map = @import("map.zig").Map;
const RawMap = @import("map.zig").RawMap;
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
                raylib.textFormat("Moves: %d / %d", .{ self.moves, self.map.val.par }),
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
            const ratio: f32 = @floatFromInt(self.moves / self.map.val.par);
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
                try self.map.reload(self.path, allocator);

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

            // Reset if needed
            if (raylib.isKeyDown(raylib.KeyboardKey.key_r)) {
                for (self.map.val.tiles, 0..) |column, i| {
                    for (column, 0..) |tile, j| {
                        if (tile == entities.Tile.start) {
                            self.player.x = j;
                            self.player.y = i;
                        }
                    }
                }
                self.complete = false;
                self.player.armed = false;
                self.score = 0;
                self.moves = 0;
                // Stop the music
                raylib.stopSound(self.song);
                // Reset level & enemies
                try self.map.reload(self.path, allocator);
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
            self.map.val.draw(self.*);
            self.player.render();

            if (self.beat.onBeat() >= 0.75) {
                raylib.drawCircle(320, 550, 15.0, raylib.Color.red);
            }

            // Render Score
            raylib.drawText(raylib.textFormat("Score: %.1f", .{self.score}), 350, 50, 25, raylib.Color.white);
        }
        return Scene.Level;
    }

    pub fn deinit(self: *Level) void {
        self.player.deinit();
        raylib.unloadTexture(self.winScreen);
        raylib.unloadTexture(self.itemSet);
        raylib.unloadTexture(self.monsterSet);
        raylib.unloadTexture(self.tileSet);
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
        raylib.unloadTexture(self.background);
    }
};
