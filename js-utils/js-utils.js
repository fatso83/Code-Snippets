/**
 * @author Carl-Erik Kopseng <carlerik@gmail.com>
 * @copyright None reserved. Use in whatever fashion you like, although a mention would be nice :)
 * @date 2010-2013
 */

/** @type UtilityLibrary */
var o = (function (g) {

	var settings = {
		decimalSeparator : ","
	};

	/** @class UtilityLibrary */
	return {
		/** @lends {UtilityLibrary} */

		/**
		 *   Creates a timed function
		 *   If the function is called before the timeout is reached, the timeout is reset, and started anew
		 *
		 *   @param {Function} fn the function to call
		 *   @param {Number} timeout the timeout to wait in milliseconds
		 *   @param {Object} [bindingObject] object to bind the function call to
		 */
		deferredResettingFunctionExecutor : function (fn, timeout, bindingObject) {
			var timerId, functionToCall = fn;
			return function () {
				var args = arguments;
				if (timerId) { clearTimeout(timerId); }
				timerId = setTimeout(function () {
					functionToCall.apply(bindingObject, args);
				}, timeout);
			};
		},

		/**
		 *  Attach this to textual input fields to prevent anything but legal characters
		 *  @return {Boolean} true if the input was legal, false otherwise
		 */
		preventIllegalInputInNumericField : function (evt) {
			var theEvent = evt || window.event;
			var key = theEvent.keyCode || theEvent.which;
			if (!this.isLegalInputForNumericField(key)) {
				theEvent.returnValue = false;
				if (theEvent.preventDefault) { theEvent.preventDefault();}
				return false;
			}
			return true;
		},

		isLegalInputForNumericField : function (keyCode) {
			var character = String.fromCharCode(keyCode);
			return (this.isNumber(character) ||
				this.isLegalSpecialKey(keyCode) ||
				character === settings.decimalSeparator);
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

		/**
		 * Compares two objects and sees if any of the fields in <code>field_names</code> differ
		 * Does not do any checking on whether the field is inherited through the prototype chain
		 *
		 * @param {Object} a the a object
		 * @param {Object} b the object whose fields might differ from a
		 * @param {Array} field_names the fields to compare
		 * @return {Boolean} true if changed, false otherwise
		 */
		fieldsDiffer : function (a, b, field_names) {
			for (var i = 0, len = field_names.length; i < len; i++) {
				var field = field_names[i];
				if (a[ field] !== b[field]) {
					return true;
				}
			}
			return false;
		},

		/**
		 * Create a namespaced object in <code>rootObject</code>
		 *
		 * Will not overwrite currently existing objects, but might add to their properties
		 * Will fail if any pre-existing overlapping names are not objects
		 *
		 * Example I: If there exists an object foo.bar.baz, where baz is an object, then
		 *    createObjectHierarchy("foo.bar.baz.babar") will succeed
		 * Example II: If there exists an object foo.bar.baz, where baz is a string, then
		 *    createObjectHierarchy("foo.bar.baz.babar") will not succeed
		 *
		 * @param {String} namespace a namespace in dotted notation. E.g. "com.foo.barsoft.utils"
		 * @param {Object} [rootObject] root object to create the hierarcy in. If left out, use the library's default global object given at initialization, usually window;
		 * @returns the namespaces object
		 */
		createObjectHierarchy : function (namespace, rootObject) {
			if (!rootObject) { rootObject = g;}

			function iter (current, existing) {
				var match = current.match(/([a-zA-Z0-9]+)\.(.+)/),
					first = current,
					rest = "",
					preExisting;

				if (current === "") { return existing; }

				if (match) {
					first = match[1];
					rest = match[2];
				}

				preExisting = first in existing;
				if (preExisting && existing[first] !== null) {
					if (typeof existing[first] !== "object") {
						var errorMsg = "Could not create namespace " + namespace + ". Pre-existing part " + first + " is not an object";
						throw new Error(errorMsg);
					}
				} else { existing[first] = {}; }

				return iter(rest, existing[first]);
			}

			return iter(namespace, rootObject);
		},

		/**
		 * @param {Boolean} condition
		 * @param {String} [msg]
		 */
		preCondition : function (condition, msg) {
			if (!condition) {
				throw new Error(msg ? msg : "Precondition failed!");
			}
		},

		isBlank : function (s) {
			return this.isEmpty(s.trim());
		},

		isEmpty : function (s) {
			return s.length === 0;
		}
	};
}(window));

o.createObjectHierarchy("no.kopseng.jsutils");

/** @type UtilityLibrary */
window.no.kopseng.jsutils = o;

