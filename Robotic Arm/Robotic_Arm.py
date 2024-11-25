import serial
import tkinter
import customtkinter
import time
import threading
import numpy as np
import json 
import tkinter.filedialog  # Import filedialog module
 # Added for file handling

# Initialize basic setup
customtkinter.set_appearance_mode("System")
customtkinter.set_default_color_theme("blue")

app = customtkinter.CTk()
app.geometry("800x600")
app.title("Maker Tutor-Python Arduino Servo Controller")

# Global variables
servo_configs = [
    {"name": "CLAW", "min": 850, "max": 1650, "default": 1400},
    {"name": "NECK", "min": 500, "max": 2500, "default": 1700},
    {"name": "BODY", "min": 500, "max": 2500, "default": 2050},
    {"name": "BASE", "min": 500, "max": 2500, "default": 2050}
]

servo_values = [config["default"] for config in servo_configs]
recorded_positions = []
is_playing = False
is_paused = False
current_replay_index = 0
replay_thread = None
transition_steps = 50
transition_time = 500
replay_speed = 1000

# Initialize serial connection
arduino = serial.Serial(port='COM9', baudrate=115200, timeout=1)

# Define all functions first
def write_data():
    data_string = ','.join(map(str, servo_values)) + ';'
    arduino.write(bytes(data_string, "utf-8"))
    return 1

def interpolate_positions(start_pos, end_pos, steps):
    return [
        [
            int(start + (end - start) * i / (steps - 1))
            for start, end in zip(start_pos, end_pos)
        ]
        for i in range(steps)
    ]

def update_servos_and_sliders(position):
    for i, value in enumerate(position):
        servo_values[i] = value
        app.after(0, lambda i=i, v=value: sliders[i].set(v))
    write_data()

def slider_event(index):
    def handler(value):
        global servo_values
        servo_values[index] = int(value)
        labels[index].configure(text=f"{servo_configs[index]['name']} : {int(value)}")
        write_data()
        print(f"{servo_configs[index]['name']}: {int(value)} ")
    return handler

def speed_slider_event(value):
    global replay_speed
    replay_speed = max(transition_time, int(value))
    speed_label.configure(text=f"Replay Speed: {replay_speed}ms")

def transition_time_slider_event(value):
    global transition_time
    transition_time = int(value)
    transition_label.configure(text=f"Transition Time: {transition_time}ms")

def save_position():
    current_position = servo_values.copy()
    recorded_positions.append(current_position)
    print("Position saved:", current_position)
    update_status("Position saved!")

def reset_recording():
    global recorded_positions, is_playing, is_paused, current_replay_index
    recorded_positions = []
    is_playing = False
    is_paused = False
    current_replay_index = 0
    start_button.configure(text="START")
    update_status("Recording reset!")

def update_status(message):
    status_label.configure(text=f"Status: {message}")

def replay_positions():
    global current_replay_index, is_playing, is_paused
    
    while is_playing and not is_paused and recorded_positions:
        if current_replay_index >= len(recorded_positions):
            current_replay_index = 0
        
        current_pos = servo_values.copy()
        target_pos = recorded_positions[current_replay_index]
        
        steps = interpolate_positions(current_pos, target_pos, transition_steps)
        step_delay = transition_time / transition_steps / 1000
        
        for step in steps:
            if not is_playing or is_paused:
                break
            update_servos_and_sliders(step)
            time.sleep(step_delay)
        
        current_replay_index += 1
        if replay_speed > transition_time:
            time.sleep((replay_speed - transition_time) / 1000)

def toggle_replay():
    global is_playing, is_paused, replay_thread
    
    if not recorded_positions:
        update_status("No positions recorded!")
        return
        
    if not is_playing:
        is_playing = True
        is_paused = False
        start_button.configure(text="PAUSE")
        update_status("Replaying positions...")
        replay_thread = threading.Thread(target=replay_positions, daemon=True)
        replay_thread.start()
    else:
        if is_paused:
            is_paused = False
            start_button.configure(text="PAUSE")
            update_status("Replay resumed...")
            replay_thread = threading.Thread(target=replay_positions, daemon=True)
            replay_thread.start()
        else:
            is_paused = True
            start_button.configure(text="RESUME")
            update_status("Replay paused...")

def export_positions():
    """Export recorded positions to a user-selected JSON file."""
    if not recorded_positions:
        update_status("No positions to export!")
        return

    file_path = tkinter.filedialog.asksaveasfilename(
        defaultextension=".json",
        filetypes=[("JSON files", "*.json"), ("All files", "*.*")]
    )
    if file_path:  # Proceed only if the user selects a file
        try:
            with open(file_path, "w") as file:
                json.dump(recorded_positions, file)
            update_status(f"Positions exported to {file_path}!")
        except Exception as e:
            update_status(f"Error exporting file: {e}")

def import_positions():
    """Import recorded positions from a user-selected JSON file."""
    global recorded_positions
    file_path = tkinter.filedialog.askopenfilename(
        filetypes=[("JSON files", "*.json"), ("All files", "*.*")]
    )
    if file_path:  # Proceed only if the user selects a file
        try:
            with open(file_path, "r") as file:
                recorded_positions = json.load(file)
            update_status(f"Positions imported from {file_path}!")
        except FileNotFoundError:
            update_status("File not found!")
        except json.JSONDecodeError:
            update_status("Invalid file format!")
        except Exception as e:
            update_status(f"Error importing file: {e}")

