from flask import Flask, render_template, request, jsonify
import requests
app = Flask(__name__)

from pymongo import MongoClient
mongo = MongoClient('localhost', 27017)
db = mongo["mp7-state-server"]


# TODO:
# PUT /<key> – Adds a versioned object
@app.route("/<key>", methods=["PUT"])
def PUT_key(key):
    val = request.data.decode("utf-8")

    if not key in db.list_collection_names():       #key does not exist, start at version 1
        col = db[key]
        record = {              #make record from scratch
            "value" : val,
            "version" : 1
        }
        col.insert(record)   #insert
        return "Success", 200
    else:
        col = db[key]

        for x in col.find().sort("version", -1):  #look for record with key, order by version desc
            new_ver = x["version"] + 1  #inc version number since the key exists
            record = {
                "value" : val,
                "version" : new_ver
            }
            col.insert(record) #ins new record
            return "Success", 200    #Success    
    
    return "Something went wrong, sorry", 500

# GET /<key> – Retrieves the latest version of a key
@app.route("/<key>", methods=["GET"])
def GET_key(key):
    col = db[key]
    for x in col.find({},{"_id":0}).sort("version", -1):
        print(x)
        return jsonify(x), 200
    return "NOT FOUND", 404


		
# GET /<key>/<version> – Retrieves a specific version of a key
@app.route("/<key>/<version>", methods=["GET"])
def GET_key_version(key,version):
    version = int(version)

    col = db[key]
    for x in col.find({},{"_id":0}).sort("version", -1):
        v = x.get('version')
        if v == version:
            print(x)
            return jsonify(x), 200
    return "NOT FOUND", 404


# DELETE /<key> – Completely deletes a key
@app.route("/<key>", methods=["DELETE"])
def DELETE_key(key):
    col = db[key]
    if (col):
        col.drop()
        return "SUCCESS",200
    return "NOT FOUND", 404
