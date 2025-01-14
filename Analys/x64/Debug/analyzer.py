import subprocess
import tkinter as tk
from tkinter import filedialog, messagebox

# Функция для выбора файла и запуска анализатора
def analyze_file():
    file_path = filedialog.askopenfilename(filetypes=[("Text files", "*.txt")])
    if not file_path:
        return  # Прерываем, если файл не выбран

    try:
        # Считываем содержимое файла и выводим его в отдельное текстовое поле
        with open(file_path, 'r', encoding='utf-8') as file:
            file_content = file.read()
        
        file_content_field.delete("1.0", tk.END)  # Очищаем текстовое поле для содержимого файла
        file_content_field.insert(tk.END, "Содержимое файла:\n")
        file_content_field.insert(tk.END, file_content + "\n")
        
        # Запуск программы на C++ с передачей пути к файлу
        result = subprocess.run(["Analys.exe", file_path], capture_output=True, text=True)
        
        # Проверяем код возврата
        if result.returncode != 0:
            messagebox.showerror("Ошибка", f"Программа завершилась с ошибкой:\n{result.stderr}")
            return  # Прекращаем выполнение, если была ошибка
        
        # Если ошибок нет, выводим результат
        output_field.delete("1.0", tk.END)  # Очищаем текстовое поле для результата
        output_field.insert(tk.END, "Результат анализа:\n")
        output_field.insert(tk.END, result.stdout)
    except FileNotFoundError:
        messagebox.showerror("Ошибка", "Не удалось найти исполняемый файл main.exe")
    except Exception as e:
        messagebox.showerror("Ошибка", f"Произошла ошибка: {str(e)}")

# Интерфейс с помощью Tkinter
window = tk.Tk()
window.title("Лексический анализатор Хохлов А.С. ИС_42.2")

input_label = tk.Label(window, text="Выберите файл для анализа:")
input_label.pack()

analyze_button = tk.Button(window, text="Анализировать файл", command=analyze_file)
analyze_button.pack()

# Текстовое поле для отображения содержимого файла
file_content_label = tk.Label(window, text="Содержимое файла:")
file_content_label.pack()

file_content_field = tk.Text(window, height=10, width=70)
file_content_field.pack()

# Текстовое поле для отображения результата анализа
output_label = tk.Label(window, text="Результат анализа:")
output_label.pack()

output_field = tk.Text(window, height=15, width=70)
output_field.pack()

window.mainloop()
