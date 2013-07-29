(function () {
	var AbstractStateMachine = require("./abstract-state-machine"),
		asm = new AbstractStateMachine,
		inputString, outputString;

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

	function parse (markdownString) {
		inputString = markdownString;
		asm.run(markdownString);
		return outputString;
	}

	/** Export module if we are running in Node, add it to the global namespace otherwise */
	if (typeof window !== "undefined") {
		window.asm_md_parser = { parse : parse };
	} else {
		module.exports = { parse : parse };
	}
}());
