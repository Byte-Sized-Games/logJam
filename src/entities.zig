const raylib = @import("raylib");
const melodie = @import("melodie.zig");
const Map = @import("screen.zig").Map;
const Level = @import("screen.zig").Level;

pub const PlayerResult = enum {
    Safe,
    Dead,
    Won,
};

pub const Player = struct {
    x: usize,
    y: usize,
    texture: raylib.Texture2D,
    rect: raylib.Rectangle,
    armed: bool,

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
        var moveSpeed: usize = 1;

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
        return -1.0;
    }

    pub fn checkTile(player: *Player, map: *Map, level: *Level) void {
        // Check players position based on tiles
        switch (map.tiles[player.y][player.x]) {
            Tile.start, Tile.log => {},
            Tile.water => {
                for (map.tiles, 0..) |column, i| {
                    for (column, 0..) |tile, j| {
                        if (tile == Tile.start) {
                            player.x = j;
                            player.y = i;
                        }
                    }
                }
            },
            Tile.end => {
                level.complete = true;
                for (map.tiles, 0..) |column, i| {
                    for (column, 0..) |tile, j| {
                        if (tile == Tile.start) {
                            player.x = j;
                            player.y = i;
                        }
                    }
                }
            },
        }
        // Check player position based on enemies
        switch (map.dungeon[player.y][player.x]) {
            Dungeon.empty => {},
            Dungeon.item => {
                if (player.armed) {} else {
                    player.armed = true;
                    map.dungeon[player.y][player.x] = Dungeon.empty;
                }
            },
            Dungeon.monster => {
                if (player.armed) {
                    map.dungeon[player.y][player.x] = Dungeon.empty;
                    player.armed = false;
                    level.score += 200;
                } else {
                    level.score -= 200;
                    for (map.tiles, 0..) |column, i| {
                        for (column, 0..) |tile, j| {
                            if (tile == Tile.start) {
                                player.x = j;
                                player.y = i;
                            }
                        }
                    }
                }
            },
        }
    }

    pub fn init(x: usize, y: usize, texture: raylib.Texture2D) Player {
        const textureHeight: f32 = @floatFromInt(texture.height);
        const textureWidth: f32 = @floatFromInt(texture.width);
        return Player{
            .x = x,
            .y = y,
            .rect = raylib.Rectangle.init(0, 0, textureWidth / 5, textureHeight / 5),
            .armed = false,
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

pub const Dungeon = enum {
    empty,
    monster,
    item,
};
