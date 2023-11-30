from tkinter import *
from paho.mqtt import client as mqtt_client
import json

from tkinter import ttk

from tkinter import ttk
from tkinter import filedialog as fd
from tkinter.messagebox import showinfo

from paho.mqtt import client as mqtt_client
import json
import random
import time

broker = '192.168.0.168'
port = 1883

#topic_sub = "mosquitto/printer/"


topic_ph = "mosquitto/printer/ph"
topic_cond = "mosquitto/printer/cond"
topic_curr = "mosquitto/printer/curr"
topic_volt = "mosquitto/printer/volt"




# Generate a Client ID with the publish prefix.
client_id = f'publish-{random.randint(200, 1600)}'
username = 'mosquitto'
password = '1234'



def connect_mqtt() -> mqtt_client:
    def on_connect(client, userdata, flags, rc):
        if rc == 0:
            print("Connected to MQTT Broker!")
        else:
            print("Failed to connect, return code %d\n", rc)

    client = mqtt_client.Client(client_id)
    client.username_pw_set(username, password)
    client.on_connect = on_connect
    client.connect(broker, port)
    return client

def subscribe(client: mqtt_client):
    def ph_message(client, userdata, msg):
        print(f"Received ph  `{msg.payload.decode()}`")
        y = json.loads(msg.payload.decode())

        ph = str(json.loads(msg.payload.decode()))

        ph_label.config(text=ph + "",
                          fg="black")

    client.subscribe(topic_ph)
    client.on_message = ph_message



window = Tk()
window.title("Slicer")
window.geometry('1280x700')
window.resizable(False,False)
window.configure(bg="white")

# Create Label
ph_label = Label(window,
                 text= "",
                 bg="white",
                 fg="black",
                 font=("Helvetica", 16))

ph_label.place(x=80,y=90)

cond_label = Label(window,
                 text= "",
                 bg="white",
                 fg="black",
                 font=("Helvetica", 16))

cond_label.place(x=360,y=90)

volt_label = Label(window,
                 text= "",
                 bg="white",
                 fg="black",
                 font=("Helvetica", 16))

volt_label.place(x=460,y=90)




# open file dialog



def select_file():
    filetypes = (
        ('3d model files', '*.obj'),
        ('All files', '*.*')
    )

    filename = fd.askopenfilename(
        title='Открыть файл',
        initialdir='/',
        filetypes=filetypes)

    showinfo(
        title='Selected File',
        message=filename
    )


# open button
open_button = ttk.Button(
    window,
    text='Открыть файл',
    command=select_file
)
open_button.place(x=800,y=250)
#open_button.pack(expand=True)

# Function for move stepper
def move_up_10():
    myLabel =Label(window, text="Вверх")
    myLabel.pack()

def move_down_10():
    myLabel =Label(window, text="Вниз")
    myLabel.pack()

def move_up_1():
    myLabel =Label(window, text="Вверх")
    myLabel.pack()

def move_down_1():
    myLabel =Label(window, text="Вниз")
    myLabel.pack()

def move_up_01():
    myLabel =Label(window, text="Вверх")
    myLabel.pack()

def move_down_01():
    myLabel =Label(window, text="Вниз")
    myLabel.pack()


# input field command
def voltage():
    enter_text = "Введеное значение"+voltage_in.get()
    lebel_voltage = Label(window, text=enter_text)
    lebel_voltage.pack()



def time():
    enter_text = "Введеное значение"+time_in.get()
    lebel_current = Label(window, text=enter_text)
    lebel_current.pack()

def height():
    enter_text = "Введеное значение"+height_in.get()
    lebel_height = Label(window, text=enter_text)
    lebel_height.pack()


# Labels

my_label1 = Label(window,     # ph
                 text="pH",
                 bg="white",
                 fg="black",
                 font=("Helvetica", 16))
my_label1.place(x=80,y=43)

my_label2 = Label(window, # Conductivity
                 text="Проводимось, мСм/см",
                 bg="white",
                 fg="black",
                 font=("Helvetica", 16))
my_label2.place(x=200,y=43)

