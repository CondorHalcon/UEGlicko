# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## Unreleased

### Added
- `UGlickoSettings` class for plugin project settings

### Changed
- Moved Glicko config from `URating` to `UGlickoSettings`

## [0.1.0] - 2025-01-09

### Added

#### `URating`
- Glicko config as static values properties.
- `MakeRating` and `MakeRatingSimple` static functions
- `UpdateMatch` and `UpdateMatches` methods
- `Decay` method
- `Apply` method
- Glicko value get accessors
    - `getRating1`, `getDeviation1`, and `getDelta1` for Glicko-1
    - `getRating2`, `getDeviation2`, `getVolatility`, and `getDelta2` for Glicko-2
- Glicko format accessors `getGlicko1` and `getGlicko2`

#### `UMatch`
- `MakeMatch` static function
- `getOpponent` and `getScore` accessors
