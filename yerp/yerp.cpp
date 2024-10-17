#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <future>
#include <vector>
#include <thread>
#include <chrono>
#include <filesystem>
#include <iomanip>
#include <string>
#include <atomic>
#include <experimental/filesystem>
namespace fs = std::filesystem;
struct Foo {
    unsigned long seed;
    unsigned short result;
};
struct Range {
    unsigned long start;
    unsigned long stop;
};
struct Seed {
    int cycles;
    unsigned long startingSeed;
};

struct BiggerSeed {
    int lowest;
    unsigned long lowestSeed;
    int highest;
    unsigned long highestSeed;
    int lowest2;
    unsigned long lowestSeed2;
    int highest2;
    unsigned long highestSeed2;
};
struct DmgCalc {
    int critRoll;
    int dmgRoll;
    int accRoll;
    Foo value;
};
struct WanderMatrix {
    int height;
    int width;
    int startX;
    int startY;
};
// int seedCounts[800];
std::vector<std::atomic<int>> seedCounts(5000);

void threadFunction(std::vector<std::atomic<int>>& seedCounts, int index) {
    // Atomic increment
    seedCounts[index]++;
}

Foo advanceSeed(unsigned long seed){
    seed = (1103515245 * seed + 24691) & 0xFFFFFFFF;
    unsigned short result = seed / 65536;
    return {seed, result};
}
long long twice(long long m) {
    return 2 * m;
}
DmgCalc getDmgRoll(Foo value) {
    value = advanceSeed(value.seed);
    int critRoll =  value.result % 16;
    value = advanceSeed(value.seed);
    int damageRoll =  value.result % 16;
    value = advanceSeed(value.seed);
    int accuracyRoll =  value.result % 100;
    return {critRoll, damageRoll, accuracyRoll, value};
}
BiggerSeed getFastestFriends(Range r){
    BiggerSeed worstSeed = {0,0};
    worstSeed.lowest = 0;
    int friendshipMax = 255;
    int currentFriendship = 70;
    int walkCycles = 0;
    Foo value = {0,0};
    unsigned long seed = 0;
    for(unsigned long i = r.start; i < r.stop; i++){
        seed = i;
        currentFriendship = 0;
        walkCycles = 0;
        while(currentFriendship < 255){
            walkCycles = walkCycles + 1;
            value = advanceSeed(seed);
            seed = value.seed;
            if(value.result & 0x1){
                currentFriendship = currentFriendship + 1;
            }
        }
        if(walkCycles > worstSeed.highest){
            worstSeed.highest = walkCycles;
            worstSeed.highestSeed = i;
        }
        seedCounts[walkCycles] = seedCounts[walkCycles] + 1; 
    }
    return worstSeed;
}

BiggerSeed getMostBalls(Range r){
    BiggerSeed worstSeed = {0,0,0,0};
    bool uncaptured = true;
    unsigned long seed = 0;
    int ballsThrown = 0;
    int comparator = 49931;
    Foo value = {0,0};
    for(unsigned long i = r.start; i < r.stop; i++){
        seed = i;
        ballsThrown = 0;
        uncaptured = true;
        while(uncaptured){
            ballsThrown++;
            uncaptured = false;
            for(int j = 0; j < 4; j++){
                value = advanceSeed(seed);
                seed = value.seed;
                if(value.result >= comparator){
                    uncaptured = true;
                }
            }
            for(int k=0; k < 16; k++){
                value = advanceSeed(seed);
                seed = value.seed;
            }
        }
        if(ballsThrown > worstSeed.highest) {
            worstSeed.highest = ballsThrown;
            worstSeed.highestSeed = i;
        }
        seedCounts[ballsThrown] = seedCounts[ballsThrown] + 1;
    }
    return worstSeed;
}
BiggerSeed getMoveOutOfTheWayTime(Range r){
    BiggerSeed worstSeed = {0,0,0,0};
    bool moved = false;
    unsigned long seed = 0;
    int framesPassed = 0;
    Foo value = {0,0};
    int waittime = 0;
    int direction = 0;
    for(unsigned long i = r.start; i < r.stop; i++){
        seed = i;
        framesPassed = 0;
        moved = false;
        while(!moved){
            value = advanceSeed(seed);
            seed = value.seed;
            waittime = 30 + (32 * (value.result % 4));
            framesPassed = framesPassed + waittime;
            value = advanceSeed(seed);
            seed = value.seed;
            direction = value.result % 4;
            if(direction == 0){
                moved = true;
            } else if(direction == 1){
                framesPassed = framesPassed + 4;
            } else if(direction == 3){
                framesPassed = framesPassed + 4;
            } else if(direction == 2){
                framesPassed = framesPassed + 4;

            }
        }
        if(framesPassed >= worstSeed.highest) {
            worstSeed.highest = framesPassed;
            worstSeed.highestSeed = i;
        }
        framesPassed = framesPassed/3600;
        seedCounts[framesPassed] = seedCounts[framesPassed] + 1;
    }
    return worstSeed;
}

