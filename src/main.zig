//
// Created on 31/05/24 by Michael Ward
//

const std = @import("std");
const raylib = @import("raylib");
const entities = @import("entities.zig");
const screen = @import("screen.zig");
const ui = @import("ui.zig");
const game = @import("game.zig");
const melodie = @import("melodie.zig");

// Initialise Game Values

// Screen
const screenHeight = 640;
const screenWidth = 640;

// Types

pub const Scenes = enum {
    MainMenu,
    Level,
};

// Allocator(s)
const pager = std.heap.page_allocator;
var arena = std.heap.ArenaAllocator.init(pager);

pub fn main() !void {
    defer arena.deinit();
    const allocator = arena.allocator();

    // Initialise Window
    raylib.initAudioDevice();
    raylib.initWindow(screenWidth, screenHeight, "Logger");
    raylib.setTargetFPS(60);
    defer raylib.closeWindow();

    // Initialise Game
    var level = try game.loadGame("test", allocator);
    // Initialise Main Menu
    var menu = try game.mainMenu(allocator);
    var currentScene = Scenes.MainMenu;

    defer {
        level.deinit();
        menu.deinit();
    }

    while (!raylib.windowShouldClose()) {

        // Rendering Loop
        raylib.beginDrawing();
        defer raylib.endDrawing();
        {
            raylib.clearBackground(raylib.Color.dark_gray);

            switch (currentScene) {
                Scenes.MainMenu => {
                    menu.draw();

                    if ((try menu.elements.items[0].unwrap()).clicked()) {
                        currentScene = Scenes.Level;
                    }

                    if ((try menu.elements.items[1].unwrap()).clicked()) {
                        currentScene = Scenes.Credits;
                    }
                },
                else => {
                    currentScene = try level.tick(allocator);
                },
            }
        }
    }
}

test "simple test" {
    var list = std.ArrayList(i32).init(std.testing.allocator);
    defer list.deinit(); // try commenting this out and see if zig detects the memory leak!
    try list.append(42);
    try std.testing.expectEqual(@as(i32, 42), list.pop());
}

test "read files" {
    const testLevel = try screen.Map.parse("testLevel.json", std.testing.allocator);
    defer testLevel.deinit();

    try std.testing.expectEqual(entities.Tile.start, testLevel.value.tiles[0][0]);
}
