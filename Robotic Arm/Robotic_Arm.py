import serial
import tkinter
import customtkinter

customtkinter.set_appearance_mode("System")
customtkinter.set_default_color_theme("blue")

app = customtkinter.CTk()
app.geometry("600x480")
app.title("Maker Tutor-Python Arduino Servo Controller")

# Global variables for servo values and configurations
servo_configs = [
    {"name": "CLAW", "min": 850, "max": 1650, "default": 1400},
    {"name": "NECK", "min": 500, "max": 2500, "default": 1700},
    {"name": "BODY", "min": 500, "max": 2500, "default": 2050},
    {"name": "BASE", "min": 500, "max": 2500, "default": 2050}
]

servo_values = [config["default"] for config in servo_configs]

arduino = serial.Serial(port='COM9', baudrate=115200, timeout=1)

def write_data():
    # Convert servo values to a comma-separated string and add delimiter
    data_string = ','.join(map(str, servo_values)) + ';'
    arduino.write(bytes(data_string, "utf-8"))
    return 1

def slider_event(index):
    def handler(value):
        global servo_values
        servo_values[index] = int(value)
        labels[index].configure(text=f"{servo_configs[index]['name']} control: {int(value)}")
        write_data()
        print(f"{servo_configs[index]['name']}: {int(value)} control")
    return handler

# Create labels and sliders for 4 servos
labels = []
sliders = []

for i, config in enumerate(servo_configs):
    label = customtkinter.CTkLabel(master=app, text=f"{config['name']} control: {config['default']}")
    label.place(relx=0.1, rely=0.2 + i*0.2, anchor=tkinter.W)
    labels.append(label)

    slider = customtkinter.CTkSlider(
        master=app, 
        from_=config['min'], 
        to=config['max'], 
        command=slider_event(i)
    )
    slider.set(config['default'])
    slider.place(relx=0.5, rely=0.3 + i*0.2, anchor=tkinter.CENTER)
    sliders.append(slider)

app.mainloop()