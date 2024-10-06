
(async function() {
    fetch('http://localhost:8080/api/v1/lastweek')
        .then(response => response.json())
        .then(data => {
            const labels = data.map(item => item.createdAt);
            const temperatureValues = data.map(item => item.temperature);
            const humidityValues = data.map(item => item.humidity);
            const moistureValues = data.map(item => item.moisture);
            const waterLevelValues = data.map(item => item.waterLevel);
            const lightLevelValues = data.map(item => item.light);

            createChart('temperature', 'Temperature of the last 7 days', labels, temperatureValues)
            createChart('humidity', 'Humidity of the last 7 days', labels, humidityValues)
            createChart('moisture', 'Moisture of the last 7 days', labels, moistureValues)
            createChart('waterLevel', 'Water level of the last 7 days', labels, waterLevelValues)
            createChart('lightLevel', 'Light level of the last 7 days', labels, lightLevelValues)
            createChart('acquisitions6', 'idk placeholder', labels, moistureValues)

        })
})();

function createChart(elementId, labelText, label, data) {
    return new Chart(
        document.getElementById(elementId),
        {
            type: 'line',
            data: {
                labels: label, // Data labels, which won't be displayed
                datasets: [
                    {
                        label: labelText,
                        data: data,
                        borderColor: 'rgba(54, 162, 235, 1)',
                        backgroundColor: 'rgba(54, 162, 235, 0.2)',
                        borderWidth: 2,
                        fill: true
                    }
                ]
            },
            options: {
                scales: {
                    x: {
                        ticks: {
                            display: false,
                        },
                        title: {
                            display: true,   // Enables the title for the x-axis
                            text: 'Date and hour',    // Static label for the x-axis
                            font: {
                                size: 16     // Adjust the size of the label
                            }
                        }
                    },
                    y: {
                        ticks: {
                            font: {
                                size: 16
                            }
                        }
                    }
                }
            }
        }
    );
}