/**
 * @author Carl-Erik Kopseng <carlerik@gmail.com>
 * @copyright None reserved. Use in whatever fashion you like, although a mention would be nice :)
 * @date 2012->2013
 */

/* A namespace for the utility functions to live in to prevent namespace pollution */
(function (g) {

	/* Local Underscore/Lo-Dash reference - if present */
	var _ = g._;

	var utilities = {

		settings : {
			decimalSeparator : ","
		},

		/**
		 *   Creates a timed function
		 *   If the function is called before the timeout is reached, the timeout is reset, and started anew
		 *
		 *   @param {Function} fn the function to call
		 *   @param {Number} timeout the timeout to wait in milliseconds
		 *   @param {Object} [bindingObject] optional object to bind the function call to
		 *
		 *   Example:
		 *   var i = 0;
		 *   var incrementer = delayedResettingFunctionExecuto(function { i++; }, 1000);
		 *   //execute three calls sequentially that finish within a few millis
		 *   incrementer();incrementer();incrementer();
		 *   i === 1; // -> true
		 *
		 */
		delayedResettingFunctionExecutor : function (fn, timeout, bindingObject) {
			var timerId, functionToCall = fn;
			return function () {
				if (timerId) { clearTimeout(timerId); }
				timerId = setTimeout(functionToCall.bind(bindingObject), timeout);
			};
		},

		/** Attach this to textual input fields to prevent anything but legal characters */
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

		/** General comparison function to use with sorting functions */
		stringComparitor : function (a, b) {
			if (a < b) //sort string ascending
			{
				return -1;
			}
			if (a > b) {
				return 1;
			}
			return 0; //default return value (no sorting)
		},

		/*********************************************************************************
		 ** Utility functions that depend on the Underscore or Lo-Dash library
		 *********************************************************************************/

		/**
		 * Compares two objects and sees if any of the fields in <code>field_names</code> differ
		 * @param {Object} a the a objects
		 * @param {Object} b the object whose fields might differ from the a
		 * @param {Array} fields the fields to compare
		 * @return {Boolean} true if changed, false otherwise
		 */
		fieldsDiffer : function (a, b, fields) {
			var fields_a = _.pick.apply(_, [a].concat(fields));
			var fields_b = _.pick.apply(_, [b].concat(fields_a));
			return (!_.isEqual(fields_a, fields_b));
		}
	};

	if (typeof(g.no) === "undefined") {g.no = {};}
	if (!g.no.kopseng) { g.no.kopseng = {};}
	if (!g.no.kopseng.jsutils) { g.no.kopseng.jsutils = utilities;}
}(window));

