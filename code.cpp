#include <iostream>
#include <random>
#include <string>
#include <pthread.h>
#include <unistd.h>
#include <fstream>

using namespace std;

pthread_mutex_t mutex ; //двоичный семафор

int field[100][100]; // сад
int n, m; // размеры сада
int from_file; // чтение будет из файла
int time_first, time_second; // время обработки клетки садовниками
int dir_first = 1, dir_second = -1; // направление движения садовника
pair<int, int> pos_first; // позиции садовников в саду
pair<int, int> pos_second;

bool gardenersSeeEachOther = false; // садовники столкнулись

ifstream fin;
ofstream fout;

// зполняет сад камнями
void FillField() {
    // инициализация генератора случайных чисел
    std::random_device random_device;
    mt19937 generator(random_device());
    uniform_int_distribution<> distribution(1, 10);
    // задаём случайные размеры сада тк в условии про них ничего не сказано
    n = distribution(generator);
    m = distribution(generator);
    // максимальное количество камней
    int rocks = m*n/3;
    uniform_int_distribution<> distribution2(0, m*n);
    
    // набрасываем в сад камней пока они будут занимать не меньше 10% от площади сада
    while(rocks - m*n/3 < m*n/10) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                if (field[i][j] == 0 && rocks > 0 && distribution2(generator) < m*n/4) {
                    rocks--;
                    field[i][j] = -1;
                } else {
                    field[i][j] = 0;
                }
            }
        }
    }
}

// возвращает следующую клетку в которую должен перейти садовник, p - номер садовника
pair<int, int> GetNext(int p) {
    pair<int, int> next;
    // логика перехода для первого садовника
    if (p == 1) {
        if (pos_first.second == 0 && dir_first == -1 ||  pos_first.second == m-1 && dir_first == 1) { // логика смены направления
            dir_first *= -1;
            next.second = pos_first.second;
            next.first = pos_first.first + 1;
            
        } else {
            next.second = pos_first.second + dir_first;
            next.first = pos_first.first;
        }
    } else { // логика перехода для второго садовника
        if (pos_second.first == 0 && dir_second == -1 ||  pos_second.first == n-1 && dir_second == 1) { // логика смены направления
            dir_second *= -1;
            next.first = pos_second.first;
            next.second = pos_second.second - 1;
        } else {
            next.first = pos_second.first + dir_second;
            next.second = pos_second.second;
        }
    }
    
    return next;
}

// передвигает первого садовника и заставляет кго работать
bool MoveFirst() {
    // если клету надо обработать обрабатываем
    if (field[pos_first.first][pos_first.second] == 0) {
        sleep(time_first);
        field[pos_first.first][pos_first.second] = 1;
        
        pthread_mutex_lock(&mutex);
        cout << "First gardener finnised working in cell " << pos_first.first << " " << pos_first.second << endl;
        fout << "First gardener finnised working in cell " << pos_first.first << " " << pos_first.second << endl;
        pthread_mutex_unlock(&mutex);
    }
    
    // старое направление движения
    int prev_dir = dir_first;
    pair<int, int> next = GetNext(1);
    
    // если вызодим за рамки останавливаем работу
    if (next.first >= n ||  next.second >= m ||  next.second < 0 ||  next.first < 0) {
        dir_first = prev_dir;
        return false;  
    }
    
    // если в следующей клетке другой садовник ждём один его рабочий цикл и никуда не идём
    if (next == pos_second){
        dir_first = prev_dir;
        pthread_mutex_lock(&mutex);
        cout << "First gardener sees Second gardener in next cell and waits" << endl;
        fout << "First gardener sees Second gardener in next cell and waits" << endl;
        pthread_mutex_unlock(&mutex);
        sleep(time_second);
        return true;
        
    }
    
    // выводим результат перехода
    pthread_mutex_lock(&mutex);
    pos_first = next;
    cout << "First gardener moved to cell " <<  pos_first.first << " " << pos_first.second << endl;
    fout << "First gardener moved to cell " <<  pos_first.first << " " << pos_first.second << endl;
    pthread_mutex_unlock(&mutex);
    
    
    
    return true;
}

