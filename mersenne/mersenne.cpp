
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <future>
#include <vector>
#include <atomic>
#include <thread>
#include <chrono>
#include <string>
#include <barrier>

// simulates the mersenne rng of platinum
// mersenne is seeded the same way that the lcrng is, but it produces values much differently
// once you finish a simulation you can produce the 624 integer long sequence that is the actual seed for the mersenne value
// note that this is only accurate to a stable mersenne seed, IE, you don't advance the mersenne frame arbitrarily before beginning the simulation
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

struct MTRNG_CONTEXT {
    unsigned long sMTRNG_State[624];
    int sMTRNG_Size;

};
std::vector<std::atomic<int>> seedCounts(200);

void threadFunction(std::vector<std::atomic<int>>& seedCounts, int index) {
    // Atomic increment
    seedCounts[index]++;
}
// static unsigned long sMTRNG_State[624];
// static int sMTRNG_Size = 624 + 1;
static unsigned long sMTRNG_Xor[2] = {0x0UL, 0x9908b0dfUL};
static int ZONE1 = 0;
static int ZONE2 = 1;
typedef struct
{
	int zone_id;
	int start_no;
	int max_no;
	int start_gx;
	int start_gz;
	int size_gx;
	int size_gz;
}ZONE_DATA;
static const ZONE_DATA TableZoneData[2] =
{
	{ZONE1,0,3,1,3,15,8},
	{ZONE2,3,5,1,3,27,20},
};
static const int FirstRoomTable[15*8] =
{
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	1,1,1,0,1,1,1,1,1,1,1,0,1,1,1,
	0,1,0,0,0,1,1,1,1,1,0,0,1,0,0,
	0,1,0,1,0,1,1,1,1,1,0,1,1,0,0, // 0,1,0,0,0,1,1,1,1,1,0,0,1,0,0,
	0,1,1,1,0,1,0,0,0,0,0,1,1,1,0,
	0,1,1,1,0,0,1,1,1,0,0,1,1,1,0,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
};

static const int SecondRoomTable[27*20] =
{
	0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
	0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
	0,1,1,1,1,1,0,0,0,1,1,1,0,1,0,1,0,0,0,1,1,0,0,0,0,1,0,
	0,1,1,1,1,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,1,1,1,1,0,
	0,1,1,1,1,1,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,1,1,1,1,1,0, //	0,1,1,1,1,1,0,0,0,0,0,0,0,1,0,0,0,3,0,0,0,1,1,1,1,1,0,
	0,0,0,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,0,0,0,
	0,0,0,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,0,0,0,//	0,0,0,1,0,0,1,1,1,1,3,1,1,1,1,1,1,1,1,1,1,0,0,1,0,0,0,
	1,1,1,0,1,1,1,1,1,0,0,0,0,1,0,0,0,0,1,1,1,1,1,0,0,0,0,
	1,1,1,0,1,1,1,1,1,0,0,0,0,1,0,0,0,0,1,1,1,1,1,0,0,0,0,
	0,0,0,0,0,1,1,1,0,0,1,1,1,0,1,1,1,0,0,1,1,1,1,1,0,0,0,
	0,0,0,0,0,1,1,1,0,0,1,1,1,0,1,1,1,0,0,1,1,1,1,1,0,0,0,
	0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,1,1,1,1,0,1,1,1,1,1,1,
	0,0,0,1,0,0,0,1,1,1,1,0,0,1,0,0,1,1,1,1,0,1,1,1,1,1,1, //	0,0,0,3,0,0,0,1,1,1,1,0,0,1,0,0,1,1,1,1,0,1,1,1,1,1,1,
	0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,
	0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,1,0,0,1,0,0,0, // 	0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,3,0,0,1,0,0,0,
	1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,0,1,1,1,0,0,1,0,0,1,1,1,0,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,0,1,1,1,0,0,1,0,0,1,1,1,0,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,0,0,1,0,0,1,1,1,0,0,1,0,0,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
};
static const int * const RoomTables[2] =
{
	FirstRoomTable,
	SecondRoomTable,
};
void MTRNG_SetSeed (unsigned long seed, MTRNG_CONTEXT *rngc, bool displaySeed = false)
{
    rngc->sMTRNG_State[0] = seed & 0xffffffff;
    if(displaySeed) {
        std::cout << "{" << rngc->sMTRNG_State[0] << ",";
    }
    for (rngc->sMTRNG_Size = 1; rngc->sMTRNG_Size < 624; rngc->sMTRNG_Size++) {
        rngc->sMTRNG_State[rngc->sMTRNG_Size] = (1812433253UL * (rngc->sMTRNG_State[rngc->sMTRNG_Size - 1] ^ (rngc->sMTRNG_State[rngc->sMTRNG_Size - 1] >> 30)) + rngc->sMTRNG_Size);
        rngc->sMTRNG_State[rngc->sMTRNG_Size] &= 0xffffffff;
        if(displaySeed) {
            std::cout << rngc->sMTRNG_State[rngc->sMTRNG_Size] << ",";
        }
    }
    if(displaySeed) {
        std::cout << "}" << std::endl;
    }
}

