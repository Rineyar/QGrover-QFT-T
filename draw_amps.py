import matplotlib.pyplot as plt
import numpy as np
from matplotlib.widgets import RadioButtons, CheckButtons, Slider

with open("saved_amps.bin", "rb") as amps_file:
    count = int.from_bytes(amps_file.read(4),"little",signed=True)

    state = np.fromfile(amps_file,dtype=np.dtype("<c16"),count=count)

x = np.arange(count)

fig = plt.figure(figsize=(12,6))

ax = fig.add_axes([0.30,0.12,0.65,0.80])

rax = fig.add_axes([0.03,0.35,0.22,0.35])

plot_modes = ["Probabilities","Magnitudes","Real / Imag","Phases","Complex plane"]

button = RadioButtons(rax, plot_modes)

def redraw(mode):
    ax.clear()

    if mode == "Probabilities":
        probs = np.pow(np.abs(state),2)

        ax.plot(x,probs)
        ax.set_xlabel("State index")
        ax.set_ylabel("|amplitude|²")
        ax.set_title("Probabilities")

    elif mode == "Magnitudes":
        ax.plot(x,np.abs(state))
        ax.set_xlabel("State index")
        ax.set_ylabel("|amplitude|")
        ax.set_title("Amplitude magnitudes")

    elif mode == "Real / Imag":
        ax.plot(x,state.real,label="Re")
        ax.plot(x,state.imag,label="Im")
        ax.set_xlabel("State index")
        ax.set_ylabel("Value")
        ax.set_title("Real and imaginary parts")
        ax.legend()

    elif mode == "Phases":
        eps = 1e-10

        mask = np.abs(state) > eps

        ax.scatter(x[mask],np.angle(state[mask]))
        ax.set_xlabel("State index")
        ax.set_ylabel("Phase, radians")
        ax.set_title("Phases")

    elif mode == "Complex plane":
        ax.scatter(state.real,state.imag)
        ax.axhline(0,linewidth=0.8)
        ax.axvline(0,linewidth=0.8)
        ax.set_xlabel("Re")
        ax.set_ylabel("Im")
        ax.set_title("Complex plane")
        ax.axis("equal")

    ax.grid()
    fig.canvas.draw_idle()

button.on_clicked(redraw)

redraw("Probabilities")

plt.show()