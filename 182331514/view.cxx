#include "view.hxx"

using Color = ge211::Color;


// Some helpful constants used throughout View.
static int const grid_size = 50;
static int const button_radius = 40;

static Color const grey {132, 132, 132};
static Color const green {0, 200, 0};
static Color const red {255, 0, 0};

static std::string const whoosh_filename{"whoosh_sound.mp3"};

//
// CONSTRUCTOR
//

View::View(Model const& model, ge211::Mixer& mixer)
        : model_(model),
          mixer_(mixer),
          initial_window_dims({800, 600}),
          tile_sprite({grid_size, grid_size}, grey),
          wrong_tile_sprite({grid_size, grid_size}, red),
          hint_tile_sprite({grid_size, grid_size}, green),
          hint_button_sprite(button_radius, green)
{

    // Initialize all letter_sprites
    for (char c = 'A'; c <= 'Z'; ++c) {
        std::string letter(1, c);
        letter_sprites_.emplace_back(letter, letter_font_);
    }

    // Loads sound effect.
    if (mixer_.is_enabled()) {
        load_audio_();
    }

}

void
View::draw(ge211::Sprite_set& set)
{
    // Render the hint functionality
    draw_hint_button_(set);

    // Render the letters onto the screen
    for (size_t i = 0; i < model_.word_posns().size(); i++)
    {
        draw_one_letter_(set, i);
    }

    // Render the points count on the screen
    draw_points_(set);

    // Render the timer on the screen
    draw_timer_(set);
}

View::Position
View::board_to_screen(View::Position logical)
{
    return {logical.x * grid_size, logical.y * grid_size};
}

View::Position
View::screen_to_board(View::Position physical)
{
    return {physical.x / grid_size, physical.y / grid_size};
}

void
View::play_whoosh_effect()
{
    if (whoosh_sound_handle.empty() || whoosh_sound_handle.get_state() ==
                                       ge211::Mixer::State::detached)
    {
        whoosh_sound_handle = mixer_.try_play_effect(whoosh_sound);
    }
}

View::Dimensions
View::initial_window_dimensions() const
{
    return initial_window_dims;
}


//
// PRIVATE HELPER FUNCTIONS
//

void
View::draw_one_letter_(ge211::Sprite_set& set, size_t i)
{
    auto scale = ge211::Transform::scale(2);
    Model::Position p = model_.word_posns()[i];

    if (p == model_.wrong_posn() && !model_.is_correct()) {
        set.add_sprite(wrong_tile_sprite, board_to_screen(p), 0);


        // for changing back to normal
        if (model_.change_in_time() >= 2.0){
            set.add_sprite(tile_sprite, board_to_screen(p), 0);
        }

    } else if (p == model_.hint_posn() && model_.hint()) {
        set.add_sprite(hint_tile_sprite, board_to_screen(p), 0);
    } else {
        set.add_sprite(tile_sprite, board_to_screen(p), 0);
    }

    auto letter_index = model_.word()[i] - 'a';
    auto const& letter = letter_sprites_.at(letter_index);
    Model::Position letter_p = board_to_screen(p);
    set.add_sprite(letter,{letter_p.x + (grid_size / 3), letter_p.y},3,
                   scale);
}

void
View::draw_timer_(ge211::Sprite_set& set)
{
    ge211::Text_sprite::Builder timer_builder(feature_font_);
    timer_builder << "TIME: " << model_.time_remaining() / 60;
    timer_sprite.reconfigure(timer_builder);
    set.add_sprite(timer_sprite, {5, 560});
}

void
View::draw_points_(ge211::Sprite_set& set)
{
    ge211::Text_sprite::Builder points_builder(feature_font_);
    points_builder << "POINTS: " << model_.points();
    points_sprite.reconfigure(points_builder);
    set.add_sprite(points_sprite, {5, 0});

    ge211::Text_sprite::Builder goal_builder(feature_font_);
    goal_builder << "GOAL: 2500";
    goal_sprite.reconfigure(goal_builder);
    set.add_sprite(goal_sprite, {5, 28});
}

void
View::draw_hint_button_(ge211::Sprite_set& set)
{
    ge211::Text_sprite::Builder hint_builder(feature_font_);
    hint_builder << "HELP";
    hint_sprite.reconfigure(hint_builder);

    // Find position for the word 'hint' on top of the hint button.
    ge211::Posn<int> physical_hint_posn = board_to_screen(model_
                                                                  .hint_button_posn());
    ge211::Posn<int> hint_word_loc = {physical_hint_posn.x - 10 +
                                      (button_radius / 2) ,
                                      physical_hint_posn.y +
                                      (button_radius / 2)};

    set.add_sprite(hint_sprite, hint_word_loc, 2);
    set.add_sprite(hint_button_sprite, board_to_screen(model_.hint_button_posn
                                                                     ()), 0);
}

void
View::load_audio_()
{
    whoosh_sound.try_load(whoosh_filename, mixer_);
}