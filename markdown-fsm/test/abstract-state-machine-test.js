var expect = require("expect.js");
var AbstractStateMachine = require("../src/abstract-state-machine");

describe('AbstractStateMachine', function () {
	var asm;

	beforeEach(function() {
		asm = new AbstractStateMachine;
	});

	it('should be created by constructor without error', function () {
		expect(function () { new AbstractStateMachine; }).to.not.throwException();
	});

	describe('run', function () {
		it('should throw an exception if no start state is set', function () {
			expect(asm.run).to.throwException();
		});
	});

	describe('addState', function() {
		it('should add a function name to .endStates if isEndState', function() {
			asm.addState("fooState", function() {}, true);
			expect(asm.endStates).to.contain("fooState".toUpperCase());
		});
	});

	describe('typical setup and run', function () {
		it('should complete without error', function () {
			var finished = false;
			asm.addState("first", function (cargo) {
				return { handler : "second", cargo : "foo" };
			}, false);
			asm.addState("second", function (cargo) {
				return { handler : "end state", cargo : null};
			}, false);
			asm.addState("end state", function () {
				finished = true;
			}, true);
			asm.setStartState("first");

			asm.run();
			expect(finished).to.be(true);
		});
	});
});

