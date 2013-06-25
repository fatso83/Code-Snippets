/**
 * @author Carl-Erik Kopseng <carlerik@gmail.com>
 * @copyright None reserved. Use in whatever fashion you like, although a mention would be nice :) */
 */

/* A namespace for the utility functions to live in to prevent namespace pollution */
(function (g) {
	var utilities = {

		settings : {
			decimalSeparator : ","
		},

		/**
		 *   Creates a timed function
		 *   If the function is called before the timeout is reached, the timeout is reset, and started anew
		 *
		 *   @param timeout the timeout to wait
		 *   @param fn the function to call
		 *
		 *   Example:
		 *   var i = 0;
		 *   var incrementer = delayedResettingFunctionExecuto(function { i++; }, 1000);
		 *   //execute three calls sequentially that finish within a few millis
		 *   incrementer();incrementer();incrementer();
		 *   i === 1; // -> true
		 *
		 */
		delayedResettingFunctionExecutor : function (fn, timeout) {
			var timerId, functionToCall = fn;
			return function () {
				if (timerId) { clearTimeout(timerId); }
				timerId = setTimeout(functionToCall, timeout);
			};
		},


		preventIllegalInputInNumericField : function (evt) {
			var theEvent = evt || window.event;
			var key = theEvent.keyCode || theEvent.which;
			if (!this.isLegalInputForNumericField(key)) {
				theEvent.returnValue = false;
				if (theEvent.preventDefault) { theEvent.preventDefault();}
			}
		},

		isLegalInputForNumericField : function (keyCode) {
			var character = String.fromCharCode(keyCode);
			return (this.isNumber(character) ||
				this.isLegalSpecialKey(keyCode) ||
				character === this.settings.decimalSeparator);
		},

		isNumber : function (stringKey) {
			return (/[0-9]/).test(stringKey);
		},

		isLegalSpecialKey : function (keyCode) {
			var keyCodes = {
				BACKSPACE : 8,
				TAB       : 9,
				ENTER     : 13,
				ESC       : 27,
				SPACE     : 32,
				LEFT      : 37,
				UP        : 38,
				RIGHT     : 39,
				DOWN      : 40,
				DEL       : 46
			};
			for (var key in keyCodes) {
				if (keyCodes[key] === keyCode) {
					return true;
				}
			}
			return false;
		},

		/**
		 * create iso date string
		 * @param {Date} date
		 * @return {String} iso
		 */
		isoDateString : function (date) {
			var dayOfMonth = date.getDate(),
				monthOfYear = date.getMonth() + 1,
				fullYear = date.getFullYear();

			function pad (num) {
				return (num < 10) ? "0" + num : "" + num;
			}

			return fullYear + "-" + pad(monthOfYear) + "-" + pad(dayOfMonth);
		},

		stringComparitor : function (a, b) {
			if (a < b) //sort string ascending
			{
				return -1;
			}
			if (a > b) {
				return 1;
			}
			return 0; //default return value (no sorting)
		}
	};

	if (typeof(g.no) === "undefined") {g.no = {};}
	if (!g.no.kopseng) { g.no.makingwaves = {};}
	if (!no.kopseng.jsutils) { g.no.makingwaves.utilities = utilities;}
}(window));

