from flask import Flask
import os

app = Flask(__name__)

# Route for "/" for a web-based interface to this micro-service:
@app.route('/')
def index():
  from flask import render_template
  return render_template("index.html")

def cleanup(png_file):
  print('CLEANING UP')
  try:
    os.remove('temp/'+ png.filename)
    os.removedirs('temp')
    return
  except Exception as e:
    print('error in removing files/directory')


# Extract a hidden "uiuc" GIF from a PNG image:
@app.route('/extract', methods=["POST"])
def extract_hidden_gif():
  from flask import send_file, request
  os.makedirs('temp',exist_ok=True)

  png = request.files['png']
  if png:
    png_save_filepath = os.path.join('temp', png.filename)
    png.save(png_save_filepath)

    command = './png-extractGIF '+ png_save_filepath + ' output.gif' 
    cmd_resp = os.system(command)
    print('command', cmd_resp)
    
    if cmd_resp == 0 :
      print('cmdresp:\t', cmd_resp)
      try: 
        if os.path.exists('output.gif'):
          send = send_file('output.gif', attachment_filename='output.gif')
          print('send:\t', send)
          os.remove('output.gif')
          return send, 200
        else:
          return "Error in reading png", 500
        # cleanup(png.filename)
      except Exception as e:
        print('EXCEPTION:\t', e)
        return "Something went wrong! Check to make sure the provided png has a hidden .gif", 500
  return "Something went wrong! Check to make sure you provided a valid png", 500

  # ...your code here...
