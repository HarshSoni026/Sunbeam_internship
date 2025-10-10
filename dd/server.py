from flask import Flask, jsonify, request

#import mqtt client
import paho.mqtt.client as mqtt

#create client for mqtt
client = mqtt.Client("Publisher")

#create server

server = Flask(__name__)

@server.route("/temperature", methods = {'POST'})
def add_temperature():
    temp = request.form.get('temperature')
    loc = request.form.get('location')
    #insert temp and loc into database

    #if temperature above threshold, publish to mqtt topic
    d = dict()
    if float(temp) > 35.0:
        client.connect("localhost")
        client.publish("room/fan", "ON")
        client.disconnect()
        d = {
            "status" : "success",
            "Fan status" : "ON"
        }
    else:
        client.connect("localhost")
        client.publish("room/fan", "OFF")
        client.disconnect()
        d = {
            "status" : "success",
            "Fan status" : "OFF"
        }
    return jsonify(d)

server.run(debug=True)
    