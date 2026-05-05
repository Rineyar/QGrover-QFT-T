import matplotlib.pyplot as plt
import numpy as np
from matplotlib.widgets import RadioButtons, Slider

with open("saved_amps.bin","rb") as amps_file:
    amps_count = int.from_bytes(amps_file.read(4),"little",signed=True)

    rest = amps_file.read()

old_file_size = 16 * amps_count

if len(rest) == old_file_size:
    states_count = 1

    states = np.frombuffer(rest,dtype=np.dtype("<c16"),count=amps_count).reshape(1,amps_count)
else:
    states_count = int.from_bytes(rest[:4],"little",signed=True)

    data = rest[4:]

    expected_size = 16 * amps_count * states_count

    if len(data) != expected_size:
        raise ValueError(f"Некорректный размер файла!\nОжидалось {expected_size} байт данных, "f"получено {len(data)}")

    states = np.frombuffer(data,dtype=np.dtype("<c16"),count=amps_count * states_count).reshape(states_count,amps_count)

x = np.arange(1,amps_count + 1)

fig = plt.figure(figsize=(12,6))

ax = fig.add_axes([0.30,0.17,0.65,0.75])

rax = fig.add_axes([0.03,0.38,0.22,0.35])

sax = fig.add_axes([0.30,0.06,0.55,0.04])

plot_modes = ["Probabilities","Magnitudes","Real / Imag","Phases","Complex plane"]

button = RadioButtons(rax,plot_modes)

slider = Slider(sax,"State",valmin=1,valmax=states_count,valinit=1,valstep=1)

current_mode = "Probabilities"

def setup_x_axis():
    ax.set_xticks(x)
    ax.set_xlim(0.5,amps_count + 0.5)

def redraw(mode=None):
    global current_mode

    if mode is not None:
        current_mode = mode

    state_id = int(slider.val) - 1

    state = states[state_id]

    ax.clear()

    if current_mode == "Probabilities":
        probs = np.pow(np.abs(state),2)

        ax.bar(x,probs)
        ax.set_xlabel("Amplitude number")
        ax.set_ylabel("|amplitude|²")
        ax.set_title(f"Probabilities, state {state_id + 1}/{states_count}")

        ax.set_ylim(0,1.05)
        ax.set_yticks(np.linspace(0,1,11))

        setup_x_axis()

    elif current_mode == "Magnitudes":
        magnitudes = np.abs(state)

        ax.bar(x,magnitudes)
        ax.set_xlabel("Amplitude number")
        ax.set_ylabel("|amplitude|")
        ax.set_title(f"Amplitude magnitudes, state {state_id + 1}/{states_count}")

        ax.set_ylim(0,max(1.05,np.max(magnitudes) * 1.1))

        setup_x_axis()

    elif current_mode == "Real / Imag":
        ax.bar(x - 0.15,state.real,width=0.3,label="Re")
        ax.bar(x + 0.15,state.imag,width=0.3,label="Im")

        ax.set_xlabel("Amplitude number")
        ax.set_ylabel("Value")
        ax.set_title(f"Real and imaginary parts, state {state_id + 1}/{states_count}")

        ax.legend()

        setup_x_axis()

    elif current_mode == "Phases":
        eps = 1e-10

        mask = np.abs(state) > eps

        ax.bar(x[mask],np.angle(state[mask]))

        ax.set_xlabel("Amplitude number")
        ax.set_ylabel("Phase, radians")
        ax.set_title(f"Phases, state {state_id + 1}/{states_count}")

        ax.set_ylim(-np.pi,np.pi)
        ax.set_yticks([-np.pi,-np.pi / 2,0,np.pi / 2,np.pi],["-π","-π/2","0","π/2","π"])

        setup_x_axis()

    elif current_mode == "Complex plane":
        ax.scatter(state.real,state.imag)

        for i in range(amps_count):
            ax.annotate(str(i + 1),(state.real[i],state.imag[i]))

        ax.axhline(0,linewidth=0.8)
        ax.axvline(0,linewidth=0.8)

        ax.set_xlabel("Re")
        ax.set_ylabel("Im")
        ax.set_title(f"Complex plane, state {state_id + 1}/{states_count}")

        ax.axis("equal")

    norm = np.sum(np.pow(np.abs(state),2))

    ax.text(0.01,0.98,f"norm = {norm:.12g}",transform=ax.transAxes,verticalalignment="top")

    ax.grid()
    fig.canvas.draw_idle()

button.on_clicked(redraw)

slider.on_changed(lambda value: redraw())

redraw("Probabilities")

plt.show()