BiggerSeed getWanderTime(Range r){
    BiggerSeed worstSeed = {0,0,0,0};
    bool seen = false;
    unsigned long seed = 0;
    int framesPassed = 0;
    int xMax = 2;
    int yMax = 2;
    Foo value = {0,0};
    int waittime = 0;
    int currentPositionX = xMax;
    int currentPositionY = yMax;
    int arr[3][3] = {
            {0,0,0},
            {0,0,0},
            {0,0,1},
        };
    int direction = 0;
    for(unsigned long i = r.start; i < r.stop; i++){

        for(int j = 0; j < xMax + 1; j++){
            for(int k = 0; k < yMax + 1; k++){
                arr[j][k] = 0;
            }
        }
        arr[xMax][yMax] = 1;
        currentPositionX = xMax;
        currentPositionY = yMax;
        seed = i;
        framesPassed = 0;
        seen = false;
        while(!seen){
            arr[currentPositionY][currentPositionX] = 0;
            value = advanceSeed(seed);
            seed = value.seed;
            waittime = 30 + (32 * (value.result % 4));
            framesPassed = framesPassed + waittime;
            value = advanceSeed(seed);
            seed = value.seed;
            direction = value.result % 4;
            if(direction == 0){
                currentPositionY = currentPositionY + 1;
                if(currentPositionY > yMax) {
                    currentPositionY = yMax;
                    framesPassed = framesPassed + 4;
                } else {
                    framesPassed = framesPassed + 18;
                }
            } else if(direction == 1){
                currentPositionY = currentPositionY - 1;
                if(currentPositionY < 0){
                    currentPositionY = 0;
                    framesPassed = framesPassed + 4;
                } else {
                    framesPassed = framesPassed + 18;
                }
            } else if(direction == 3){
                currentPositionX = currentPositionX + 1;
                if(currentPositionX > xMax) {
                    currentPositionX = xMax;
                    framesPassed = framesPassed + 4;
                } else {
                    framesPassed = framesPassed + 18;
                }
            } else if(direction == 2){
                currentPositionX = currentPositionX - 1;
                if(currentPositionX < 0){
                    currentPositionX = 0;
                    framesPassed = framesPassed + 4;
                } else {
                    framesPassed = framesPassed + 18;
                }
            }
            if(currentPositionX == 0 && currentPositionY == 0 && direction == 1){
                seen = true;
            }
            arr[currentPositionY][currentPositionX] = direction + 1;
        }

        if(framesPassed >= worstSeed.highest) {
            worstSeed.highest = framesPassed;
            worstSeed.highestSeed = i;
        }
        if(framesPassed < worstSeed.lowest || worstSeed.lowest == 0){
            worstSeed.lowest = framesPassed;
            worstSeed.lowestSeed = i;
        }
        framesPassed = framesPassed / 60;
        if(framesPassed >= worstSeed.highest2) {
            worstSeed.highest2 = framesPassed;
            worstSeed.highestSeed2 = i;
        }
        if(framesPassed < worstSeed.lowest2 || worstSeed.lowest2 == 0){
            worstSeed.lowest2 = framesPassed;
            worstSeed.lowestSeed2 = i;
        }
        threadFunction(seedCounts, framesPassed);
    }
    return worstSeed;
}
//
BiggerSeed getWanderTimeSolo(unsigned long i){
    BiggerSeed worstSeed = {0,0,0,0};
    bool seen = false;
    unsigned long seed = 0;
    int framesPassed = 0;
    Foo value = {0,0};
    int waittime = 0;
    int currentPositionX = 2;
    int currentPositionY = 2;
    int arr[3][3] = {
            {0,0,0},
            {0,0,0},
            {0,0,1}
        };
    int direction = 0;

    for(int j = 0; j < 3; j++){
        for(int k = 0; k < 3; k++){
            arr[j][k] = 0;
        }
    }
    arr[2][2] = 1;
    currentPositionX = 2;
    currentPositionY = 2;
    seed = i;
    framesPassed = 0;
    seen = false;
    while(!seen){
        arr[currentPositionY][currentPositionX] = 0;
        value = advanceSeed(seed);
        seed = value.seed;
        waittime = 30 + (32 * (value.result % 4));
        framesPassed = framesPassed + waittime;
        value = advanceSeed(seed);
        seed = value.seed;
        direction = value.result % 4;
        if(direction == 0){
            // std::cout << "up" << std::endl;
            currentPositionY = currentPositionY + 1;
            if(currentPositionY > 2) {
                currentPositionY = 2;
                framesPassed = framesPassed + 4;
            } else {
                framesPassed = framesPassed + 18;
            }
        } else if(direction == 1){
            // std::cout << "down" << std::endl;
            currentPositionY = currentPositionY - 1;
            if(currentPositionY < 0){
                currentPositionY = 0;
                framesPassed = framesPassed + 4;
            } else {
                framesPassed = framesPassed + 18;
            }
        } else if(direction == 3){
            // std::cout << "right" << std::endl;
            currentPositionX = currentPositionX + 1;
            if(currentPositionX > 2) {
                currentPositionX = 2;
                framesPassed = framesPassed + 4;
            } else {
                framesPassed = framesPassed + 18;
            }
        } else if(direction == 2){
            // std::cout << "left" << std::endl;
            currentPositionX = currentPositionX - 1;
            if(currentPositionX < 0){
                currentPositionX = 0;
                framesPassed = framesPassed + 4;
            } else {
                framesPassed = framesPassed + 18;
            }
        }
        if(currentPositionX == 0 & currentPositionY == 0 & direction == 1){
            seen = true;
        } else if(currentPositionX == 0 && currentPositionY == 0) { 
            std::cout << "in the clurb" << std::endl;
        }
        arr[currentPositionY][currentPositionX] = direction + 1;
    }

    if(framesPassed >= worstSeed.highest) {
        worstSeed.highest = framesPassed;
        worstSeed.highestSeed = i;
    }
    if(framesPassed < worstSeed.lowest || worstSeed.lowest == 0){
        worstSeed.lowest = framesPassed;
        worstSeed.lowestSeed = i;
    }
    framesPassed = framesPassed / 60;
    std::cout << framesPassed << std::endl;
    if(framesPassed >= worstSeed.highest2) {
        worstSeed.highest2 = framesPassed;
        worstSeed.highestSeed2 = i;
    }
    if(framesPassed < worstSeed.lowest2 || worstSeed.lowest2 == 0){
        worstSeed.lowest2 = framesPassed;
        worstSeed.lowestSeed2 = i;
    }
    threadFunction(seedCounts, framesPassed);
    return worstSeed;
}
// rng cheatsheet
// +2 at start of round of combat
// +2 at start of your turn
    // +3 for using an attack (crit/dmg/acc)
    // +1 for using a status (acc) and +1 if the status has a random component (eg taunt)
    // +0 for using an item with no random component
    // +2 if you hit (CONDITIONAL on using something that hits)
