# Парсер аргументов командной строки

## Описание

Класс для парсинга аргументов командной строки.

Парсер умеет обрабатывать строки, целочисленные и булевые аргументы.  
Поддерживается:  
- полное и сокращенное название параметра
- единственное и множественное значение аргумента
- позиционность значений аргумента 
- сложение или умножение переданных аргументов.
- описание для каждого параметра

## Тесты

В качестве тестового фреймворка используется [GoogleTest](https://google.github.io/googletest/). Каждый тест представляет из себя функцию, в рамках которой проверяются некоторые условия за счет [assert'ов](https://google.github.io/googletest/reference/assertions.html).  Тесты используют два вида assert'a:

    - ASSERT_TRUE  - проверяет, что выражение является истинным (в противном случае тест неуспешен)
    - ASSERT_EQ    - проверяет, что аргументы эквивалентны

Например, тест

```cpp
TEST(ArgParserTestSuite, StringTest) {
    ArgParser parser("My Parser");
    parser.AddStringArgument("param1");

    ASSERT_TRUE(parser.Parse(SplitString("app --param1=value1")));
    ASSERT_EQ(parser.GetStringValue("param1"), "value1");
}
```

Проверяет, что

 - Вызов `parser.Parse(SplitString("app --param1=value1"))` вернет `true`
 - Вызов `parser.GetStringValue("param1")` вернет `"value1"`

## Примеры запуска

*labwork4 --sum 1 2 3 4 5*

*labwork4 --mult 1 2 3 4 5*
