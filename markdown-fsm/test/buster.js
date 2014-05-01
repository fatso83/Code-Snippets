var config = module.exports;

config["My tests"] = {
    rootPath: "../",
    //environment: "browser", // or "node"
    environment: "node",
    sources: [
        "lib/lodash.js",
        "src/*.js"
    ],
    tests: [
        "test/*-test.js",
        "test/*-tests.js"
    ]
}

