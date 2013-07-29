var expect = require("expect.js");

function parserTests (parser) {
	describe('parse', function () {
		it('should convert # to h1 tag', function () {
			expect(parser.parse("# a header")).to.eql("<h1> a header</h1>")
		});

		it('should convert a standalone text block to a paragraph', function() {
			var text = [
				"Lorem ipsum dolor sit amet,",
				" consectetur adipisicing elit, ",
				"sed do eiusmod tempor incididunt ut labore"].join("");
			expect(parser.parse(text)).to.eql("<p>" + text + "</p>")
		})
	});
}

// Test all the different implementations agains the same interface
parserTests(require("../src/asm-md-parser"));
