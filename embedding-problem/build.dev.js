({
    mainConfigFile: "js/demo.js",
    appDir: ".",
    baseUrl: ".",
    dir: "/tmp/problem-build",
    optimize: "none",

     // To embed nested dependencies
     // Background for why this is not the default:
     // https://twitter.com/kopseng/status/305956838577430528
     findNestedDependencies: true,

    paths: {
        //jQuery is not included since it is already in require-jquery.js
        jquery: "empty:",
        requireLib: "js/require-jquery"
    },

    name: "js/demo",
    include: ["requireLib"]
})