my_label3 = Label(window, # Voltage
                 text="Напряжение, В",
                 bg="white",
                 fg="black",
                 font=("Helvetica", 16))
my_label3.place(x=500,y=43)


my_label4 = Label(window, # Current
                 text="Ток, мА",
                 bg="white",
                 fg="black",
                 font=("Helvetica", 16))
my_label4.place(x=700,y=43)


my_label5 = Label(window,
                 text="Перемещение печатной головы",
                 bg="white",
                 fg="black",
                 font=("Helvetica", 16))
my_label5.place(x=100,y=430)

# labels for stepper
# UP
my_label6 = Label(window,
                 text="10 мм",
                 bg="white",
                 fg="black",
                 font=("Helvetica", 16))
my_label6.place(x=200,y=500)

#Buttons
MyButton1 = Button(window, text= "Вверх", pady =5, command= move_up_10)
MyButton1.place(x=150,y=500)
# down

#Buttons
MyButton2 = Button(window, text= "Вниз", pady =5, command= move_down_10)
MyButton2.place(x=150,y=600)

my_label7 = Label(window,
                 text="10 мм",
                 bg="white",
                 fg="black",
                 font=("Helvetica", 16))
my_label7.place(x=200,y=600)

# UP
my_label8 = Label(window,
                 text="1 мм",
                 bg="white",
                 fg="black",
                 font=("Helvetica", 16))
my_label8.place(x=500,y=500)

#Buttons
MyButton3 = Button(window, text= "Вверх", pady =5, command= move_up_1)
MyButton3.place(x=450,y=500)
# down
my_label9 = Label(window,
                 text="1 мм",
                 bg="white",
                 fg="black",
                 font=("Helvetica", 16))
my_label9.place(x=500,y=600)

MyButton4 = Button(window, text= "Вниз", pady =5, command= move_down_1)
MyButton4.place(x=450,y=600)

# UP

my_label10 = Label(window,
                 text="0.1 мм",
                 bg="white",
                 fg="black",
                 font=("Helvetica", 16))
my_label10.place(x=800,y=500)


#Buttons
MyButton5 = Button(window, text= "Вверх", pady =5, command= move_up_01)
MyButton5.place(x=750,y=500)
# down
my_label11 = Label(window,
                 text="0.1 мм",
                 bg="white",
                 fg="black",
                 font=("Helvetica", 16))
my_label11.place(x=800,y=600)

#Buttons
MyButton5 = Button(window, text= "Вниз", pady =5, command= move_down_01)
MyButton5.place(x=750,y=600)
# Voltage
my_label12 = Label(window,
                 text="E, mV",
                 bg="white",
                 fg="black",
                 font=("Helvetica", 16))
my_label12.place(x=100,y=200)

voltage_in = Entry (window, width=10, bg="white", fg="black", font=("Helvetica", 16))
voltage_in.place(x=400,y=200)

VoltageButton = Button(window, text= "OK", pady =5, command= voltage)
VoltageButton.place(x=550,y=200)




# input fields
my_label13 = Label(window,
                 text="Открыть 3D модель для печати",
                 bg="white",
                 fg="black",
                 font=("Helvetica", 16))
my_label13.place(x=800,y=200)
# input fields
my_label14 = Label(window,
                 text="Высота слоя, мм",
                 bg="white",
                 fg="black",
                 font=("Helvetica", 16))
my_label14.place(x=100,y=250)

height_in = Entry (window, width=10, bg="white", fg="black", font=("Helvetica", 16))
height_in.place(x=400,y=250)


heightButton = Button(window, text= "OK", pady =5, command= height)
heightButton.place(x=550,y=250)

my_label14 = Label(window,
                 text="Время печати слоя, сек",
                 bg="white",
                 fg="black",
                 font=("Helvetica", 16))
my_label14.place(x=100,y=300)




time_in = Entry (window, width=10, bg="white", fg="black", font=("Helvetica", 16))
time_in.place(x=400,y=300)

timeButton = Button(window, text= "OK", pady =5, command= time)
timeButton.place(x=550,y=300)


client = connect_mqtt()
subscribe(client)
client.loop_start()


window.mainloop()
client.loop_stop()





