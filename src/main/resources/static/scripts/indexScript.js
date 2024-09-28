document.addEventListener('DOMContentLoaded', function() {
    fetch('http://localhost:8080/api/v1/latest')
        .then(response => response.json())
        .then(data => {
            // Display the latest data on the page
            document.getElementById('temperature').innerText = `${data.temperature}°C`;
            document.getElementById('humidity').innerText = `${data.humidity}%`;
            document.getElementById('moisture').innerText = `${data.moisture}%`;
            document.getElementById('waterLevel').innerText = `${data.waterLevel}cm`;
            document.getElementById('light').innerText = `${data.light}lum`;

            // Now send the prediction request with the latest data
            sendPredictionRequest(data.moisture, data.humidity, data.temperature);
        })
        .catch(error => console.error('Error fetching data:', error));

    function sendPredictionRequest(soilMoisture, humidity, temperature) {
        fetch(`/api/predict?soilMoisture=${soilMoisture}&humidity=${humidity}&temperature=${temperature}`, {
            method: 'POST'
        })
            .then(response => response.json())
            .then(prediction => {
                console.log('Prediction:', prediction);
                document.getElementById('prediction').innerText = `${prediction} hours`;
            })
            .catch(error => {
                console.error('Error in prediction request:', error);
            });
    }
});
