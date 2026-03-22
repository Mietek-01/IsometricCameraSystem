# IsometricCameraSystem

## Opis
System kamery izometrycznej dla city buildera. Zaimplementowany jako Pawn (`ACBIsometricCameraPawn`) — wyjątek od reguły "systemy lokalne = komponenty", ponieważ kamera wymaga posiadania przez PlayerController.

## Klasy

### ACBIsometricCameraPawn
Pawn z kamerą izometryczną. Obsługuje:
- **Pan** — WASD, edge scrolling (kursor przy krawędzi ekranu), middle mouse drag
- **Zoom** — scroll wheel, płynna interpolacja, konfigurowalne limity
- **Orbiting** — obrót wokół punktu na mapie (Q/E)
- **Tilt** — regulacja kąta nachylenia kamery (pitch)

Hierarchia komponentów:
```
RootPivot (USceneComponent) — punkt na mapie
  └─ SpringArm — dystans i kąt
       └─ Camera — renderowanie
```

Ruch jest obliczany względem yaw SpringArm, więc WASD zawsze odpowiada kierunkowi patrzenia kamery.

### UCBIsometricCameraDA
`UDataAsset` z konfiguracją kamery: prędkości ruchu, limity zoom/tilt, progi edge scrolling, prędkości interpolacji.

## Zależności
- **EnhancedInput** — bindowanie InputActions
- **UCBIsometricCameraDA** — wymagany (przypisywany w edytorze)

## Kategoria logów
`LogCBIsometricCameraSystem`

## Konfiguracja w edytorze
1. Stwórz 6 InputAction assetów (IA_CameraMove, IA_CameraZoom, IA_CameraRotate, IA_CameraTilt, IA_CameraDragPan, IA_CameraDragPanToggle)
2. Stwórz InputMappingContext i przypisz klawisze
3. Stwórz instancję UCBIsometricCameraDA
4. Przypisz IA, IMC i DataAsset do Pawna
5. Ustaw Pawna jako Default Pawn Class w PlayerController
