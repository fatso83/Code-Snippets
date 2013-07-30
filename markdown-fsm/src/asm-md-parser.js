(function () {
	var AbstractStateMachine = require("./abstract-state-machine")
		, asm = new AbstractStateMachine
		, inputStrings
		, index
		, outputString
		, BLANK_LINE = /^\s*$/
		, HEADER_LINE = /^#.+/
		, DEBUG = false;

	asm.DEBUG = DEBUG;

	function parse (markdownString) {
		inputStrings = markdownString.split("\n");
		index = -1;
		asm.run(["",""]);
		return outputString;
	}

	function hasNext () {
		if(DEBUG) console.log("Current index = " + index)
		return index + 1 < inputStrings.length;
	}

	function nextLine () {
		index += 1;
		return inputStrings[ index ];
	}

	asm.addState("initial", function (cargo) {
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
	}, false);

	asm.addState("header", function (cargo) {
		var handler
			, result = cargo[1] + "<h1>" + cargo[0].substr(1) + "</h1>" ;

		if(hasNext()) handler = "initial";
		else handler = "input finished";

		return { handler : handler, cargo : [null, result] };
	}, false);

	asm.addState("paragraph start", function (cargo) {
		var result = cargo[1] + "<p>" + cargo[0]
			, s
			, handler;

		if (!hasNext()) {
			handler = "paragraph end";
		} else {
			s = nextLine();
			if(s.match(BLANK_LINE)) handler = "paragraph end";
			else handler = "paragraph in";
		}

		return { handler : handler, cargo : [s, result]};
	}, false);

	asm.addState("paragraph in", function(cargo) {
		var handler
			, currentLine = cargo[0]
			, result = cargo[1] + "\n" + currentLine;

		if(!hasNext()) handler = "paragraph end";
		else {
			currentLine = nextLine();
			if(currentLine.match(BLANK_LINE)) handler = "paragraph end";
			else  handler = "paragraph in";
		}

		return { handler : handler, cargo : [currentLine, result]};
	},false);

	asm.addState("paragraph end", function (cargo) {
		var result;

		result = cargo[1] + "</p>";

		return { handler : "initial", cargo : [null, result] };
	}, false);

	asm.addState("input finished", function (cargo) {
		outputString = cargo[1];
	}, true);
	asm.setStartState("initial");

	/** Export module if we are running in Node, add it to the global namespace otherwise */
	if (typeof window !== "undefined") {
		window.asm_md_parser = { parse : parse };
	} else {
		module.exports = { parse : parse };
	}
}());
