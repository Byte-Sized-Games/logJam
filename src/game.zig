const ui = @import("ui.zig");
const screen = @import("screen.zig");
const entities = @import("entities.zig");
const Beat = @import("melodie.zig").Beat;

const raylib = @import("raylib");
const std = @import("std");

pub fn mainMenu(allocator: std.mem.Allocator) !screen.Menu {
    var menu = screen.Menu{ .elements = std.ArrayList(ui.Element).init(allocator) };

    try menu.elements.append(ui.Element{ .button = ui.Button{
        .x = 40,
        .y = 200,
        .width = 150,
        .height = 50,
        .bg = raylib.Color.red,
        .fg = raylib.Color.white,
        .content = "Play!",
    } });

    try menu.elements.append(ui.Element{ .textBox = ui.TextBox{
        .x = 50,
        .y = 40,
        .colour = raylib.Color.white,
        .content = "CRAWL",
    } });

    return menu;
}

pub fn loadGame(path: [:0]const u8, allocator: std.mem.Allocator) !screen.Level {
    var game: screen.Level = undefined;

    game.complete = false;
    game.tileSet = raylib.Texture2D.init("assets/32rogues/tiles.png");
    game.monsterSet = raylib.Texture2D.init("assets/32rogues/monsters.png");
    game.itemSet = raylib.Texture2D.init("assets/32rogues/items.png");
    game.map = (try screen.Map.parse(path, allocator)).value;
    game.beat = (try Beat.parse(path, allocator)).value;
    game.player = entities.Player.init(0, 0, raylib.Texture2D.init("assets/32rogues/player.png"));
    for (game.map.tiles, 0..) |column, i| {
        for (column, 0..) |tile, j| {
            if (tile == entities.Tile.start) {
                game.player.x = j;
                game.player.y = i;
            }
        }
    }
    game.score = 0;
    return game;
}
