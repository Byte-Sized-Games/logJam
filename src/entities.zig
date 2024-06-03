const raylib = @import("raylib");

pub const Player = struct {
    x: i32,
    y: i32,
    texture: raylib.Texture2D,

    pub fn render(self: Player) void {
        const posX = 100 * self.x + 50;
        const posY = 100 * self.y + 50;
        raylib.drawTexture(self.texture, posX, posY, raylib.Color.white);
    }

    pub fn listen(self: *Player) void {
        var moveSpeed: i32 = 1;

        if (raylib.isKeyDown(raylib.KeyboardKey.key_space)) {
            moveSpeed = 2;
        }

        switch (raylib.getKeyPressed()) {
            raylib.KeyboardKey.key_w, raylib.KeyboardKey.key_up => self.y -= moveSpeed,
            raylib.KeyboardKey.key_s, raylib.KeyboardKey.key_down => self.y += moveSpeed,
            raylib.KeyboardKey.key_a, raylib.KeyboardKey.key_left => self.x -= moveSpeed,
            raylib.KeyboardKey.key_d, raylib.KeyboardKey.key_right => self.x += moveSpeed,
            else => {},
        }
    }

    pub fn init(x: i32, y: i32, texture: raylib.Texture2D) Player {
        return Player{
            .x = x,
            .y = y,
            .texture = texture,
        };
    }

    pub fn deinit(self: *Player) void {
        raylib.unloadTexture(self.texture);
    }
};
