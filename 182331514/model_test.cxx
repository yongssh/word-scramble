#include "model.hxx"
#include <catch.hxx>

using Dimensions = ge211::Dims<int>;
using Position = ge211::Posn<int>;


//
// TODO: Write preliminary model tests.
//
// These tests should demonstrate your functional requirements.
//

/*
 * TABLE OF CONTENTS:
 *
 * TEST ONE: CLICKING LETTERS (L24)
 * TEST TWO: FINISHING A WORD
 * TEST THREE: TIMER
 * TEST FOUR: HINT FUNCTION
 * TEST FIVE: GAME OVER
 */

TEST_CASE("TEST ONE: CLICKING LETTERS")
{
    /// This test shows what occurs when (1) a wrong letter is clicked, (2) a
    /// correct letter is clicked, and (3) a non-letter position is clicked (i.e,
    /// a random position on the screen).

    // Initialize Model with a small word bank.
    std::vector<std::string> wb = {"apple", "kitchen"};
    Model m = Model(wb);

    // The program randomly chooses a word from the word bank and generates
    // random positions for each letter in the word (word positions).
    // Check that word and its positions have been initialized from the
    // word bank.
    bool in_bank = false;
    for ( auto word : m.word_bank() ) {
        if (word == m.word()) {
            in_bank = true;
        }
    }
    CHECK( in_bank );
    CHECK( m.word_posns().size() == m.word().length() );

    // But we want to be able to keep track for the sake of this test so
    // let's make sure we know the word and its positions for certain.
    m.set_word(wb[0]); // i.e., "apple"
    std::vector<Position> wp = { {1, 4}, {3, 7}, {4, 4}, {8, 9}, {2, 5} };
    m.set_word_posns(wp);
    CHECK( m.points() == 0 ); // Points are empty.

    // Click the correct letter. The correct letter is always the first
    // position in word_posns. It is removed from word_posns once clicked.
    m.click_letter(m.word_posns()[0]);
    CHECK( m.word_posns().size() == 4 ); // That position was removed.
    CHECK( m.word() == "pple" ); // The correct letter was removed.
    CHECK( m.points() == 50 ); // Points increased by 50 for correct letter.

    // Click the incorrect letter (i.e. any other position in word_posns)
    m.click_letter(m.word_posns()[3]);
    CHECK( m.points() == 25 ); // Points decreased by 25 for wrong letter.

    // Click a random position on the screen. No change - all variables
    // remain the same.
    m.click_letter( {3, 2});
    CHECK( m.word_posns().size() == 4 );
    CHECK( m.word() == "pple" );
    CHECK( m.points() == 25 );

    std::cout << "TEST ONE: CLICKING LETTERS PASSED" << std::endl;
}


TEST_CASE("TEST TWO: COMPLETING A WORD")
{
    /// This test shows what occurs when a word is completed.

    // Initialize Model with a small word bank.
    std::vector<std::string> wb = {"kitchen", "spoon", "sink"};
    Model m = Model(wb);

    // The program randomly chooses a word from the
    // word bank and generates random positions for each letter in the word.
    // Check that word and its positions have been initialized from the
    // word bank.
    bool in_bank = false;
    for ( auto word : m.word_bank() ) {
        if (word == m.word()) {
            in_bank = true;
        }
    }
    CHECK( in_bank );
    CHECK( m.word_posns().size() == m.word().length() );

    // For the sake of this test, let's manually set the word and its
    // positions so we know what we're dealing with.
    m.set_word("cats");
    m.set_word_posns({ {1, 4}, {3, 7}, {4, 4}, {2, 5} });

    // Check that points are 0 at the start of the game.
    CHECK( m.points() == 0 );

    // Click the first three letters in our word, "cats".
    m.click_letter(m.word_posns()[0]);
    CHECK( m.word_posns().size() == 3 ); // Word positions decreased.
    CHECK( m.word() == "ats" ); // Word length decreased.
    CHECK( m.points() == 50 ); // Points increased by 50.

    m.click_letter(m.word_posns()[0]);
    CHECK( m.word_posns().size() == 2 );
    CHECK( m.word() == "ts" );
    CHECK( m.points() == 100 );

    m.click_letter(m.word_posns()[0]);
    CHECK( m.word_posns().size() == 1 );
    CHECK( m.word() == "s" );
    CHECK( m.points() == 150 );

    // Click the last letter, 's'.
    m.click_letter(m.word_posns()[0]);

    // Result: Word and word_posns reset to a random word in the word bank.
    // Points increase by 100 instead of 50 for completing a word.
    // Check that word and its positions have been initialized from the
    // word bank.
    in_bank = false;
    for ( auto word : m.word_bank() ) {
        if (word == m.word()) {
            in_bank = true;
        }
    }
    CHECK( in_bank );
    CHECK( m.word_posns().size() == m.word().length() );
    CHECK( m.points() == 250 );

    std::cout << "TEST TWO: COMPLETING A WORD PASSED" << std::endl;
}

