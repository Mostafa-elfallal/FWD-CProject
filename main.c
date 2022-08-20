

#include "Application/app.h"

int main() {
  DB_load();
  appStart();
  DB_store();
}