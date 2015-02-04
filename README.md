**Brainfuck-ng** is a powerful interpreter of a superset of *brainfuck*  language.  It  is full compatible with brainfuck, and has in addition some enhancements.

##Language overview
Brainfuck-ng operates on an array of memory cells, also referred to as the tape, each initially set to zero. There is a pointer, initially pointing to the first memory cell. The commands are:

| ------- | ------- |
| Command | Description |
| -------- | ---------- |
| > | Move the pointer to the right                                               |
| < | Move the pointer to the left                                                |
| + | Increment the memory cell under the pointer                                 |
| - | Decrement the memory cell under the pointer                                 |
| . | Output the character signified by the cell at the pointer                   |
| , | Input a character and store it in the cell at the pointer                   |
| `[` | Jump past the matching `]` if the cell under the pointer is 0             |
| ] | Jump back to the matching [ if the cell under the pointer is nonzero        |
| ( language extentions )                                                         |
| ! | output the integer signified by the cell at the pointer                     |
| ? | Input a character and store it in the cell at the pointer                   |
| @ | exit execution and return the value pointed by the last execution executed. |


All characters other than `><;+-.,[]!?@` should be considered comments and ignored. 

If no input environment array is given, the environment array is initially an array of a fixed dimension with all cells septed to zero . The initial dimension depends on interpreter implementation.

##Return value

Brainfuck-ng always return the value of the cell pointed by the last instruction executed.

##bfng - The brainfuck-ng interpreter

###repositories
* [brainfuck-ng on Github](http://github.com/elboza/brainfuck-ng/)
* [brainfuck-ng on Bitbucket](http://bitbucket.org/elboza/brainfuck-ng/)

###usage

```
 brainfuck-ng v0.1
 
 USAGE: bfng [options] [file]
 valid options:
 -i              --shell         interactive (shell mode)
 -h              --help          show this help
 -v              --version       prints brainfuck-ng version number
 -s              --stdin         read bf prog from stdin
 -d 'datum'      --data          set environment-array
 -o              --out           print env-array to stdout after computation
 -p              --print         same as -o but with newline added.
 -r              --reversefuck   switch 'ReverseFuck' mode on.
 -x 'prog'       --exec          gets & execute bf prog
 -c              --cin           gets environment-array from stdin
 -b  file        --xfile         gets bf prog from file
 -a  file        --dfile         gets environment-array from file
 ```
 
###interpreter's interactive commands
in shell interactive mode you can  give  brainfuck-ng  interpreter  the following command:

```
       :q     quit interpreter
       :h     print help summary
       :ks (yes|no)
              :ks  print  the  keep  state variable. :ks yes or :ks no set the
              keep state variable. The keep  state  variable  can  retain  the
              value of the environment-array between line input.
       :p     print the environment-array to stdout (added with \n endline).
       :set datum
              set the environment-array to datum (and set :ks yes)
       :z     print the last return value to stdout
```

##ReverseFuck mode
In the **ReverseFuck** mode all commands are switched to their semantic opposite. Here is an overview:

```
 Instructions:
 -      Increments the current cell (under the pointer).
 +      Decrements the current cell (under the pointer).
 >      Decrements pointer.
 <      Increments pointer.
 .      Inputs a character to current cell.
 ,      Outputs a character from current cell.
 ]      While the current cell value is not zero, do stuff inside ] and [ braces
 [      End while.
 !      input integer at current cell.
 ?      output integer from current cell.
 @      exit program returning current cel value( the only command not changed).
```

You can switch to ReverseFuck mode by adding the *-r* or *--reversefuck* option to the *bfng* interpreter.

##Examples

###Hello, World!

```
 ++++++++[>++++[>++>+++>+++>+<<<<-]>+>+>->>+[<]<-]>>.>---.+++++++..+++.>>.<-.<.+++.------.--------.>>+.>++.
```

This is a slightly more complex variant that often triggers interpreter bugs

```
 >++++++++[<+++++++++>-]<.>>+>+>++>[-]+<[>[->+<<++++>]<<]>.+++++++..+++.>
 >+++++++.<<<[[-]<[-]>]<+++++++++++++++.>>.+++.------.--------.>>+.>++++.
```

###bfng - The brainfuck-ng interpreter

* get the return value:

```
 % echo "+++"|bfng -s
 % echo $?
 3           #(the putput)
```

* give an environment and get the result:

```
 % echo "foobar"|bfng -c -x'+' -o
 goobar       #(the output)
 
 or
 
 % echo "+"|bfng -d'foobar' -p
 goobar        #(the output)
```

* give an environment and a program from files:

```
 % bfng -b program.bf -a env.data
```

* simply execute a program

```
 % bfng HelloWorld.bf
```

##External resources

* [brainfuck-ng on Github](http://github.com/elboza/brainfuck-ng/)
* [brainfuck-ng on Bitbucket](http://bitbucket.org/elboza/brainfuck-ng)



