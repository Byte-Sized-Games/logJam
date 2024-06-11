pub const packages = struct {
    pub const @"122002d98ca255ec706ef8e5497b3723d6c6e163511761d116dac3aee87747d46cf1" = struct {
        pub const build_root = "/home/phoenix/.cache/zig/p/122002d98ca255ec706ef8e5497b3723d6c6e163511761d116dac3aee87747d46cf1";
        pub const deps: []const struct { []const u8, []const u8 } = &.{};
    };
    pub const @"12204a223b19043e17b79300413d02f60fc8004c0d9629b8d8072831e352a78bf212" = struct {
        pub const build_root = "/home/phoenix/.cache/zig/p/12204a223b19043e17b79300413d02f60fc8004c0d9629b8d8072831e352a78bf212";
        pub const build_zig = @import("12204a223b19043e17b79300413d02f60fc8004c0d9629b8d8072831e352a78bf212");
        pub const deps: []const struct { []const u8, []const u8 } = &.{
            .{ "raylib", "1220aa75240ee6459499456ef520ab7e8bddffaed8a5055441da457b198fc4e92b26" },
            .{ "raygui", "122002d98ca255ec706ef8e5497b3723d6c6e163511761d116dac3aee87747d46cf1" },
        };
    };
    pub const @"1220aa75240ee6459499456ef520ab7e8bddffaed8a5055441da457b198fc4e92b26" = struct {
        pub const build_root = "/home/phoenix/.cache/zig/p/1220aa75240ee6459499456ef520ab7e8bddffaed8a5055441da457b198fc4e92b26";
        pub const build_zig = @import("1220aa75240ee6459499456ef520ab7e8bddffaed8a5055441da457b198fc4e92b26");
        pub const deps: []const struct { []const u8, []const u8 } = &.{};
    };
};

pub const root_deps: []const struct { []const u8, []const u8 } = &.{
    .{ "raylib-zig", "12204a223b19043e17b79300413d02f60fc8004c0d9629b8d8072831e352a78bf212" },
};
