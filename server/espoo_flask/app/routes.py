from app import app
from flask import jsonify, request
import json


file_path = 'colors.json'
try:
    fp = open(file_path)
    fp.close()
except IOError:
    fp = open(file_path, 'w+')
    fp.write('""')
    fp.close()

colors = []


@app.route('/colors', methods=['GET'])
def get_colors():
    return jsonify({"colors":  colors})


@app.route('/colors/<name>', methods=['GET'])
def get_color(name):
    return_color = []
    for color in colors:
        if color["name"] == name:
            return_color.append(color)
    return jsonify(return_color)


@app.route('/colors', methods=['POST'])
def create_color():
    # TODO: this function can add multiple sames colors
    color = {
        'name': request.json['name'],
        'value': request.json['value']
    }
    print(colors)
    colors.append(color)
    print(colors)
    return jsonify(color), 201


@app.route('/colors/<name>', methods=['PUT'])
def update_color(name):
    for color in colors:
        if color["name"] == name:
            color['value'] = request.json.get('value', color['value'])
            return jsonify(color)
    return jsonify({'error': True})


@app.route('/colors/<name>', methods=['DELETE'])
def delete_color(name):
    for color in colors:
        if color["name"] == name:
            colors.remove(color)
            return jsonify(color)
    return jsonify({'error': True})


@app.route('/')
@app.route('/index')
def index():
    return "Hello, World from PyCharm"
