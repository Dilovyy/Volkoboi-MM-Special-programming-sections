# Лабораторна робота №1
## Наука про дані: підготовчий етап
Дедлайн: 30 вересня
Посилання, з якими треба ознайомитись:
Python: 
download: https://www.python.org/downloads/
download guide : https://wiki.python.org/moin/BeginnersGuide/Download
add python to PATH : https://realpython.com/add-python-to-path/
pip : 
https://pypi.org/project/pip/
https://pip.pypa.io/en/stable/getting-started/
https://pip.pypa.io/en/stable/user_guide/
alt package manager - anaconda : https://www.anaconda.com/download/success
venv : 
https://docs.python.org/3/library/venv.html
very easy guide: https://realpython.com/python-virtual-environments-a-primer/
Jupyter Notebook: 
try online: https://jupyter.org/try
docs: https://docs.jupyter.org/en/latest/
tutorial: https://www.dataquest.io/blog/jupyter-notebook-tutorial/
pandas : 
https://pandas.pydata.org/docs/index.html
pandas 10 min intro: https://pandas.pydata.org/docs/user_guide/10min.html
interactive intro: https://www.w3schools.com/python/pandas/default.asp
urllib:
https://docs.python.org/uk/3.13/howto/urllib2.html

# Хід виконання роботи
### Частина 1
Створити віртуальне середовище (venv) в якому будуть встановлені всі необхідні бібліотеки та налаштування для даної лабораторної роботи;
Для кожної з адміністративних одиниць України завантажити (urllib) тестові структуровані файли, що містять значення VHI-індексу. При зберіганні файлу, до його імені потрібно додати дату та час завантаження. Передбачити повторні запуски скрипту, реалізувати механізм запобігання повторного довантаження та колізії даних;
Приклад посилання для області №8:
https://www.star.nesdis.noaa.gov/smcd/emb/vci/VH/get_TS_admin.php?country=UKR&provinceID=8&year1=1981&year2=2024&type=Mean
Якщо вказати provinceID=0, сформується вибірка в середньому по Україні. Її завантажувати не потрібно;
Зчитати завантажені текстові файли у pandas dataframe. Здійснити data cleaning: прибрати зайві стовпці, заповнити пропуски, видалити зайвий текст тощо. Додати стовпчики з назвою та індексом області (див. How to Handle Missing Data in Python?)
Реалізувати процедуру зміни індексів: в завантажених з NOAA даних області індексуються за англійською абеткою (Province 1 - Cherkasy), потрібно замінити індекси так, щоб області індексувались за українською абеткою (1 область - Вінницька). 
Реалізувати процедури для формування вибірок (https://www.geeksforgeeks.org/pandas/ways-to-filter-pandas-dataframe-by-column-values/)  наступного виду:
Ряд VHI для області за вказаний рік;
Ряд VHI за вказаний діапазон років для вказаних областей;
Пошук екстремумів (min та max) для вказаних областей та років, середнього, медіани;
Інші вибірки на вимогу викладача.
Всі процедури необхідно реалізувати окремими функціями та викликати їх в окремих комірках Jupyter Notebook. Додати текст завдань перед кожним пунктом та читабельний вивід функцій. 
Для розуміння фізичної природи даних, можна ознайомитись з інтерактивною картою: https://www.star.nesdis.noaa.gov/smcd/emb/vci/VH/vh_browse.php .

# Частина 2
Необхідно виконати завдання, використовуючи  pandas dataframe, проаналізувати часові витрати на виконання процедур (профілювання часу виконання здійснити за допомогою модуля timeit).

Звантажити та відкрити (вручну або через запропонований скрипт на сайті) наступний датасет: Individual Household Electric Power Consumption Dataset 
Здійснити data cleaning (див. How to Handle Missing Data in Python?)
Окремими функціями сформувати вибірки:
Обрати всі записи, у яких загальна активна споживана потужність перевищує 5 кВт.
Обрати всі записи, у яких сила струму лежить в межах 19-20 А, для них виявити ті, у яких пральна машина та холодильних споживають більше, ніж бойлер та кондиціонер.
Обрати випадковим чином 500000 записів (без повторів елементів вибірки), для них обчислити середні величини усіх 3-х груп споживання електричної енергії
Обрати ті записи, які після 18-00 споживають понад 6 кВт за хвилину в середньому, серед відібраних визначити ті, у яких основне споживання електроенергії у вказаний проміжок часу припадає на пральну машину, сушарку, холодильник та освітлення (група 2 є найбільшою), а потім обрати кожен третій результат із першої половини та кожен четвертий результат із другої половини.
Пронормувати та стандартизувати вибраний датасет
Підрахувати коефіцієнт Пірсона та Спірмена для двох integer/real атрибутів.
Провести One Hot Encoding категоріального атрибута.

Обидві частини виконати у вигляді окремих .ipynb файлів, і завантажити на свій репозиторій GitHub.


