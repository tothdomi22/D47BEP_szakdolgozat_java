
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
            const siteTrafficValues = [];

            for (let i = 0; i < 42; i++) {
                let randomNumber = Math.floor(Math.random() * 171); // Random number between 0 and 170 (inclusive)
                siteTrafficValues.push(randomNumber);
            }


            createChart('temperature', 'Temperature of the last 7 days', labels, temperatureValues)
            createChart('humidity', 'Humidity of the last 7 days', labels, humidityValues)
            createChart('moisture', 'Moisture of the last 7 days', labels, moistureValues)
            createChart('waterLevel', 'Water level of the last 7 days', labels, waterLevelValues)
            createChart('lightLevel', 'Light level of the last 7 days', labels, lightLevelValues)
            createChart('siteTraffic', 'Website traffic of the last 7 days', labels, siteTrafficValues)
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
                        fill: true,
                        tension: 0.2
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
                            text: 'Date and hour (4 hour intervals)',    // Static label for the x-axis
                            font: {
                                size: 12     // Adjust the size of the label
                            },
                            color: 'rgba(180,180,180,0.85)'
                        },
                        grid: {
                            color: 'rgba(180,180,180,0.2)'
                        }
                    },
                    y: {
                        ticks: {
                            font: {
                                size: 12
                            },
                            color: 'rgba(180,180,180,0.85)'
                        },
                        grid: {
                            color: 'rgba(180,180,180,0.2)'
                        }
                    },

                }
            }
        }
    );
}