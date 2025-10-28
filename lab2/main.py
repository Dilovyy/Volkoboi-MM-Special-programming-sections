import numpy as np
import matplotlib.pyplot as plt
from matplotlib.widgets import Slider, Button, CheckButtons
from scipy import signal

init_amplitude = 1.0
init_frequency = 0.3
init_phase = 0.0
init_noise_mean = 0.0
init_noise_covariance = 0.1
init_cutoff_freq = 5.0
init_filter_order = 5

current_noise = None
current_t = None

# Створення часового масиву
t = np.linspace(0, 10, 1000)
fs = len(t) / (t[-1] - t[0]) 

def harmonic_with_noise(t, amplitude, frequency, phase, noise_mean, noise_covariance, show_noise=True):
    global current_noise, current_t
    
    clean_signal = amplitude * np.sin(2 * np.pi * frequency * t + phase)
    
    # якщо параметри змінились або шуму ще немає
    if current_noise is None or len(current_noise) != len(t):
        current_noise = np.random.normal(noise_mean, np.sqrt(noise_covariance), len(t))
        current_t = t.copy()
    
    # якщо чекбокс - тру, додаємо шум
    if show_noise:
        noisy_signal = clean_signal + current_noise
    else:
        noisy_signal = clean_signal
    
    return clean_signal, noisy_signal

def regenerate_noise(noise_mean, noise_covariance, t):
    global current_noise
    current_noise = np.random.normal(noise_mean, np.sqrt(noise_covariance), len(t))

def apply_filter(signal_data, cutoff_freq, fs, filter_order):
    
    nyquist = 0.5 * fs
    normal_cutoff = cutoff_freq / nyquist
    
    # фільтр Баттерворта
    #b, a = signal.butter(filter_order, normal_cutoff, btype='low', analog=False)


    #фільтр Чебишева
    b, a = signal.cheby2(filter_order, 40, normal_cutoff, btype='low', analog=False)
    
    filtered_signal = signal.filtfilt(b, a, signal_data)
    
    return filtered_signal




clean, noisy = harmonic_with_noise(t, init_amplitude, init_frequency, init_phase, 
                                    init_noise_mean, init_noise_covariance, True)
filtered = apply_filter(noisy, init_cutoff_freq, fs, init_filter_order)

fig, ax = plt.subplots(figsize=(12, 6))
fig.suptitle('Гармоніка з шумом і фільтрацією', fontsize=14, fontweight='bold')

line_clean, = ax.plot(t, clean, 'b-', linewidth=2, label='Чиста гармоніка', alpha=0.7)
line_noisy, = ax.plot(t, noisy, 'orange', linewidth=1, label='Зашумлена гармоніка', alpha=0.6)
line_filtered, = ax.plot(t, filtered, 'r-', linewidth=1.5, label='Відфільтрована гармоніка', alpha=0.8)

ax.set_xlabel('Час (t)')
ax.set_ylabel('Амплітуда')
ax.grid(True, alpha=0.3)
ax.legend(loc='upper right')
ax.set_ylim(-3, 3)

plt.subplots_adjust(left=0.1, bottom=0.35, right=0.95, top=0.88)


# Створення слайдерів
slider_color = 'lightgoldenrodyellow'

ax_amp = plt.axes([0.15, 0.26, 0.3, 0.02], facecolor=slider_color)
ax_freq = plt.axes([0.15, 0.23, 0.3, 0.02], facecolor=slider_color)
ax_phase = plt.axes([0.15, 0.20, 0.3, 0.02], facecolor=slider_color)
ax_noise_mean = plt.axes([0.15, 0.16, 0.3, 0.02], facecolor=slider_color)
ax_noise_cov = plt.axes([0.15, 0.13, 0.3, 0.02], facecolor=slider_color)
ax_cutoff = plt.axes([0.15, 0.09, 0.3, 0.02], facecolor=slider_color)
ax_filter_order = plt.axes([0.15, 0.06, 0.3, 0.02], facecolor=slider_color)

