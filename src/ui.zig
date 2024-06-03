//
//
//

const raylib = @import("raylib");

pub const Element = union(enum) {
    button: Button,
};

pub const Button = struct {
    x: i32,
    y: i32,
    content: []const u8,
    bg: raylib.Color,
    fg: raylib.Color,

    pub fn render(self: Button) void {
        raylib.drawText(self.x, self.y, self.content, self.fg);
    }
};