// +2 at start of opponents turn
// +3/1/0 based on their move/action
    // +2 if they hit 
// AI advances
    // +4 if the AI has a guarenteed action (using a potion)
    // +10 if the AI is just selecting a move
    // +9 if the AI just swapped in a new pokemon
    // edit: this is outdated, AI rng use is much more complicated
Foo advanceSeedBy(Foo oSeed, int cycles) {
    for(int i = 0; i < cycles; i++){
        oSeed = advanceSeed(oSeed.seed);
    }
    return oSeed;
}
// simulation of the roark fight
Seed getDoubleCrit(Range r) {
    Seed worstSeed = {0,0};
    unsigned long seed = 0;
    Foo value = {0,0};
    int critRoll = 0;
    int damageRoll = 0;
    int accuracyRoll = 0;
    for(unsigned long i = r.start; i < r.stop; i++){
        seed = i;
        for(int j = 0; j < 6; j++){
            // advance 6 for turn start and geodude turn start nohit
            value = advanceSeed(seed);
            seed = value.seed;
            // using x attack this turn
        }
        value = advanceSeed(seed);
        critRoll = value.result % 16;
        value = advanceSeed(value.seed);
        damageRoll = value.result % 16;
        value = advanceSeed(value.seed);
        accuracyRoll = value.result % 100;
        if(accuracyRoll < 90) {
            // hits
            if(critRoll == 0 & damageRoll == 0){
                // max damage crit
                for(int j = 0; j < 16; j++){
                    // advance 16 for ai advances, hitting, and beginning of next turn
                    value = advanceSeed(value.seed);
                }
                value = advanceSeed(value.seed);
                critRoll = value.result % 16;
                value = advanceSeed(value.seed);
                damageRoll = value.result % 16;
                value = advanceSeed(value.seed);
                accuracyRoll = value.result % 100;
                // mach punch will hit
                if(critRoll != 0 & damageRoll > 5){
                    // mach punch does min damage
                    for(int j = 0; j < 4; j++){
                        // advance 4 for geodude start and getting hit
                        value = advanceSeed(value.seed);
                    }
                    value = advanceSeed(value.seed);
                    critRoll = value.result % 16;
                    value = advanceSeed(value.seed);
                    damageRoll = value.result % 16;
                    value = advanceSeed(value.seed);
                    accuracyRoll = value.result % 100;
                    if(critRoll == 0 & damageRoll == 0 & accuracyRoll < 90){
                        value = advanceSeedBy(value, 51); // using potions for a turn
                        value = advanceSeed(value.seed);
                        critRoll = value.result % 16;
                        value = advanceSeed(value.seed);
                        damageRoll = value.result % 16;
                        value = advanceSeed(value.seed);
                        accuracyRoll = value.result % 100;
                        if(critRoll == 0 & accuracyRoll < 90){
                            if(worstSeed.cycles == 0){
                                // worstSeed = {5, i};
                            }
                        }
                    }
                }

            }


        } else {
            value = advanceSeedBy(value, 26);
            accuracyRoll = value.result % 100;
            if(accuracyRoll > 90){
                if(worstSeed.cycles == 0){
                    worstSeed = {5, i};
                }
            }
        }



    }
    return worstSeed;
}
// spinning on a tile, how many moves must we make
// before guarenteed encounter

