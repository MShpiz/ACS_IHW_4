# ACS_IHW_4
Вариант: 21
Выполнила: Панфилова Мария Денисовна
Группа: БПИ226
Желаемая оценка: 8
## Условие зарианта 21
<b>Задача о нелюдимых садовниках.</b> \
Имеется пустой участок земли (двумерный массив размером M × N) и план сада, разбитого на отдельные квадраты. От 10 до 30 процентов (задается случайно) площади сада заняты прудами или камнями. То есть недоступны для ухаживания. Эти квадраты располагаются на плане произвольным (случайным) образом. Ухаживание за садом выполняют два садовника, которые не хотят встречаться друг другом (то есть, одновременно появляться в одном и том же квадрате). Первый садовник начинает работу с верхнего левого угла сада и перемещается слева направо, сделав ряд, он спускается вниз и идет в обратном направлении, пропуская обработанные участки. Второй садовник начинает работу с нижнего правого угла сада и перемещается снизу вверх, сделав ряд, он перемещается влево и также идет в обратную сторону. Если садовник видит, что участок сада уже обработан другим садовником или является необрабатываемым, он идет дальше. Если по пути какой-то участок занят другим садовником, то садовник ожидает когда участок освободится, чтобы пройти дальше на доступный ему необработанный участок. Садовники должны работать одновременно со скоростями, определяемыми как параметры задачи. Прохождение через любой квадрат занимает некоторое время, которое задается константой, меньшей чем времена обработки и принимается за единицу времени. Создать многопоточное приложение, моделирующее работу садовников. Каждый садовник — это отдельный поток.
## Сценарий поведения садовников
Так как сад разбит на квадраты будем считать что верхний левый угол имеет координаты (0, 0), а нижний правый - (M-1, N-1). \
Первый садовник начинает работу в квадрате с координатами (0, 0), а второй - (M-1, N-1). Первый садовник перемещается слева направо, сделав ряд, он спускается вниз и идет в обратном направлении, пропуская обработанные участки. Второй садовник перемещается снизу вверх, сделав ряд, он перемещается влево и также идет в обратную сторону. Перемещение для каждого садовника происходит так, сначала садовник обрабатывает участок на котором он стоит (если он не обработан) со своей скоростью, а за тем заглядывает в следующий квадрат, если там есть другой садовник он ждёт пока тот не уйдёт, иначе переходит в следующий квадрат за еденицу времени. Если получается так, что садовники находятся в следующих квадратах друг друга, они заканчивают работу.
## Алгоритм решения
Собственно программное решение  моделирует происходящее в сценарии, где каждый садовник является отдельным потоком выполняющим функцию обработки сада. Сад это двумерный массив размера M × N заполненный числами где -1 - камень, 0 - необработанный квадрат, 1 - обработанный квадрат. \
Функция обработки сада состоит из 3 частей - выполнения работ в текущем квадрате, проверка следующего квадрата на наличие другого садовника или существование квадрата, пререход на следующий квадрат.  \
На первом этапе садовник заполняет ячейку в массиве соответствующую его позиции в саду с задержкой на время обработки. \
На втором этапе проверяется следующий квадрат на пути садовника, если квадрата нет - садовник прошёл весь сад, то выполнение функции завершается. Если в следующей клетке другой садовник, проверяется является ли та клетка где стоит садовник следующей клеткой для другого садовника. Если это так выполнение функции завершается, иначе садовник ждёт одно время работы другого садовника и проверяет следующую клетку опять. Если ничто не препятствует перемещению на следующую клетку садовник переходт на неё и ждёт одно время передвижения.

## Входные данные
Все входные данные - это аргументы для запуска программы из командной строки.
1) имя файла для вывода результирующих данных
2) аргумент показывающий будут скорости садовников введены из файла (1) или из командной строки (0)
      3) Если предыдущий аргумент равен 0 то 3 аргумент - файл с исходными данными
      4) Если предыдущий аргумент равен 1 то 3 и 4 аргументы - скорости первого и второого садовников соответственно

## Решение на 8
• В программу, наряду с выводом в консоль, добавлен вывод результатов в файл. Имя файла для вывода данных задается в командной строке как один из ее параметров.
• Результаты работы программы должны выводиться на экран и записываться в файл.
• Наряду с вводом исходных данных через командную строку добавлен альтернативный вариант их ввода из файла, который по сути играет роль конфигурационного файла. Имя этого файла задается в командной строке вместо параметров, которые в этом случае не вводятся. Управление вводом в командной строке осуществляется с использованием соответствующих ключей.
• [Тут](https://github.com/MShpiz/ACS_IHW_4/tree/main/examples) приведено не менее трех вариантов входных и выходных файлов с различными исходным данными и результатами выполнения программы.
• Ввод данных из командной строки расширен с учетом введенных изменений.

## Решение на 6-7
• В отчете подробно описан обобщенный алгоритм, используемый при реализации программы исходного словесного сценария. В котором показано, как на программу отображается каждый из субъектов предметной области.
• В программу добавлена генерация случайных данных в допустимых диапазонах. (она так-то осталась из вариации задания на 4-5, но ладно)
• Реализован ввод исходных данных из командной строки при запуске программы вместо ввода параметров с консоли во время выполнения программы в соответствии с параметрами входных данных.

## Решение на 4-5 
* Модель параллельных вычислений, используемая при разработке многопоточной программы - взаимодействующие равные.
* Описаны входные данные программы, включающие вариативные диапазоны, возможные при многократных запусках. 
* Реализовано [консольное приложение](https://github.com/MShpiz/ACS_IHW_4/blob/main/code.cpp), решающее поставленную задачу с использованием одного варианта изученных синхропримитивов (мьютекс). 
* ~~Ввод данных в приложение реализован с консоли во время выполнения программы (без использования аргументов командной строки).~~ Отменено требованиями на 6-7.
* Для используемых генераторов случайных чисел описаны их диапазоны и то, как интерпретируются данные этих генераторов:
* Диапазоны случайных велечин: 
    * N, M - [1, 100] - длина и ширина сада 
    * [0, NM] - число используемое для определения наличия камня в текущей клетке, используется в заполнении поля.
* Вывод программы должен быть информативным, отражая все ключевые протекающие в ней события в терминах предметной области. Наблюдатель на основе вывода программы должен понимать, что в ней происходит в каждый момент времени ее работы. 
* В программе присутствуют комментарии, поясняющие выполняемые действия и описание используемых объектов и переменных. 
* Результаты работы программы представлены в отчете. 
