//
// Created by Michael Ward on 15/05/24

use macroquad::prelude::*;

pub trait Call {
    fn call(&mut self) {
        println!("SEKAI!");
    }
}

pub struct Scene<'s> {
    pub function_stack: Vec<Box<dyn Call + 's>>,
}

impl Scene<'_> {
    pub fn run(&mut self) {
        for function in &mut self.function_stack {
            function.call();
        }
    }
}
