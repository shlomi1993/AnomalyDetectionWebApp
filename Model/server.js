console.log("server started");

var fs = require('fs');
var data = fs.readFileSync('words.json');
var words = JSON.parse(data);
console.log(words);

var express = require('express');
var app = express();
var server = app.listen(3000, listening);

function listening() {
    console.log("listening...");
}

app.use(express.static('website'));

app.get('/add/:word/:score?', addWord);

function addWord(request, response) {
    var data = request.params;
    var word = data.word;
    var score = Number(data.score);
    if (!score) {
        var reply = {
            msg: "Score is req!"
        };
        response.send(reply);
    } else {
        words[word] = score;
        var data = JSON.stringify(words, null, 4);
        fs.writeFile('words.json', data, finished);

        function finished(err) {
            var reply = {
                word: word,
                score: score,
                status: "success"
            }
            console.log('all set.');
            response.send(reply);
        }
    }

}



app.get('/all', sendAll);

function sendAll(request, response) {
    response.send(words);
}

app.get('/search/:word/', searchWord);

function searchWord(request, response) {
    var word = request.params.word;
    var reply;
    if (words[word]) {
        reply = {
            status: "found",
            word: word,
            score: words[word]
        }
    } else {
        reply = {
            status: "not found",
            word: word
        }
    }
    response.send(reply);
}