TEST_CASE("TEST THREE: TIMER") {

    /// This test shows what occurs when (1) the word IS NOT completed before
    /// the timer runs out, and (2) the word IS completed before the timer
    /// runs out.

    // Initialize Model with a small word bank.
    std::vector<std::string> wb = {"laptop", "sleepy"};
    Model m = Model(wb);

    // Our timer starts at 960. At 60 frames/sec, this is 16 seconds (15
    // seconds to play the actual word and 1 second to consider the delay of
    // loading a new word).
    CHECK( m.time_remaining() == 960 );

    // (1) The word is not completed before the timer runs out. If this
    // occurs, a new word is loaded from the word bank, and corresponding
    // variables (word positions, time remaining) are reset based on the new
    // word.
    std::string word1 = "cat"; // First word (not in word bank).
    m.set_word(word1); // And corresponding word_posns
    m.set_word_posns({{3, 1}, {2, 3}, {5, 6} });
    CHECK( m.points() == 0 ); // Points start at 0

    m.click_letter( m.word_posns()[0] ); // Let's click one letter.
    m.on_frame(m.time_remaining()); // Let timer reach 0.

    // Result: new word is loaded from word bank and the timer is reset.
    // Check that word and its positions have been initialized from the
    // word bank.
    bool in_bank = false;
    for ( auto word : m.word_bank() ) {
        if (word == m.word()) {
            in_bank = true;
        }
    }
    CHECK( in_bank );
    CHECK( m.word_posns().size() == m.word().length() );
    CHECK( m.time_remaining() == 960 );

    // (2) Complete the word before the timer runs out. If this occurs, a new
    // word is loaded and corresponding variables are reset.

    m.on_frame(60); // Let's let the timer run 1 second.
    CHECK( m.time_remaining() == 900 ); // Check time was updated.

    for ( auto pos : m.word_posns() ) {
        m.click_letter(pos); // Click all the correct letters.
    }

    // Result: new word is loaded from word bank, word positions are randomly
    // generated for the word, and the timer is reset.
    // Check that word and its positions have been initialized from the
    // word bank.
    in_bank = false;
    for ( auto word : m.word_bank() ) {
        if (word == m.word()) {
            in_bank = true;
        }
    }
    CHECK( in_bank );
    CHECK( m.word_posns().size() == m.word().length() );
    CHECK( m.time_remaining() == 960 );

    std::cout << "TEST THREE: TIMER PASSED" << std::endl;
}


TEST_CASE("TEST FOUR: HINT FUNCTION")
{
    /// This test shows what occurs when the hint button is pressed.

    // Construct Model. BTW, fleabag is the best show ever!
    Model m =  Model({"fleabag"});

    // Check that first word and its positions have been initialized from the
    // word bank.
    CHECK( m.word() == "fleabag" );
    CHECK( m.word_posns().size() == m.word().length() );

    // The hint button is always at {14, 10} on the screen. Let's click it.
    m.click_letter(m.hint_button_posn());

    // Result: The hint boolean becomes true and the position of the next
    // correct letter is stored in a variable called hint_posn.
    // These values are used by View to turn hint_posn's letter green on the
    // screen, which the user can then click.
    CHECK( m.hint() );
    CHECK( m.hint_posn() == m.word_posns()[0] );

    // Let's click our hint position like the player would!
    m.click_letter(m.hint_posn());
    CHECK( m.word() == "leabag" );
    CHECK( m.word_posns().size() == 6 );
    CHECK( m.points() == 50 );

    std::cout << "TEST FOUR: HINT FUNCTION PASSED" << std::endl;
}


