pub fn load_state() {}

/// @brief master logic callstack. not recommended in most cases, try pushing to a scene-specific callstack to avoid transition hell
static mut LOGIC_STACK: Vec<Vec<bool>> = vec![];
/// @brief master draw callstack. not recommended in most cases, try pushing to a scene-specific callstack to avoid transition hell
static mut DRAW_STACK: Vec<Vec<bool>> = vec![];

// @brief currently rendering GameState
// TODO: implement init scene
// static mut CURRENT_STATE: super::GameState;

struct GameState<'g> {
    /// @brief scene-specific logic callstack. runs after master callstack on each priority level
    logic_stack: Vec<Vec<bool>>,
    /// @brief scene-specific draw callstack. runs after master callstack on each priority level
    draw_stack: Vec<Vec<bool>>,
    /// @brief vector of elements drawn on first state call via a draw_stack item
    init_scene: Vec<Box<dyn super::Entity + 'g>>,
}
