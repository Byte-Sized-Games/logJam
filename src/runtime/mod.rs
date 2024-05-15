pub trait Call {
    fn call(&mut self) {
        println!("SEKAI!");
    }
}

pub struct Scene<'scene> {
    pub function_stack: Vec<Box<dyn Call + 'scene>>,
}

impl Scene<'static> {
    pub fn run(&mut self) {
        for function in &mut self.function_stack {
            function.call();
        }
    }
}
