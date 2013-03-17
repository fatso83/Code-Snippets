import org.junit.Test;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertThat;
import static org.junit.Assert.assertTrue;

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
 * <p>The member of the pair without their hands on the keyboard must be very strict in enforcing these rules, especially 4.1 and 4.2</p>
 * <p/>
 * <p>After some respectable time coding, contrast and compare solutions. Consider the classes created. How many? How big? What mutable state? Consider the methods created How many? How long? Apply a few simple design metrics. How was the experience of working this way different from the usual? How could these ideas be applied in your day job?</p>
 * <p/>
 * <ul>
 * <li>a game is over when all fields are taken</li>
 * <li>a game is over when all fields in a column are taken by a player</li>
 * <li>a game is over when all fields in a row are taken by a player        </li>
 * <li>a game is over when all fields in a diagonal are taken by a player        </li>
 * <li>a player can take a field if not already taken                                 </li>
 * </ul>
 * players take turns taking fields until the game is over
 *
 * @see <a href="http://cumulative-hypotheses.org/2011/08/30/tdd-as-if-you-meant-it">original article</a>
 * @see <a href="http://gojko.net/2009/02/27/thought-provoking-tdd-exercise-at-the-software-craftsmanship-conference/">Gojko's version</a>
 * @see <a href="http://www.codersdojo.com/review/f80623b0fc6c2951dbcd8dc33f9dfb842212e788?page=19#first_modification">Stepwise TDD as if you meant it</a>
 */
public class TicTacToeTest {
	@Test
	public void a_player_can_take_a_field_that_is_not_taken(){
		String cell = "not taken";
		boolean taken = !cell.equals("not taken");
		if(!taken) cell = "player1";
		assertEquals(cell, "player1");
	}
}
