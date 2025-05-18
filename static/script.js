// Element references
const deliverySection = document.getElementById('deliverySection');
const loginModal = document.getElementById('loginModal');
const signupModal = document.getElementById('signupModal');
const overlay = document.getElementById('overlay');

// Show modal helper
function showModal(modal) {
  modal.style.display = 'block';
  overlay.style.display = 'block';
}

// Close all modals helper
function closeModals() {
  loginModal.style.display = 'none';
  signupModal.style.display = 'none';
  overlay.style.display = 'none';
}

// Show login/signup modals event listeners
document.getElementById('showLoginBtn').addEventListener('click', () => showModal(loginModal));
document.getElementById('showSignupBtn').addEventListener('click', () => showModal(signupModal));

// Login submit event
document.getElementById('loginSubmitBtn').addEventListener('click', () => {
  const username = document.getElementById('loginUsername').value;
  const password = document.getElementById('loginPassword').value;

  fetch('/login', {
    method: 'POST',
    headers: { 'Content-Type': 'application/json' },
    body: JSON.stringify({ username, password }),
  })
    .then(res => res.json())
    .then(data => {
      if (data.success) {
        closeModals();
        loadAuthenticatedUI();
      } else {
        alert('Login failed');
      }
    });
});

// Signup submit event
document.getElementById('signupSubmitBtn').addEventListener('click', () => {
  const username = document.getElementById('signupUsername').value;
  const password = document.getElementById('signupPassword').value;

  fetch('/signup', {
    method: 'POST',
    headers: { 'Content-Type': 'application/json' },
    body: JSON.stringify({ username, password }),
  })
    .then(res => res.json())
    .then(data => {
      if (data.success) {
        alert('Signup successful! You can log in now.');
        closeModals();
      } else {
        alert('Signup failed');
      }
    });
});

// Logout event
document.getElementById('logoutBtn').addEventListener('click', () => {
  fetch('/logout').then(() => {
    location.reload();
  });
});

// Check login status on load
fetch('/check_login')
  .then(res => res.json())
  .then(data => {
    if (data.logged_in) {
      loadAuthenticatedUI();
    }
  });

// UI update for authenticated user
function loadAuthenticatedUI() {
  const authSection = document.getElementById('authSection');
  authSection.querySelector('#showLoginBtn').classList.add('hidden');
  authSection.querySelector('#showSignupBtn').classList.add('hidden');
  document.getElementById('logoutBtn').classList.remove('hidden');
  deliverySection.classList.remove('hidden');
  initializeMapAndRouteFeatures();
}

// Leaflet map and route features initialization
function initializeMapAndRouteFeatures() {
  const map = L.map('map').setView([37.7749, -122.4194], 13);

  L.tileLayer('https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png', {
    attribution: '&copy; OpenStreetMap contributors',
  }).addTo(map);

  let routeLayer = null;
  let vehicleMarker = null;

  // Feature group for geofences
  const drawnItems = new L.FeatureGroup();
  map.addLayer(drawnItems);

  // Draw control for polygons and rectangles
  const drawControl = new L.Control.Draw({
    edit: { featureGroup: drawnItems },
    draw: {
      polygon: true,
      polyline: false,
      rectangle: true,
      circle: false,
      marker: false,
    },
  });
  map.addControl(drawControl);

  // Save geofence on creation
  map.on(L.Draw.Event.CREATED, function (e) {
    const layer = e.layer;
    drawnItems.addLayer(layer);

    const geojson = layer.toGeoJSON();
    const coords = geojson.geometry.coordinates;

    fetch('/save_geofence', {
      method: 'POST',
      headers: { 'Content-Type': 'application/json' },
      body: JSON.stringify({ geofence: coords }),
    }).then(() => console.log('Geofence saved'));
  });

  // Update geofence on edit
  map.on('draw:edited', function (e) {
    e.layers.eachLayer(layer => {
      const updatedCoords = layer.toGeoJSON().geometry.coordinates;

      fetch('/update_geofence', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ updated: updatedCoords }),
      });
    });
  });

  // Setup auto-suggestions for pickup and dropoff inputs
  setupAutoSuggest('pickup', 'pickupSuggestions', '/suggest_pickup');
  setupAutoSuggest('dropoff', 'dropoffSuggestions', '/suggest_dropoff');

  // Optimize route button click
  document.getElementById('optimizeRouteBtn').addEventListener('click', () => {
    const pickup = document.getElementById('pickup').value;
    const dropoff = document.getElementById('dropoff').value;
    const algorithm = document.getElementById('algorithm').value;

    fetch('/optimize_route', {
      method: 'POST',
      headers: { 'Content-Type': 'application/json' },
      body: JSON.stringify({ pickup, dropoff, algorithm }),
    })
      .then(res => res.json())
      .then(data => {
        if (routeLayer) map.removeLayer(routeLayer);

        const latlngs = data.route.map(p => [p.lat, p.lng]);
        routeLayer = L.polyline(latlngs, { color: 'blue' }).addTo(map);
        map.fitBounds(routeLayer.getBounds());

        animateRoute(latlngs);
      });
  });

  // Animate route function
  function animateRoute(coords) {
    if (vehicleMarker) map.removeLayer(vehicleMarker);

    vehicleMarker = L.marker(coords[0], {
      icon: L.icon({
        iconUrl: '/static/truck.png',
        iconSize: [32, 32],
      }),
    }).addTo(map);

    let i = 0;
    function move() {
      if (i < coords.length) {
        vehicleMarker.setLatLng(coords[i]);
        i++;
        setTimeout(move, 500);
      }
    }
    move();
  }

  // Average delivery time display
  document.getElementById('getAvgTimeBtn').addEventListener('click', () => {
    fetch('/average_time')
      .then(res => res.json())
      .then(data => {
        document.getElementById('averageTimeDisplay').innerText =
          `Average Delivery Time: ${data.average_time} minutes`;
      });
  });

  // Helper for live vehicle tracking simulation (optional)
  function simulateLiveTracking(path) {
    let i = 0;
    const interval = setInterval(() => {
      if (i >= path.length) {
        clearInterval(interval);
        return;
      }
      if (vehicleMarker) vehicleMarker.setLatLng(path[i]);
      i++;
    }, 1000);
  }
}

// Setup auto-suggestion on input fields
function setupAutoSuggest(inputId, suggestionBoxId, url) {
  const input = document.getElementById(inputId);
  const suggestionBox = document.getElementById(suggestionBoxId);

  input.addEventListener('input', () => {
    const query = input.value;
    if (!query) {
      suggestionBox.innerHTML = '';
      return;
    }

    fetch(`${url}?query=${encodeURIComponent(query)}`)
      .then(res => res.json())
      .then(data => {
        suggestionBox.innerHTML = '';
        data.forEach(item => {
          const div = document.createElement('div');
          div.textContent = item;
          div.addEventListener('click', () => {
            input.value = item;
            suggestionBox.innerHTML = '';
          });
          suggestionBox.appendChild(div);
        });
      });
  });
}
