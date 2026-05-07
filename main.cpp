#include "heapfile.h"
#include <cstring>
#include <iostream>

// Para compilar:
//   g++ main.cpp page.cpp heapfile.cpp -o lab03
// Para ejecutar:
//   ./lab03

void separador(const std::string &titulo) {
  std::cout << "\n========================================" << std::endl;
  std::cout << "  " << titulo << std::endl;
  std::cout << "========================================" << std::endl;
}

int main() {

  HeapFile hf("heapfile.bin");

  // ================================================
  // INSERT — agregar 3 registros
  // Equivale a:
  //   INSERT INTO usuarios VALUES (1, 'Ana');
  //   INSERT INTO usuarios VALUES (2, 'Luis');
  //   INSERT INTO usuarios VALUES (3, 'Maria');
  // ================================================
  separador("INSERT");

  Record r1;
  r1.id = 1;
  strcpy(r1.nombre, "Ana");
  Record r2;
  r2.id = 2;
  strcpy(r2.nombre, "Luis");
  Record r3;
  r3.id = 3;
  strcpy(r3.nombre, "Maria");

  HeapFile::RecordID rid1 = hf.insert(r1);
  HeapFile::RecordID rid2 = hf.insert(r2);
  HeapFile::RecordID rid3 = hf.insert(r3);

  std::cout << "Insertado: id=1 nombre=Ana    -> pageID=" << rid1.pageID
            << " slotID=" << rid1.slotID << std::endl;
  std::cout << "Insertado: id=2 nombre=Luis   -> pageID=" << rid2.pageID
            << " slotID=" << rid2.slotID << std::endl;
  std::cout << "Insertado: id=3 nombre=Maria  -> pageID=" << rid3.pageID
            << " slotID=" << rid3.slotID << std::endl;

  std::cout << "\nEstado de la pagina despues del INSERT:" << std::endl;
  hf.printAll();

  // ================================================
  // SELECT — leer los 3 registros
  // Equivale a:
  //   SELECT * FROM usuarios;
  // ================================================
  separador("SELECT");

  Record resultado;

  if (hf.read(rid1.pageID, rid1.slotID, resultado))
    std::cout << "Leido slot " << rid1.slotID << ": id=" << resultado.id
              << " nombre=" << resultado.nombre << std::endl;

  if (hf.read(rid2.pageID, rid2.slotID, resultado))
    std::cout << "Leido slot " << rid2.slotID << ": id=" << resultado.id
              << " nombre=" << resultado.nombre << std::endl;

  if (hf.read(rid3.pageID, rid3.slotID, resultado))
    std::cout << "Leido slot " << rid3.slotID << ": id=" << resultado.id
              << " nombre=" << resultado.nombre << std::endl;

  // ================================================
  // UPDATE — modificar el registro de Luis
  // Equivale a:
  //   UPDATE usuarios SET nombre='Luis M.' WHERE id=2;
  // ================================================
  separador("UPDATE");

  Record r2actualizado;
  r2actualizado.id = 2;
  strcpy(r2actualizado.nombre, "Luis M.");

  HeapFile::RecordID ridActualizado =
      hf.update(rid2.pageID, rid2.slotID, r2actualizado);
  std::cout << "Actualizado: id=2 nombre='Luis' -> 'Luis M.'" << std::endl;
  std::cout << "Nuevo pageID=" << ridActualizado.pageID
            << " slotID=" << ridActualizado.slotID << std::endl;

  std::cout << "\nEstado de la pagina despues del UPDATE:" << std::endl;
  hf.printAll();

  // ================================================
  // DELETE — borrar el registro de Ana
  // Equivale a:
  //   DELETE FROM usuarios WHERE id=1;
  // ================================================
  separador("DELETE");

  if (hf.remove(rid1.pageID, rid1.slotID))
    std::cout << "Borrado: id=1 nombre=Ana (slotID=" << rid1.slotID << ")"
              << std::endl;

  std::cout << "\nEstado final de la pagina despues del DELETE:" << std::endl;
  hf.printAll();

  return 0;
}
