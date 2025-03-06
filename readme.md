# GOMOKU


## For compile the project :

```
make
```
## Game's Commands

**START [size]**

When the brain receives this command, it initializes itself and creates an empty board, but doesn't make any move yet. The parameter is size of the board. The brain must be able to play on board of size 20, because this size will be used in Gomocup tournaments. It is recommended but not required to support other board sizes. If the brain doesn't like the size, it responds ERROR. There can be a message after the ERROR word. The manager can try other sizes or it can display an error message to a user. The brain responds OK if it has been initialized successfully.

Example:

```
 The manager sends:
  START 20
 The brain answers:
  OK - everything is good
  ERROR message - unsupported size or other error
```

**TURN [X],[Y]**

The parameters are coordinate of the opponent's move. All coordinates are numbered from zero.

Expected answer:

```
 two comma-separated numbers - coordinates of the brain's move
```

Example:

```
 The manager sends:
  TURN 10,10
 The brain answers:
  11,10
```

**BEGIN**

This command is send by the manager to one of the players (brains) at the beginning of a match. This means that the brain is expected to play (open the match) on the empty playing board. After that the other brain obtains the TURN command with the first opponent's move. The BEGIN command is not used when automatic openings are enabled, because in that case both brains receive BOARD commands instead.

Expected answer:

```
 two numbers separated by comma - coordinates of the brain's move
```

Example:

```
 The manager sends:
  BEGIN
 The brain answers:
  10,10
```

**BOARD**

This command imposes entirely new playing field. It is suitable for continuation of an opened match or for undo/redo user commands. The BOARD command is usually send after START, RESTART or RECTSTART command when the board is empty. If there is any open match, the manager sends RESTART command before the BOARD command.

After this command the data forming the playing field are send. Every line is in the form:

 [X],[Y],[field]

where [X] and [Y] are coordinates and [field] is either number 1 (own stone) or number 2 (opponent's stone) or number 3 (only if continuous game is enabled, stone is part of winning line or is forbidden according to renju rules).

If game rule is renju, then the manager must send these lines in the same order as moves were made. If game rule is Gomoku, then the manager may send moves in any order and the brain must somehow cope with it. Data are ended by DONE command. Then the brain is expected to answer such as to TURN or BEGIN command.

Example:

```
 The manager sends:
  BOARD
  10,10,1
  10,11,2
  11,11,1
  9,10,2
  DONE
 The brain answers:
  9,9
```

**END**

When the brain obtains this command, it must terminate as soon as possible. The manager waits until the brain is finished. If the time of termination is too long (e.g. 1 second), the brain will be terminated by the manager. The brain should not write anything to output after the END command. However, the manager should not close the pipe until the brain is ended.

Expected answer:

```
none
```

The brain should delete its temporary files.

**ABOUT**

The brain is expected to send some information about itself on one line. Each info must be written as keyword, equals sign, text value in quotation marks. Recommended keywords are name, version, author, country, www, email. Values should be separated by commas that can be followed by spaces. The manager can use this info, but must cope with old brains that used to send only human-readable text.

Example:

```
 The manager sends:
  ABOUT
 The brain answers:
  name="SomeBrain", version="1.0", author="Nymand", country="USA"
```