bool MoveSecond() {
    // если клету надо обработать обрабатываем
    if (field[pos_second.first][pos_second.second] == 0) {
        sleep(time_second);
        field[pos_second.first][pos_second.second] = 1;
        
        pthread_mutex_lock(&mutex);
        cout << "Second gardener finnised working in cell " << pos_second.first << " " << pos_second.second << endl;
        fout << "Second gardener finnised working in cell " << pos_second.first << " " << pos_second.second << endl;
        pthread_mutex_unlock(&mutex);
    }
    
    // старое направление движения
    int prev_dir = dir_second;
    pair<int, int> next = GetNext(2);
    
    // если вызодим за рамки останавливаем работу
    if (next.first >= n ||  next.second >= m  || next.second < 0 ||  next.first < 0){
      dir_second = prev_dir;
      return false;  
    } 
    
    // если в следующей клетке другой садовник ждём один его рабочий цикл и никуда не идём
    if (next == pos_first){
        pthread_mutex_lock(&mutex);
        cout << "Second gardener sees First gardener in next cell and waits" << endl;
        fout << "Second gardener sees First gardener in next cell and waits" << endl;
        pthread_mutex_unlock(&mutex);
        sleep(time_first);
        return false;
        
    }
    
    // выводим результат перехода
    pthread_mutex_lock(&mutex);
    pos_second = next;
    cout << "Second gardener moved to cell " <<  pos_second.first << " " << pos_second.second << endl;
    fout << "Second gardener moved to cell " <<  pos_second.first << " " << pos_second.second << endl;
    //cout << "first gardener is at " <<  pos_first.first << " " << pos_first.second << endl;
    pthread_mutex_unlock(&mutex);
    sleep(1);
    
    return true;
}

void* func(void *param) {
    int p=*(int *)param ;  //номер садовника
    bool move = true;   // садовник может передвигаться дальше
    bool see = true;    // садовники не видят друг друга в своей следующей клетке и не могут идти дальше
    
    while (move && see) {
        // в зависимости от параметра двигаем соответствующего садовника
        if (p == 1) {
            move = MoveFirst();
            int prev_dir = dir_second;
            see = GetNext(2) != pos_first;
            dir_second = prev_dir;
        } else {
            move = MoveSecond();
            int prev_dir = dir_first;
            see = GetNext(1) != pos_second;
            dir_first = prev_dir;
        }
    }
    
    if (!see) {
        gardenersSeeEachOther = true; // если остановились из-за того что садовники столкнулись отмечаем это  событие
    }
    //вычисление элементов результирующей матрицы, стоящих в строке с номером p
    
    return nullptr;
}

// печатает сад
void PrintField() {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            cout << field[i][j] << " ";
            fout << field[i][j] << " ";
        }
        cout << endl;
        fout << endl;
    }
}

//int main(int argc, char* argv[])
int main()
{
    // получаем файл для вывода
    fout.open(stoi(argv[1]));
    
    from_file = stoi(argv[2]);
    
    if (from_file == 1) {
        fin.open(argv[3]);
        fin >> time_first >> time_second;
        
    } else {
        time_first = stoi(argv[3]);
        time_second = stoi(argv[4]);
    }
    
    time_first = max(time_first , 2);
    time_second = max(time_second, 2);
    
    //cin >> time_first >> time_second; // для online gdb
    cout << "Garden before works" << endl;
    fout << "Garden before works" << endl;
    FillField();    // зполняем поле (сад) камнями
    
    // выставляем позиции садовникам
    pos_first.first = 0;
    pos_first.second = 0;
    
    pos_second.first = n-1;
    pos_second.second = m-1;
    
    PrintField(); // выводим начальное состояние поля (сада)
    
     pthread_mutex_init(&mutex, NULL) ; //инициализация двоичного семафора
    //идентификаторы для дочерних потоков
    pthread_t gardener1,  gardener2 ;
    //аргументы для дочерних потоков
    int* par_1 = new int(1);
    int* par_2 = new int(2);
    
    // создаем потоки
    pthread_create(&gardener1, NULL, func, (void *)(par_1)) ;
    pthread_create(&gardener2, NULL, func, (void *)(par_2)) ;
    
    // соединяем потоки
    pthread_join(gardener1,NULL) ;
    pthread_join(gardener2,NULL) ;
    
    // если садовники не могут продолжать движение потому что они должны идти навстречу друг другу сообщаем об этом
    // иначе они закончили работать в саду
    if (!gardenersSeeEachOther) {
        cout << "Gardeners finnished working successfully" << endl;
    } else {
        cout << "Gardeners can not move" << endl;
    }
    
    // показываем результат работы
    PrintField();

    return 0;
}
