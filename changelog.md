# Changelog

All notable changes to this project will be documented in this file.

This project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

---

## [Unreleased]

### Added
- `interface` module to separate UI logic from core functionality.
- Placeholder for progress feedback (e.g., percentage or progress bar).
- Support for pressing ENTER to use default values during input.

### Changed
- rewrite seed scanning algorithm to spiral out from origin.

### Fixed
- 

### Deprecated
- 

### Removed
- 

---

## [0.1.0] – 2025-04-18

### Added
- Initial command-line version of MinecraftSeedFinder.
- User input for seed range and biome constraints.
- Biome name mapping via `unordered_map` in `config.h`.
- Static and dynamic linking support for Cubiomes.
- Working build system using CMake and MinGW Makefiles.
- Base `README.md` with build instructions.
- MIT License applied.

---

## [0.1.1] - 2025-04-25

### Added
- Refactored the I/O to an interface module

## [0.1.2] - 2025-04-30
- Added input validation
- Added default inputs (user can press enter to accept a defult value)
- Implemented a progress bar

<!-- TEMPLATE FOR FUTURE VERSIONS

## [X.Y.Z] – YYYY-MM-DD

### Added
- 

### Changed
- 

### Fixed
- 

### Deprecated
- 

### Removed
- 

-->
