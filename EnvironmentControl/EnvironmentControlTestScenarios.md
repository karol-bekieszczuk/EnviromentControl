# EnvironmentControl - Scenariusze testowe

Poniżej znajdują się scenariusze testowe dla logiki wykonywanej w `loop()` z pliku `EnvironmentControl.ino`.

## 1) Start cyklu (10:00)

- Ustaw czas na `10:00` (`currentTimeInMinutes = 600`).
- Oczekiwane działanie:
  - Swiatlo wlaczone (`digitalWrite(LIGHT_CTRL_PIN, HIGH)`).
  - Wentylator wylaczony, bo `600 % 21 != 0`.
  - `FanController` ustawiony na PWM `0%`.

## 2) W trakcie cyklu, bez aktywacji wentylatora (10:13)

- Ustaw czas na `10:13` (`currentTimeInMinutes = 613`).
- Oczekiwane działanie:
  - Swiatlo wlaczone.
  - Wentylator wylaczony, bo `613 % 21 != 0`.
  - `FanController` ustawiony na PWM `0%`.

## 3) Koniec cyklu (22:00)

- Ustaw czas na `22:00` (`currentTimeInMinutes = 1320`).
- Oczekiwane działanie:
  - Swiatlo wylaczone (`digitalWrite(LIGHT_CTRL_PIN, LOW)`).
  - Wentylator wylaczony.
  - `FanController` ustawiony na PWM `0%`.

## 4) Poza cyklem (03:00)

- Ustaw czas na `03:00` (`currentTimeInMinutes = 180`).
- Oczekiwane działanie:
  - Swiatlo wylaczone.
  - Wentylator wylaczony.
  - `FanController` ustawiony na PWM `0%`.

## 5) Granica startu cyklu (09:59 -> 10:00)

- Ustaw czas najpierw na `09:59` (`599`), a potem na `10:00` (`600`).
- Oczekiwane działanie:
  - Przy `599`: swiatlo wylaczone.
  - Przy `600`: swiatlo wlaczone.
  - Przy `600`: wentylator pozostaje wylaczony, bo `600 % 21 != 0`, PWM `0%`.
