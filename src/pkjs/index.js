var backendAPIKey = '';
var backendDatabase = 'pebble-emoter';
var backendCollection = 'emotions-history';
var baseAPIUrl = 'https://api.mlab.com/api/1/databases/' + backendDatabase + '/collections/' + backendCollection  + '?apiKey=';
var fullAPIUrl = baseAPIUrl + backendAPIKey

function iconFromWeatherId(weatherId) {
  if (weatherId < 600) {
    return 2;
  } else if (weatherId < 700) {
    return 3;
  } else if (weatherId > 800) {
    return 1;
  } else {
    return 0;
  }
}

function fetchWeather(emotion) {
  console.log("Testing.......")
  var req = new XMLHttpRequest();
  req.setRequestHeader("Content-Type", "application/json;charset=UTF-8");
  req.open('POST', fullAPIUrl, true);

  req.onload = function () {
    if (req.readyState === 4) {
      if (req.status === 200) {
        console.log("Response: " + req.responseText);
        Pebble.sendAppMessage({
          'WEATHER_ICON_KEY': "",
          'WEATHER_TEMPERATURE_KEY': req.responseText ,
          'WEATHER_CITY_KEY': ""
        });
      } else {
        console.log('Error : ' + req.statusText);
      }
    }
  };
  console.log("sending...");
  var date = new Date();
  var dateStr = date.toDateString();
  var timeStr = date.toLocaleTimeString();
  var dateTimeStr = dateStr + " " + timeStr;
  req.send(JSON.stringify({ "emotion": emotion, "date": dateTimeStr }));
  console.log("sending complete");
}



function locationError(err) {
  console.warn('location error (' + err.code + '): ' + err.message);
  Pebble.sendAppMessage({
    'WEATHER_CITY_KEY': 'Loc Unavailable',
    'WEATHER_TEMPERATURE_KEY': 'N/A'
  });
}

var locationOptions = {
  'timeout': 15000,
  'maximumAge': 60000
};

Pebble.addEventListener('ready', function (e) {
  console.log('connect!' + e.ready);
  console.log(e);
});

Pebble.addEventListener('appmessage', function (e) {
  
  console.log(e.payload['0']);
  fetchWeather(e.payload['0']);
  console.log("Type: " + e.type);
  console.log("Payload : " + e.payload);
   console.log('Got message: ' + JSON.stringify(e.payload));
  console.log('message!');
});

Pebble.addEventListener('webviewclosed', function (e) {
  console.log('webview closed');
  console.log(e.type);
  console.log(e.response);
});