BiggerSeed getWildSpin(Range r) {
    BiggerSeed worstSeed = {0, 0};
    unsigned long seed = 0;
    Foo value = {0,0};
    int encRate = 30; // cave
    int moveRate = 30; // walking (spinning)
    bool battleStarted = false;
    for(unsigned long i = r.start; i < r.stop; i++){
        int cycles = 0;
        seed = i;
        battleStarted = false;
        while(!battleStarted) {
            value = advanceSeed(seed);
            seed = value.seed;
            if(value.result/0x290 < moveRate) {
                value = advanceSeed(seed);
                seed = value.seed;
                if(value.result/0x290 < encRate) {
                    battleStarted = true;
                }
            }
            cycles++;
        }
        if(cycles > worstSeed.highest) {
            worstSeed.highest = cycles;
            worstSeed.highestSeed = i;
        }
        seedCounts[cycles] = seedCounts[cycles] + 1; 

    }
    return worstSeed;
}
BiggerSeed getWildRepelEncounter(Range r) {
    BiggerSeed worstSeed = {0, 0, 0, 0};
    unsigned long seed = 0;
    Foo value = {0,0};
    int encRate = 30; // ravaged path // grass is 30
    int moveRate = 30; // walking (spinning)
    int maxSteps =  8 - ((encRate << 8) / 2560);
    int totalXp = 0;
    int totalEv = 0;
    int battles = 0;
    int stepCounter = 8;
    int totalSteps = 0;
    int slot = 0;
    int nature = 0;
    bool loop = true;
    unsigned long pidTop = 0;
    unsigned long pidBottom = 0;
    unsigned long pid = 0;
    // int xpSlots[12] = {30, 45, 38, 57, 23, 23, 68, 46, 46, 46, 46, 46}; // ravaged path
    int xpSlots[12] = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0};  // set xpslot with correct level mon to 1, others to 0
    int evSlots[12] = {0,1,0,1,0,1,0,1,0,1,0,1}; 
    for(unsigned long i = r.start; i < r.stop; i++){
        seed = i;
        totalXp = 0;
        totalEv = 0;
        battles = 0;
        stepCounter = 0;
        totalSteps = 0;
        value = advanceSeed(seed);
        slot = 0;
        nature = 0;
        bool foundMon = false;
        while(!foundMon) {
            if(stepCounter >= maxSteps | value.result/0x290 < 5){
                if(stepCounter < maxSteps){
                    value = advanceSeed(value.seed);
                }
                if(value.result/0x290 < moveRate) {
                    value = advanceSeed(value.seed);
                    if(value.result/0x290 < encRate) {
                        value = advanceSeed(value.seed);
                        slot = value.result/656;
                        if(slot < 20){
                            slot = 0;
                        } else if(slot < 40) {
                            slot = 1;
                        } else if(slot < 50) {
                            slot = 2;
                        } else if(slot < 60) {
                            slot = 3;
                        } else if(slot < 70) {
                            slot = 4;
                        } else if(slot < 80) {
                            slot = 5;
                        } else if(slot < 85) {
                            slot = 6;
                        } else if(slot < 90) {
                            slot = 7;
                        } else if(slot < 94){
                            slot = 8;
                        } else if(slot < 98) {
                            slot = 9;
                        } else if(slot == 98) {
                            slot = 10;
                        } else {
                            slot = 11;
                        }

                        // temp code
                        if(xpSlots[slot] > 0) {
                            foundMon = true;
                        }
                        if(foundMon) {
                            value = advanceSeed(value.seed);
                            nature = value.result / 0xA3E; // 0-24
                            loop = true;
                            while(loop) {
                                value = advanceSeed(value.seed);
                                pidBottom = value.result;
                                value = advanceSeed(value.seed);
                                pidTop = value.result * 65536;

                                pid = pidTop + pidBottom;
                                if(pid % 25 == nature){
                                    loop = false;
                                }
                            }
                            value = advanceSeed(value.seed); // iv
                            int IV = value.result;
                            value = advanceSeed(value.seed); // iv2
                            int IV2 = value.result;
                            int hpIV = IV%32;
                            int atkIV = IV/32;
                            int speedIv = IV2%32;
                            int spdIV = (IV2/1024)%32;
                            atkIV = atkIV%32;
                        }


                        // if(xpSlots[slot] > 0 && hpIV < 2 && atkIV < 2 && nature == 15 && nature != 0 && speedIv < 2 && spdIV < 2) {
                        //     battles++;
                        // } else if(xpSlots[slot] > 0) {
                        //     totalSteps = 9999999;
                        // }
                    stepCounter = -1;
                    }
                }
            }
            stepCounter++;
            totalSteps++;
            value= advanceSeed(value.seed);

        }
        threadFunction(seedCounts, totalSteps);

        if(totalSteps > worstSeed.highest | worstSeed.highest == 0){
            worstSeed.highest = totalSteps;
            worstSeed.highestSeed = i;
        } else if(totalSteps < worstSeed.lowest | worstSeed.lowest == 0 ) {
            worstSeed.lowest = totalSteps;
            worstSeed.lowestSeed = i;
        }

        if(battles > worstSeed.highest2 | worstSeed.highest2 == 0){
            worstSeed.highest2 = battles;
            worstSeed.highestSeed2 = i;
        } else if(battles < worstSeed.lowest2 | worstSeed.lowest2 == 0) {
            worstSeed.lowest2 = battles;
            worstSeed.lowestSeed2 = i;
        }
    }
    std::cout<< worstSeed.highest <<std::endl;

    return worstSeed;
}


