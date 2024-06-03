//
//
//

const raylib = @import("raylib");

pub const button = struct {
    x: i32,
    y: i32,
    content: []const u8,
    bg: raylib.Color,
    fg: raylib.Color,

    pub fn render(self: button) void {
        raylib.drawText(self.x, self.y, self.content, self.fg);
    }
};
