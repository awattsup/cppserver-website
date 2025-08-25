function loadBrews() {
    fetch('/api/brews')
        .then(res => res.json())
        .then(data => document.getElementById('brewList').textContent = JSON.stringify(data, null, 2));
}

function loadDevices() {
    fetch('/api/devices')
        .then(res => res.json())
        .then(data => document.getElementById('deviceList').textContent = JSON.stringify(data, null, 2));
}

document.getElementById('brewForm').onsubmit = function(e) {
    e.preventDefault();
    const form = e.target;
    const brew = {
        brewName: form.brewName.value,
        brewType: form.brewType.value,
        brewDate: form.brewDate.value
    };
    fetch('/api/brews', {
        method: 'POST',
        headers: {'Content-Type': 'application/json'},
        body: JSON.stringify(brew)
    }).then(() => loadBrews());
};

loadBrews();
loadDevices();
