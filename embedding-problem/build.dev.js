({
    mainConfigFile: "js/demo.js",
    appDir: ".",
    baseUrl: ".",
    dir: "/tmp/problem-build",
    optimize: "none",

    paths: {
        //jQuery is not included since it is already in require-jquery.js
        jquery: "empty:",
        requireLib: "js/require-jquery"
    },

    name: "js/demo",
    include: ["requireLib"]
})
