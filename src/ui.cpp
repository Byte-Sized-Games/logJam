#include "ui.hpp"
#include "iostream"
#include <utility>

// -- UiElement Skeleton Methods -- //

// These are Useless and solely exist to satisfy linter & compiler warnings
// Please Note that UiElement should only be extended upon, and never used
UiElement::UiElement(void) { position = raylib::Vector2(400, 400); }
void UiElement::Render(void) { raylib::DrawText("ERROR", position.GetX(), position.GetY(), 30, raylib::Color::Red()); }

/// @brief Get X value of Element position
/// @return float value of elements X position
float UiElement::X(void) { return position.GetX(); }

/// @brief Get Y value of Element position
/// @return float value of elements Y position
float UiElement::Y(void) { return position.GetY(); }

/// @brief Set X position Value of Element
/// @param X float value of new X position on Screen
void UiElement::X(float X) { position.SetX(X); }

/// @brief Set Y position Value of Element
/// @param Y float value of new Y position on Screen
void UiElement::Y(float Y) { position.SetY(Y); }

// -- TextObject Ui Element -- //
TextObject::TextObject(void) {
  position = raylib::Vector2(0, 0);
  content = "null";
  fontSize = 20;
  colour = raylib::Color::Blue();
}

TextObject::TextObject(float X, float Y) {
  position = raylib::Vector2(X, Y);
  content = "null";
  fontSize = 20;
  colour = raylib::Color::White();
}

TextObject::TextObject(float X, float Y, std::string Content) {
  position = raylib::Vector2(X, Y);
  content = Content;
  fontSize = 20;
  colour = raylib::Color::Blue();
}

TextObject::TextObject(float X, float Y, std::string Content, raylib::Color Colour) {
  position = raylib::Vector2(X, Y);
  content = Content;
  fontSize = 20;
  colour = Colour;
}

TextObject::TextObject(float X, float Y, std::string Content, int FontSize) {
  position = raylib::Vector2(X, Y);
  content = Content;
  fontSize = FontSize;
  colour = raylib::Color::Blue();
}

TextObject::TextObject(float X, float Y, std::string Content, int FontSize, raylib::Color Colour) {
  position = raylib::Vector2(X, Y);
  content = Content;
  fontSize = FontSize;
  colour = Colour;
}

TextObject::~TextObject(void) {}

// -- TextObject Methods -- //

void TextObject::Render(void) { raylib::DrawText(content, position.GetX(), position.GetY(), 20, colour); }

/// @brief Get String Content of textObject
/// @return c++ string value of textObjects content
std::string TextObject::Content(void) { return content; }

/// @brief Set string Content of textObject
/// @param Content c++ string value of new textObject content
void TextObject::Content(std::string Content) { content = Content; }

// -- Button Ui Element -- //

/// @brief Default constructor. Leaves fields "empty"
Button::Button() = default;

/// @brief Constructor for Button Ui element. Initialises all fields aside from box height and width
/// @param X X coordinate for the button
/// @param Y Y coordinate for the button
/// @param Content Text on Button
/// @param Background Background Colour
/// @param Foreground Text and Outline Colour
Button::Button(float X, float Y, std::string Content, raylib::Color Background, raylib::Color Foreground) {
  position = raylib::Vector2(X, Y);
  box = raylib::Rectangle(position, raylib::Vector2(80, 40));
  content = std::move(Content);
  background = Background;
  foreground = Foreground;
}

/// @brief Constructor for Button Ui element. Initialises all fields aside from box height and width
/// @param X X coordinate for the button
/// @param Y Y coordinate for the button
/// @param Height Box height of button
/// @param Width Box width of button
/// @param Content Text on Button
/// @param Background Background Colour
/// @param Foreground Text and Outline Colour
Button::Button(float X, float Y, float Height, float Width, std::string Content, raylib::Color Background, raylib::Color Foreground) {
  position = raylib::Vector2(X, Y);
  box = raylib::Rectangle(position, raylib::Vector2(Width, Height));
  content = Content;
  background = Background;
  foreground = Foreground;
}

/// @brief Deconstructor
Button::~Button() { /* TODO */ }

// -- Button Methods -- //

/// @brief Checks if the button has been pressed
/// @return Returns true if mouse down inside of button area, else return false
bool Button::IsPressed(void) {
  // Check that mouse left button is down
  if (raylib::Mouse::IsButtonDown(MOUSE_BUTTON_LEFT)) {
    // Check if mouse pointer position is within bounds of Button box.
    if (raylib::Mouse::GetPosition().GetX() >= position.GetX() && raylib::Mouse::GetPosition().GetX() <= position.GetX() + box.GetWidth()) {
      if (raylib::Mouse::GetPosition().GetY() >= position.GetY() && raylib::Mouse::GetPosition().GetY() <= position.GetY() + box.GetHeight()) {
        return true;
      }
    }
  }
  return false;
}

/// @brief Renders Button to screen.
/// Updates Box position, then draws Button box and Button Content to current position
void Button::Render(void) {
  box.SetPosition(position);
  box.Draw(background);
  raylib::DrawText(content.c_str(), position.GetX() + 10, position.GetY() + 10, 25, foreground);
}

// -- Button Getters -- //

/// @brief Get Length of button box
/// @return float value of buttons Length
float Button::Length(void) { return box.GetHeight(); }

/// @brief Get Width of button box
/// @return float value of buttons Width
float Button::Width(void) { return box.GetWidth(); }

/// @brief Get String Content of button
/// @return c++ string value of buttons content
std::string Button::Content(void) { return content; }

// -- Button Setters -- //

/// @brief Set Length of button box
/// @param Length float value of new Button box Length
void Button::Length(float Length) { box.SetHeight(Length); }

/// @brief Set Width of button box
/// @param Width float value of new Button box Width
void Button::Width(float Width) { box.SetWidth(Width); }

/// @brief Set string Content of Button
/// @param Content c++ string value of new Button content
void Button::Content(std::string Content) { content = Content; }
