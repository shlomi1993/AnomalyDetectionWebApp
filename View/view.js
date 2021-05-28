// Need the fs to read results.html file.
const fs = require('fs')

// This function parse the controller's json and create an HTML object that can be sent to the browser.
function sendHTML(req, res, result) {

    // If an error occured, send is ans stop.
    let jsons = JSON.parse(result);
    if (jsons.length == 1 && jsons[0].ID === '-1') {
        res.write(jsons[0].Error);
    }

    // Else, we have to create a new HTML page that will be shown in an iframe component.
    else {

        // Read an HTML template strored in the server.
        let html = fs.readFileSync('../View/results.html', 'utf8');

        // Replace parts with new data.
        html = html.replace('<!--algorithm-->', req.body.algorithm);
        html = html.replace('<!--threshold-->', req.body.new_threshold);
        html = html.replace('<!--trainName-->', req.files.training_file.name);
        html = html.replace('<!--testName-->', req.files.testing_file.name);

        // Transform anomalies to a list in HTML.
        let anomalies = '';
        jsons.forEach(json => {
            let a = '<tr><td>Anomaly ID:</td><td>' + json.ID + '</td></tr>';
            let b = '<tr><td>Start time step:</td><td>' + json.startTimeStep + '</td></tr>';
            let c = '<tr><td>End time step: </td><td>' + json.endTimeStep + '</td></tr>';
            let d = '<tr><td>Property 1:</td><td>' + json.property1 + '</font></tr>';
            let e = '<tr><td>Property 2:</td><td>' + json.property2 + '</font></tr>';
            anomalies += '<table style="width:80%">' + a + b + c + d + e + '</table><br>';
        });

        // Set list.
        if (anomalies === '') {
            anomalies = '<font>No anomalies found.</font><br>';
        }
        html = html.replace('<!--anomalies-->', anomalies);

        // Send it to the iframe.
        res.write(html)

    }

    // Done.
    res.end();
}

// Expose sendHTML function (for Controller's use)
module.exports.sendHTML = sendHTML;