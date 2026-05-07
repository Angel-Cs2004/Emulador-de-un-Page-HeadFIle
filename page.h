#ifndef PAGE_H
#define PAGE_H

#include <cstring>
#include <iostream>

// Tamaño fijo de una página: 4 KB
#define PAGE_SIZE 4096

// Cuántos slots puede tener como máximo una página
#define MAX_SLOTS 50

// -------------------------------------------------------
// Struct: un registro (fila de la "tabla")
// Por ahora guardamos id + nombre, como si fuera:
//   INSERT INTO usuarios VALUES (1, 'Ana');
// -------------------------------------------------------
struct Record {
  int id;
  char nombre[50];
};

// -------------------------------------------------------
// Struct: una entrada del directorio de slots
// Cada slot dice dónde está un registro dentro de la página
// y si está ocupado o fue borrado
// -------------------------------------------------------
struct SlotEntry {
  int offset; // posición en bytes dentro de la página
  int size;   // tamaño del registro en bytes
  bool valid; // true = ocupado, false = borrado/libre
};

// -------------------------------------------------------
// Struct: cabecera de la página
// Va al inicio de cada página y describe su estado
// -------------------------------------------------------
struct PageHeader {
  int pageID;          // número de esta página
  int numSlots;        // cuántos slots hay registrados
  int freeSpaceOffset; // hasta dónde llegan los registros (en bytes)
};

// -------------------------------------------------------
// Clase Page
// Representa una página de 4096 bytes en memoria.
// Organización interna:
//
//  [PageHeader | Record0 | Record1 | ... libre ... | SlotN | ... | Slot0]
//
// Los registros crecen desde el inicio (después del header)
// Los slots crecen desde el final hacia el inicio
// -------------------------------------------------------
class Page {
public:
  char data[PAGE_SIZE];
  Page(int id);
  Page();
  // Devuelve una referencia al header de la página
  PageHeader &getHeader();

  // Devuelve el slot en la posición slotIndex
  SlotEntry &getSlot(int slotIndex);

  // ---- Operaciones CRUD ----

  // INSERT: inserta un registro en la página
  // Devuelve el slotID asignado, o -1 si no hay espacio
  int insertRecord(const Record &rec);

  // SELECT: lee el registro del slot indicado
  // Devuelve true si lo encontró, false si el slot es inválido
  bool readRecord(int slotID, Record &out);

  // DELETE: marca el slot como inválido (borrado lógico)
  // Devuelve true si lo borró, false si el slotID no existe
  bool deleteRecord(int slotID);

  // UPDATE: actualiza el registro en slotID
  // Si cabe en el mismo espacio lo sobreescribe,
  // si no: borra e inserta de nuevo
  // Devuelve el slotID final (puede cambiar si se reinsertó)
  int updateRecord(int slotID, const Record &newRec);

  // ---- Utilidades ----

  // Imprime el estado actual de la página (para debug)
  void printPage();

  // Compacta el espacio libre (mueve registros válidos juntos)
  void compact();

  // Calcula cuántos bytes libres quedan en la página
  int freeSpace();
};

#endif // PAGE_H