BiggerSeed getWildRepelSurfEncounter(Range r) {
    BiggerSeed worstSeed = {0, 0, 0, 0};
    unsigned long seed = 0;
    Foo value = {0,0};
    int encRate = 10; // ravaged path // grass is 30
    int moveRate = 30; // walking (spinning)
    int maxSteps =  8 - ((encRate << 8) / 2560);
    int totalXp = 0;
    int totalEv = 0;
    int battles = 0;
    int stepCounter = 8;
    int totalSteps = 0;
    int slot = 0;
    int nature = 0;
    bool loop = true;
    unsigned long pidTop = 0;
    unsigned long pidBottom = 0;
    unsigned long pid = 0;
    // int xpSlots[12] = {30, 45, 38, 57, 23, 23, 68, 46, 46, 46, 46, 46}; // ravaged path
    int xpSlots[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1};  // set xpslot with correct level mon to 1, others to 0
    int evSlots[12] = {0,1,0,1,0,1,0,1,0,1,0,1}; 
    for(unsigned long i = r.start; i < r.stop; i++){
        seed = i;
        totalXp = 0;
        totalEv = 0;
        battles = 0;
        stepCounter = 8;
        totalSteps = 0;
        value = advanceSeed(seed);
        slot = 0;
        nature = 0;
        bool foundMon = false;
        while(!foundMon) {
            if(stepCounter >= maxSteps | value.result/0x290 < 5){
                if(stepCounter < maxSteps){
                    value = advanceSeed(value.seed);
                }
                if(value.result/0x290 < moveRate) {
                    value = advanceSeed(value.seed);
                    if(value.result/0x290 < encRate) {
                        value = advanceSeed(value.seed);
                        slot = value.result/656;
                        if(slot < 60){
                            slot = 0;
                        } else if(slot < 90) {
                            slot = 1;
                        } else if(slot < 95) {
                            slot = 2;
                        } else if(slot < 99) {
                            slot = 3;
                        } else {
                            slot = 4;
                        }

                        value = advanceSeed(value.seed);                        
                        if(slot > 1) {
                            if ((value.result % 21) + 20 == 40) {
                                foundMon = true;
                            }
                        }
                        if(foundMon) {
                            value = advanceSeed(value.seed);
                            nature = value.result / 0xA3E; // 0-24
                            loop = true;
                            while(loop) {
                                value = advanceSeed(value.seed);
                                pidBottom = value.result;
                                value = advanceSeed(value.seed);
                                pidTop = value.result * 65536;

                                pid = pidTop + pidBottom;
                                if(pid % 25 == nature){
                                    loop = false;
                                }
                            }
                            value = advanceSeed(value.seed); // iv
                            int IV = value.result;
                            value = advanceSeed(value.seed); // iv2
                            int IV2 = value.result;
                            int hpIV = IV%32;
                            int atkIV = IV/32;
                            int speedIv = IV2%32;
                            int spdIV = (IV2/1024)%32;
                            atkIV = atkIV%32;
                        }


                        // if(xpSlots[slot] > 0 && hpIV < 2 && atkIV < 2 && nature == 15 && nature != 0 && speedIv < 2 && spdIV < 2) {
                        //     battles++;
                        // } else if(xpSlots[slot] > 0) {
                        //     totalSteps = 9999999;
                        // }
                    stepCounter = -1;
                    }
                }
            }
            stepCounter++;
            totalSteps++;
            value= advanceSeed(value.seed);

        }
        if(totalSteps > worstSeed.highest | worstSeed.highest == 0){
            worstSeed.highest = totalSteps;
            worstSeed.highestSeed = i;
        } else if(totalSteps < worstSeed.lowest | worstSeed.lowest == 0 ) {
            worstSeed.lowest = totalSteps;
            worstSeed.lowestSeed = i;
        }

        if(battles > worstSeed.highest2 | worstSeed.highest2 == 0){
            worstSeed.highest2 = battles;
            worstSeed.highestSeed2 = i;
        } else if(battles < worstSeed.lowest2 | worstSeed.lowest2 == 0) {
            worstSeed.lowest2 = battles;
            worstSeed.lowestSeed2 = i;
        }
    }
    std::cout<< worstSeed.highest <<std::endl;

    return worstSeed;
}
BiggerSeed getWildXp(Range r) {
    BiggerSeed worstSeed = {0, 0, 0, 0};
    unsigned long seed = 0;
    Foo value = {0,0};
    int encRate = 30; // ravaged path // grass is 30
    int moveRate = 30; // walking (spinning)
    int maxSteps =  8 - ((encRate << 8) / 2560);
    int totalXp = 0;
    int totalEv = 0;
    int totalSteps = 0;
    int battles = 0;
    int stepCounter = 0;
    int slot = 0;
    int nature = 0;
    bool loop = true;
    unsigned long pidTop = 0;
    unsigned long pidBottom = 0;
    unsigned long pid = 0;
    // int xpSlots[12] = {30, 45, 38, 57, 23, 23, 68, 46, 46, 46, 46, 46}; // ravaged path
    // int xpSlots[12] = {16, 16, 24, 24, 24, 24, 32, 33, 32, 33, 32, 33}; // lake verity
    // int xpSlots[12] = {95, 101, 95, 101, 108, 115, 108, 115, 115, 115, 115, 115}; // old chat
    // int xpSlots[12] = {119, 114, 109, 119, 119, 119, 119, 119, 119, 149, 119, 162}; // maniac cave
    int xpSlots[12] = {16, 16, 24, 23, 24, 24, 24, 24, 16, 16, 16, 16}; // lake verity

    // int evSlots[12] = {0,1,0,1,0,1,0,1,0,1,0,1}; // lake verity
        int evSlots[12] = {1,1,1,1,1,1,1,1,1,1,1,1}; // lake verity
    for(unsigned long i = r.start; i < r.stop; i++){
        seed = i;
        totalXp = 0;
        totalEv = 0;
        battles = 0;
        stepCounter = 0;
        totalSteps = 0;
        value = advanceSeed(seed);
        slot = 0;
        nature = 0;
        while(totalXp < 284) {
            if(stepCounter >= maxSteps | value.result/0x290 < 5){
                if(stepCounter < maxSteps){
                    value = advanceSeed(value.seed);
                }
                if(value.result/0x290 < moveRate) {
                    value = advanceSeed(value.seed);
                    if(value.result/0x290 < encRate) {
                        value = advanceSeed(value.seed);
                        slot = value.result/656;
                        if(slot < 20){
                            slot = 0;
                        } else if(slot < 40) {
                            slot = 1;
                        } else if(slot < 50) {
                            slot = 2;
                        } else if(slot < 60) {
                            slot = 3;
                        } else if(slot < 70) {
                            slot = 4;
                        } else if(slot < 80) {
                            slot = 5;
                        } else if(slot < 85) {
                            slot = 6;
                        } else if(slot < 90) {
                            slot = 7;
                        } else if(slot < 94){
                            slot = 8;
                        } else if(slot < 98) {
                            slot = 9;
                        } else if(slot == 98) {
                            slot = 10;
                        } else {
                            slot = 11;
                        }
                    value = advanceSeed(value.seed);
                    nature = value.result / 0xA3E; // 0-24
                    loop = true;
                    while(loop) {
                        value = advanceSeed(value.seed);
                        pidBottom = value.result;
                        value = advanceSeed(value.seed);
                        pidTop = value.result * 65536;

                        pid = pidTop + pidBottom;
                        if(pid % 25 == nature){
                            loop = false;
                        }
                    }
                    value = advanceSeed(value.seed); // iv
                    value = advanceSeed(value.seed); // iv2
                    value = advanceSeed(value.seed);
                    value = advanceSeed(value.seed);
                    stepCounter = -1;
                    battles++;
                    totalXp = totalXp + xpSlots[slot];
                    totalEv = totalEv + evSlots[slot];
                    // std::cout << "slot: " << slot << std::endl;

                    }
                }
            }
            stepCounter++;
            totalSteps++;
            value= advanceSeed(value.seed);

        }
        threadFunction(seedCounts, totalXp - 284);
        if(totalXp > worstSeed.highest | worstSeed.highest == 0){
            worstSeed.highest = totalXp;
            worstSeed.highestSeed = i;
        } else if(totalXp < worstSeed.lowest | worstSeed.lowest == 0 ) {
            worstSeed.lowest = totalXp;
            worstSeed.lowestSeed = i;
        }

        if(battles > worstSeed.highest2 | worstSeed.highest2 == 0){
            worstSeed.highest2 = battles;
            worstSeed.highestSeed2 = i;
        } else if(battles < worstSeed.lowest2 | worstSeed.lowest2 == 0) {
            worstSeed.lowest2 = battles;
            worstSeed.lowestSeed2 = i;
        }
    }

    return worstSeed;
}
BiggerSeed getWildXpSweetScent(Range r) {
    BiggerSeed worstSeed = {0, 0, 0, 0};
    unsigned long seed = 0;
    Foo value = {0,0};
    int encRate = 10; // ravaged path // grass is 30
    int moveRate = 40; // walking (spinning)
    int maxSteps = 5;
    int totalXp = 0;
    int totalEv = 0;
    int battles = 0;
    int stepCounter = 0;
    int slot = 0;
    int nature = 0;
    bool loop = true;
    unsigned long pidTop = 0;
    unsigned long pidBottom = 0;
    unsigned long pid = 0;
    // int xpSlots[12] = {30, 45, 38, 57, 23, 23, 68, 46, 46, 46, 46, 46}; // ravaged path
    // int xpSlots[12] = {15, 22, 19, 28, 11, 11, 34, 23, 23, 23, 23, 23}; // ravaged path w/ xp share
    // int xpSlots[12] = {16, 16, 24, 24, 24, 24, 32, 33, 32, 33, 32, 33}; // lake verity
    // int xpSlots[12] = {95, 101, 95, 101, 108, 115, 108, 115, 115, 115, 115, 115}; // old chat
    int xpSlots[12] = {119, 114, 109, 119, 119, 119, 119, 119, 119, 149, 119, 162}; // maniac cave

    // int evSlots[12] = {0,2,0,2,0,2,0,2,0,2,0,2}; // lake verity
    // int evSlots[12] = {2,2,2,2,2,2,2,2,2,2,2,2}; // lake verity pokerus

    int evSlots[12] = {1,1,1,1,1,1,1,1,1,1,1,1}; // maniac cave
    // int evSlots[12] = {0,1,0,1,0,0,1,0,0,0,0,0}; // ravaged path
    // int evSlots[12] = {1,0,1,0,1,1,0,1,1,1,1,1}; // ravaged path
    int speedEvCount = 0;
    for(unsigned long i = r.start; i < r.stop; i++){
        seed = i;
        totalXp = 0;
        totalEv = 0;
        battles = 0;

        speedEvCount = 0;
        stepCounter = 0;
        value = advanceSeed(seed);
        slot = 0;
        nature = 0;
        while(battles < 25) {
           
                        value = advanceSeed(value.seed);
                        slot = value.result/656;
                        if(slot < 20){
                            slot = 0;
                        } else if(slot < 40) {
                            slot = 1;
                        } else if(slot < 50) {
                            slot = 2;
                        } else if(slot < 60) {
                            slot = 3;
                        } else if(slot < 70) {
                            slot = 4;
                        } else if(slot < 80) {
                            slot = 5;
                        } else if(slot < 85) {
                            slot = 6;
                        } else if(slot < 90) {
                            slot = 7;
                        } else if(slot < 94){
                            slot = 8;
                        } else if(slot < 98) {
                            slot = 9;
                        } else if(slot == 98) {
                            slot = 10;
                        } else {
                            slot = 11;
                        }
                    value = advanceSeed(value.seed);
                    nature = value.result / 0xA3E; // 0-24
                    loop = true;
                    while(loop) {
                        value = advanceSeed(value.seed);
                        pidBottom = value.result;
                        value = advanceSeed(value.seed);
                        pidTop = value.result * 65536;

                        pid = pidTop + pidBottom;
                        if(pid % 25 == nature){
                            loop = false;
                        }
                    }
                    value = advanceSeed(value.seed); // iv
                    value = advanceSeed(value.seed); // iv2
                    value = advanceSeed(value.seed);
                    value = advanceSeed(value.seed);
                    stepCounter = -1;
                    battles++;
                    if(slot%2 == 0) {
                        speedEvCount = speedEvCount + 2;
                    }
                    totalXp = totalXp + xpSlots[slot];
                    totalEv = totalEv + evSlots[slot];
        }
        threadFunction(seedCounts, totalXp);
        if(totalXp > worstSeed.highest | worstSeed.highest == 0){
            worstSeed.highest = totalXp;
            worstSeed.highestSeed = i;
        } else if(totalXp < worstSeed.lowest | worstSeed.lowest == 0 ) {
            worstSeed.lowest = totalXp;
            worstSeed.lowestSeed = i;
        }

        if(speedEvCount > worstSeed.highest2 | worstSeed.highest2 == 0){
            worstSeed.highest2 = speedEvCount;
            worstSeed.highestSeed2 = i;
        } else if(speedEvCount < worstSeed.lowest2 | worstSeed.lowest2 == 0) {
            worstSeed.lowest2 = speedEvCount;
            worstSeed.lowestSeed2 = i;
        }
    }


    return worstSeed;
}
void generateWild(unsigned long startingSeed) {
        int slot = 0;
    int nature = 0;
    bool loop = true;
    unsigned long pidTop = 0;
    unsigned long pidBottom = 0;
    unsigned long pid = 0;
    Foo value = advanceSeed(startingSeed);
    slot = value.result/656;
    if(slot < 20){
        slot = 0;
    } else if(slot < 40) {
        slot = 1;
    } else if(slot < 50) {
        slot = 2;
    } else if(slot < 60) {
        slot = 3;
    } else if(slot < 70) {
        slot = 4;
    } else if(slot < 80) {
        slot = 5;
    } else if(slot < 85) {
        slot = 6;
    } else if(slot < 90) {
        slot = 7;
    } else if(slot < 94){
        slot = 8;
    } else if(slot < 98) {
        slot = 9;
    } else if(slot == 98) {
        slot = 10;
    } else {
        slot = 11;
    }
value = advanceSeed(value.seed);
nature = value.result / 0xA3E; // 0-24
while(loop) {
    value = advanceSeed(value.seed);
    pidBottom = value.result;
    value = advanceSeed(value.seed);
    pidTop = value.result * 65536;

    pid = pidTop + pidBottom;
    if(pid % 25 == nature){
        loop = false;
    }
}
value = advanceSeed(value.seed); // iv
value = advanceSeed(value.seed); // iv2
value = advanceSeed(value.seed);
value = advanceSeed(value.seed);
std::cout << value.seed << std::endl;
}
BiggerSeed getShiny(Range r) {
    BiggerSeed worstSeed = {0, 0, 0, 0};
    unsigned long seed = 0;
    Foo value = {0,0};
    int encRate = 30; // ravaged path // grass is 30
    int moveRate = 40; // walking (spinning)
    int maxSteps =  8 - ((encRate << 8) / 2560);
    int sid = 18422;
    int tid = 38945;
    int battles = 0;
    int stepCounter = 0;
    int slot = 0;
    int nature = 0;
    bool loop = true;
    int shinyCount = 0;
    unsigned long pidTop = 0;
    unsigned long pidTopOriginal = 0;
    unsigned long pidBottom = 0;
    unsigned long pid = 0;
    // int xpSlots[12] = {30, 45, 38, 57, 23, 23, 68, 46, 46, 46, 46, 46}; // ravaged path
    int xpSlots[12] = {16, 16, 24, 24, 24, 24, 32, 33, 32, 33, 32, 33}; // lake verity
    int evSlots[12] = {0,1,0,1,0,1,0,1,0,1,0,1}; // lake verity
    for(unsigned long i = r.start; i < r.stop; i++){
        seed = i;
        battles = 0;
        stepCounter = 0;
        shinyCount = 0;
        value = advanceSeed(seed);
        slot = 0;
        nature = 0;
        while(battles < 1) {
            if(stepCounter >= maxSteps | value.result/0x290 < 5){
                if(stepCounter < maxSteps){
                    value = advanceSeed(value.seed);
                }
                if(value.result/0x290 < moveRate) {
                    value = advanceSeed(value.seed);
                    if(value.result/0x290 < encRate) {
                        value = advanceSeed(value.seed);
                        slot = value.result/656;
                        if(slot < 20){
                            slot = 0;
                        } else if(slot < 40) {
                            slot = 1;
                        } else if(slot < 50) {
                            slot = 2;
                        } else if(slot < 60) {
                            slot = 3;
                        } else if(slot < 70) {
                            slot = 4;
                        } else if(slot < 80) {
                            slot = 5;
                        } else if(slot < 85) {
                            slot = 6;
                        } else if(slot < 90) {
                            slot = 7;
                        } else if(slot < 94){
                            slot = 8;
                        } else if(slot < 98) {
                            slot = 9;
                        } else if(slot == 98) {
                            slot = 10;
                        } else {
                            slot = 11;
                        }
                    value = advanceSeed(value.seed);
                    nature = value.result / 0xA3E; // 0-24
                    loop = true;
                    while(loop) {
                        value = advanceSeed(value.seed);
                        pidBottom = value.result;
                        value = advanceSeed(value.seed);
                        pidTopOriginal = value.result;
                        pidTop = pidTopOriginal * 65536;

                        pid = pidTop + pidBottom;
                        if(pid % 25 == nature){
                            loop = false;
                        }
                    }
                    if((tid ^ sid ^ pidTopOriginal ^ pidBottom) < 8) {
                        if(slot==0) {
                            shinyCount++;
                        }
                    }
                    value = advanceSeed(value.seed); // iv
                    value = advanceSeed(value.seed); // iv2
                    value = advanceSeed(value.seed);
                    value = advanceSeed(value.seed);
                    stepCounter = -1;
                    battles++;

                    // std::cout << "slot: " << slot << std::endl;

                    }
                }
            }
            stepCounter++;
            value= advanceSeed(value.seed);

        }
        if(shinyCount >= worstSeed.highest | worstSeed.highest == 0){
            worstSeed.highest = shinyCount;
            worstSeed.highestSeed = i;
        }
    }

    return worstSeed;
}
BiggerSeed getPuruglyCrits(Range r) {
    BiggerSeed worstSeed = {0,0,0,0};
    unsigned long seed = 0;
    Foo value = {0,0};
    int critRoll = 0;
    int damageRoll = 0;
    int accuracyRoll = 0;
    DmgCalc dmg = {0,0,0,value};
    for(unsigned long i = r.start; i < r.stop; i++){
        seed = i;
        value.seed = i;
        value = advanceSeedBy(value, 6);
        dmg = getDmgRoll(value);
        value = dmg.value;
        if(dmg.critRoll == 0 & dmg.dmgRoll == 0){
            value = advanceSeedBy(value, 23); // go to next turn
            dmg = getDmgRoll(value);
            value = dmg.value;
            if(dmg.critRoll == 0 & dmg.dmgRoll == 0){
                value = advanceSeedBy(value, 17);
                dmg = getDmgRoll(value);
                value = dmg.value;
                if(dmg.critRoll == 0 & dmg.dmgRoll < 16) {
                    worstSeed.highestSeed = i;
                    worstSeed.highest = 5;
                }

            }

        }
    }
    return worstSeed;
}

