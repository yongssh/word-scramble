#include "controller.hxx"

//
// CONSTRUCTOR
//

Controller::Controller()
        : model_(),
          view_(model_, mixer())
{ }

//
// FUNCTIONS
//

void
Controller::draw(ge211::Sprite_set& set)
{
    view_.draw(set);
}

void
Controller::on_frame(double dt)
{
    model_.on_frame(dt);
}

void
Controller::on_mouse_down(ge211::Mouse_button, ge211::Posn<int> p)
{
    model_.click_letter(view_.screen_to_board(p));

    if (!model_.is_correct()) {
        view_.play_whoosh_effect();
    }
}

void
Controller::on_key(ge211::Key key)
{
    if (key == ge211::Key::code(' ')) {
        model_.add_time_remaining(200);
    }
}

View::Dimensions
Controller::initial_window_dimensions() const
{
    return view_.initial_window_dimensions();
}

