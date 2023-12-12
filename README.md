

0. Сделать так чтобы из файла test_slicer.py можно было полученный массив передать в виде 256 символьной стороки 
0.1. передать на ардуино (dac_and_sh_reg-mqtt.ino файл) 
1. Переписать dac_and_sh_reg-mqtt.ino файл с со сдвиговыми регистрами переписать под mqtt (приемник)
2. Основы сдвигового регистра shift.ino (работает)
3. из shift.ino дополнить mqtt (как подписчик) и объединить с файлом dac_and_sh_reg-mqtt.ino
4. Сделать так чтобы в можно было выбрать файл 3d модели из графического интрефейса (tk.py) 


stepper-mqtt.ino - файл для шагового двигателя (готов)
sensor-mqtt.ino - файл для передачи данных с сенсоров по mqtt


tk.py - графический интерфейс и mqtt 
printer_utilities.py файл слайсера 
 test_slicer.py как работать со слайсером 

топики

(чтение)
  "mosquitto/printer/ph" - ph
 "mosquitto/printer/cond" - проводимость
 "mosquitto/printer/curr" - ток
 "mosquitto/printer/volt" - напряжение

Запись

mosquitto/printer/dac - ЦАП (передаем в мВ)
выход с регулятора по ф-ле E( reg mV) =  1.9879*E(dac) +1266.4
 

"mosquitto/printer/stepper_up" - перемещение шагового двигателя вверх
"mosquitto/printer/stepper_down" - перемещение шагового двигателя вверх
нужно передать в топики 01, 1, 10 - перемещение на 0.1 мм, 1 мм, 10 мм 


cone.obj; cube.obj; sphere.obj - 3d модели