slider_amp = Slider(ax_amp, 'Amplitude', 0.1, 2.0, valinit=init_amplitude, valstep=0.01)
slider_freq = Slider(ax_freq, 'Frequency', 0.1, 1.0, valinit=init_frequency, valstep=0.01)
slider_phase = Slider(ax_phase, 'Phase', -np.pi, np.pi, valinit=init_phase, valstep=0.01)
slider_noise_mean = Slider(ax_noise_mean, 'Noise Mean', -0.5, 0.5, valinit=init_noise_mean, valstep=0.01)
slider_noise_cov = Slider(ax_noise_cov, 'Noise Covariance', 0.0, 0.5, valinit=init_noise_covariance, valstep=0.01)
slider_cutoff = Slider(ax_cutoff, 'Cutoff Frequency', 0.5, 15.0, valinit=init_cutoff_freq, valstep=0.1)
slider_filter_order = Slider(ax_filter_order, 'Filter Order', 1, 10, valinit=init_filter_order, valstep=1)

# Чекбокси
ax_check = plt.axes([0.55, 0.20, 0.15, 0.08], facecolor='lightgray')
check = CheckButtons(ax_check, ['Show Noise', 'Show Filtered'], [True, True])

# Кнопка Reset
ax_reset = plt.axes([0.55, 0.10, 0.1, 0.04])
btn_reset = Button(ax_reset, 'Reset', color=slider_color, hovercolor='0.975')

# Змінні для відстеження попередніх значень
prev_noise_params = [init_noise_mean, init_noise_covariance]


def update(val):
    global prev_noise_params
    
    amp = slider_amp.val
    freq = slider_freq.val
    ph = slider_phase.val
    noise_mean = slider_noise_mean.val
    noise_cov = slider_noise_cov.val
    cutoff = slider_cutoff.val
    filter_ord = int(slider_filter_order.val)
    
    # Перевіряємо, чи змінились параметри шуму
    if [noise_mean, noise_cov] != prev_noise_params:
        regenerate_noise(noise_mean, noise_cov, t)
        prev_noise_params = [noise_mean, noise_cov]
    
    # Отримуємо стан чекбоксів
    show_noise = check.get_status()[0]
    show_filtered = check.get_status()[1]
    
    clean, noisy = harmonic_with_noise(t, amp, freq, ph, noise_mean, noise_cov, show_noise)
    filtered = apply_filter(noisy, cutoff, fs, filter_ord)
    line_clean.set_ydata(clean)
    line_noisy.set_ydata(noisy)
    line_filtered.set_ydata(clean)
    
    # Показати/сховати відфільтрований сигнал
    if show_filtered:
        line_filtered.set_ydata(filtered)
        line_filtered.set_visible(True)
    else:
        line_filtered.set_visible(False)
    
    # Автомасштабування
    #max_val = max(np.max(np.abs(clean)), np.max(np.abs(noisy)), np.max(np.abs(filtered)))
    #margin = max_val * 0.2
    #ax.set_ylim(-max_val - margin, max_val + margin)
    
    fig.canvas.draw_idle()

def reset(event):
    global current_noise, prev_noise_params
    
    slider_amp.reset()
    slider_freq.reset()
    slider_phase.reset()
    slider_noise_mean.reset()
    slider_noise_cov.reset()
    slider_cutoff.reset()
    slider_filter_order.reset()
    
    regenerate_noise(init_noise_mean, init_noise_covariance, t)
    prev_noise_params = [init_noise_mean, init_noise_covariance]
    
    update(None)


slider_amp.on_changed(update)
slider_freq.on_changed(update)
slider_phase.on_changed(update)
slider_noise_mean.on_changed(update)
slider_noise_cov.on_changed(update)
slider_cutoff.on_changed(update)
slider_filter_order.on_changed(update)
check.on_clicked(lambda label: update(None))
btn_reset.on_clicked(reset)

plt.show()