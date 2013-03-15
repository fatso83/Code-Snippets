import org.junit.Before;

/**
 * TDD as if you Meant It
 *
 * The Rules *
 1. Write exactly one new test, the smallest test you can that seems to point in the direction of a solution
 2. See it fail
 3. Make the test from (1) pass by writing the least implementation code you can in the test method.
 4. Refactor to remove duplication, and otherwise as required to improve the design. Be strict about using these moves:
    1. you want a new method—wait until refactoring time, then… create new (non-test) methods by doing one of these, and in no other way:
        1. preferred: do Extract Method on implementation code created as per (3) to create a new method in the test class, or
        2. if you must: move implementation code as per (3) into an existing implementation method
    2. you want a new class—wait until refactoring time, then… create non-test classes to provide a destination for a Move Method and for no other reason
        1. populate implementation classes with methods by doing Move Method, and no other way

 The member of the pair without their hands on the keyboard must be very strict in enforcing these rules, especially 4.1 and 4.2

 After some respectable time coding, contrast and compare solutions. Consider the classes created. How many? How big?
 What mutable state? Consider the methods created How many? How long? Apply a few simple design metrics.
 How was the experience of working this way different from the usual? How could these ideas be applied in your day job?


 a game is over when all fields are taken
 a game is over when all fields in a column are taken by a player
 a game is over when all fields in a row are taken by a player
 a game is over when all fields in a diagonal are taken by a player
 a player can take a field if not already taken
 players take turns taking fields until the game is over

 * @see <a href="http://cumulative-hypotheses.org/2011/08/30/tdd-as-if-you-meant-it">original article</a>
 * @see <a href="http://gojko.net/2009/02/27/thought-provoking-tdd-exercise-at-the-software-craftsmanship-conference/">Gojko's version</a>
 */
public class TicTacToeTest {
    public void should_end_game_when_all_fields_are_taken() {
    }

}
