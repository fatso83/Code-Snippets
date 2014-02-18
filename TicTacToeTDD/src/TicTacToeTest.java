import org.junit.Ignore;
import org.junit.Test;

import java.util.*;

import static java.util.Arrays.asList;
import static org.junit.Assert.*;

import static java.util.Collections.EMPTY_SET;
/**
 * TDD as if you Meant It
 * <h2>The Rules</h2>
 * <ol>
 * <li>Write <em>exactly one</em> new test, the smallest test you can that seems to point in the direction of a solution</li>
 * <li>See it fail</li>
 * <li>Make the test from (1) pass by writing the least implementation code you can <em>in the test method. </em></li>
 * <li>Refactor to remove duplication, and otherwise as required to improve the design. Be strict about using these moves:
 * <ol>
 * <li><strong>you want a new method—wait until refactoring time, then&#8230; </strong>create new (non-test) methods by doing one of these, and in no other way:</li>
 * <ol>
 * <li>preferred: do Extract Method on implementation code created as per (3) to create a new method in the test class, or</li>
 * <li>if you must: move implementation code as per (3) into an existing implementation method</li>
 * </ol>
 * <li><strong>you want a new class—<strong>wait until refactoring time, then&#8230; </strong></strong>create non-test classes to provide a destination for a Move Method and for no other reason</li>
 * <ol>
 * <li>populate implementation classes with methods by doing Move Method, and no other way</li>
 * </ol>
 * </ol>
 * </li>
 * </ol>
 * <p/>
 * <p>The member of the pair without their hands on the keyboard must be very strict in enforcing these
 * rules, especially 4.1 and 4.2</p>
 * <p/>
 * <p>After some respectable time coding, contrast and compare solutions. Consider the classes created.
 * How many? How big? What mutable ownedBy? Consider the methods created How many? How long?
 * Apply a few simple design metrics. How was the experience of working this way different from the usual?
 * How could these ideas be applied in your day job?
 * </p>
 * <p/>
 * <ul>
 * <li>a game is over when all fields are taken</li>
 * <li>a game is over when all fields in a column are taken by a player</li>
 * <li>a game is over when all fields in a row are taken by a player        </li>
 * <li>a game is over when all fields in a diagonal are taken by a player        </li>
 * <li>a player can take a field if not already taken                                 </li>
 * <li>players take turns taking fields until the game is over</li>
 * </ul>

 *
 * @see <a href="http://cumulative-hypotheses.org/2011/08/30/tdd-as-if-you-meant-it">original article</a>
 * @see <a href="http://gojko.net/2009/02/27/thought-provoking-tdd-exercise-at-the-software-craftsmanship-conference/">Gojko's version</a>
 * @see <a href="http://www.codersdojo.com/review/f80623b0fc6c2951dbcd8dc33f9dfb842212e788?page=19#first_modification">Stepwise TDD as if you meant it</a>
 */
public class TicTacToeTest {

    @Test
    public void a_game_is_over_when_all_fields_are_taken() {
        Set<String> availableFields = EMPTY_SET;

        assertTrue(isGameOver(availableFields, EMPTY_SET, EMPTY_SET, EMPTY_SET, EMPTY_SET, EMPTY_SET));
    }

    @Test
    public void a_game_is_over_when_all_fields_in_a_column_are_taken_by_a_player() {

        Set<Set<String>> columns = buildSetOfSetsOfStrings(
                new String[]{"A1", "B1"},
                new String[]{"A2", "B2"}
        );

        Set<String> availableFields = buildAllAvailableFields(columns);

        Set<String> player1Fields = new HashSet<>();
        takeField(availableFields, player1Fields, "A2");
        takeField(availableFields, player1Fields, "B2");

        assertTrue(isGameOver(availableFields, columns, EMPTY_SET, EMPTY_SET, player1Fields, EMPTY_SET));
    }

    @Test
    public void a_game_is_over_when_all_fields_in_a_row_are_taken_by_a_player() {

        Set<Set<String>> rows = buildSetOfSetsOfStrings(
                new String[]{"A1", "A2"},
                new String[]{"B1", "B2"}
        );

        Set<String> availableFields = buildAllAvailableFields(rows);

        Set<String> player1Fields = new HashSet<>();
        takeField(availableFields, player1Fields, "B1");
        takeField(availableFields, player1Fields, "B2");

        assertTrue(isGameOver(availableFields, EMPTY_SET, rows, EMPTY_SET, player1Fields, EMPTY_SET));
    }

