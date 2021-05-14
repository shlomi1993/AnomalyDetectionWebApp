const express = require("express");
const fileupload = require("express-fileupload");
const model = require("../model/searchInFile");

const app = express();

/*let commands = new Map();
commands.set("/", app.get)
*/
//Check if need to do like server.js


app.use(
  express.urlencoded({
    extended: false,
  })
);
app.use(fileupload());
// Determine root as ../view
app.use(express.static("../view"));

//GET retrieves resources
app.get("/", (req, res) => {
  res.sendFile("./index.html");
});

//POST submits new data to the server.
// input algo - HYBRID/REG, CSV file, another file
//output JSON - get JSON file from Model to View
app.post("/detect", (req, res) => {
  //Check how to do req.body.key - get selected option
  res.write("Algorithm type:" + "CSV filename:" + "another filename:")
  //Check if files are valid
  if(req.files) {
    //use given algo on files and find anomalies, also return it to view
  }
  res.end()
});

/*
//PUT updates existing data.
app.put("", (req, res) => {
  //CHECK IF NEED TO DO
});

//DELETE removes data.
app.delete("", (req, res) => {
  //CHECK IF NEED TO DO
});
*/



app.listen(8080);

