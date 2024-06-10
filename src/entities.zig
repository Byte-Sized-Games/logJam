const raylib = @import("raylib");
const melodie = @import("melodie.zig");
const Map = @import("map.zig").Map;
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
    keys: u32,

    pub fn render(self: Player) void {
        const posX: f32 = @floatFromInt(self.x);
        const posY: f32 = @floatFromInt(self.y);
        self.texture.drawEx(
            raylib.Vector2.init(posX * 64, posY * 64),
            0,
            2,
            raylib.Color.white,
        );
    }

    pub fn listen(self: *Player, beat: *melodie.Beat, level: *Level) f32 {
        switch (raylib.getKeyPressed()) {
            raylib.KeyboardKey.key_w, raylib.KeyboardKey.key_up => {
                self.y -= self.collide(&level.map, Direction.north);
                level.moves += 1;
                return beat.onBeat();
            },
            raylib.KeyboardKey.key_s, raylib.KeyboardKey.key_down => {
                self.y += self.collide(&level.map, Direction.south);
                level.moves += 1;
                return beat.onBeat();
            },
            raylib.KeyboardKey.key_a, raylib.KeyboardKey.key_left => {
                self.x -= self.collide(&level.map, Direction.west);
                level.moves += 1;
                return beat.onBeat();
            },
            raylib.KeyboardKey.key_d, raylib.KeyboardKey.key_right => {
                self.x += self.collide(&level.map, Direction.east);
                level.moves += 1;
                return beat.onBeat();
            },
            else => {},
        }
        return -1.0;
    }

    /// Check & react to players position in relation to tilemap and entities
    /// Essentially a "collision" manager
    pub fn checkTile(player: *Player, map: *Map, level: *Level) void {
        // Check players position based on tiles
        switch (map.val.tiles[player.y][player.x]) {
            Tile.start, Tile.floor => {},
            Tile.wall => {
                for (map.val.tiles, 0..) |column, i| {
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
                for (map.val.tiles, 0..) |column, i| {
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
        switch (map.val.dungeon[player.y][player.x]) {
            Dungeon.empty => {}, // Nothing Happens
            Dungeon.item => {
                if (player.armed) {} else {
                    player.armed = true;
                    map.val.dungeon[player.y][player.x] = Dungeon.empty;
                }
            },
            Dungeon.monster => {
                if (player.armed) {
                    map.val.dungeon[player.y][player.x] = Dungeon.empty;
                    player.armed = false;
                    level.score += 200;
                } else {
                    level.score -= 200;
                    for (map.val.tiles, 0..) |column, i| {
                        for (column, 0..) |tile, j| {
                            if (tile == Tile.start) {
                                player.x = j;
                                player.y = i;
                            }
                        }
                    }
                }
            },
            Dungeon.key => {
                player.keys += 1;
                map.val.dungeon[player.y][player.x] = Dungeon.empty;
            },
            Dungeon.door => {
                if (player.keys > 0) {
                    player.keys -= 1;
                    map.val.dungeon[player.y][player.x] = Dungeon.doorOpen;
                } else {}
            },
            else => {},
        }
    }

    fn collide(self: *Player, map: *Map, dir: Direction) usize {
        switch (dir) {
            Direction.north => {
                if (map.val.tiles[self.y - 1][self.x] == Tile.wall or map.val.dungeon[self.y - 1][self.x] == Dungeon.door) {
                    if (map.val.dungeon[self.y - 1][self.x] == Dungeon.door and self.keys > 0) {
                        map.val.dungeon[self.y - 1][self.x] = Dungeon.doorOpen;
                    }
                    return 0;
                }
            },
            Direction.south => {
                if (map.val.tiles[self.y + 1][self.x] == Tile.wall or map.val.dungeon[self.y + 1][self.x] == Dungeon.door) {
                    if (map.val.dungeon[self.y + 1][self.x] == Dungeon.door and self.keys > 0) {
                        map.val.dungeon[self.y + 1][self.x] = Dungeon.doorOpen;
                    }
                    return 0;
                }
            },
            Direction.east => {
                if (map.val.tiles[self.y][self.x + 1] == Tile.wall or map.val.dungeon[self.y][self.x + 1] == Dungeon.door) {
                    if (map.val.dungeon[self.y][self.x + 1] == Dungeon.door and self.keys > 0) {
                        map.val.dungeon[self.y][self.x + 1] = Dungeon.doorOpen;
                    }
                    return 0;
                }
            },
            Direction.west => {
                if (map.val.tiles[self.y][self.x - 1] == Tile.wall or map.val.dungeon[self.y][self.x - 1] == Dungeon.door) {
                    if (map.val.dungeon[self.y][self.x - 1] == Dungeon.door and self.keys > 0) {
                        map.val.dungeon[self.y][self.x - 1] = Dungeon.doorOpen;
                    }
                    return 0;
                }
            },
        }
        return 1;
    }

    pub fn init(x: usize, y: usize, texture: raylib.Texture2D) Player {
        const textureHeight: f32 = @floatFromInt(texture.height);
        const textureWidth: f32 = @floatFromInt(texture.width);
        return Player{
            .x = x,
            .y = y,
            .rect = raylib.Rectangle.init(0, 0, textureWidth / 5, textureHeight / 5),
            .armed = false,
            .keys = 0,
            .texture = texture,
        };
    }

    pub fn deinit(self: *Player) void {
        raylib.unloadTexture(self.texture);
    }
};

pub const Tile = enum {
    start,
    floor,
    wall,
    end,
};

pub const Dungeon = enum {
    empty,
    monster,
    item,
    key,
    door,
    doorOpen,
};

const Direction = enum {
    north,
    south,
    east,
    west,
};
