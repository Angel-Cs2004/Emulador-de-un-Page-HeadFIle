#ifndef HEAPFILE_H
#define HEAPFILE_H

#include "page.h"
#include <fstream>
#include <string>

// -------------------------------------------------------
// Clase HeapFile
// Maneja el archivo binario en disco (heapfile.bin).
// Un archivo heap es simplemente varias páginas de 4096 bytes
// guardadas una tras otra:
//
//  [ Página 0 (4096 bytes) | Página 1 (4096 bytes) | ... ]
//
// Para leer la página N hacemos seek a: N * PAGE_SIZE
// -------------------------------------------------------
class HeapFile {
private:
  std::string filename; // nombre del archivo, ej: "heapfile.bin"
  std::fstream file;    // flujo de lectura/escritura binaria
  int numPages;         // cuántas páginas tiene el archivo actualmente

public:
  // Abre (o crea) el archivo heap en disco
  // Si el archivo no existe, lo crea vacío
  HeapFile(const std::string &fname);

  // Cierra el archivo al destruir el objeto
  ~HeapFile();

  // ---- Operaciones sobre páginas ----

  // Crea una nueva página al final del archivo
  // Devuelve el pageID de la nueva página
  int createPage();

  // Lee la página con ese pageID desde disco a memoria
  // Devuelve true si la leyó bien
  bool readPage(int pageID, Page &out);

  // Escribe la página en disco en la posición correcta
  // Devuelve true si la escribió bien
  bool writePage(int pageID, const Page &pg);

  // ---- Operaciones CRUD de alto nivel ----
  // (usan internamente readPage / writePage)

  // Inserta un registro buscando espacio en alguna página
  // Si ninguna tiene espacio, crea una página nueva
  // Devuelve {pageID, slotID} del registro insertado
  struct RecordID {
    int pageID;
    int slotID;
  };

  RecordID insert(const Record &rec);

  // Lee el registro en (pageID, slotID)
  // Devuelve true si lo encontró
  bool read(int pageID, int slotID, Record &out);

  // Borra el registro en (pageID, slotID)
  // Devuelve true si lo borró
  bool remove(int pageID, int slotID);

  // Actualiza el registro en (pageID, slotID)
  // Devuelve el nuevo RecordID (puede cambiar si se reinsertó)
  RecordID update(int pageID, int slotID, const Record &newRec);

  // ---- Utilidades ----

  // Devuelve cuántas páginas tiene el archivo
  int getNumPages();

  // Imprime todas las páginas (para debug)
  void printAll();
};

#endif // HEAPFILE_H