BiggerSeed flipACoin(Range r) {
    BiggerSeed worstSeed = {0,0,0,0};
    unsigned long seed = 0;
    Foo value = {0,0};
    int totalHeads = 0;
    int totalTails = 0;
    for(unsigned long i = r.start; i < r.stop; i++){
        totalHeads = 0;
        totalTails = 0;
        value.seed = i;
        value = advanceSeed(value.seed);
        for(int j = 0; j<50;j++) {
            if(value.result % 2 == 0) {
                // even
                totalHeads++;
            } else {
                totalTails++;
            }
            value = advanceSeed(value.seed);
        }
        if(worstSeed.highest < totalHeads) {
            worstSeed.highest = totalHeads;
            worstSeed.highestSeed = i;
        }
    }
    return worstSeed;

}

BiggerSeed getPsyDuckCatch(Range r) {
    BiggerSeed worstSeed = {0,0,0,0};
    unsigned long seed = 0;
    Foo value = {0,0};
    int roll1 = 0;
    int roll2 = 0;
    int roll3 = 0;
    int roll4 = 0;
    bool captured = false;
    for(unsigned long i = r.start; i < r.stop; i++){
        value.seed = i;
        value = advanceSeedBy(value, 4);
        value = advanceSeed(value.seed);
        roll1 = value.result;
        if(roll1 < 65535){
            value = advanceSeed(value.seed);
            roll2 = value.result;
            if(roll2 < 65535){
                value = advanceSeed(value.seed);
                roll3 = value.result;
                if(roll3 < 65535){
                    value = advanceSeed(value.seed);
                    roll4 = value.result;
                    captured = true;
                }
            }
        }

        value = advanceSeed(value.seed);

    }
    return worstSeed;
}

