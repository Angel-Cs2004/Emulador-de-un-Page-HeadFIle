# Laboratorio 03 — Heap File Storage

Implementación básica de un sistema de almacenamiento tipo Heap File en C++.  
Simula cómo una base de datos organiza y gestiona registros en páginas de disco.

## Descripción

El sistema organiza los datos en páginas de **4096 bytes**. Cada página tiene tres zonas internas: un header con metadatos, un área de registros, y un slot directory al final que indica dónde está cada registro. Se implementan las 4 operaciones CRUD sobre estas páginas, con persistencia en un archivo binario.

## Archivos

| Archivo | Descripción |
|---|---|
| `page.h` | Structs (`PageHeader`, `Record`, `SlotEntry`) y declaración de la clase `Page` |
| `page.cpp` | Lógica CRUD sobre la página en memoria |
| `heapfile.h` | Declaración de la clase `HeapFile` |
| `heapfile.cpp` | Lectura y escritura de páginas en disco (`heapfile.bin`) |
| `main.cpp` | Demostración de las 4 operaciones con datos de ejemplo |

## Requisitos

- Compilador g++ con soporte C++11 o superior
- Sistema operativo Linux, macOS o Windows con MinGW

## Compilar y ejecutar

```bash
g++ main.cpp page.cpp heapfile.cpp -o lab03
./lab03
```

## Ejemplo de salida

```
INSERT
  Insertado: id=1 nombre=Ana   -> pageID=0 slotID=0
  Insertado: id=2 nombre=Luis  -> pageID=0 slotID=1
  Insertado: id=3 nombre=Maria -> pageID=0 slotID=2

SELECT
  Leido slot 0: id=1 nombre=Ana
  Leido slot 1: id=2 nombre=Luis
  Leido slot 2: id=3 nombre=Maria

UPDATE
  Actualizado: id=2 nombre='Luis' -> 'Luis M.'

DELETE
  Borrado: id=1 nombre=Ana (slotID=0)
  Slot 0: [borrado]
```

## Estructura de una página

```
[ Header | Registro0 | Registro1 | ... libre ... | SlotN | ... | Slot0 ]
0 bytes                                                          4096 bytes
```

Los registros crecen desde el inicio. Los slots crecen desde el final hacia adentro.
