#pragma once

#include "model.hxx"
#include "view.hxx"

#include <ge211.hxx>

class Controller : public ge211::Abstract_game
{

public:

    //
    // CONSTRUCTOR
    //

    Controller();

protected:

    //
    // PROTECTED FUNCTIONS
    //

    /// Updates model based on mouse clicks.
    void on_mouse_down(ge211::Mouse_button, ge211::Posn<int> p) override;

    /// Calls View's draw function.
    void draw(ge211::Sprite_set& set) override;

    /// Updates model based on time passing.
    void on_frame(double dt) override;

    /// Updates model based on key presses.
    void on_key(ge211::Key key) override;

    /// Initializes window dimensions, which is delegated to View.
    View::Dimensions initial_window_dimensions() const override;

private:

    //
    // PRIVATE MEMBER VARIABLES
    //

    Model model_;
    View view_;
};
