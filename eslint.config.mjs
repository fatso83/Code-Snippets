import { defineConfig, globalIgnores } from "eslint/config";
import globals from "globals";
import js from "@eslint/js";
import html from "eslint-plugin-html";

export default defineConfig([
  // historic folders
  globalIgnores([
    "embedding-problem/",
    "js-utils/",
    "markdown-fsm/",
    "memory-leak-demo",
    "studweb",
  ]),

  {
    files: ["**/*.{js,mjs,cjs,html}"],
    plugins: { js, html },
    languageOptions: {
      globals: globals.browser,
      parserOptions: {
        ecmaVersion: 13, // ES 2022
        sourceType: "module",
      },
    },
    extends: ["js/recommended"],
    rules: {
      "prefer-const": "error",
      "no-var": "error",
    },
  },
]);