int main()
{
    std::cout<<"<booting>"<<std::endl;

    // BiggerSeed ss = getWildRepelEncounter({1058110760, 1058110780});
    // std::cout << ss.highest << std::endl;
    // std::cout << ss.highestSeed << std::endl;
    // return 0;
    // getWanderTimeSolo(947055);
    // return 0;

    for(int i = 0; i < 5000; i++){
        seedCounts[i] = 0;
    }
    using std::chrono::high_resolution_clock;
    using std::chrono::duration_cast;
    using std::chrono::duration;
    using std::chrono::milliseconds;
    auto t1 = high_resolution_clock::now();
    std::string dirName = "./simulationResults";
    std::string resultsFilename = dirName + "/simulationResults.txt";
    fs::create_directories(dirName);
    std::ofstream resultsFile(resultsFilename);
    // unsigned long end = 4294967294UL;
    // unsigned long end = 1073741824UL;
    unsigned long end = 4310720UL;
    // unsigned long end = 1111262144UL;
    // unsigned long end = 9859990UL;
    int divisor = 16; // how many chunks to use
    unsigned long neow = end/divisor;
    BiggerSeed max = {0,0};
    BiggerSeed min = {0,0};
    BiggerSeed max2 = {0,0};
    BiggerSeed min2 = {0,0};
    BiggerSeed val = {0,0};
    std::vector<Range> ranges;
    for(int i = 0; i < divisor; i++){
        ranges.push_back({i * neow, (i+1) * neow});
    }
    // bigger seed for high/low stuff
    std::vector<std::future<BiggerSeed>> futures;
    for(auto &e : ranges){
        futures.push_back(std::async(getWanderTime, e));
    }
    //retrive and print the value stored in the future
    for(auto &e : futures) {
        val = e.get();

        std::cout << "starting seed: " << val.lowestSeed << "  lowest: " << val.lowest << "| starting seed: " << val.highestSeed << "  highest: " << val.highest << std::endl;
        std::cout << "starting seed: " << val.lowestSeed2 << "  lowest2: " << val.lowest2 << "| starting seed: " << val.highestSeed2 << "  highest2: " << val.highest2 << std::endl;
        if(val.highest > max.highest){
            max = val;
        }
        if(val.lowest < min.lowest | min.lowest == 0){
            min = val;
        }
        if(val.highest2 > max2.highest2){
            max2.highest2 = val.highest2;
            max2.highestSeed2 = val.highestSeed2;
        }
        if(val.lowest2 < min2.lowest2 | min2.lowest2 == 0){
            min2 = val;
        }
    }
    resultsFile << "______" << std::endl;
    resultsFile << "highest cycles: " << max.highest << std::endl;
    resultsFile << "starting seed: "<<max.highestSeed << std::endl;
    resultsFile << "lowest cycles: " << min.lowest << std::endl;
    resultsFile << "starting seed: "<< min.lowestSeed << std::endl;
    resultsFile << "highest2 cycles: " << max2.highest2 << "  starting seed: "<<max2.highestSeed2 << std::endl;
    resultsFile << "lowest2 cycles: " << min2.lowest2 << "  starting seed: "<< min2.lowestSeed2 << std::endl;
    auto t2 = high_resolution_clock::now();
     /* Getting number of milliseconds as an integer. */
    auto ms_int = duration_cast<milliseconds>(t2 - t1);

    /* Getting number of milliseconds as a double. */
    duration<double, std::milli> ms_double = t2 - t1;

    resultsFile << ms_int.count() << "ms\n";
    resultsFile << ms_double.count() << "ms\n";
    int totalCount = 0;
    int unlock = 0;
    for(int i = 0; i < 5000; i++){
        int val = seedCounts[i];
        totalCount = totalCount + val;
        if(val > 0) {
            unlock = 1;
        }
        if(unlock > 0) {
            resultsFile << seedCounts[i] << ", ";
        }

        // if(val > 0){
        //     // std::cout << seedCounts[i] << ", ";
        //     std::cout << i << ": " << seedCounts[i] << "\n";
        // }
    }
    std::cout << totalCount << std::endl;
    resultsFile.close();
    return 0;

}