    @Test
    public void a_game_is_over_when_all_fields_in_a_diagonal_are_taken_by_a_player() {

        Set<Set<String>> diagonals = buildSetOfSetsOfStrings(
                new String[]{"A1", "B2"},
                new String[]{"B1", "A2"}
        );

        Set<String> availableFields = buildAllAvailableFields(diagonals);

        Set<String> player1Fields = new HashSet<>();
        takeField(availableFields, player1Fields, "A1");
        takeField(availableFields, player1Fields, "B2");

        assertTrue(isGameOver(availableFields, EMPTY_SET, EMPTY_SET, diagonals, player1Fields, EMPTY_SET));
    }


    @Test
    public void a_player_can_take_a_field_that_is_not_taken() {
        Set<String> availableFields = new HashSet<String>();
        Set<String> player1Fields = new HashSet<String>();
        availableFields.addAll(asList("A", "B"));

        String fieldIWant = "B";

        takeField(availableFields, player1Fields, fieldIWant);

        assertTrue(player1Fields.contains(fieldIWant));
    }

    @Test
    public void a_player_can_not_take_a_field_that_is_taken() {
        Set<String> availableFields = new HashSet<String>(Arrays.asList("A", "B"));
        Set<String> player1Fields = new HashSet<String>();
        Set<String> player2Fields = new HashSet<String>();


        player2Fields.add("C");

        String fieldIWant = "C";

        takeField(availableFields, player1Fields, fieldIWant);

        assertFalse(player1Fields.contains(fieldIWant));
    }


    @Ignore
    @Test
    public void the_game_should_ensure_players_should_take_turns() {

    }


    private static Set<String> buildAllAvailableFields(Set<Set<String>> setOfSets) {
        Set<String> availableFields = new HashSet<String>();

        for(Set<String> set: setOfSets)                                 availableFields.addAll(set);
        return availableFields;
    }

    private static Set<Set<String>> buildSetOfSetsOfStrings(String[]... columns) {
        Set<Set<String>> allCols = new HashSet<>();

        for(String[] cArr : columns) {
            Set<String> column = Collections.unmodifiableSet(new  HashSet<String>(asList(cArr)));
            allCols.add(column);
        }

        return allCols;
    }

    private boolean playerHasColumn(Set<String> player1Fields, Set<Set<String>> columns) {
        return playerSetContainsOneOfSets(player1Fields, columns);
    }

    private boolean playerHasRow(Set<String> player1Fields, Set<Set<String>> columns) {
        return playerSetContainsOneOfSets(player1Fields, columns);
    }

    private boolean playerHasDiagonal(Set<String> player1Fields, Set<Set<String>> columns) {
        return playerSetContainsOneOfSets(player1Fields, columns);
    }

    private boolean playerSetContainsOneOfSets(Set<String> player1Fields, Set<Set<String>> columns) {
        boolean hasColumn = false;
        for( Set<String> c : columns) {
            if(player1Fields.containsAll(c)) {
                hasColumn = true;
                break;
            }
        }
        return hasColumn;
    }

    private boolean isGameOver(
            Set<String> availableFields,
            Set<Set<String>> columns,
            Set<Set<String>> rows,
            Set<Set<String>> diagonals,
            Set<String> p1,
            Set<String> p2
    ) {
        return availableFields.equals(Collections.EMPTY_SET) ||
                hasWinState(p1,columns,rows,diagonals) ||
                hasWinState(p2,columns,rows,diagonals);
    }

    private boolean hasWinState(
            Set<String> player,
            Set<Set<String>> columns,
            Set<Set<String>> rows,
            Set<Set<String>> diagonals
    ) {
        return  playerHasColumn(player, columns) ||
                playerHasRow(player, rows) ||
                playerHasDiagonal(player, diagonals);
    }



    void takeField(Set<String> availableFields, Set<String> playerFields, String fieldIWant) {
        if(availableFields.contains(fieldIWant)){
            availableFields.remove(fieldIWant);

            playerFields.add(fieldIWant);
        }
    }
}

