from flask import Flask, render_template, request, jsonify
import requests
app = Flask(__name__)


db = {}


# TODO:
# PUT /<key> – Adds a versioned object
@app.route("/<key>", methods=["PUT"])
def PUT_key(key):
	val = request.data.decode("utf-8")
	if not key in db.keys(): 
		db[key] = [{'val': val}]
	else:
		k = db.get(key)
		k.append({'val': val})
		print(db)
	ind = len(db[key]) - 1
	value = db[key][ind].get('val')
	ret = {'value': value, "version": ind + 1}

	return jsonify(ret), 200


# GET /<key> – Retrieves the latest version of a key
@app.route("/<key>", methods=["GET"])
def GET_key(key):
	if key in db.keys():
		vals = db[key]
		ind = len(db[key]) - 1
		value = db[key][ind].get('val')
		ret = {'value': value, "version": ind + 1}
		return jsonify(ret), 200
	return "KEY NOT FOUND", 404


# GET /<key>/<version> – Retrieves a specific version of a key
@app.route("/<key>/<version>", methods=["GET"])
def GET_key_version(key,version):
	version = int(version)
	if key in db.keys():
		vals = db[key]
		
		ind = version - 1
		if ind in range(0,len(vals)):
			value = db[key][ind].get('val')
			ret = {'value': value, "version": version}
			return jsonify(ret), 200
		else:
				return "VERSION NOT FOUND", 404
	return "KEY NOT FOUND", 404

# DELETE /<key> – Completely deletes a key
@app.route("/<key>", methods=["DELETE"])
def DELETE_key(key):
	if key in db.keys():
		db.pop(key)
		return key, 200
	else:
		return "CANNOT DELETE NON EXISTENT KEY", 500
