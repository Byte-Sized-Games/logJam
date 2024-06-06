const raylib = @import("raylib");
const melodie = @import("melodie.zig");

pub const PlayerResult = enum {
    Safe,
    Dead,
    Won,
};

pub const Player = struct {
    x: i32,
    y: i32,
    texture: raylib.Texture2D,
    rect: raylib.Rectangle,

    pub fn render(self: Player) void {
        const posX: f32 = @floatFromInt(self.x);
        const posY: f32 = @floatFromInt(self.y);
        self.texture.drawEx(
            raylib.Vector2.init(posX * 100, posY * 100),
            0,
            3.125,
            raylib.Color.white,
        );
    }

    pub fn listen(self: *Player, beat: *melodie.Beat) f32 {
        var moveSpeed: i32 = 1;

        if (raylib.isKeyDown(raylib.KeyboardKey.key_space)) {
            moveSpeed = 2;
        }

        switch (raylib.getKeyPressed()) {
            raylib.KeyboardKey.key_w, raylib.KeyboardKey.key_up => {
                self.y -= moveSpeed;
                return beat.onBeat();
            },
            raylib.KeyboardKey.key_s, raylib.KeyboardKey.key_down => {
                self.y += moveSpeed;
                return beat.onBeat();
            },
            raylib.KeyboardKey.key_a, raylib.KeyboardKey.key_left => {
                self.x -= moveSpeed;
                return beat.onBeat();
            },
            raylib.KeyboardKey.key_d, raylib.KeyboardKey.key_right => {
                self.x += moveSpeed;
                return beat.onBeat();
            },
            else => {},
        }
        return 0.0;
    }

    pub fn init(x: i32, y: i32, texture: raylib.Texture2D) Player {
        const textureHeight: f32 = @floatFromInt(texture.height);
        const textureWidth: f32 = @floatFromInt(texture.width);
        return Player{
            .x = x,
            .y = y,
            .rect = raylib.Rectangle.init(0, 0, textureWidth / 5, textureHeight / 5),
            .texture = texture,
        };
    }

    pub fn deinit(self: *Player) void {
        raylib.unloadTexture(self.texture);
    }
};

pub const Tile = enum {
    start,
    log,
    water,
    end,
};

pub fn checkTile(player: Player, tile: [][]Tile) PlayerResult {
    switch (tile[player.x][player.y]) {
        Tile.start, Tile.log => {},
        Tile.water => {
            return PlayerResult.Dead;
        },
        Tile.end => {
            return PlayerResult.Won;
        },
    }
    return PlayerResult.Safe;
}