TEST_CASE("TEST FIVE: GAME OVER")
{
    /// This test shows what occurs when the game is over (i.e., points reach
    /// 2500).

    // Construct Model. BTW, fleabag is the best show ever!
    Model m =  Model({"game", "of", "thrones"});

    // Check that first word and its positions have been initialized from the
    // word bank.
    bool in_bank = false;
    for ( auto word : m.word_bank() ) {
        if (word == m.word()) {
            in_bank = true;
        }
    }
    CHECK( in_bank );
    CHECK( m.word().length() == m.word_posns().size() );

    // Check that points are 0.
    CHECK( m.points() == 0 );

    // Get some points! Click the first letter.
    m.click_letter(m.word_posns()[0]);

    // Easier: just reset points to goal: 2500.
    m.set_points(2500);

    // Click another letter so Model can run.
    m.click_letter(m.word_posns()[0]);

    // Result: Word and word positions are cleared (and so is the screen).
    CHECK( m.word() == "" );
    CHECK( m.word_posns().empty() );
}


//
// TESTING HELPER FUNCTIONS
//
// (TESTED BEFORE HELPERS WERE MADE PRIVATE)

/*
TEST_CASE("Model::get_rand_posn_()")
{
    // Creates a model with default scene_dims = {1000, 800}.
    Model m = Model();

    Position rand_p = m.get_rand_posn_();
    CHECK( rand_p.x < 15 );
    CHECK( rand_p.y < 11 );

    rand_p = m.get_rand_posn();
    CHECK( rand_p.x < 15 );
    CHECK( rand_p.y < 11 );

    rand_p = m.get_rand_posn();
    CHECK( rand_p.x < 15);
    CHECK( rand_p.y < 11);
}

TEST_CASE("Model::check_duplicates()")
{
    Model m = Model();

    Position p = {3, 1};
    std::vector<Position> v = { {6, 3}, {2, 1}, {3, 1}, {4, 5} };
    CHECK( m.check_duplicates(p, v) );

    std::vector<Position> v2 = { {6, 3}, {2, 1}, {4, 5} };
    CHECK_FALSE( m.check_duplicates(p, v2) );
}

TEST_CASE("Model::get_many_rand_posns()")
{
    Model m = Model();

    std::vector<Position> v = m.get_many_rand_posns("cat");
    CHECK( v.size() == 3 );

    v.clear();
    v = m.get_many_rand_posns("apple");
    CHECK( v.size() == 5 );

    v.clear();
    v = m.get_many_rand_posns("carrington");
    CHECK( v.size() == 10 );
}

TEST_CASE("Model::load_new_word()")
{
    Model m = Model();

    std::vector<std::string> wb = {"apple", "dawdle", "man", "sync", "woman",
                                   "tv", "blanket"};
    m.set_word_bank(wb);

    CHECK( m.word() == "apple" );
    CHECK( m.word_posns().size() == 5 );

    m.load_new_word();

    CHECK( m.word() == m.word_bank()[1] );
    CHECK( m.word_posns().size() == 6 );
}

TEST_CASE("Model::update_points()")
{
    Model m = Model();

    m.set_points(0);

    m.update_points(true);
    CHECK( m.points() == 50 );

    m.set_word_posns({ {4, 5} });
    m.update_points(true);
    CHECK( m.points() == 100 );

    m.update_points(false);
    CHECK( m.points() == 75 );

    m.set_points(2500);
    m.update_points(true);
}

TEST_CASE("Model::on_frame()")
{
    Model m = Model();

    std::vector<std::string> wb = {"apple", "dawdle", "man", "sync", "woman",
                                   "tv", "blanket"};
    std::vector<Position> wp = { {3, 8}, {2, 2}, {5, 4}, {7, 4}, {9, 0} };

    m.set_word_bank(wb);
    m.set_word_posns(wp);

    CHECK( m.word() == "apple" );
    CHECK( m.word_posns().size() == 5 );

    // initialize dt as 1
    double dt = 1;
    CHECK( m.seconds_remaining() == 10 );

    m.on_frame(dt);
    CHECK( m.seconds_remaining() == 9 );

    // Timer has ended. Check that load_new_word() was called.
    m.set_seconds_remaining(1);

    m.on_frame(dt);
    CHECK( m.word() == "dawdle" );
    CHECK( m.word_posns().size() == 6 );

    std::cout << "on_frame() works" << std::endl;

}


TEST_CASE("Model::click_letter() -- TEST ONE")
{
    Model m = Model();
    CHECK(m.points_ == 0);

    // check if points are decremented if wrong letter is clicked
    m.points_ = 100;
    CHECK(m.points_ == 100);
    m.word_posns_ = { {6, 3}, {2, 1}, {3, 1}, {4, 5} };
    CHECK(m.word_posns_.size() == 4);
    m.click_letter({3, 1});
    CHECK(m.word_posns_.size() == 4);
    CHECK(m.points_ == 75);

    // check for no change
    m.points_ = 100;
    CHECK(m.points_ == 100);
    m.word_posns_ = { {6, 3}, {2, 1}, {3, 1}, {4, 5} };
    CHECK(m.word_posns_.size() == 4);
    m.click_letter({5, 1});
    CHECK(m.word_posns_.size() == 4);
    CHECK(m.points_ == 100);

    // check if it removes first one, points increased
    m.points_ = 100;
    m.word_posns_ = { {6, 3}, {2, 1}, {3, 1}, {4, 5} };
    m.click_letter({6, 3});
    CHECK(m.word_posns_.size() == 3);
    CHECK(m.points_ == 150);

    std::cout << "click_letter() points works" << std::endl;

}

TEST_CASE("Model::click_letter() -- TEST TWO")
{
    Model m = Model();

    std::vector<std::string> wb = {"apple", "dawdle", "man", "sync", "woman",
                                   "tv", "blanket"};
    std::vector<Position> wp = { {3, 8}, {2, 2}, {5, 4}, {7, 4}, {9, 0} };

    m.set_word_bank(wb);
    m.set_word_posns(wp);

    CHECK( m.points() == 0 );
    CHECK( m.word() == "apple" );
    CHECK( m.word_posns().size() == 5 );
    CHECK( m.word().length() == 5 );

    // Click the correct letter
    m.click_letter( {3, 8} );
    CHECK( m.word_posns().size() == 4 );
    CHECK( m.word().length() == 4 );
    CHECK( m.points() == 50 );

    // Click the correct letter again
    m.click_letter( {2, 2} );
    CHECK( m.word_posns().size() == 3 );
    CHECK( m.word().length() == 3 );
    CHECK( m.points() == 100 );

    // Click the wrong letter
    m.click_letter( {7, 4} );
    CHECK( m.word_posns().size() == 3 );
    CHECK( m.word().length() == 3 );
    CHECK_FALSE( m.is_correct() );
    CHECK( m.points() == 75 );

    // Click neither a correct nor incorrect letter
    m.click_letter( {8, 8} );
    CHECK( m.word_posns().size() == 3 );
    CHECK( m.word().length() == 3 );
    CHECK( m.is_correct() );
    CHECK( m.points() == 75 );

    // Click the hint button
    m.click_letter( m.hint_posn() );
    CHECK( m.hint() );
    CHECK( m.points() == 75 );

    // Click the wrong letter again
    m.click_letter( {7, 4} );
    CHECK( m.word_posns().size() == 3 );
    CHECK( m.word().length() == 3 );
    CHECK_FALSE( m.is_correct() );
    CHECK( m.points() == 50 );

    // Click the hint button again
    m.click_letter( m.hint_posn() );
    CHECK( m.hint() );
    CHECK( m.is_correct() );
    CHECK( m.points() == 50 );

    // Click the correct letter
    m.click_letter( {5, 4} );
    CHECK( m.points() == 100 );

    // Click the correct letter
    m.click_letter( {7, 4} );
    CHECK( m.word_posns().size() == 1 );
    CHECK( m.word().length() == 1 );
    CHECK( m.points() == 150 );

    // Click the last correct letter. Add 50 points for clicking the correct
    // letter and add 100 points for finishing the word.
    // load_new_word() should be called and m word_ and word_posns_
    // should be reinitialized.
    m.click_letter( {9, 0} );
    CHECK( m.points() == 250 );

    // Check that new word has been loaded
    CHECK( m.word() == "dawdle" );
    CHECK( m.word_posns().size() == 6 );
    CHECK( m.word().length() == 6 );
    CHECK( m.points() == 250 );

    std::cout << "click_letter words yay!" << std::endl;
}
 */