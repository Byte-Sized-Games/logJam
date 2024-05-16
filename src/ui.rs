//
// created on 02/04/24 by Michael Ward
//

// Declare submodules
pub mod button;
pub mod text_object;
// ---

pub mod prelude {
    pub use super::button::*;
    pub use super::text_object::*;
    pub use super::Ui;
}

/// Trait for any structure related to rendering the User interface.
/// Used in combination with the Menu struct to bundle multiple elements.
pub trait Ui {
    /// Displays element on screen.
    /// Required for any implementation
    fn render(&self);

    /// Returns interaction with object.
    /// Used to handle actions with interactive elements.
    /// Ex. Buttons, sliders
    fn interacted(&self) -> bool {
        false
    }
}

impl<T: ?Sized + Ui> Ui for &'_ T {
    fn render(&self) {
        <T as Ui>::render(&**self);
    }
}

impl<T: ?Sized + Ui> Ui for &'_ mut T {
    fn render(&self) {
        <T as Ui>::render(&**self);
    }
}
