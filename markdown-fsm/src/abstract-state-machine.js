/* The most basic state machine I could find. Based on (ripped off) python code from the article
   "Charming Python" by David Mertz (http://www.ibm.com/developerworks/library/l-python-state/index.html)
*/
(function () {
	"use strict";
	var _ = require("../lib/lodash");

	/** @class AbstractStateMachine */
	function AbstractStateMachine () {
		this.handlers = [];
		this.startState = null;
		this.endStates = [];
		this.DEBUG = false;
	}

	/**
	 *
	 * @param name
	 * @param handler
	 * @param isEndState is this an endstate?
	 */
	AbstractStateMachine.prototype.addState = function (name, handler, isEndState) {
		var upperName = name.toUpperCase();
		this.handlers[upperName] = handler;
		if (isEndState) { this.endStates.push(upperName); }
	};

	AbstractStateMachine.prototype.setStartState = function (name) {
		this.startState = name.toUpperCase();
	};

	/**
	 *
	 * @param {*} [startCargo] data to send to the first handler function
	 */
	AbstractStateMachine.prototype.run = function (startCargo) {
		var handler,
			handlerName,
			cargo,

			/** @type {Object}
			 * @description an object { handler, cargo }. The handler is the name of the next handler to invoke
			 */
				nextStep;

		if(this.DEBUG) console.log("\nStarting new run");

		cargo = startCargo;
		handlerName = this.startState;
		if (!handlerName) {
			throw new Error("No start state set");
		}

		if (this.endStates.length === 0) {
			throw new Error("There needs to be at least one end state");
		}

		do {
			handler = this.handlers[ handlerName];
			if(!handler) throw new Error("'" + handlerName + "' is not a handler");
			if(this.DEBUG) {console.log("Entering state " + handlerName)}
			nextStep = handler(cargo);
			if (_(this.endStates).contains(handlerName)) { break; }

			handlerName = nextStep.handler.toUpperCase();
			cargo = nextStep.cargo;
		} while (true);
	};

	/** Export module if we are running in Node, add it to the global namespace otherwise */
	if (typeof window !== "undefined") {
		window.AbstractStateMachine;
	} else {
		module.exports = AbstractStateMachine;
	}
}());
