from googleplaces import GooglePlaces, types
import requests
from flask import Flask, request,json, jsonify
import os

app = Flask(__name__)

port = '5040'  #24002
host = 'http://127.0.0.1'
mixUrl = 'http://127.0.0.1:5000/microservice'

     
mixMiddleJson = {
    'port' : port,
    'ip' : host,
    'name': 'Country and alpha2 code',
    'creator': 'Maanav Patel',
    'tile': 'Country',
    'dependencies' : []
}

API_KEY = 'd47881129f35a5fde1fffc33b2b314f5'





@app.route('/', methods=["GET"])
def getCountry():
    if not (request.json):
        return {'err'}, 200
    lat = (request.json)['latitude']
    lng = (request.json)['longitude']

    url = "http://api.positionstack.com/v1/reverse?"

    querystring = {
        "query":str(lat)+','+str(lng),
        "result_type":"country",
        'access_key':API_KEY,
        'country_module':1
    }

    response = requests.request("GET", url, params=querystring)
    
    myobj = {
        "Country":"",
        "Alpha2": "",
        "Flag": "",
        }

    if len(response.text) == 0:
        return jsonify(""), 200

    rjson = json.loads(response.text)
    # print(rjson)
    data = rjson['data'][0]

    cm = data['country_module']
    country = cm['common_name']
    gl = cm['global']
    code = gl['alpha2']
    flag = '<h1>' + cm['flag'] + '</h1>'

    myobj['Country'] = country
    myobj['Alpha2'] = code
    myobj['Flag'] = flag

    return jsonify(myobj), 200


@app.route('/MIX/add', methods=["GET"])
def add_IM():
    print('add IM')
    url = mixUrl
    x = requests.put(url, json = mixMiddleJson)
    if (x.status_code != 200 and x.status_code != 201):
        print(x.text, x.status_code)
        return "ERROR", 500
    return jsonify("success"), 200

@app.route('/MIX/delete', methods=["GET"])
def delete_IM():
    print('delete IM')
    url = mixUrl
    delObj = {
        'port': port,
        'ip': host
    }
    x = requests.request('DELETE',url, json = delObj)
    if (x.status_code != 200 and x.status_code != 201):
        print(x.text, x.status_code)
        return "ERROR", 500
    return jsonify("success"), 200


@app.after_request
def add_headers(response):
    response.headers['Cache-Control'] = "max-age=31536000" #one month = 2592000
    response.headers['Age'] = 0
    return response