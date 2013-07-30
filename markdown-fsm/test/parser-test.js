var expect = require("expect.js");

function parserTests (parser) {
	describe('parse', function () {
		it('should convert a stand-alone hash-prefixed line to h1 header', function () {
			expect(parser.parse("\n# a header\n")).to.eql("<h1> a header</h1>")
		});

		it('should convert a standalone text line to a paragraph', function() {
			var text =  "\nLorem ipsum dolor sit amet\n";
			expect(parser.parse(text)).to.be("<p>" + text.trim() + "</p>")
		});

		it.skip('should convert a standalone text block to a paragraph', function() {
			var text = [
				"Lorem ipsum dolor sit amet,",
				" consectetur adipisicing elit, ",
				"sed do eiusmod tempor incididunt ut labore"].join("");
			expect(parser.parse(text)).to.be("<p>" + text + "</p>")
		})
	});
}

// Test all the different implementations agains the same interface
parserTests(require("../src/asm-md-parser"));
