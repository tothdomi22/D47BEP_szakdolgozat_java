document.addEventListener("DOMContentLoaded", () => {

    const wateringSlider = document.getElementById("wateringDuration");
    const wateringValue = document.getElementById("wateringValue")
    const percentSlider = document.getElementById("wateringPercent");
    const percentValue = document.getElementById("wateringPercentValue");
    const tankValue = document.getElementById("tankDepthValue");
    const tankSlider = document.getElementById("tankDepth");
    const presetText = document.getElementById("current-preset-text");
    let preset;
    let id;

    let socket;

    function connectWebSocket() {
        socket = new WebSocket('ws://192.168.0.110:81/');  // Replace with Arduino IP

        socket.onopen = function () {
            console.log('WebSocket is connected');
            document.getElementById('status').textContent = 'WebSocket connection established';
            document.getElementById('status').className = 'connected';
        };

        socket.onmessage = function (event) {
            console.log('Message received from Arduino: ' + event.data);
        };

        socket.onclose = function () {
            console.log('WebSocket is closed');
            document.getElementById('status').textContent = 'WebSocket connection closed. Reconnecting...';
            document.getElementById('status').className = 'disconnected';
            setTimeout(connectWebSocket, 3000); // Reconnect after 3 seconds
        };

        socket.onerror = function (error) {
            console.error('WebSocket error:', error);
            document.getElementById('status').textContent = 'WebSocket error. Reconnecting...';
            document.getElementById('status').className = 'disconnected';
            socket.close(); // Close and attempt to reconnect
        };
    }

    function sendData() {
        let data = {
            "wateringDuration": wateringSlider.value,
            "wateringThreshold": percentSlider.value,
            "tankDepth": tankSlider.value
        };
        console.log(data)
        if (socket && socket.readyState === WebSocket.OPEN) {
            socket.send(JSON.stringify(data));
            console.log('Data sent:', data);
        } else {
            console.log('WebSocket is not connected');
            document.getElementById('status').textContent = 'WebSocket is not connected';
            document.getElementById('status').className = 'disconnected';
        }
    }

    window.onload = function () {
        connectWebSocket();
    };


    fetch("http://localhost:8080/api/control-panel")
        .then(response => response.json())
        .then(data => {

                const item = data.find(obj => obj.preset === "preset1")
                wateringSlider.value = item.wateringDuration;
                percentSlider.value = item.wateringPercent;
                tankSlider.value = item.tankDepth;
                percentValue.innerText = `${item.wateringPercent} %`;
                wateringValue.innerText = `${item.wateringDuration} s`;
                tankValue.innerText = `${item.tankDepth} cm`;
                id = item.id;
                preset = item.preset;
        })

    document.getElementById('preset1').addEventListener('click', function(){
        fetch("api/control-panel")
            .then(response => response.json())
            .then(data => {

                const item = data.find(obj => obj.preset === "preset1")


                presetText.innerText = "Preset 1";
                preset = item.preset;
                wateringSlider.value = item.wateringDuration;
                percentSlider.value = item.wateringPercent;
                tankSlider.value = item.tankDepth;
                percentValue.innerText = `${item.wateringPercent} %`;
                wateringValue.innerText = `${item.wateringDuration} s`;
                tankValue.innerText = `${item.tankDepth} cm`;
                id = item.id;
            })
    })

    document.getElementById('preset2').addEventListener('click', function(){
        fetch("api/control-panel")
            .then(response => response.json())
            .then(data => {

                const item = data.find(obj => obj.preset === "preset2")

                presetText.innerText = "Preset 2";
                wateringSlider.value = item.wateringDuration;
                preset = item.preset;
                percentSlider.value = item.wateringPercent;
                tankSlider.value = item.tankDepth;
                percentValue.innerText = `${item.wateringPercent} %`;
                wateringValue.innerText = `${item.wateringDuration} s`;
                tankValue.innerText = `${item.tankDepth} cm`;
                id = item.id;
            })
    })

    document.getElementById('update-form').addEventListener('submit', function(event) {
        event.preventDefault()
        let formData = {
            wateringDuration: document.getElementById('wateringDuration').value,
            wateringPercent: document.getElementById('wateringPercent').value,
            tankDepth: document.getElementById('tankDepth').value,
            preset: preset,
            id: id,
        };
        fetch("api/control-panel/update", {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json'
            },
            body: JSON.stringify(formData),
        })
            .then(response => response.json())
            .catch(error => {
                console.error('Error: ', error);
            });
    })

    percentSlider.addEventListener("input", () => {
        percentValue.innerText = `${percentSlider.value} %`;
    })

    wateringSlider.addEventListener("input",() => {
        wateringValue.innerText = `${wateringSlider.value} s`;
    })

    tankSlider.addEventListener("input", () => {
        tankValue.innerText = `${tankSlider.value} cm`
    })

})