
// Hello World Example
var msg = 'Hello World';
console.log(msg);
console.log(msg.charAt(1));

// SearchInFile Example.
function searchText(key, text) {
    var result = ''
    text.split("\n").forEach(row => {
        if (row.search(key) != -1) {
            result += row + '\n'
        }
    })
    return result
}
module.exports.searchText = searchText