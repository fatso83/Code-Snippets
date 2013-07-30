var expect = require("expect.js");

function parserTests (parser) {
	var lorem = [
			"Lorem ipsum dolor sit amet,",
			" consectetur adipisicing elit, ",
			"sed do eiusmod tempor incididunt ut labore"]
		, LOREM_BLOCK = lorem.join("\n")
		, CONVERTED_LOREM_BLOCK = "<p>" + LOREM_BLOCK + "</p>"
		, HEADER = "# a header"
		, CONVERTED_HEADER = "<h1> a header</h1>";

	describe('parse', function () {
		it('should convert a hash-prefixed line to h1 header', function () {
			expect(parser.parse(HEADER)).to.eql(CONVERTED_HEADER);
		});

		it('should convert a standalone text line to a paragraph', function () {
			var text = "\nLorem ipsum dolor sit amet\n";
			expect(parser.parse(text)).to.be("<p>" + text.trim() + "</p>")
		});

		it('should convert a text block to a paragraphs', function () {
			expect(parser.parse(LOREM_BLOCK)).to.be(CONVERTED_LOREM_BLOCK);
		});

		it('should convert two standalone text blocks to two paragraph', function () {
			var text = [LOREM_BLOCK, "\n", LOREM_BLOCK].join("\n");
			expect(parser.parse(text)).to.be(
				CONVERTED_LOREM_BLOCK + CONVERTED_LOREM_BLOCK);
		});

		it('should handle header followed by block', function() {
			var text = [HEADER, LOREM_BLOCK].join("\n");
			expect(parser.parse(text)).to.equal(CONVERTED_HEADER + CONVERTED_LOREM_BLOCK)
		});

		it('should handle header, block, block', function() {
			var text = [HEADER, LOREM_BLOCK, "\n", LOREM_BLOCK].join("\n");
			expect(parser.parse(text)).to.equal(
				CONVERTED_HEADER + CONVERTED_LOREM_BLOCK + CONVERTED_LOREM_BLOCK)
		});

		it('should handle header, block, multiple blanks, block', function() {
			var text = [HEADER, LOREM_BLOCK, "\n", "\n","\n","\n", LOREM_BLOCK].join("\n");
			expect(parser.parse(text)).to.equal(
				CONVERTED_HEADER + CONVERTED_LOREM_BLOCK + CONVERTED_LOREM_BLOCK)
		});

		it('should handle blanks, block, header, block', function() {
			var text = ["\n", "\n","\n", HEADER,  "\n", LOREM_BLOCK].join("\n");
			expect(parser.parse(text)).to.equal(
				CONVERTED_HEADER + CONVERTED_LOREM_BLOCK);
		});

	});
}

// Test all the different implementations agains the same interface
parserTests(require("../src/asm-md-parser"));
