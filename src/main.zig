//
// Created on 31/05/24 by Michael Ward
//

const std = @import("std");
const raylib = @import("raylib");
const Player = @import("entities.zig").Player;
const screen = @import("screen.zig");
const ui = @import("ui.zig");
const game = @import("game.zig");

// Initialise Game Values

// Screen
const screenHeight = 800;
const screenWidth = 800;

// Allocator
var arena = std.heap.ArenaAllocator.init(std.heap.page_allocator);

pub fn main() !void {
    defer arena.deinit();
    const allocator = arena.allocator();

    // const level = try toml.decode(screen.Map, arena, levelConfig);

    // Initialise Window
    raylib.initAudioDevice();
    raylib.initWindow(screenWidth, screenHeight, "Logger");
    raylib.setTargetFPS(60);
    defer raylib.closeWindow();

    // Initialise Player
    var player = Player.init(4, 4, raylib.Texture2D.init("assets/miku.png"));

    // Initialise Main Menu
    const menu = try game.mainMenu(&allocator);

    while (!raylib.windowShouldClose()) {
        // Logic Loop
        player.listen();
        // Rendering Loop
        raylib.beginDrawing();
        defer raylib.endDrawing();
        {
            raylib.clearBackground(raylib.Color.sky_blue);
            raylib.drawText("Logger", 50, 40, 40, raylib.Color.white);
            // level.draw();
            player.render();
            menu.draw();
        }
    }
}

test "simple test" {
    var list = std.ArrayList(i32).init(std.testing.allocator);
    defer list.deinit(); // try commenting this out and see if zig detects the memory leak!
    try list.append(42);
    try std.testing.expectEqual(@as(i32, 42), list.pop());
}