# Initialize lists for UI elements
labels = []
sliders = []

# Create UI elements
# Title frame
title_frame = customtkinter.CTkFrame(master=app)
title_frame.pack(pady=20, padx=20, fill="x")

title_label = customtkinter.CTkLabel(
    master=title_frame, 
    text="Robotic Arm Project",
    text_color="#1F6AA5",
    font=customtkinter.CTkFont(size=32, weight="bold")
)
title_label.pack(pady=10)

# Servo control frame
servo_frame = customtkinter.CTkFrame(master=app)
servo_frame.pack(pady=20, padx=20, fill="x")

# Create servos
for i, config in enumerate(servo_configs):
    frame = customtkinter.CTkFrame(master=servo_frame)
    frame.pack(pady=10, padx=10, fill="x")
    
    label = customtkinter.CTkLabel(master=frame, text=f"{config['name']} : {config['default']}")
    label.pack(side="left", padx=10)
    labels.append(label)

    slider = customtkinter.CTkSlider(
        master=frame, 
        from_=config['min'], 
        to=config['max'], 
        command=slider_event(i),
        width=400
    )
    slider.set(config['default'])
    slider.pack(side="right", padx=10)
    sliders.append(slider)

# Transition time control frame
transition_frame = customtkinter.CTkFrame(master=app)
transition_frame.pack(pady=10, padx=20, fill="x")

transition_label = customtkinter.CTkLabel(master=transition_frame, text="Transition Time: 500ms")
transition_label.pack(side="left", padx=10)

transition_slider = customtkinter.CTkSlider(
    master=transition_frame,
    from_=100,
    to=1000,
    command=transition_time_slider_event,
    width=400
)
transition_slider.set(500)
transition_slider.pack(side="right", padx=10)

# Speed control frame
speed_frame = customtkinter.CTkFrame(master=app)
speed_frame.pack(pady=10, padx=20, fill="x")

speed_label = customtkinter.CTkLabel(master=speed_frame, text="Replay Speed: 1000ms")
speed_label.pack(side="left", padx=10)

speed_slider = customtkinter.CTkSlider(
    master=speed_frame,
    from_=500,
    to=3000,
    command=speed_slider_event,
    width=400
)
speed_slider.set(1000)
speed_slider.pack(side="right", padx=10)

# Buttons frame
button_frame = customtkinter.CTkFrame(master=app)
button_frame.pack(pady=20, padx=20, fill="x")

button_frame_left = customtkinter.CTkFrame(master=button_frame)
button_frame_left.pack(side="left", expand=True)

button_frame_center = customtkinter.CTkFrame(master=button_frame)
button_frame_center.pack(side="left", expand=True)

button_frame_right = customtkinter.CTkFrame(master=button_frame)
button_frame_right.pack(side="left", expand=True)

save_button = customtkinter.CTkButton(
    master=button_frame_left, 
    text="SAVE", 
    command=save_position,
    width=120,
    height=40,
    font=customtkinter.CTkFont(size=14, weight="bold")
)
save_button.pack(padx=10, pady=5)

start_button = customtkinter.CTkButton(
    master=button_frame_center, 
    text="START", 
    command=toggle_replay,
    width=120,
    height=40,
    font=customtkinter.CTkFont(size=14, weight="bold")
)
start_button.pack(padx=10, pady=5)

reset_button = customtkinter.CTkButton(
    master=button_frame_right, 
    text="RESET", 
    command=reset_recording,
    width=120,
    height=40,
    font=customtkinter.CTkFont(size=14, weight="bold")
)
reset_button.pack(padx=10, pady=5)

# Import/Export Buttons
# Import/Export Buttons Frame
file_frame = customtkinter.CTkFrame(master=app)
file_frame.pack(pady=10, padx=20, fill="x")

# Center-align the buttons using grid layout
file_frame.columnconfigure(0, weight=1)  # Center column
file_frame.columnconfigure(1, weight=1)  # Left padding for Export

import_button = customtkinter.CTkButton(
    master=file_frame,
    text="IMPORT",
    command=import_positions,
    fg_color="#1F6AA5",
    width=120,
    height=40,
    font=customtkinter.CTkFont(size=14, weight="bold")
)
import_button.grid(row=0, column=0, padx=10, pady=5)

export_button = customtkinter.CTkButton(
    master=file_frame,
    text="EXPORT",
    command=export_positions,
    fg_color="#4CAF50",
    width=120,
    height=40,
    font=customtkinter.CTkFont(size=14, weight="bold")
)
export_button.grid(row=0, column=1, padx=10, pady=5)


# Status frame
status_frame = customtkinter.CTkFrame(master=app)
status_frame.pack(pady=10, padx=20, fill="x")

status_label = customtkinter.CTkLabel(
    master=status_frame, 
    text="Status: Ready",
    font=customtkinter.CTkFont(size=14)
)
status_label.pack(pady=10)

# Info label
info_label = customtkinter.CTkLabel(
    master=app,
    text="Use sliders to control servos. Save positions and use Start/Pause to replay them.",
    text_color="gray",
    font=customtkinter.CTkFont(size=12)
)
info_label.pack(pady=5)

app.mainloop()
