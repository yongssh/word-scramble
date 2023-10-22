#pragma once

#include <ge211.hxx>
#include <iostream>
#include <vector>
#include <algorithm>

class Model
{

public:

    using Dimensions = ge211::Dims<int>;
    using Position = ge211::Posn<int>;

    //
    // MODEL CONSTRUCTOR
    //

    /// Default constructor
    Model();

    /// Constructor used for testing
    explicit Model(std::vector<std::string> dictionary);

    //
    // PUBLIC ACCESSOR FUNCTIONS
    //

    std::vector<ge211::Posn<int>> word_posns() const;
    std::string word() const;
    std::vector<std::string> word_bank() const;
    size_t word_index() const;
    int points() const;
    Position hint_button_posn() const;
    bool hint() const;
    int time_remaining() const;
    bool is_correct() const;
    Position wrong_posn() const;
    Position hint_posn() const;
    double change_in_time() const;


    //
    // PUBLIC MUTATOR FUNCTIONS
    //

    void set_word_bank(std::vector<std::string> v);
    void set_word(std::string w);
    void set_word_posns(std::vector<Position> v);
    void set_points(int p);
    void set_time_remaining(int s);
    void add_time_remaining(int s);
    void set_is_correct(bool t);

    //
    // PUBLIC GAME FUNCTIONS
    //

    /// Decrements seconds_remaining_ by dt. If seconds_remaining <= 0, call
    /// load_new_word() to move onto the next word.
    ///
    /// NOTE: this function will be called in Controller.
    void on_frame(double dt);

    /// This is the main game-playing function. It takes in a Position p
    /// (which is the position given by a mouse click) and updates model in
    /// the following ways:
    ///
    ///      (1) Sets is_correct_ to true.
    ///
    ///      (2) Call check_hint(p).
    ///
    ///      (2) If the correct letter was clicked (i.e., p equals
    ///          word_posns_[0]), remove the first element in word_posns_ and
    ///          word_ and update points. If the word was just finished (i.e.
    ///          word_posns_ is empty and points < 2500), load a new word.
    ///
    ///      (3) Otherwise, compare p to the rest of the positions in
    ///          word_posns_ (i.e. the wrong letter). For each wp in
    ///          word_posns_ (starting at index 1, not 0), check if p equals
    ///          wp. If it does, update points. Stores the wrong position.
    ///
    /// Otherwise, nothing happens (i.e. p was neither a hint nor a valid
    /// letter on the screen).
    ///
    /// NOTE: this function will be called by Controller.
    void click_letter(Position p);

private:

    //
    // PRIVATE MEMBER VARIABLES
    //

    /// on_frame() runs at 1/60 of a second. If we want each word to have 15
    /// seconds (plus an extra second for load_new_word()) delay, then
    /// time_remaining is initialized to 960.
    int time_remaining_;

    /// All initialized by calling load_new_word() in the Constructor.
    std::vector<std::string> word_bank_;
    size_t word_index_;
    std::string word_;
    std::vector<Position> word_posns_;

    int points_;
    bool hint_;
    Position hint_button_posn_;

    /// Is the clicked letter correct or not? More useful for determining if
    /// clicked letter was incorrect.
    bool is_correct_;

    /// Stores the position of the wrong letter if it was clicked and the
    /// position of the next correct letter if the hint button was clicked.
    Position wrong_posn_;
    Position hint_posn_;

    double change_in_time_;

    //
    // PRIVATE HELPER FUNCTIONS
    //

    /// Uses two random numbers and model dimensions to generate and return a
    /// random position.
    ///
    /// NOTE: this is a helper for get_word_posns().
    ge211::Posn<int> get_rand_posn_();

    /// Compares a given Position p to each Position in a vector of Positions.
    /// Returns true if p already exists in v. Otherwise, returns false.
    ///
    /// NOTE: this is a helper for get_word_posns().
    bool check_duplicates_(Position p, std::vector<ge211::Posn<int>> v);

    /// Generates a random position for each letter in word_ and puts those
    /// positions into word_posns_. Uses the helpers get_rand_posn() and
    /// check_duplicates(). Note that the order of positions in word_posns
    /// corresponds to the order of the letters in word_. For example:
    ///
    ///     if word_ = "CAT"
    ///     and word_posns_ = { {1, 3}, {5, 6}, {7, 1} }, then:
    ///
    ///     c_posn = {1, 3},
    ///     a_posn = {5, 6}, and
    ///     t_posn = {7, 1}
    ///
    /// NOTE: this is a helper for load_new_word()
    std::vector<Position> get_many_rand_posns_(std::string w);

    /// Updates model's variables for a new word in word_bank_ by:
    ///     (1) Resetting time_remaining_ to 960 (16 seconds).
    ///     (2) Clearing word_posns_.
    ///     (3) Setting word_ equal to the next word in word_bank_.
    ///     (2) Setting word_posns_ equal to get_many_rand_posns(word_)
    ///
    /// NOTE: this is a helper function for the Constructor and click_letter()
    void load_new_word_();

    /// Given whether or not the chosen letter was correct, increments points
    /// by 50 or decrements points by 25. If the word was finished,
    /// increments points by 100 instead of 50. If points are greater
    /// than 2500, sets game over by clearing word_posns_ and word_.
    ///
    /// NOTE: this is a helper function for click_letter()
    void update_points_(bool is_correct);

    /// Turns off the hint button after the player has used it. Turns on the
    /// hint button if the player has clicked on it.
    ///
    /// NOTE: this is a helper function for click_letter()
    void check_hint_(ge211::Posn<int> p);
};
