#include "ui.hpp"
#include "iostream"
#include <utility>

namespace ui {
    // -- UiElement Skeleton Methods -- //

// These are Useless and solely exist to satisfy linter & compiler warnings
// Please Note that UiElement should only be extended upon, and never used
    UiElement::UiElement() { position = raylib::Vector2(400, 400); }
    void UiElement::Render() { raylib::DrawText("ERROR", position.GetX(), position.GetY(), 30, raylib::Color::Red()); }

/// @brief Get x value of Element position
/// @return float value of elements x position
    float UiElement::x() { return position.GetX(); }

/// @brief Get y value of Element position
/// @return float value of elements y position
    float UiElement::y() { return position.GetY(); }

/// @brief Set x position Value of Element
/// @param X float value of new x position on Screen
    void UiElement::x(const float X) { position.SetX(X); }

/// @brief Set y position Value of Element
/// @param Y float value of new y position on Screen
    void UiElement::y(const float Y) { position.SetY(Y); }

// -- TextObject Ui Element -- //
    TextObject::TextObject() {
        position = raylib::Vector2(0, 0);
        content = "null";
        fontSize = 20;
        colour = raylib::Color::Blue();
    }

    TextObject::TextObject(float x, float y) {
        position = raylib::Vector2(x, y);
        content = "null";
        fontSize = 20;
        colour = raylib::Color::White();
    }

    TextObject::TextObject(float x, float y, std::string Content) {
        position = raylib::Vector2(x, y);
        content = std::move(Content);
        fontSize = 20;
        colour = raylib::Color::Blue();
    }

    TextObject::TextObject(float X, float y, std::string Content, raylib::Color Colour) {
        position = raylib::Vector2(X, y);
        content = std::move(Content);
        fontSize = 20;
        colour = Colour;
    }

    TextObject::TextObject(float X, float Y, std::string Content, int FontSize) {
        position = raylib::Vector2(X, Y);
        content = std::move(Content);
        fontSize = FontSize;
        colour = raylib::Color::Blue();
    }

    TextObject::TextObject(float X, float Y, std::string Content, int FontSize, raylib::Color Colour) {
        position = raylib::Vector2(X, Y);
        content = std::move(Content);
        fontSize = FontSize;
        colour = Colour;
    }

    TextObject::~TextObject() {}

// -- TextObject Methods -- //

    void TextObject::Render() { raylib::DrawText(content, position.GetX(), position.GetY(), 20, colour); }

/// @brief Get String Content of textObject
/// @return c++ string value of textObjects content
    std::string TextObject::Content() {
        return content;
    }

/// @brief Set string Content of textObject
/// @param Content c++ string value of new textObject content
    void TextObject::Content(std::string Content) { content = std::move(Content); }

// -- Button Ui Element -- //

/// @brief Default constructor. Leaves fields "empty"
    Button::Button() = default;

/// @brief Constructor for Button Ui element. Initialises all fields aside from box height and width
/// @param x x coordinate for the button
/// @param y y coordinate for the button
/// @param Content Text on Button
/// @param Background Background Colour
/// @param Foreground Text and Outline Colour
    Button::Button(float x, float y, std::string Content, raylib::Color Background, raylib::Color Foreground) {
        position = raylib::Vector2(x, y);
        box = raylib::Rectangle(position, raylib::Vector2(80, 40));
        content = std::move(Content);
        background = Background;
        foreground = Foreground;
    }

/// @brief Constructor for Button Ui element. Initialises all fields aside from box height and width
/// @param x x coordinate for the button
/// @param y y coordinate for the button
/// @param Height Box height of button
/// @param Width Box width of button
/// @param Content Text on Button
/// @param Background Background Colour
/// @param Foreground Text and Outline Colour
    Button::Button(float x, float y, float Height, float Width, std::string Content, raylib::Color Background, raylib::Color Foreground) {
        position = raylib::Vector2(x, y);
        box = raylib::Rectangle(position, raylib::Vector2(Width, Height));
        content = std::move(Content);
        background = Background;
        foreground = Foreground;
    }

/// @brief Deconstructor
    Button::~Button() { /* TODO */ }

// -- Button Methods -- //

/// @brief Checks if the button has been pressed
/// @return Returns true if mouse down inside of button area, else return false
    bool Button::IsPressed() {
        // Check that mouse left button is down
        return ((raylib::Mouse::IsButtonDown(MOUSE_BUTTON_LEFT)) && (raylib::Mouse::GetPosition().GetX() >= position.GetX() && raylib::Mouse::GetPosition().GetX() <= position.GetX() + box.GetWidth()) && (raylib::Mouse::GetPosition().GetY() >= position.GetY() && raylib::Mouse::GetPosition().GetY() <= position.GetY() + box.GetHeight()));
    }

/// @brief Renders Button to screen.
/// Updates Box position, then draws Button box and Button Content to current position
    void Button::Render() {
        box.SetPosition(position);
        box.Draw(background);
        raylib::DrawText(content, position.GetX() + 10, position.GetY() + 10, 25, foreground);
    }

// -- Button Getters -- //

/// @brief Get Length of button box
/// @return float value of buttons Length
    float Button::Length() { return box.GetHeight(); }

/// @brief Get Width of button box
/// @return float value of buttons Width
    float Button::Width() { return box.GetWidth(); }

/// @brief Get String Content of button
/// @return c++ string value of buttons content
    std::string Button::Content() { return content; }

// -- Button Setters -- //

/// @brief Set Length of button box
/// @param Length float value of new Button box Length
    void Button::Length(float Length) { box.SetHeight(Length); }

/// @brief Set Width of button box
/// @param Width float value of new Button box Width
    void Button::Width(float Width) { box.SetWidth(Width); }

/// @brief Set string Content of Button
/// @param Content c++ string value of new Button content
    void Button::Content(std::string Content) { content = std::move(Content); }

}
