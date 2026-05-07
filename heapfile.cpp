#include "heapfile.h"
#include <iostream>

HeapFile::HeapFile(const std::string &fname) {
  filename = fname;
  file.open(fname, std::ios::in | std::ios::out | std::ios::binary);

  if (!file.is_open()) {
    file.open(fname, std::ios::out | std::ios::binary);
    file.close();
    file.open(fname, std::ios::in | std::ios::out | std::ios::binary);
    numPages = 0;
  } else {
    file.seekg(0, std::ios::end);
    numPages = file.tellg() / PAGE_SIZE;
  }
}

HeapFile::~HeapFile() {
  if (file.is_open())
    file.close();
}

int HeapFile::createPage() {
  Page p(numPages);
  writePage(numPages, p);
  numPages++;
  return numPages - 1;
}

bool HeapFile::readPage(int pageID, Page &out) {
  if (pageID < 0 || pageID >= numPages)
    return false;
  file.seekg(pageID * PAGE_SIZE, std::ios::beg);
  file.read(out.data, PAGE_SIZE);
  return file.good();
}

bool HeapFile::writePage(int pageID, const Page &pg) {
  file.seekp(pageID * PAGE_SIZE, std::ios::beg);
  file.write(pg.data, PAGE_SIZE);
  file.flush();
  return file.good();
}

HeapFile::RecordID HeapFile::insert(const Record &rec) {
  for (int i = 0; i < numPages; i++) {
    Page p;
    readPage(i, p);
    int slotID = p.insertRecord(rec);
    if (slotID != -1) {
      writePage(i, p);
      return {i, slotID};
    }
  }

  int newPageID = createPage();
  Page p;
  readPage(newPageID, p);
  int slotID = p.insertRecord(rec);
  writePage(newPageID, p);
  return {newPageID, slotID};
}

bool HeapFile::read(int pageID, int slotID, Record &out) {
  Page p;
  if (!readPage(pageID, p))
    return false;
  return p.readRecord(slotID, out);
}

bool HeapFile::remove(int pageID, int slotID) {
  Page p;
  if (!readPage(pageID, p))
    return false;
  bool result = p.deleteRecord(slotID);
  if (result)
    writePage(pageID, p);
  return result;
}

HeapFile::RecordID HeapFile::update(int pageID, int slotID,
                                    const Record &newRec) {
  Page p;
  if (!readPage(pageID, p))
    return {-1, -1};

  int newSlotID = p.updateRecord(slotID, newRec);
  writePage(pageID, p);
  return {pageID, newSlotID};
}

int HeapFile::getNumPages() { return numPages; }

void HeapFile::printAll() {
  std::cout << "=== HeapFile: " << filename << " ===" << std::endl;
  std::cout << "Total paginas: " << numPages << std::endl;
  for (int i = 0; i < numPages; i++) {
    Page p;
    readPage(i, p);
    p.printPage();
  }
}
