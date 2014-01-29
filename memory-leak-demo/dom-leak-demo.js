/* DOM leak demo
*
* @author Carl-Erik Kopseng, github.com/fatso83
*
* Shows the various stages of memory leaks of a typical dom handling object
* - creation, deletion of internal references to dom, removing all dom elements, deleting references to the objects
*
* Intended usage:
* - open up Chrome Dev Tools and the Profiles pane
* - paste the script into the console
* - start taking heap snapshots when requested in the console
* - select a snapshot, select comparison mode, and see the difference in objects created/deleted for each step
* - example images found in the GitHub repo
*/

window.ns = {};
var created = [];

(function () {

	var className = 'loading-loader',
		hideClass = 'hide',
		ns = window.ns;

	function isElement (obj) {
		return !!(obj && obj.nodeType === 1);
	}

	function hasLoaderDivInChildren (parentNode) {
		if (parentNode.children.length === 0)  return false;

		var children = parentNode.children,
			i = children.length;

		while (i--) {
			if (children[i].classList.contains(className)) { return true; }
		}
		return false;
	}

	function warn (string) {
		if (!this.silent) window.console.warn(string);
	}

	ns.LoadingLoader = function () {
		this.init.apply(this, arguments);
	};

	ns.LoadingLoader.prototype = {
		/**
		 * Construct the loader
		 * @param parentNode an HTMLElement
		 */
		init : function (parentNode) {
			if (!isElement(parentNode)) { throw new Error("Missing DOM Element in constructor"); }
			if (hasLoaderDivInChildren(parentNode)) { throw new Error("A loader was already attached to that node"); }

			var element = document.createElement('div');
			parentNode.appendChild(element);
			element.classList.add(className);
			element.classList.add(hideClass);

			var computedStyle = window.getComputedStyle(element, null);
			if (computedStyle.getPropertyValue('display') !== 'none') {
				warn.call(this, 'LoadingLoader: the "hide" class seems to be missing. The "display" property is not set to "none"');
			}

			this.element = element;
		},

		show : function () {
			this.element.classList.remove(hideClass);
		},

		hide : function () {
			this.element.classList.add(hideClass);
		},

		/**
		 * Removed the node from the DOM and the reference to the DOM element that was created
		 */
		destroy : function () {
			if (!this.element) return;

			if (this.element.parentNode) {
				this.element.parentNode.removeChild(this.element);
			}
			this.element = null;
		}
	};
}());


function createMany (num) {
	var fragment = document.createDocumentFragment();
	var element, l;
	for (var i = 0; i < num; i++) {
		element = document.createElement('div');
		l = new ns.LoadingLoader(element);
		fragment.appendChild(element);
		created.push(l);
	}
	document.body.appendChild(fragment);
}

ns.LoadingLoader.prototype.silent = true;

var seconds = 3000,
	showCreateHeapMessage = console.log.bind(console, "Create heap snapshot now!"),
	createStep = function (action, message) {
		return function (nextSteps) {
			if (!nextSteps) throw new Error("Missing next steps to execute");

			action();
			showCreateHeapMessage(message);

			if (nextSteps.length > 0) {
				var nextStep = nextSteps.shift();
				setTimeout(nextStep.bind(null, nextSteps), seconds);
			}
		}
	},
	steps = [
		createStep(function () { document.body.innerHTML = ''; }, "before creation"),
		createStep(createMany.bind(null, 10000), "after creation"),
		createStep([].map.bind(created, function (elem) { elem.destroy(); }), "after object self destruction"),
		createStep(function () { document.body.innerHTML = ''; }, "after removing all dom elements"),
		createStep(function () { created = []; }, "after deleting remaining references")
	],
	startStepping = function () { steps[0](steps.slice(1)); };

startStepping();
