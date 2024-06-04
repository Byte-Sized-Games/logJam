const ui = @import("ui.zig");
const screen = @import("screen.zig");

const raylib = @import("raylib");
const std = @import("std");

pub fn mainMenu(allocator: *const std.mem.Allocator) !screen.Menu {
    var menu = screen.Menu{ .elements = std.ArrayList(ui.Element).init(allocator.*) };

    try menu.elements.append(ui.Element{ .button = ui.Button{ .x = 40, .y = 200, .width = 150, .height = 50, .bg = raylib.Color.red, .fg = raylib.Color.white, .content = "Play!" } });

    return menu;
}
