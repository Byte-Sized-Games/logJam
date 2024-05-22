//
// created on 02/04/24 by Michael Ward
//

use macroquad::prelude::*;

use super::Ui;
use crate::RunAction;

pub struct Button {
    pub x: f32,
    pub y: f32,
    content: String,
    pub action: RunAction,
    pub background_colour: Color,
    pub foreground_colour: Color,
}

impl Button {
    pub fn new(
        x: f32,
        y: f32,
        content: String,
        action: RunAction,
        background_colour: Color,
        foreground_colour: Color,
    ) -> Button {
        Button {
            x,
            y,
            content,
            action,
            background_colour,
            foreground_colour,
        }
    }

    pub fn action(&self) -> RunAction {
        if self.interacted() {
            return self.action;
        } else {
            return RunAction::None;
        }
    }
}

impl Ui for Button {
    fn render(&self) {
        draw_rectangle(
            self.x,
            self.y,
            self.content.len() as f32 * 30.0,
            60.0,
            self.background_colour,
        );

        draw_text(
            &self.content,
            self.x + 20.0,
            self.y + 35.0,
            30.0,
            self.foreground_colour,
        )
    }

    fn interacted(&self) -> bool {
        if is_mouse_button_down(MouseButton::Left) {
            if mouse_position().0 > self.x
                && mouse_position().0 < self.x + (self.content.len() as f32 * 30.0)
            {
                if mouse_position().1 > self.y && mouse_position().1 < self.y + 60.0 {
                    return true;
                }
            }
        }
        false
    }
}
