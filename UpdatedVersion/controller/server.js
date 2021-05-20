const http = require("http");
const fs = require("fs");
const model = require("../model/searchInFile");

function displayFormCommand(req, res) {
  fs.readFile("../view/index.html", "utf8", (err, data) => {
    if (err) {
      console.error(err);
      return;
    } else {
      res.write(data);
      res.end();
    }
  });
}

function searchCommand(req, res) {
  var result = model.searchText("hello", "hello world\n goodbye\n");
  res.write(result);
  res.end();
}

let commands = new Map();

// Adding key-value to the map
commands.set("/", displayFormCommand);
commands.set("/search", searchCommand);
const server = http.createServer((req, res) => {
  if (commands.has(req.url)) {
    commands.get(req.url)(req, res);
  } else {
    res.write("Invalid request");
  }
});

server.listen(8080, () => {
  console.log("server started on port 8080");
});
