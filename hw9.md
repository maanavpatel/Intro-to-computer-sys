Provide an one-sentence description of the example code they provide in the paper

+ The code counts the occurrences of each word in a string.

Provide an example input that might be used with their example code.

+ map("name","Wade is awesome and so is Maanav")
+ reduce("is", list(1,1))

Provide the output of their result to your sample input (it’s a set of key-value pairs, which is easily represented in JSON format)

+ map:
```
{
"Wade": 1,
"is": 1,
"awesome":1,
"and": 1,
"so" : 1,
"is": 1,
"Maanav": 1
}
```
+ reduce for `is`:
```
2
```
