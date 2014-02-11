/**
 * This was meant to be a FSM with a switch statement as the main motor, but it turned into a
 * function lookup by use of an associative array (object). So it is more of an implicit switch.
 *
 * The implementation was copy-pasted from asm-md-parser.js, where I removed the reference to
 * AbstractStateMachine. And it turned out that was almost all I needed to change ...
 *
 * To make it simpler, I could use some global object to pass the current string, next state, and result
 *
 */


(function () {
	var inputStrings
		, index
		, outputString
		, BLANK_LINE = /^\s*$/
		, HEADER_LINE = /^#.+/
		, DEBUG = false
		, handlers = {}
		, log = DEBUG ?
			console.log.bind(console)
			: function () {};

	function parse (markdownString) {
		var cargo = ["", ""]
			, nextStateCargo
			, state;

		inputStrings = markdownString.split("\n");
		index = -1;
		state = "initial";

		while (true) {
			log(state, cargo);
			nextStateCargo = handlers[state](cargo);
			if (state === "input finished") {
				break;
			}
			state = nextStateCargo.handler;
			cargo = nextStateCargo.cargo;
		}

		return outputString;
	}

	function hasNext () {
		if (DEBUG) {
			console.log("Current index = " + index)
		}
		return index + 1 < inputStrings.length;
	}

	function nextLine () {
		index += 1;
		return inputStrings[ index ];
	}

	handlers["initial"] = function (cargo) {
		var s, handler;

		if (!hasNext()) {
			handler = "input finished";
		}
		else {
			s = nextLine();
			if (BLANK_LINE.test(s)) { handler = "initial" }
			else if (HEADER_LINE.test(s)) { handler = "header" }
			else { handler = "paragraph start"; }
		}

		return { handler : handler, cargo : [s, cargo[1]] };
	};

	handlers["header"] = function (cargo) {
		var handler
			, result = cargo[1] + "<h1>" + cargo[0].substr(1) + "</h1>";

		if (hasNext()) {
			handler = "initial";
		}
		else {
			handler = "input finished";
		}

		return { handler : handler, cargo : [null, result] };
	};

	handlers["paragraph start"] = function (cargo) {
		var result = cargo[1] + "<p>" + cargo[0]
			, s
			, handler;

		if (!hasNext()) {
			handler = "paragraph end";
		} else {
			s = nextLine();
			if (s.match(BLANK_LINE)) {
				handler = "paragraph end";
			}
			else {
				handler = "paragraph in";
			}
		}

		return { handler : handler, cargo : [s, result]};
	};

	handlers["paragraph in"] = function (cargo) {
		var handler
			, currentLine = cargo[0]
			, result = cargo[1] + "\n" + currentLine;

		if (!hasNext()) {
			handler = "paragraph end";
		}
		else {
			currentLine = nextLine();
			if (currentLine.match(BLANK_LINE)) {
				handler = "paragraph end";
			}
			else {
				handler = "paragraph in";
			}
		}

		return { handler : handler, cargo : [currentLine, result]};
	};

	handlers["paragraph end"] = function (cargo) {
		var result;

		result = cargo[1] + "</p>";

		return { handler : "initial", cargo : [null, result] };
	};

	handlers["input finished"] = function (cargo) {
		outputString = cargo[1];
	};

	/** Export module if we are running in Node, add it to the global namespace otherwise */
	if (typeof window !== "undefined") {
		window.switch_md_parser = { parse : parse };
	} else {
		module.exports = { parse : parse };
	}
}());
