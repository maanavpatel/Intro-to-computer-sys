from googleplaces import GooglePlaces, types
import requests
from flask import Flask, request,json, jsonify
import os

app = Flask(__name__)

port = os.getenv("FLASK_RUN_PORT")  #24001
host = 'http://172.22.150.37'
mixUrl = 'http://cs240-adm-01.cs.illinois.edu:5000/microservice'

     
mixMiddleJson = {
    'port' : '24012',
    'ip' : host,
    'name': '5 Richest Celebrities',
    'creator': 'Maanav Patel',
    'tile': 'Rich Celebrities',
    'dependencies' : [
        {
            'host': 'http://172.22.150.37',
            'port': '24002',
            'dependencies':[]
        }
    ]
}

API_KEY = 'e24c3f4d57msh8b15e7492cbce0bp19949cjsn60c94b53d728'

@app.route('/', methods=["GET"])
def getNearestHospital():
    lat = (request.json)['latitude']
    lng = (request.json)['longitude']

    #call the country code IM

    country_url = 'http://172.22.150.37:24002/'
    country_resp = requests.request("GET", country_url, json=request.json)
    country_json = json.loads(country_resp.text)

    country_code = country_json['Alpha2']
    
    url = "https://celebrity-by-api-ninjas.p.rapidapi.com/v1/celebrity"

    querystring = {"nationality":country_code}

    headers = {
        'x-rapidapi-host': "celebrity-by-api-ninjas.p.rapidapi.com",
        'x-rapidapi-key': API_KEY
        }

    response = requests.request("GET", url, headers=headers, params=querystring)

    rjson = json.loads(response.text)

    print(rjson)
    networth_order = sorted(rjson, key = lambda i: i['net_worth'], reverse=True)
    top5 = networth_order
    if len(networth_order) > 5:
        top5 = networth_order[:5]
    
    celebs = []

    for celeb in top5:
        info = {
            'Name': '<h5 style="background-color:MediumSeaGreen;">' + celeb['name'] + '</h5>',
            'Net Worth': '<p style="border:2px solid MediumSeaGreen;">' + celeb['net_worth'] + '</p>'
        }
        celebs.append(info)

    myObj = {
        "5 of the Richest Celebrities in the Country" : celebs
    }
    print(myObj)
    return jsonify(myObj), 200

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
    response.headers['Cache-Control'] = "max-age=2592000" #one month = 2592000
    response.headers['Age'] = 0
    return response