unsigned long MTRNG_Next (MTRNG_CONTEXT *rngc)
{
    unsigned long result;

    if (rngc->sMTRNG_Size >= 624) {
        int i;

        if (rngc->sMTRNG_Size == 624 + 1) {
            MTRNG_SetSeed(5489UL, rngc);
        }

        for (i = 0; i < 624 - 397; i++) {
            result = (rngc->sMTRNG_State[i] & 0x80000000UL) | (rngc->sMTRNG_State[i + 1] & 0x7fffffffUL);
            rngc->sMTRNG_State[i] = rngc->sMTRNG_State[i + 397] ^ (result >> 1) ^ sMTRNG_Xor[result & 0x1UL];
        }

        for (; i < 624 - 1; i++) {
            result = (rngc->sMTRNG_State[i] & 0x80000000UL) | (rngc->sMTRNG_State[i + 1] & 0x7fffffffUL);
            rngc->sMTRNG_State[i] = rngc->sMTRNG_State[i + (397 - 624)] ^ (result >> 1) ^ sMTRNG_Xor[result & 0x1UL];
        }

        result = (rngc->sMTRNG_State[624 - 1] & 0x80000000UL) | (rngc->sMTRNG_State[0] & 0x7fffffffUL);

        rngc->sMTRNG_State[624 - 1] = rngc->sMTRNG_State[397 - 1] ^ (result >> 1) ^ sMTRNG_Xor[result & 0x1UL];
        rngc->sMTRNG_Size = 0;
    }

    result = rngc->sMTRNG_State[rngc->sMTRNG_Size++];

    result ^= (result >> 11);
    result ^= (result << 7) & 0x9d2c5680UL;
    result ^= (result << 15) & 0xefc60000UL;
    result ^= (result >> 18);

    return result;
}
int simulate(int zone_id, MTRNG_CONTEXT *rngc) {

    const ZONE_DATA *hint = TableZoneData;
    // int zone_id = ZONE2; // start in zone 1
    int stage = 0;
    do{
        if( hint->zone_id == zone_id ){ break; }
        hint++;
        stage++;
	}while(	stage < 2 );
    int hint_no = 0;
    int hint_gx =0;
    int hint_gz = 0;
    if( stage != 2 ){
        const int *hit = RoomTables[stage];	
        int rnd = MTRNG_Next(rngc) % hint->max_no;		
        hint_no = rnd + hint->start_no;
        
        do{
            hint_gx = MTRNG_Next(rngc) % hint->size_gx;
            hint_gz = MTRNG_Next(rngc) % hint->size_gz;
        }while( hit[(hint_gz*hint->size_gx)+hint_gx] == 1 );
        hint_gx += hint->start_gx;
        hint_gz += hint->start_gz;
    }
    return hint_no;
}
Seed fantinaSim(Range r) {
    // circle 0
    // square 1
    // triangle 2?

    // star 6
    Seed retSeed = {0, 0};
    int cycles;
    bool goToZone2;
    int hintNumber;
    bool finishedPuzzle;
    MTRNG_CONTEXT rngc;
    rngc.sMTRNG_Size = 624 + 1;
    for(int i = r.start; i < r.stop; i++){
        finishedPuzzle = false;
        MTRNG_SetSeed(i, &rngc);
        hintNumber = 0;
        cycles = 0;
        goToZone2 = false;
        while(!finishedPuzzle) {
            hintNumber = simulate(ZONE1, &rngc);
            if(hintNumber == 2) {
                // is triangle, the door we chose
                // goToZone2 = true;
                finishedPuzzle = true;
            }
            if(goToZone2) {
                hintNumber = simulate(ZONE2, &rngc);
                if(hintNumber == 7) {
                    finishedPuzzle = true;
                } else {
                    goToZone2 = false;
                }
            }
            cycles++;
        }

        if(cycles > retSeed.cycles) {
            retSeed.cycles = cycles;
            retSeed.startingSeed = i;
        }

    }

    return retSeed;

}

