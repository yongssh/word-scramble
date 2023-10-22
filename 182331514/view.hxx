#pragma once

#include "model.hxx"

class View
{
public:

    using Dimensions = ge211::Dims<int>;
    using Position = ge211::Posn<int>;

    //
    // CONSTRUCTOR
    //

    explicit View(Model const& model, ge211::Mixer& mixer);

    //
    // PUBLIC FUNCTIONS
    //

    /// Renders sprites onto the screen, including the letters and their
    /// tiles, the hint button, the timer and the points.
    void draw(ge211::Sprite_set& set);

    /// Translates board positions to screen positions.
    Position board_to_screen(Position logical);

    /// Translates screen positions to board positions.
    Position screen_to_board(Position physical);

    /// Plays a sound effect when the wrong letter is clicked.
    void play_whoosh_effect();

    /// Defines initial window dimensions. Called by Controller to override.
    Dimensions initial_window_dimensions() const;

private:

    //
    // PRIVATE MEMBER VARIABLES
    //

    Model const& model_;
    ge211::Mixer& mixer_;
    Dimensions initial_window_dims;

    // Text and letters.
    ge211::Font letter_font_{"sans.ttf", 16};
    ge211::Font feature_font_{"sans.ttf", 24};
    ge211::Text_sprite points_sprite;
    ge211::Text_sprite timer_sprite;
    ge211::Text_sprite hint_sprite;
    ge211::Text_sprite goal_sprite;

    // Tiles and hint.
    ge211::Rectangle_sprite const tile_sprite;
    ge211::Rectangle_sprite const wrong_tile_sprite;
    ge211::Rectangle_sprite const hint_tile_sprite;
    ge211::Circle_sprite const hint_button_sprite;

    // The whole alphabet in text_sprites.
    std::vector<ge211::Text_sprite> letter_sprites_;

    // Sound effects.
    ge211::Sound_effect whoosh_sound;
    ge211::Sound_effect_handle whoosh_sound_handle;

    //
    // PRIVATE HELPER FUNCTIONS
    //

    /// Draws one letter onto the screen given an index for the word and word
    /// positions.
    void draw_one_letter_(ge211::Sprite_set& set, size_t i);

    /// Draws and updates the timer onto the screen.
    void draw_timer_(ge211::Sprite_set& set);

    /// Draws and updates the points onto the screen.
    void draw_points_(ge211::Sprite_set& set);

    /// Draws and updates the hint button onto the screen.
    void draw_hint_button_(ge211::Sprite_set& set);

    /// Loads the sound effect. Called by Constructor.
    void load_audio_();
};
