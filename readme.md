# MinecraftSeedFinder

A command-line tool for brute-force analyzing Minecraft world seeds using biome constraints. This project leverages the [Cubiomes](https://github.com/Cubitect/cubiomes) library to determine biome layouts for Java Edition seeds, allowing users to find world seeds with desirable biome configurations near spawn.

---

## Features

- Search Minecraft world seeds by biome proximity to (0, 0)
- Specify starting seed and range
- Fast native performance via C++ and Cubiomes
- Support for common biome names
- Minecraft version selection via command-line
- Multithreaded search
- Rapid searching using JSON
- Search results are output into a logfile (seed_results.log)

---

## Help Menu

You can access the help menu from the command line by supplying the `-h` flag when launching the program:

```bash
./seedfinder.exe -h
```

---

## Minecraft Version Selection

You can choose which Minecraft version's world generation rules to use by supplying the `-v` flag when launching the program:

```bash
./seedfinder.exe -v MC_1_20_6
```

If no version is supplied, the latest available (`MC_NEWEST`) will be used by default.

**Supported versions:**


- MC_1_0_0  
- MC_1_1_0  
- MC_1_2_5  
- MC_1_3_2  
- MC_1_4_7  
- MC_1_5_2  
- MC_1_6_4  
- MC_1_7_10  
- MC_1_8_9  
- MC_1_9_4  
- MC_1_10_2  
- MC_1_11_2  
- MC_1_12_2  
- MC_1_13_2  
- MC_1_14_4  
- MC_1_15_2  
- MC_1_16_1  
- MC_1_16_5  
- MC_1_17_1  
- MC_1_18_2  
- MC_1_19_2  
- MC_1_19_4  
- MC_1_20_6  
- MC_1_21_1  
- MC_1_21_3  
- MC_NEWEST
---

## Multithreading

If `hardware_concurrency()` has trouble detecting a core count, it can be specified manually by using the `-t` flag when launching the program:

```bash
./seedfinder.exe -t 8
```
---
## Match Mode Setting

The program supports changing how the ouput is delivered. Users can select single match mode, or all match mode. Single match will return a single match (which one depends on the thread that finds a match first). All Matches will return all the matches in a search. options are `SINGLE_MATCH`, `ALL_MATCHES`. The default is `ALL_MATCHES`. The mode can be specified by using the `-m` flag when launching the program:

```bash
./seedfinder.exe -m FIRST_MATCH
```
---

## JSON Configuration Format

The program supports loading search parameters from a JSON file using the `-j` or `--json` command line option:

```
./seedfinder -j search_parameters.json
```

### JSON Format

```json
{
  "startSeed": 123456789,       // Starting seed (uint64_t)
  "seedsToCount": 10000,        // Number of seeds to check (uint64_t)
  "randomizeStartSeed": false,  // Whether to randomize the starting seed (bool)
  "constraints": [              // Array of biome constraints
    {
      "biomeID": "jagged_peaks",  // Biome name (string) or ID (integer)
      "maxDistance": 500          // Maximum distance from spawn (integer)
    },
    {
      "biomeID": 137,             // You can use numeric biome IDs as well
      "maxDistance": 300          // Default is 500 if not specified
    }
  ]
}
```

### Required Fields

- `constraints`: At least one constraint must be specified.
  - Each constraint must have a `biomeID` (either as a string name or numeric ID).
  - `maxDistance` is optional and defaults to 500 if not specified.

### Optional Fields

- `startSeed`: Starting seed for the search (defaults to 0).
- `seedsToCount`: Number of seeds to check (defaults to 1000).
- `randomizeStartSeed`: Whether to randomize the starting seed (defaults to false). **NOT YET IMPLEMENTED**

### Example Usage

```bash
# Use with custom thread count and Minecraft version
./seedfinder -j my_search.json -t 8 -v 1.18.2

# Just use JSON file with default settings
./seedfinder -j my_search.json
```
- A sample of the json can be found in `sample_search.json`
- When using a JSON file, the program will bypass the interactive interface and immediately start searching with the provided parameters.

## Project Structure

```
.
├── CMakeLists.txt         # Project build file
├── main.cpp               # Program entry point
├── argparser.h/.cpp       # Functions for parsing command line arguments
├── biomecache.h/.cpp      # Contains logic for caching the seed data
├── config.h/.cpp          # Configuration and Enums
├── interface.h/.cpp       # User interface & input/output logic
├── multithreading.h/.cpp  # Contains multithreading controls
├── seedfinder.h/.cpp      # Core biome constraint logic
└── cubiomes/              # External biome generation engine
```

---

## Building the Project with CMake + MinGW

This project uses CMake to build with MinGW on Windows.

### Prerequisites

- [CMake](https://cmake.org/download/)
- [MinGW-w64](http://mingw-w64.org/)

### Steps to Build

1. Create a build directory:

   mkdir build  
   cd build  

2. Build the Cubiomes library:

   cd ../cubiomes  
   mingw32-make  
   cd ../build  

3. Generate Makefiles using the MinGW generator:

   cmake .. -G "MinGW Makefiles"  

4. Build the main project:

   mingw32-make  

   The executable (e.g., `seedfinder.exe`) will be created in the `build/` directory.
---

## Running the Program

After successful compilation, run the executable from the `build` folder:

```
cd build
./seedfinder.exe
```

Optional:

```
./seedfinder.exe -v MC_1_20_6
./seedfinder.exe -t n (n = number of cores you want to run concurrently)
```

---

## Biome Name Reference

These names are currently supported via `config.h`:

|        |           |               |
|----------------------|--------------------------|--------------------------------|
| bamboo_jungle        | bamboo_jungle_hills      | basalt_deltas                  |
| beach                | badlands                 | badlands_plateau              |
| birch_forest         | birch_forest_hills       | birchForest                    |
| birchForestHills     | cherry_grove             | cold_ocean                     |
| coldBeach            | coldDeepOcean            | coldOcean                      |
| coldTaiga            | coldTaigaHills           | crimson_forest                 |
| deep_cold_ocean      | deep_dark                | deep_frozen_ocean              |
| deep_lukewarm_ocean  | deep_ocean               | deep_warm_ocean                |
| desert               | desert_hills             | desert_lakes                   |
| desertHills          | dripstone_caves          | end_barrens                    |
| end_highlands        | end_midlands             | eroded_badlands                |
| extremeHills         | extremeHillsEdge         | extremeHillsPlus               |
| flower_forest        | forest                   | forestHills                    |
| frozen_ocean         | frozen_river             | frozen_peaks                   |
| frozenDeepOcean      | frozenOcean              | frozenRiver                    |
| giant_spruce_taiga   | giant_spruce_taiga_hills | giant_tree_taiga              |
| giant_tree_taiga_hills | glacier               | gravelly_mountains            |
| grove                | hell                     | iceMountains                   |
| icePlains            | ice_spikes               | jagged_peaks                   |
| jungle               | jungle_edge              | jungle_hills                   |
| jungleEdge           | jungleHills              | lush_caves                     |
| lukewarm_ocean       | lukewarmDeepOcean        | lukewarmOcean                  |
| mangrove_swamp       | meadow                   | mesa                           |
| mesaPlateau          | mesaPlateau_F            | modified_badlands_plateau     |
| modified_gravelly_mountains | modified_jungle      | modified_jungle_edge          |
| modified_wooded_badlands_plateau | mountain_edge | mountains                      |
| mushroom_fields      | mushroom_field_shore     | mushroomIsland                 |
| mushroomIslandShore  | nether_wastes            | none                           |
| ocean                | old_growth_birch_forest  | old_growth_pine_taiga         |
| old_growth_spruce_taiga | pale_garden          | plains                         |
| rainforest           | river                    | roofedForest                   |
| savanna              | savanna_plateau          | savannaPlateau                |
| seasonal_forest      | shattered_savanna        | shattered_savanna_plateau     |
| shrubland            | sky                      | small_end_islands             |
| snow_plains          | snowy_beach              | snowy_mountains                |
| snowy_slopes         | snowy_taiga              | snowy_taiga_hills             |
| snowy_taiga_mountains | snowy_tundra           | sparse_jungle                 |
| stony_peaks          | stony_shore              | stone_shore                    |
| stoneBeach           | sunflower_plains         | swamp                          |
| swamp_hills          | swampland                | taiga                          |
| taiga_hills          | taiga_mountains          | taigaHills                     |
| the_end              | the_void                 | tall_birch_forest              |
| tall_birch_hills     | warped_forest            | warm_ocean                     |
| warmDeepOcean        | warmOcean                | windswept_forest              |
| windswept_gravelly_hills | windswept_hills     | windswept_savanna             |
| wooded_badlands      | wooded_badlands_plateau  | wooded_hills                   |
| wooded_mountains     | ice_spikes               | deepOcean                      |

---

## Performance Notes

Searching with a seed range of 50,000 may take a few seconds to a couple of minutes depending on biome constraints. Progress feedback and optimization features are under active development.

---

## Other Notes

- If your progress bar looks like this: ```[ΓûêΓûêΓûêΓûêΓûêΓûêΓûêΓûêΓûêΓ]``` you need to make sure your terminal is using UTF-8. In a Windows CMD terminal you can use:
 
  ```bash
  chcp 65001
  ```

- Logfile saves by default to the same folder as the executable.
- Single match mode returns a single match, not necessarily the first match in the seed sequence. 

---

## Future Plans

- Configurable search coordinates
- GUI frontend with seed map visualization

---

## Contributing

Feel free to fork, file issues, or submit pull requests. Contributions are welcome.

*TODO: Add contribution guidelines.*

---

## Author(s)

- Phrook – Project creator & maintainer

---

## License

This project is licensed under the MIT License. See the `LICENSE` file for details.

---

## Acknowledgements

- [Cubiomes Library](https://github.com/Cubitect/cubiomes) – Biome generation engine
- Minecraft® is a trademark of Mojang AB. This is an unofficial tool not affiliated with Mojang or Microsoft.

