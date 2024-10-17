# PlatinumSimulator

This is the code used to simulate randomness in Pokemon Platinum, used to make a sequence of buttons that always beats the game.  Please note a few things:

- This repository is put up as a reference for people who are curious, I had and have no intention of making it user-friendly

- This is the first piece of code I've ever written in C++

- Using it requires modifying the code directly, and there are lots of bits either missing or unincorporated for a variety of reasons

This is not a full simulator of pokemon, it is broken up into three sections:

`yerp` simulates overworld RNG

`mersenne` simulates the Mersenne Twister RNG, which is used for coin flips and Fantina's gym puzzle.  It's used for other things, but none that really matter.  The pattern in which rain falls, for example.

`everything else` simulates battles.  This is the main portion of the code, and is based off of work done in [the Pret Platinum Decompilation](https://github.com/pret/pokeplatinum).  Though it is not a fork, most of the code is rewritten or reconfigured, with some notable exceptions when it comes to lists of constant values.

The battle code is incomplete, it does not support any pokemon that are not present in battles for my project to beat every game of Pokemon Platinum, and does not support any battle mechanics that are absent.  For example, Inner Focus does not do anything, despite being on several pokemon, because I never use a move that flinches on a pokemon with Inner Focus throughout my run, and never had a reason to program in its functionality.

This applies to many many many different abilities, moves, and mechanics that are not present for the specific run that I did.

# Using this code

To use `yerp.cpp` or `mersenne.cpp`, you must edit the code, then compile it.  The two main variables you will want to edit are `end`, which defines how many seeds to test (I would not recommend setting it to 4 billion unless you know what you are doing) and `futures.push_back(std::async(<function>, e));`, which defines what function to run the seeds on.  

Each function defines some routine to test for each seed.  Functions pushed to `futures` will be passed a range of seeds to test.  You can edit the `divisor` variable to change how many chunks to create.  Each chunk will be passed to a different core in your computer.  If you have 16 cores, you can run the simulations in parallel by setting the divisor to `16`.  This will cause the program to consume 100% of your CPU power for as long as it takes to run all the simulations, by passing each core 1/16th of the set of seeds.

## The Battle Simulator

Using the battle simulator is somewhat complicated.  First, you must define the battle participants in `pokemon.cpp` and set their clients in `setupVarFight`.  You can view some exampes I've left in the code, though the list in this file is not complete.

Next you must set the commands you wish the simulator to use during the battle.  This can be done in `simulator.cpp` by modifying the `cList`.  

### Commands

The system of commands is a bit finicky, and uses branching and bitwise logic to handle reading in the commands.

Each command corresponds to some action.  `COMMAND_MOVE_SLOT_1` uses the move in slot 1 for the pokemon you currently have out.  `COMMAND_MOVE_SLOT_3` uses the 3rd move, `COMMAND_USE_ITEM_X_ATK` uses an XAttack, `COMMAND_USE_ITEM_FULL_RESTORE` uses a full restore, etc.  You can find a full list of commands in `battle.h`.  

The commands are actually bits in a 32 bit integer, where each command corresponds to a particular bit.  You can combine commands using the pipe operator `|`.  For example `COMMAND_USE_ITEM_FULL_RESTORE | POKE_SLOT_2` uses a full restore on the pokemon in your team at slot 2, instead of your current battler.  For switch commands, this is mandatory.  For example `COMMAND_SWITCH | POKE_SLOT_2` switches to the pokemon in slot 2.

Note that pokemon never change their slot during battle.  Just whether they are the current battler or not.

Commands are put into a nested list, where the index in the list corresponds to the branch and turn number for that command.  

### Branching

If the simulator receives a command it cannot complete, for example using a potion on a pokemon that is at full health, it will branch.  This causes it to start reading in commands on a different layer of the command matrix.  Here's a simple example:

```
cList.defaultCommand = COMMAND_MOVE_SLOT_2;
cList.commands[1][0] = COMMAND_USE_ITEM_ANTIDOTE;
cList.commands[2][0] = COMMAND_USE_ITEM_FULL_HEAL;
cList.commands[3][0] = COMMAND_MOVE_SLOT_2;

cList.commands[1][1] = COMMAND_USE_ITEM_FULL_HEAL;
cList.commands[2][1] = COMMAND_MOVE_SLOT_2;
cList.commands[3][1] = COMMAND_USE_ITEM_ANTIDOTE;

cList.commands[1][2] = COMMAND_MOVE_SLOT_2;
cList.commands[2][2] = COMMAND_USE_ITEM_ANTIDOTE;
cList.commands[3][2] = COMMAND_USE_ITEM_FULL_HEAL;
```

The commands start being read on branch 0, which is denoted by the second number in braces.  The turn number is denoted by the first number in braces.  After defining the default command, which will be used any time the simulator cannot find a command for the current turn/branch, I define three commands for branch 0 for the first 3 turns.  

The command 1 is to use an antidote.  In this example, I am unsure if my pokemon will be poisoned, which means if the simulator tries to use one and my pokemon is not poisoned, it will take no action.  This will cause it to branch one level up.  Now on branch 1, it will look again for command 1 (the command for turn 1), and finds a command to use a full heal.  Again, if the full heal does not do anything, it will branch up and find `COMMAND_MOVE_SLOT_2` on branch 2 for turn 1.  This is a command it can execute, so it executes this and the turn advances.

The simulator will remain on branch 2 now.  There is no way to go back down the branches unless you add `COMMAND_BRANCH_DEC` to a command to forcibly push it back down.  This would look like `COMMAND_MOVE_SLOT_2 | COMMAND_BRANCH_DEC`, and will decrease the branch by 1.  In-game, this would represent realigning different scenarios. 

There is currently no way to decrease the branch by more than 1 at a time, and I fear anybody that would require this functionality.

The simulator will continue executing commands for each turn until the battle is over.  Given that in the example there is nothing defined for branches above branch 2, branching again will just cause it to start picking the default command.

In this particular example, these commands represent using an antidote and full heal to test for poison/confusion before trying to attack.  So, first an antidote is used to check for poison.  If we aren't poisoned, we branch up and try a full heal instead.  If we are confused, this works and we will use an attacking move next turn.  If we are not confused, this branches up again and we use an attacking move this turn instead.  

In game, this must be achieved through the use of option-selecting and button/menu management to keep desynced games (games on different branches) moving along in the battle, or stalled while other games make their moves.

### Switching

If trying to simulate shift mode, commands can be appended with a switch command to indicate that the simulator should try switching to a different pokemon.  For example:

`COMMAND_MOVE_SLOT_2 | COMMAND_SWITCH | POKE_SLOT_3;`

This command indicates that the simulator should use the move in slot 2, and then if it KOs our opponent, switch to the pokemon on our team in slot 3.  The same thing applies if we want to switch after our own pokemon faints.  Essentially, given any opportunity to switch, the simulator will execute the tacked on switch command.  Otherwise, it will just ignore the switch command.

If you attempt to switch out a pokemon that is already out, the simulator will branch.  I recommend being careful with the tacked-on switch commands, as they can quickly cause branching nightmares.  Switch commands by themselves are executed normally.

Specifically, if you try to use baton pass, you must append the switch command for the slot you want to baton pass to.  Unfortunately, if you get hit and KOd on the turn you try to use baton pass, the simulator has no way of distinguishing whether to use the switch command that is meant for baton pass, or to branch because that is no longer an option, and will just switch out the pokemon you meant to baton pass out.  There is currently no way to avoid this niche problem, so don't try baton passing if there's a chance you might faint.


### Running a Simulation

To run a simulation, you must compile the code.  I do this using the following command in the root directory:

- Windows 

    - `g++ -o output.exe *.cpp`

- Linux

    - `g++ -o output.out *.cpp`

Running the resulting file, `output.out`, will run a simulation of whatever battle you have defined in the `setupVarFight` function of `pokemon.cpp`, using whatever commands you have defined in the `main` function of `simulator.cpp`. 

The executable takes several command line arguments:

| Flag | Description |
| ------------- | ------------- |
| -s  | The number of seeds to run.  `output.exe -s 16000` will run a simulation of the first 16000 seeds, for example.|
| -t | How many threads to use.  Set equal to CPU cores to run at full power.  eg. `output.exe -t 16` will use 16 threads to run the simulations faster |
| -l | Runs only the seeds in the `seeds.csv` file.  Mostly deprecated in favor of `-i`
| -i | Run seeds from the input file.  eg `output.exe -i customSeeds.csv` will only run the seeds defined in the given file.  This is useful if you know there is a specific subset of seeds you want to simulate
| -d | Debug options, has three modes.  Set to `1` for basic info, `3` for advanced info, and `7` to dump everything.  Not recommended for simulations of more than 1 seed.
| -z | Run a specific seed by itself, eg `output.exe -z 34918` will run a simulation of the battle on seed `34918`.
| -e | Causes threads to finish early the first time they find a losing seed.  Useful if you do not care about how many seeds lose, or which ones, to avoid wasting time simulating seeds on a set of commands that does not win in every seed.
| -w | Watch seeds that get to turn N, where N is the specified number.  eg `output.exe -w 5` will log all seeds that make it to turn 5.  This can then be used with `-i` to only rerun this specific subset of seeds.
|- o | Name of the output directory, will be created if it doesn't exist already.  This is where the simulation result report will be created.
| -p | Output specific results about the pokemon in the given slot.  eg `output.exe -p 3` will output hp info about the pokemon in slot 3.  Use `output.exe -p 7` to output hp info about the team as a whole, and log if anybody fainted during the battle regardless of if it was a win or loss.

A typical command might look like this:

`./output.exe -t 64 -s 4294967295 -o testResults -p 7`

This runs all 4 billion seeds, on 64 threads, logs the health/status of every pokemon on the team, and outputs the results to the `simulationResults/testResults` directory.  

I do not recommand running battle simulations of all 4 billion seeds unless you 

- Know what you're doing
- Don't mind your computer being unusable for a few hours

Depending on the strength of your computer, and how many cores the cpu has, full simulations of all 4 billion seeds could take from between 2-60 hours.  

For context, any full simulation I ran was done on a machine in the cloud with 64 cores and a highly powerful CPU.  These simulations took on average 5 hours.

Typically, the simulator will output summary text in the terminal after executing, which looks like this:

```
Player won. Lowest hp -> 46 // Seed: 1840 // Fainted: 0
Player won. Lowest hp -> 56 // Seed: 2382 // Fainted: 0
Player won. Lowest hp -> 71 // Seed: 3061 // Fainted: 0
Player won. Lowest hp -> 58 // Seed: 3496 // Fainted: 0
```

This indicates the lowest HP seed it found in each chunk (thread), indicates the seed number, and if you left the default option on or specified `-p 7`, how many of your team members fainted.



