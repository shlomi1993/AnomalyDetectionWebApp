// Need the fs to read results.html file.
const fs = require('fs')

// This function parse the controller's json and create an HTML object that can be sent to the browser.
function sendHTML(req, res, result) {

    // If an error occured, send is ans stop.
    let jsons = JSON.parse(result);
    if (jsons[0].ID === '-1') {
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
            let a = '<font size=4 color=black>Anomaly ID:       ' + json.ID + '</font><br>';
            let b = '<font size=4 color=black>Start time step:  ' + json.startTimeStep + '</font><br>';
            let c = '<font size=4 color=black>End time step:    ' + json.endTimeStep + '</font><br>';
            let d = '<font size=4 color=black>Property 1:       ' + json.property1 + '</font><br>';
            let e = '<font size=4 color=black>Property 2:       ' + json.property2 + '</font><br>';
            anomalies += '<pre>' + a + b + c + d + e + '</pre>';
        });

        // Set list.
        html = html.replace('<!--anomalies-->', anomalies);

        // Send it to the iframe.
        res.write(html)


    }

    // Done.
    res.end();
}

// Expose sendHTML function (for Controller's use)
module.exports.sendHTML = sendHTML;