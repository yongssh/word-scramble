#include "model.hxx"

//
// MODEL CONSTRUCTOR
//


// Wordle Dictionaries pulled from GitHub.
// Link: https://gist.github.com/scholtes/94f3c0303ba6a7768b47583aff36654d
static std::string const short_dictionary{"wordle-La.txt"};
static std::string const long_dictionary{"wordle-Ta.txt"};

//
/// CONSTRUCTOR
//

// Default constructor.
Model::Model()
        : time_remaining_(),
          word_bank_(),
          word_index_(),
          word_(),
          word_posns_(),
          points_(0),
          hint_(false),
          hint_button_posn_(14, 10),
          is_correct_(true),
          wrong_posn_(0, 0),
          hint_posn_(0,0),
          change_in_time_(0.0)
{
    // Load in the dictionary.
    std::ifstream dict_stream = ge211::open_resource_file(short_dictionary);

    // Check if the stream is valid.
    if (dict_stream.bad()) {
        throw std::runtime_error("could not read dictionary from: " +
                                 short_dictionary);
    }

    // Read each word into a vector.
    std::string buffer;
    while (std::getline(dict_stream, buffer)) {
        word_bank_.push_back(buffer);
    }

    // Called to initialize member variables above.
    load_new_word_();
}

// Constructor used for testing.
Model::Model(std::vector<std::string> dictionary)
        : time_remaining_(960),
          word_bank_(dictionary),
          word_index_(rand() % word_bank_.size()),
          word_(word_bank_[word_index_]),
          word_posns_(get_many_rand_posns_(word_)),
          points_(0),
          hint_(false),
          hint_button_posn_(14, 10),
          is_correct_(true),
          wrong_posn_(0, 0),
          hint_posn_(0,0),
          change_in_time_(0.0)
{}


//
// PUBLIC FUNCTIONS
//

void
Model::on_frame(double dt)
{
    time_remaining_ -= dt;

    if (time_remaining_ <= 0 && points_ < 2500) {
        load_new_word_();
    }

    if (!is_correct_) {
        // keeps track of time elapsing for wrong_tile_ so we know when to
        // turn back
        change_in_time_ += dt;
        // For wrong tile timer
        if (change_in_time_ >= 2.0) {
            wrong_posn_ = {0, 0};
            change_in_time_ = 0.0;
        }
    }
}

void
Model::click_letter(Position p)
{
    is_correct_ = true;
    check_hint_(p);

    if (p == word_posns_[0]) {
        word_posns_.erase(word_posns_.begin());
        word_.erase(0, 1);
        update_points_(is_correct_);

        if (word_posns_.empty() && points_ < 2500) {
            load_new_word_();
        }
    }

    else if (std::count(word_posns_.begin(), word_posns_.end(), p) > 0){
        is_correct_ = false;
        update_points_(is_correct_);
        wrong_posn_ = p;
    }
}


//
// PRIVATE HELPER FUNCTIONS
//

ge211::Posn<int>
Model::get_rand_posn_()
{
    // 15 and 11 are the amount of tiles that can fit on the screen (x and y
    // respectively.
    return {rand() % 15, rand() % 11};
}

bool
Model::check_duplicates_(Position p, std::vector<ge211::Posn<int>> v)
{
    // Used tutorial by TechieDelight.
    // https://www.techiedelight.com/check-vector-contains-given-element-cpp/

    // if count (num of times appearing in vector) > 0, return true
    // else, return false
    return ((std::count(v.begin(), v.end(), p)) > 0);
}

std::vector<Model::Position>
Model::get_many_rand_posns_(std::string w)
{
    std::vector<Model::Position> v;

    while (v.size() < w.length()) {

        Model::Position p = get_rand_posn_();

        if (!check_duplicates_(p, v) && p != hint_button_posn_) {
            v.push_back(p);
        }
    }

    return v;
}

void
Model::load_new_word_()
{
    time_remaining_ = 960;
    word_posns_.clear();

    // Assigns word_index_ a random value from 0 to the size of the dictionary.
    word_index_ = rand() % word_bank_.size();

    word_ = word_bank_[word_index_];

    for (Position p : get_many_rand_posns_(word_))
    {
        word_posns_.push_back(p);
    }
}

void
Model::update_points_(bool is_correct)
{
    if (points_ < 2500) { // i.e., if game is still running
        if (is_correct && word_posns_.empty()) {
            points_ += 100;

            // Just added this condition. It works. See click_letter() L62
        } else if (is_correct && !word_posns_.empty()) {
            points_ += 50;

        } else {
            points_ -= 25;
        }

    } else { // i.e., if game is over
        word_posns_.clear();
        word_ = "";
    }
}

void
Model::check_hint_(ge211::Posn<int> p){

    // Resets hint function so that it can be used again.
    if (hint_){
        hint_ = false;
    }
    // if hint button is clicked, stores hint_posn_ with the position of the
    // next correct letter.
    if (p == hint_button_posn_){
        hint_ = true;
        hint_posn_ = word_posns_[0];
    }
}


//
// PUBLIC ACCESSOR FUNCTIONS
//

std::vector<ge211::Posn<int>>
Model::word_posns() const
{
    return word_posns_;
}

std::string
Model::word() const
{
    return word_;
}

std::vector<std::string>
Model::word_bank() const
{
    return word_bank_;
}

size_t
Model::word_index() const
{
    return word_index_;
}

int
Model::points() const
{
    return points_;
}

Model::Position
Model::hint_button_posn() const
{
    return hint_button_posn_;
}

bool
Model::hint() const
{
    return hint_;
}

int
Model::time_remaining() const
{
    return time_remaining_;
}

bool
Model::is_correct() const
{
    return is_correct_;
}

Model::Position
Model::wrong_posn() const
{
    return wrong_posn_;
}

Model::Position
Model::hint_posn() const
{
    return hint_posn_;
}

double
Model::change_in_time() const
{
    return change_in_time_;
}



//
// PUBLIC MUTATOR FUNCTIONS (FOR TESTING)
//

void
Model::set_word_bank(std::vector<std::string> v)
{
    word_bank_.clear();

    for (size_t i = 0; i < v.size(); i++)
    {
        word_bank_.push_back(v[i]);
    }
}


void
Model::set_word(std::string w)
{
    word_ = w;
}

void
Model::set_word_posns(std::vector<Model::Position> v)
{
    word_posns_.clear();

    for (size_t i = 0; i < v.size(); i++)
    {
        word_posns_.push_back(v[i]);
    }
}

void
Model::set_points(int p)
{
    points_ = p;
}

void
Model::set_time_remaining(int s)
{
    time_remaining_ = s;
}

void
Model::set_is_correct(bool t)
{
    is_correct_ = t;
}

void
Model::add_time_remaining(int s)
{
    time_remaining_ += s;
}