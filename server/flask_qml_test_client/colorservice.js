function getColors(cb) {
    // GET http://localhost:5000/colors
    request('GET', '/colors', null, cb)
}

function createColor(entry, cb) {
    // POST http://localhost:5000/colors
    request('POST', null, entry, cb)
}

function getColor(name, cb) {
    // GET http://localhost:5000/colors/${name}
    request('GET', name, null, cb)
}

function updateColor(name, entry, cb) {
    // PUT http://localhost:5000/colors/${name}
    request('PUT', name, entry, cb)
}

function deleteColor(name, cb) {
    // DELETE http://localhost:5000/colors/${name}
    request('DELETE', name, null, cb)
}

var BASE = 'http://localhost:5000';

function request(verb, endpoint, obj, cb) {
    print('request: ' + verb + ' ' + BASE + (endpoint ? '/' + endpoint : ''))
    var xhr = new XMLHttpRequest()
    xhr.onreadystatechange = function() {
        print('xhr: on ready state change: ' + xhr.readyState)
        if(xhr.readyState === XMLHttpRequest.DONE) {
            if(cb) {
                var res = JSON.parse(xhr.responseText.toString())
                cb(res)
            }
        }
    }
    xhr.open(verb, BASE + (endpoint ? '/' + endpoint : ''))
    xhr.setRequestHeader('Content-Type', 'application/json')
    xhr.setRequestHeader('Accept', 'application/json')
    var data = obj ? JSON.stringify(obj) : ''
    xhr.send(data)
}
