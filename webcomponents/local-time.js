const options = {
  weekday: "long",
  year: "numeric",
  month: "numeric",
  day: "numeric",
  hour: "numeric",
  minute: "numeric",
  second: "numeric",
};
// use default locale of the browser
const formatter = new Intl.DateTimeFormat(undefined, options);

/**
 * Convert ISO8601 timestamp to local time representation
 * - avoids displaying UTC time
 */
class LocalTime extends HTMLElement {
  connectedCallback() {
    const datetime = this.innerText;
    const parsedTimestamp = new Date(datetime);
    if (isNaN(parsedTimestamp)) return;

    this.innerHTML = `
      <time datetime="${datetime}">${formatter.format(parsedTimestamp)}</time>
      `;
  }
}

// normally one would not like to create side effects like this in a module
if (!customElements.get("local-time"))
  customElements.define("local-time", LocalTime);
