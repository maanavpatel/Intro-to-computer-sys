from json import load

from flask.wrappers import Response
from googleplaces import GooglePlaces
import requests
from flask import Flask, json, request, jsonify
import os

app = Flask(__name__)

port = os.getenv("FLASK_RUN_PORT")
host = 'http://172.22.150.37'
mixUrl = 'http://cs240-adm-01.cs.illinois.edu:5000/microservice'

     
mixMiddleJson = {
    'port' : port,
    'ip' : host,
    'name': 'Nearby Hosptial',
    'creator': 'Maanav Patel',
    'tile': 'Nearby Hosptial',
    'dependencies' : []
}


API_KEY = 'AIzaSyD7qRPzmHRDuneoRip9FmtcIdDhfG2tIxQ'
@app.route('/', methods=["GET"])
def getNearestHospital():
    lat = (request.json)['latitude']
    lng = (request.json)['longitude']

    search_url = 'https://maps.googleapis.com/maps/api/place/findplacefromtext/json?'

    payload = {
        'input':'hospital',
        'inputtype':'textquery',
        'fields':'formatted_address,name',
        'locationbias':'point:'+str(lat)+','+str(lng),
        'key':API_KEY
    }
    result  = requests.get(search_url, params=payload)
    
    myobj = {
        "Hospital_Name":"",
        "Address": ""
        # "map": ""
        }

    if len(result.text) == 0:
        return jsonify(""), 200
    rjson = json.loads(result.text)
    hospital = rjson['candidates'][0]
    print(hospital)
    myobj['Hospital_Name'] = hospital['name']
    myobj['Address'] = hospital['formatted_address']
    # addr = myobj['Address']
    
    # addr = addr.replace(' ', '%20')

    # print(addr)
    # myobj['map'] = '<div class="mapouter"><div class="gmap_canvas"><iframe width="387" height="378" id="gmap_canvas" src="https://maps.google.com/maps?q='+ addr +'&t=&z=13&ie=UTF8&iwloc=&output=embed" frameborder="0" scrolling="no" marginheight="0"'+ 'marginwidth="0"></iframe><a href="https://www.embedgooglemap.net/blog/divi-discount-code-elegant-themes-coupon/"></a><br><style>.mapouter{ position:relative;text-align:right;height:378px;width:387px;}</style><a href="https://www.embedgooglemap.net">add a google map to website</a><style>.gmap_canvas { overflow:hidden;background:none!important;height:378px;width:387px;}</style></div></div>'
    # print(myobj['map'])
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
    x = requests.delete(url, json = delObj)
    if (x.status_code != 200 and x.status_code != 201):
        print(x.text, x.status_code)
        return "ERROR", 500
    return jsonify("success"), 200

@app.after_request
def add_headers(response):
    response.headers['Cache-Control'] = "max-age=2592000" #one month = 2592000
    response.headers['Age'] = 0
    return response