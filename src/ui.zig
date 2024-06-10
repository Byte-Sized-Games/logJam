//
//
//

const raylib = @import("raylib");

pub const UiError = error{
    NoElementFound,
    InvalidElement,
};

pub const Element = union(enum) {
    button: Button,
    textBox: TextBox,

    pub fn unwrap(self: Element) !Button {
        switch (self) {
            .button => |btn| return btn,
            else => return UiError.InvalidElement,
        }
        return UiError.NoElementFound;
    }
};

pub const Button = struct {
    x: i32,
    y: i32,
    height: i32,
    width: i32,
    content: [:0]const u8,
    bg: raylib.Color,
    fg: raylib.Color,

    pub fn render(self: Button) void {
        raylib.drawRectangle(self.x, self.y, self.width, self.height, self.bg);
        raylib.drawText(self.content, self.x + 10, self.y + 10, 30, self.fg);
    }

    pub fn clicked(self: Button) bool {
        if (raylib.isMouseButtonPressed(raylib.MouseButton.mouse_button_left)) {
            if (raylib.getMouseX() > self.x and raylib.getMouseX() < self.x + self.width) {
                if (raylib.getMouseY() > self.y and raylib.getMouseY() < self.y + self.height) {
                    return true;
                }
            }
        }
        return false;
    }
};

pub const TextBox = struct {
    x: i32,
    y: i32,
    content: [:0]const u8,
    colour: raylib.Color,

    pub fn render(self: TextBox) void {
        raylib.drawText(self.content, self.x, self.y, 50, self.colour);
    }
};
