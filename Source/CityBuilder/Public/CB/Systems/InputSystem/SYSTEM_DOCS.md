# InputSystem

## Opis
System zarządzania inputem i trybami sterowania (Gameplay / UI). Komponent lokalny dodawany do PlayerController — obsługuje przełączanie InputMappingContext oraz widoczność kursora.

## Klasy

### UCBInputControllerComp
Komponent zarządzający:
- **Input Mapping Contexts** — dodawanie/usuwanie IMC z EnhancedInput
- **Tryb Game** — ukryty kursor, `FInputModeGameOnly`
- **Tryb UI** — widoczny kursor, `FInputModeGameAndUI` z focusem na viewport (klawiatura działa bez klikania)
- **Przełączanie trybów** — `EnableUIMode()` / `DisableUIMode()`, z zapamiętywaniem poprzednich map

Konfiguracja:
- `IsGameModeOnStart` — czy zaczynać w trybie Game (true) czy UI (false)
- `DefaultGameModeInputMaps` — IMC dla trybu Game
- `DefaultUIModeInputMaps` — IMC dla trybu UI

Event:
- `InputMapChanged` — broadcast przy każdej zmianie map (poprzednie mapy, nowe mapy, referencja do komponentu)

## Zależności
- **EnhancedInput** — `UEnhancedInputLocalPlayerSubsystem` do zarządzania IMC
- **Slate** — `FSlateApplication` do ustawienia focusu viewportu w trybie UI

## Kategoria logów
`LogCBInputController`

## Konfiguracja w edytorze
1. Dodaj `UCBInputControllerComp` do PlayerController
2. Ustaw `IsGameModeOnStart` (domyślnie true)
3. Przypisz IMC do `DefaultGameModeInputMaps` i/lub `DefaultUIModeInputMaps`
