//
// created on 02/04/24 by Michael Ward
//

// Namespaces
use super::ui;
use super::Call;
// ---

pub struct Menu<'m> {
    pub elements: Vec<Box<dyn ui::Ui + 'm>>,
}

impl Call for Menu<'_> {
    fn call(&self) {
        for element in &self.elements {
            element.render();
        }
    }
}
