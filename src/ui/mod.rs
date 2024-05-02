//
// created on 02/04/24 by Michael Ward
//

// Declare submodules
pub mod button;
pub mod text_object;
// ---

pub trait Ui {
    fn render(&self);

    fn interacted(&self) -> bool {
        false
    }
}