Seed fantinaOptimalSim(Range r) {
      // circle 0
    // square 1
    // triangle 2?

    // star 6
    std::vector<std::atomic<int>> indexHint(200);
    for(int i = 0; i < 200; i++){
        indexHint[i] = 0;
    }
    indexHint[0] = 0;
    Seed retSeed = {0, 0};
    int cycles;
    bool goToZone2;
    int hintNumber;
    bool finishedPuzzle;
    MTRNG_CONTEXT rngc;
    rngc.sMTRNG_Size = 624 + 1;
    int checkCycle = 1;
    for(int i = r.start; i < r.stop; i++){
        finishedPuzzle = false;
        MTRNG_SetSeed(i, &rngc);
        hintNumber = 0;
        cycles = 0;
        goToZone2 = false;
        while(cycles < checkCycle) {
            hintNumber = simulate(ZONE1, &rngc);


            if(hintNumber == indexHint[cycles]) {
                // is triangle, the door we chose
                goToZone2 = true;
            }
            if(goToZone2) {
                hintNumber = simulate(ZONE2, &rngc);
                if(hintNumber == 7) {
                    finishedPuzzle = true;
                } else {
                    goToZone2 = false;
                }
            }
            cycles++;
        }

        if(cycles > retSeed.cycles) {
            retSeed.cycles = cycles;
            retSeed.startingSeed = i;
        }

    }

    return retSeed;
}
Seed coinFlipSim(Range r) {
    Seed retSeed = {0, 0};
    int cycles;
    MTRNG_CONTEXT rngc;
    rngc.sMTRNG_Size = 624 + 1;
    for(int i = r.start; i < r.stop; i++){
        MTRNG_SetSeed(i, &rngc);
        cycles = 0;
        for(int j = 0; j < 100; j++) {
            // flip a coin 100 times
            cycles = cycles + (MTRNG_Next(&rngc) & 1);
        }
        threadFunction(seedCounts, cycles);
        if(cycles > retSeed.cycles) {
            retSeed.cycles = cycles;
            retSeed.startingSeed = i;
        }
    }
    return retSeed;

}
Seed coinFlipIndividual(unsigned long seed) {
    Seed retSeed = {0, 0};
    int cycles;
    MTRNG_CONTEXT rngc;
    rngc.sMTRNG_Size = 624 + 1;
    MTRNG_SetSeed(seed, &rngc);
    cycles = 0;
    for(int j = 0; j < 100; j++) {
        // flip a coin 100 times
        if(MTRNG_Next(&rngc) & 1) {
            std::cout << 1 << ',';
        } else {
            std::cout << 0 << ',';
        }
        // cycles = cycles + (MTRNG_Next(&rngc) & 1);
    }
    return retSeed;

}
void displaySeed(unsigned long i) {
    MTRNG_CONTEXT rngc;
    rngc.sMTRNG_Size = 624 + 1;
    MTRNG_SetSeed(i, &rngc, true);
}
int main()
{
    std::cout<<"<booting>"<<std::endl;
    // displaySeed(2462131795);
    // coinFlipIndividual(2462131795);
    // return 0;

    using std::chrono::high_resolution_clock;
    using std::chrono::duration_cast;
    using std::chrono::duration;
    using std::chrono::milliseconds;
    auto t1 = high_resolution_clock::now();
    for(int j = 0; j < 200; j++) {
        seedCounts[j] = 0;
    }
    // unsigned long end = 4294967294UL;
    // unsigned long end = 169496720UL;
    unsigned long end = 16000000UL;
    int divisor = 16; // how many chunks to use
    unsigned long neow = end/divisor;
    Seed val;
    Seed max = {0, 0};
    std::vector<Range> ranges;
    for(int i = 0; i < divisor; i++){
        ranges.push_back({i * neow, (i+1) * neow});
    }
    // bigger seed for high/low stuff
    std::vector<std::future<Seed>> futures;
    for(auto &e : ranges){
        futures.push_back(std::async(coinFlipSim, e));
    }
    //retrive and print the value stored in the future
    for(auto &e : futures) {
        val = e.get();
        std::cout << "starting seed: " << val.startingSeed << "  cycles: " << val.cycles << std::endl;
        if(val.cycles > max.cycles){
            max = val;
        }
    }
    std::cout << "______" << std::endl;
    std::cout << "highest cycles: " << max.cycles << std::endl;
    std::cout << "starting seed: "<<max.startingSeed << std::endl;
    auto t2 = high_resolution_clock::now();
     /* Getting number of milliseconds as an integer. */
    auto ms_int = duration_cast<milliseconds>(t2 - t1);

    /* Getting number of milliseconds as a double. */
    duration<double, std::milli> ms_double = t2 - t1;

    std::cout << ms_int.count() << "ms\n";
    std::cout << ms_double.count() << "ms\n";
    int op = 0;
    int unlock = 1;
    for(int i = 0; i < 200; i++){
        op = seedCounts[i];
        if(op > 0) {
            unlock = 1;
        }
        if(unlock > 0) {
            std::cout << seedCounts[i] << ", ";
        }

        // if(val > 0){
        //     // std::cout << seedCounts[i] << ", ";
        //     std::cout << i << ": " << seedCounts[i] << "\n";
        // }
    }
    return 0;

}

