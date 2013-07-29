(function () {
	var AbstractStateMachine = require("./abstract-state-machine"),
		asm = new AbstractStateMachine,
		inputStrings, index = 0, outputString;

	function parse (markdownString) {
		inputStrings = markdownString.split("\n");
		asm.run();
		return outputString;
	}

	function nextLine() {
		return inputStrings[index++];
	}

	asm.addState("initial", function (result) {
		return { handler : "header", cargo : ""};
	}, false);
	asm.addState("header", function(result) {
		return { handler : "input finished", cargo : result + "<h1> a header</h1>"};
	}, false);
	asm.addState("input finished", function(result) {
		outputString = result;
	}, true);
	asm.setStartState("initial");

	/** Export module if we are running in Node, add it to the global namespace otherwise */
	if (typeof window !== "undefined") {
		window.asm_md_parser = { parse : parse };
	} else {
		module.exports = { parse : parse };
	}
}());
