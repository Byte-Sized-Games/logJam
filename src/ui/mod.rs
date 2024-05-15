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
