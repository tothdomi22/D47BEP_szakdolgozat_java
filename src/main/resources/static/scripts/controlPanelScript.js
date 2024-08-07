document.addEventListener("DOMContentLoaded", () => {

    const wateringSlider = document.getElementById("wateringDuration");
    const wateringValue = document.getElementById("wateringValue")
    const percentSlider = document.getElementById("wateringPercent");
    const percentValue = document.getElementById("wateringPercentValue");
    const tankValue = document.getElementById("tankDepthValue");
    const tankSlider = document.getElementById("tankDepth");
    let preset;
    let id;


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