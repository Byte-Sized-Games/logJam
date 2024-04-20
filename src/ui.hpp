#include "raylib-cpp.hpp"
#include <string>

namespace ui {
    /// @brief Base Ui Element class. All elements derive from this for polymorphism
    class UiElement {
    protected:
        raylib::Vector2 position;

    public:
        UiElement();
        virtual void Render(void);
        // -- Setters -- //
        void x(float);
        void y(float);
        // -- Getters -- //
        float x(void);
        float y(void);
    };

/// @brief A simple way to display text on screen
    class TextObject : public UiElement {
    private:
        std::string content;
        raylib::Color colour;
        unsigned int fontSize;

    public:
        // -- Constructors -- //
        TextObject(void);
        TextObject(float, float);
        TextObject(float, float, std::string);
        TextObject(float, float, std::string, int);
        TextObject(float, float, std::string, raylib::Color);
        TextObject(float, float, std::string, int, raylib::Color);
        // -- Deconstructors -- //
        ~TextObject(void);
        // -- Methods -- //
        void Render(void);
        // -- Setters -- //
        void Content(std::string);
        // -- Getters -- //
        std::string Content(void);
    };

/// @brief A button that can be used to trigger events
    class Button : public UiElement {
    private:
        std::string content;
        raylib::Rectangle box;
        raylib::Color background;
        raylib::Color foreground;

    public:
        // -- Constructors -- //
        Button(void);
        Button(float, float, std::string, raylib::Color, raylib::Color);
        Button(float, float, float, std::string, raylib::Color, raylib::Color);
        Button(float, float, float, float, std::string, raylib::Color, raylib::Color);
        // -- Deconstructors -- //
        ~Button(void);
        // -- Methods -- //
        bool IsPressed(void);
        void Render(void);
        // -- Setters -- //
        void Length(float);
        void Width(float);
        void Content(std::string);
        // -- Getters -- //
        float Length(void);
        float Width(void);
        std::string Content(void);
    };

}
