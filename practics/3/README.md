# Задание 1
### Условие

Реализовать на Jsonnet приведенный ниже пример в формате JSON. Использовать в реализации свойство программируемости и принцип DRY.

### Код

```jsonnet
local groupPrefix = 'БИВТ-';
local year = '-21';
local groupNum = std.range(1, 10);

local studentData = [
  {name: "Александров А.А.", age: 20, groupIndex: 2},
  {name: "Борисов Б.Б.", age: 21, groupIndex: 3},
  {name: "Васильев В.В.", age: 22, groupIndex: 1},
  {name: "Григорьев Г.Г.", age: 20, groupIndex: 4}
];

{
  groups: [groupPrefix + std.toString(i) + year for i in groupNum],

  students: [
    {
      age: student.age,
      group: groupPrefix + std.toString(student.groupIndex) + year,
      name: student.name
    } for student in studentData
  ],

  subject: "Программирование"
}

```

### Вывод

```json
{
  "groups": [
    "БИВТ-1-21",
    "БИВТ-2-21",
    "БИВТ-3-21",
    "БИВТ-4-21",
    "БИВТ-5-21",
    "БИВТ-6-21",
    "БИВТ-7-21",
    "БИВТ-8-21",
    "БИВТ-9-21",
    "БИВТ-10-21"
  ],
  "students": [
    {
      "age": 20,
      "group": "БИВТ-2-21",
      "name": "Александров А.А."
    },
    {
      "age": 21,
      "group": "БИВТ-3-21",
      "name": "Борисов Б.Б."
    },
    {
      "age": 22,
      "group": "БИВТ-1-21",
      "name": "Васильев В.В."
    },
    {
      "age": 20,
      "group": "БИВТ-4-21",
      "name": "Григорьев Г.Г."
    }
  ],
  "subject": "Программирование"
}
```



# Задание 2
### Условие

Реализовать на Dhall приведенный ниже пример в формате JSON. Использовать в реализации свойство программируемости и принцип DRY.

### Код

```dhall
let Group = Text
let Student = { age : Natural, group : Group, name : Text }

let createGroup : Natural -> Group =
      λ(n : Natural) → "БИВТ-" ++ (Natural/show n) ++ "-21"

let groups =
      List/map
        Natural
        Group
        createGroup
        (List/replicate 10 (λ(i : Natural) → i + 1))

let createStudent : Natural -> Group -> Text -> Student =
      λ(age : Natural) → λ(group : Group) → λ(name : Text) →
        { age = age, group = group, name = name }

let students =
  [ createStudent 20 (createGroup 2) "Александров А.А."
  , createStudent 21 (createGroup 3) "Борисов Б.Б."
  , createStudent 22 (createGroup 1) "Васильев В.В."
  , createStudent 20 (createGroup 4) "Григорьев Г.Г."
  ]

in  { groups = groups, students = students, subject = "Программирование" }
```



# Задание 3
### Условие

Язык нулей и единиц.

### Код

```
<digit> ::= 0 | 1
<string> ::= <digit> | <digit> <string>
```



# Задание 4
### Условие

Язык правильно расставленных скобок двух видов.

### Код

```
<openFirst> ::= "("
<openSecond> ::= "{"
<closeFirst> ::= ")"
<closeSecond> ::= "}"
<string> ::= <openFirst> <string> <closeFirst> 
           | <openSecond> <string> <closeSecond> 
           | <openFirst> <closeFirst> 
           | <openSecond> <closeSecond> 
           | ""
```



# Задание 5
### Условие

Язык правильно расставленных скобок двух видов.

### Код

```
<expression> ::= <term>
               | <open> <term> <operation> <term> <close>
               | <negative> <open> <term> <operation> <term> <close>
               | <open> <expression> <operation> <expression> <close>
               | <negative> <open> <expression> <close>

<term> ::= <variable>
         | <negative> <variable>
         | <open> <variable> <operation> <variable> <close>
         | <negative> <open> <variable> <operation> <variable> <close>

<variable> ::= x | y | z | w

<operation> ::= & | |

<negative> ::= ~

<open> ::= (

<close> ::= )
```
