### Фролов Александр Сергеевич, БПИ217

### Вариант 12

Работа выполнялась на 9 баллов 

### Условие задачи:
Задача о гостинице - 1. В гостинице 30 номеров, клиенты гостиницы снимают номер на одни или несколько суток. Если в гостинице нет свободных номеров, клиенты не уходят, а устраиваются рядом с гостиницей на скамейках и ждут, пока любой из номеров не освободится. Создать приложение, моделирующее работу гостиницы. Каждого клиента реализовать в виде отдельного процесса.

### Идея задачи

Изначально есть некоторое количество клиентов. Каждый клиент пытается попасть в комнату, обращаясь к портье. Если свободные комнаты есть, его заселяют. Иначе гость ждёт. Если гостя заселили, то он проводит некоторое запланированное время в отеле, затем выселяется.
  
  
### Задача на языке программирования

Каждый клиент является процессом. Клиент периодически обращается к портье, стоящему на ресепшене в гостинице с 30 номерами, выводится сообщение о том, что клиент пытается заселиться. Если один из номеров свободен, то процесс данного клиента блокируется, выводится соответствующее сообщение о том, что клиент был заселен.Если ни одна из комнат не была свободна, то клиент ждёт и потом вновь обращается к портье. Когда клиент был заселен, его процесс ждёт время пребывания клиента в отеле. После того, как время остановки клиента истечет, его процесс будет возобновлён, выведется соответствующее сообщение. Так же у клиента изменится значение "has_stayed_for_his_time_", сигнализирующее о том, что клиент уже был в отеле планируемое время и более не обратится к портье за время работы программы.

### Что будет, если убрать процессы и разделяемую память ?

Если убрать из программы процессы и разделяемую память, все клиенты будут обслуживаться в порядке очереди. Это приведет к существенному замедлению работы программы (программа работает кратно медленнее). Вывод станет более организованым, но задача перестанет быть настолько приближенной к реальной жизни, так как каждый клиент будет ждать выселения предыдущего для того, чтобы попытаться заселиться. 

### Программа на 4 балла:

Задача решена с помощью семафоров UNIX SYSTEM V. Описание задачи и её идеи выше. Работа написанной программы описана в комментариях к коду. Вкратце: 

Есть некоторое кол-во клиентов. Кол-во клиентов вводится из файла. Каждый клиент представляет из себя отдельный процесс. Каждый клиент обращается к портье и пытается заселиться в понравившийся(рандомный) номер на жедлаемое(рандомное) кол-во дней. Если понравившийся номер занят, то посетитель ждёт, пока этот номер освободится. Если номер свободен, то клиент заселяется в него на случайное кол-во дней. На это кол-во "дней"номер занят. Затем номер высвобождается и новый клиент может в него заселиться. Когда все клиенты провели запланированное кол-во дней в отеле, программа завершает свою работу. 

### Программа на 5 баллов: 

В дополнение к реализации программы с помощью семафоров UNIX SYSTEM V приведено решение той же задачи, но с использованием именованных POSIX семафоров. Обмен данными ведется через разделяемую память в стандарте POSIX.

Основные различия семафоров POSIX и UNIX SYSTEM V: 

1) Именованные семафоры POSIX создаются в файловой системе, а системные семафоры UNIX SYSTEM V - в ядре операционной системы.

2) Именованные семафоры POSIX являются переносимыми между процессами и машинами, тогда как системные семафоры UNIX SYSTEM V могут использоваться только в пределах одной системы.

3) Семантика блокировки семафоров POSIX отличается от семантики блокировки семафоров UNIX SYSTEM V. В POSIX блокировка семафора не гарантирует блокировку процесса, который вызвал блокировку, в то время как в UNIX SYSTEM V блокировка семафора блокирует вызывающий процесс до тех пор, пока семафор не станет доступен.

4) В POSIX семафоры могут быть установлены с помощью атрибутов, в то время как в UNIX SYSTEM V все атрибуты определяются при создании семафора.

5) В POSIX семафоры могут иметь значение в диапазоне от 0 до SEM_VALUE_MAX, в то время как в UNIX SYSTEM V значения семафоров ограничены диапазоном int.

### Программа на 6 баллов:

В дополнение к реализации программы с помощью семафоров UNIX SYSTEM V приведено решение той же задачи, но использованием неименованных POSIX семафоров расположенных в разделяемой памяти. Обмен данными также ведется через разделяемую память в стандарте POSIX.

Основные отличия именованных POSIX семафоров от неименованных:

1) Именованные семафоры имеют имя в файловой системе и могут использоваться для синхронизации между процессами, в то время как неименованные семафоры располагаются в разделяемой памяти и могут использоваться только для синхронизации между потоками в рамках одного процесса.

2) Именованные семафоры могут создаваться до запуска процессов и жить в файловой системе даже после того, как процессы, создавшие их, завершат работу. Неименованные семафоры живут только в течение времени работы процесса, который их создал.

3) Именованные семафоры могут иметь несколько ссылок на один и тот же объект в файловой системе, что позволяет разным процессам или потокам синхронизироваться с одним и тем же объектом. Неименованные семафоры доступны только из одного процесса, создавшего их.

4) Для работы с именованными семафорами используются функции sem_open(), sem_close(), sem_unlink(), а для работы с неименованными - функции sem_init(), sem_destroy().

5) Именованные семафоры могут использоваться для синхронизации между разными процессами, а неименованные - только между потоками одного процесса. Это связано с тем, что для доступа к разделяемой памяти, в которой располагаются неименованные семафоры, требуется наличие общего адресного пространства, что возможно только для потоков, работающих в рамках одного процесса.

### Программа на 7 баллов:

