const ui = @import("ui.zig");
const screen = @import("screen.zig");
const entities = @import("entities.zig");
const map = @import("map.zig");
const Beat = @import("melodie.zig").Beat;
const Scenes = @import("main.zig").Scenes;

const raylib = @import("raylib");
const std = @import("std");

pub fn mainMenu(allocator: std.mem.Allocator) !screen.Menu {
    var menu = screen.Menu{
        .elements = std.ArrayList(ui.Element).init(allocator),
        .background = raylib.Texture2D.init("assets/mainMenu.png"),
    };

    try menu.elements.append(ui.Element{ .button = ui.Button{
        .x = 50,
        .y = 200,
        .width = 150,
        .height = 50,
        .bg = raylib.Color.red,
        .fg = raylib.Color.white,
        .content = "Play!",
    } });

    try menu.elements.append(ui.Element{ .button = ui.Button{
        .x = 50,
        .y = 300,
        .width = 150,
        .height = 50,
        .bg = raylib.Color.gray,
        .fg = raylib.Color.white,
        .content = "Credits",
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
    // Initialise Level
    var game: screen.Level = undefined;
    game.complete = false;
    game.path = path;
    // Load Tilesets
    game.tileSet = raylib.Texture2D.init("assets/32rogues/tiles.png");
    game.monsterSet = raylib.Texture2D.init("assets/32rogues/monsters.png");
    game.itemSet = raylib.Texture2D.init("assets/32rogues/items.png");
    game.winScreen = raylib.Texture2D.init("assets/win.png");
    // Load Map
    game.map = map.Map.init(try map.RawMap.parse(path, allocator));
    // Load Music
    game.beat = (try Beat.parse(path, allocator)).value;

    // Get audio path
    var audioPath = try allocator.allocSentinel(u8, path.len + "/audio.wav".len, 0);
    defer allocator.free(audioPath);
    std.mem.copyForwards(u8, audioPath[0..], path);
    std.mem.copyForwards(u8, audioPath[path.len..], "/audio.wav");
    const audio: [:0]u8 = audioPath;
    game.song = raylib.loadSound(audio);
    // Initialise player
    game.player = entities.Player.init(0, 0, raylib.Texture2D.init("assets/32rogues/player.png"));
    for (game.map.val.tiles, 0..) |column, i| {
        for (column, 0..) |tile, j| {
            if (tile == entities.Tile.start) {
                game.player.x = j;
                game.player.y = i;
            }
        }
    }
    game.score = 0;
    game.moves = 0;
    return game;
}
