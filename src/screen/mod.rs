//
// created on 02/04/24 by Michael Ward
//

// Namespaces
use super::runtime::Call;
use super::ui;
// ---

pub trait Screen {
    fn render(&self);
}

pub struct Menu<'m> {
    pub elements: Vec<Box<dyn ui::Ui + 'm>>,
}

impl Screen for Menu<'_> {
    fn render(&self) {
        for element in &self.elements {
            element.render();
        }
    }
}

impl Call for Menu<'_> {
    fn call(&mut self) {
        self.render();
    }
}
