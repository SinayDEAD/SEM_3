Ибрагимова София 

Группа: 208

Задание: P05 

Вариант: всё, что успею 

Основное, 

       # конвейер

       # перенаправление ввода-вывода

       # запуск в фоновом режиме

Дополнительно из пункта IV,
       
       1. pr1 ; pr2 ; ... ; prN

       2. pr1 && pr2

       3. pr1 || pr2
     
_________________________________________________________________________________________


Итак, это крайняя посылка)

В папке, кроме ReadMe.txt, есть 

                                  shell.c          makefile           tests

Первое, это сама программа, вторая сборка, где появится "shell" и файл с тестами.

Для режима User, 
Требуется просто запустить программу, там просьба только не вводить 
Русские буквы, обидно вылетает иногда. На полную ахинею Шелл не будет реагировать.
В целом, +- всё работает и стабильно выдаёт ответ, возможно, где-то есть косяки 
(чуть-чуть бы времени ещё) :з


Для режима Test,
Можно просто ввести ./shell (имя файла), в нашем случае "tests". Он последовательно, 
построчно выполнит все команды.

Из полезного, как-то есть cd как идея, можно создать директорию и в неё уйти и потом,
если что, вернутся обратно по "cd ..". 


Для удобства просмотра, код +- весь прописан функциями последовательно.
(Парсинг только структурно выглядит пугающе, а так он очень удобный)) ).

Идея программы:

1. Прошу ввести строку (если там бред или ошибка в скобках, то она сразу ливнёт)
2. После парсирования идёт анализ того, что было введено ( строится дерево структуры)
3. Там тоже анализ бреда, например если увидит cat >> >, то ливнёт
4. Далее запускается выполнений
5. Если фон, то просто выйдет без ожидания
6. Смысл скобок, что я использую структуру и мотаю все дочерние и потом выхожу, команды 
   из пункта IV сами идейно не  сложны, просто прописала тип 0/1/2 и когда 
   получаю их на выбор, то жду и по выбору их делаю.
7. После выполнения, я удаляю дерево и строку и возвращаюсь ко вводу



P.S. Спасибо вам за задание, их было весело решать ночами 
     напролёт и разбираться в языке)



