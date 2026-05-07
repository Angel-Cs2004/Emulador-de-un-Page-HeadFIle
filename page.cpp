#include "page.h"

Page::Page(int id) {
  memset(data, 0, PAGE_SIZE);
  PageHeader &h = getHeader();
  h.pageID = id;
  h.numSlots = 0;
  h.freeSpaceOffset = sizeof(PageHeader);
}

Page::Page() { memset(data, 0, PAGE_SIZE); }

PageHeader &Page::getHeader() { return *reinterpret_cast<PageHeader *>(data); }

SlotEntry &Page::getSlot(int slotIndex) {
  int pos = PAGE_SIZE - (slotIndex + 1) * sizeof(SlotEntry);
  return *reinterpret_cast<SlotEntry *>(data + pos);
}

int Page::freeSpace() {
  PageHeader &h = getHeader();
  int slotsEnd = PAGE_SIZE - h.numSlots * sizeof(SlotEntry);
  return slotsEnd - h.freeSpaceOffset;
}

int Page::insertRecord(const Record &rec) {
  int needed = sizeof(Record) + sizeof(SlotEntry);
  if (freeSpace() < needed)
    return -1;

  PageHeader &h = getHeader();

  memcpy(data + h.freeSpaceOffset, &rec, sizeof(Record));

  SlotEntry &slot = getSlot(h.numSlots);
  slot.offset = h.freeSpaceOffset;
  slot.size = sizeof(Record);
  slot.valid = true;

  int slotID = h.numSlots;
  h.freeSpaceOffset += sizeof(Record);
  h.numSlots++;

  return slotID;
}

bool Page::readRecord(int slotID, Record &out) {
  PageHeader &h = getHeader();
  if (slotID < 0 || slotID >= h.numSlots)
    return false;

  SlotEntry &slot = getSlot(slotID);
  if (!slot.valid)
    return false;

  memcpy(&out, data + slot.offset, sizeof(Record));
  return true;
}

bool Page::deleteRecord(int slotID) {
  PageHeader &h = getHeader();
  if (slotID < 0 || slotID >= h.numSlots)
    return false;

  SlotEntry &slot = getSlot(slotID);
  if (!slot.valid)
    return false;

  slot.valid = false;
  return true;
}

int Page::updateRecord(int slotID, const Record &newRec) {
  PageHeader &h = getHeader();
  if (slotID < 0 || slotID >= h.numSlots)
    return -1;

  SlotEntry &slot = getSlot(slotID);
  if (!slot.valid)
    return -1;

  if (slot.size >= (int)sizeof(Record)) {
    memcpy(data + slot.offset, &newRec, sizeof(Record));
    return slotID;
  }

  deleteRecord(slotID);
  return insertRecord(newRec);
}

void Page::compact() {
  PageHeader &h = getHeader();
  char temp[PAGE_SIZE];
  memset(temp, 0, PAGE_SIZE);

  int writeOffset = sizeof(PageHeader);
  int validCount = 0;

  for (int i = 0; i < h.numSlots; i++) {
    SlotEntry &slot = getSlot(i);
    if (slot.valid) {
      memcpy(temp + writeOffset, data + slot.offset, slot.size);
      slot.offset = writeOffset;
      writeOffset += slot.size;
      validCount++;
    }
  }

  memcpy(data + sizeof(PageHeader), temp + sizeof(PageHeader),
         writeOffset - sizeof(PageHeader));
  h.freeSpaceOffset = writeOffset;
}

void Page::printPage() {
  PageHeader &h = getHeader();
  std::cout << "--- Pagina " << h.pageID << " ---" << std::endl;
  std::cout << "numSlots: " << h.numSlots << std::endl;
  std::cout << "freeSpaceOffset: " << h.freeSpaceOffset << std::endl;
  std::cout << "Espacio libre: " << freeSpace() << " bytes" << std::endl;

  for (int i = 0; i < h.numSlots; i++) {
    SlotEntry &slot = getSlot(i);
    std::cout << "  Slot " << i << ": ";
    if (slot.valid) {
      Record rec;
      readRecord(i, rec);
      std::cout << "id=" << rec.id << " nombre=" << rec.nombre;
    } else {
      std::cout << "[borrado]";
    }
    std::cout << std::endl;
  }
}
