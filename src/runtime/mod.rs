//
// Created by Michael Ward on 15/05/24
//

pub trait Call {
    fn call(&self) {
        println!("SEKAI!");
    }
    fn call_mut(&mut self) {
        println!("SEKAI!");
    }
}

impl<T: ?Sized + Call> Call for &'_ T {
    fn call(&self) {
        <T as Call>::call(&**self);
    }
}

impl<T: ?Sized + Call> Call for &'_ mut T {
    fn call_mut(&mut self) {
        <T as Call>::call_mut(&mut **self);
    }
}

pub struct Scene<'s> {
    pub function_stack: Vec<Box<dyn Call + 's>>, // Used to run/handle game logic
                                                 // pub draw_stack: Vec<Box<dyn GameObject + 's>>, // Used to render elements on scene
}

impl<'s> Scene<'s> {
    pub fn tick(&mut self) {
        for function in &mut self.function_stack {
            function.call();
            function.call_mut();
        }
    }

    pub fn push_mut<T: Call>(&mut self, item: &'s mut T) {
        self.function_stack.push(Box::new(item));
    }

    pub fn push<T: Call>(&mut self, item: &'s T) {
        self.function_stack.push(Box::new(item));
    }
}
