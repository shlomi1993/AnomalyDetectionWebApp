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