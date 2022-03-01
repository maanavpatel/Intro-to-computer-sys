from logging import info
from flask import Flask, json, render_template, request, jsonify
from flask.wrappers import Response
import requests
import time

app = Flask(__name__)

# Route for "/" (frontend):
@app.route('/')
def index():
  return render_template("index.html")


db = {} #would be better as a proper database but ran out of time

cache = {} #key = url, value = {the response, age, max-age, time}

# Route for "/MIX" (middleware):
@app.route('/MIX', methods=["POST"])
def POST_MIX():
  location = request.form["location"]
  location = location.split(', ')
  if len(location) != 2:
    message = "Be sure you entered valid coordinates and try again!"
    return message, 404
  coor = [float(x) for x in location]
  lat = coor[0]
  lng = coor[1]

  
  information = []
  for entry in db:
    # print('entry', entry, db[entry]['name'])
    IM_url = db[entry]['url']
    IM_Data = ""
    resp = None
    requrl = f'{IM_url}/{lat},{lng}/'
    makereq = True
    if requrl in cache: 
      #update age
      curr_time = int(time.time())

      cache[requrl]['time'] = curr_time
      if cache[requrl]['time'] < cache[requrl]['max-time']:
        IM_Data = cache[requrl]['response']
        makereq = False

    if makereq:
      resp = requests.get(requrl)
      IM_Data = resp.json()
      curr_time = int(time.time())
      reqAge = resp.headers.get('Age')
      age = 0
      if reqAge:
        age = int(resp.headers.get('Age')) + curr_time
      else:
        age = curr_time
      maxage = int(resp.headers.get('Cache-Control').split('max-age=')[1]) + curr_time
      
      cache[requrl] = {
        'time' : age,
        'max-time': maxage,
        'response': IM_Data,
      }
    
    if len(IM_Data) == 0: #empty response means no data
      continue
    
    IM_Info = {
      "Name": db[entry]['name'],
      "Info":  IM_Data
    }
    information.append(IM_Info)

  if len(information) == 0:
    return jsonify("Sorry there are no MIX services available, please check later"), 201
  return jsonify(information),200


# Route for "/MIX/add/IM" (middleware):
@app.route('/MIX/IM/add',methods=["POST"])
def POST_MIX_ADD_IM():
  # print(name,IMurl)
  data = request.json

  if (not 'name' in data.keys() or not 'url' in data.keys()):
    return jsonify("INVALID DATA: be sure to provide a json like { name:<name>, url:<url> }"), 400
  
  name = data['name']
  url = data['url']
  
  if not (name+url) in db:      #add entry
    db[name+url] = {'name': name, 'url' : url}
    return jsonify("Success"), 201

  # print(db)
  return jsonify("Success"), 200

#IM needs to keep track of age regardless of how the client will function
#Middleware will keep track of age by itself to make quicker decisions

# Route for "/MIX/delete/IM" (middleware):
@app.route('/MIX/IM/delete',methods=["POST"])
def POST_MIX_DELETE_IM():
  data = request.json
  
  if (not 'name' in data.keys() or not 'url' in data.keys()):
    return jsonify("INVALID DATA: be sure to provide a json like { name:<name>, url:<url> }"), 500
  
  name = data['name']
  url = data['url']

  if not (name+url) in db:      
    return jsonify("Not found"), 404
  else:
    db.pop(name+url)
    return jsonify("Successfully removed"), 200



