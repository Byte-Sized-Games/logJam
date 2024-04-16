#include "inc/raylib-cpp.hpp"
#include <string>

/// @brief Base Ui Element class. All elements derive from this for polymorphism
class UiElement {
protected:
  raylib::Vector2 position;

public:
  virtual void Render(void);
  // -- Setters -- //
  void X(float);
  void Y(float);
  // -- Getters -- //
  float X(void);
  float Y(void);
  virtual void Render();
  UiElement();
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
