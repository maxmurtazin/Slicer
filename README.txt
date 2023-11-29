1.stepper.ino нужно переписать под управлением mqtt

2.добавить файл с сенсорами, которые передают данные по mqtt (ph, cond, voltage, current) (написан)

3.файл с ЦАП и регистрами переписать под mqtt

printer_utilities.py файл слайсера 
test_slicer как с ним работать 

pic_gui-что хотим получить

mqtt_pic_gui -общая схема

tk.py - графический интерфейс


  "mosquitto/printer/ph" - ph
 "mosquitto/printer/cond"); - проводимость
 "mosquitto/printer/curr"); - ток
 "mosquitto/printer/volt"); -напряжение


