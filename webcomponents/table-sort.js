/**
 * Lookup table for parsing various table cell types
 */
const cellParsers = {
  numeric(cell) {
    return parseInt(cell.innerText, 10);
  },
  datetime(cell) {
    // assuming <time> or other element with datetime attribute
    const datetimeElem = cell.querySelector("[datetime]");

    // fallback - try to parse
    if (!datetimeElem) {
      return new Date(cell.innerText);
    }

    return datetimeElem.getAttribute("datetime");
  },
  duration(cell) {
    // assuming <time> or other element with datetime attribute being an ISO Duration (ex. PT2H30M)
    const datetimeElem = cell.querySelector("[datetime]");

    if (!datetimeElem) {
      return 0;
    }

    const duration = datetimeElem.getAttribute("datetime");
    return parseISODurationIntoSeconds(duration);
  },
  text(cell) {
    return cell.innerText;
  },
};

function parseISODurationIntoSeconds(iso) {
  const match = iso.match(/PT(?:(\d+)H)?(?:(\d+)M)?(?:(\d+(?:\.\d+)?)S)?/);

  if (!match) return 0;

  const hours = parseInt(match[1] || "0", 10);
  const minutes = parseInt(match[2] || "0", 10);
  const seconds = parseFloat(match[3] || "0");

  return hours * 3600 + minutes * 60 + seconds;
}

/**
 * <table-sort numeric="2,5" datetime="3" duration="4" />
 *
 * Custom Element to progressively enhance static tables
 * with dynamic sorting behavior
 *
 * Does not re-write rows or cells: reorders existing elements
 *
 * Based on ideas, discussions and code from Raymon Camden,
 * David Copeland and Zach Leatherman <3
 */
class TableSort extends HTMLElement {
  #lastSortColumn = null;
  #sortAsc = true;
  #numericColumns = [];
  #datetimeColumns = [];
  #durationColumns = [];
  #tbody = null;

  connectedCallback() {
    const table = this.querySelector("table");

    if (!table) {
      console.warn(`${TableSort.name}: No table found. Exiting.`);
      return;
    }

    // require tbody and thead
    const tbody = table.querySelector("tbody");
    const thead = table.querySelector("thead");
    if (!tbody || !thead) {
      console.warn(`${TableSort.name}: No tbody or thead found. Exiting.`);
      return;
    }

    const parseColumnsFromAttribute = (attributeKey) =>
      (this.getAttribute(attributeKey) || "")
        .split(",")
        .map((x) => parseInt(x - 1, 10)); // make columns 0-based

    this.#numericColumns = parseColumnsFromAttribute("numeric");
    this.#datetimeColumns = parseColumnsFromAttribute("datetime");
    this.#durationColumns = parseColumnsFromAttribute("duration");

    // Enhance headers: indicate clickability
    const headers = thead.querySelectorAll("th");
    headers.forEach((h, columnIndex) => {
      h.style.cursor = "pointer";
      h.addEventListener("click", (e) => this.#clickHandler(e, columnIndex));
    });

    // copy body to this scope so we can use it again later
    this.#tbody = tbody;
  }

  /**
   * - side-effects
   */
  #clickHandler(e, columnIndex) {
    if (this.#lastSortColumn === columnIndex) {
      this.#sortAsc = !this.#sortAsc;
    } else this.#sortAsc = true;

    this.#lastSortColumn = columnIndex;

    const sorted = this.#sortRows(
      // nested table row elements are invalid HTML - not checking
      this.#tbody.querySelectorAll("tr"),
      columnIndex,
    );
    this.#renderTable(sorted);
  }

  /**
   * Update the DOM
   * /impure/
   * Uses about 25 ms painting/reattaching 2000 rows
   */
  #renderTable(sortedRows) {
    console.time("renderTable");

    // Renders the table in one reflow by batching updates in a DocumentFragment
    const fragment = document.createDocumentFragment();
    fragment.append(...sortedRows);
    this.#tbody.append(fragment);

    console.timeEnd("renderTable");
  }

  #columnType(column) {
    if (this.#numericColumns.indexOf(column) > -1) {
      return "numeric";
    } else if (this.#datetimeColumns.indexOf(column) > -1) {
      return "datetime";
    } else if (this.#durationColumns.indexOf(column) > -1) {
      return "duration";
    }
    return "text";
  }

  /**
   * Collects the row elements, sorts them and returns the sorted collection
   * /pure/
   * @param {HTMLCollection<HTMLTableRowElement>} rows
   * @param {number} columnToSort
   * @returns {HTMLElement[]}
   */
  #sortRows(tableRowCollection, columnToSort) {
    const rows = Array.from(tableRowCollection);
    const sortToggle = this.#sortAsc ? 1 : -1;
    const parseValue = cellParsers[this.#columnType(columnToSort)];

    rows.sort((rowA, rowB) => {
      const aCells = rowA.getElementsByTagName("td");
      const bCells = rowB.getElementsByTagName("td");
      const aVal = parseValue(aCells[columnToSort]);
      const bVal = parseValue(bCells[columnToSort]);

      if (aVal < bVal) return -1 * sortToggle;
      if (aVal > bVal) return 1 * sortToggle;
      return 0;
    });

    return rows;
  }
}

// normally one would not like to create side effects like this in a module
if (!customElements.get("table-sort"))
  customElements.define("table-sort", TableSort);
