from printer_utilities import Slicer, OBJGeometry
import json
import random
from paho.mqtt import client as mqtt_client

broker = '192.168.0.168'
port = 1883
topic_slice = "mosquitto/printer/slice"
client_id = f'publish-{random.randint(200, 1600)}'
username = 'mosquitto'
password = '1234'

geometry = OBJGeometry('cone.obj')
slicer = Slicer(geometry, dots_per_side = 16)
layers = slicer.slice(thickness = 1.0)
def connect_mqtt():
    def on_connect(client, userdata, flags, rc):
        if rc==0:
            print("Successfully connected to MQTT broker")
        else:
            print("Failed to connect, return code %d", rc)


    client = mqtt_client.Client(client_id)
    client.username_pw_set(username, password)
    client.on_connect = on_connect
    client.connect(broker, port)
    return client
def publish(client, topic):
    data = ""
    for lay in layers:
        str = lay.__str__()
        sli = json.dumps(str.replace('\n', "$") + "$$")
        data = data + sli
    result = client.publish(topic, data)
    msg_status = result[0]
    if msg_status == 0:
        print(f"message : {data} sent to topic {topic}")
    else:
        print(f"Failed to send message to topic {topic}")
def main():
    client = connect_mqtt()
    client.loop_start()
    publish(client, topic_slice)
    client.loop_stop()

#    print(lay)
#    print('\n')
if __name__ == '__main__':
    main()