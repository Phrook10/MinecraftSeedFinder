# MinecraftSeedFinder

A command-line tool for brute-force analyzing Minecraft world seeds using biome constraints. This project leverages the [Cubiomes](https://github.com/Cubitect/cubiomes) library to determine biome layouts for Java Edition seeds, allowing users to find world seeds with desirable biome configurations near spawn.

---

## 📸 Example Usage

```
Minecraft Biome Finder

Enter starting seed (default = 0): 123456
Enter seed range (default = 50000): 50000

Enter a biome ID (or type 'done' to finish): jungle
Enter max distance for this biome: 5000
Biome constraint added: jungle with max distance 5000 from origin

Searching...
Seed 123498 meets all biome constraints.
```

---

## 🧱 Features

- Search Minecraft world seeds by biome proximity to (0, 0)
- Specify starting seed and range
- Fast native performance via C++ and Cubiomes
- Support for common biome names (mapped in `config.h`)
- Modular architecture for future expansion
- Planned GUI in future releases

---

## 📂 Project Structure

```
.
├── CMakeLists.txt         # Project build file
├── main.cpp               # Program entry point
├── config.h               # Biome ID mappings and constants
├── seedfinder.h/.cpp      # Core biome constraint logic
├── interface.h/.cpp       # (WIP) User interface & input/output
└── cubiomes/              # External biome generation engine
```

---

## 🛠️ Building the Project with CMake + MinGW

This project uses CMake to build with MinGW on Windows.

### ✅ Prerequisites

- [CMake](https://cmake.org/download/) (add to PATH)
- [MinGW-w64](http://mingw-w64.org/) installed and added to PATH

### 📂 Steps to Build

1. **Create a Build Directory**  

   ```sh
   mkdir build
   cd build
   ```

2. **Generate Makefiles Using MinGW Generator**

   ```sh
   cmake -G "MinGW Makefiles" ..
   ```

3. **Build the Project**

   ```sh
   mingw32-make
   ```

   > This will produce the final executable (e.g., `seedfinder.exe`) in the `build/` directory.

---

Let me know if you'd like me to update it to include the `interface` library (once it's added) or if you want to support other platforms like MSVC or Linux later too.

### Running

After successful compilation, run the executable from the `build` folder:

```bash
cd build
./seedfinder.exe
```

---

## 🌍 Biome Name Reference

These names are supported via `config.h`:

- `jungle`
- `plains`
- `desert`
- `ice_spikes`
- `badlands`
- `mushroom_fields`
- `savanna`
- `taiga`
- *(more coming soon...)*

---

## 🧪 Performance Notes

> ⏱ Searching with a seed range of 50,000 may take a few seconds to a couple minutes depending on biome constraints. Progress feedback and optimization are in development.

---

## 💡 Future Plans

- [ ] GUI frontend with seed map visualization
- [ ] Multi-threaded scanning
- [ ] Configurable search coordinates
- [ ] Seed database integration
- [ ] Support for Minecraft Bedrock Edition
- [ ] Exportable search results
- [ ] Platform agnostic execution
- [ ] containerization

---

## 🙋‍♂️ Contributing

Feel free to fork, file issues, or submit PRs. Contributions welcome!\
TODO: Add contribution guidelines.

---

## 👤 Author(s)

- **Phrook** – Project creator & maintainer

---

## 📜 License

This project is licensed under the **MIT License**. See the [`LICENSE`](./LICENSE) file for details.

---

## 🧷 Acknowledgements

- [Cubiomes Library](https://github.com/Cubitect/cubiomes) – Biome generation engine
- Minecraft® is a trademark of Mojang AB. This is an unofficial tool not affiliated with Mojang or Microsoft.
