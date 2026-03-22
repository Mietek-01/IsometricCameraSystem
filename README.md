# Isometric Camera System for UE 5.7

Gotowy system kamery izometrycznej dla Unreal Engine 5.7, napisany w C++. Zaprojektowany z myślą o grach typu city builder, RTS i podobnych gatunkach wymagających widoku z góry.

## Funkcjonalności

- **Pan** — WASD, edge scrolling (kursor przy krawędzi ekranu), przeciąganie środkowym przyciskiem myszy
- **Zoom** — scroll wheel z płynną interpolacją i konfigurowalnymi limitami (domyślnie 300–8000 jednostek)
- **Orbiting** — obrót kamery wokół punktu na mapie (Q/E)
- **Tilt** — regulacja kąta nachylenia (od -70° widok z góry do -20° prawie horyzontalny)
- **Dwutrybowy input** — przełączanie między trybem Gameplay (ukryty kursor) a trybem UI (widoczny kursor) z automatyczną podmianą Input Mapping Contexts

Wszystkie parametry (prędkości, limity, interpolacja, edge scroll threshold) są konfigurowalne przez Data Asset — bez konieczności zmian w kodzie.

## Architektura

```
ACBIsometricCameraPawn          — Pawn z kamerą, obsługuje input i ruch
│
├── RootPivot (SceneComponent)  — punkt obrotu na mapie
│   └── SpringArm               — kontroluje dystans (zoom) i kąt (tilt)
│       └── Camera               — renderowanie widoku

UCBIsometricCameraDA        — Data Asset z konfiguracją kamery
UCBInputControllerComp      — komponent na PlayerController, zarządza trybami inputu

```

Ruch jest obliczany względem yaw SpringArm — WASD zawsze odpowiada kierunkowi patrzenia kamery, niezależnie od obrotu.

## Struktura plików

```
Source/CityBuilder/
├── Public/CB/
│   ├── Systems/
│   │   ├── IsometricCameraSystem/
│   │   │   ├── CBIsometricCameraPawn.h
│   │   │   └── Data/CBIsometricCameraDA.h
│   │   └── InputSystem/
│   │       └── CBInputControllerComp.h
│   └── Utils/
│       └── CBLogCategories.h
└── Private/CB/                  # Implementacje (mirror struktury Public)

Content/CB/Systems/IsometricCameraSystem/
├── BP_IsoCamPawn.uasset         — Blueprint Pawna z przypisaną konfiguracją
├── Data/
│   └── DA_IsometricCameraConfig — Data Asset z parametrami kamery
└── Input/
    ├── IMC_IsoCamera            — Input Mapping Context
    ├── IA_IsoCameraMove         — WASD
    ├── IA_IsoCameraZoom         — scroll wheel
    ├── IA_IsoCameraRotate       — Q/E
    ├── IA_IsoCameraTilt         — zmiana kąta
    ├── IA_IsoCameraDragPan      — przeciąganie środkowym przyciskiem
    └── IA_IsoCameraDragPanToggle — toggle drag pan
```

## Wymagania

- Unreal Engine 5.7
- Visual Studio 2022+

## Setup

1. Sklonuj repozytorium i otwórz `CityBuilder.uproject`
2. Uruchom level `LVL_Basic` — wszystko jest już skonfigurowane (Input Actions, IMC, Data Asset, GameMode, PlayerController)

## Licencja

All rights reserved.
